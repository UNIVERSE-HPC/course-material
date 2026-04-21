---
name: LLM Fundamentals
dependsOn:
  - technology_and_tooling.chatbot_development.m1-01-introduction-to-modern-chatbots
tags: [chatbots, llm]
learningOutcomes:
  - Explain how Large Language Models work at a conceptual level.
  - Understand what tokens are and why they matter for cost and performance.
  - Describe context windows and their limitations.
  - Use sampling parameters like temperature and top-p to control LLM output.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    image: https://github.com/OxfordCompetencyCenters.png
    license: CC-BY-4.0
---

Before we start writing code, we need to understand what's happening under the hood when we call an LLM API. You don't need to become a machine learning researcher, but a working understanding of these concepts — tokens, context windows, and sampling parameters — is essential for building chatbots that work reliably and cost-effectively.

## How Large Language Models Work

A Large Language Model is a neural network that has been trained on an enormous corpus of text — books, websites, code repositories, conversations, and much more. Through this training, it learns statistical patterns in language: which words tend to follow which other words, how sentences are structured, how ideas relate to one another, and how different styles of writing work.

At its core, an LLM is a **next-token prediction engine**. Given a sequence of text, it predicts what token is most likely to come next. When you type "The capital of France is", the model has seen this pattern so many times during training that it assigns a very high probability to the token "Paris". This prediction mechanism, applied iteratively — predict a token, append it, predict the next one — is what produces the fluent, coherent text that makes LLMs so useful.

The training process happens in stages. First, during **pre-training**, the model learns from billions of text examples in an unsupervised fashion. It develops a broad understanding of language, facts, reasoning patterns, and even some degree of common sense. Next, during **fine-tuning**, the model is trained on more specific data to improve its performance on particular tasks, such as following instructions or holding conversations. Finally, **Reinforcement Learning from Human Feedback (RLHF)** aligns the model's behaviour with human preferences — teaching it to be helpful, harmless, and honest.

It's important to understand what LLMs are *not*. They don't "understand" language the way humans do. They don't have beliefs, intentions, or experiences. They are extraordinarily sophisticated pattern-matching machines. This distinction matters in practice because it explains both their strengths — they can generate fluent text on virtually any topic, adapt to different styles and contexts, and follow complex instructions — and their weaknesses. They can "hallucinate" plausible-sounding but entirely incorrect information, because they're generating text that *looks right* based on patterns, not verifying facts against a reliable source. They have no knowledge of events after their training cutoff. And they can be confidently wrong in ways that are hard to detect without domain expertise.

Understanding these properties will help you build chatbots that leverage LLM strengths while mitigating their weaknesses — through techniques like Retrieval-Augmented Generation (Module 2), careful prompt engineering, and appropriate guardrails.

## Understanding Tokens

Tokens are the fundamental unit of text that LLMs process. A token is not exactly a word — it's a chunk of text determined by the model's tokenizer. In English, one token is roughly four characters, or about three-quarters of a word on average.

Here are some examples of how text gets split into tokens:

```
"Hello, world!" → ["Hello", ",", " world", "!"] = 4 tokens
"Chatbot development" → ["Chat", "bot", " development"] = 3 tokens
"AI" → ["AI"] = 1 token
```

Notice that punctuation, spaces, and common word fragments all become separate tokens. Short, common words often map to a single token, while longer or rarer words get split into multiple tokens.

Tokens matter for three practical reasons. First, **cost**: LLM APIs charge per token, both for the tokens you send (input/prompt tokens) and the tokens the model generates (output/completion tokens). Every message in your conversation history, every word of your system prompt, and every word of the model's response contributes to your bill. Second, **context limits**: every model has a maximum number of tokens it can process in a single request, called the context window. If your conversation history plus your new message plus the system prompt exceeds this limit, something has to be dropped. Third, **latency**: more tokens means more processing time, which means slower responses for your users.

When you're building a chatbot, you'll need to think about tokens constantly. How many tokens does your system prompt consume? How quickly does your conversation history grow? At what point do you need to start pruning old messages? These are questions we'll address throughout the course, with practical solutions in Module 4.

## Context Windows

The context window is the total number of tokens an LLM can process in a single request. Think of it as the model's working memory — everything it can "see" at once. This includes the system prompt, the full conversation history, the user's latest message, and the space reserved for the model's response.

Different models have different context window sizes:

| Model | Context Window | Approximate Words |
|-------|----------------|-------------------|
| GPT-3.5-turbo | 16K tokens | ~12,000 words |
| GPT-4 | 8K – 128K | Variable by version |
| GPT-4-turbo | 128K tokens | ~96,000 words |
| Claude 3 | 200K tokens | ~150,000 words |

These numbers might seem large, but they fill up faster than you'd expect. A detailed system prompt might use 500 tokens. Each conversational turn (user message plus assistant response) might average 200–400 tokens. After 20–30 turns of conversation with a 16K-token model, you're approaching the limit — and that's without including any retrieved documents or other context.

When a conversation exceeds the context window, you have to make choices. The simplest approach is to drop the oldest messages, but this means the bot loses memory of the early parts of the conversation. More sophisticated approaches include summarizing older messages into a compact form, or selectively retaining only the most important parts of the history. We'll implement these strategies in Module 4.

Beyond context windows, LLMs have other important limitations to keep in mind. They have no knowledge of events that occurred after their training data cutoff. They cannot access the internet or external databases on their own. And they can hallucinate — generating text that sounds authoritative but is factually incorrect. Each of these limitations has a corresponding solution that we'll cover later in the course: Retrieval-Augmented Generation (Module 2) grounds responses in real documents, memory management (Module 4) handles efficient context usage, and function calling (Module 7) lets the chatbot access external data sources.

## Sampling Parameters

When an LLM generates a response, it doesn't simply output the single most likely next token. Instead, it produces a probability distribution over all possible next tokens, and then *samples* from that distribution. Sampling parameters let you control how that sampling works, which in turn controls the style, creativity, and consistency of the output.

The most important parameter is **temperature**, which ranges from 0.0 to 2.0. At temperature 0.0, the model always picks the highest-probability token — this produces deterministic, focused, and repetitive output. As you increase the temperature, the model becomes more willing to select lower-probability tokens, producing more varied and creative output. At temperature 2.0, the output becomes quite random and often incoherent.

In practice, the right temperature depends on your use case. For a customer support chatbot, you want consistent, reliable answers — set temperature to 0.0–0.3. For a general-purpose conversational assistant, 0.7–1.0 works well. For brainstorming or creative writing, you might go as high as 1.5. You'll often use different temperature settings for different parts of the same system — for example, low temperature when generating a search query for document retrieval, and a moderate temperature when composing the final user-facing response.

**Top-P** (also called nucleus sampling) is another way to control randomness. Instead of adjusting the probability distribution like temperature does, top-p sets a cumulative probability threshold. If you set top-p to 0.9, the model only considers the smallest set of tokens whose combined probability is at least 90%, and samples from that set. Generally, you adjust either temperature or top-p, not both at once.

Two other useful parameters are **frequency penalty** and **presence penalty**. Frequency penalty reduces the likelihood of tokens that have already appeared in the output, proportional to how many times they've appeared. This helps reduce repetitive phrasing. Presence penalty applies a flat reduction to any token that has appeared at all, encouraging the model to bring up new topics rather than rehashing the same ones.

Finally, **max_tokens** sets a hard limit on the length of the model's response. This is important in production to prevent unexpectedly long (and expensive) responses. If the model hits the max_tokens limit before finishing its thought, the response will be cut off mid-sentence — the `finish_reason` field in the API response will be `"length"` rather than `"stop"`, which you can check and handle in your code.

Here's how these parameters look in an API call:

```python
response = openai.ChatCompletion.create(
    model="gpt-3.5-turbo",
    messages=messages,
    temperature=0.7,
    max_tokens=150,
    top_p=0.9,
    frequency_penalty=0.5
)
```

As you build chatbots throughout this course, you'll develop an intuition for how these parameters affect output quality. The key is to experiment: try different settings, compare the results, and choose values that match the behaviour you want for your specific application.

## Hands-on practical

Work through the accompanying notebook: [`module_1/02-llm-fundamentals.ipynb`](https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules/blob/main/module_1/02-llm-fundamentals.ipynb). It walks through token-counting with `tiktoken`, a context-window budgeting calculation, temperature comparisons, and the `finish_reason == "length"` truncation case.
