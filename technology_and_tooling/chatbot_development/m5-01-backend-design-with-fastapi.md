---
name: Backend Design with FastAPI
dependsOn:
  - technology_and_tooling.chatbot_development.m4-05-user-personalisation
tags: [chatbots, deployment, fastapi, api-design, optional]
learningOutcomes:
  - Design RESTful API endpoints for a chatbot service.
  - Implement a FastAPI backend with Pydantic request/response models.
  - Add real-time response streaming using Server-Sent Events.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    image: https://github.com/OxfordCompetencyCenters.png
    license: CC-BY-4.0
---

Up to this point, your chatbot has lived in a Python script or Jupyter notebook. It works on your machine, but no one else can use it. To turn it into a product — something accessible over the internet, integrated into other applications, or used by multiple people simultaneously — you need to wrap it in an API. The API is the boundary between your chatbot's intelligence and the outside world. Clients (web apps, mobile apps, other services) send HTTP requests to your API, and the API returns the chatbot's responses.

FastAPI has become the standard framework for building Python APIs in the machine learning space, and for good reason. It is fast — its performance is comparable to Node.js and Go frameworks, thanks to its async foundation on Starlette and uvicorn. It provides automatic request validation through Pydantic models, so malformed requests are rejected before they reach your chatbot logic. It generates interactive API documentation (Swagger UI) at `/docs` with no additional work. And it supports async natively, which is critical for chatbots: you don't want one slow LLM call to block every other user's request.

Other Python frameworks are viable alternatives. Flask is simpler and more mature but lacks built-in validation and async support. Django is full-featured but heavyweight for a pure API service. For chatbot backends specifically, FastAPI hits the sweet spot of simplicity, performance, and features.

## Designing Chatbot REST Endpoints

A well-designed chatbot API follows RESTful conventions. The core endpoints are:

```
POST   /chat              # Send a message, get a response
GET    /conversations     # List a user's conversations
GET    /conversations/:id # Get a specific conversation
DELETE /conversations/:id # Delete a conversation
POST   /conversations     # Start a new conversation
GET    /health            # Health check for monitoring
```

The `/chat` endpoint is the heart of the service. It accepts a message, an optional conversation ID (to continue an existing conversation), and a user ID. It returns the chatbot's response along with metadata: the conversation ID, any source documents used (if the chatbot uses RAG), and the number of tokens consumed.

```python
# POST /chat
# Request:
{
  "message": "How do I deploy my app?",
  "conversation_id": "conv_123",
  "user_id": "user_456"
}

# Response:
{
  "response": "To deploy your app, you can...",
  "conversation_id": "conv_123",
  "sources": ["doc1.pdf", "doc2.pdf"],
  "tokens_used": 450
}
```

Returning `sources` gives users transparency into where the chatbot's answer came from — this is especially important for RAG-powered chatbots where trust and verifiability matter. Returning `tokens_used` enables cost tracking on the client side, which is useful for billing or usage dashboards.

The `/health` endpoint is simple but essential. Load balancers, container orchestrators (like Kubernetes), and monitoring systems use it to determine whether your service is running and ready to accept traffic. If the health check fails, the platform can automatically restart the container or route traffic elsewhere.

Good REST API design also means using HTTP methods correctly (GET for reading, POST for creating, DELETE for removing), returning appropriate status codes (200 for success, 201 for creation, 400 for bad requests, 401 for authentication failures, 500 for server errors), and versioning your API (`/v1/chat`) so you can evolve the interface without breaking existing clients.

## Building the FastAPI Backend

Here is a complete implementation that wraps the hybrid memory chatbot from Module 4 in a FastAPI service:

```python
from fastapi import FastAPI, HTTPException
from pydantic import BaseModel
from typing import Optional, List
import uvicorn
import time

app = FastAPI(
    title="Chatbot API",
    description="Production chatbot with RAG and memory",
    version="1.0.0"
)

class ChatRequest(BaseModel):
    message: str
    conversation_id: Optional[str] = None
    user_id: str

class ChatResponse(BaseModel):
    response: str
    conversation_id: str
    sources: List[str] = []
    tokens_used: int

from your_chatbot import HybridMemoryChatbot

chatbot_instances = {}

@app.post("/chat", response_model=ChatResponse)
async def chat(request: ChatRequest):
    """Send a message to the chatbot and get a response."""
    try:
        conv_id = request.conversation_id or f"conv_{request.user_id}_{int(time.time())}"

        if conv_id not in chatbot_instances:
            chatbot_instances[conv_id] = HybridMemoryChatbot(user_id=request.user_id)

        chatbot = chatbot_instances[conv_id]
        result = chatbot.chat(request.message)

        return ChatResponse(
            response=result["answer"],
            conversation_id=conv_id,
            sources=result.get("sources", []),
            tokens_used=result.get("tokens_used", 0)
        )

    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@app.get("/conversations")
async def list_conversations(user_id: str):
    """List all conversations for a user."""
    user_convs = [
        conv_id for conv_id in chatbot_instances
        if conv_id.startswith(f"conv_{user_id}_")
    ]
    return {"conversations": user_convs}

@app.delete("/conversations/{conversation_id}")
async def delete_conversation(conversation_id: str):
    """Delete a conversation."""
    if conversation_id in chatbot_instances:
        del chatbot_instances[conversation_id]
        return {"status": "deleted"}
    raise HTTPException(status_code=404, detail="Conversation not found")

@app.get("/health")
async def health_check():
    """Health check endpoint."""
    return {"status": "healthy", "version": "1.0.0"}

if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8000)
```

The Pydantic models (`ChatRequest` and `ChatResponse`) serve double duty: they validate incoming requests automatically (rejecting any request that doesn't match the schema) and they generate the API documentation. When you run this server and visit `http://localhost:8000/docs`, you get an interactive Swagger UI where you can test every endpoint directly from the browser.

The `chatbot_instances` dictionary stores active chatbot instances in memory, keyed by conversation ID. This is sufficient for development and single-server deployments. In a production environment with multiple server instances, you would store conversation state in a shared database (PostgreSQL, Redis, or similar) so that any server can handle any request.

To run the server:

```bash
pip install fastapi uvicorn[standard]
python main.py
```

The API is then available at `http://localhost:8000`, with interactive documentation at `http://localhost:8000/docs`.

## Streaming Responses

LLM responses can take 5–10 seconds or more for long outputs. Without streaming, the user stares at a blank screen for the entire duration and then sees the complete response appear all at once. With streaming, tokens appear as they are generated — the same experience as ChatGPT. The perceived performance improvement is dramatic, even though the total time to generate the response is the same.

The standard approach for streaming in HTTP APIs is Server-Sent Events (SSE). SSE is simpler than WebSockets for this use case because the communication is one-directional: the server streams data to the client. The client opens a connection, and the server sends events as they become available.

```python
from fastapi.responses import StreamingResponse
import json

@app.post("/chat/stream")
async def chat_stream(request: ChatRequest):
    """Stream chatbot response token by token."""

    async def generate():
        conv_id = request.conversation_id or f"conv_{request.user_id}_{int(time.time())}"
        chatbot = chatbot_instances.get(conv_id) or HybridMemoryChatbot(request.user_id)
        chatbot_instances[conv_id] = chatbot

        # OpenAI streaming
        response = openai.ChatCompletion.create(
            model="gpt-3.5-turbo",
            messages=chatbot.build_messages(request.message),
            stream=True
        )

        for chunk in response:
            if chunk.choices[0].delta.get("content"):
                token = chunk.choices[0].delta.content
                yield f"data: {json.dumps({'token': token})}\n\n"

        yield f"data: {json.dumps({'done': True, 'conversation_id': conv_id})}\n\n"

    return StreamingResponse(generate(), media_type="text/event-stream")
```

The OpenAI API supports streaming natively through the `stream=True` parameter. Instead of returning the complete response, it yields chunks as they are generated. Each chunk contains a small piece of the response (typically one or a few tokens). The FastAPI `StreamingResponse` wraps the generator function and sends each yielded string to the client as an SSE event.

On the client side, JavaScript's `EventSource` API handles SSE connections:

```javascript
const eventSource = new EventSource('/chat/stream');

eventSource.onmessage = (event) => {
  const data = JSON.parse(event.data);

  if (data.done) {
    eventSource.close();
  } else {
    chatDisplay.textContent += data.token;
  }
};
```

Each token arrives as a separate event. The client appends it to the display, creating the characteristic "typing" effect. When the server sends the `done` signal, the client closes the connection. This pattern works well for chatbot interfaces because users can start reading the response immediately, which makes even slow LLM calls feel responsive.
