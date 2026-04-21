---
name: Capstone Project Brief
dependsOn:
  - technology_and_tooling.chatbot_development.m4-05-user-personalisation
tags: [chatbots, capstone, project, rag, chromadb, memory, evaluation]
learningOutcomes:
  - Define a domain-specific chatbot project with a clear goal and document corpus.
  - Build an integrated chatbot combining RAG over ChromaDB with conversation memory and auto-summarisation.
  - Design and execute experiments that evaluate retrieval quality and response quality.
  - Analyse and report results with quantitative metrics, and reflect on lessons learned.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    license: CC-BY-4.0
---

The capstone project is a structured engineering project, not a tutorial exercise. Over Modules 1, 2, and 4 you built individual components — LLM integration and chatbot fundamentals, RAG pipelines backed by a vector database, and conversation memory with summarisation. The capstone brings them together into a single system that solves a real information-retrieval problem for a specific audience. This section defines the project, provides a structured process for executing it, and explains how to evaluate and report your results.

## Project Goal

Build a **domain-specific chatbot** that retrieves information from a curated document corpus, maintains conversation context across turns using auto-summarisation, and runs end-to-end locally as a Streamlit application that users open in a browser.

The emphasis on "domain-specific" is deliberate. A generic chatbot that wraps an LLM API adds little value — users can already access ChatGPT or Claude directly. A domain-specific chatbot adds value by grounding its responses in a particular corpus of documents that the base LLM does not have access to, applying domain-appropriate constraints (what to answer, what to refuse, how to handle ambiguity), and serving a defined audience with specific information needs.

For example, a legal research assistant answers questions grounded in case law documents. A company onboarding bot answers new-hire questions from HR policies and IT setup guides. A medical literature chatbot synthesises findings from PubMed papers. Each solves a problem that a general-purpose LLM cannot solve as well, because the chatbot has access to specialised documents and is tuned for the domain.

Your project should identify a domain, a target audience, and a document corpus, then build a chatbot that serves that audience using that corpus.

## Objectives

The capstone must satisfy the following objectives. Each is testable — you should be able to demonstrate it working.

1. **Retrieval-augmented generation over ChromaDB.** The chatbot ingests a document corpus into a ChromaDB vector store, retrieves relevant chunks at query time, and grounds its responses in those documents. Responses cite or reference the source material rather than relying solely on the LLM's training data.

2. **Conversation memory with auto-summarisation.** The chatbot maintains context across multiple turns within a conversation. A follow-up question like "tell me more about that" correctly refers to the previous response. When the conversation grows beyond a configured threshold, older turns are automatically summarised so the chatbot continues to operate within the model's context window.

3. **Streamlit chat application.** The chatbot runs as a local Streamlit app launched with `streamlit run app.py`. Users interact via a browser: they type messages into a chat input, see the assistant's replies alongside the retrieved source documents, and can reset the session. No containerisation or cloud deployment is required — the app is run locally on the participant's own machine or server.

4. **Documentation.** The project includes a README with setup instructions and usage examples, and at least one Architecture Decision Record for a key technical choice (e.g. ChromaDB as the vector store, or the summarisation strategy).

## Methodology

The project follows a two-phase methodology. Each phase builds on the previous one, and the integration checklist at the end of this section serves as a quality gate to verify completeness before moving forward.

### Phase 1: Domain and Data

Choose a domain and assemble a document corpus. A good domain choice has three properties: the documents are available (you can collect or access them), the questions are non-trivial (they require searching across multiple documents, not just looking up a single fact), and the audience is defined (you can describe who would use this chatbot and what they need).

Prepare the corpus for RAG ingestion. Chunk documents using the strategies from Module 2 — experiment with chunk sizes (500, 1,000, 1,500 tokens) to find the best trade-off between granularity and context. Generate embeddings and store them in a ChromaDB collection on local disk. Verify that similarity search returns relevant results for a sample of test queries before proceeding to the next phase.

### Phase 2: Core Pipeline

Build the end-to-end chat flow: the user sends a message, the RAG engine retrieves relevant documents from ChromaDB, the memory manager supplies prior conversation history (summarising older turns when necessary), the prompt is constructed with the retrieved context and the conversation memory, the LLM generates a response, and the response is returned to the user.

The system has three core components arranged around the LLM:

```
                ┌────────────────────────┐
                │   Streamlit UI         │
                │   (browser chat)       │
                └───────────┬────────────┘
                            │ message
                            ▼
                ┌────────────────────────┐
                │      Chatbot Core      │
                │  (orchestration loop)  │
                └─────┬─────────────┬────┘
                      │             │
          retrieve    │             │   recall / summarise
                      ▼             ▼
          ┌───────────────────┐  ┌──────────────────────┐
          │    RAG Engine     │  │   Memory Manager     │
          │  (ChromaDB store) │  │  (history + auto-    │
          │                   │  │   summarisation)     │
          └─────────┬─────────┘  └──────────┬───────────┘
                    │  retrieved chunks     │  history + summary
                    └───────────┬───────────┘
                                ▼
                    ┌────────────────────────┐
                    │       OpenAI LLM       │
                    │        (chat)          │
                    └────────────────────────┘
```

The **Streamlit UI** is the entry point — `st.chat_input` captures each user message and `st.chat_message` renders both the user turn and the assistant's reply (with source references). Session state holds the conversation history between reruns. The **chatbot core** orchestrates each turn. The **RAG engine** owns the ChromaDB collection — ingestion, embedding generation, and similarity search. The **memory manager** owns the conversation history and the summarisation policy: recent turns are kept verbatim, older turns are collapsed into a rolling summary when a configurable turn or token threshold is exceeded. The LLM receives a prompt built from the system instructions, the memory-manager output, the retrieved document chunks, and the new user message.

### Integration Checklist

Use this checklist as a quality gate to verify completeness before you start the experiments.

**Core Functionality:**
- `streamlit run app.py` launches the chatbot in a browser.
- The chat input accepts a user message and the assistant's reply renders in the conversation, with visible source references.
- RAG retrieval fetches relevant documents from ChromaDB for a given query.
- LLM generation creates responses grounded in the retrieved context.
- Conversation history persists across reruns via `st.session_state`, and a "Reset conversation" control clears it.
- API keys are loaded from environment variables (e.g. via `python-dotenv`), not hardcoded.

**Data Flow:**
- User messages flow through the full pipeline: user input → memory manager → RAG retrieval → prompt assembly → LLM generation → response.
- Retrieved document chunks are properly formatted and injected into the LLM prompt.
- Conversation history is stored and retrieved correctly between turns within a session.

**Context Management:**
- Running a 20-turn conversation does not exceed the model's context window.
- When the configured threshold is crossed, older turns are collapsed into a rolling summary.
- A follow-up question such as "tell me more about that" correctly resolves against earlier content — even after summarisation.

### Priority Framework

When time is limited, work through priorities in order. Priority 1 must be complete before moving to Priority 2.

**Priority 1: Core Functionality (Must Have).** End-to-end chat with RAG over ChromaDB and conversation memory with auto-summarisation. This is Phases 1–2.

**Priority 2: Documentation (Important).** README, at least one ADR, and an architecture diagram. Allocate dedicated time for this regardless of progress on other priorities.

## Experiments

Once the system is built, evaluate it systematically. The experiments below assess whether the system meets its objectives.

### Retrieval Quality

Prepare a small set of 5–10 test queries with known answers that exist in your document corpus. For each query, record which documents the RAG engine retrieves and whether the correct document appears in the top-k results. A handful of well-chosen queries is more useful than a large set of shallow ones.

```python
test_queries = [
    {"query": "What is the refund policy?", "expected_doc": "returns-policy.md"},
    {"query": "How do I set up two-factor authentication?", "expected_doc": "security-guide.md"},
    # ... more test queries
]

results = []
for test in test_queries:
    retrieved = rag_engine.retrieve(test["query"], top_k=5)
    doc_names = [doc["metadata"]["source"] for doc in retrieved]
    hit = test["expected_doc"] in doc_names
    results.append({
        "query": test["query"],
        "hit": hit,
        "top_score": retrieved[0]["score"] if retrieved else 0,
        "retrieved_docs": doc_names
    })

hit_rate = sum(r["hit"] for r in results) / len(results)
print(f"Retrieval hit rate (top-5): {hit_rate:.0%}")
```

If retrieval quality is poor, the most common causes are chunk size mismatches (try 500, 1,000, and 1,500 tokens), embedding model mismatches (verify the same model is used for documents and queries), and similarity thresholds that are too high or too low.

### Response Quality

For the same test queries, evaluate whether the chatbot's full response correctly answers the question based on the retrieved documents. Check for three failure modes: incorrect answers (the response contradicts the source documents), hallucinated answers (the response includes information not present in any retrieved document), and refusal to answer when the information is available.

Manual evaluation is the most reliable method for a capstone-scale project. For each test query, rate the response as correct, partially correct, or incorrect. If you have reference answers, you can also use an LLM-as-judge approach — ask a separate LLM call to score the response against the reference.

### Robustness

Test how the system handles inputs outside the happy path:

- **Empty input:** Send an empty string. The system should return a helpful prompt, not crash.
- **Very long input:** Send a message over 1,000 words. The system should truncate or handle it gracefully.
- **Gibberish:** Send random characters. The system should ask for clarification.
- **Off-topic queries:** Ask about something completely outside the domain. The system should redirect politely.

Record the result of each test: pass (reasonable response), fail (crash, unhelpful error, or inappropriate response).

### Performance

Measure two simple performance properties:

- **Latency:** Time each of your test queries once and report the average response time. No need for p95/p99 — a mean over 5–10 requests is enough to give a sense of typical latency.
- **Memory stability:** Run a 20-turn conversation and confirm that token counts stay within the model's context window. This directly tests whether auto-summarisation is working — without it, a long conversation would eventually overflow the window.

```python
import time

latencies = []
for test in test_queries:
    start = time.time()
    response = chatbot.chat(test["query"])
    latencies.append(time.time() - start)

print(f"Average latency: {sum(latencies) / len(latencies):.2f}s")
```

## Results

Document your experimental results in a structured format. Tables are more effective than prose for conveying quantitative data. The numbers below are illustrative — yours will differ. The goal is to *report* your results honestly and clearly, not to hit any particular target.

**Retrieval quality:**

| Metric | Value |
|--------|-------|
| Test queries | 8 |
| Hit rate (top-5) | 6 / 8 |

**Response quality:**

| Rating | Count |
|--------|-------|
| Correct | 6 |
| Partially correct | 1 |
| Incorrect | 1 |

**Performance:**

| Metric | Value |
|--------|-------|
| Average latency (8 requests) | 2.1s |
| 20-turn conversation fits in context | Pass |

**Robustness:**

| Test case | Result |
|-----------|--------|
| Empty input | Pass |
| Long input (1,000+ words) | Pass |
| Gibberish | Pass |
| Off-topic query | Pass |

**Optional baseline (if time allows).** Pick *one* comparison that illustrates the value of an engineering choice you made — for example, retrieval hit rate with two different chunk sizes, or the token count of a 20-turn conversation with auto-summarisation enabled versus disabled. A single well-presented comparison is sufficient.

## Conclusion

The conclusion is a structured reflection on the project. It should address the following:

**What worked well.** Which components performed as expected? Which design decisions proved effective? For example: "The combination of 800-token chunks with cross-encoder re-ranking achieved 85% retrieval accuracy, significantly better than the 60% achieved with default settings."

**What didn't work.** Be specific about failures and their root causes. "The initial summarisation strategy lost important context from early conversation turns. Switching to a hybrid approach (recent turns in full + summary of older turns) resolved this."

**Key technical challenges.** Describe 2–3 significant challenges and how they were resolved. These demonstrate problem-solving ability and depth of understanding.

**Lessons learned.** What would you do differently if starting over? What surprised you about building an LLM application in practice versus in theory?

**Future improvements.** List 3–5 concrete, prioritised improvements. For example: fine-tune a smaller model on domain data to reduce cost and improve accuracy; add function calling so the chatbot can take actions (not just answer questions); implement multi-modal support for image and PDF uploads.
