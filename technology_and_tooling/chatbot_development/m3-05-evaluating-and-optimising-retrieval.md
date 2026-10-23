---
name: Evaluating and Optimising Retrieval
dependsOn:
  - technology_and_tooling.chatbot_development.m3-01-agentic-rag
tags: [chatbots, rag, evaluation]
learningOutcomes:
  - Calculate standard retrieval metrics (Precision@K, Recall@K, MRR).
  - Build an evaluation suite to measure retrieval quality objectively.
  - Compare retrieval strategies using A/B testing with statistical significance.
  - Make data-driven decisions about which optimisations to deploy.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    image: https://github.com/OxfordCompetencyCenters.png
    license: CC-BY-4.0
---

You've now learned several techniques for improving RAG: query expansion, re-ranking, hybrid search, and semantic caching. But how do you know whether these techniques actually help for *your* data and *your* queries? The only way to know is to measure. This section introduces the standard metrics for evaluating retrieval quality and shows you how to build an evaluation framework that lets you compare strategies objectively.

## Retrieval Quality Metrics

Information retrieval has a well-established set of metrics. The ones most relevant to RAG are Precision@K, Recall@K, and Mean Reciprocal Rank (MRR).

**Precision@K** answers the question: of the K documents you retrieved, how many were actually relevant?

```
Precision@K = (Relevant documents in top K) / K
```

If you retrieve 5 documents and 3 of them are relevant, your Precision@5 is 3/5 = 0.60. This is the most intuitive metric for RAG because it directly measures the quality of the context you're feeding to the LLM. If your Precision@5 is low, the LLM is receiving a lot of irrelevant context alongside the relevant information, which can confuse it and reduce answer quality.

**Recall@K** answers the question: of all the relevant documents in your corpus, how many did you retrieve in the top K?

```
Recall@K = (Relevant docs in top K) / (Total relevant docs in corpus)
```

If there are 10 relevant documents and you retrieved 3 of them, your Recall@5 is 3/10 = 0.30. Recall matters less than precision for most RAG applications, because you typically only need a few good documents to answer a question — you don't need to find every relevant document in the corpus.

**Mean Reciprocal Rank (MRR)** answers the question: how quickly does the first relevant result appear?

```
MRR = Average of (1 / rank of first relevant result)
```

If the first relevant result appears at position 1, the reciprocal rank is 1.0. If it appears at position 3, it's 0.33. MRR is averaged across all queries in your test set. A high MRR means your system consistently surfaces a relevant document near the top of the results, which is important because the LLM pays most attention to the context provided first.

## Building an Evaluation Suite

To compute these metrics, you need a test dataset: a set of queries paired with the IDs of documents that should be retrieved for each query. Building this dataset is the hardest part of evaluation, but it's essential.

```python
class RetrievalEvaluator:
    def __init__(self, rag_system):
        self.rag = rag_system

    def precision_at_k(self, query, relevant_doc_ids, k=5):
        """What fraction of retrieved docs are relevant?"""
        retrieved = self.rag.retrieve(query, top_k=k)
        retrieved_ids = [doc['id'] for doc in retrieved]
        relevant_retrieved = len(set(retrieved_ids) & set(relevant_doc_ids))
        return relevant_retrieved / k

    def recall_at_k(self, query, relevant_doc_ids, k=5):
        """What fraction of all relevant docs did we retrieve?"""
        retrieved = self.rag.retrieve(query, top_k=k)
        retrieved_ids = [doc['id'] for doc in retrieved]
        relevant_retrieved = len(set(retrieved_ids) & set(relevant_doc_ids))
        return relevant_retrieved / len(relevant_doc_ids)

    def mrr(self, query, relevant_doc_ids):
        """How quickly does the first relevant result appear?"""
        retrieved = self.rag.retrieve(query, top_k=10)
        retrieved_ids = [doc['id'] for doc in retrieved]

        for rank, doc_id in enumerate(retrieved_ids, start=1):
            if doc_id in relevant_doc_ids:
                return 1.0 / rank
        return 0.0

    def evaluate_dataset(self, test_queries):
        """Evaluate across a full test dataset."""
        precisions, recalls, mrrs = [], [], []

        for item in test_queries:
            p = self.precision_at_k(item['query'], item['relevant_doc_ids'], k=5)
            r = self.recall_at_k(item['query'], item['relevant_doc_ids'], k=5)
            m = self.mrr(item['query'], item['relevant_doc_ids'])

            precisions.append(p)
            recalls.append(r)
            mrrs.append(m)

        return {
            "precision@5": sum(precisions) / len(precisions),
            "recall@5": sum(recalls) / len(recalls),
            "mrr": sum(mrrs) / len(mrrs)
        }
```

You can create test datasets in two ways. **Manual curation** is the gold standard: read your documents, write natural questions about them, and record which documents each question should retrieve. This is time-consuming but produces high-quality evaluations. Start with 20–30 queries — that's enough to get meaningful signal.

**Synthetic generation** is faster: use an LLM to generate questions for each document, then use the source document as the expected retrieval target.

```python
def generate_test_queries(document, doc_id):
    """Use an LLM to generate questions that this document should answer."""
    prompt = f"""Given this document, generate 3 questions that this document
    should be able to answer.

    Document: {document}

    Questions:"""

    response = openai.ChatCompletion.create(
        model="gpt-3.5-turbo",
        messages=[{"role": "user", "content": prompt}]
    )

    questions = response.choices[0].message.content.split('\n')
    return [{"query": q.strip(), "relevant_doc_ids": [doc_id]}
            for q in questions if q.strip()]
```

Synthetic test sets are useful for quick iteration, but they can be biased toward the vocabulary of the documents (since the LLM generates questions from the document text). Always supplement with some manually written queries that use different phrasing.

## Comparing Strategies with A/B Testing

Once you have an evaluation suite, you can objectively compare retrieval strategies:

```python
class RetrievalComparison:
    def __init__(self):
        self.strategies = {}

    def add_strategy(self, name, strategy_func):
        """Register a retrieval strategy for comparison."""
        self.strategies[name] = strategy_func

    def compare(self, test_queries):
        """Run all strategies on the test dataset and compare."""
        results = {name: [] for name in self.strategies}

        for query_item in test_queries:
            query = query_item['query']
            relevant_ids = query_item['relevant_doc_ids']

            for name, strategy in self.strategies.items():
                retrieved = strategy(query, top_k=5)
                retrieved_ids = [doc['id'] for doc in retrieved]
                relevant_retrieved = len(set(retrieved_ids) & set(relevant_ids))
                precision = relevant_retrieved / 5
                results[name].append(precision)

        summary = {}
        for name, precisions in results.items():
            summary[name] = {
                "avg_precision": sum(precisions) / len(precisions),
                "min": min(precisions),
                "max": max(precisions)
            }
        return summary

# Usage
comparison = RetrievalComparison()
comparison.add_strategy("basic", lambda q, top_k: basic_rag.retrieve(q, top_k))
comparison.add_strategy("reranked", lambda q, top_k: reranked_rag.retrieve(q, top_k))
comparison.add_strategy("hybrid", lambda q, top_k: hybrid_rag.retrieve(q, top_k))

results = comparison.compare(test_queries)

for strategy, metrics in results.items():
    print(f"{strategy}: Avg P@5={metrics['avg_precision']:.2f} "
          f"(range: {metrics['min']:.2f}-{metrics['max']:.2f})")
```

When comparing strategies, it's important to check whether differences are statistically significant, not just noise. A paired t-test is the standard approach:

```python
from scipy import stats

def is_significantly_better(scores_a, scores_b, alpha=0.05):
    """Check if strategy A is significantly better than strategy B."""
    t_stat, p_value = stats.ttest_rel(scores_a, scores_b)
    return (p_value < alpha and t_stat > 0), p_value
```

If the p-value is below 0.05, you can be 95% confident that the difference is real, not a fluke of your test set. For production changes, always test on held-out queries — not the ones you used to tune your system.

::::challenge{id=m3-05-evaluation title="Build a Complete Evaluation Pipeline"}

Starting from your RAG system, build an evaluation pipeline and use it to measure the impact of each optimisation technique you've learned in this module.

**Step 1: Create a test dataset.** Write at least 20 (query, relevant document) pairs for your corpus. Include a mix of simple factual questions, questions that use different phrasing from the documents, and complex multi-part questions.

**Step 2: Measure your baseline.** Evaluate your Module 2 basic RAG system on the test dataset. Record Precision@5, Recall@5, and MRR.

**Step 3: Add improvements one at a time.** After each improvement (query expansion, re-ranking, hybrid search, caching), re-evaluate on the same test dataset. Record the metrics and the average response latency.

**Step 4: Produce a comparison report.** Your report should look something like this:

```
Strategy         P@5    MRR    Avg Latency
──────────────────────────────────────────
Basic RAG        0.62   0.71   850ms
+ Expansion      0.71   0.78   1200ms
+ Re-ranking     0.79   0.85   1450ms
+ Hybrid         0.81   0.87   1100ms
+ Cache (avg)    0.81   0.87   320ms
```

Think about the trade-offs. Re-ranking improved precision by 0.08 but added 250ms of latency — is that worth it for your application? Caching didn't improve precision at all but cut average latency by 75% — is that the right investment? These are the kinds of engineering decisions you'll make constantly in production.
::::

::::challenge{id=m3-05-checkpoint title="Project Checkpoint 3: Optimise Your RAG System"}

Enhance your capstone project with at least two of the advanced techniques from this module. Create an evaluation test set of at least 20 queries and measure the impact of each optimisation.

Your deliverables should include:

- Implementation of at least two advanced techniques (query expansion, re-ranking, hybrid search, or semantic caching)
- An evaluation test dataset (20+ queries with expected retrieval targets)
- A comparison report documenting baseline metrics, metrics after each optimisation, latency and cost analysis, and your recommendation for which techniques to deploy and why

This checkpoint mirrors the work of a production ML engineer: implement improvements, measure their impact, and make data-driven recommendations.
::::

## Summary and What's Next

In this module, you've learned how to take a basic RAG system and make it production-grade. Agentic RAG handles complex and ambiguous queries through query expansion and decomposition. Re-ranking improves precision by using cross-encoders to refine vector search results. Hybrid search combines the semantic understanding of dense retrieval with the exact-matching power of BM25. Knowledge graphs complement vector search with structured relationship reasoning. Semantic caching reduces cost and latency by serving similar queries from cache. And retrieval evaluation gives you the metrics to make informed decisions about which techniques to use.

In Module 4, we turn our attention to conversation management. Long conversations quickly consume your context window, and naive approaches — like sending the entire history with every message — become expensive and eventually hit token limits. You'll learn sophisticated strategies for summarising conversation history, selectively retaining important context, implementing retrieval-based memory, and personalising responses based on what the chatbot has learned about the user over time.

## Additional Resources

- ["Lost in the Middle: How Language Models Use Long Contexts"](https://arxiv.org/abs/2307.03172) (Liu et al., 2023) — Important findings on how LLMs use context window position
- ["Precise Zero-Shot Dense Retrieval without Relevance Labels"](https://arxiv.org/abs/2212.10496) (Gao et al., 2021) — Foundational work on dense retrieval
- [Pinecone Blog: "Hybrid Search Explained"](https://www.pinecone.io/learn/hybrid-search-intro/) — Practical guide to combining dense and sparse search
- [sentence-transformers](https://www.sbert.net/) — Library for cross-encoder re-ranking models
- [rank-bm25](https://github.com/dorianbrown/rank_bm25) — BM25 implementation for Python
- [NetworkX](https://networkx.org/) — Graph manipulation library
- [ragas](https://github.com/explodinggradients/ragas) — RAG evaluation framework

## Hands-on practical

Work through the accompanying notebook: [`module_3/05-evaluating-and-optimising-retrieval.ipynb`](https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules/blob/main/module_3/05-evaluating-and-optimising-retrieval.ipynb). It walks through a tiny labelled test set, Precision@K / Recall@K / MRR implemented from scratch, and a comparison framework that evaluates basic vs query-expanded retrieval.
