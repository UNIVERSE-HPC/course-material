---
name: Chatbot Development
id: chatbot_development
dependsOn: []
files:
  [
    m1-01-introduction-to-modern-chatbots.md,
    m1-02-llm-fundamentals.md,
    m1-03-api-integration-basics.md,
    m1-04-building-stateless-and-stateful-chatbots.md,
    m2-01-the-rag-paradigm.md,
    m2-02-vector-embeddings.md,
    m2-03-document-processing-and-chunking.md,
    m2-04-vector-databases.md,
    m2-05-building-a-rag-chatbot.md,
    m3-01-agentic-rag.md,
    m3-02-reranking-and-hybrid-search.md,
    m3-03-knowledge-graphs.md,
    m3-04-semantic-caching.md,
    m3-05-evaluating-and-optimising-retrieval.md,
    m4-01-conversation-history-management.md,
    m4-02-conversation-summarisation.md,
    m4-03-selective-memory.md,
    m4-04-retrieval-based-memory.md,
    m4-05-user-personalisation.md,
    m5-01-backend-design-with-fastapi.md,
    m5-02-frontend-integration.md,
    m5-03-docker-containerisation.md,
    m5-04-cloud-deployment.md,
    m5-05-authentication-and-security.md,
    m6-01-user-engagement-metrics.md,
    m6-02-conversation-analytics.md,
    m6-03-performance-monitoring.md,
    m6-04-logging-and-observability.md,
    m6-05-ab-testing-and-experimentation.md,
    m7-01-production-prompt-engineering.md,
    m7-02-safety-and-content-moderation.md,
    m7-03-cost-optimisation.md,
    m7-04-scaling-strategies.md,
    m7-05-alternative-llm-providers.md,
    m8-01-capstone-project-brief.md,
    m8-02-deliverables-and-documentation.md,
  ]
learningOutcomes:
  - Integrate an LLM (OpenAI) into a chatbot with stateful conversation and token-budget management.
  - Build a Retrieval-Augmented Generation pipeline over a ChromaDB vector store with source citation.
  - Apply advanced retrieval techniques including query expansion, re-ranking, hybrid search, knowledge graphs, and semantic caching.
  - Manage conversation context with windowing, rolling summarisation, selective memory, and retrieval-based memory.
  - Deliver a domain-specific Streamlit chatbot capstone, evaluated with retrieval and response-quality metrics.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    image: https://github.com/OxfordCompetencyCenters.png
    license: CC-BY-4.0
summary: |
  Hands-on training in building production-ready chatbots using Large Language Models (LLMs).
  Participants progress from fundamental concepts through Retrieval-Augmented Generation (RAG),
  memory management, and advanced retrieval techniques, finishing with a Streamlit-based capstone
  project. Three optional modules cover deployment, analytics, and production hardening.
---

# Chatbot Development Course

Hands-on training in building production-ready chatbots using Large Language Models (LLMs). Participants progress from fundamental concepts through advanced techniques including Retrieval-Augmented Generation (RAG), memory management, and advanced retrieval, culminating in a local Streamlit chatbot as the capstone.

**Duration:** 10 hours of core content (four core modules of 2.5 hours each) delivered as four half-day sessions on non-consecutive weeks. Three optional modules (deployment, analytics, production hardening) are self-paced.
**Level:** Intermediate
**Target audience:** Software engineers with no prior AI experience.

## Prerequisites

### Required technical skills

- **Python programming:** intermediate proficiency including classes, functions, error handling, and working with APIs.
- **Command line:** comfortable with terminal / shell operations, environment variables, and package management (pip).
- **Git & version control:** basic understanding of repositories, cloning, and commits.
- **JSON & REST APIs:** experience making HTTP requests and parsing JSON responses.

### Recommended background

- Basic understanding of machine learning concepts (helpful but not required).
- Familiarity with natural language processing terminology.
- Prior exposure to any API / backend framework (Flask, FastAPI, Django).

## Pre-course setup

Participants complete this once before the first session. The goal is that `00-environment-check.ipynb` in the companion repo runs top to bottom with every cell printing `OK:`.

1. Install Python 3.10+ and verify: `python --version`.
2. Clone the companion repository that holds the runnable notebooks and the Streamlit capstone:
   ```
   git clone https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules.git
   cd aicc_chatbot_modules
   ```
3. Create and activate a virtual environment:
   ```
   python -m venv .venv
   source .venv/bin/activate          # macOS / Linux
   .venv\Scripts\activate             # Windows PowerShell
   ```
4. Install the pinned dependencies:
   ```
   pip install -r requirements.txt
   ```
5. Register for an OpenAI API key via [Oxford API access](https://oerc.ox.ac.uk/ai-centre/generative-ai-tools/api-access).
6. Copy the example env file and paste your key:
   ```
   cp .env.example .env
   # edit .env and replace sk-replace-me with your actual key
   ```
7. Run the environment check:
   ```
   jupyter lab 00-environment-check.ipynb
   ```
   Execute every cell. If they all print `OK:`, you are ready for Module 1.

## Module overview

### Core modules (taught, four half-day sessions)

- **Module 1 — Chatbot Foundations & API Integration.** Evolution from rule-based to LLM-powered chatbots. LLM fundamentals (tokens, context windows, sampling parameters). OpenAI API integration. Stateless vs stateful chatbot design. System prompts and conversation management.
- **Module 2 — RAG: Vector Embeddings & Semantic Search.** Retrieval-Augmented Generation paradigm. Vector embeddings and semantic similarity. Document chunking strategies. Vector databases (ChromaDB). Building an end-to-end RAG chatbot with source citations.
- **Module 3 — Advanced RAG & Knowledge Graph Integration.** Agentic RAG with query expansion and decomposition. Re-ranking and hybrid search. Knowledge graphs for structured reasoning. Semantic caching for cost reduction. Retrieval quality metrics and evaluation.
- **Module 4 — Memory & Context Management.** Short-term vs long-term memory architectures. Conversation window management. Summarisation. Selective memory and importance scoring. Retrieval-based memory and cross-session recall. User profile building and personalisation.

### Optional modules (self-paced)

- **Module 5 — Deployment Architecture.** RESTful API design with FastAPI. Frontend development (Streamlit, React). Docker containerisation. Cloud deployment. Authentication and security. (A separate OxRSE course covers Docker in detail — participants are encouraged to take that alongside.)
- **Module 6 — Chatbot Analytics & Observability.** User engagement metrics. Conversation quality analytics. Performance monitoring. Structured logging and tracing. A/B testing.
- **Module 7 — Production Best Practices.** Production prompt engineering. Content moderation and safety. Cost optimisation. Scaling. Alternative LLM providers.

### Capstone

- **Module 8 — Capstone Project Integration.** Build a domain-specific chatbot that combines RAG over a ChromaDB vector store with conversation memory and auto-summarisation. The deliverable is a local Streamlit application launched with `streamlit run app.py`. Experiments evaluate retrieval quality and response quality; results are reported in a structured format alongside a reflection and documentation (README + Architecture Decision Records).

## Companion repository

Runnable notebooks (one per section), a pinned Python environment, a Streamlit capstone scaffold, and the `00-environment-check.ipynb` smoke test live in the companion repo: [OxfordCompetencyCenters/aicc_chatbot_modules](https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules). Each lesson in this course ends with a "Hands-on practical" link that points to the matching notebook.
