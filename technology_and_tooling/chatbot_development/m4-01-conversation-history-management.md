---
name: Conversation History Management
dependsOn:
  - technology_and_tooling.chatbot_development.m3-05-evaluating-and-optimising-retrieval
tags: [chatbots, memory, context-management]
learningOutcomes:
  - Distinguish between short-term and long-term memory in conversational AI.
  - Explain why unbounded conversation history is unsustainable in production.
  - Implement fixed-window and token-based window management strategies.
  - Understand the trade-offs between simplicity and context preservation.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    image: https://github.com/OxfordCompetencyCenters.png
    license: CC-BY-4.0
---

In Module 1, you built a stateful chatbot that appends every message to a growing history list and sends the whole thing with every API call. That approach works for short conversations, but it's unsustainable. Real-world chatbot sessions — customer support troubleshooting, multi-day tutoring, personal assistant interactions — can span dozens or hundreds of turns. Without intelligent management, the conversation history grows until it exceeds the context window, at which point the system either crashes or silently drops messages. This module introduces the strategies that production chatbots use to handle conversations of any length.

## Short-Term vs Long-Term Memory

It helps to think about chatbot memory in the same terms we use for human memory. **Short-term memory** (or working memory) is the information that's immediately available — the last few turns of conversation, the current topic, what the user just said. In chatbot terms, this is whatever fits in the LLM's context window. It's fast to access (it's already in the prompt) but limited in capacity.

**Long-term memory** is everything else — past conversation sessions, user preferences, decisions made weeks ago. In chatbot terms, this information lives in a database or vector store and must be explicitly retrieved when it's needed. It's unlimited in capacity but requires a retrieval step to access.

Consider a customer support scenario. Short-term memory lets the bot follow the current troubleshooting thread: the user described an error, the bot asked for details, the user provided a stack trace. Long-term memory lets the bot recall that this same user had a similar issue last month and the fix was to update a configuration file.

The challenge is bridging these two layers. Short-term memory fits naturally in the context window, but long-term memory doesn't. We need strategies to decide what goes into the context window on each turn, how to compress older information, and when to retrieve from long-term storage. The rest of this module builds those strategies.

## The Context Window Problem

To see why this matters, let's trace the token usage of a customer support conversation using GPT-3.5-turbo with an 8,000-token context window.

```
Turn 1:  System prompt (200) + User (50) + Response (150)    = 400 tokens
Turn 5:  System + History (1,500) + New turn (200)           = 1,700 tokens
Turn 10: System + History (4,000) + New turn (200)           = 4,200 tokens
Turn 20: System + History (9,500) + New turn (200)           = 9,700 tokens
```

By turn 20, the conversation has blown past the 8,000-token limit. Even with GPT-4-turbo's 128K context window, a particularly detailed conversation can still hit limits — and you're paying for every token in every request. A 50-turn conversation where you send the full history on every call is sending roughly 25,000 tokens per request by the end, which is expensive and slow.

The real cost isn't just financial. When the context window fills up, the bot can't process new messages without dropping old ones. If it drops them blindly, critical context is lost. A user who established important information at the start of the conversation — "I'm on version 2.3, running Windows, with a premium account" — finds that the bot has forgotten all of it by turn 15.

## Fixed-Window Management

The simplest strategy is to keep only the most recent N messages. Old messages are dropped entirely as new ones arrive.

```python
class FixedWindowChatbot:
    def __init__(self, system_prompt, window_size=10):
        self.system_prompt = system_prompt
        self.window_size = window_size
        self.conversation_history = []

    def chat(self, user_message):
        self.conversation_history.append({
            "role": "user",
            "content": user_message
        })

        # Keep only the most recent N messages
        if len(self.conversation_history) > self.window_size:
            self.conversation_history = self.conversation_history[-self.window_size:]

        # Build the prompt
        messages = [
            {"role": "system", "content": self.system_prompt}
        ] + self.conversation_history

        response = openai.ChatCompletion.create(
            model="gpt-3.5-turbo",
            messages=messages
        )

        assistant_message = response.choices[0].message.content
        self.conversation_history.append({
            "role": "assistant",
            "content": assistant_message
        })

        return assistant_message
```

This is simple and guarantees you'll never exceed the context window. But it has a significant flaw: the forgetting is abrupt and indiscriminate. Message 11 disappears the moment message 12 arrives, regardless of whether message 11 contained critical information. A user's account ID, stated in message 3, is gone by the time you reach message 14.

## Token-Based Window Management

A refinement is to manage the window by token count rather than message count. This accounts for the fact that messages vary in length — a one-word acknowledgement costs fewer tokens than a detailed error description.

```python
import tiktoken

class TokenWindowChatbot:
    def __init__(self, system_prompt, max_tokens=2000):
        self.system_prompt = system_prompt
        self.max_tokens = max_tokens
        self.conversation_history = []
        self.encoding = tiktoken.encoding_for_model("gpt-3.5-turbo")

    def count_tokens(self, messages):
        total = 0
        for message in messages:
            total += len(self.encoding.encode(message["content"]))
        return total

    def chat(self, user_message):
        self.conversation_history.append({
            "role": "user",
            "content": user_message
        })

        # Prune oldest message pairs until under budget
        while self.count_tokens(self.conversation_history) > self.max_tokens:
            if len(self.conversation_history) > 2:
                self.conversation_history.pop(0)
                self.conversation_history.pop(0)
            else:
                break

        messages = [
            {"role": "system", "content": self.system_prompt}
        ] + self.conversation_history

        response = openai.ChatCompletion.create(
            model="gpt-3.5-turbo",
            messages=messages
        )

        assistant_message = response.choices[0].message.content
        self.conversation_history.append({
            "role": "assistant",
            "content": assistant_message
        })

        return assistant_message
```

Token-based pruning adapts to message length — a conversation with short messages keeps more turns in memory than one with long messages. But it still suffers from the same fundamental problem: it treats all messages as equally important. A one-word "Thanks!" consumes a trivial number of tokens but occupies a slot that could be used by critical context.

Both window strategies are useful as building blocks. In the following sections, we'll layer smarter techniques on top — summarisation, selective memory, and retrieval-based memory — that preserve important information even as conversations grow beyond the window size.

## Hands-on practical

Work through the accompanying notebook: [`module_4/01-conversation-history-management.ipynb`](https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules/blob/main/module_4/01-conversation-history-management.ipynb). It walks through simulating an unbounded-history chatbot, a fixed-window variant, and a token-window variant — with an account-number recall test that shows how each one forgets.
