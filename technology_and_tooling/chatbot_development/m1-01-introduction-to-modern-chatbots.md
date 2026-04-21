---
name: Introduction to Modern Chatbots
dependsOn: []
tags: [chatbots]
learningOutcomes:
  - Understand the evolution from rule-based to LLM-powered chatbots.
  - Describe modern chatbot architecture and its key components.
  - Distinguish between stateless and stateful chatbot systems.
  - Identify real-world chatbot applications and their requirements.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    license: CC-BY-4.0
---

Chatbots have been around for decades, but the ones we build today are fundamentally different from their predecessors. In this section we trace that evolution, look at the architecture of a modern chatbot, and introduce a distinction — stateless vs stateful — that will shape every design decision you make throughout this course.

## The Evolution: From ELIZA to ChatGPT

The history of chatbots falls into three broad eras, each defined by a different approach to understanding and generating language.

In the 1960s through the 1990s, chatbots were **rule-based systems**. The most famous example is ELIZA, created at MIT in 1966. ELIZA worked by scanning user input for keywords and applying pattern-matching rules. If you typed "I'm sad," it might respond "Why are you sad?" — not because it understood sadness, but because it had a rule that transformed "I'm [X]" into "Why are you [X]?" Other systems like ALICE refined this approach, but the fundamental limitation remained: an engineer had to anticipate every possible user input and write a rule for it. Anything outside the rules produced nonsensical or generic responses.

From the 2000s through the 2010s, chatbots shifted to **statistical methods**. Instead of hand-written rules, these systems used machine learning to classify user intent ("Is this person asking about shipping or returns?") and extract named entities ("They mentioned order #12345"). This was a significant improvement — the system could generalize from training examples rather than requiring exhaustive rules — but it still required substantial labelled data, and the responses were typically selected from a predefined set rather than generated on the fly.

The 2020s brought **Large Language Models** (LLMs), and with them a paradigm shift. Models like GPT and Claude are trained on billions of text examples and learn deep statistical patterns in language. They don't just classify intent — they understand context, handle nuance, and generate entirely novel responses. A rule-based system could never hold a coherent multi-turn conversation about debugging your Python code. An LLM can, because it has learned the patterns of how such conversations work from vast amounts of text.

This isn't just an incremental improvement. It changes what's possible. Instead of building chatbots that handle a narrow set of predefined scenarios, we can now build systems that engage in open-ended, natural conversation. The rest of this course focuses on how to do that well.

## Modern Chatbot Architecture

It's tempting to think of a chatbot as a simple pipeline: take user input, send it to an LLM, return the response. In practice, production chatbots are more complex. Here is a simplified view of the data flow:

```
User Input
    ↓
[Preprocessing & Intent Detection]
    ↓
[Context Management] ← [Conversation History]
    ↓
[LLM Processing]
    ↓
[Response Generation]
    ↓
[Post-processing & Safety]
    ↓
User Output
```

Each layer serves a purpose. The **frontend** is whatever interface the user interacts with — a web chat widget, a mobile app, a Slack integration, or a voice assistant. The **backend** sits behind that interface and handles the business logic: routing requests, managing sessions, enforcing access control, and calling the LLM.

The **LLM layer** is where text generation happens, but it doesn't operate in isolation. A **context management** component assembles the right information to send alongside the user's message — this includes the conversation history, the system prompt that defines the chatbot's behaviour, and potentially retrieved documents (which we'll cover in Module 2 when we discuss Retrieval-Augmented Generation).

Finally, **post-processing and safety** layers check the generated response before it reaches the user. This might involve filtering harmful content, enforcing response length limits, or formatting the output for the target platform.

The **storage layer** ties everything together, persisting conversation histories, user preferences, and analytics data. As you progress through this course, you'll build out each of these components. For now, the key takeaway is that a production chatbot is a system, not just an API call.

## Stateless vs Stateful: A Critical Distinction

One of the most important architectural decisions you'll make is whether your chatbot is stateless or stateful.

A **stateless** chatbot treats every message as an independent request. When a user asks "What is Python?", the bot answers. When the same user then asks "Can you show me an example?", the bot has no idea what "an example" refers to — it has no memory of the previous exchange. Each request arrives in isolation, is processed, and is forgotten. This is simple to implement and cheap to run, because you only send a single user message (plus a system prompt) to the LLM on every call.

A **stateful** chatbot maintains a conversation history. It remembers that the user just asked about Python, so when they say "Can you show me an example?", it understands the context and provides a Python example. This is essential for any interaction that involves follow-up questions, multi-step problem solving, or building on prior context — which is to say, most real conversations.

:::callout
The trade-off is cost and complexity. A stateful chatbot must store the conversation history and include it in every API call. As the conversation grows, so does the number of tokens sent to the LLM, which means higher latency and higher cost. Managing this growth — pruning old messages, summarising context, staying within the model's context window — is a core challenge we'll address throughout the course, particularly in Module 4.
:::

For simple, one-shot use cases like answering frequently asked questions ("What are your hours?", "Where are you located?"), a stateless design is perfectly adequate. For anything resembling a real conversation — customer support, tutoring, sales assistance, technical troubleshooting — you need statefulness.

## Real-World Chatbot Applications

To make these concepts concrete, consider how chatbots are used in production today.

**Customer support** is one of the most common applications. A support chatbot needs to be stateful because resolving a problem typically takes multiple turns. The user describes an issue, the bot asks clarifying questions, suggests a solution, and checks whether it worked. The bot also needs to know when to escalate to a human agent — a judgement call that depends on the full conversation context, not just the latest message.

**Internal knowledge bases** combine statefulness with retrieval. When a new employee asks "How do I set up my development environment?", the bot needs to retrieve the relevant documentation and walk them through it step by step. If they ask a follow-up question like "What if I'm on Windows?", the bot must remember what it was explaining and adjust. This pattern — stateful conversation plus document retrieval (RAG) — is one of the most powerful and practical architectures, and we'll build it in Module 2.

**E-commerce assistants** help shoppers find products. A user might say "I'm looking for running shoes," and the bot asks about their budget, preferred brands, and running style. Over several turns, it narrows down recommendations. Without conversation state, the bot would forget the user's budget the moment they mention a brand preference.

**Educational tutors** are particularly interesting because the system prompt can fundamentally change the chatbot's behaviour. A good tutoring bot doesn't just answer questions — it guides students to discover answers themselves through Socratic questioning. It remembers what the student has already tried, celebrates progress, and adjusts its approach based on the student's level of understanding. This requires both statefulness and a carefully crafted system prompt.

Notice the pattern: nearly every production chatbot requires statefulness. Pure stateless bots are the exception, not the rule. This is why we'll spend significant time on conversation management, token budgeting, and memory strategies throughout this course.

## Hands-on practical

Work through the accompanying notebook: [`module_1/01-introduction-to-modern-chatbots.ipynb`](https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules/blob/main/module_1/01-introduction-to-modern-chatbots.ipynb). It runs a tiny ELIZA-style rule-based responder alongside an LLM call on the same inputs so you can see the difference first-hand.
