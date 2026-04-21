---
name: Agentic RAG
dependsOn:
  - technology_and_tooling.chatbot_development.m2-05-building-a-rag-chatbot
tags: [chatbots, rag, agents]
learningOutcomes:
  - Identify the limitations of single-step retrieval for complex queries.
  - Implement query expansion to improve retrieval recall.
  - Use query decomposition to handle multi-part questions.
  - Build a multi-step retrieval pipeline with deduplication and ranking.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    image: https://github.com/OxfordCompetencyCenters.png
    license: CC-BY-4.0
---

The basic RAG system you built in Module 2 follows a straightforward pipeline: take the user's query, embed it, search the vector database, retrieve the top results, and generate a response. This works well for simple, well-phrased questions. But real users ask complex, ambiguous, and multi-part questions that a single retrieval step often can't handle. Agentic RAG addresses this by making the retrieval process iterative and intelligent.

## The Limitations of Single-Step Retrieval

Consider what happens when a user asks "Compare Python and JavaScript for web development." A single-step retrieval embeds this entire question as one vector and searches for similar chunks. The results might be documents that mention both Python and JavaScript together, but they're unlikely to include the best documents about Python web frameworks *and* the best documents about JavaScript web frameworks separately. The query is too broad for a single embedding to capture both facets.

The problem gets worse with ambiguous queries. If a user in a multi-turn conversation asks "How do I use it?", single-step retrieval has no way to resolve what "it" refers to without conversation context. It will embed the literal words and return generic or irrelevant results.

Multi-hop questions pose yet another challenge. "What framework does the creator of Python recommend for web development?" requires two retrieval steps: first, find out who created Python (Guido van Rossum), then search for his framework recommendations. No single embedding can capture both hops.

Finally, there's the vocabulary mismatch problem. If the user asks about "handling errors" but your documents use the term "exception management", the embeddings might not be close enough for a strong match. A single retrieval gives you one shot to find the right documents.

Agentic RAG solves these problems by using the LLM itself to reformulate queries, break complex questions into sub-questions, and perform multiple retrieval steps that are then combined intelligently.

## Query Expansion

Query expansion generates multiple variations of the user's original question, each phrased differently. By searching with several variations, you're more likely to hit documents that use different terminology.

```python
def expand_query(original_query):
    """Generate multiple phrasings of the same question."""
    prompt = f"""Given this user question, generate 3 alternative phrasings
    that preserve the meaning but use different words.

    Original: {original_query}

    Generate 3 variations:"""

    response = openai.ChatCompletion.create(
        model="gpt-3.5-turbo",
        messages=[{"role": "user", "content": prompt}],
        temperature=0.7
    )

    variations = response.choices[0].message.content.split('\n')
    return [original_query] + [v.strip() for v in variations if v.strip()]
```

For the query "How do I handle errors in Python?", this might produce:

```
["How do I handle errors in Python?",
 "What is exception handling in Python?",
 "How to catch and manage exceptions in Python?",
 "Python error and exception management techniques"]
```

Now you search the vector database with all four queries. A document about "exception handling" that might not have matched the original phrasing will likely match one of the expanded versions. The cost is additional LLM and embedding calls, but for high-value queries the quality improvement is significant.

## Query Decomposition

For complex questions that involve multiple distinct sub-topics, decomposition is more appropriate than expansion. Instead of rephrasing the same question, decomposition breaks it into independent sub-questions that can each be answered by retrieving different documents.

```python
def decompose_query(complex_query):
    """Break a complex question into simpler sub-questions."""
    prompt = f"""Break this complex question into simpler sub-questions
    that can each be answered independently:

    Complex Question: {complex_query}

    Sub-questions:"""

    response = openai.ChatCompletion.create(
        model="gpt-3.5-turbo",
        messages=[{"role": "user", "content": prompt}],
        temperature=0.3
    )

    subquestions = response.choices[0].message.content.split('\n')
    return [q.strip() for q in subquestions if q.strip()]
```

For "Compare Python and JavaScript for building web applications", decomposition might produce:

```
["What are Python's features for web development?",
 "What are JavaScript's features for web development?",
 "What are common Python web frameworks?",
 "What are common JavaScript web frameworks?"]
```

Each sub-question targets a different set of documents, and together they cover the breadth of the original question. The LLM can then synthesise the retrieved results into a coherent comparison.

## Building a Multi-Step Retrieval Pipeline

The following implementation brings query expansion, multiple retrievals, deduplication, and frequency-based ranking together into a single class:

```python
class AgenticRAG:
    def __init__(self, collection):
        self.collection = collection

    def agentic_retrieve(self, query, strategy="expand"):
        """Multi-step retrieval with query expansion or decomposition."""
        if strategy == "expand":
            queries = self.expand_query(query)
        elif strategy == "decompose":
            queries = self.decompose_query(query)
        else:
            queries = [query]

        # Retrieve for each query variant
        all_results = []
        for q in queries:
            results = self.retrieve_documents(q, top_k=3)
            all_results.extend(results)

        # Deduplicate and rank
        unique_results = self.deduplicate(all_results)
        ranked_results = self.rank_by_frequency(unique_results)

        return ranked_results[:5]

    def deduplicate(self, documents):
        """Remove duplicate documents that appeared across multiple queries."""
        seen = set()
        unique = []
        for doc in documents:
            doc_id = doc.get('id') or hash(doc['text'])
            if doc_id not in seen:
                seen.add(doc_id)
                unique.append(doc)
        return unique

    def rank_by_frequency(self, documents):
        """Rank documents by how often they appeared across query variants."""
        from collections import Counter
        doc_counts = Counter([doc['id'] for doc in documents])
        return sorted(documents, key=lambda doc: doc_counts[doc['id']], reverse=True)

    def chat(self, user_query):
        """Answer using agentic retrieval."""
        documents = self.agentic_retrieve(user_query, strategy="expand")
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

The frequency-based ranking is a simple but effective heuristic: documents that appear in the results for multiple query variants are likely more relevant than documents that appear for only one. If a document about Python exception handling is retrieved by "handle errors", "exception handling", and "catch exceptions", it's almost certainly a good match.

In production, you'd choose between expansion and decomposition based on the query. Simple questions benefit from expansion (same question, different words). Complex or multi-part questions benefit from decomposition (different questions entirely). You could even use the LLM to classify the query type before deciding which strategy to use.

The trade-off is cost and latency. Each expanded or decomposed query requires an embedding call and a vector search. A query that expands into four variants takes roughly four times as long as a single retrieval. For latency-sensitive applications, you might reserve agentic retrieval for queries where basic retrieval returns low-confidence results, and use single-step retrieval for straightforward questions.

## Hands-on practical

Work through the accompanying notebook: [`module_3/01-agentic-rag.ipynb`](https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules/blob/main/module_3/01-agentic-rag.ipynb). It walks through query expansion, query decomposition, and a multi-step retrieval pipeline with deduplication plus frequency ranking.
