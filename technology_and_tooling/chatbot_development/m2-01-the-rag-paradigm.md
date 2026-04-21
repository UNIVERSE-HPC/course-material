---
name: The RAG Paradigm
dependsOn:
  - technology_and_tooling.chatbot_development.m1-04-building-stateless-and-stateful-chatbots
tags: [chatbots, rag]
learningOutcomes:
  - Explain what Retrieval-Augmented Generation is and how it works.
  - Identify the limitations of base LLMs that RAG addresses.
  - Describe the two phases of a RAG system (indexing and retrieval).
  - Distinguish between RAG and fine-tuning and know when to use each.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    image: https://github.com/OxfordCompetencyCenters.png
    license: CC-BY-4.0
---

In Module 1, you built a chatbot that can hold conversations and manage its context window. But that chatbot has a fundamental limitation: it only knows what was in the LLM's training data. Ask it about your company's internal documentation, last week's policy changes, or the specifics of your product catalogue, and it will either refuse to answer or — worse — confidently make something up. Retrieval-Augmented Generation (RAG) solves this problem, and it's the single most important pattern in production chatbot development.

## What is Retrieval-Augmented Generation?

RAG combines information retrieval with text generation. Instead of relying solely on what the LLM learned during training, a RAG system retrieves relevant documents from an external knowledge base and includes them in the prompt. The LLM then generates a response grounded in those retrieved documents, rather than drawing entirely on its internal parameters.

Think of it like an open-book exam. Without RAG, the LLM is taking a closed-book test — it can only answer from memory, and its memory has a cutoff date and no knowledge of your private data. With RAG, the LLM gets to look things up before answering.

The process works in six steps:

```
1. User asks a question
   ↓
2. Convert the question to a vector embedding
   ↓
3. Search a vector database for similar documents
   ↓
4. Retrieve the top K most relevant chunks
   ↓
5. Inject the retrieved context into the LLM prompt
   ↓
6. LLM generates a response grounded in the retrieved documents
```

The critical insight is that the LLM doesn't "know" the information in any deep sense. It reads the retrieved documents at inference time, just as a human would read reference material before answering a question. This means you can update your knowledge base at any time — add new documents, remove outdated ones, correct errors — and the chatbot's responses reflect those changes immediately, without any retraining.

## Why RAG? The Limitations It Solves

Base LLMs, no matter how capable, have four limitations that make them insufficient for most production use cases.

The first is **knowledge cutoff**. Every LLM has a training data cutoff date. GPT-4's training data, for example, stops at a specific point in time. Anything that happened after that date — new product releases, updated policies, recent events — simply doesn't exist in the model's knowledge. For a customer support chatbot that needs to reference the latest version of your documentation, this is a deal-breaker.

The second is **no access to private or proprietary knowledge**. Even within its training period, the LLM has never seen your company's internal documents, your customer database, your HR policies, or your product specifications. It cannot answer questions about information it has never encountered, no matter how well you prompt it.

The third is **hallucination**. When an LLM doesn't know the answer to a question, it doesn't say "I don't know." It generates a plausible-sounding response based on patterns in its training data. For general knowledge, this often works well enough. But for specific, factual questions — "What is our return policy for electronics purchased after January 2024?" — a hallucinated answer can be confidently wrong and actively harmful.

The fourth is **inability to cite sources**. Even when an LLM gives a correct answer, it can't tell you where that information came from. In high-stakes domains like legal, medical, or financial services, the inability to audit and verify responses is a serious problem.

RAG addresses all four of these limitations. It gives the chatbot access to current documents (solving the knowledge cutoff). It can index your proprietary data (solving the private knowledge gap). It grounds responses in actual documents, dramatically reducing hallucination. And it can cite exactly which documents it drew from, enabling verification and building user trust.

## RAG Architecture: Indexing and Retrieval

A RAG system operates in two distinct phases.

The **indexing phase** happens offline, before any user interacts with the system. You take your document corpus — PDFs, text files, web pages, database records, whatever you have — and process it through a pipeline: clean the text, split it into chunks, convert each chunk into a vector embedding, and store those embeddings in a vector database alongside the original text and any metadata. This is a one-time operation (or periodic, if your documents change).

The **retrieval phase** happens in real time, every time a user asks a question. The user's query is converted into a vector embedding using the same model that embedded the documents. That query vector is compared against all the document vectors in the database to find the most similar ones. The top K results are retrieved, and their text is injected into the LLM prompt as context. The LLM then generates a response based on that context.

Here is the complete architecture:

```
┌─────────────────┐
│  Document       │
│  Corpus         │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ Preprocessing   │  ← Text cleaning, chunking
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ Embedding       │  ← Convert to vectors
│ Generation      │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ Vector Database │  ← Store and index
└────────┬────────┘
         │
    [Index Time]
         │
         │           [Query Time]
         │                │
         │    ┌───────────▼──────────┐
         │    │ User Query → Embed   │
         │    └───────────┬──────────┘
         │                │
         │    ┌───────────▼──────────┐
         └───►│ Similarity Search    │
              └───────────┬──────────┘
                          │
              ┌───────────▼──────────┐
              │ Retrieve Top K       │
              └───────────┬──────────┘
                          │
              ┌───────────▼──────────┐
              │ Inject into Prompt   │
              └───────────┬──────────┘
                          │
              ┌───────────▼──────────┐
              │ Generate Response    │
              └──────────────────────┘
```

The embedding generation step is crucial, and it happens twice: once for documents during indexing, and once for user queries during retrieval. Both must use the same embedding model — if you embed your documents with `text-embedding-3-small` and your queries with a different model, the vectors won't be comparable and search will fail.

## RAG vs Fine-Tuning

A common question is: "Should I fine-tune the model on my data instead of using RAG?" The short answer is that RAG and fine-tuning solve different problems, and for most chatbot use cases, RAG is the right choice.

| Aspect | RAG | Fine-Tuning |
|--------|-----|-------------|
| **Purpose** | Add external knowledge and facts | Change behaviour, style, or format |
| **Update speed** | Instant (update your documents) | Slow (retrain the model) |
| **Cost** | Low (inference only) | High (training compute) |
| **Complexity** | Moderate | High |
| **Transparency** | Can cite sources | Black box |
| **Best for** | Knowledge bases, document Q&A, customer support | Specific tone/style, domain-specific task formats |

Fine-tuning changes *how* the model behaves — its tone, its response format, its approach to certain types of tasks. RAG changes *what* the model knows — the factual information it can draw on. If you need your chatbot to answer questions about your product documentation, that's a RAG problem. If you need your chatbot to respond in a very specific medical dialogue format, that might be a fine-tuning problem.

The practical recommendation is to start with RAG. Many use cases that initially seem like they need fine-tuning can actually be solved with RAG combined with a well-designed system prompt. Fine-tuning is expensive, slow to iterate on, and creates a model that's harder to update. RAG is cheap, instantly updatable, and transparent. Only consider fine-tuning if you've exhausted what RAG and prompt engineering can do and still need specialised behaviour.

## Hands-on practical

Work through the accompanying notebook: [`module_2/01-the-rag-paradigm.ipynb`](https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules/blob/main/module_2/01-the-rag-paradigm.ipynb). It walks through the knowledge-cutoff and hallucination problems first-hand, then contrasts them with a manual RAG call where reference text is supplied in the prompt.
