---
name: API Integration Basics
dependsOn:
  - technology_and_tooling.chatbot_development.m1-01-introduction-to-modern-chatbots
tags: [chatbots, python, api]
learningOutcomes:
  - Set up OpenAI API credentials securely.
  - Make API calls to generate chatbot responses.
  - Understand the request and response structure of the Chat Completions API.
  - Implement production-ready error handling with retries and exponential backoff.
  - Understand rate limits and cost management strategies.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    image: https://github.com/OxfordCompetencyCenters.png
    license: CC-BY-4.0
---

With the conceptual foundations in place, it's time to write code. In this section you'll set up the OpenAI API, make your first chatbot request, understand the structure of what goes in and what comes back, and build error handling that's robust enough for production use.

## Setting Up the OpenAI API

Before you can make API calls, you need an account and an API key. Visit [platform.openai.com](https://platform.openai.com), create an account, and verify your email. Once logged in, navigate to the API Keys section and create a new secret key.

This key is a secret — treat it like a password. Never hardcode it in your source files, and never commit it to version control. The standard approach is to store it in an environment variable:

```bash
export OPENAI_API_KEY='sk-...'
```

In production systems, you'd use a dedicated secret management service like AWS Secrets Manager or HashiCorp Vault, but environment variables are fine for development.

Next, install the Python client library:

```bash
pip install openai
```

You should also set spending limits in the OpenAI dashboard. This is your safety net — a bug that causes an infinite loop of API calls can rack up significant charges before you notice. Set a monthly budget that's appropriate for your usage, and monitor it regularly.

## Your First API Call

Here is a minimal working example that sends a message to the LLM and prints the response:

```python
import openai
import os

openai.api_key = os.getenv("OPENAI_API_KEY")

response = openai.ChatCompletion.create(
    model="gpt-3.5-turbo",
    messages=[
        {"role": "system", "content": "You are a helpful assistant."},
        {"role": "user", "content": "What is a chatbot?"}
    ]
)

answer = response.choices[0].message.content
print(answer)
```

The `messages` parameter is a list of message objects, each with a `role` and `content`. There are three roles. The `system` message comes first and defines the chatbot's behaviour — its personality, capabilities, and constraints. The `user` messages represent what the human has said. The `assistant` messages represent what the chatbot has previously replied. By including a sequence of alternating user and assistant messages, you provide the model with conversation history — but we'll get to that in the next section.

For now, notice that this example sends just two messages: a system prompt and a single user message. The model responds as if this is the very first (and only) thing the user has said, because it is. There's no memory of prior interactions. This is a stateless call.

## Understanding the Request and Response

It's worth looking at the full structure of what the API sends and receives, because you'll work with these data structures constantly.

A request looks like this:

```python
{
    "model": "gpt-3.5-turbo",
    "messages": [
        {"role": "system", "content": "You are..."},
        {"role": "user", "content": "Hello"},
        {"role": "assistant", "content": "Hi!"},
        {"role": "user", "content": "How are you?"}
    ],
    "temperature": 0.7,
    "max_tokens": 150
}
```

The `model` field selects which LLM to use. The `messages` array is the conversation so far — the model sees all of it and generates a response that continues the conversation. The optional parameters (`temperature`, `max_tokens`, etc.) control how the response is generated, as we discussed in the previous section.

The response comes back as a JSON object:

```python
{
    "id": "chatcmpl-abc123",
    "object": "chat.completion",
    "created": 1677652288,
    "model": "gpt-3.5-turbo-0301",
    "choices": [{
        "index": 0,
        "message": {
            "role": "assistant",
            "content": "I'm doing well, thank you!"
        },
        "finish_reason": "stop"
    }],
    "usage": {
        "prompt_tokens": 20,
        "completion_tokens": 8,
        "total_tokens": 28
    }
}
```

The three fields you'll use most are `choices[0].message.content` (the actual text of the response), `finish_reason` (whether the response completed naturally or was cut off by the token limit), and `usage` (the token counts for this request).

The `usage` field deserves special attention. It tells you exactly how many tokens went in (`prompt_tokens`) and how many came out (`completion_tokens`). In production, you should log this for every request. It's your primary tool for understanding costs, spotting unexpectedly expensive conversations, and optimizing your token budget. If `finish_reason` is `"length"` rather than `"stop"`, the response was truncated because it hit `max_tokens` — you may want to retry with a higher limit or handle the incomplete response gracefully in your UI.

## Error Handling for Production

API calls can fail for many reasons. The OpenAI service might be temporarily overloaded, your account might hit a rate limit, your API key might be invalid, or your request might be malformed. A production chatbot needs to handle all of these gracefully, rather than crashing or showing a raw error to the user.

The most common errors you'll encounter are:

- `RateLimitError` — You've exceeded your allowed number of requests or tokens per minute.
- `APIError` — Something went wrong on OpenAI's side.
- `Timeout` — The request took too long to complete.
- `AuthenticationError` — Your API key is invalid or missing.
- `InvalidRequestError` — Your request was malformed (e.g., too many tokens, invalid model name).

The standard pattern for handling transient errors (rate limits, temporary API issues) is **exponential backoff**: if the first attempt fails, wait 1 second and retry; if the second attempt fails, wait 2 seconds; then 4 seconds, and so on. This gives the service time to recover without overwhelming it with immediate retries.

Here's a robust implementation:

```python
import openai
import time
from openai.error import RateLimitError, APIError

def chat_with_retry(messages, max_retries=3):
    for attempt in range(max_retries):
        try:
            response = openai.ChatCompletion.create(
                model="gpt-3.5-turbo",
                messages=messages,
                timeout=30
            )
            return response.choices[0].message.content

        except RateLimitError:
            if attempt < max_retries - 1:
                wait_time = 2 ** attempt
                print(f"Rate limited. Waiting {wait_time}s...")
                time.sleep(wait_time)
            else:
                return "I'm experiencing high demand. Please try again."

        except APIError as e:
            print(f"OpenAI API error: {e}")
            return "I'm having technical difficulties. Please try again."

        except Exception as e:
            print(f"Unexpected error: {e}")
            return "Something went wrong. Please try again."
```

A few things to note about this code. The `timeout=30` parameter ensures that a hung API call won't block your application indefinitely. The exponential backoff (`2 ** attempt`) spaces out retries progressively. And the error messages returned to the user are friendly and don't expose internal details. In a real production system, you'd also log these errors to a monitoring service so you can track failure rates and respond to outages. We'll cover monitoring and observability in Module 6.

## Rate Limits and Cost Management

OpenAI enforces rate limits on API usage, measured in both requests per minute and tokens per minute. These limits vary by account tier:

| Tier | Requests/min | Tokens/min |
|------|--------------|------------|
| Free Trial | 3 | 40,000 |
| Pay-as-you-go (new) | 60 | 150,000 |
| Pay-as-you-go (established) | 3,500 | 90,000 |

Your limits increase as you build a spending history with OpenAI. A brand-new account is heavily restricted, which can be frustrating during development. If you're building a production application, you'll want to plan for these limits and implement request queuing to handle bursts gracefully.

Understanding costs is equally important. GPT-3.5-turbo charges approximately $0.0005 per 1,000 input tokens and $0.0015 per 1,000 output tokens. To put that in perspective: if your chatbot handles 100 conversations per day, each averaging 1,000 total tokens, your daily cost is about $0.10 and your monthly cost is about $3.00. That's quite affordable — but costs can escalate quickly if conversations are long, if you use more expensive models like GPT-4, or if a bug causes unnecessary API calls.

There are several strategies for keeping costs under control. Use GPT-3.5 for simpler tasks and reserve GPT-4 for situations that genuinely benefit from its greater capability. Set `max_tokens` on every request to prevent unexpectedly long responses. Prune conversation history to avoid sending thousands of tokens of old messages with every new request (we'll cover this in detail shortly). Implement response caching so identical questions don't trigger redundant API calls (Module 3). And always, always log your token usage so you can spot anomalies before they become expensive surprises.

::::challenge{id=m1-03-first-call title="Make Your First API Call"}

Now it's your turn. Set up your environment and make a simple API call.

1. Install the OpenAI Python library if you haven't already:

```bash
pip install openai
```

2. Set your API key as an environment variable.

3. Write a short script that sends a question to the API and prints the response. Try changing the system prompt to see how it affects the output. For example, compare the response you get with `"You are a helpful assistant"` versus `"You are a pirate who only speaks in nautical metaphors"`.

4. Modify your script to print the `usage` field from the response. How many tokens did your prompt use? How many tokens were in the response?

5. Try adjusting the `temperature` parameter. Make the same request three times at temperature 0.0, then three times at temperature 1.5. How does the variation in responses change?

These experiments will give you a hands-on feel for how the API behaves — something that's hard to get from reading alone.
::::

## Hands-on practical

Work through the accompanying notebook: [`module_1/03-api-integration-basics.ipynb`](https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules/blob/main/module_1/03-api-integration-basics.ipynb). It walks through your first chat completion call, response-object inspection, system-prompt comparison, and a retry-with-exponential-backoff implementation.
