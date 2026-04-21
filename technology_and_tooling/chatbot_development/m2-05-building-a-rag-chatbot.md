---
name: Building a RAG Chatbot
dependsOn:
  - technology_and_tooling.chatbot_development.m2-04-vector-databases
tags: [chatbots, rag, python]
learningOutcomes:
  - Build a complete RAG chatbot that retrieves documents and generates grounded responses.
  - Write effective RAG prompts that constrain the LLM to use retrieved context.
  - Implement source citation in chatbot responses.
  - Manage the context window when combining retrieved documents with conversation history.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    image: https://github.com/OxfordCompetencyCenters.png
    license: CC-BY-4.0
---

You now have all the building blocks: embeddings for semantic representation, chunking for document preparation, and a vector database for storage and search. In this section, we assemble them into a complete RAG chatbot — a system that answers user questions by retrieving relevant documents and generating responses grounded in those documents.

## The Complete RAG Chatbot

Here is a full implementation that ties together everything from this module. The `RAGChatbot` class handles document indexing, retrieval, and response generation:

```python
import chromadb
import openai
import os

openai.api_key = os.getenv("OPENAI_API_KEY")

class RAGChatbot:
    def __init__(self, collection_name="rag_docs"):
        self.client = chromadb.PersistentClient(path="./chroma_db")
        self.collection = self.client.get_or_create_collection(collection_name)

    def add_documents(self, documents, metadatas=None):
        """Index documents for retrieval."""
        response = openai.Embedding.create(
            model="text-embedding-3-small",
            input=documents
        )
        embeddings = [item['embedding'] for item in response['data']]

        ids = [f"doc_{i}" for i in range(len(documents))]
        self.collection.add(
            ids=ids,
            embeddings=embeddings,
            documents=documents,
            metadatas=metadatas or [{}] * len(documents)
        )

    def retrieve(self, query, top_k=3):
        """Retrieve the most relevant documents for a query."""
        response = openai.Embedding.create(
            model="text-embedding-3-small",
            input=query
        )
        query_embedding = response['data'][0]['embedding']

        results = self.collection.query(
            query_embeddings=[query_embedding],
            n_results=top_k
        )
        return results['documents'][0], results['metadatas'][0]

    def chat(self, user_query, top_k=3):
        """Answer a query using RAG: retrieve, then generate."""
        # 1. Retrieve relevant documents
        documents, metadatas = self.retrieve(user_query, top_k)

        # 2. Build context from retrieved documents
        context = "\n\n".join([
            f"Document {i+1}: {doc}"
            for i, doc in enumerate(documents)
        ])

        # 3. Build the prompt
        prompt = f"""You are a helpful assistant. Answer the user's question
using ONLY the information provided in the context below.

Context:
{context}

Question: {user_query}

Instructions:
- Base your answer strictly on the provided context.
- If the context doesn't contain enough information, say so.
- Cite which document(s) you drew from when possible.

Answer:"""

        # 4. Generate the response
        response = openai.ChatCompletion.create(
            model="gpt-3.5-turbo",
            messages=[
                {"role": "system", "content": "You are a helpful assistant that answers questions based on provided context."},
                {"role": "user", "content": prompt}
            ],
            temperature=0.3
        )

        answer = response.choices[0].message.content
        sources = [meta.get('source', 'Unknown') for meta in metadatas]

        return {
            "answer": answer,
            "sources": sources,
            "retrieved_docs": documents
        }
```

The flow is straightforward: the user asks a question, we embed it and search the vector database for similar chunks, we inject those chunks into the prompt as context, and the LLM generates a response based on that context. The low temperature (0.3) is deliberate — for factual questions grounded in documents, you want consistent, focused answers rather than creative variations.

Here's how to use it:

```python
bot = RAGChatbot()

# Index some documents
docs = [
    "Python was created by Guido van Rossum and first released in 1991.",
    "Python is known for its simple and readable syntax.",
    "Python is widely used in web development, data science, and AI."
]
bot.add_documents(docs, metadatas=[
    {"source": "python_history.txt"},
    {"source": "python_features.txt"},
    {"source": "python_applications.txt"}
])

# Query
result = bot.chat("When was Python created?")
print("Answer:", result['answer'])
print("Sources:", result['sources'])
```

The chatbot retrieves the relevant document about Python's history, includes it as context, and the LLM generates an answer grounded in that specific document. The response includes source information so the user can verify the answer.

## Writing Effective RAG Prompts

The prompt you use when combining retrieved documents with the user's question has an enormous impact on response quality. The most important instruction is to tell the LLM to answer **only** from the provided context. Without this constraint, the model may draw on its training data and produce answers that sound authoritative but aren't grounded in your documents — defeating the purpose of RAG.

A basic prompt might look like this:

```python
prompt = f"""Answer based on the context below.

Context: {context}

Question: {query}"""
```

This works but lacks guardrails. An enhanced prompt adds explicit instructions for edge cases:

```python
prompt = f"""You are a knowledgeable assistant. Answer the user's question
using ONLY the information provided in the context below.

Context:
{context}

Question: {query}

Instructions:
- Base your answer strictly on the provided context
- If the context doesn't contain enough information to answer fully, say
  "I don't have enough information to answer that"
- Cite specific details from the context when possible
- Be concise but complete
- If multiple documents provide relevant information, synthesize them"""
```

The "I don't have enough information" instruction is critical. Without it, when retrieval fails to find relevant documents (which will happen), the LLM fills the gap with its training data. That's exactly the hallucination problem RAG is supposed to solve. By explicitly instructing the model to admit uncertainty, you preserve the trustworthiness of the system.

For complex questions that require reasoning across multiple retrieved documents, a chain-of-thought approach can improve accuracy:

```python
prompt = f"""Context:
{context}

Question: {query}

Think through this step-by-step:
1. What information from the context is relevant to this question?
2. Is there enough information to answer fully?
3. What is the most accurate answer based on the context?

Answer:"""
```

This encourages the model to reason explicitly about which parts of the context are relevant before composing its answer, which tends to produce more accurate and well-supported responses.

## Managing Context with Retrieved Documents

When you combine RAG with a stateful chatbot (which most production systems are), you need to fit three things into the context window: the system prompt, the conversation history, and the retrieved documents. Token budget management becomes even more important than in Module 1, because retrieved context can be substantial.

Here's an approach that limits the total context by selecting only as many retrieved documents as will fit:

```python
import tiktoken

def chat_with_token_limit(self, user_question, top_k=5, max_context_tokens=2000):
    """RAG with context window management."""
    enc = tiktoken.encoding_for_model("gpt-3.5-turbo")

    # Retrieve more documents than we might need
    documents, metadatas = self.retrieve(user_question, top_k=top_k)

    # Add documents until we hit the token budget
    context_docs = []
    current_tokens = 0

    for doc in documents:
        doc_tokens = len(enc.encode(doc))
        if current_tokens + doc_tokens < max_context_tokens:
            context_docs.append(doc)
            current_tokens += doc_tokens
        else:
            break

    # Build the prompt with the trimmed context
    context = "\n\n".join(context_docs)
    # ... continue with prompt construction and LLM call
```

The strategy is to retrieve more documents than you expect to use (`top_k=5`), then include them in order of relevance until you run out of token budget. Since the vector database returns results sorted by similarity, the most relevant documents are included first and less relevant ones are dropped if space is tight.

In Module 4, we'll explore more sophisticated approaches to balancing conversation history with retrieved context. For now, the key principle is: always be aware of how many tokens your retrieved context consumes, and set explicit budgets.

::::challenge{id=m2-05-ex1 title="Process and Index Documents"}

Build the document processing pipeline for a set of sample documents. Load each file, chunk it, generate embeddings, and store everything in ChromaDB.

```python
import openai
from langchain.text_splitter import RecursiveCharacterTextSplitter
import os

openai.api_key = os.getenv("OPENAI_API_KEY")

def process_documents(file_paths, chunk_size=1000, overlap=200):
    """
    Process multiple documents: load, chunk, embed, and return
    structured data ready for indexing.
    """
    all_chunks = []

    for file_path in file_paths:
        # TODO: Read the file
        # TODO: Chunk the text using RecursiveCharacterTextSplitter
        # TODO: Generate embeddings for each chunk (batch for efficiency)
        # TODO: Attach metadata (source file, chunk index)
        pass

    return all_chunks

# Test with sample documents
file_paths = [
    "./sample_docs/python_basics.txt",
    "./sample_docs/python_functions.txt",
    "./sample_docs/python_classes.txt"
]

chunks = process_documents(file_paths)
print(f"Processed {len(chunks)} chunks")
```

After implementing the basic pipeline, test with different chunk sizes (500, 1000, 2000) and observe how the number of chunks and their coherence change.
::::

::::challenge{id=m2-05-ex2 title="Index and Search with ChromaDB"}

Take the processed chunks from Exercise 1 and build a search system.

```python
import chromadb

client = chromadb.PersistentClient(path="./lab_chroma_db")
collection = client.get_or_create_collection("python_docs")

def index_chunks(chunks):
    """Add processed chunks to ChromaDB."""
    # TODO: Extract ids, embeddings, documents, and metadatas from chunks
    # TODO: Add to collection
    pass

def semantic_search(query, top_k=3):
    """Search for relevant chunks."""
    # TODO: Embed the query
    # TODO: Search the collection
    # TODO: Return results with metadata
    pass

# Index your chunks
index_chunks(chunks)

# Test with queries you know the answer to
test_queries = [
    "How do I define a function in Python?",
    "What are Python classes?",
    "Explain Python data types"
]

for query in test_queries:
    print(f"\nQuery: {query}")
    results = semantic_search(query, top_k=2)
    print(f"Top result: {results['documents'][0][0][:200]}...")
```

Write a simple evaluation function to check whether the right source document is retrieved for each query:

```python
def evaluate_retrieval(query, expected_source):
    """Check if the correct document is retrieved."""
    results = semantic_search(query, top_k=1)
    retrieved_source = results['metadatas'][0][0]['source']

    if expected_source in retrieved_source:
        print(f"  Correct retrieval for: {query}")
    else:
        print(f"  Wrong retrieval for: {query}")
        print(f"  Expected: {expected_source}, Got: {retrieved_source}")

evaluate_retrieval("How do I define a function?", "python_functions.txt")
evaluate_retrieval("What is a class?", "python_classes.txt")
```
::::

::::challenge{id=m2-05-ex3 title="Build the Complete RAG Chatbot"}

Combine retrieval with generation to build a working RAG chatbot.

```python
class PythonDocsRAG:
    def __init__(self, collection):
        self.collection = collection

    def chat(self, user_question, top_k=3):
        """Answer a question using RAG."""
        # TODO: 1. Retrieve relevant documents
        # TODO: 2. Build context from retrieved docs
        # TODO: 3. Create a prompt that instructs the LLM to use only the context
        # TODO: 4. Generate the response
        # TODO: 5. Return the answer with source citations
        pass

# Test your chatbot
rag_bot = PythonDocsRAG(collection)

questions = [
    "How do I create a list in Python?",
    "What is the difference between a function and a method?",
    "How do I handle exceptions in Python?",
    "What is inheritance in Python classes?"
]

for question in questions:
    print(f"\nQ: {question}")
    result = rag_bot.chat(question)
    print(f"A: {result['answer']}")
    print(f"Sources: {', '.join(result['sources'])}")
    print("-" * 80)
```

Test with questions where you know the answer and can verify whether the chatbot is drawing from the correct documents. Also test with a question that your documents don't cover — does the chatbot admit it doesn't have enough information, or does it hallucinate?
::::

::::challenge{id=m2-05-checkpoint title="Project Checkpoint 2: Add RAG to Your Capstone"}

Building on your Module 1 capstone, add document-powered knowledge retrieval to your chatbot.

Prepare a document corpus for your chosen domain — at least 10 documents. For a customer support chatbot, this might be product documentation, FAQs, and troubleshooting guides. For an educational tutor, it might be course materials and textbook excerpts. For an e-commerce assistant, it might be product catalogues and policy documents.

By the end of this checkpoint, your capstone should include:

- A document processing pipeline (chunking and embedding)
- ChromaDB integration for storage and retrieval
- A RAG-powered chat function that retrieves relevant context before generating responses
- Source citations in responses
- Updated documentation describing your approach

Test extensively by asking questions where you know which document should be retrieved. If retrieval is poor, experiment with different chunk sizes. If answers are wrong despite correct retrieval, improve your RAG prompt.
::::

## Summary and What's Next

In this module, you've built a RAG system from the ground up: embeddings that capture semantic meaning, chunking strategies that prepare documents for retrieval, a vector database for efficient search, and a chatbot that grounds its responses in retrieved documents. Your chatbot can now answer questions about your specific data, cite its sources, and admit when it doesn't have enough information.

In Module 3, we'll make this RAG system significantly better. You'll learn advanced techniques like re-ranking (reordering search results for higher precision), hybrid search (combining semantic and keyword search), multi-step retrieval (breaking complex questions into sub-queries), and semantic caching (avoiding redundant API calls for similar questions). These optimisations can dramatically improve both the quality and cost-efficiency of your RAG system.

## Additional Resources

- [Original RAG Paper](https://arxiv.org/abs/2005.11401) — "Retrieval-Augmented Generation for Knowledge-Intensive NLP Tasks" (Lewis et al., 2020)
- [OpenAI Embeddings Guide](https://platform.openai.com/docs/guides/embeddings) — Official documentation for the Embeddings API
- [ChromaDB Documentation](https://docs.trychroma.com) — Full reference for ChromaDB
- [The Illustrated Word2vec](https://jalammar.github.io/illustrated-word2vec/) — Visual guide to understanding word embeddings
- [Embedding Projector](https://projector.tensorflow.org) — Interactive tool for visualising embeddings in 3D

## Hands-on practical

Work through the accompanying notebook: [`module_2/05-building-a-rag-chatbot.ipynb`](https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules/blob/main/module_2/05-building-a-rag-chatbot.ipynb). It walks through building the full `RAGChatbot` class, indexing a small corpus, querying with source citations, and exercising a "don't-know" fallback case.
