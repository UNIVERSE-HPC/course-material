---
name: Authentication and Security
dependsOn:
  - technology_and_tooling.chatbot_development.m5-04-cloud-deployment
tags: [chatbots, deployment, authentication, jwt, security, optional]
learningOutcomes:
  - Implement JWT-based authentication for a FastAPI chatbot backend.
  - Manage secrets and environment variables securely across environments.
  - Apply security best practices for production chatbot deployments.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    image: https://github.com/OxfordCompetencyCenters.png
    license: CC-BY-4.0
---

A publicly deployed chatbot API without authentication is an open invitation for abuse. Anyone who discovers the URL can send unlimited requests, consuming your LLM API credits and potentially accessing other users' conversation data. Authentication verifies that each request comes from a legitimate user, and authorisation ensures they can only access their own data.

## JWT Authentication

JSON Web Tokens (JWT) are the standard approach for stateless API authentication. A JWT is a signed token that contains encoded claims — typically a user identifier and an expiration timestamp. The server issues a token when the user logs in, and the client includes that token in the `Authorization` header of every subsequent request. The server validates the token's signature and expiration without needing to look up a session in a database, which makes JWT inherently scalable.

A JWT consists of three parts separated by dots: a header (specifying the algorithm), a payload (containing claims like user ID and expiration), and a signature (proving the token hasn't been tampered with). The token looks like this:

```
eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.
eyJ1c2VyX2lkIjoiMTIzIiwiZXhwIjoxNjk5OTk5OTk5fQ.
SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c
```

The authentication flow works as follows. The user sends their credentials (username and password) to a login endpoint. The server validates the credentials, creates a JWT containing the user's identity and an expiration time, and returns it. The client stores the token and includes it as a `Bearer` token in the `Authorization` header of all subsequent requests. The server extracts the token from the header, verifies its signature and expiration, and either processes the request or returns a 401 Unauthorized error.

Here is a complete implementation in FastAPI:

```python
from fastapi import FastAPI, Depends, HTTPException, status
from fastapi.security import HTTPBearer, HTTPAuthorizationCredentials
from pydantic import BaseModel
from datetime import datetime, timedelta
import jwt
import bcrypt
import os

app = FastAPI()

SECRET_KEY = os.getenv("SECRET_KEY", "change-this-in-production")
ALGORITHM = "HS256"
ACCESS_TOKEN_EXPIRE_MINUTES = 60

security = HTTPBearer()

class User(BaseModel):
    username: str
    password: str

class Token(BaseModel):
    access_token: str
    token_type: str

# In production, use a real database
fake_users_db = {
    "demo": {
        "username": "demo",
        "password_hash": bcrypt.hashpw(b"password123", bcrypt.gensalt()).decode()
    }
}

def create_access_token(data: dict):
    """Create a JWT token."""
    to_encode = data.copy()
    expire = datetime.utcnow() + timedelta(minutes=ACCESS_TOKEN_EXPIRE_MINUTES)
    to_encode.update({"exp": expire})
    return jwt.encode(to_encode, SECRET_KEY, algorithm=ALGORITHM)

def verify_token(token: str):
    """Verify and decode a JWT token."""
    try:
        payload = jwt.decode(token, SECRET_KEY, algorithms=[ALGORITHM])
        username: str = payload.get("sub")
        if username is None:
            raise HTTPException(status_code=401, detail="Invalid token")
        return username
    except jwt.ExpiredSignatureError:
        raise HTTPException(status_code=401, detail="Token expired")
    except jwt.JWTError:
        raise HTTPException(status_code=401, detail="Invalid token")

def get_current_user(credentials: HTTPAuthorizationCredentials = Depends(security)):
    """FastAPI dependency that extracts and validates the current user."""
    return verify_token(credentials.credentials)

@app.post("/login", response_model=Token)
async def login(user: User):
    """Authenticate and return a JWT token."""
    db_user = fake_users_db.get(user.username)

    if not db_user:
        raise HTTPException(status_code=401, detail="Invalid credentials")

    if not bcrypt.checkpw(user.password.encode(), db_user["password_hash"].encode()):
        raise HTTPException(status_code=401, detail="Invalid credentials")

    access_token = create_access_token(data={"sub": user.username})
    return Token(access_token=access_token, token_type="bearer")

@app.post("/chat")
async def chat(
    request: ChatRequest,
    current_user: str = Depends(get_current_user)
):
    """Protected chat endpoint — requires authentication."""
    # current_user is automatically extracted from the token
    # Use it to scope conversations to the authenticated user
    # ... your chatbot logic ...
    return {"response": "Hello!", "user": current_user}

@app.get("/me")
async def get_user_info(current_user: str = Depends(get_current_user)):
    """Return the current user's information."""
    return {"username": current_user}
```

The `get_current_user` function is a FastAPI dependency. By including `current_user: str = Depends(get_current_user)` in any endpoint's parameters, FastAPI automatically extracts the token from the `Authorization` header, validates it, and provides the username to the endpoint function. If the token is missing, expired, or invalid, FastAPI returns a 401 error before the endpoint logic runs.

Client usage looks like this:

```python
import requests

# Login to get a token
response = requests.post("http://localhost:8000/login", json={
    "username": "demo",
    "password": "password123"
})
token = response.json()["access_token"]

# Use the token for subsequent requests
headers = {"Authorization": f"Bearer {token}"}

chat_response = requests.post(
    "http://localhost:8000/chat",
    json={"message": "Hello!"},
    headers=headers
)
```

For a production system, you would add several enhancements: a real user database (PostgreSQL or similar), refresh tokens for long sessions (so users don't need to re-login every 60 minutes), password strength requirements, rate limiting on the login endpoint to prevent brute-force attacks, and account lockout after repeated failed attempts.

## Managing Secrets and Environment Variables

One of the most common security mistakes is hardcoding secrets — API keys, database passwords, JWT signing keys — directly in source code. If the code is pushed to a public repository or the Docker image is inspected, the secrets are exposed.

The correct approach uses environment variables. In code, read secrets from the environment:

```python
from dotenv import load_dotenv
import os

load_dotenv()  # Load from .env file for local development

openai_key = os.getenv("OPENAI_API_KEY")
database_url = os.getenv("DATABASE_URL")
secret_key = os.getenv("SECRET_KEY")
```

For local development, store secrets in a `.env` file:

```
OPENAI_API_KEY=sk-abc123...
DATABASE_URL=postgresql://user:pass@localhost/chatbot
SECRET_KEY=your-jwt-secret
ENVIRONMENT=development
```

This file must never be committed to version control. Add it to `.gitignore`:

```
.env
.env.local
.env.*.local
```

For production, use your cloud provider's secret management service. These services encrypt secrets at rest, provide fine-grained access control, maintain audit logs of who accessed what, and support automatic secret rotation.

```bash
# AWS Secrets Manager
aws secretsmanager create-secret \
  --name chatbot/openai-key \
  --secret-string sk-abc123...

# Google Secret Manager
gcloud secrets create openai-key \
  --data-file=- <<< "sk-abc123..."

# Azure Key Vault
az keyvault secret set \
  --vault-name myvault \
  --name openai-key \
  --value sk-abc123...
```

You can also pass environment variables directly during deployment:

```bash
# Cloud Run
gcloud run deploy chatbot-api \
  --set-env-vars OPENAI_API_KEY=sk-...

# Kubernetes
kubectl create secret generic chatbot-secrets \
  --from-literal=openai-key=sk-...
```

To access secrets from a cloud secret manager in your application code:

```python
import boto3

def get_secret(secret_name):
    """Retrieve a secret from AWS Secrets Manager."""
    client = boto3.client('secretsmanager')
    response = client.get_secret_value(SecretId=secret_name)
    return response['SecretString']

openai_key = get_secret("chatbot/openai-key")
```

The principle is simple: secrets should never appear in source code, Docker images, or version control. They should be injected at runtime through environment variables or retrieved from a secret manager.

::::challenge{id=m5-05-ex1 title="Deploy Your Chatbot"}

Build a complete deployment stack for your capstone chatbot.

**Step 1: Backend API (30 minutes).** Create a FastAPI application with `/chat`, `/health`, and `/conversations` endpoints. Use Pydantic models for request/response validation. Verify the auto-generated documentation at `/docs`.

**Step 2: Frontend (20 minutes).** Build a Streamlit interface that communicates with your FastAPI backend. Implement chat history display, message input, and a clear conversation button.

**Step 3: Docker (20 minutes).** Write a Dockerfile for your backend. Create a `docker-compose.yml` that runs both the backend and a PostgreSQL database. Verify the stack starts with `docker-compose up`.

**Step 4: Cloud Deployment (15 minutes).** Deploy your backend to Google Cloud Run (or an alternative platform like Render). Verify the deployed URL responds to requests.

**Step 5 (Bonus): Authentication (15 minutes).** Add JWT authentication to your API. Implement a `/login` endpoint and protect the `/chat` endpoint.
::::

::::challenge{id=m5-05-ex2 title="Project Checkpoint 5: Deploy Your Chatbot"}

Your capstone project should now be a fully deployed application accessible via a public URL.

Deliverables:

- FastAPI backend with properly designed endpoints (`/chat`, `/health`, `/conversations`)
- Frontend interface (Streamlit or React) connected to the backend
- Dockerfile and `docker-compose.yml` for local development
- Deployed backend with a public URL
- Environment variables and secrets properly managed (not hardcoded)
- (Bonus) JWT authentication on protected endpoints
- Deployment documentation: setup instructions, API documentation (Swagger), and deployment guide

Test your deployment against this checklist:

1. The API responds correctly to chat requests.
2. The frontend communicates with the backend and displays responses.
3. Docker containers start without errors using `docker-compose up`.
4. The deployed URL is accessible from the internet.
5. No secrets are hardcoded in source code or Docker images.
6. The health check endpoint returns a successful response.
::::

## Summary and What's Next

In this module, you've taken your chatbot from a local script to a deployed application. You designed a RESTful API with FastAPI, providing a clean interface for clients to interact with your chatbot. You built frontend interfaces — Streamlit for rapid prototyping and React for production — that connect to the API. You containerised the application with Docker, ensuring it runs identically everywhere. You deployed it to the cloud, making it accessible to users on the internet. And you secured it with JWT authentication and proper secret management.

In Module 6, you'll focus on understanding how your deployed chatbot is performing. You'll implement analytics to track user engagement, conversation quality, and system performance. You'll set up logging and monitoring to detect issues before users report them. And you'll build A/B testing frameworks to measure the impact of changes to your chatbot's behaviour.

## Additional Resources

- [FastAPI Documentation](https://fastapi.tiangolo.com/) — Comprehensive guide to FastAPI features
- [Docker Documentation](https://docs.docker.com/) — Official Docker reference
- ["The Twelve-Factor App"](https://12factor.net/) — Principles for building modern, deployable applications
- [Google Cloud Run Documentation](https://cloud.google.com/run/docs) — Cloud Run setup and configuration
- ["Building Microservices"](https://www.oreilly.com/library/view/building-microservices-2nd/9781492034018/) by Sam Newman — Architecture patterns for distributed systems
- [Postman](https://www.postman.com/) — API testing and documentation tool
