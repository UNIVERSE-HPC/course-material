---
name: Cloud Deployment
dependsOn:
  - technology_and_tooling.chatbot_development.m5-01-backend-design-with-fastapi
tags: [chatbots, deployment, cloud, google-cloud-run, optional]
learningOutcomes:
  - Compare deployment platforms and choose the right one for your use case.
  - Deploy a containerised chatbot API to Google Cloud Run.
  - Configure cloud resources including memory, CPU, and scaling limits.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    image: https://github.com/OxfordCompetencyCenters.png
    license: CC-BY-4.0
---

Your chatbot is containerised and runs locally. The next step is making it accessible to users on the internet. Cloud deployment options range from simple push-to-deploy platforms to fully managed container services to complex but powerful Kubernetes clusters. The right choice depends on your traffic volume, budget, operational expertise, and scaling requirements.

## Choosing a Deployment Platform

There are four broad categories of deployment platforms, each with distinct trade-offs.

**Serverless functions** (AWS Lambda, Google Cloud Functions) execute your code in response to individual requests. They scale automatically from zero to thousands of concurrent invocations and charge only for compute time used. However, they suffer from cold starts — the first request after a period of inactivity can be slow as the runtime initialises. They also impose execution time limits (typically 15 minutes maximum) and are awkward for stateful applications. For chatbots, cold starts are particularly problematic because they add noticeable latency to the first message in a conversation, and LLM calls can approach the time limit for complex queries.

**Container services** (AWS ECS, Google Cloud Run, Azure Container Instances) run your Docker containers in a managed environment. They handle provisioning, scaling, and networking while giving you control over the container itself. They support long-running processes, handle state more naturally than serverless functions, and scale based on traffic. For most production chatbots, container services are the sweet spot — they provide enough control for custom configurations without the operational burden of managing servers.

**Kubernetes** (AWS EKS, Google GKE, Azure AKS) provides maximum control and flexibility. It handles container orchestration, service discovery, load balancing, rolling updates, and self-healing. It's portable across cloud providers and supports complex multi-service architectures. The downside is complexity: Kubernetes requires significant DevOps expertise to configure and maintain, and it's overkill for small-to-medium projects. Reserve Kubernetes for large-scale applications with multiple services that need fine-grained control over networking, scaling policies, and deployment strategies.

**Platform-as-a-Service** (Render, Railway, Heroku) offers the simplest deployment experience. You connect your Git repository, and the platform handles building, deploying, and scaling. Many offer free tiers for development and hobby projects. The trade-off is less control and higher costs at scale. These platforms are excellent for MVPs, prototypes, and small applications where time-to-deploy matters more than cost optimisation.

The practical recommendation is to start with a PaaS (Render or Railway) or Cloud Run for initial deployment. Graduate to ECS or Kubernetes as your traffic and complexity grow.

## Deploying to Google Cloud Run

Google Cloud Run is a fully managed container service that runs Docker containers without requiring you to manage servers. It scales automatically based on traffic — including scaling to zero when idle, so you pay nothing during quiet periods. It provides HTTPS endpoints automatically and integrates with Google Cloud's ecosystem for databases, secret management, and monitoring.

The deployment process has four steps: install the Google Cloud SDK, build and push your Docker image, deploy to Cloud Run, and verify the deployment.

**Step 1: Install and configure the Google Cloud SDK.**

```bash
curl https://sdk.cloud.google.com | bash
gcloud auth login
gcloud config set project your-project-id
```

**Step 2: Build and push your Docker image to Google Container Registry.**

```bash
# Enable required APIs
gcloud services enable run.googleapis.com
gcloud services enable containerregistry.googleapis.com

# Build and push using Google Cloud Build
gcloud builds submit --tag gcr.io/your-project-id/chatbot-api
```

This command sends your source code to Google Cloud Build, which builds the Docker image remotely and pushes it to the container registry. Alternatively, you can build locally and push:

```bash
docker build -t gcr.io/your-project-id/chatbot-api .
docker push gcr.io/your-project-id/chatbot-api
```

**Step 3: Deploy the image to Cloud Run.**

```bash
gcloud run deploy chatbot-api \
  --image gcr.io/your-project-id/chatbot-api \
  --platform managed \
  --region us-central1 \
  --allow-unauthenticated \
  --set-env-vars OPENAI_API_KEY=sk-... \
  --memory 2Gi \
  --cpu 2 \
  --max-instances 10 \
  --timeout 60s
```

The flags configure the deployment: `--memory 2Gi` and `--cpu 2` allocate resources for each container instance (LLM-backed chatbots need more memory than typical web services). `--max-instances 10` caps the number of concurrent containers to control costs. `--timeout 60s` sets the maximum request duration. `--allow-unauthenticated` makes the endpoint publicly accessible (remove this flag if you're handling authentication in your API).

**Step 4: Retrieve the deployment URL.**

```bash
gcloud run services describe chatbot-api \
  --platform managed \
  --region us-central1 \
  --format 'value(status.url)'
```

This outputs a URL like `https://chatbot-api-abc123-uc.a.run.app`. Your chatbot API is now live on the internet.

To update after code changes, rebuild the image and redeploy:

```bash
gcloud builds submit --tag gcr.io/your-project-id/chatbot-api
gcloud run deploy chatbot-api \
  --image gcr.io/your-project-id/chatbot-api \
  --platform managed \
  --region us-central1
```

For production use, you would add several refinements: a custom domain instead of the auto-generated URL, Google Secret Manager for API keys instead of passing them as environment variables, a Cloud SQL database for persistent storage, Cloud Monitoring for alerts and dashboards, and a CI/CD pipeline (using Cloud Build or GitHub Actions) to automate the build-and-deploy process on every push to main.

Cost-wise, Cloud Run is economical for moderate traffic. Handling 100,000 requests per month with the configuration above might cost $5–10 in compute (not counting the LLM API costs, which are typically the dominant expense for chatbot applications). The scale-to-zero feature means you pay nothing during periods of inactivity, which makes it particularly cost-effective for internal tools and low-traffic applications.
