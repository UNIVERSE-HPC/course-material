---
name: Building Stateless and Stateful Chatbots
dependsOn:
  - technology_and_tooling.chatbot_development.m1-03-api-integration-basics
tags: [chatbots, python]
learningOutcomes:
  - Build a stateless chatbot for FAQ-style interactions.
  - Build a stateful chatbot that maintains conversation history.
  - Implement token budget management to control costs and stay within context limits.
  - Craft effective system prompts for different use cases.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    image: https://github.com/OxfordCompetencyCenters.png
    license: CC-BY-4.0
---

In the previous section you made individual API calls. Now we'll wrap that capability into reusable chatbot classes — first a stateless one, then a stateful one — and tackle the practical challenges that arise when conversations get long. We'll also look closely at system prompts, which are your primary tool for shaping a chatbot's behaviour.

## Building a Stateless Chatbot

A stateless chatbot is the simplest architecture: each user message is handled independently, with no memory of previous exchanges. The implementation is straightforward — store a system prompt, and on every call, send just the system prompt and the user's current message to the API.

```python
import openai
import os

openai.api_key = os.getenv("OPENAI_API_KEY")

class StatelessChatbot:
    def __init__(self, system_prompt):
        self.system_prompt = system_prompt

    def chat(self, user_message):
        """Process a single message without history."""
        response = openai.ChatCompletion.create(
            model="gpt-3.5-turbo",
            messages=[
                {"role": "system", "content": self.system_prompt},
                {"role": "user", "content": user_message}
            ],
            temperature=0.7
        )
        return response.choices[0].message.content
```

You can use it like this:

```python
bot = StatelessChatbot("You are a Python programming expert.")
print(bot.chat("What is a list comprehension?"))
print(bot.chat("Can you show an example?"))
```

Try running this and pay attention to what happens with the second message. The bot has no idea what "an example" refers to, because it has no memory of the first exchange. It might give you an example of something completely unrelated, or ask you to clarify. This is the fundamental limitation of stateless design: every message exists in isolation.

For certain use cases, that's perfectly fine. A FAQ bot for a restaurant ("What are your hours?", "Do you take reservations?", "Where are you located?") works well as a stateless system because each question is self-contained. But the moment users need to have a conversation — asking follow-up questions, referring back to earlier context, building on previous answers — you need state.

## Building a Stateful Chatbot

A stateful chatbot solves the memory problem by maintaining a list of all messages exchanged so far and including that list in every API call. The model sees the entire conversation history and can respond in context.

```python
class StatefulChatbot:
    def __init__(self, system_prompt):
        self.system_prompt = system_prompt
        self.conversation_history = [
            {"role": "system", "content": system_prompt}
        ]

    def chat(self, user_message):
        """Process message with full conversation history."""
        self.conversation_history.append({
            "role": "user",
            "content": user_message
        })

        response = openai.ChatCompletion.create(
            model="gpt-3.5-turbo",
            messages=self.conversation_history,
            temperature=0.7
        )

        assistant_message = response.choices[0].message.content
        self.conversation_history.append({
            "role": "assistant",
            "content": assistant_message
        })

        return assistant_message

    def get_history(self):
        """Return conversation history, excluding the system prompt."""
        return self.conversation_history[1:]

    def reset(self):
        """Clear conversation history and start fresh."""
        self.conversation_history = [
            {"role": "system", "content": self.system_prompt}
        ]
```

The key difference is the `conversation_history` list. It starts with just the system prompt. Each time the user sends a message, we append it to the list. Each time the model responds, we append that too. When we make the next API call, we send the entire list, so the model can see everything that's been said.

```python
bot = StatefulChatbot("You are a Python programming expert.")
print(bot.chat("What is a list comprehension?"))
print(bot.chat("Can you show an example?"))  # Now it knows we're talking about list comprehensions
print(f"History: {len(bot.get_history())} messages")
```

This time, the second message works as expected. The model sees the full conversation, understands that "an example" refers to list comprehensions, and provides a relevant code sample. Follow-up questions like "What if I want to filter?", "Is that faster than a for loop?", and "Can you make it more complex?" all work naturally because the context is preserved.

The `reset()` method clears the history, which is useful when a user wants to start a new topic or when you want to free up token budget. In a web application, you'd typically create a new `StatefulChatbot` instance (or call `reset()`) for each new conversation session.

## Managing Token Budgets

The stateful chatbot above has a problem: the conversation history grows without bound. Every turn adds more messages, and every message adds more tokens. Since the full history is sent with every API call, both the cost and the latency increase with every exchange.

To see how quickly this adds up, consider a conversation where each turn (one user message plus one assistant response) averages 200 tokens. The system prompt adds another 100 tokens. Here's how the total tokens per request grow:

```
Turn 1:  100 (system) + 50 (user) + 150 (assistant) = 300 tokens
Turn 2:  300 (previous) + 50 (user) + 150 (assistant) = 500 tokens
Turn 5:  everything so far ≈ 1,100 tokens
Turn 10: everything so far ≈ 2,100 tokens
Turn 20: everything so far ≈ 4,100 tokens
```

With GPT-3.5-turbo's 16K token context window, you'd hit the limit around turn 40–50 in a typical conversation. With a more detailed system prompt or longer messages, you'd hit it sooner. And you're paying for all of those tokens on every single API call — not just the new ones.

The simplest solution is to set a token budget and prune the oldest messages when the conversation exceeds it. Here's an implementation that does this using OpenAI's `tiktoken` library to count tokens accurately:

```python
import tiktoken

class TokenAwareChatbot(StatefulChatbot):
    def __init__(self, system_prompt, max_history_tokens=2000):
        super().__init__(system_prompt)
        self.max_history_tokens = max_history_tokens
        self.encoding = tiktoken.encoding_for_model("gpt-3.5-turbo")

    def count_tokens(self, messages):
        """Count the total tokens in a list of messages."""
        total = 0
        for message in messages:
            total += len(self.encoding.encode(message["content"]))
        return total

    def chat(self, user_message):
        self.conversation_history.append({
            "role": "user",
            "content": user_message
        })

        # Prune oldest messages (but never the system prompt) if over budget
        while self.count_tokens(self.conversation_history) > self.max_history_tokens:
            if len(self.conversation_history) > 3:
                self.conversation_history.pop(1)  # Remove oldest user message
                self.conversation_history.pop(1)  # Remove oldest assistant message
            else:
                break

        response = openai.ChatCompletion.create(
            model="gpt-3.5-turbo",
            messages=self.conversation_history
        )

        assistant_message = response.choices[0].message.content
        self.conversation_history.append({
            "role": "assistant",
            "content": assistant_message
        })

        return assistant_message
```

To use `tiktoken`, install it with `pip install tiktoken`. The `encoding_for_model` function returns the tokenizer that matches a specific model, so your token counts will be accurate.

This pruning approach works, but it's crude. Dropping old messages means the bot loses memory of the early parts of the conversation. If a user established important context at the beginning — "I'm running Windows 11 and using Python 3.12" — that context disappears once those messages get pruned. In Module 4, we'll explore more sophisticated strategies: summarizing older messages into a compact form that preserves key information, selectively retaining important context, and using retrieval-based approaches to recall relevant parts of the conversation history on demand.

## Crafting Effective System Prompts

The system prompt is the single most powerful tool you have for controlling a chatbot's behaviour. It's the first message in every conversation, it's included in every API call, and it tells the model who it is, what it can do, how it should communicate, and what it should avoid.

A good system prompt has several components. It starts with a **role definition** — who is this chatbot? Not just "a helpful assistant," but a specific character with a specific job. It defines **capabilities** — what the chatbot can and should help with. It specifies **personality and tone** — is it formal or casual, brief or detailed, cautious or enthusiastic? It sets **constraints** — what the chatbot should never do, what topics it should avoid, how it should handle questions outside its scope. And for complex workflows, it includes **procedures** — step-by-step instructions for how to handle specific situations.

Here's an example that brings all of these elements together:

```python
system_prompt = """You are an expert customer support agent for TechCorp,
a software company specializing in project management tools.

Your capabilities:
- Answer questions about our product features
- Troubleshoot common technical issues
- Guide users through setup and configuration
- Escalate complex issues to human agents when needed

Your personality:
- Professional yet friendly
- Patient and empathetic
- Clear and concise in explanations

Important guidelines:
- Never make up information about features that don't exist
- If you don't know something, admit it and offer to connect them with someone who does
- Always prioritize the user's success and satisfaction
- Keep responses under 150 words unless detailed explanation is needed

When handling issues:
1. Acknowledge the user's problem
2. Ask clarifying questions if needed
3. Provide step-by-step solutions
4. Verify the issue is resolved
"""
```

Compare this to a system prompt that just says "You are a helpful assistant." The detailed version produces dramatically different behaviour. The chatbot will stay in character, follow the prescribed workflow, decline to speculate about features it doesn't know about, and proactively offer to escalate when it's out of its depth.

The right system prompt depends entirely on the use case. A FAQ bot should be brief and direct, with clear boundaries on what topics it handles. A technical support bot should ask clarifying questions before jumping to solutions. An educational tutor should never give direct answers — instead, it should guide students to discover solutions through Socratic questioning. A sales assistant should be enthusiastic and persuasive while remaining honest.

Here are four system prompts that illustrate how different goals lead to different designs:

```python
# FAQ Bot — stateless, brief, boundary-aware
"""You are a FAQ assistant for a restaurant.
Answer questions about hours, location, menu, and reservations.
Be brief and friendly. If asked something outside these topics,
politely redirect to calling the restaurant."""

# Technical Support — stateful, methodical
"""You are a senior technical support engineer.
Help users debug issues with detailed, step-by-step guidance.
Ask clarifying questions before suggesting solutions.
Include code examples when relevant."""

# Educational Tutor — stateful, Socratic
"""You are a patient programming tutor.
Never give direct answers - guide students to discover solutions.
Ask Socratic questions. Encourage experimentation.
Celebrate progress and provide hints when students are stuck."""

# Sales Assistant — stateful, personality-driven
"""You are an enthusiastic sales assistant for an e-commerce store.
Help customers find products matching their needs.
Ask about preferences, budget, and use cases.
Suggest 2-3 options with pros/cons. Be persuasive but honest."""
```

When writing system prompts, be as specific as possible. Vague instructions like "be helpful" give the model too much latitude. Specific instructions like "answer questions about X, Y, and Z; if asked about anything else, say you can only help with those topics and suggest they contact support@example.com" produce consistent, predictable behaviour.

Keep your system prompts under 500 words. Every word costs tokens, and the system prompt is included in every single API call. A verbose system prompt that consumes 1,000 tokens will add $0.0005 to every request — which adds up quickly at scale.

Finally, test your system prompts thoroughly. Small wording changes can produce surprisingly different behaviour. Try edge cases: what happens if the user asks about a topic outside the chatbot's scope? What if they're rude? What if they try to override the system prompt? Iterate on your prompts the same way you'd iterate on code.

::::challenge{id=m1-04-ex1 title="Build a Stateless FAQ Bot"}

Create a stateless chatbot for a fictional coffee shop called "DevBrew" that specializes in coffee for programmers. The shop is open 6 AM to 10 PM daily, located at 123 Code Street, and its menu highlights include Espresso, Cold Brew, and the "Debug Latte."

Write a system prompt that gives the bot the information it needs, defines its personality, and tells it how to handle off-topic questions. Then implement the `StatelessChatbot` class:

```python
import openai
import os

openai.api_key = os.getenv("OPENAI_API_KEY")

system_prompt = """
[YOUR SYSTEM PROMPT HERE]
"""

class StatelessChatbot:
    def __init__(self, system_prompt):
        # YOUR CODE HERE
        pass

    def chat(self, user_message):
        # YOUR CODE HERE
        pass

# Test your bot
bot = StatelessChatbot(system_prompt)
print(bot.chat("What are your hours?"))
print(bot.chat("Do you have cold brew?"))
print(bot.chat("What's the meaning of life?"))  # Off-topic — how does it respond?
```

Once it's working, try these extensions: add the error handling pattern from the previous section, print the token usage for each call, and experiment with different temperature values to see how they affect the responses.
::::

::::challenge{id=m1-04-ex2 title="Add Conversation History"}

Extend your coffee shop bot to maintain conversation history. Implement the `StatefulChatbot` class with `chat()`, `get_history()`, and `reset()` methods:

```python
class StatefulChatbot:
    def __init__(self, system_prompt):
        self.system_prompt = system_prompt
        self.conversation_history = []
        # TODO: Initialize with system prompt

    def chat(self, user_message):
        # TODO: Append user message to history
        # TODO: Make API call with full history
        # TODO: Append assistant response to history
        # TODO: Return the response
        pass

    def get_history(self):
        # TODO: Return history excluding system prompt
        pass

    def reset(self):
        # TODO: Clear history, keeping system prompt
        pass
```

Test it with a sequence of messages that require context:

```python
bot = StatefulChatbot(system_prompt)
print(bot.chat("Tell me about your cold brew"))
print(bot.chat("How much does that cost?"))  # "that" should refer to cold brew
print(bot.chat("I'll take two"))             # Should understand the order

assert len(bot.get_history()) == 6  # 3 user + 3 assistant messages

bot.reset()
assert len(bot.get_history()) == 0
```

The key test is whether follow-up messages that use pronouns ("that", "it", "those") work correctly. If they do, your conversation history is working.
::::

::::challenge{id=m1-04-ex3 title="Implement Token Budgeting"}

Add token counting and automatic history pruning to your stateful chatbot. You'll need to install `tiktoken`:

```bash
pip install tiktoken
```

Then implement the `TokenManagedChatbot` class:

```python
import tiktoken

class TokenManagedChatbot(StatefulChatbot):
    def __init__(self, system_prompt, max_history_tokens=1000):
        super().__init__(system_prompt)
        self.max_history_tokens = max_history_tokens
        self.encoding = tiktoken.encoding_for_model("gpt-3.5-turbo")

    def count_tokens(self, messages):
        # TODO: Count total tokens across all messages
        pass

    def prune_history(self):
        # TODO: Remove oldest user/assistant pairs until under budget
        # Never remove the system prompt (index 0)
        pass

    def chat(self, user_message):
        self.conversation_history.append({
            "role": "user",
            "content": user_message
        })

        self.prune_history()

        response = openai.ChatCompletion.create(
            model="gpt-3.5-turbo",
            messages=self.conversation_history
        )

        assistant_message = response.choices[0].message.content
        self.conversation_history.append({
            "role": "assistant",
            "content": assistant_message
        })

        return assistant_message
```

Test it with a long conversation and observe what happens:

```python
bot = TokenManagedChatbot(system_prompt, max_history_tokens=500)
for i in range(10):
    response = bot.chat(f"Tell me about coffee drink number {i}")
    token_count = bot.count_tokens(bot.conversation_history)
    print(f"Turn {i+1}: {len(bot.get_history())} messages, {token_count} tokens")
```

Watch how the message count stabilises as old messages get pruned. Think about what information is being lost. If the user mentioned something important in turn 1 and you're now on turn 8, is that information still available? What would happen if `max_history_tokens` were set very low — say, 200? These are the questions that motivate the more sophisticated memory strategies we'll explore in Module 4.
::::

::::challenge{id=m1-04-checkpoint title="Project Checkpoint 1: Capstone Foundation"}

This is a good time to begin your capstone project — the production-ready chatbot you'll build incrementally throughout the course.

Choose a domain that interests you. It could be customer support for a product you know well, a tutor for a subject you're studying, a scheduling assistant, an HR onboarding bot, or anything else that involves multi-turn conversation. Define 3–5 core capabilities your chatbot should have.

By the end of this module, you should have:

- A working `StatefulChatbot` class with conversation history
- A thoughtfully designed system prompt with a written rationale for your choices
- Basic token management
- Error handling with retries
- A test conversation (at least 10 turns) that demonstrates your chatbot's core capabilities

This foundation will grow significantly as you add retrieval (Module 2), caching (Module 3), memory management (Module 4), and the other capabilities covered in later modules.
::::

## Summary and What's Next

In this module, you've built a solid foundation for chatbot development. You understand how chatbots evolved from simple pattern matching to LLM-powered conversation, how tokens and context windows constrain what's possible, how to interact with the OpenAI API reliably, and how to build both stateless and stateful conversation systems with token management and effective system prompts.

In Module 2, we tackle one of the biggest limitations of LLMs: they only know what they were trained on. If your chatbot needs to answer questions about your company's documentation, a specific textbook, or today's news, the model's training data won't help. Retrieval-Augmented Generation (RAG) solves this by letting your chatbot search through external documents, retrieve relevant passages, and ground its responses in real, authoritative sources. It's one of the most powerful and practical patterns in production chatbot development, and it builds directly on the stateful conversation architecture you've just built.

## Additional Resources

- [OpenAI API Documentation](https://platform.openai.com/docs) — The official reference for all API features.
- [tiktoken](https://github.com/openai/tiktoken) — OpenAI's tokenizer library, essential for accurate token counting.
- [OpenAI Playground](https://platform.openai.com/playground) — A web-based interface for experimenting with prompts and parameters without writing code.
- "The Illustrated Transformer" — A visual, accessible guide to the architecture behind modern LLMs.
- "A Survey of Large Language Models" — An academic overview of the field for those who want to go deeper.

## Hands-on practical

Work through the accompanying notebook: [`module_1/04-building-stateless-and-stateful-chatbots.ipynb`](https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules/blob/main/module_1/04-building-stateless-and-stateful-chatbots.ipynb). It walks through the DevBrew stateless bot, a stateful follow-up bot, and a token-managed version that prunes history when it exceeds a configurable budget.
