---
name: Document Processing and Chunking
dependsOn:
  - technology_and_tooling.chatbot_development.m2-02-vector-embeddings
tags: [chatbots, rag, chunking]
learningOutcomes:
  - Explain why documents must be chunked before embedding.
  - Implement fixed-size, semantic, and recursive chunking strategies.
  - Use chunk overlap to preserve context across boundaries.
  - Enrich chunks with metadata for filtering and citation.
  - Build a complete document processing pipeline.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    image: https://github.com/OxfordCompetencyCenters.png
    license: CC-BY-4.0
---

You now know how to convert text into vector embeddings. But before you can embed your documents, you need to decide how to break them into pieces. This step — called chunking — has an outsized impact on retrieval quality. Get it right, and your chatbot finds exactly the information it needs. Get it wrong, and it returns irrelevant or fragmented results no matter how good your embedding model is.

## Why Chunking Matters

You might wonder: why not just embed each document as a whole? There are three reasons.

First, embeddings work best on focused, coherent text. When you embed a 100-page PDF as a single vector, that vector tries to represent the average meaning of everything in the document. It captures the broad topic but loses all the fine-grained detail. A search for "how to configure email notifications" would match a document about your product's settings, but wouldn't be able to pinpoint the specific paragraph about email notifications versus the one about Slack integrations.

Second, LLMs have context windows. Even if you could retrieve a relevant 100-page document, you can't fit it into a single prompt alongside the system message, conversation history, and instructions. You need to retrieve only the relevant portions — which means you need to have broken the document into portions that can be retrieved independently.

Third, precision matters for user trust. If your chatbot says "According to the documentation..." and then cites an entire 50-page manual, that's not useful. If it cites a specific 200-word passage that directly answers the question, that's credible and verifiable.

The trade-off in chunking is between chunks that are too large (diluted semantics, wasted context window space) and chunks that are too small (fragmented information, lost context). A chunk size of 200–500 tokens is typical for most RAG applications, but the right size depends on your documents and your use case.

## Chunking Strategies

There are three main approaches to chunking, each with different strengths.

### Fixed-Size Chunking

The simplest approach is to split the text into chunks of a fixed number of characters or tokens, regardless of where sentences or paragraphs fall:

```python
def fixed_size_chunking(text, chunk_size=500, overlap=50):
    """Split text into fixed-size chunks with overlap."""
    chunks = []
    start = 0
    while start < len(text):
        end = start + chunk_size
        chunks.append(text[start:end])
        start += chunk_size - overlap
    return chunks
```

This is fast and produces consistent chunk sizes, which is convenient for token budgeting. The downside is that it breaks text at arbitrary positions — potentially in the middle of a sentence, or even in the middle of a word. A chunk that starts with "...increased by" and another that starts with "15% in Q3 2023" are both incomplete. Neither captures the full meaning on its own.

### Semantic Chunking

Semantic chunking splits on natural text boundaries — paragraphs, sections, or sentences:

```python
def semantic_chunking(text):
    """Split on paragraph boundaries."""
    chunks = text.split('\n\n')
    return [chunk.strip() for chunk in chunks if chunk.strip()]
```

This respects the structure of the document, so each chunk is more likely to be a coherent, self-contained unit of information. The downside is that chunk sizes can vary wildly. A short paragraph might produce a 20-token chunk, while a long section might produce a 2,000-token chunk. Inconsistent sizes complicate token budgeting and can lead to some chunks being too small to be meaningful and others too large to be focused.

### Recursive Chunking

Recursive chunking — implemented in LangChain's `RecursiveCharacterTextSplitter` — combines the strengths of both approaches. It tries to split on paragraph boundaries first. If the resulting chunks are still too large, it falls back to sentence boundaries. If those are still too large, it falls back to word boundaries. This respects natural structure while keeping chunks within a target size:

```python
from langchain.text_splitter import RecursiveCharacterTextSplitter

splitter = RecursiveCharacterTextSplitter(
    chunk_size=500,
    chunk_overlap=50,
    separators=["\n\n", "\n", ". ", " ", ""]
)
chunks = splitter.split_text(text)
```

The `separators` list defines the priority order for splitting. The splitter tries `"\n\n"` (paragraph breaks) first, then `"\n"` (line breaks), then `". "` (sentence ends), then `" "` (word boundaries), and finally `""` (individual characters) as a last resort. This produces chunks that are both coherent and consistently sized.

For most RAG applications, recursive chunking is the best default choice. It's what you should start with unless you have a specific reason to use something else.

## Chunk Overlap

When you split a document into chunks, information at the boundaries can be lost. Consider a passage where one sentence sets up context and the next sentence provides the key fact. If the chunk boundary falls between those two sentences, neither chunk captures the complete picture.

Chunk overlap solves this by making adjacent chunks share some content:

```
Without overlap:
  Chunk 1: [...important information starts here]
  Chunk 2: [continues here with critical context...]
  → If a query matches chunk 2, the setup from chunk 1 is missing

With overlap:
  Chunk 1: [...important information starts here...]
  Chunk 2: [...starts here continues here with critical context...]
  → Both chunks have enough context to be meaningful
```

A typical overlap is 10–20% of the chunk size. For 500-token chunks, that means 50–100 tokens of overlap. More overlap means more redundancy (you're storing and embedding the same text multiple times), but better context preservation at boundaries.

Here's how to configure overlap with LangChain:

```python
from langchain.text_splitter import RecursiveCharacterTextSplitter

splitter = RecursiveCharacterTextSplitter(
    chunk_size=1000,
    chunk_overlap=200,  # 20% overlap
    length_function=len,
)

text = open("document.txt").read()
chunks = splitter.split_text(text)

print(f"Created {len(chunks)} chunks")
for i, chunk in enumerate(chunks[:3]):
    print(f"\nChunk {i}: {chunk[:100]}...")
```

## Enriching Chunks with Metadata

Raw text chunks are useful, but chunks enriched with metadata are far more powerful. Metadata lets you filter search results ("only search documents from 2024"), track provenance for citations ("this answer came from page 5 of the user guide"), and implement hybrid search strategies.

When you process a document, attach metadata to each chunk:

```python
chunk_with_metadata = {
    "text": "Python is a versatile programming language...",
    "metadata": {
        "source": "python_guide.pdf",
        "page": 5,
        "section": "Introduction",
        "author": "John Doe",
        "date": "2024-01-15",
        "doc_type": "tutorial",
        "language": "en"
    }
}
```

At retrieval time, you can combine vector search with metadata filters:

```python
results = vector_db.search(
    query="How do I use Python lists?",
    filter={"doc_type": "tutorial", "language": "en"},
    top_k=5
)
```

This is particularly valuable in production. A customer support chatbot might filter by product version so it only retrieves documentation relevant to the customer's setup. A legal assistant might filter by jurisdiction and date. An internal knowledge base might filter by department.

Metadata is also essential for citations. When your chatbot cites its sources, it needs to know which file, which page, and which section each chunk came from:

```python
def generate_response_with_citations(query, retrieved_chunks):
    context = ""
    sources = []

    for chunk in retrieved_chunks:
        context += chunk["text"] + "\n"
        sources.append(f"{chunk['source']} (page {chunk['page']})")

    prompt = f"""Context: {context}

    Question: {query}

    Answer the question using the context above. Cite your sources."""

    response = llm.generate(prompt)
    return response, sources
```

## Building a Complete Document Processing Pipeline

Putting it all together, here is a complete pipeline that takes a file, cleans the text, chunks it, generates embeddings, and attaches metadata:

```python
import openai
from langchain.text_splitter import RecursiveCharacterTextSplitter

def process_document(file_path, metadata=None):
    """Complete pipeline: load → clean → chunk → embed."""

    # 1. Load the document
    with open(file_path, 'r') as f:
        text = f.read()

    # 2. Clean the text
    text = text.replace('\n\n\n', '\n\n')
    text = text.strip()

    # 3. Chunk with overlap
    splitter = RecursiveCharacterTextSplitter(
        chunk_size=1000,
        chunk_overlap=200,
        separators=["\n\n", "\n", ". ", " ", ""]
    )
    chunks = splitter.split_text(text)

    # 4. Generate embeddings in batches
    embeddings = []
    batch_size = 100

    for i in range(0, len(chunks), batch_size):
        batch = chunks[i:i+batch_size]
        response = openai.Embedding.create(
            model="text-embedding-3-small",
            input=batch
        )
        batch_embeddings = [item['embedding'] for item in response['data']]
        embeddings.extend(batch_embeddings)

    # 5. Combine chunks, embeddings, and metadata
    processed_chunks = []
    for idx, (chunk, embedding) in enumerate(zip(chunks, embeddings)):
        processed_chunks.append({
            "id": f"{file_path}_{idx}",
            "text": chunk,
            "embedding": embedding,
            "metadata": {
                "source": file_path,
                "chunk_index": idx,
                **(metadata or {})
            }
        })

    return processed_chunks

# Usage
chunks = process_document(
    "docs/user_guide.txt",
    metadata={"doc_type": "guide", "version": "2.0"}
)
print(f"Processed {len(chunks)} chunks")
```

This pipeline is your indexing step — the offline phase of RAG. You run it once for each document (or periodically when documents are updated). The output is a list of chunks, each with its text, its embedding, and its metadata, ready to be stored in a vector database.

In production, you'd add error handling, progress logging, and support for different file formats (PDF, HTML, DOCX). You might also add a deduplication step to avoid indexing the same content twice. But this basic pipeline captures the essential structure that every RAG indexing system follows.

::::challenge{id=m2-03-compare title="Compare Chunking Strategies"}

Process the same document using three different chunking strategies and compare the results.

```python
from langchain.text_splitter import RecursiveCharacterTextSplitter

# Load a sample document (use any text file you have)
with open("sample_document.txt", "r") as f:
    text = f.read()

# Strategy 1: Small chunks, no overlap
splitter_small = RecursiveCharacterTextSplitter(chunk_size=200, chunk_overlap=0)
chunks_small = splitter_small.split_text(text)

# Strategy 2: Medium chunks with overlap
splitter_medium = RecursiveCharacterTextSplitter(chunk_size=500, chunk_overlap=50)
chunks_medium = splitter_medium.split_text(text)

# Strategy 3: Large chunks with overlap
splitter_large = RecursiveCharacterTextSplitter(chunk_size=1000, chunk_overlap=200)
chunks_large = splitter_large.split_text(text)

print(f"Small chunks: {len(chunks_small)} chunks")
print(f"Medium chunks: {len(chunks_medium)} chunks")
print(f"Large chunks: {len(chunks_large)} chunks")

# Inspect the first few chunks from each strategy
for name, chunks in [("Small", chunks_small), ("Medium", chunks_medium), ("Large", chunks_large)]:
    print(f"\n--- {name} chunks (first 2) ---")
    for i, chunk in enumerate(chunks[:2]):
        print(f"  Chunk {i} ({len(chunk)} chars): {chunk[:100]}...")
```

Look at the chunks each strategy produces. Are the small chunks coherent on their own, or do they feel fragmented? Are the large chunks too broad, mixing multiple topics? Which strategy produces chunks that feel like self-contained units of information? The answers will depend on your specific document, which is why experimenting with your actual data is so important.
::::

## Hands-on practical

Work through the accompanying notebook: [`module_2/03-document-processing-and-chunking.ipynb`](https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules/blob/main/module_2/03-document-processing-and-chunking.ipynb). It walks through fixed-size chunking, recursive splitting on natural boundaries, and a complete chunk-plus-metadata-plus-embed pipeline.
