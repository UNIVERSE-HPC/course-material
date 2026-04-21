---
name: Conversation Summarisation
dependsOn:
  - technology_and_tooling.chatbot_development.m4-01-conversation-history-management
tags: [chatbots, memory, summarisation]
learningOutcomes:
  - Implement rolling summarisation to compress older conversation turns.
  - Build a hierarchical memory system for very long conversations.
  - Understand the trade-offs between summarisation fidelity and token savings.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    license: CC-BY-4.0
---

Window strategies drop old messages entirely. Summarisation offers a better alternative: instead of forgetting older parts of the conversation, compress them into a concise summary that captures the key points. This preserves context at a fraction of the token cost.

## Rolling Summarisation

The idea is straightforward. Keep the most recent messages in full detail — these are the active part of the conversation. When the history grows beyond a threshold, take the older messages, generate a summary, and replace them with that summary. The summary lives at the beginning of the context, giving the LLM awareness of what was discussed earlier without the full verbatim history.

Consider a 20-turn customer support conversation about email notifications. The full history might consume 3,000 tokens. A summary of the first 14 turns might read: "User is setting up email notifications on version 2.3. After trying Settings > Notifications (not available in 2.3), we directed them to Preferences > Email. User successfully configured notifications but had a question about frequency settings." That's roughly 200 tokens — a 10–15x compression.

Here's a complete implementation:

```python
class SummarizingChatbot:
    def __init__(self, system_prompt, summary_threshold=10):
        self.system_prompt = system_prompt
        self.conversation_summary = ""
        self.recent_messages = []
        self.summary_threshold = summary_threshold

    def summarize_conversation(self, messages):
        """Generate a concise summary of a set of messages."""
        conversation_text = "\n".join([
            f"{msg['role']}: {msg['content']}"
            for msg in messages
        ])

        summary_prompt = f"""Summarize the key points of this conversation concisely.
Focus on:
- Main topics discussed
- Important facts or decisions
- Current state of the conversation

Conversation:
{conversation_text}

Summary (2-3 sentences):"""

        response = openai.ChatCompletion.create(
            model="gpt-3.5-turbo",
            messages=[{"role": "user", "content": summary_prompt}],
            temperature=0.3,
            max_tokens=150
        )

        return response.choices[0].message.content

    def chat(self, user_message):
        self.recent_messages.append({
            "role": "user",
            "content": user_message
        })

        # When recent messages exceed the threshold, summarise the older half
        if len(self.recent_messages) > self.summary_threshold * 2:
            to_summarize = self.recent_messages[:self.summary_threshold]
            self.recent_messages = self.recent_messages[self.summary_threshold:]

            new_summary = self.summarize_conversation(to_summarize)

            if self.conversation_summary:
                self.conversation_summary += f"\n\n{new_summary}"
            else:
                self.conversation_summary = new_summary

        # Build the prompt
        messages = [{"role": "system", "content": self.system_prompt}]

        if self.conversation_summary:
            messages.append({
                "role": "system",
                "content": f"Conversation summary so far:\n{self.conversation_summary}"
            })

        messages.extend(self.recent_messages)

        response = openai.ChatCompletion.create(
            model="gpt-3.5-turbo",
            messages=messages
        )

        assistant_message = response.choices[0].message.content
        self.recent_messages.append({
            "role": "assistant",
            "content": assistant_message
        })

        return assistant_message
```

The `summary_threshold` controls how often summarisation happens. With a threshold of 10, every 10 messages the oldest messages are compressed into a summary. The summary accumulates as the conversation continues, but it grows slowly — a few hundred tokens per cycle — compared to the thousands of tokens that the full history would consume.

Each summarisation costs one additional API call, but it pays for itself quickly. Without summarisation, a 30-turn conversation might send 5,000+ tokens per request. With summarisation, the total context stays around 2,000–2,500 tokens: roughly 600 tokens of accumulated summary plus 1,500 tokens of recent messages. That's less than half the cost on every subsequent request.

The trade-off is lossy compression. The summary captures key facts and decisions but loses the exact wording, nuances, and the back-and-forth of the conversation. If the user says "Go back to what you said third — the one about environment variables", the summarised context may not retain enough detail to know what "third" refers to. For most practical purposes, this level of detail loss is acceptable. For use cases that require verbatim recall, combine summarisation with retrieval-based memory (covered in Section 4).

## Hierarchical Summarisation

In very long conversations (100+ turns), even summaries accumulate. A flat summary that grows every 10 turns will eventually consume thousands of tokens itself. Hierarchical summarisation solves this by applying multiple levels of compression, similar to how a computer's memory hierarchy works.

```
Level 0 (Current):  Last 5 turns in full detail        ~500 tokens
Level 1 (Recent):   Summary of turns 6–20              ~200 tokens
Level 2 (Medium):   Summary of turns 21–50             ~100 tokens
Level 3 (Distant):  Summary of turns 51–100            ~50 tokens
```

As information ages, it gets progressively more compressed. The most recent turns are stored verbatim. Slightly older turns are summarised. Even older turns have their summaries summarised — a "summary of summaries". The earliest parts of the conversation are reduced to just the essential facts.

```python
class HierarchicalMemory:
    def __init__(self):
        self.current_window = []     # Last 5 turns, full detail
        self.level1_summary = ""     # Turns 6–20
        self.level2_summary = ""     # Turns 21–50
        self.level3_summary = ""     # Turns 51+

    def add_turn(self, user_msg, assistant_msg):
        self.current_window.extend([
            {"role": "user", "content": user_msg},
            {"role": "assistant", "content": assistant_msg}
        ])

        # Promote from current window to level 1
        if len(self.current_window) > 10:
            to_summarize = self.current_window[:6]
            self.current_window = self.current_window[6:]

            summary = self.summarize(to_summarize)

            # If level 1 is getting long, promote to level 2
            if len(self.level1_summary) > 500:
                self.level2_summary = self.summarize_summaries(
                    [self.level2_summary, self.level1_summary]
                )
                self.level1_summary = summary
            else:
                self.level1_summary += f"\n{summary}"

    def build_context(self):
        """Assemble the full context from all memory layers."""
        context_parts = []

        if self.level3_summary:
            context_parts.append(f"Early conversation: {self.level3_summary}")
        if self.level2_summary:
            context_parts.append(f"Previous discussion: {self.level2_summary}")
        if self.level1_summary:
            context_parts.append(f"Recent context: {self.level1_summary}")

        context_parts.append("Current conversation:")
        context_parts.extend([
            f"{msg['role']}: {msg['content']}"
            for msg in self.current_window
        ])

        return "\n\n".join(context_parts)
```

To see the impact, compare token usage at turn 100 across different strategies:

```
No memory management:        Exceeds context limit at turn ~20
Fixed window (10 messages):  2,000 tokens — but lost everything before turn 90
Flat summarisation:          3,500 tokens — growing
Hierarchical summarisation:  1,800 tokens — bounded, retains all context
```

The hierarchical approach keeps token usage bounded regardless of conversation length. A 100-turn conversation might use: 500 tokens for the current window, 200 tokens for level 1, 100 tokens for level 2, and 50 tokens for level 3 — roughly 850 tokens of context total. The bot can still reference information from turn 1, just in highly condensed form.

The right number of hierarchy levels and the promotion thresholds depend on your use case. For most applications, two or three levels are sufficient. Customer support conversations rarely exceed 50 turns, so flat summarisation with a threshold of 10 works well. Tutoring sessions that span hours might benefit from a three-level hierarchy. Personal assistants that operate over weeks or months might need four levels plus retrieval-based memory.

::::challenge{id=m4-02-compare title="Implement and Compare Summarisation Strategies"}

Build both a flat summarising chatbot and a hierarchical memory system, then test them with a 30-turn conversation.

```python
# Flat summarisation
bot_flat = SummarizingChatbot(system_prompt="You are a helpful assistant.", summary_threshold=10)

# Run 30 turns
for i in range(30):
    response = bot_flat.chat(f"This is turn {i+1}. Tell me about topic {i+1}.")
    tokens = bot_flat.get_token_count()
    print(f"Turn {i+1}: {tokens} tokens")
```

Test recall by asking the bot about information from early turns. After 30 turns, does the bot remember what was discussed at turn 5? At turn 1? How does the quality of recall differ between flat summarisation and the hierarchical approach?

A particularly revealing test: at turn 1, tell the bot "My account ID is ABC123." Then, at turn 25, ask "What is my account ID?" Does the summary preserve this specific fact? If not, how could you ensure it does? (The next section on selective memory addresses this question directly.)
::::

## Hands-on practical

Work through the accompanying notebook: [`module_4/02-conversation-summarisation.ipynb`](https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules/blob/main/module_4/02-conversation-summarisation.ipynb). It walks through summarising a block of turns, building a rolling-summary chatbot, and running the account-number recall test.
