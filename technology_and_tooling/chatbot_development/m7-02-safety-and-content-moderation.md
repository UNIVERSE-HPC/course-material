---
name: Safety and Content Moderation
dependsOn:
  - technology_and_tooling.chatbot_development.m7-01-production-prompt-engineering
tags: [chatbots, safety, moderation, pii, privacy, optional]
learningOutcomes:
  - Implement automated content moderation using the OpenAI Moderation API.
  - Detect and redact personally identifiable information (PII) from chatbot interactions.
  - Build GDPR-compliant data handling for user conversations and profiles.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    image: https://github.com/OxfordCompetencyCenters.png
    license: CC-BY-4.0
---

A production chatbot must be safe. Users will send inappropriate content — sometimes deliberately, sometimes accidentally. The bot itself can generate harmful output if prompted in certain ways or if the underlying model produces unexpected responses. And users will share sensitive personal information (email addresses, phone numbers, credit card details) that needs to be handled carefully. Content moderation, PII protection, and privacy compliance are not optional features. They are requirements.

## Automated Content Moderation

OpenAI provides a free Moderation API that checks text for categories of harmful content: hate speech, harassment, self-harm, sexual content, and violence. The API is fast (roughly 100ms per call) and should be applied to both user inputs and bot outputs — checking inputs prevents processing harmful requests, and checking outputs catches cases where the model generates something inappropriate despite safe input.

```python
import openai

class ContentModerator:
    CATEGORIES = [
        "hate", "hate/threatening",
        "harassment", "harassment/threatening",
        "self-harm", "self-harm/intent", "self-harm/instructions",
        "sexual", "sexual/minors",
        "violence", "violence/graphic"
    ]

    def __init__(self, threshold=0.5):
        self.threshold = threshold

    def check_content(self, text):
        """Check if content violates policies."""
        response = openai.Moderation.create(input=text)
        result = response["results"][0]

        violations = []
        for category, flagged in result["categories"].items():
            if flagged:
                score = result["category_scores"][category]
                if score > self.threshold:
                    violations.append({
                        "category": category,
                        "score": score
                    })

        return {
            "flagged": result["flagged"],
            "violations": violations,
            "safe": not result["flagged"]
        }
```

Integrating the moderator into the chat endpoint creates a defence-in-depth approach — check the input, then check the output:

```python
moderator = ContentModerator()

@app.post("/chat")
async def chat(request: ChatRequest):
    # Check user input
    user_check = moderator.check_content(request.message)

    if not user_check["safe"]:
        logger.warning("unsafe_user_input", extra={
            "user_id": request.user_id,
            "violations": user_check["violations"]
        })
        return {
            "response": "I can't process that request. Please keep our conversation appropriate.",
            "flagged": True
        }

    # Generate response
    response = llm.generate(...)

    # Check bot output
    bot_check = moderator.check_content(response)

    if not bot_check["safe"]:
        logger.error("unsafe_bot_output", extra={
            "user_id": request.user_id,
            "violations": bot_check["violations"],
            "prompt": request.message
        })
        return {
            "response": "I apologize, I can't provide that information. How else can I help?",
            "flagged": True
        }

    return {"response": response, "flagged": False}
```

Beyond the general-purpose moderation API, most applications need custom moderation rules specific to their domain. A corporate chatbot might block mentions of competitors. A financial services bot might flag investment advice. These rules use pattern matching:

```python
import re

class CustomModerator:
    def __init__(self):
        self.blocked_patterns = [
            r'\b(competitor_name_1|competitor_name_2)\b',
            r'\b(confidential|internal_only)\b',
        ]

        self.pii_patterns = {
            "email": r'\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Z|a-z]{2,}\b',
            "ssn": r'\b\d{3}-\d{2}-\d{4}\b',
            "credit_card": r'\b\d{4}[- ]?\d{4}[- ]?\d{4}[- ]?\d{4}\b'
        }

    def check_custom_rules(self, text):
        """Check custom moderation rules."""
        violations = []

        for pattern in self.blocked_patterns:
            if re.search(pattern, text, re.IGNORECASE):
                violations.append({"type": "blocklist", "pattern": pattern})

        for pii_type, pattern in self.pii_patterns.items():
            if re.search(pattern, text):
                violations.append({"type": "pii", "pii_type": pii_type})

        return {"safe": len(violations) == 0, "violations": violations}
```

Log all moderation violations for analysis. Patterns in the violations data can reveal attack attempts, gaps in your moderation rules, or topics that need better handling.

## PII Detection and Redaction

Users frequently share sensitive personal information in chatbot conversations — email addresses, phone numbers, credit card numbers, social security numbers. This data needs to be detected and either redacted before processing or handled with special care.

Microsoft's Presidio library provides production-grade PII detection:

```python
from presidio_analyzer import AnalyzerEngine
from presidio_anonymizer import AnonymizerEngine

class PIIHandler:
    def __init__(self):
        self.analyzer = AnalyzerEngine()
        self.anonymizer = AnonymizerEngine()

    def detect_pii(self, text):
        """Detect PII in text."""
        results = self.analyzer.analyze(
            text=text,
            language="en",
            entities=[
                "PHONE_NUMBER", "EMAIL_ADDRESS",
                "CREDIT_CARD", "IBAN_CODE",
                "US_SSN", "US_PASSPORT",
                "PERSON", "LOCATION"
            ]
        )
        return results

    def redact_pii(self, text):
        """Redact PII from text, replacing with placeholders."""
        results = self.detect_pii(text)
        anonymized = self.anonymizer.anonymize(text=text, analyzer_results=results)
        return anonymized.text

    def mask_sensitive_data(self, text):
        """Mask common sensitive patterns using regex."""
        text = re.sub(
            r'\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Z|a-z]{2,}\b',
            '[EMAIL]', text)
        text = re.sub(r'\b\d{3}[-.]?\d{3}[-.]?\d{4}\b', '[PHONE]', text)
        text = re.sub(r'\b\d{4}[- ]?\d{4}[- ]?\d{4}[- ]?\d{4}\b', '[CREDIT_CARD]', text)
        text = re.sub(r'\b\d{3}-\d{2}-\d{4}\b', '[SSN]', text)
        return text
```

When PII is detected in user input, you have two options: reject the request and ask the user not to share sensitive information, or redact the PII and continue processing with the sanitised text. The choice depends on your application. A customer support bot might redact and continue (the user might include their email naturally). A general chatbot might warn the user.

Critically, PII must be masked before logging. You do not want credit card numbers appearing in your log aggregation system:

```python
pii_handler = PIIHandler()

@app.post("/chat")
async def chat(request: ChatRequest):
    pii_detected = pii_handler.detect_pii(request.message)

    if pii_detected:
        logger.warning("pii_detected_in_input", extra={
            "user_id": request.user_id,
            "pii_types": [result.entity_type for result in pii_detected]
        })

    # Mask before logging
    safe_log_message = pii_handler.mask_sensitive_data(request.message)
    logger.info("chat_request", extra={"message": safe_log_message})

    return response
```

## GDPR Compliance

If your chatbot serves users in the European Union, GDPR compliance is a legal requirement. Two key rights that affect chatbot systems are the right to access (users can request all data you hold about them) and the right to erasure (users can request deletion of all their data):

```python
class GDPRCompliance:
    def __init__(self, db):
        self.db = db

    def user_data_export(self, user_id):
        """Export all data for a user (right to access)."""
        return {
            "conversations": self.db.get_user_conversations(user_id),
            "profile": self.db.get_user_profile(user_id),
            "logs": self.db.get_user_logs(user_id)
        }

    def delete_user_data(self, user_id):
        """Delete all user data (right to be forgotten)."""
        self.db.delete_user_conversations(user_id)
        self.db.delete_user_profile(user_id)
        self.db.anonymize_user_logs(user_id)
        logger.info("user_data_deleted", extra={"user_id": user_id})
```

Implement these as API endpoints that users (or your support team) can invoke. For the deletion endpoint, note that logs are anonymised rather than deleted — you may need to retain anonymised logs for operational purposes, but they must not be linkable back to the individual user. Also define and communicate a data retention policy: how long do you store conversations? When are they automatically purged? For healthcare applications (HIPAA compliance), the requirements are even stricter and may require a Business Associate Agreement with your cloud provider.
