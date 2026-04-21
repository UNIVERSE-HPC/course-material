---
name: Logging and Observability
dependsOn:
  - technology_and_tooling.chatbot_development.m6-03-performance-monitoring
tags: [chatbots, analytics, logging, observability, tracing, optional]
learningOutcomes:
  - Implement structured JSON logging with trace IDs for production chatbot systems.
  - Set up distributed tracing with OpenTelemetry to visualise request flow.
  - Design a logging strategy that balances detail with cost and performance.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    image: https://github.com/OxfordCompetencyCenters.png
    license: CC-BY-4.0
---

Metrics tell you that something is wrong — latency increased, error rate spiked, cost jumped. Logs and traces tell you why. When a user reports that the chatbot gave a nonsensical answer, you need to reconstruct exactly what happened: what message did they send, what documents were retrieved, what prompt was constructed, what the LLM returned. Without structured logging, this investigation involves grepping through unstructured text files. With structured logging, it's a query on indexed fields.

## Structured Logging

The difference between unstructured and structured logging determines whether your logs are useful in production:

```python
# Unstructured — hard to search, impossible to aggregate
logging.info(f"User {user_id} sent message: {message}")

# Structured — searchable by any field, aggregatable
logging.info("user_message_received", extra={
    "user_id": user_id,
    "session_id": session_id,
    "message_length": len(message),
    "timestamp": datetime.now().isoformat()
})
```

Structured logs output JSON, which is natively parseable by log aggregation tools like Elasticsearch, CloudWatch, Datadog, and Grafana Loki. Every log entry has typed, named fields that you can filter, sort, and aggregate. This makes queries like "show me all requests where retrieval took longer than 1 second" or "find all errors for user X in the last hour" trivial.

Here is a production logging setup using `python-json-logger`:

```python
import logging
import os
from datetime import datetime
from pythonjsonlogger import jsonlogger

class CustomJsonFormatter(jsonlogger.JsonFormatter):
    """Custom JSON log formatter with service metadata."""

    def add_fields(self, log_record, record, message_dict):
        super().add_fields(log_record, record, message_dict)
        log_record['timestamp'] = datetime.utcnow().isoformat()
        log_record['service'] = 'chatbot-api'
        log_record['environment'] = os.getenv('ENVIRONMENT', 'development')

        if hasattr(record, 'trace_id'):
            log_record['trace_id'] = record.trace_id

logger = logging.getLogger()
handler = logging.StreamHandler()
formatter = CustomJsonFormatter('%(timestamp)s %(level)s %(name)s %(message)s')
handler.setFormatter(formatter)
logger.addHandler(handler)
logger.setLevel(logging.INFO)
```

The key practice is to log at every decision point in the request lifecycle, with a trace ID that ties all logs from a single request together:

```python
@app.post("/chat")
async def chat(request: ChatRequest):
    trace_id = generate_trace_id()

    logger.info("chat_request_received", extra={
        "trace_id": trace_id,
        "user_id": request.user_id,
        "session_id": request.conversation_id,
        "message_length": len(request.message)
    })

    try:
        start = time.time()
        documents = rag.retrieve(request.message)
        retrieval_time = time.time() - start

        logger.info("rag_retrieval_completed", extra={
            "trace_id": trace_id,
            "num_documents": len(documents),
            "retrieval_time_ms": retrieval_time * 1000
        })

        start = time.time()
        response = llm.generate(documents, request.message)
        generation_time = time.time() - start

        logger.info("llm_generation_completed", extra={
            "trace_id": trace_id,
            "response_length": len(response),
            "generation_time_ms": generation_time * 1000,
            "model": "gpt-3.5-turbo"
        })

        logger.info("chat_request_completed", extra={
            "trace_id": trace_id,
            "total_time_ms": (retrieval_time + generation_time) * 1000,
            "status": "success"
        })

        return {"response": response, "trace_id": trace_id}

    except Exception as e:
        logger.error("chat_request_failed", extra={
            "trace_id": trace_id,
            "error": str(e),
            "error_type": type(e).__name__
        }, exc_info=True)
        raise
```

Each log entry produces a JSON object like:

```json
{
  "timestamp": "2024-01-15T10:30:45.123Z",
  "level": "INFO",
  "message": "rag_retrieval_completed",
  "service": "chatbot-api",
  "environment": "production",
  "trace_id": "abc-123-def",
  "num_documents": 5,
  "retrieval_time_ms": 452.3
}
```

When something goes wrong, you search by `trace_id` and see the complete lifecycle of that request: what was received, what was retrieved, how long generation took, and where it failed. The fields to include in every log entry are: `trace_id` (ties the request together), `user_id` (identifies who is affected), `session_id` (groups related requests), the operation name (what happened), `duration_ms` (how long it took), and `status` (success or failure with error details).

A practical note on cost: log aggregation services charge by volume. Don't log the full text of every message and response — that can be enormous at scale. Log metadata (lengths, timings, IDs) and store full message content in your application database where it's needed for debugging specific issues.

## Distributed Tracing with OpenTelemetry

In a microservices architecture — where the chat request might pass through an API gateway, a retrieval service, an LLM proxy, and a memory service — structured logging alone isn't enough. You need distributed tracing to visualise how a single request flows across services.

OpenTelemetry is the industry standard for distributed tracing. It creates spans for each operation, nests them to show the call hierarchy, and exports them to visualisation tools like Jaeger or Zipkin.

```python
from opentelemetry import trace
from opentelemetry.sdk.trace import TracerProvider
from opentelemetry.sdk.trace.export import BatchSpanProcessor
from opentelemetry.exporter.jaeger import JaegerExporter
from opentelemetry.instrumentation.fastapi import FastAPIInstrumentor

trace.set_tracer_provider(TracerProvider())
tracer = trace.get_tracer(__name__)

jaeger_exporter = JaegerExporter(
    agent_host_name="localhost",
    agent_port=6831,
)
trace.get_tracer_provider().add_span_processor(
    BatchSpanProcessor(jaeger_exporter)
)

# Automatically instrument all FastAPI endpoints
FastAPIInstrumentor.instrument_app(app)

# Add manual instrumentation for custom operations
@app.post("/chat")
async def chat(request: ChatRequest):
    with tracer.start_as_current_span("chat_request") as span:
        span.set_attribute("user_id", request.user_id)
        span.set_attribute("message_length", len(request.message))

        with tracer.start_as_current_span("rag_retrieval"):
            documents = rag.retrieve(request.message)
            span.set_attribute("documents_retrieved", len(documents))

        with tracer.start_as_current_span("llm_generation"):
            span.set_attribute("model", "gpt-3.5-turbo")
            response = llm.generate(documents, request.message)
            span.set_attribute("response_length", len(response))

        with tracer.start_as_current_span("memory_update"):
            memory.store(request.user_id, request.message, response)

        span.set_attribute("status", "success")
        return {"response": response}
```

In the Jaeger or Zipkin UI, a single request appears as a waterfall of nested spans:

```
Chat Request (2.4s)
├─ RAG Retrieval (450ms)
│  ├─ Vector Search (380ms)
│  └─ Re-ranking (70ms)
├─ LLM Generation (1.8s)
│  └─ OpenAI API Call (1.75s)
└─ Memory Update (150ms)
   ├─ Database Write (100ms)
   └─ Cache Update (50ms)
```

This visualisation makes bottlenecks obvious at a glance. The span attributes enable powerful filtering: "show traces where documents_retrieved is greater than 10" to isolate expensive retrieval operations, or "show traces where the LLM generation span exceeds 5 seconds" to investigate timeouts. For teams operating chatbots in production, distributed tracing is the single most valuable debugging tool — it turns opaque "the bot was slow" reports into specific, actionable engineering tasks.
