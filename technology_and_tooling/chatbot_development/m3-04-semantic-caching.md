---
name: Semantic Caching
dependsOn:
  - technology_and_tooling.chatbot_development.m3-03-knowledge-graphs
tags: [chatbots, rag, caching, optimization]
learningOutcomes:
  - Explain why traditional exact-match caching is insufficient for chatbot queries.
  - Implement a semantic cache that matches similar (not identical) queries.
  - Configure similarity thresholds and TTL for cache accuracy and freshness.
  - Integrate semantic caching into a RAG pipeline to reduce cost and latency.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    image: https://github.com/OxfordCompetencyCenters.png
    license: CC-BY-4.0
---

Every RAG query involves at least three expensive operations: embedding the query, searching the vector database, and calling the LLM to generate a response. In a production system handling thousands of queries per day, these costs add up. Semantic caching can reduce them dramatically by storing responses and returning them for similar future queries — without repeating the full retrieval and generation pipeline.

## The Problem with Exact-Match Caching

Traditional caching stores a response for a specific input and returns it when the exact same input appears again:

```python
cache = {}
if query in cache:
    return cache[query]
else:
    result = expensive_rag_call(query)
    cache[query] = result
    return result
```

This works for APIs where the input is deterministic, but chatbot queries are natural language. Users don't type the same question the same way twice. "How do I deploy my app?", "How to deploy application?", "Deployment instructions?", and "Steps to deploy" all ask for the same information, but none of them are exact string matches. An exact-match cache would miss all of these and make four separate RAG calls.

Semantic caching solves this by comparing the *meaning* of queries rather than their exact text. If a new query is semantically similar enough to a cached query, the cached response is returned. This can reduce LLM API calls by 40–60% in systems where users frequently ask about the same topics.

## Implementing a Semantic Cache

The implementation uses the same embeddings and vector search that power RAG itself. Instead of storing document chunks, the cache stores (query, response) pairs. When a new query arrives, it's embedded and compared against the cached query embeddings. If a close enough match is found (above a similarity threshold) and the cached entry hasn't expired (within the TTL), the cached response is returned directly.

```python
import chromadb
import openai
from datetime import datetime, timedelta

class SemanticCache:
    def __init__(self, similarity_threshold=0.95, ttl_hours=24):
        """
        similarity_threshold: Minimum cosine similarity for a cache hit.
            Higher values (0.97-0.99) mean fewer false positives but lower hit rate.
            Lower values (0.90-0.93) mean more hits but risk returning slightly
            off-target answers.
        ttl_hours: How long cache entries remain valid.
        """
        self.client = chromadb.PersistentClient(path="./cache_db")
        self.collection = self.client.get_or_create_collection("query_cache")
        self.similarity_threshold = similarity_threshold
        self.ttl = timedelta(hours=ttl_hours)

    def _embed_query(self, query):
        """Generate an embedding for a query."""
        response = openai.Embedding.create(
            model="text-embedding-3-small",
            input=query
        )
        return response['data'][0]['embedding']

    def find_similar(self, query):
        """Check if a similar query is already cached."""
        if self.collection.count() == 0:
            return None

        query_embedding = self._embed_query(query)

        results = self.collection.query(
            query_embeddings=[query_embedding],
            n_results=1
        )

        if not results['ids'][0]:
            return None

        # Check similarity threshold
        distance = results['distances'][0][0]
        similarity = 1 - distance

        if similarity >= self.similarity_threshold:
            # Check TTL
            metadata = results['metadatas'][0][0]
            cached_time = datetime.fromisoformat(metadata['timestamp'])

            if datetime.now() - cached_time < self.ttl:
                return results['ids'][0][0]

        return None

    def get(self, query_id):
        """Retrieve a cached response by query ID."""
        result = self.collection.get(ids=[query_id])
        if result['metadatas']:
            return result['metadatas'][0]['response']
        return None

    def store(self, query, response):
        """Store a query-response pair in the cache."""
        query_embedding = self._embed_query(query)

        self.collection.add(
            ids=[f"query_{hash(query)}"],
            embeddings=[query_embedding],
            documents=[query],
            metadatas=[{
                "response": response,
                "timestamp": datetime.now().isoformat()
            }]
        )
```

## Integrating the Cache into Your RAG Pipeline

Wrapping your existing RAG system with semantic caching requires only a thin layer:

```python
class CachedRAG:
    def __init__(self, rag_system, similarity_threshold=0.95, ttl_hours=24):
        self.rag = rag_system
        self.cache = SemanticCache(
            similarity_threshold=similarity_threshold,
            ttl_hours=ttl_hours
        )

    def chat(self, user_query):
        """Chat with semantic caching."""
        # Check cache first
        cached_query_id = self.cache.find_similar(user_query)

        if cached_query_id:
            return self.cache.get(cached_query_id)

        # Cache miss — generate a fresh response
        response = self.rag.chat(user_query)

        # Store in cache for future similar queries
        self.cache.store(user_query, response)

        return response
```

Usage is transparent to the rest of your code:

```python
cached_rag = CachedRAG(rag_system)

# First call — cache miss, full RAG pipeline runs
answer1 = cached_rag.chat("How do I deploy my app?")

# Similar query — cache hit, returns instantly
answer2 = cached_rag.chat("How to deploy application?")

# Different topic — cache miss, full RAG pipeline runs
answer3 = cached_rag.chat("What is Python?")
```

## Tuning the Cache

The two main parameters to tune are the **similarity threshold** and the **TTL**.

The similarity threshold controls how similar a new query must be to a cached query for a cache hit. Setting it too high (0.99) means the cache only matches near-identical queries, which gives you very few hits. Setting it too low (0.85) means the cache matches queries that are somewhat related but not really the same, which can return misleading answers. For most applications, 0.93–0.97 is the sweet spot. Start at 0.95 and adjust based on whether you're seeing false positives (wrong answers returned from cache) or a low hit rate.

The TTL controls how long cached entries remain valid. For documentation that changes infrequently, a longer TTL (24–48 hours) maximises cache utilisation. For rapidly changing data — news, stock prices, real-time dashboards — you need a short TTL (1–4 hours) or even no caching at all for certain query types.

In a production system, you'd also monitor your cache hit rate and the accuracy of cache hits. If users report incorrect answers that turn out to be stale cache entries, your TTL is too long. If your cache hit rate is below 20–30%, your threshold might be too strict or your user base might not have enough query overlap to benefit from caching.

The potential savings are substantial. Companies that have implemented semantic caching report 40–60% reduction in LLM API calls, 50–70% reduction in average response latency (cache hits return near-instantly), and proportional cost savings on embedding and LLM generation. These savings scale with traffic: the more users you have, the more likely any given query has a similar cached predecessor.

## Hands-on practical

Work through the accompanying notebook: [`module_3/04-semantic-caching.ipynb`](https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules/blob/main/module_3/04-semantic-caching.ipynb). It walks through the failure of exact-match caching, a semantic cache built from scratch, threshold tuning, and a `CachedRAG` wrapper around the full pipeline.
