---
name: Deliverables and Documentation
dependsOn:
  - technology_and_tooling.chatbot_development.m8-01-capstone-project-brief
tags: [chatbots, documentation, architecture-decision-records, code-quality, capstone, deliverables]
learningOutcomes:
  - Write a professional README that enables someone to understand, install, and use the project in under 15 minutes.
  - Create Architecture Decision Records that document the rationale behind key technical choices.
  - Apply code quality standards including type hints, docstrings, and input validation.
  - Compile and verify a complete set of capstone deliverables covering code, documentation, experimentation, and results.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    license: CC-BY-4.0
---

Documentation is how you communicate your project's goal, methodology, results, and conclusions to others. A working chatbot with no documentation is a black box — no one can run it, evaluate it, or build on it. This section covers the documentation standards your capstone should meet, the code quality expectations, and the final submission checklist.

## Writing a Professional README

The README is the front door of your project. A good README answers six questions: What does this project do? How do I install it? How do I use it? How is it structured? How do I contribute? What are its limitations?

The structure below covers these questions:

```markdown
# [Project Name] - AI-Powered Chatbot

## Overview
[2-3 sentence description of what your chatbot does and who it's for]

**Key Features:**
- RAG-powered knowledge retrieval from [domain] documents
- Conversation memory with auto-summarisation for long sessions
- Source citations for every answer

**Tech Stack:**
- Language: Python 3.10
- Web UI: Streamlit (local browser app)
- Vector DB: ChromaDB (local, persistent)
- LLM: OpenAI GPT-3.5-turbo (or equivalent chat model)
- Embeddings: `text-embedding-3-small` (or sentence-transformers)

## Architecture

[Include your architecture diagram here]

**Core Components:**
- **Chatbot Core:** Orchestrates each turn — assembles the prompt and calls the LLM.
- **RAG Engine:** Ingests the document corpus into ChromaDB and retrieves relevant chunks at query time.
- **Memory Manager:** Maintains conversation history and automatically summarises older turns once a configured threshold is exceeded.

## Quick Start

### Prerequisites
- Python 3.10+
- OpenAI API key

### Installation

1. Clone the repository:
git clone https://github.com/yourusername/your-chatbot.git
cd your-chatbot

2. Create a virtual environment and install dependencies:
python -m venv .venv
source .venv/bin/activate  # or .venv\Scripts\activate on Windows
pip install -r requirements.txt

3. Set up environment variables:
cp .env.example .env
# Edit .env and add your OPENAI_API_KEY

4. Ingest the document corpus into ChromaDB:
python -m chatbot.ingest --source data/documents

5. Run the chatbot:
streamlit run app.py
# then open http://localhost:8501 in your browser

## Usage

Once `streamlit run app.py` is running, open the URL it prints (usually `http://localhost:8501`). You should see:

- A chat input at the bottom of the page.
- Previous messages rendered as a conversation above the input.
- A sidebar with a "Reset conversation" button and any configuration toggles (e.g. show retrieved sources).

### Example session

    You: What is the refund policy?
    Assistant: Refunds are available within 30 days of purchase, provided the item
               is unused.
               Sources: returns-policy.md

    You: And for digital products?
    Assistant: Digital products are non-refundable once downloaded.
               Sources: returns-policy.md

### Programmatic example (for tests)

    from chatbot import Chatbot

    bot = Chatbot()
    reply = bot.chat("What is the refund policy?")
    print(reply.text)
    print(reply.sources)

## Configuration

### Environment Variables
- OPENAI_API_KEY: Your OpenAI API key (required)
- MODEL_NAME: LLM model to use (default: gpt-3.5-turbo)
- MAX_TOKENS: Max response length (default: 500)
- CHROMA_DB_PATH: Path to the local ChromaDB persistence directory
- SUMMARY_THRESHOLD: Number of turns before older history is summarised (default: 10)

## Project Structure
├── app.py                   # Streamlit entry point
├── chatbot/
│   ├── __init__.py
│   ├── core.py              # Chatbot orchestration (called from app.py)
│   ├── rag.py               # RAG over ChromaDB
│   ├── memory.py            # Memory with auto-summarisation
│   ├── llm.py               # OpenAI client wrapper
│   └── ingest.py            # Corpus ingestion script
├── data/
│   └── documents/           # Source documents
├── tests/                   # Unit tests
├── requirements.txt
├── .env.example
└── README.md

## Running Tests
pytest tests/

## Performance
- Average response time: ~2 seconds per query
- Cost: ~$0.002 per conversation (average)
- 20-turn conversations stay within the model's context window via auto-summarisation

## Limitations
- Only supports English language
- Document ingestion limited to text and PDF
- Runs locally on a single machine — no multi-user hosting
```

The overview communicates what the project does and who it's for. The quick start provides copy-paste commands to get the project running. The example session lets a reader see the chatbot's behaviour without running it themselves. The limitations section demonstrates understanding of the system's boundaries.

Include a `.env.example` file in your repository listing all required environment variables with placeholder values. Never commit actual secrets.

## Architecture Decision Records

An Architecture Decision Record (ADR) documents a significant technical decision: what was decided, why, what alternatives were considered, and what the consequences are. ADRs capture reasoning that would otherwise be lost.

Here is the ADR format:

```markdown
# ADR 001: Use ChromaDB for Vector Storage

## Context
Need to store and search document embeddings for RAG. The system will
initially serve a small to medium document corpus (under 100,000 vectors)
with potential to grow.

## Decision
Use ChromaDB for local development and initial production.

## Rationale
- Simple setup with no separate server required
- Good performance for small to medium scale (<1M vectors)
- Free and open-source
- Easy migration path to Pinecone if scale demands it

## Consequences
- **Positive:** Fast development cycle, no infrastructure management
- **Negative:** Limited to single-machine scale
- **Mitigation:** Plan migration to Pinecone if scale exceeds 500K vectors

## Alternatives Considered
- Pinecone: Too expensive for MVP stage
- Weaviate: More complex setup for comparable features
- FAISS: No persistent storage out of the box
```

Write at least one ADR for a significant decision — two or three is ideal. Good candidates: vector database choice (ChromaDB), summarisation strategy (when to trigger, what to keep verbatim, what to collapse), chunk size and retrieval top-k, embedding model selection. Place them in a `docs/adr/` directory.

The key to a useful ADR is the "Alternatives Considered" section. It shows the decision was deliberate — you evaluated options and chose the best fit for your constraints.

## Code Quality Standards

Each component should live in its own module: separate files for RAG, memory, LLM integration, and API endpoints. Use clear naming (`get_user_conversation()` not `func1()`). Avoid duplication.

**Code review checklist** before submission:

- No hardcoded secrets (API keys, passwords) anywhere in the codebase
- No commented-out code (use git history to recover old code)
- Consistent formatting (use Black for Python)
- Error handling for all external calls (OpenAI API, ChromaDB, file system)
- Basic logging for key operations (ingestion, retrieval, summarisation events)
- Type hints for function signatures
- Docstrings for public functions
- No debug print statements in submitted code

The following example demonstrates production-quality Python code:

```python
def retrieve_relevant_documents(
    query: str,
    top_k: int = 5,
    min_similarity: float = 0.7
) -> List[Dict[str, Any]]:
    """
    Retrieve documents relevant to the query using semantic search.

    Args:
        query: User's question or search query.
        top_k: Maximum number of documents to retrieve.
        min_similarity: Minimum cosine similarity threshold (0-1).

    Returns:
        List of documents with metadata, sorted by relevance.

    Raises:
        ValueError: If top_k < 1 or min_similarity not in [0, 1].
        VectorDBError: If vector database is unreachable.
    """
    if top_k < 1:
        raise ValueError(f"top_k must be >= 1, got {top_k}")

    if not 0 <= min_similarity <= 1:
        raise ValueError(
            f"min_similarity must be in [0, 1], got {min_similarity}"
        )

    try:
        query_embedding = embedder.embed(query)

        results = vector_db.search(
            query_embedding,
            top_k=top_k,
            filter={"similarity": {"$gte": min_similarity}}
        )

        logger.info("document_retrieval", extra={
            "query_length": len(query),
            "top_k": top_k,
            "results_found": len(results)
        })

        return results

    except VectorDBConnectionError as e:
        logger.error(f"Vector DB connection failed: {e}")
        raise VectorDBError("Unable to retrieve documents") from e
```

This demonstrates type hints, a complete docstring, input validation, structured logging, and specific exception handling. Run a linter (pylint or flake8) before submitting. Remove debug print statements. Add missing type hints and docstrings.

## Submission Checklist

Use this checklist as a final quality gate.

**Code:**
- GitHub repository with all code, public and accessible
- README.md with setup instructions and usage examples
- `.env.example` with all required environment variables listed (no actual secrets)
- `requirements.txt` with pinned dependency versions
- Ingestion script that populates ChromaDB from the document corpus
- `app.py` Streamlit entry point that runs the full chat loop in the browser

**Documentation:**
- Architecture diagram included in the README
- At least 1 Architecture Decision Record for a key technical choice (2–3 is ideal)
- Project goal, objectives, and methodology documented

**Experimentation and Results:**
- Retrieval quality evaluated against a set of test queries with known answers
- Response quality assessed (correct, partially correct, incorrect)
- Robustness tested across edge cases (empty input, long input, gibberish, off-topic)
- Performance benchmarks documented (average latency, 20-turn conversation stays within context)
- Results presented in tables with quantitative metrics
- Baseline comparison included where applicable (e.g. with / without RAG, with / without auto-summarisation)

**Reflection:**
- Conclusion documenting what worked, what didn't, and key challenges
- Lessons learned and what you would do differently
- Future improvements listed (concrete and prioritised)

**Optional (strengthens the project):**
- User testing feedback from real or simulated users
- Ablation comparing different chunk sizes or summarisation thresholds
- Video demo of the Streamlit app in use (5–10 minutes)

::::challenge{id=m8-02-final-integration title="Final Integration and Evaluation"}

Work through this exercise in four phases.

**Phase 1: Integration Testing (20 minutes).** Start the Streamlit app with `streamlit run app.py` and verify the end-to-end flow locally. Send a message through the chat input, confirm RAG retrieval over ChromaDB and LLM generation work correctly, and verify the response appears with source citations. Test that conversation memory persists across reruns (Streamlit re-executes the script on every interaction — session state must survive) and that auto-summarisation kicks in once the configured threshold is crossed. Fix any broken connections.

**Phase 2: Run Experiments (30 minutes).** Execute the experiments defined in the project brief. Run your 5–10 test queries through the system and record retrieval hit rates and response quality ratings. Test the robustness scenarios (empty input, gibberish, off-topic, very long input). Record the average latency across those same queries. Run a 20-turn conversation and confirm token counts stay within the model's context window.

**Phase 3: Document Results (20 minutes).** Compile your experimental results into the tables described in the project brief. Write the conclusion: what worked, what didn't, key challenges, lessons learned, and future improvements. Add the results to your project documentation.

**Phase 4: Polish (10 minutes).** Clean up the codebase: remove debug print statements, add missing type hints and docstrings, run a formatter (Black), and verify error messages are helpful. Walk through the submission checklist and check off each item.
::::

::::challenge{id=m8-02-checkpoint title="Project Checkpoint 8: Complete Capstone"}

Your capstone project should now be complete.

Deliverables:

- All components integrated and working end-to-end locally
- README with project overview, setup instructions, and architecture diagram
- At least 1 ADR documenting a key technical decision (2–3 is ideal)
- Experimental results documented with quantitative metrics
- Conclusion with reflection on what worked, what didn't, and future improvements
- Code cleaned up, formatted, and linted
- Submission checklist completed
::::

## Additional Resources

- [arXiv](https://arxiv.org/) — Preprint server for AI/ML research papers
- [OpenAI Blog](https://openai.com/blog) — Updates on models, APIs, and best practices
- [Anthropic Blog](https://www.anthropic.com/blog) — Research and engineering insights
- [Hugging Face Blog](https://huggingface.co/blog) — Open-source model releases and tutorials
- [Fast.ai](https://www.fast.ai/) — Practical deep learning courses
- [DeepLearning.AI](https://www.deeplearning.ai/) — Specialised AI/ML courses
- ["Building LLM Applications for Production"](https://huyenchip.com/2023/04/11/llm-engineering.html) by Chip Huyen — Practical LLM production engineering
