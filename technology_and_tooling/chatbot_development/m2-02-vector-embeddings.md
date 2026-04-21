---
name: Vector Embeddings
dependsOn:
  - technology_and_tooling.chatbot_development.m2-01-the-rag-paradigm
tags: [chatbots, rag, embeddings]
learningOutcomes:
  - Explain what vector embeddings are and how they capture semantic meaning.
  - Create embeddings using the OpenAI Embeddings API.
  - Measure similarity between embeddings using cosine similarity.
  - Understand the practical implications of embedding dimensions and model choice.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    image: https://github.com/OxfordCompetencyCenters.png
    license: CC-BY-4.0
---

The entire RAG pipeline depends on one fundamental capability: converting text into numbers in a way that preserves meaning. That's what vector embeddings do, and understanding them is essential for building effective retrieval systems.

## What Are Vector Embeddings?

An embedding is a dense vector — a list of floating-point numbers — that represents the semantic meaning of a piece of text. When you pass a sentence through an embedding model, you get back something like this:

```
Text: "The cat sat on the mat"
Embedding: [0.02, -0.15, 0.43, 0.08, ..., 0.21]
            └─────────── 1536 dimensions ──────────┘
```

Each number in the vector captures some aspect of the text's meaning. No single dimension corresponds to a human-readable concept like "about animals" or "describes a location" — the meaning is distributed across all dimensions. But the remarkable property of these vectors is that **texts with similar meanings produce similar vectors**. The embedding for "a feline rested on the rug" will be very close to the embedding for "the cat sat on the mat", even though the two sentences share almost no words.

This is what makes embeddings useful for search. Traditional keyword search fails when users use different words than the documents. If your documentation says "automobile" and the user searches for "car", a keyword search finds nothing. With embeddings, those two words map to nearby points in vector space, and the search succeeds.

## How Embeddings Capture Semantics

Embedding models learn semantic relationships through exposure to enormous amounts of text during training. The resulting vector space has several powerful properties.

**Synonyms map to similar vectors.** Words and phrases with the same meaning end up close together:

```python
"happy" → [0.2, 0.8, 0.1, ...]
"joyful" → [0.21, 0.79, 0.11, ...]
# Cosine similarity: 0.92 — very similar
```

**Context changes the embedding.** Modern embedding models are contextual, meaning the same word gets different embeddings depending on how it's used:

```python
"bank" in "river bank" → [0.5, 0.2, -0.3, ...]
"bank" in "bank account" → [-0.2, 0.7, 0.4, ...]
# Different meanings produce different vectors
```

**Semantic relationships are preserved geometrically.** The famous example is that `vector("king") - vector("man") + vector("woman") ≈ vector("queen")`. The direction from "man" to "king" (roughly: "add royalty") is the same as the direction from "woman" to "queen". This is a simplified illustration, but it demonstrates that embeddings capture meaningful relationships, not just surface-level word patterns.

For RAG, this means that a user's question doesn't need to use the exact same words as the documents. "How do I reset my password?" will match a document titled "Account credential recovery procedure" because the underlying meaning is similar, even though the phrasing is completely different.

## Creating Embeddings with the OpenAI API

OpenAI provides several embedding models with different trade-offs:

| Model | Dimensions | Cost (per 1M tokens) | Use Case |
|-------|------------|---------------------|----------|
| text-embedding-3-small | 1536 | $0.02 | Most use cases |
| text-embedding-3-large | 3072 | $0.13 | Higher accuracy needed |
| text-embedding-ada-002 | 1536 | $0.10 | Legacy (still good) |

For most RAG applications, `text-embedding-3-small` is the best choice. It's inexpensive and produces high-quality embeddings. Only use `text-embedding-3-large` if you've tested both and found that the larger model produces meaningfully better retrieval for your specific data.

Here's how to create an embedding for a single text:

```python
import openai
import os

openai.api_key = os.getenv("OPENAI_API_KEY")

response = openai.Embedding.create(
    model="text-embedding-3-small",
    input="The quick brown fox jumps over the lazy dog"
)

embedding = response['data'][0]['embedding']
print(f"Embedding dimensions: {len(embedding)}")   # 1536
print(f"First 5 values: {embedding[:5]}")
```

When you need to embed multiple texts — which you will, frequently, during document indexing — batch them into a single API call. This is significantly more efficient than making one call per text:

```python
texts = [
    "Python is a programming language",
    "Java is also a programming language",
    "I love pizza"
]

response = openai.Embedding.create(
    model="text-embedding-3-small",
    input=texts
)

embeddings = [item['embedding'] for item in response['data']]
```

The API can handle up to 2,048 texts in a single batch. For document indexing, you'll typically process your chunks in batches of 100–500 to stay comfortably within limits while maximising throughput.

One critical rule: **always use the same embedding model for documents and queries.** If you embed your documents with `text-embedding-3-small` and then embed your user queries with `text-embedding-3-large`, the resulting vectors live in different vector spaces and comparing them is meaningless. Pick one model and use it consistently.

## Measuring Similarity

Once you have embeddings, you need a way to measure how similar two vectors are. The most common metric for text embeddings is **cosine similarity**, which measures the angle between two vectors on a scale from -1 (opposite directions) to 1 (same direction).

```python
import numpy as np

def cosine_similarity(vec1, vec2):
    """Measures the angle between two vectors. Range: -1 to 1."""
    dot_product = np.dot(vec1, vec2)
    norm1 = np.linalg.norm(vec1)
    norm2 = np.linalg.norm(vec2)
    return dot_product / (norm1 * norm2)
```

Cosine similarity is the standard choice for text embeddings because it measures direction rather than magnitude. Two vectors pointing in the same direction are considered similar regardless of their length. This matches how text embeddings work — the direction captures meaning, while the magnitude is less meaningful.

Two other metrics you'll encounter are **Euclidean distance**, which measures the straight-line distance between two points in vector space (smaller means more similar), and **dot product**, which is computationally fast and equivalent to cosine similarity when the vectors are normalised.

```python
def euclidean_distance(vec1, vec2):
    """Measures straight-line distance. Range: 0 to infinity."""
    return np.linalg.norm(np.array(vec1) - np.array(vec2))

def dot_product_similarity(vec1, vec2):
    """Simple multiplication and sum."""
    return np.dot(vec1, vec2)
```

In practice, you rarely compute these yourself. Vector databases handle similarity calculations internally. But understanding the metrics matters when you're configuring your database and interpreting results. For text embeddings, default to cosine similarity unless you have a specific reason to choose something else.

::::challenge{id=m2-02-explore title="Explore Embeddings"}

To build intuition for how embeddings work, try the following experiments:

1. Embed a set of sentences that are semantically similar but use different words (e.g., "The car is fast" and "The automobile moves quickly"). Compute their cosine similarity. Then embed a semantically different sentence (e.g., "I enjoy reading books") and compare. How different are the similarity scores?

2. Embed the same word in two different contexts (e.g., "I deposited money at the bank" and "I sat on the river bank"). Are the embeddings different? How different?

3. Embed a short sentence and a long paragraph that discuss the same topic. How does length affect the similarity?

```python
import openai
import numpy as np
import os

openai.api_key = os.getenv("OPENAI_API_KEY")

def get_embedding(text):
    response = openai.Embedding.create(
        model="text-embedding-3-small",
        input=text
    )
    return response['data'][0]['embedding']

def cosine_sim(a, b):
    return np.dot(a, b) / (np.linalg.norm(a) * np.linalg.norm(b))

# Experiment 1: Semantic similarity with different words
emb1 = get_embedding("The car is fast")
emb2 = get_embedding("The automobile moves quickly")
emb3 = get_embedding("I enjoy reading books")

print(f"Similar meaning, different words: {cosine_sim(emb1, emb2):.3f}")
print(f"Different meaning: {cosine_sim(emb1, emb3):.3f}")

# Try your own experiments with contexts and lengths...
```

These experiments will give you a concrete feel for what embeddings capture and where they might surprise you. This intuition is valuable when debugging retrieval quality later.
::::

## Hands-on practical

Work through the accompanying notebook: [`module_2/02-vector-embeddings.ipynb`](https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules/blob/main/module_2/02-vector-embeddings.ipynb). It walks through creating embeddings, computing cosine similarity, contextual meaning for the same word in different sentences, and how length affects similarity.
