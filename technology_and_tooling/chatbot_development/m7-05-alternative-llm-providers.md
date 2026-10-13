---
name: Alternative LLM Providers
dependsOn:
  - technology_and_tooling.chatbot_development.m7-01-production-prompt-engineering
tags: [chatbots, llm-providers, open-source, production-checklist, optional]
learningOutcomes:
  - Compare LLM providers across cost, quality, speed, and context length.
  - Implement a multi-provider architecture with automatic fallback.
  - Apply a production readiness checklist to prepare a chatbot for deployment.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    image: https://github.com/OxfordCompetencyCenters.png
    license: CC-BY-4.0
---

Throughout this course, the examples have used OpenAI's models. But OpenAI is not the only option, and it's not always the best one. Depending on your requirements — cost, quality, context length, data privacy, regulatory constraints — other providers may be a better fit. A production system should also be resilient to provider outages, which means supporting multiple providers with automatic fallback.

## Provider Comparison

The major LLM providers each have distinct strengths:

**OpenAI** offers GPT-4 (highest overall quality, expensive) and GPT-3.5-turbo (fast, cheap, good for most tasks). OpenAI has the most mature API ecosystem and the broadest developer adoption. It's the safe default choice for most applications.

**Anthropic** offers the Claude model family. Claude's strengths are very long context windows (up to 200K tokens), strong reasoning capabilities, and a generally lower hallucination rate. Claude is well-suited for tasks that require processing large amounts of text or careful analytical reasoning.

**Google** offers Gemini models with good quality and native multimodal capabilities (text, images, video). Gemini integrates well with the Google Cloud ecosystem.

**Open-source models** (Llama, Mistral, and others) offer full control over the model and infrastructure. There are no per-token costs — you pay only for the compute infrastructure. Data never leaves your servers, which satisfies strict privacy and regulatory requirements. The trade-off is lower quality than the best commercial models (open-source models at the 70B parameter scale are roughly comparable to GPT-3.5), infrastructure complexity, and the need for GPU hardware.

The right choice depends on your use case. For most applications starting out, OpenAI or Anthropic commercial APIs are the pragmatic choice. At very high volume (millions of tokens per day) or with strict data residency requirements, self-hosted open-source models become cost-effective.

## Multi-Provider Architecture

A multi-provider strategy provides both flexibility and resilience. If your primary provider experiences an outage — which happens to every cloud service — your chatbot continues operating by falling back to an alternative:

```python
class MultiProviderLLM:
    def __init__(self):
        self.providers = {
            "openai": OpenAIProvider(),
            "anthropic": AnthropicProvider(),
            "local": LocalLlamaProvider()
        }

    def generate(self, prompt, provider="openai", fallback=True):
        """Generate with the specified provider, falling back on failure."""
        try:
            return self.providers[provider].generate(prompt)
        except Exception as e:
            logger.error(f"{provider} failed: {e}")

            if fallback:
                fallback_provider = "anthropic" if provider == "openai" else "openai"
                logger.info(f"Falling back to {fallback_provider}")
                return self.providers[fallback_provider].generate(prompt)

            raise

llm = MultiProviderLLM()
response = llm.generate(prompt, provider="openai", fallback=True)
```

You can also combine the multi-provider architecture with the smart model router from the cost optimisation section: route simple queries to a cheap, fast model (or even a self-hosted open-source model), and route complex queries to a high-quality commercial model.

## Self-Hosted LLMs

For organisations where data privacy is paramount or where per-token costs at scale are prohibitive, self-hosting an open-source LLM is an option. vLLM is the standard serving framework — it optimises GPU memory usage and throughput:

```bash
pip install vllm

python -m vllm.entrypoints.api_server \
    --model meta-llama/Llama-2-7b-chat-hf \
    --host 0.0.0.0 \
    --port 8000
```

```python
import requests

response = requests.post("http://localhost:8000/generate", json={
    "prompt": "What is Python?",
    "max_tokens": 100
})
print(response.json()["text"])
```

Self-hosting requires GPU infrastructure. An AWS p3.8xlarge instance with 4 V100 GPUs costs roughly $12/hour ($8,640/month). The break-even point compared to API pricing depends on your volume: at approximately 50 million tokens per month, self-hosting a model in the GPT-3.5 quality tier becomes cheaper than using the API. Below that volume, the API is more cost-effective and far simpler to operate.

For production self-hosted deployments, use TensorRT-LLM or vLLM for inference optimisation, deploy on GPU instances (AWS p3/p4, GCP A100), use Kubernetes for orchestration and scaling, and monitor GPU utilisation to ensure you're getting value from the hardware.

## Production Readiness Checklist

Before deploying a chatbot to production, review this checklist. Each item prevents a class of failures:

**Security:**
- All secrets stored in environment variables or a secret manager
- HTTPS enabled with valid certificates
- Authentication implemented (JWT or similar)
- Content moderation active on both inputs and outputs
- PII detection and handling implemented
- Rate limiting configured
- Input validation on all endpoints

**Reliability:**
- Error handling and graceful degradation on all code paths
- Health check endpoint functional
- Retry logic with exponential backoff for external API calls
- Timeouts configured for all network requests
- Circuit breakers for external services
- Multi-provider fallback (if applicable)

**Observability:**
- Structured logging with trace IDs
- Metrics tracking for latency, cost, and error rate
- Distributed tracing (if running microservices)
- Alerting configured for critical metrics
- Dashboard for real-time monitoring

**Performance:**
- Semantic caching implemented
- Prompt optimisation completed
- Response length limits set
- Model routing strategy defined
- Load testing completed

**Scalability:**
- Stateless design with externalised state
- Horizontal scaling tested
- Database connection pooling configured
- Async processing for slow operations

**Compliance:**
- Data retention policy defined and implemented
- GDPR compliance (if serving EU users)
- Terms of service and privacy policy published
- Audit logging for sensitive operations

::::challenge{id=m7-05-ex1 title="Production Hardening"}

Apply production best practices to your capstone chatbot:

1. **Content moderation (15 minutes):** Integrate the OpenAI Moderation API. Check both user inputs and bot outputs. Log all violations.

2. **PII handling (15 minutes):** Add PII detection using regex patterns or Presidio. Mask PII before logging. Decide whether to reject, redact, or warn when PII is detected.

3. **Cost optimisation (15 minutes):** Compress your system prompt. Add selective context inclusion for RAG. Set `max_tokens` on API calls.

4. **Rate limiting (10 minutes):** Add per-IP and per-user rate limits using `slowapi`.

5. **Adversarial testing (20 minutes):** Test your chatbot with off-topic queries, prompt injection attempts, PII in messages, high request volume, and malformed inputs. Fix any failures.

6. **Checklist review:** Walk through the production readiness checklist and check off each item.
::::

::::challenge{id=m7-05-checkpoint title="Project Checkpoint 7: Production-Ready Chatbot"}

Your capstone project should now be hardened for production use.

Deliverables:

- Content moderation on inputs and outputs
- PII detection and handling
- At least three cost optimisation techniques implemented
- Rate limiting configured
- Adversarial test results documented
- Production readiness checklist completed
- Architecture documentation describing your production setup
::::

## Summary and What's Next

In this module, you've applied the finishing touches that separate a prototype from a production system. You've learned to craft robust, versioned prompts that handle edge cases gracefully. You've implemented content moderation and PII protection to keep your chatbot safe and compliant. You've optimised costs through prompt compression, selective context, response limits, and smart model routing. You've designed for horizontal scale with stateless architecture, externalised state, and load balancing. You've explored alternative LLM providers and built resilience through multi-provider fallback. And you've reviewed a comprehensive production checklist to ensure nothing is missed.

In Module 8, you'll bring everything together for the capstone project. You'll integrate all the components built across Modules 1–7 into a complete, production-grade chatbot system, prepare a final presentation, and document your architecture and design decisions.

## Additional Resources

- [Prompt Engineering Guide](https://www.promptingguide.ai/) — Comprehensive prompt engineering techniques
- ["Building LLM Applications for Production"](https://huyenchip.com/2023/04/11/llm-engineering.html) by Chip Huyen — Practical LLM production engineering
- [OpenAI Moderation API](https://platform.openai.com/docs/guides/moderation) — Content moderation documentation
- [Presidio](https://microsoft.github.io/presidio/) — Microsoft's PII detection and anonymisation library
- [slowapi](https://github.com/laurentS/slowapi) — Rate limiting for FastAPI
- [vLLM](https://github.com/vllm-project/vllm) — High-throughput LLM serving engine
- [Redis Documentation](https://redis.io/docs/) — Distributed state management
