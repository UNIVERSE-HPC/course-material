---
name: Knowledge Graphs
dependsOn:
  - technology_and_tooling.chatbot_development.m3-02-reranking-and-hybrid-search
tags: [chatbots, rag, knowledge-graphs]
learningOutcomes:
  - Explain what knowledge graphs are and when they complement vector search.
  - Build a simple knowledge graph using NetworkX.
  - Extract entities and relationships from text using LLMs.
  - Combine vector search with knowledge graph reasoning in a hybrid system.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    image: https://github.com/OxfordCompetencyCenters.png
    license: CC-BY-4.0
---

Vector search and BM25 are both good at finding documents that are *similar* to a query. But some questions aren't about similarity — they're about *relationships*. "Who created Python?" "What frameworks are built with Python?" "What programming languages were created by people who worked at Google?" These questions require traversing explicit connections between entities, not searching for similar text. That's where knowledge graphs come in.

## What Are Knowledge Graphs?

A knowledge graph represents information as a network of **entities** (things) connected by **relationships** (how those things relate to each other). Each fact in the graph is a triple: (entity, relationship, entity). For example:

```
(Python) --created_by--> (Guido van Rossum)
(Python) --released_in--> (1991)
(Python) --used_for--> (Web Development)
(Python) --used_for--> (Data Science)
(Guido van Rossum) --nationality--> (Dutch)
(Guido van Rossum) --worked_at--> (Google)
(Django) --built_with--> (Python)
(Django) --used_for--> (Web Development)
```

This structure makes it easy to answer relationship questions by traversing the graph. "Who created Python?" — follow the `created_by` edge from Python. "What frameworks use Python?" — find all nodes connected to Python by the `built_with` relationship. "What languages were created by people who worked at Google?" — find people with `worked_at → Google`, then follow their `created` edges. That last query requires two hops through the graph — something that vector search cannot do at all.

Knowledge graphs and vector databases serve different purposes:

| Aspect | Vector Database | Knowledge Graph |
|--------|----------------|-----------------|
| Data type | Unstructured text | Structured relationships |
| Search method | Semantic similarity | Graph traversal |
| Reasoning | Statistical | Explicit, rule-based |
| Best for | Finding similar content | Understanding relationships |

In practice, many production systems use both. Vector search handles "how-to" and "explain" questions where the answer lives in document text. Knowledge graphs handle "who", "when", "where", and "what relates to what" questions where the answer is a specific fact or a chain of relationships.

## Building a Knowledge Graph with NetworkX

For prototyping, Python's `networkx` library provides everything you need. In production, you'd use a dedicated graph database like Neo4j, Amazon Neptune, or TigerGraph, but the concepts are identical.

```python
import networkx as nx

class SimpleKnowledgeGraph:
    def __init__(self):
        self.graph = nx.DiGraph()  # Directed graph

    def add_entity(self, entity, entity_type=None, properties=None):
        """Add an entity (node) to the graph."""
        self.graph.add_node(
            entity,
            type=entity_type,
            properties=properties or {}
        )

    def add_relationship(self, entity1, relationship, entity2):
        """Add a relationship (directed edge) between two entities."""
        self.graph.add_edge(entity1, entity2, relation=relationship)

    def query(self, start_entity, relationship):
        """Find all entities connected to start_entity by a given relationship."""
        results = []
        for neighbor in self.graph.successors(start_entity):
            edge_data = self.graph.get_edge_data(start_entity, neighbor)
            if edge_data and edge_data.get('relation') == relationship:
                results.append(neighbor)
        return results

    def get_neighbors(self, entity, max_depth=2):
        """Find all entities within max_depth hops."""
        visited = set()
        to_visit = [(entity, 0)]
        results = []

        while to_visit:
            current, depth = to_visit.pop(0)
            if current in visited or depth > max_depth:
                continue
            visited.add(current)

            for neighbor in self.graph.successors(current):
                edge_data = self.graph.get_edge_data(current, neighbor)
                results.append({
                    "from": current,
                    "relation": edge_data.get('relation'),
                    "to": neighbor
                })
                to_visit.append((neighbor, depth + 1))

        return results
```

Here's how to populate and query it:

```python
kg = SimpleKnowledgeGraph()

# Add entities
kg.add_entity("Python", entity_type="Programming Language")
kg.add_entity("Guido van Rossum", entity_type="Person")
kg.add_entity("Django", entity_type="Framework")
kg.add_entity("Web Development", entity_type="Domain")

# Add relationships
kg.add_relationship("Python", "created_by", "Guido van Rossum")
kg.add_relationship("Django", "built_with", "Python")
kg.add_relationship("Django", "used_for", "Web Development")
kg.add_relationship("Python", "used_for", "Web Development")

# Simple queries
creator = kg.query("Python", "created_by")
print(f"Python was created by: {creator}")  # ["Guido van Rossum"]

# Multi-hop: everything connected to Python within 2 hops
related = kg.get_neighbors("Python", max_depth=2)
for r in related:
    print(f"  {r['from']} --{r['relation']}--> {r['to']}")
```

## Extracting Entities from Documents

Manually building a knowledge graph is fine for small, well-defined domains. But for large document corpora, you need automated extraction. LLMs are surprisingly good at identifying entities and relationships in text.

```python
def extract_entities_and_relationships(text):
    """Use an LLM to extract structured knowledge from text."""
    prompt = f"""Extract entities and relationships from the text below.

    Text: {text}

    Output format:
    Entities:
    - EntityName (EntityType)

    Relationships:
    - (Entity1, relationship, Entity2)
    """

    response = openai.ChatCompletion.create(
        model="gpt-4",
        messages=[{"role": "user", "content": prompt}],
        temperature=0.0
    )

    return response.choices[0].message.content
```

For example, given the text "Python is a programming language created by Guido van Rossum in 1991. Django is a popular web framework built with Python", the LLM would extract:

```
Entities:
- Python (Programming Language)
- Guido van Rossum (Person)
- Django (Framework)

Relationships:
- (Python, created_by, Guido van Rossum)
- (Python, released_in, 1991)
- (Django, built_with, Python)
```

For faster extraction that doesn't require LLM calls, you can use NER (Named Entity Recognition) libraries like spaCy:

```python
import spacy

nlp = spacy.load("en_core_web_sm")

def extract_entities_spacy(text):
    """Extract named entities using spaCy."""
    doc = nlp(text)
    return [{"text": ent.text, "type": ent.label_} for ent in doc.ents]
```

spaCy is faster and cheaper (no API calls), but it only extracts entities — not relationships. LLMs can extract both, but at the cost of API calls and latency. In practice, you might use spaCy for entity extraction at scale and LLMs for relationship extraction on a smaller, curated subset of documents.

## Combining Vector Search with Knowledge Graphs

The most powerful approach is to use both vector search and knowledge graph reasoning in a single system. The key is deciding which questions need which approach.

```python
class HybridRAGKG:
    def __init__(self, vector_collection, knowledge_graph):
        self.vector_collection = vector_collection
        self.kg = knowledge_graph

    def query(self, user_question):
        """Route the query to vector search, knowledge graph, or both."""

        # Determine if the query involves entity relationships
        needs_kg = self.requires_graph_reasoning(user_question)

        # Knowledge graph context
        kg_context = ""
        if needs_kg:
            entities = self.extract_entities_from_question(user_question)
            kg_results = []
            for entity in entities:
                related = self.kg.get_neighbors(entity, max_depth=2)
                kg_results.extend(related)
            kg_context = self.format_kg_results(kg_results)

        # Vector search context
        vector_docs = self.vector_search(user_question, top_k=3)
        vector_context = "\n\n".join(vector_docs)

        # Combine and generate
        combined_context = f"""Knowledge Graph Information:
{kg_context}

Document Information:
{vector_context}"""

        prompt = f"""Context:
{combined_context}

Question: {user_question}

Answer the question using both the structured knowledge graph information
and the document content. Cite your sources."""

        response = openai.ChatCompletion.create(
            model="gpt-3.5-turbo",
            messages=[{"role": "user", "content": prompt}]
        )

        return response.choices[0].message.content

    def requires_graph_reasoning(self, question):
        """Simple heuristic: does the question ask about relationships?"""
        relationship_keywords = [
            "who created", "when was", "where is",
            "relationship between", "connected to",
            "works at", "founder of", "related to"
        ]
        question_lower = question.lower()
        return any(kw in question_lower for kw in relationship_keywords)
```

With this hybrid approach, a question like "How do I use Python list comprehensions?" routes primarily through vector search, retrieving relevant tutorial text. A question like "Who created Python and what companies did they work for?" routes through the knowledge graph, following the relationship chain from Python to its creator to their employment history. And a question like "What web frameworks use the language created by Guido?" uses both: the knowledge graph to resolve "language created by Guido" to "Python", and then vector search to find detailed information about Python web frameworks.

Knowledge graphs are not necessary for every RAG system. They add complexity and require either manual curation or an extraction pipeline. But for domains with rich entity relationships — organisational structures, product ecosystems, regulatory frameworks, scientific taxonomies — they can answer questions that pure vector search simply cannot.

## Hands-on practical

Work through the accompanying notebook: [`module_3/03-knowledge-graphs.ipynb`](https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules/blob/main/module_3/03-knowledge-graphs.ipynb). It walks through building a small directed graph with NetworkX, extracting triples via the LLM, multi-hop queries, and a simple heuristic for routing questions to graph vs vector search.
