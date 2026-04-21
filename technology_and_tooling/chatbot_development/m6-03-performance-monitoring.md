---
name: Performance Monitoring
dependsOn:
  - technology_and_tooling.chatbot_development.m6-02-conversation-analytics
tags: [chatbots, analytics, performance, latency, cost-tracking, optional]
learningOutcomes:
  - Track response latency at the component level using percentile metrics.
  - Implement cost tracking for LLM API usage across models.
  - Identify performance bottlenecks and apply targeted optimisations.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    license: CC-BY-4.0
---

User engagement and conversation quality metrics measure the experience from the user's perspective. Performance monitoring measures the system from the engineering perspective: how fast are responses, how much do they cost, and where are the bottlenecks? These metrics directly impact user satisfaction (nobody wants to wait 10 seconds for a response) and your budget (LLM API costs can spiral quickly at scale).

## Latency Tracking

Users expect chatbot responses within 2–3 seconds. Beyond 5 seconds, the experience starts to feel slow. Beyond 10 seconds, users begin to abandon conversations. To meet these targets, you need to know exactly where time is being spent in each request.

The key latency metrics are percentiles rather than averages. The median (p50) shows the typical experience. The 95th percentile (p95) shows what 5% of users experience in the worst case. The 99th percentile (p99) shows extreme outliers. Good targets for a chatbot API are p50 under 2 seconds, p95 under 5 seconds, and p99 under 10 seconds. If p99 is dramatically higher than p95, you have outliers worth investigating — they might indicate timeout issues, cold starts, or specific query patterns that trigger expensive processing.

The `PerformanceMonitor` class uses context managers to track latency at the component level:

```python
import time
from collections import defaultdict
import numpy as np

class PerformanceMonitor:
    def __init__(self):
        self.latencies = defaultdict(list)

    def track_latency(self, operation_name):
        """Context manager to track latency of an operation."""
        class LatencyTracker:
            def __init__(self, monitor, name):
                self.monitor = monitor
                self.name = name
                self.start_time = None

            def __enter__(self):
                self.start_time = time.time()
                return self

            def __exit__(self, *args):
                duration = time.time() - self.start_time
                self.monitor.record_latency(self.name, duration)

        return LatencyTracker(self, operation_name)

    def record_latency(self, name, duration):
        """Record a latency measurement."""
        self.latencies[name].append(duration)
        if len(self.latencies[name]) > 1000:
            self.latencies[name] = self.latencies[name][-1000:]

    def get_stats(self, name):
        """Get latency statistics for an operation."""
        if name not in self.latencies or not self.latencies[name]:
            return None

        latencies = np.array(self.latencies[name])
        return {
            "count": len(latencies),
            "mean": np.mean(latencies),
            "median": np.median(latencies),
            "p95": np.percentile(latencies, 95),
            "p99": np.percentile(latencies, 99),
            "min": np.min(latencies),
            "max": np.max(latencies)
        }

    def get_all_stats(self):
        """Get stats for all tracked operations."""
        return {name: self.get_stats(name) for name in self.latencies.keys()}
```

Wrapping each component of the chat endpoint with the tracker reveals where time is spent:

```python
monitor = PerformanceMonitor()

@app.post("/chat")
async def chat(request: ChatRequest):
    with monitor.track_latency("total_request"):
        with monitor.track_latency("rag_retrieval"):
            documents = rag.retrieve(request.message)

        with monitor.track_latency("llm_generation"):
            response = llm.generate(documents, request.message)

        with monitor.track_latency("memory_update"):
            memory.store(request.user_id, request.message, response)

    return {"response": response}

@app.get("/metrics/latency")
async def get_latency_metrics():
    """Expose latency metrics for monitoring."""
    stats = monitor.get_all_stats()
    return {
        operation: {
            "mean_ms": s["mean"] * 1000,
            "p95_ms": s["p95"] * 1000,
            "p99_ms": s["p99"] * 1000
        }
        for operation, s in stats.items()
    }
```

A typical breakdown might look like:

```
total_request:    mean 2340ms, p95 4500ms, p99 7200ms
rag_retrieval:    mean  450ms, p95  800ms, p99 1200ms
llm_generation:   mean 1800ms, p95 3500ms, p99 6000ms
memory_update:    mean   90ms, p95  150ms, p99  200ms
```

This breakdown makes optimisation decisions clear. LLM generation accounts for 77% of the total time. Optimising the RAG retrieval — which takes only 19% — won't meaningfully improve the user experience. The high-impact options are: using a faster model (GPT-3.5 instead of GPT-4), reducing the prompt length to lower generation time, or implementing streaming so the user sees tokens appear immediately even though total generation time is unchanged.

## Cost Tracking

LLM API costs are typically the largest operational expense for a chatbot. Without tracking, costs can spiral unexpectedly — a change that increases average prompt length by 200 tokens might seem minor, but at 10,000 requests per day it adds up to significant monthly spend.

The `CostTracker` class records token usage per model and calculates costs based on current pricing:

```python
class CostTracker:
    COSTS = {
        "gpt-3.5-turbo": {"input": 0.0005, "output": 0.0015},
        "gpt-4": {"input": 0.03, "output": 0.06},
        "embedding-3-small": {"input": 0.00002, "output": 0}
    }

    def __init__(self):
        self.usage = defaultdict(
            lambda: {"input_tokens": 0, "output_tokens": 0, "calls": 0}
        )

    def record_usage(self, model, input_tokens, output_tokens):
        """Record API usage for a request."""
        self.usage[model]["input_tokens"] += input_tokens
        self.usage[model]["output_tokens"] += output_tokens
        self.usage[model]["calls"] += 1

    def calculate_cost(self, model, input_tokens, output_tokens):
        """Calculate the dollar cost for a request."""
        pricing = self.COSTS.get(model, {"input": 0, "output": 0})
        input_cost = (input_tokens / 1000) * pricing["input"]
        output_cost = (output_tokens / 1000) * pricing["output"]
        return input_cost + output_cost

    def get_cost_breakdown(self):
        """Get cost breakdown by model."""
        breakdown = {}
        for model, usage in self.usage.items():
            cost = self.calculate_cost(
                model, usage["input_tokens"], usage["output_tokens"]
            )
            breakdown[model] = {
                "total_cost": cost,
                "calls": usage["calls"],
                "cost_per_call": cost / usage["calls"] if usage["calls"] > 0 else 0,
                "input_tokens": usage["input_tokens"],
                "output_tokens": usage["output_tokens"]
            }
        return breakdown
```

Integrating this into the chat endpoint captures costs automatically:

```python
cost_tracker = CostTracker()

@app.post("/chat")
async def chat(request: ChatRequest):
    response = openai.ChatCompletion.create(
        model="gpt-3.5-turbo",
        messages=messages
    )

    usage = response.usage
    cost_tracker.record_usage(
        "gpt-3.5-turbo",
        usage.prompt_tokens,
        usage.completion_tokens
    )

    cost = cost_tracker.calculate_cost(
        "gpt-3.5-turbo",
        usage.prompt_tokens,
        usage.completion_tokens
    )
    logger.info(f"Request cost: ${cost:.4f}")

    return {"response": response.choices[0].message.content}
```

The cost breakdown reveals optimisation opportunities. If GPT-3.5 accounts for 84% of total costs despite its lower per-token price, it's because it handles 67 times more requests than GPT-4. The key cost optimisation strategies are: using the cheapest model that meets quality requirements for each query type, implementing semantic caching to avoid re-processing similar queries (covered in Module 3), trimming unnecessary conversation history from prompts, batching embedding requests, and setting `max_tokens` limits to prevent runaway generations. Budget alerts are also essential — configure your system to alert you if daily costs exceed a threshold, so a traffic spike or a bug that generates excessively long prompts doesn't drain your budget before you notice.
