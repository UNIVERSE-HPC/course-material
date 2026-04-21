---
name: User Personalisation
dependsOn:
  - technology_and_tooling.chatbot_development.m4-04-retrieval-based-memory
tags: [chatbots, memory, personalisation, user-profiles]
learningOutcomes:
  - Extract user facts and preferences from conversation history using LLM-based analysis.
  - Build a persistent user profile system that accumulates knowledge across sessions.
  - Integrate user profiles into chatbot responses for tailored interactions.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    license: CC-BY-4.0
---

The memory strategies covered so far — windowing, summarisation, selective memory, and retrieval — all focus on preserving *what was said*. Personalisation goes a step further: it extracts and stores *who the user is*. Over the course of a conversation, users reveal a great deal about themselves. They mention their skill level, their preferences, their constraints, their goals. A beginner says "I'm new to programming." An expert says "I've been using Kubernetes for years." A user on Windows mentions compatibility concerns. A data scientist asks about pandas.

A generic chatbot ignores all of this and gives the same response to every user. A personalised chatbot accumulates these facts into a user profile and uses them to tailor every subsequent response. The difference in user experience is substantial.

## Extracting User Information

The extraction process uses an LLM to analyse conversation history and pull out key facts about the user. The categories of information worth tracking include preferences ("I prefer Python over JavaScript"), context ("I'm a software engineer at a startup"), constraints ("I need solutions that work on Windows"), goals ("I'm learning machine learning"), and technical level (beginner, intermediate, expert).

```python
def extract_user_info(conversation_history):
    """Extract key facts about the user from conversation history."""

    conversation_text = "\n".join([
        f"{msg['role']}: {msg['content']}"
        for msg in conversation_history
    ])

    prompt = f"""From this conversation, extract key information about the user.
Include: preferences, context about their situation, technical level, constraints, goals.

Format as bullet points.

Conversation:
{conversation_text}

User profile:"""

    response = openai.ChatCompletion.create(
        model="gpt-4",
        messages=[{"role": "user", "content": prompt}],
        temperature=0.0
    )

    return response.choices[0].message.content
```

This function takes a list of conversation messages and returns a structured summary of what the bot has learned about the user. Given a conversation where the user says "I'm a beginner learning Python for data science" and "I prefer using Jupyter notebooks," the extraction might produce:

```
User Profile:
- Technical level: Beginner
- Learning: Python
- Goal: Data science
- Preference: Jupyter notebooks
- Context: New to programming
```

The extraction doesn't need to happen on every turn — that would be expensive. A practical approach is to run it periodically (every 10 turns) or at the end of each session. The extracted facts accumulate over time, building an increasingly detailed picture of the user.

## Storing and Managing User Profiles

The `UserProfileManager` class handles the persistence layer. In production, you'd store profiles in a database (PostgreSQL, MongoDB, or similar). For development and prototyping, a local JSON file per user works well:

```python
import json
from datetime import datetime

class UserProfileManager:
    def __init__(self, user_id):
        self.user_id = user_id
        self.profile = self.load_profile()

    def load_profile(self):
        """Load user profile from storage."""
        try:
            with open(f"profiles/user_{self.user_id}.json", "r") as f:
                return json.load(f)
        except FileNotFoundError:
            return {"user_id": self.user_id, "facts": []}

    def save_profile(self):
        """Save profile to storage."""
        with open(f"profiles/user_{self.user_id}.json", "w") as f:
            json.dump(self.profile, f)

    def update_profile(self, new_facts):
        """Add new facts to profile."""
        self.profile["facts"].extend(new_facts)
        self.profile["last_updated"] = datetime.now().isoformat()
        self.save_profile()

    def get_profile_summary(self):
        """Get summary for LLM context."""
        if not self.profile["facts"]:
            return ""

        return "User profile:\n" + "\n".join([
            f"- {fact}" for fact in self.profile["facts"]
        ])
```

The profile is a simple structure: a user ID, a list of facts, and a timestamp for the last update. The `get_profile_summary` method formats the profile as a string that can be injected directly into the system prompt.

In a production system, you would also want deduplication (don't store "prefers Python" five times), conflict resolution (if the user says "I switched from Flask to Django," update the profile rather than keeping both), and expiry (some facts become stale over time). These refinements add complexity but significantly improve profile quality. A simple approach to deduplication is to periodically ask the LLM to consolidate the profile — merge duplicates, resolve contradictions, and remove outdated information.

## Personalised Responses

The `PersonalizedChatbot` class ties everything together. It combines the hybrid memory architecture from the previous section with the user profile system to produce responses that are tailored to the specific user:

```python
class PersonalizedChatbot:
    def __init__(self, user_id):
        self.user_id = user_id
        self.profile_manager = UserProfileManager(user_id)
        self.memory = HybridMemoryChatbot(user_id)

    def chat(self, user_message):
        """Generate personalized response."""

        profile_summary = self.profile_manager.get_profile_summary()

        system_prompt = f"""You are a helpful programming assistant.

{profile_summary}

Tailor your responses based on the user's profile:
- Adjust technical level to their experience
- Consider their preferences and constraints
- Reference their goals when relevant
- Build on previous conversations
"""

        response = self.memory.chat(user_message, system_prompt=system_prompt)

        # Periodically update profile
        if self.memory.turn_counter % 10 == 0:
            self.update_user_profile()

        return response

    def update_user_profile(self):
        """Extract new facts from recent conversation."""
        recent_turns = self.memory.current_window[-20:]

        if len(recent_turns) < 4:
            return

        new_facts = extract_user_info(recent_turns)

        if new_facts:
            self.profile_manager.update_profile([new_facts])
```

The impact of personalisation is best illustrated by comparing responses to the same question. Without personalisation, the question "Which database should I use?" produces a generic overview of popular options. With personalisation — knowing that the user is a beginner, uses Python and Flask, is building a blog, and works on Windows — the response becomes specific and actionable: "Since you're building a blog with Python and you're still learning, I'd recommend starting with SQLite. It's simple, requires no setup, and works great with Flask. Once your blog grows, you can migrate to PostgreSQL."

The personalised response is more useful because it considers the user's actual situation rather than providing a one-size-fits-all answer. Over multiple sessions, the profile accumulates enough context to make every response feel like it comes from an assistant who genuinely knows the user.

A note on privacy: personalisation requires storing information about users. Be transparent about what you're storing, give users the ability to view and delete their profiles, and comply with data protection regulations (GDPR, CCPA, etc.). The "right to be forgotten" is not just a legal requirement — it's good practice. If a user asks the bot to forget something, you should be able to remove it from their profile and conversation history.

::::challenge{id=m4-05-ex1 title="Token Window Management"}

Implement a `TokenWindowChatbot` that manages conversation history by token count. Test it with a 30-turn conversation and track how token usage changes over time.

```python
class TokenWindowChatbot:
    def __init__(self, max_tokens=2000):
        # TODO: Initialize with system prompt, history list, and tiktoken encoding
        pass

    def chat(self, user_message):
        # TODO: Add the user message to history
        # TODO: Prune oldest message pairs while over the token limit
        # TODO: Call the API and append the assistant response
        pass

    def get_token_count(self):
        # TODO: Return current token usage
        pass

# Test with a long conversation
bot = TokenWindowChatbot(max_tokens=1000)
for i in range(30):
    bot.chat(f"Question {i+1}: Tell me about topic {i+1}.")
    print(f"Turn {i+1}: {bot.get_token_count()} tokens")
```

Observe how the token count climbs and then stabilises once the window limit kicks in. Try different `max_tokens` values (500, 1000, 2000) and note the trade-off between context retention and token budget.
::::

::::challenge{id=m4-05-ex2 title="Conversation Summarisation"}

Build a `SummarizingChatbot` and test whether summarisation preserves specific facts across a long conversation.

```python
class SummarizingChatbot:
    def summarize_messages(self, messages):
        # TODO: Generate a concise summary using the LLM
        pass

    def chat(self, user_message):
        # TODO: Add message, summarise if threshold reached, generate response
        pass

# Recall test
bot = SummarizingChatbot()
bot.chat("My account ID is ABC123")  # Turn 1
# ... 23 more turns about various topics ...
response = bot.chat("What was my account ID?")  # Turn 25
assert "ABC123" in response  # Should be preserved in the summary
```

If the account ID is lost in the summary, think about why. Summarisation is lossy by design — the LLM decides what's important enough to keep. This is exactly the problem that selective memory (covered earlier in this module) addresses. Consider how you might combine the two approaches.
::::

::::challenge{id=m4-05-ex3 title="Retrieval-Based Memory"}

Implement a `ConversationalMemory` class and test cross-session recall.

```python
memory = ConversationalMemory(user_id="test_user")

# Day 1
memory.chat("I'm having trouble with Docker deployment")
memory.chat("The error is 'permission denied'")
# ... 10 more turns about Docker ...

# Day 2 (new session, same user)
memory2 = ConversationalMemory(user_id="test_user")
response = memory2.chat("What was that Docker issue we discussed?")

# Verify retrieval worked
assert "Docker" in response
assert "permission denied" in response
```

The key test is whether the second session can access information from the first. If it works, the bot has genuine long-term memory. Try varying the number of retrieved turns (`top_k`) and see how it affects both response quality and latency.
::::

::::challenge{id=m4-05-ex4 title="User Profile Extraction"}

Build a user profile system and verify that it extracts facts correctly.

```python
profile_mgr = UserProfileManager(user_id="test_user")

conversation = [
    {"role": "user", "content": "I'm a data scientist using Python"},
    {"role": "assistant", "content": "Great, Python is excellent for data science."},
    {"role": "user", "content": "I prefer pandas over numpy for data manipulation"},
    {"role": "assistant", "content": "Pandas is very intuitive for tabular data."},
    {"role": "user", "content": "I work on a MacBook Pro"},
    {"role": "assistant", "content": "macOS is a popular choice for data science."},
]

# Extract profile from conversation
profile_text = extract_user_info(conversation)
profile_mgr.update_profile([profile_text])

profile = profile_mgr.get_profile_summary()

# Verify extraction
assert "data scientist" in profile.lower()
assert "python" in profile.lower()
assert "pandas" in profile.lower()
```

After verifying that extraction works, integrate the profile into a chatbot's system prompt and observe how responses change. Ask the same technical question with and without the profile to see the difference.
::::

::::challenge{id=m4-05-checkpoint title="Project Checkpoint 4: Add Memory to Your Chatbot"}

Enhance your capstone project with at least two memory strategies from this module. Your chatbot should handle extended conversations gracefully, retaining important context without exceeding token limits.

Your deliverables should include:

- Implementation of at least two memory strategies (conversation summarisation, token management, retrieval-based memory, or user profiles)
- Successful handling of 50+ turn conversations without exceeding the context window
- Demonstration of memory recall across turns (referencing information from turn 10 at turn 40)
- Evidence of token usage staying bounded over time
- Documentation of which strategies you chose and why

Test your implementation against these cases:

1. **Long Conversation:** Run 50 turns without errors or context window overflow.
2. **Recall Test:** Tell the bot something specific at turn 10, then ask about it at turn 40.
3. **Cross-Session Memory:** End a session, start a new one, and verify the bot remembers key facts.
4. **Personalisation:** Confirm that the bot adapts its responses based on accumulated user profile information.

The choice of which strategies to combine depends on your application domain. Customer support bots benefit most from summarisation (to track the troubleshooting thread) and retrieval (to recall previous tickets). Tutoring bots prioritise user profiles (to track learning progress) and selective memory (to retain key concepts the student has mastered). Personal assistants need all four layers to provide a truly continuous experience.
::::

## Summary and What's Next

In this module, you've built the memory systems that transform a stateless chatbot into a contextual, continuous assistant. You started with the fundamental challenge — unbounded conversation history exceeds context windows and budgets — and progressively developed more sophisticated solutions. Fixed and token-based windows provide a simple baseline. Rolling and hierarchical summarisation compress older history while preserving key points. Selective memory uses importance scoring to ensure critical facts survive pruning. Retrieval-based memory extends recall to any point in the user's history, across sessions. User profiles accumulate knowledge about the user and enable personalised responses. The hybrid architecture combines all four layers into a production-ready system that handles conversations of any length within a bounded token budget.

In Module 5, you'll shift from building chatbot intelligence to deploying it. You'll design a FastAPI backend to serve your chatbot as an API, build frontend interfaces with Streamlit and React, containerise everything with Docker, deploy to the cloud, and implement authentication and security. The chatbot you've built across Modules 1–4 will become a production service that real users can access.

## Additional Resources

- ["Memory-Augmented Neural Networks"](https://arxiv.org/abs/1410.5401) (Graves et al., 2014) — Foundational work on external memory for neural networks
- ["Memory Networks"](https://arxiv.org/abs/1410.3916) (Weston et al., 2015) — End-to-end memory architectures
- ["Attention Is All You Need"](https://arxiv.org/abs/1706.03762) (Vaswani et al., 2017) — The transformer architecture that makes context windows possible
- [tiktoken](https://github.com/openai/tiktoken) — OpenAI's token counting library
- [ChromaDB](https://www.trychroma.com/) — Vector database for conversation memory storage

## Hands-on practical

Work through the accompanying notebook: [`module_4/05-user-personalisation.ipynb`](https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules/blob/main/module_4/05-user-personalisation.ipynb). It walks through LLM-based fact extraction, a JSON-backed `UserProfile` class, and a comparison of generic vs personalised answers to the same question.
