---
name: Selective Memory
dependsOn:
  - technology_and_tooling.chatbot_development.m4-02-conversation-summarisation
tags: [chatbots, memory, context-management]
learningOutcomes:
  - Score message importance using both LLM-based and heuristic approaches.
  - Implement selective pruning that drops low-importance messages first.
  - Combine importance scoring with summarisation for optimal context management.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    image: https://github.com/OxfordCompetencyCenters.png
    license: CC-BY-4.0
---

Window strategies and summarisation treat all messages as equally important. But in any conversation, some messages carry critical information — account IDs, error codes, deadlines, decisions — while others are filler: "Thanks!", "OK, sure", "Let me think about that." When you need to free up token budget, it makes far more sense to drop the filler first and preserve the critical facts.

## Scoring Message Importance

There are two approaches to importance scoring: using an LLM for nuanced assessment, or using rule-based heuristics for speed and cost.

The LLM-based approach asks the model to rate each message's importance on a scale:

```python
def score_message_importance(message):
    """Use an LLM to rate importance from 1-10."""
    prompt = f"""Rate the importance of this message for maintaining conversation context.
Score from 1-10 where:
1-3: Pleasantries, filler, not critical
4-6: Relevant but not essential
7-10: Critical facts, decisions, or information that must be retained

Message: {message}

Score (just the number):"""

    response = openai.ChatCompletion.create(
        model="gpt-3.5-turbo",
        messages=[{"role": "user", "content": prompt}],
        temperature=0.0,
        max_tokens=5
    )

    try:
        score = int(response.choices[0].message.content.strip())
        return max(1, min(10, score))
    except:
        return 5
```

This is accurate but expensive — it requires an API call for every message. For high-volume production systems, heuristic scoring is more practical:

```python
import re

def score_message_heuristic(message):
    """Rule-based importance scoring — fast and free."""
    score = 5

    # Indicators of high importance
    high_patterns = [
        r'\b(account|id|number|email|phone)\b',
        r'\b(ticket|order|reference)\s*#?\d+',
        r'\b(deadline|urgent|critical|asap)\b',
        r'\b(password|security|privacy)\b',
        r'\b(error|bug|issue|problem)\b',
        r'^\d+[.:]',  # Numbered steps
    ]

    # Indicators of low importance
    low_patterns = [
        r'^(thanks?|thx|ty|thank you)',
        r'^(ok|okay|sure|yes|no)$',
        r'^(hi|hello|hey)',
        r'(lol|haha|hmm)',
    ]

    for pattern in high_patterns:
        if re.search(pattern, message, re.IGNORECASE):
            score += 2

    for pattern in low_patterns:
        if re.search(pattern, message, re.IGNORECASE):
            score -= 2

    if len(message.split()) > 30:
        score += 1  # Longer messages tend to be more substantive

    return max(1, min(10, score))
```

Heuristic scoring works surprisingly well in practice. Messages containing account numbers, error codes, and deadlines almost always score high. Greetings, one-word acknowledgements, and filler expressions score low. The rules can be customised for your domain — a medical chatbot might add high-importance patterns for symptoms and medications, while an e-commerce bot might flag order numbers and shipping addresses.

## Selective Pruning

With importance scores attached to each message, pruning becomes intelligent. Instead of dropping the oldest messages, drop the least important ones:

```python
class SelectiveMemoryChatbot:
    def __init__(self, system_prompt, max_tokens=2000):
        self.system_prompt = system_prompt
        self.max_tokens = max_tokens
        self.conversation = []
        self.encoding = tiktoken.encoding_for_model("gpt-3.5-turbo")

    def chat(self, user_message):
        importance = score_message_heuristic(user_message)

        self.conversation.append({
            "role": "user",
            "content": user_message,
            "importance": importance
        })

        self._prune_by_importance()

        # Build messages for the API (without the importance field)
        messages = [{"role": "system", "content": self.system_prompt}]
        messages += [{"role": msg["role"], "content": msg["content"]}
                     for msg in self.conversation]

        response = openai.ChatCompletion.create(
            model="gpt-3.5-turbo",
            messages=messages
        )

        assistant_message = response.choices[0].message.content
        assistant_importance = score_message_heuristic(assistant_message)

        self.conversation.append({
            "role": "assistant",
            "content": assistant_message,
            "importance": assistant_importance
        })

        self._prune_by_importance()
        return assistant_message

    def _prune_by_importance(self):
        """Remove the least important messages to stay under the token budget."""
        while self._count_tokens() > self.max_tokens:
            if len(self.conversation) <= 2:
                break

            # Find the least important message, excluding the most recent pair
            prunable = self.conversation[:-2]
            if not prunable:
                break

            least_important = min(prunable, key=lambda x: x["importance"])
            idx = self.conversation.index(least_important)

            # Remove the message and its pair (user + assistant go together)
            if least_important["role"] == "user" and idx + 1 < len(self.conversation):
                self.conversation.pop(idx)
                self.conversation.pop(idx)  # The assistant reply shifts down
            elif least_important["role"] == "assistant" and idx > 0:
                self.conversation.pop(idx)
                self.conversation.pop(idx - 1)  # The preceding user message
            else:
                self.conversation.pop(idx)

    def _count_tokens(self):
        return sum(len(self.encoding.encode(msg["content"])) for msg in self.conversation)
```

The effect is dramatic. In a 30-turn customer support conversation, the messages "Thanks!", "OK", "Sure, go ahead", and "Hmm, let me think" are dropped first. Meanwhile, "My account ID is ABC123", "I'm getting error code 500", and "I need this resolved by Friday" are preserved even as the conversation grows far beyond the token budget.

## Combining Strategies

In practice, the best results come from combining selective memory with summarisation. Use importance scoring to decide what to keep in full and what to summarise. High-importance messages stay verbatim in the context. Low-importance messages get summarised or dropped entirely. Medium-importance messages get included in summaries.

This gives you the best of both worlds: critical facts are always available in their exact form, while the general flow and context of the conversation is preserved through summaries. The token budget is used efficiently because it's allocated to the information that matters most.

The architecture looks like this:

1. Score every incoming message for importance.
2. Always keep the most recent 3–5 turns in full (regardless of importance — they're the active conversation).
3. For older messages, keep high-importance messages verbatim and summarise the rest.
4. When the token budget is tight, drop the low-importance messages entirely before touching the summaries or high-importance messages.

This is the approach used by production chatbot systems that need to handle extended conversations without losing critical context. In the next section, we'll add another layer: retrieval-based memory that lets the bot access information from any point in the conversation history, even across sessions.

## Hands-on practical

Work through the accompanying notebook: [`module_4/03-selective-memory.ipynb`](https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules/blob/main/module_4/03-selective-memory.ipynb). It walks through heuristic importance scoring, an LLM-based scorer, and a `SelectiveMemoryChatbot` that prunes the lowest-importance messages first.
