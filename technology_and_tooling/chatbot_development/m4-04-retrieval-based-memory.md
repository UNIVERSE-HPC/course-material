---
name: Retrieval-Based Memory
dependsOn:
  - technology_and_tooling.chatbot_development.m4-01-conversation-history-management
tags: [chatbots, memory, retrieval, vector-search]
learningOutcomes:
  - Store conversation turns as vector embeddings for long-term retrieval.
  - Implement cross-session memory that persists between conversations.
  - Design a hybrid memory architecture that combines windowing, summarisation, retrieval, and user profiles.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    image: https://github.com/OxfordCompetencyCenters.png
    license: CC-BY-4.0
---

Window management, summarisation, and selective pruning all operate within a single session. Once the user closes the chat and opens a new one, the context is gone. The bot has no way to recall what was discussed yesterday, last week, or in any previous session. For many applications — customer support, tutoring, personal assistants — this is a serious limitation. Users expect continuity. They don't want to re-explain their setup, re-state their preferences, or re-describe the problem they've been troubleshooting across multiple sessions.

Retrieval-based memory solves this by treating conversation history the same way RAG treats documents. Every conversation turn is embedded and stored in a vector database. When the user sends a new message, the system searches those stored embeddings for semantically relevant past turns and injects them into the context. The bot can "remember" something from any point in its history with a user, even if that information was exchanged weeks ago.

## Storing and Retrieving Past Conversations

The concept is straightforward if you've already built a RAG system. Instead of embedding document chunks, you embed conversation turns. Instead of retrieving documents relevant to a user's question, you retrieve past conversation turns relevant to the current message. The mechanics are identical — the only difference is what you're storing.

Consider a user who discussed a deployment issue with your bot three days ago. Today they return and ask, "What was that deployment issue we discussed?" Without retrieval-based memory, the bot has no idea — it has no access to previous sessions. With retrieval-based memory, the system embeds the user's question, searches all past conversation embeddings, finds the turn from three days ago where the user said "Deployment failing due to missing env var," and includes that context in the prompt. The bot can then respond: "We discussed a deployment issue where an environment variable was missing."

This approach has several advantages over the strategies covered so far. It is not limited by the context window, since stored turns live in a database that can hold millions of entries. It works across sessions, because the vector database persists between conversations. It retrieves selectively, pulling in only the turns that are relevant to the current question rather than dumping the entire history into the prompt. And it enables personalisation, since the system can retrieve past preferences, decisions, and context about the user.

Here is a complete implementation using ChromaDB:

```python
import chromadb
import openai
from datetime import datetime

class ConversationalMemory:
    def __init__(self, user_id):
        self.user_id = user_id
        self.client = chromadb.PersistentClient(path="./conversation_memory")
        self.collection = self.client.get_or_create_collection(
            f"user_{user_id}_conversations"
        )
        self.current_session = []
        self.turn_counter = 0

    def store_turn(self, role, content):
        """Store a conversation turn in vector memory."""
        self.turn_counter += 1

        response = openai.Embedding.create(
            model="text-embedding-3-small",
            input=content
        )
        embedding = response['data'][0]['embedding']

        self.collection.add(
            ids=[f"turn_{self.turn_counter}"],
            embeddings=[embedding],
            documents=[content],
            metadatas=[{
                "role": role,
                "timestamp": datetime.now().isoformat(),
                "turn": self.turn_counter,
                "session_id": "current"
            }]
        )

        self.current_session.append({
            "role": role,
            "content": content,
            "turn": self.turn_counter
        })

    def retrieve_relevant_history(self, query, top_k=3):
        """Retrieve relevant past conversation turns."""
        response = openai.Embedding.create(
            model="text-embedding-3-small",
            input=query
        )
        query_embedding = response['data'][0]['embedding']

        # Search past turns, excluding recent ones already in the current window
        results = self.collection.query(
            query_embeddings=[query_embedding],
            n_results=top_k,
            where={"turn": {"$lt": self.turn_counter - 10}}
        )

        relevant_turns = []
        if results['documents'][0]:
            for doc, meta in zip(results['documents'][0], results['metadatas'][0]):
                relevant_turns.append({
                    "content": doc,
                    "role": meta['role'],
                    "turn": meta['turn'],
                    "timestamp": meta['timestamp']
                })

        return relevant_turns

    def chat(self, user_message):
        """Chat with retrieval-based memory."""
        self.store_turn("user", user_message)

        relevant_past = self.retrieve_relevant_history(user_message, top_k=3)

        messages = [{"role": "system", "content": "You are a helpful assistant with memory of past conversations."}]

        if relevant_past:
            past_context = "Relevant past conversation:\n"
            for turn in relevant_past:
                past_context += f"[Turn {turn['turn']}] {turn['role']}: {turn['content']}\n"

            messages.append({
                "role": "system",
                "content": past_context
            })

        # Add recent conversation (last 5 exchanges)
        recent = self.current_session[-10:]
        for turn in recent:
            messages.append({
                "role": turn["role"],
                "content": turn["content"]
            })

        response = openai.ChatCompletion.create(
            model="gpt-3.5-turbo",
            messages=messages
        )

        assistant_message = response.choices[0].message.content
        self.store_turn("assistant", assistant_message)

        return assistant_message
```

The `store_turn` method embeds every message and saves it to ChromaDB with metadata including the role (user or assistant), timestamp, turn number, and session ID. The `retrieve_relevant_history` method embeds the current query and searches for semantically similar past turns, deliberately excluding the most recent turns (which are already in the current session window). The `chat` method ties everything together: it stores the user's message, retrieves relevant past context, builds the prompt with both retrieved history and recent turns, generates a response, and stores the assistant's reply.

The cross-session capability is built into the design. Because ChromaDB uses persistent storage, the conversation history survives between sessions. When a user returns days later, a new `ConversationalMemory` instance for the same `user_id` connects to the same ChromaDB collection, giving it access to every past turn:

```python
# Day 1 conversation
memory = ConversationalMemory(user_id="user_123")
memory.chat("I'm working on a Python web app using Flask")
memory.chat("I'm having trouble with database connections")
# ... discussion about database issues ...

# Day 3 conversation (new session, same user)
memory2 = ConversationalMemory(user_id="user_123")
response = memory2.chat("I'm back! Can you remind me what we discussed about databases?")
# Bot retrieves turns from Day 1 and references them
```

The key insight is that retrieval-based memory doesn't include all past turns — that would be just as expensive as sending the full history. It includes only the turns that are semantically relevant to the current query. If the user asks about databases, turns about databases are retrieved. If the user asks about deployment, turns about deployment are retrieved. The token cost stays predictable: typically 3 retrieved turns at roughly 100 tokens each, adding about 300 tokens to the context regardless of how many thousands of turns are stored in the database.

## Hybrid Memory Architecture

Each of the memory strategies covered in this module has strengths and weaknesses. Window management is fast and simple but forgets everything outside the window. Summarisation preserves the general flow of conversation but loses specific details. Selective memory retains important facts but still operates within a single session. Retrieval-based memory accesses any past turn but adds the cost of an embedding API call on every turn.

In production, the best results come from combining all four strategies into a layered architecture. Each layer serves a distinct purpose and has its own token budget:

```
Layer 1: Current Window (0-2 minutes)
- Last 5 turns in full detail
- Always included
- ~500 tokens

Layer 2: Recent Summary (2-15 minutes)
- Rolling summary of turns 6-30
- Updated every 5 turns
- ~200 tokens

Layer 3: Retrieved Memory (any time)
- Top 3 relevant past turns
- From vector search
- ~300 tokens

Layer 4: User Profile (persistent)
- Key facts about user
- Preferences, context
- ~100 tokens

Total: ~1,100 tokens for context
```

Layer 1 provides the immediate conversational context — the bot knows what was just said and can follow the thread of the current exchange. Layer 2 preserves awareness of the broader current session without consuming the token budget of the full history. Layer 3 reaches back into any point in the user's history, surfacing information from past sessions when it's relevant. Layer 4 provides persistent knowledge about the user that doesn't need to be retrieved because it's always relevant.

Here is an implementation of this architecture:

```python
class HybridMemoryChatbot:
    def __init__(self, user_id):
        self.user_id = user_id
        self.current_window = []                        # Layer 1
        self.recent_summary = ""                        # Layer 2
        self.memory = ConversationalMemory(user_id)     # Layer 3
        self.user_profile = self.load_user_profile()    # Layer 4

    def chat(self, user_message):
        # Layer 1: Add to current window
        self.current_window.append({"role": "user", "content": user_message})

        # Keep only last 5 turns in the window
        if len(self.current_window) > 10:
            to_summarize = self.current_window[:6]
            self.current_window = self.current_window[6:]

            # Layer 2: Update summary
            new_summary = self.summarize(to_summarize)
            self.recent_summary = new_summary

        # Layer 3: Retrieve relevant past turns
        relevant_past = self.memory.retrieve_relevant_history(user_message)

        # Build final context
        messages = [{"role": "system", "content": "You are a helpful assistant."}]

        # Add user profile (Layer 4)
        if self.user_profile:
            messages.append({
                "role": "system",
                "content": f"User context: {self.user_profile}"
            })

        # Add retrieved memory (Layer 3)
        if relevant_past:
            past_context = self.format_retrieved_memory(relevant_past)
            messages.append({"role": "system", "content": past_context})

        # Add recent summary (Layer 2)
        if self.recent_summary:
            messages.append({
                "role": "system",
                "content": f"Recent conversation: {self.recent_summary}"
            })

        # Add current window (Layer 1)
        messages.extend(self.current_window)

        # Generate response
        response = openai.ChatCompletion.create(
            model="gpt-3.5-turbo",
            messages=messages
        )

        assistant_message = response.choices[0].message.content

        # Update layers
        self.current_window.append({"role": "assistant", "content": assistant_message})
        self.memory.store_turn("assistant", assistant_message)

        return assistant_message
```

The ordering of context in the prompt matters. The user profile comes first because it provides the broadest context. Retrieved memory comes next, giving the LLM awareness of relevant past interactions. The recent summary follows, providing continuity within the current session. The current window comes last, placing the active conversation — the most important context — closest to the generation point, where the LLM pays the most attention (as the "Lost in the Middle" research from Module 3 demonstrated).

This architecture scales to conversations of any length while keeping total context around 1,100 tokens. A user can have a 200-turn conversation across multiple sessions spanning weeks, and the bot maintains coherent, contextual responses throughout. The token budget is bounded and predictable, making cost management straightforward.

The right configuration of layers depends on your use case. Customer support bots might allocate more tokens to the current window (since the active troubleshooting thread is most important) and fewer to retrieved memory. Personal assistants might do the opposite, investing more in retrieval and user profiles to maintain long-term continuity. Tutoring bots might emphasise the summary layer to track the student's learning progression within a session. Experiment with the token allocations and the number of retrieved turns to find the balance that works best for your application.

## Hands-on practical

Work through the accompanying notebook: [`module_4/04-retrieval-based-memory.ipynb`](https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules/blob/main/module_4/04-retrieval-based-memory.ipynb). It walks through a per-user ChromaDB conversation store, a chatbot that retrieves semantically relevant past turns, and a cross-session recall test.
