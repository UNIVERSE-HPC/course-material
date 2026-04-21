---
name: Docker Containerisation
dependsOn:
  - technology_and_tooling.chatbot_development.m5-02-frontend-integration
tags: [chatbots, deployment, docker, containerisation, optional]
learningOutcomes:
  - Write a production-quality Dockerfile for a Python chatbot API.
  - Use multi-stage builds to reduce Docker image size.
  - Define multi-container development environments with Docker Compose.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    license: CC-BY-4.0
---

Your chatbot runs on your machine. It requires a specific Python version, a set of pip packages at particular versions, system libraries for certain dependencies, and environment variables for API keys. If a colleague tries to run it, they'll spend an hour debugging installation issues. If you deploy it to a server, the server's Python version might be different, or a system library might be missing. Docker solves this by packaging your application with everything it needs into a single, portable unit called a container.

A Docker **image** is a blueprint — it defines what goes into the container (operating system, Python, dependencies, your code). A **container** is a running instance of an image. A **Dockerfile** is the set of instructions that builds an image. **Docker Compose** defines multi-container applications (for example, your API plus a database plus a cache) so you can start the entire stack with one command.

The workflow is: write a Dockerfile, build it into an image, run the image as a container, and deploy that container to the cloud. The same container that runs on your laptop runs identically in production.

## Writing a Production Dockerfile

Here is a production-quality Dockerfile for the chatbot API:

```dockerfile
# Use official Python runtime as base
FROM python:3.10-slim

# Set working directory
WORKDIR /app

# Set environment variables
ENV PYTHONDONTWRITEBYTECODE=1 \
    PYTHONUNBUFFERED=1 \
    PIP_NO_CACHE_DIR=1

# Install system dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Copy requirements first (for layer caching)
COPY requirements.txt .

# Install Python dependencies
RUN pip install --no-cache-dir -r requirements.txt

# Copy application code
COPY . .

# Create non-root user for security
RUN useradd -m -u 1000 appuser && \
    chown -R appuser:appuser /app
USER appuser

# Expose port
EXPOSE 8000

# Health check
HEALTHCHECK --interval=30s --timeout=10s --start-period=5s --retries=3 \
    CMD curl -f http://localhost:8000/health || exit 1

# Run the application
CMD ["uvicorn", "main:app", "--host", "0.0.0.0", "--port", "8000"]
```

Several production practices are at work here. The `requirements.txt` is copied and installed before the rest of the application code. Docker caches each instruction as a layer, so if your code changes but your dependencies don't, Docker reuses the cached dependency layer — this speeds up rebuilds dramatically. The `PYTHONUNBUFFERED=1` setting ensures Python output appears in the container logs immediately rather than being buffered. Running as a non-root user (`appuser`) is a security best practice: if the application is compromised, the attacker has limited permissions. The `HEALTHCHECK` instruction tells Docker how to determine whether the container is healthy, which container orchestrators use for automatic restarts and traffic routing.

The corresponding `requirements.txt` lists your dependencies with pinned versions:

```
fastapi==0.104.1
uvicorn[standard]==0.24.0
pydantic==2.5.0
openai==1.3.0
chromadb==0.4.15
tiktoken==0.5.1
python-dotenv==1.0.0
```

A `.dockerignore` file prevents unnecessary files from being copied into the image:

```
__pycache__
*.pyc
*.pyo
*.pyd
.Python
env/
venv/
.git
.gitignore
.DS_Store
.env
*.log
chroma_db/
```

This is important both for image size and for security — you don't want your `.env` file with API keys ending up inside the Docker image.

To build and run:

```bash
# Build image
docker build -t chatbot-api:v1 .

# Run container
docker run -d \
  -p 8000:8000 \
  -e OPENAI_API_KEY=sk-... \
  --name chatbot \
  chatbot-api:v1

# Check logs
docker logs chatbot

# Stop container
docker stop chatbot
```

The `-p 8000:8000` flag maps port 8000 inside the container to port 8000 on the host. The `-e` flag passes environment variables — this is how you provide API keys without hardcoding them in the image. The resulting image is typically around 500–650 MB, which includes the Python runtime, all dependencies, and your application code.

## Multi-Stage Builds

A 650 MB image works but is larger than necessary. Much of the size comes from build tools (compilers, headers) that are needed to install dependencies but aren't needed to run the application. Multi-stage builds solve this by separating the build environment from the runtime environment:

```dockerfile
# Stage 1: Builder
FROM python:3.10 as builder

WORKDIR /app
COPY requirements.txt .
RUN pip install --user --no-cache-dir -r requirements.txt

# Stage 2: Runtime
FROM python:3.10-slim

WORKDIR /app

# Copy only installed packages from builder
COPY --from=builder /root/.local /root/.local

# Copy application code
COPY . .

ENV PATH=/root/.local/bin:$PATH

CMD ["uvicorn", "main:app", "--host", "0.0.0.0", "--port", "8000"]
```

The first stage uses the full Python image (which includes build tools) to compile and install all dependencies. The second stage starts from the slim image (which has only the Python runtime) and copies over the installed packages. The build tools are left behind in the first stage, never appearing in the final image.

The size difference is substantial:

```
Single-stage build:  ~650 MB
Multi-stage build:   ~180 MB
```

Smaller images mean faster pulls from the container registry, faster scaling (when new containers need to start), and lower storage costs. At scale — hundreds of containers across multiple environments — this adds up.

For even smaller images, you can use Alpine Linux as the base (`python:3.10-alpine`), though this can introduce compatibility issues with some Python packages that expect a glibc-based system. For most chatbot applications, the slim variant strikes the right balance between size and compatibility.

## Docker Compose for Local Development

A production chatbot system typically involves more than just the API server. You'll have a database for storing conversations and user profiles, a cache for session management, and potentially a frontend application. Docker Compose lets you define all of these services in a single YAML file and start them together:

```yaml
version: '3.8'

services:
  # FastAPI backend
  api:
    build: ./backend
    ports:
      - "8000:8000"
    environment:
      - OPENAI_API_KEY=${OPENAI_API_KEY}
      - DATABASE_URL=postgresql://user:pass@db:5432/chatbot
      - REDIS_URL=redis://redis:6379
    depends_on:
      - db
      - redis
    volumes:
      - ./backend:/app
    command: uvicorn main:app --host 0.0.0.0 --reload

  # PostgreSQL database
  db:
    image: postgres:15-alpine
    environment:
      - POSTGRES_USER=user
      - POSTGRES_PASSWORD=pass
      - POSTGRES_DB=chatbot
    volumes:
      - postgres_data:/var/lib/postgresql/data
    ports:
      - "5432:5432"

  # Redis for caching and sessions
  redis:
    image: redis:7-alpine
    ports:
      - "6379:6379"
    volumes:
      - redis_data:/data

  # React frontend
  frontend:
    build: ./frontend
    ports:
      - "3000:3000"
    environment:
      - REACT_APP_API_URL=http://localhost:8000
    volumes:
      - ./frontend:/app
      - /app/node_modules
    command: npm start

volumes:
  postgres_data:
  redis_data:
```

This configuration defines four services. The `api` service builds from the `./backend` directory and connects to both the database and Redis. The `db` service runs PostgreSQL with a persistent volume so data survives container restarts. The `redis` service provides a cache layer. The `frontend` service runs the React development server.

Services can reference each other by name — the API connects to the database at `postgresql://user:pass@db:5432/chatbot` where `db` is the service name, not a hostname. Docker Compose handles the networking automatically. The `depends_on` directive ensures the database and cache start before the API.

The `volumes` entries serve two purposes. Named volumes (`postgres_data`, `redis_data`) persist data across container restarts. Bind mounts (`./backend:/app`) map your local source code into the container, so code changes are reflected immediately without rebuilding the image. The `--reload` flag on the uvicorn command enables auto-restart on file changes, giving you a fast development loop.

To manage the stack:

```bash
# Start all services
docker-compose up

# Start in background
docker-compose up -d

# View logs for a specific service
docker-compose logs -f api

# Stop all services
docker-compose down

# Rebuild after dependency changes
docker-compose up --build
```

With Docker Compose, every developer on the team gets an identical environment with a single command. There's no need to install PostgreSQL locally, configure Redis, or worry about version mismatches. The development environment matches production as closely as possible, reducing the "works on my machine" class of bugs.
