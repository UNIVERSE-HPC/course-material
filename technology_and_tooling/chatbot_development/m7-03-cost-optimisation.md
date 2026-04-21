---
name: Cost Optimisation
dependsOn:
  - technology_and_tooling.chatbot_development.m7-02-safety-and-content-moderation
tags: [chatbots, cost-optimisation, token-management, model-routing, optional]
learningOutcomes:
  - Apply token reduction strategies including prompt compression and selective context.
  - Implement a smart model router that selects cheaper models for simpler queries.
  - Estimate and measure cost savings from optimisation techniques.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    image: https://github.com/OxfordCompetencyCenters.png
    license: CC-BY-4.0
---

LLM API costs are the dominant operational expense for most chatbot systems, and they scale linearly with usage. Every token in every request costs money — both the tokens you send (the prompt, context, and conversation history) and the tokens the model generates (the response). At 10,000 requests per day, even small per-request optimisations compound into significant monthly savings. Cost optimisation is not about cutting corners on quality; it's about eliminating waste so your budget is spent on tokens that actually improve the user experience.

## Token Reduction Strategies

**Prompt compression** is the easiest win. Many system prompts contain redundant or verbose phrasing that can be tightened without changing behaviour:

```python
# Verbose: ~150 tokens
prompt = """
You are a helpful customer support assistant. You should always
be polite and professional. When answering questions, make sure
to be clear and concise. If you don't know something, say so.
Always try to help the customer as best as you can.
"""

# Concise: ~60 tokens (60% reduction)
prompt = """
You are a polite, professional customer support assistant.
Be clear and concise. If unsure, admit it.
"""
```

Both prompts produce essentially the same behaviour, but the concise version saves 90 tokens per request. At 10,000 requests per day with GPT-3.5-turbo, that's 900,000 fewer input tokens per day — roughly $0.45/day or $13.50/month just from tightening the system prompt.

**Selective context inclusion** prevents sending irrelevant documents to the LLM. In a RAG system, not all retrieved documents are equally relevant. Rather than sending a fixed number of documents, rank them by relevance and include only those that fit within a token budget:

```python
def build_context_smartly(query, all_documents, max_tokens=1500):
    """Include only the most relevant context within a token budget."""
    ranked_docs = rank_by_relevance(query, all_documents)

    context = []
    token_count = 0

    for doc in ranked_docs:
        doc_tokens = count_tokens(doc)
        if token_count + doc_tokens < max_tokens:
            context.append(doc)
            token_count += doc_tokens
        else:
            break

    return context
```

**Response length limits** prevent the model from generating unnecessarily long responses. Set `max_tokens` on the API call and reinforce it in the prompt:

```python
response = openai.ChatCompletion.create(
    model="gpt-3.5-turbo",
    messages=messages,
    max_tokens=150
)

# Or via prompt instruction
system_prompt = """...[your prompt]...

Keep responses under 100 words unless a detailed explanation is needed.
"""
```

**Context summarisation** (covered in Module 4) replaces full conversation history with compressed summaries, reducing the tokens sent on each subsequent turn from thousands to hundreds.

## Smart Model Routing

Not every query needs the most expensive model. Simple factual questions, greetings, and straightforward lookups are handled well by GPT-3.5-turbo. Complex reasoning, nuanced analysis, and code generation benefit from GPT-4. A smart router classifies query complexity and routes to the appropriate model:

```python
class SmartModelRouter:
    def __init__(self):
        self.models = {
            "simple": {"name": "gpt-3.5-turbo", "cost": 0.002},
            "complex": {"name": "gpt-4", "cost": 0.06}
        }

    def classify_query_complexity(self, query):
        """Classify a query as simple or complex."""
        complexity_indicators = {
            "simple": [
                "what is", "who is", "when",
                "list", "define", "meaning"
            ],
            "complex": [
                "analyze", "compare", "why", "how does",
                "explain the difference", "design", "create"
            ]
        }

        query_lower = query.lower()

        simple_count = sum(1 for ind in complexity_indicators["simple"]
                          if ind in query_lower)
        complex_count = sum(1 for ind in complexity_indicators["complex"]
                           if ind in query_lower)

        if len(query.split()) > 30:
            complex_count += 1

        return "complex" if complex_count > simple_count else "simple"

    def route_request(self, query):
        """Route to the appropriate model."""
        complexity = self.classify_query_complexity(query)
        model = self.models[complexity]

        logger.info("model_routing", extra={
            "complexity": complexity,
            "model": model["name"],
            "query_length": len(query)
        })

        return model["name"]
```

The cost impact is dramatic. If 80% of queries are simple and routed to GPT-3.5, and 20% are complex and routed to GPT-4, the daily cost at 10,000 requests drops from $600 (all GPT-4) to roughly $136 — a 77% reduction. Monitor the routing accuracy over time: if simple queries routed to GPT-3.5 frequently produce poor responses, adjust the classification thresholds.

## Estimating Savings

Before implementing optimisations, estimate their impact to prioritise your effort:

```python
class TokenOptimizer:
    def estimate_savings(self, optimizations):
        """Estimate cost savings from a set of optimisations."""
        baseline = {
            "avg_prompt_tokens": 2000,
            "avg_completion_tokens": 400,
            "requests_per_day": 10000
        }

        baseline_cost_per_request = (
            (baseline["avg_prompt_tokens"] / 1000) * 0.0005 +
            (baseline["avg_completion_tokens"] / 1000) * 0.0015
        )
        baseline_daily_cost = baseline_cost_per_request * baseline["requests_per_day"]

        optimized = baseline.copy()
        for opt in optimizations:
            if opt["type"] == "prompt_compression":
                optimized["avg_prompt_tokens"] *= (1 - opt["reduction"])
            elif opt["type"] == "response_limit":
                optimized["avg_completion_tokens"] *= (1 - opt["reduction"])
            elif opt["type"] == "caching":
                optimized["requests_per_day"] *= (1 - opt["hit_rate"])

        optimized_cost_per_request = (
            (optimized["avg_prompt_tokens"] / 1000) * 0.0005 +
            (optimized["avg_completion_tokens"] / 1000) * 0.0015
        )
        optimized_daily_cost = optimized_cost_per_request * optimized["requests_per_day"]

        return {
            "baseline_daily": baseline_daily_cost,
            "optimized_daily": optimized_daily_cost,
            "daily_savings": baseline_daily_cost - optimized_daily_cost,
            "monthly_savings": (baseline_daily_cost - optimized_daily_cost) * 30,
            "savings_percentage": (1 - optimized_daily_cost / baseline_daily_cost) * 100
        }

optimizer = TokenOptimizer()
savings = optimizer.estimate_savings([
    {"type": "prompt_compression", "reduction": 0.30},
    {"type": "response_limit", "reduction": 0.20},
    {"type": "caching", "hit_rate": 0.40}
])

print(f"Monthly savings: ${savings['monthly_savings']:.2f} ({savings['savings_percentage']:.1f}%)")
```

Combining 30% prompt compression, 20% shorter responses, and a 40% cache hit rate can reduce costs by 60% or more. At 10,000 requests per day, this translates to over $1,000 per month in savings. Use the cost tracking system from Module 6 to verify actual savings after implementation and identify further opportunities.
