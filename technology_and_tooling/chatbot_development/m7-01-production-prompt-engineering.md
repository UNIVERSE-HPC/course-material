---
name: Production Prompt Engineering
dependsOn:
  - technology_and_tooling.chatbot_development.m6-05-ab-testing-and-experimentation
tags: [chatbots, prompt-engineering, production, optional]
learningOutcomes:
  - Apply structured prompt design patterns for production chatbots.
  - Handle edge cases including out-of-scope queries, ambiguous input, and adversarial messages.
  - Version-control and systematically test prompts before deployment.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    license: CC-BY-4.0
---

The system prompt is the single most important piece of configuration in your chatbot. It determines the bot's personality, capabilities, boundaries, and behaviour in edge cases. A well-crafted prompt handles the easy cases and the hard ones — off-topic questions, ambiguous input, hostile users, requests for information the bot doesn't have. A poorly crafted prompt produces inconsistent behaviour that erodes user trust.

Production prompt engineering is systematic. You design prompts with explicit structure, test them against a suite of edge cases, version them like code, and deploy changes through A/B tests. Small wording changes can have significant behavioural impact, so every revision should be deliberate and measured.

## System Prompt Design Patterns

Three patterns cover the majority of production chatbot prompts.

**Pattern 1: Role + Task + Constraints** is the most common. It defines who the bot is, what it can do, what it cannot do, and how it should behave:

```python
system_prompt = """You are an expert customer support agent for TechCorp,
a B2B SaaS company specializing in project management software.

Your capabilities:
- Answer questions about product features and pricing
- Troubleshoot common technical issues
- Guide users through setup and configuration
- Access product documentation via search

Your constraints:
- Never make up information about features that don't exist
- Never share sensitive user data
- Cannot process refunds (escalate to billing team)
- Cannot modify user accounts directly

Guidelines:
1. Always be professional, patient, and empathetic
2. Ask clarifying questions before providing solutions
3. Cite documentation sources when available
4. If unsure, say so and offer to escalate
5. Keep responses concise (under 150 words unless detailed explanation needed)

When troubleshooting:
1. Acknowledge the issue
2. Ask diagnostic questions
3. Provide step-by-step solutions
4. Verify resolution
"""
```

The explicit constraints are critical. Without "never make up information about features that don't exist," the bot will confidently describe non-existent features. Without "cannot process refunds," it might promise a refund it can't deliver. Every constraint prevents a class of failures you've either observed or can anticipate.

**Pattern 2: Few-Shot Examples** teaches the model through demonstration rather than description. This is more effective for subtle behavioural nuances that are hard to specify in words:

```
You are a code review assistant.

Examples:

User: "Is this code good? x = x + 1"
Assistant: "This code works but can be simplified to `x += 1` which is more Pythonic."

User: "Should I use a list or set here?"
Assistant: "It depends on your use case:
- Use a list if you need ordered elements or duplicates
- Use a set if you need fast lookups and unique elements only
What's your specific requirement?"
```

The examples demonstrate the desired tone, level of detail, and conversational style more precisely than any description could.

**Pattern 3: Chain-of-Thought** improves reasoning quality for complex tasks by instructing the model to think step-by-step before answering:

```
You are a math tutor. For each problem:

1. First, think through the problem step-by-step
2. Show your reasoning process
3. Then provide the final answer

Example:
Problem: "What is 15% of 80?"

Thinking:
- 15% means 15/100 = 0.15
- We multiply: 0.15 x 80
- 0.15 x 80 = 12

Answer: 15% of 80 is 12.
```

This pattern is particularly useful for RAG chatbots that need to synthesise information from multiple retrieved documents.

## Handling Edge Cases

Real users do not behave like test scripts. They ask off-topic questions, provide vague input, send hostile messages, ask multiple questions at once, and test the boundaries of the system. Your prompt must handle all of these gracefully.

**Out-of-scope questions** are the most common edge case. A customer support bot will be asked about the weather, sports, and politics. The prompt should redirect politely: "I'm a TechCorp support assistant and can't check weather information. I can help with questions about our project management software, troubleshooting, or account issues. What can I assist you with today?"

**Ambiguous queries** like "it's not working" need clarification rather than guessing. The prompt should instruct the bot to ask diagnostic questions: "What specifically isn't working? What were you trying to do? What error message did you see?"

**Multi-intent queries** like "What's your pricing and can you reset my password?" should be addressed systematically, handling each question separately.

**Hallucination prevention** is especially important for RAG chatbots. The prompt should include an explicit instruction: "CRITICAL: Only provide information based on the provided context documents. If you don't know something or the context doesn't contain the answer, explicitly say 'I don't have that information' rather than guessing."

You can build these handlers programmatically into your prompt:

```python
def build_robust_prompt(base_prompt, edge_case_handlers):
    """Build prompt with edge case handling."""
    prompt = base_prompt + "\n\nEdge Case Handling:\n"

    for case, handler in edge_case_handlers.items():
        prompt += f"\n- {case}: {handler}"

    return prompt

edge_cases = {
    "out_of_scope": "Politely redirect to in-scope topics",
    "ambiguous": "Ask 2-3 clarifying questions",
    "multiple_questions": "Address each question separately",
    "hostile_user": "Remain professional, offer to escalate if needed"
}

system_prompt = build_robust_prompt(base_prompt, edge_cases)
```

## Prompt Versioning and Testing

Prompts should be treated like code: stored in version-controlled files, tested against a suite of cases, and deployed through a controlled rollout process.

```python
SYSTEM_PROMPTS = {
    "v1.0": """You are a helpful assistant.""",
    "v1.1": """You are a helpful assistant who provides clear, concise answers.""",
    "v2.0": """You are an expert customer support agent...[detailed prompt]""",
    "v2.1": """You are an expert customer support agent...[with edge case handling]"""
}

CURRENT_VERSION = "v2.1"

def get_system_prompt(version=None):
    """Get system prompt by version."""
    version = version or CURRENT_VERSION
    return SYSTEM_PROMPTS[version]
```

A prompt testing framework validates expected behaviour across edge cases before deployment:

```python
class PromptTester:
    def __init__(self, test_cases):
        self.test_cases = test_cases

    def test_prompt(self, system_prompt):
        """Test a prompt against predefined cases."""
        results = []

        for case in self.test_cases:
            response = llm.generate(
                system_prompt=system_prompt,
                user_message=case["input"]
            )

            passed = case["validator"](response)

            results.append({
                "input": case["input"],
                "response": response,
                "expected": case["description"],
                "passed": passed
            })

        pass_rate = sum(r["passed"] for r in results) / len(results)
        return {"pass_rate": pass_rate, "results": results}

test_cases = [
    {
        "input": "What's the weather?",
        "description": "Should redirect out-of-scope questions",
        "validator": lambda r: "can't help with weather" in r.lower()
    },
    {
        "input": "How do I reset password?",
        "description": "Should provide relevant answer",
        "validator": lambda r: "password" in r.lower() and len(r) > 50
    },
    {
        "input": "asdfghjkl",
        "description": "Should handle gibberish gracefully",
        "validator": lambda r: "don't understand" in r.lower() or "clarify" in r.lower()
    }
]

tester = PromptTester(test_cases)
v2_results = tester.test_prompt(get_system_prompt("v2.0"))
v2_1_results = tester.test_prompt(get_system_prompt("v2.1"))

print(f"v2.0 pass rate: {v2_results['pass_rate']:.0%}")
print(f"v2.1 pass rate: {v2_1_results['pass_rate']:.0%}")
```

The workflow is: write a new prompt version, run it through the test suite, compare pass rates against the current version, A/B test in production with a small percentage of traffic (10–20%), and roll out to 100% if metrics improve. If a deployed prompt causes issues, roll back to the previous version immediately — this is why versioning matters.
