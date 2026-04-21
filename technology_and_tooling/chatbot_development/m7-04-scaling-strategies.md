---
name: Scaling Strategies
dependsOn:
  - technology_and_tooling.chatbot_development.m7-03-cost-optimisation
tags: [chatbots, scaling, redis, load-balancing, rate-limiting, optional]
learningOutcomes:
  - Design stateless chatbot services that support horizontal scaling.
  - Externalise conversation state to Redis for distributed deployments.
  - Implement rate limiting and backpressure to protect the system under load.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    image: https://github.com/OxfordCompetencyCenters.png
    license: CC-BY-4.0
---

As your chatbot gains users, a single server instance will eventually become a bottleneck. LLM API calls take seconds, and each request ties up server resources while waiting for the response. With 100 concurrent users and 3-second response times, your server is processing roughly 33 requests per second — manageable for a single instance. At 1,000 concurrent users, you need multiple server instances sharing the load. Horizontal scaling — adding more server instances behind a load balancer — is the standard approach, but it requires your application to be stateless.

## Stateless Design

The challenge with horizontal scaling is state. If Server A holds User 1's conversation history in memory, and the load balancer routes User 1's next request to Server B, the conversation context is lost. Server B doesn't know what was discussed on Server A.

The solution is to externalise all state to a shared data store. Redis is the standard choice for conversation state because it's fast (sub-millisecond reads and writes), supports expiration (conversations can automatically expire after inactivity), and is designed for this exact use case.

```python
import redis
import json
from datetime import timedelta

class DistributedMemory:
    def __init__(self):
        self.redis = redis.Redis(
            host='redis-server',
            port=6379,
            decode_responses=True
        )

    def store_conversation(self, session_id, messages):
        """Store conversation in Redis with 24-hour expiration."""
        key = f"conversation:{session_id}"
        self.redis.setex(
            key,
            timedelta(hours=24),
            json.dumps(messages)
        )

    def get_conversation(self, session_id):
        """Retrieve conversation from Redis."""
        key = f"conversation:{session_id}"
        data = self.redis.get(key)
        if data:
            return json.loads(data)
        return []

    def append_message(self, session_id, role, content):
        """Append a message to a conversation."""
        messages = self.get_conversation(session_id)
        messages.append({"role": role, "content": content})
        self.store_conversation(session_id, messages)
```

With externalised state, the chat endpoint becomes truly stateless — any server instance can handle any request:

```python
memory = DistributedMemory()

@app.post("/chat")
async def chat(request: ChatRequest):
    messages = memory.get_conversation(request.session_id)
    messages.append({"role": "user", "content": request.message})

    response = llm.generate(messages=messages)

    messages.append({"role": "assistant", "content": response})
    memory.store_conversation(request.session_id, messages)

    return {"response": response}
```

## Load Balancing

With stateless servers, you can run multiple instances behind a load balancer. NGINX is a common choice:

```yaml
# docker-compose.yml
services:
  nginx:
    image: nginx
    ports:
      - "80:80"
    volumes:
      - ./nginx.conf:/etc/nginx/nginx.conf

  backend1:
    build: ./backend
    environment:
      - REDIS_URL=redis://redis:6379

  backend2:
    build: ./backend
    environment:
      - REDIS_URL=redis://redis:6379

  backend3:
    build: ./backend
    environment:
      - REDIS_URL=redis://redis:6379

  redis:
    image: redis:7-alpine
```

```nginx
upstream backend {
    least_conn;
    server backend1:8000;
    server backend2:8000;
    server backend3:8000;
}

server {
    listen 80;

    location / {
        proxy_pass http://backend;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
    }
}
```

The `least_conn` directive routes each request to the server with the fewest active connections, distributing load evenly. Scaling is now a matter of adding more backend containers. All instances connect to the same Redis instance, so they share conversation state seamlessly.

For very high scale, Redis itself can become a bottleneck. Redis Cluster distributes data across multiple Redis nodes. Alternatively, PostgreSQL with connection pooling (using PgBouncer) provides a more durable storage option at the cost of slightly higher latency.

## Rate Limiting and Backpressure

Without rate limiting, a single user (or an attacker) can consume all your server capacity and LLM API budget. Rate limiting caps the number of requests per user per time window. The `slowapi` library integrates cleanly with FastAPI:

```python
from slowapi import Limiter, _rate_limit_exceeded_handler
from slowapi.util import get_remote_address
from slowapi.errors import RateLimitExceeded

limiter = Limiter(key_func=get_remote_address)
app.state.limiter = limiter
app.add_exception_handler(RateLimitExceeded, _rate_limit_exceeded_handler)

@app.post("/chat")
@limiter.limit("10/minute")
async def chat(request: Request, chat_request: ChatRequest):
    return response
```

This limits each IP address to 10 requests per minute. For authenticated users, you can rate limit per user ID instead, allowing higher limits for paying customers and stricter limits for free-tier users.

Backpressure handles traffic spikes by limiting the number of concurrent LLM calls. Without backpressure, a traffic spike sends hundreds of simultaneous requests to the LLM API, all of which time out or fail. With backpressure, a semaphore limits concurrent processing, and excess requests are queued:

```python
import asyncio
from asyncio import Queue

class RequestQueue:
    def __init__(self, max_concurrent=10):
        self.queue = Queue()
        self.semaphore = asyncio.Semaphore(max_concurrent)

    async def process_request(self, request):
        """Process a request with concurrency limit."""
        async with self.semaphore:
            response = await generate_response(request)
            return response

    async def enqueue(self, request):
        """Add a request to the queue."""
        await self.queue.put(request)

    async def worker(self):
        """Worker that processes queued requests."""
        while True:
            request = await self.queue.get()
            try:
                response = await self.process_request(request)
            except Exception as e:
                logger.error(f"Request failed: {e}")
            finally:
                self.queue.task_done()

request_queue = RequestQueue(max_concurrent=10)

@app.on_event("startup")
async def startup():
    for _ in range(5):
        asyncio.create_task(request_queue.worker())
```

The semaphore ensures that at most 10 LLM calls are in flight at any time. Additional requests wait in the queue rather than overwhelming the system. For user-facing applications where waiting in a queue is acceptable, this provides graceful degradation under load. For applications where latency is critical, return a job ID immediately and let the client poll for the result — this decouples request submission from response delivery.
