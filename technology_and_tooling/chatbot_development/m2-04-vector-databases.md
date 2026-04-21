---
name: Vector Databases
dependsOn:
  - technology_and_tooling.chatbot_development.m2-03-document-processing-and-chunking
tags: [chatbots, rag, databases]
learningOutcomes:
  - Explain why vector databases are needed for RAG at scale.
  - Set up ChromaDB and perform basic operations (add, query, filter).
  - Integrate ChromaDB with OpenAI embeddings for semantic search.
  - Understand the landscape of vector database options for production use.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    image: https://github.com/OxfordCompetencyCenters.png
    license: CC-BY-4.0
---

You now have chunks with embeddings. The next question is: where do you store them, and how do you search through them efficiently? For a handful of documents, you could compute cosine similarity against every chunk on every query. But for thousands or millions of chunks, that brute-force approach is far too slow. Vector databases solve this problem with specialised indexing algorithms that make similarity search fast even at massive scale.

## Why Vector Databases?

A vector database is a specialised storage system designed for storing, indexing, and searching vector embeddings. Unlike traditional databases that search by exact matches or range queries on structured fields, vector databases find the nearest neighbours to a query vector in high-dimensional space.

The core challenge is speed. If you have a million document chunks, each represented by a 1,536-dimensional vector, a brute-force search computes 1 million cosine similarities for every query. That's feasible but slow — and it gets worse as your corpus grows. Vector databases use approximate nearest neighbour (ANN) algorithms, such as HNSW (Hierarchical Navigable Small World graphs), to find similar vectors in logarithmic time rather than linear time. The results are slightly approximate — you might miss the absolute best match occasionally — but the speed improvement is dramatic, and for RAG purposes the approximation is more than good enough.

Beyond fast search, vector databases provide metadata filtering (combine vector similarity with structured filters), CRUD operations (add, update, and delete vectors as your corpus changes), persistence (data survives restarts), and scalability (handle millions or billions of vectors).

The landscape of vector databases has grown rapidly. Here are the main options:

| Database | Type | Best For |
|----------|------|----------|
| ChromaDB | Local/Embedded | Development, prototyping, small scale |
| Pinecone | Managed Cloud | Production at scale |
| Weaviate | Self-hosted/Cloud | Flexibility, open-source |
| Qdrant | Self-hosted/Cloud | High performance |
| Milvus | Self-hosted | Large scale, open-source |

For learning and development, ChromaDB is the ideal choice. It's free, runs locally with no infrastructure to manage, and has a clean, intuitive API. For production systems that need to handle millions of users and billions of vectors, a managed service like Pinecone removes the operational burden.

## Getting Started with ChromaDB

Install ChromaDB with pip:

```bash
pip install chromadb
```

ChromaDB organises data into **collections**, which are analogous to tables in a relational database. Each collection stores vectors alongside their original text and metadata. Here's a basic example that creates a collection, adds some documents, and searches:

```python
import chromadb

# Initialize a persistent client (data saved to disk)
client = chromadb.PersistentClient(path="./chroma_db")

# Create a collection
collection = client.create_collection(
    name="my_documents",
    metadata={"description": "My document embeddings"}
)

# Add documents with pre-computed embeddings
collection.add(
    ids=["doc1", "doc2", "doc3"],
    embeddings=[
        [0.1, 0.2, 0.3],
        [0.2, 0.3, 0.4],
        [0.5, 0.6, 0.7]
    ],
    documents=[
        "This is document 1",
        "This is document 2",
        "This is document 3"
    ],
    metadatas=[
        {"source": "file1.txt", "page": 1},
        {"source": "file1.txt", "page": 2},
        {"source": "file2.txt", "page": 1}
    ]
)

# Search for similar documents
results = collection.query(
    query_embeddings=[[0.15, 0.25, 0.35]],
    n_results=2
)

print("Top results:", results['documents'])
print("Metadata:", results['metadatas'])
print("Distances:", results['distances'])
```

A few things to note. The `PersistentClient` saves data to disk at the specified path, so your data survives when you restart your script. Each document gets a unique ID — if you try to add a document with an ID that already exists, ChromaDB will raise an error (use `upsert` instead if you want to update). The `documents` field stores the original text alongside the embedding, which is crucial for RAG since you need the actual text to include in the LLM prompt. And the `metadatas` field accepts any dictionary of JSON-serializable values.

If you want to avoid re-creating a collection that already exists, use `get_or_create_collection` instead of `create_collection`:

```python
collection = client.get_or_create_collection(name="my_documents")
```

## Integrating ChromaDB with OpenAI Embeddings

In practice, you won't pre-compute embeddings by hand. You'll generate them from the OpenAI API and store them in ChromaDB. Here's a complete integration that provides two essential operations: indexing documents and searching for relevant ones.

```python
import chromadb
import openai
import os

openai.api_key = os.getenv("OPENAI_API_KEY")

client = chromadb.PersistentClient(path="./chroma_db")
collection = client.get_or_create_collection(name="documents")

def add_documents(documents, metadatas=None):
    """Embed documents and add them to ChromaDB."""
    response = openai.Embedding.create(
        model="text-embedding-3-small",
        input=documents
    )
    embeddings = [item['embedding'] for item in response['data']]

    ids = [f"doc_{i}" for i in range(len(documents))]

    collection.add(
        ids=ids,
        embeddings=embeddings,
        documents=documents,
        metadatas=metadatas or [{}] * len(documents)
    )
    print(f"Added {len(documents)} documents")

def search_documents(query, top_k=3):
    """Embed a query and search for similar documents."""
    response = openai.Embedding.create(
        model="text-embedding-3-small",
        input=query
    )
    query_embedding = response['data'][0]['embedding']

    results = collection.query(
        query_embeddings=[query_embedding],
        n_results=top_k
    )
    return results

# Index some documents
docs = [
    "Python is a high-level programming language.",
    "JavaScript is used for web development.",
    "Machine learning is a subset of artificial intelligence."
]
add_documents(docs)

# Search
results = search_documents("What is Python?")
print("Top result:", results['documents'][0])
```

This is the core of a RAG retrieval system. You embed documents once during indexing, then embed each user query at search time and find the most similar stored documents. ChromaDB handles the similarity calculation internally using cosine distance by default.

## Choosing a Similarity Metric

ChromaDB supports three similarity metrics, configured at collection creation time:

```python
# Cosine similarity (default) — measures angle between vectors
collection = client.create_collection(
    name="docs_cosine",
    metadata={"hnsw:space": "cosine"}
)

# Euclidean distance (L2) — measures straight-line distance
collection = client.create_collection(
    name="docs_l2",
    metadata={"hnsw:space": "l2"}
)

# Inner product (dot product) — fast, best with normalized embeddings
collection = client.create_collection(
    name="docs_ip",
    metadata={"hnsw:space": "ip"}
)
```

For text embeddings from OpenAI, cosine similarity is the right default. It focuses on the direction of vectors (which captures meaning) rather than their magnitude. The differences between metrics are usually minor for well-trained embeddings, so don't spend much time optimising this unless you've identified it as a bottleneck in retrieval quality.

## A Note on Production: Pinecone and Other Managed Services

ChromaDB is excellent for development and small-scale applications, but production systems with millions of users and billions of vectors typically need a managed service. Pinecone is the most widely used option. It handles infrastructure, scaling, backups, and monitoring so you can focus on your application logic.

The API is conceptually similar to ChromaDB:

```python
import pinecone

pinecone.init(api_key="your-api-key", environment="us-west1-gcp")

pinecone.create_index(
    name="documents",
    dimension=1536,
    metric="cosine"
)

index = pinecone.Index("documents")

# Upsert vectors
index.upsert(vectors=[
    ("doc1", [0.1, 0.2, ...], {"text": "Document 1", "source": "file1.txt"}),
    ("doc2", [0.3, 0.4, ...], {"text": "Document 2", "source": "file2.txt"})
])

# Query
results = index.query(vector=[0.15, 0.25, ...], top_k=3, include_metadata=True)
```

The core operations — upsert vectors, query by similarity, filter by metadata — are the same across all vector databases. The differences are in scaling, pricing, and operational features. For this course, we'll use ChromaDB throughout. If you later move to Pinecone, Weaviate, or Qdrant, the concepts transfer directly; only the API details change.

::::challenge{id=m2-04-semantic-search title="Build a Semantic Search System"}

Put the pieces together by building a semantic search system from scratch. Create a ChromaDB collection, index a set of documents with OpenAI embeddings, and test the search with various queries.

```python
import chromadb
import openai
import os

openai.api_key = os.getenv("OPENAI_API_KEY")
client = chromadb.PersistentClient(path="./exercise_chroma_db")
collection = client.get_or_create_collection("exercise_docs")

# 1. Prepare a set of documents about different topics
documents = [
    "Python was created by Guido van Rossum and first released in 1991.",
    "Lists in Python are ordered, mutable sequences that can hold any type of object.",
    "A Python dictionary stores key-value pairs and provides fast lookup by key.",
    "Functions in Python are defined using the def keyword followed by the function name.",
    "Classes in Python support inheritance, allowing child classes to extend parent classes.",
    "Exception handling in Python uses try, except, and finally blocks.",
]

metadatas = [
    {"source": "python_history.txt", "topic": "history"},
    {"source": "python_collections.txt", "topic": "data structures"},
    {"source": "python_collections.txt", "topic": "data structures"},
    {"source": "python_functions.txt", "topic": "functions"},
    {"source": "python_oop.txt", "topic": "classes"},
    {"source": "python_errors.txt", "topic": "error handling"},
]

# 2. TODO: Embed and index these documents

# 3. TODO: Search with these queries and verify the results make sense:
test_queries = [
    "Who created Python?",
    "How do I store key-value data?",
    "How do I handle errors in my code?",
]

# 4. TODO: Try a query with metadata filtering (e.g., only "data structures" topic)
```

After implementing the basic search, experiment with queries that use different phrasing from the documents. Does "error handling" match the document about exceptions? Does "inventor of Python" match the document about Guido van Rossum? Exploring these edge cases builds your intuition for how semantic search behaves in practice.
::::

## Hands-on practical

Work through the accompanying notebook: [`module_2/04-vector-databases.ipynb`](https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules/blob/main/module_2/04-vector-databases.ipynb). It walks through creating a persistent ChromaDB collection, adding documents with OpenAI embeddings, performing similarity search, and applying metadata filters.
