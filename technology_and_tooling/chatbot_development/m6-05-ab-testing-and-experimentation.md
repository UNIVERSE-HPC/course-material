---
name: A/B Testing and Experimentation
dependsOn:
  - technology_and_tooling.chatbot_development.m6-01-user-engagement-metrics
tags: [chatbots, analytics, ab-testing, experimentation, optional]
learningOutcomes:
  - Build a deterministic A/B testing framework for chatbot experiments.
  - Track experiment metrics and evaluate results with statistical significance.
  - Apply A/B testing to optimise prompts, models, and retrieval strategies.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    image: https://github.com/OxfordCompetencyCenters.png
    license: CC-BY-4.0
---

Analytics tells you how your chatbot is performing. A/B testing tells you whether a change makes it better. Without controlled experiments, you can't distinguish genuine improvements from noise — a metric might improve after a code change simply because of seasonal variation in traffic, not because the change was beneficial. A/B testing gives you scientific confidence that changes actually work.

The types of changes worth A/B testing in a chatbot include different system prompts, RAG strategies (basic retrieval versus re-ranked), models (GPT-3.5 versus GPT-4 for specific query types), UI changes (different message layouts, input prompts), and response styles (concise versus detailed, formal versus casual).

## Building an A/B Testing Framework

The core of A/B testing is deterministic assignment: the same user must always see the same variant. If a user gets the "detailed prompt" on one request and the "concise prompt" on the next, the experiment is contaminated. Deterministic assignment is achieved by hashing the combination of user ID and experiment name:

```python
import hashlib
from collections import defaultdict
from datetime import datetime
import numpy as np

class ABTestManager:
    def __init__(self):
        self.experiments = {}

    def create_experiment(self, experiment_name, variants, traffic_split):
        """Create an A/B test.

        Args:
            experiment_name: Unique identifier for the experiment.
            variants: Dict mapping variant names to their configurations.
            traffic_split: Dict mapping variant names to percentage of traffic
                          (must sum to 100).
        """
        self.experiments[experiment_name] = {
            "variants": variants,
            "traffic_split": traffic_split
        }

    def assign_variant(self, experiment_name, user_id):
        """Deterministically assign a user to a variant."""
        if experiment_name not in self.experiments:
            return None

        hash_input = f"{user_id}:{experiment_name}"
        hash_value = int(hashlib.md5(hash_input.encode()).hexdigest(), 16)
        percentage = hash_value % 100

        traffic_split = self.experiments[experiment_name]["traffic_split"]
        cumulative = 0

        for variant, split in traffic_split.items():
            cumulative += split
            if percentage < cumulative:
                return variant

        return list(traffic_split.keys())[0]

    def get_variant_config(self, experiment_name, user_id):
        """Get the configuration for a user's assigned variant."""
        variant = self.assign_variant(experiment_name, user_id)
        if variant is None:
            return None
        return self.experiments[experiment_name]["variants"][variant]
```

Using this framework in a chat endpoint is straightforward:

```python
ab_test = ABTestManager()

ab_test.create_experiment(
    experiment_name="prompt_style",
    variants={
        "control": {
            "system_prompt": "You are a helpful assistant."
        },
        "treatment": {
            "system_prompt": "You are an expert assistant who provides detailed, "
                           "well-structured answers."
        }
    },
    traffic_split={"control": 50, "treatment": 50}
)

@app.post("/chat")
async def chat(request: ChatRequest):
    variant_config = ab_test.get_variant_config("prompt_style", request.user_id)
    system_prompt = variant_config["system_prompt"]

    logger.info("ab_test_assignment", extra={
        "user_id": request.user_id,
        "experiment": "prompt_style",
        "variant": ab_test.assign_variant("prompt_style", request.user_id)
    })

    response = llm.generate(system_prompt=system_prompt, ...)
    return {"response": response}
```

## Tracking Experiment Results

Each experiment needs a tracker that records metrics per variant so you can compare them:

```python
class ExperimentTracker:
    def __init__(self):
        self.metrics = defaultdict(lambda: defaultdict(list))

    def record_metric(self, experiment, variant, metric_name, value):
        """Record a metric observation for an experiment variant."""
        self.metrics[experiment][variant].append({
            "metric": metric_name,
            "value": value,
            "timestamp": datetime.now()
        })

    def get_results(self, experiment):
        """Get aggregated results for an experiment."""
        results = {}

        for variant, metrics in self.metrics[experiment].items():
            by_metric = defaultdict(list)
            for m in metrics:
                by_metric[m["metric"]].append(m["value"])

            variant_results = {}
            for metric_name, values in by_metric.items():
                variant_results[metric_name] = {
                    "mean": np.mean(values),
                    "std": np.std(values),
                    "count": len(values)
                }

            results[variant] = variant_results

        return results
```

Recording metrics in the chat endpoint ties each observation to the user's assigned variant:

```python
exp_tracker = ExperimentTracker()

@app.post("/chat")
async def chat(request: ChatRequest):
    variant = ab_test.assign_variant("prompt_style", request.user_id)

    start = time.time()
    response = generate_response(...)
    latency = time.time() - start

    exp_tracker.record_metric("prompt_style", variant, "latency", latency)
    exp_tracker.record_metric("prompt_style", variant, "response_length", len(response))

    return response
```

After collecting sufficient data (typically 1,000 or more observations per variant), you can compare the results:

```python
results = exp_tracker.get_results("prompt_style")
# {
#   "control": {
#     "latency": {"mean": 2.1, "std": 0.5, "count": 1543},
#     "response_length": {"mean": 342, "std": 120, "count": 1543}
#   },
#   "treatment": {
#     "latency": {"mean": 2.8, "std": 0.7, "count": 1521},
#     "response_length": {"mean": 485, "std": 150, "count": 1521}
#   }
# }
```

In this example, the treatment variant produces longer responses (485 versus 342 characters) but takes longer (2.8 versus 2.1 seconds). Whether this is an improvement depends on which metric you optimise for. Longer responses might mean more thorough answers, but the increased latency degrades the user experience. You would need to measure engagement metrics (session length, return rate) for each variant to determine which one users actually prefer.

To determine whether the observed differences are real or just noise, use a statistical significance test:

```python
from scipy import stats

def is_significant(control_values, treatment_values, alpha=0.05):
    """Check if the difference between variants is statistically significant."""
    t_stat, p_value = stats.ttest_ind(control_values, treatment_values)
    return {
        "p_value": p_value,
        "is_significant": p_value < alpha,
        "effect_size": (np.mean(treatment_values) - np.mean(control_values))
                      / np.std(control_values)
    }
```

If the p-value is below 0.05, you can be 95% confident the difference is real. If the treatment is significantly better on the metrics you care about, roll it out to 100% of traffic. If it's significantly worse, kill the experiment. If the difference is not significant, either collect more data or accept that the change has no meaningful impact.

For production A/B testing at scale, tools like LaunchDarkly, Optimizely, or GrowthBook provide feature flag management, automatic traffic splitting, and statistical analysis without requiring you to build the framework from scratch.

::::challenge{id=m6-05-ex1 title="Engagement Tracking"}

Implement the `EngagementTracker` class and integrate it into your chatbot API. Log all messages and generate a daily engagement report including DAU, average session duration, and messages per session. Calculate day-7 retention for a cohort of your choosing.
::::

::::challenge{id=m6-05-ex2 title="Performance Monitoring"}

Add latency tracking to your chat endpoint, instrumenting each component (retrieval, generation, memory update) separately. Implement the `CostTracker` and create a `/metrics` endpoint that exposes both latency percentiles and cost breakdown.
::::

::::challenge{id=m6-05-ex3 title="Analytics Dashboard"}

Build a Streamlit dashboard that displays your chatbot's key metrics:

```python
import streamlit as st
import plotly.express as px

st.title("Chatbot Analytics Dashboard")

st.header("Engagement")
col1, col2, col3 = st.columns(3)
col1.metric("DAU", report['dau'])
col2.metric("Avg Session Duration", f"{report['avg_duration']:.1f}s")
col3.metric("Messages/Session", f"{report['avg_messages']:.1f}")

st.header("Performance")
latency_df = get_latency_timeseries()
fig = px.line(latency_df, x="timestamp", y="p95_latency", title="P95 Latency")
st.plotly_chart(fig)

st.header("Costs")
cost_breakdown = cost_tracker.get_cost_breakdown()
# Visualise as a bar chart or table
```

Include engagement metrics, latency trends over time, cost breakdown by model, and conversation quality indicators.
::::

::::challenge{id=m6-05-ex4 title="A/B Test"}

Set up an A/B test comparing two system prompts. Run at least 100 conversations through each variant and analyse the results. Report on which variant performs better and whether the difference is statistically significant.

```python
ab_test.create_experiment(
    "system_prompt_test",
    variants={
        "control": {"prompt": "You are a helpful assistant."},
        "treatment": {"prompt": "You are a knowledgeable expert."}
    },
    traffic_split={"control": 50, "treatment": 50}
)
```
::::

::::challenge{id=m6-05-checkpoint title="Project Checkpoint 6: Add Analytics"}

Instrument your capstone project with a complete analytics system.

Deliverables:

- Engagement tracking: DAU, retention, session statistics
- Performance monitoring: latency percentiles (p50/p95/p99), cost per request, total spend
- Structured logging with trace IDs
- Analytics dashboard (Streamlit or similar) with real-time or near-real-time updates, historical trends, and filterable time ranges
- A/B test with at least 100 samples per variant, statistical significance analysis, and a decision (roll out or kill)
- Documentation of your analytics strategy and findings
::::

## Summary and What's Next

In this module, you've built a complete observability stack for your chatbot. You can now measure user engagement (who's using it, how often, and do they return), analyse conversation quality (where do conversations succeed and fail, what do users actually want), monitor performance (how fast and how expensive), trace individual requests across your entire system, and run controlled experiments to measure the impact of changes.

Every improvement to your chatbot is now measurable. When you change a prompt, you can quantify the impact on engagement. When you optimise retrieval, you can verify the latency improvement. When you add a new feature, you can track whether users adopt it. This data-driven approach is what separates prototype chatbots from production systems.

In Module 7, you'll apply production best practices to make your chatbot robust, safe, and cost-effective. You'll learn advanced prompt engineering for reliability, implement safety guardrails and content moderation, optimise costs at scale, and explore strategies for handling high traffic.

## Additional Resources

- ["Lean Analytics"](https://leananalyticsbook.com/) by Alistair Croll & Benjamin Yoskovitz — Choosing the right metrics for your stage
- ["Trustworthy Online Controlled Experiments"](https://www.cambridge.org/core/books/trustworthy-online-controlled-experiments/D97B26382EB0EB2DC2019A7A7B518F59) by Kohavi, Tang & Xu — Comprehensive A/B testing guide
- ["Observability Engineering"](https://www.oreilly.com/library/view/observability-engineering/9781492076438/) by Charity Majors, Liz Fong-Jones & George Miranda — Modern observability practices
- [OpenTelemetry Documentation](https://opentelemetry.io/docs/) — Distributed tracing and metrics standard
- [Prometheus](https://prometheus.io/) — Open-source monitoring and alerting
- [Grafana](https://grafana.com/) — Dashboarding and visualisation
- [Jaeger](https://www.jaegertracing.io/) — Distributed tracing backend
