---
name: Re-ranking and Hybrid Search
dependsOn:
  - technology_and_tooling.chatbot_development.m3-01-agentic-rag
tags: [chatbots, rag, search]
learningOutcomes:
  - Explain why vector search alone can produce imprecise results and how re-ranking helps.
  - Implement cross-encoder re-ranking using a two-stage retrieval pipeline.
  - Understand the difference between dense (semantic) and sparse (keyword) retrieval.
  - Build a hybrid search system that combines BM25 with vector search.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    license: CC-BY-4.0
---

Agentic RAG improves retrieval by generating better queries. The two techniques in this section — re-ranking and hybrid search — improve retrieval by making the search and ranking process itself more sophisticated.

## The Precision Problem in Vector Search

Vector search is fast and captures semantic meaning, but its ranking isn't always precise. There are several reasons for this.

First, vector databases use **approximate** nearest neighbour algorithms. They sacrifice a small amount of accuracy for dramatic speed improvements. The top result from an approximate search isn't always the true nearest neighbour — it's usually close, but "close" isn't always good enough.

Second, each document chunk is compressed into a single embedding vector. All the nuance of a 500-word passage is reduced to 1,536 numbers. This compression inevitably loses fine-grained relevance signals. Two passages might have similar embeddings because they discuss the same general topic, even though one directly answers the user's question and the other is tangentially related.

Third, embedding similarity doesn't always correspond to relevance. "Python tutorial" and "Python snake" might have somewhat similar embeddings because they share the word "Python", even though they're about completely different topics.

The solution is a **two-stage retrieval** pipeline. In the first stage, vector search casts a wide net and retrieves a large set of candidates — say, the top 20. This stage prioritises recall: find everything that might be relevant. In the second stage, a more sophisticated model examines each candidate individually and re-scores it for relevance to the specific query. This stage prioritises precision: among the candidates, which are truly the best matches? Only the top 3–5 re-ranked results are passed to the LLM.

## Cross-Encoder Re-ranking

A cross-encoder is a model specifically trained to score the relevance of a (query, document) pair. Unlike the bi-encoder approach used for embeddings — where the query and document are embedded separately and then compared — a cross-encoder processes the query and document together, allowing it to capture fine-grained interactions between them. This makes it significantly more accurate, but also significantly slower. That's why it's used only on a small set of pre-filtered candidates, not the entire corpus.

Here's an implementation using the `sentence-transformers` library:

```python
from sentence_transformers import CrossEncoder

class RerankedRAG:
    def __init__(self, collection):
        self.collection = collection
        self.reranker = CrossEncoder('cross-encoder/ms-marco-MiniLM-L-6-v2')

    def retrieve_and_rerank(self, query, initial_k=20, final_k=5):
        """Two-stage retrieval: vector search then cross-encoder re-ranking."""

        # Stage 1: Cast a wide net with vector search
        candidates = self.retrieve_documents(query, top_k=initial_k)

        # Stage 2: Score each candidate against the query
        pairs = [[query, doc['text']] for doc in candidates]
        scores = self.reranker.predict(pairs)

        # Attach scores and sort
        for doc, score in zip(candidates, scores):
            doc['rerank_score'] = float(score)

        reranked = sorted(candidates, key=lambda x: x['rerank_score'], reverse=True)

        return reranked[:final_k]

    def chat(self, user_query):
        """RAG with re-ranking."""
        documents = self.retrieve_and_rerank(user_query)

        context = "\n\n".join([doc['text'] for doc in documents])

        prompt = f"""Context:\n{context}\n\nQuestion: {user_query}\n\nAnswer:"""

        response = openai.ChatCompletion.create(
            model="gpt-3.5-turbo",
            messages=[
                {"role": "system", "content": "Answer based on the provided context."},
                {"role": "user", "content": prompt}
            ]
        )

        return response.choices[0].message.content
```

To use this, install the required library:

```bash
pip install sentence-transformers
```

Several pre-trained cross-encoder models are available, trained on the MS MARCO search relevance dataset. They differ in speed and accuracy:

- `cross-encoder/ms-marco-MiniLM-L-6-v2` — fast, good quality (recommended starting point)
- `cross-encoder/ms-marco-TinyBERT-L-2-v2` — faster, slightly lower quality
- `cross-encoder/ms-marco-electra-base` — slower, highest quality

In practice, re-ranking typically improves Precision@5 by 15–40%. The added latency is modest because you're only scoring 20 candidates, not millions. For most production RAG systems, the quality improvement is well worth the cost.

## Dense vs Sparse Retrieval

So far, all our retrieval has been **dense** — using vector embeddings to capture semantic similarity. But there's another approach that's been used in information retrieval for decades: **sparse** retrieval, commonly implemented as BM25 (a refinement of TF-IDF).

Dense retrieval represents text as dense vectors where every dimension has a value. It excels at semantic understanding: "automobile" and "car" have similar embeddings even though they share no characters. But it can struggle with exact matches. If a user searches for product code "XR-7821", dense search might return documents about products in general rather than the specific product, because the embedding doesn't strongly encode the exact string.

Sparse retrieval represents text as sparse vectors where most dimensions are zero. Each dimension corresponds to a word in the vocabulary, and the value reflects how important that word is in the document (based on frequency and rarity). Sparse retrieval excels at exact keyword matching: "XR-7821" will match documents containing exactly that string. But it fails on synonyms: "automobile" and "car" are completely different words and won't match.

The two approaches are complementary. Dense search understands meaning; sparse search finds exact terms. For most production systems, combining them — hybrid search — outperforms either approach alone.

## Implementing Hybrid Search

Hybrid search runs both dense (vector) and sparse (BM25) retrieval on every query, then combines the results using a weighted score. The `alpha` parameter controls the balance between the two approaches.

```python
from rank_bm25 import BM25Okapi
import numpy as np

class HybridRAG:
    def __init__(self, collection):
        self.collection = collection
        self.documents = []
        self.bm25 = None

    def index_documents(self, documents):
        """Index documents for both vector and BM25 search."""
        self.documents = documents

        # Vector indexing happens via ChromaDB (as before)
        # ...

        # BM25 indexing
        tokenized_docs = [doc.lower().split() for doc in documents]
        self.bm25 = BM25Okapi(tokenized_docs)

    def hybrid_search(self, query, top_k=5, alpha=0.5):
        """
        Combine vector and BM25 search.

        alpha=1.0 means pure dense search.
        alpha=0.0 means pure sparse search.
        alpha=0.5 means equal weight.
        """
        # Dense retrieval
        vector_results = self.collection.query(
            query_embeddings=[self.embed_query(query)],
            n_results=top_k * 2
        )

        # Sparse retrieval
        tokenized_query = query.lower().split()
        bm25_scores = self.bm25.get_scores(tokenized_query)

        # Normalise both score sets to [0, 1]
        vector_scores_norm = self._normalize(vector_results['distances'][0])
        bm25_scores_norm = self._normalize(bm25_scores)

        # Combine with weighted sum
        combined_scores = {}
        for idx, doc in enumerate(self.documents):
            combined = (
                alpha * vector_scores_norm[idx] +
                (1 - alpha) * bm25_scores_norm[idx]
            )
            combined_scores[idx] = combined

        # Return top K
        top_indices = sorted(
            combined_scores.keys(),
            key=lambda i: combined_scores[i],
            reverse=True
        )[:top_k]

        return [self.documents[i] for i in top_indices]

    def _normalize(self, scores):
        """Normalise scores to [0, 1] range."""
        scores = np.array(scores)
        if scores.max() == scores.min():
            return scores
        return (scores - scores.min()) / (scores.max() - scores.min())
```

Install BM25 with:

```bash
pip install rank-bm25
```

The `alpha` parameter is the key tuning knob. For queries that are likely semantic in nature ("How do I handle errors in my code?"), set alpha higher (0.7–0.9) to weight dense search more heavily. For queries that contain specific identifiers or technical terms ("bug #12345", "API endpoint /v2/users"), set alpha lower (0.2–0.3) to weight keyword matching more heavily. For general queries, 0.5 is a reasonable default.

In a sophisticated system, you could even choose alpha dynamically based on the query. If the query contains tokens that look like identifiers (numbers, codes, URLs), reduce alpha. If it's a natural language question, increase alpha. This kind of adaptive hybrid search is used in production by companies like Notion and Stripe.

::::challenge{id=m3-02-compare title="Implement and Compare Retrieval Strategies"}

Starting from your Module 2 RAG system, add re-ranking and hybrid search, then compare all approaches.

**Step 1: Add re-ranking.** Retrieve 20 candidates with vector search, re-rank them with a cross-encoder, and return the top 5. Compare Precision@5 against your baseline.

```python
from sentence_transformers import CrossEncoder

# TODO: Implement retrieve_and_rerank
# Retrieve top 20 candidates, re-rank with cross-encoder, return top 5

# Compare against baseline
print(f"Baseline P@5: {baseline_precision:.2f}")
print(f"Reranked P@5: {reranked_precision:.2f}")
```

**Step 2: Add hybrid search.** Index your documents for BM25 alongside the existing vector index. Implement the hybrid search function and experiment with different alpha values.

```python
from rank_bm25 import BM25Okapi

# TODO: Implement hybrid search with configurable alpha

# Find the best alpha
for alpha in [0.0, 0.3, 0.5, 0.7, 1.0]:
    precision = evaluate_with_alpha(alpha)
    print(f"Alpha {alpha}: P@5 = {precision:.2f}")
```

**Step 3: Combine everything.** Use hybrid search to retrieve candidates, then re-rank. Does combining both techniques outperform either alone?
::::

## Hands-on practical

Work through the accompanying notebook: [`module_3/02-reranking-and-hybrid-search.ipynb`](https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules/blob/main/module_3/02-reranking-and-hybrid-search.ipynb). It walks through vector search as a baseline, an LLM-based re-ranker on top of it, BM25 keyword search with `rank-bm25`, and a tunable hybrid combination.
