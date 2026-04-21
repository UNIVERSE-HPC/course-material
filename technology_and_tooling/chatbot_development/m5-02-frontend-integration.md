---
name: Frontend Integration
dependsOn:
  - technology_and_tooling.chatbot_development.m5-01-backend-design-with-fastapi
tags: [chatbots, deployment, streamlit, react, frontend, optional]
learningOutcomes:
  - Build a rapid-prototype chatbot UI with Streamlit.
  - Create a production-grade chat interface with React.
  - Connect frontend applications to a FastAPI backend.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    license: CC-BY-4.0
---

An API is useful for programmatic access and integration with other services, but most end users interact with chatbots through a visual interface. The choice of frontend technology depends on your audience and timeline. For internal tools, demos, and rapid prototyping, Streamlit lets you build a functional chat UI in pure Python with no web development knowledge. For production-facing applications where you need full control over the user experience, React (or a similar JavaScript framework) is the standard choice.

## Streamlit for Rapid Prototyping

Streamlit is a Python library that turns scripts into interactive web applications. You write Python, and Streamlit handles the HTML, CSS, and JavaScript. For chatbot interfaces, Streamlit provides dedicated components — `st.chat_message` for rendering message bubbles and `st.chat_input` for the text input field — that create a ChatGPT-like experience with minimal code.

The typical use cases for a Streamlit chatbot frontend are internal tools, demos for stakeholders, MVPs, and non-public applications. Anywhere that speed of development matters more than pixel-perfect design, Streamlit is the right choice.

Here is a complete Streamlit chatbot interface that communicates with the FastAPI backend:

```python
import streamlit as st
import requests

st.title("AI Chatbot")

# Initialize chat history in session state
if "messages" not in st.session_state:
    st.session_state.messages = []

# Display chat history
for message in st.session_state.messages:
    with st.chat_message(message["role"]):
        st.markdown(message["content"])

# Chat input
if prompt := st.chat_input("Ask me anything..."):
    st.session_state.messages.append({"role": "user", "content": prompt})

    with st.chat_message("user"):
        st.markdown(prompt)

    with st.chat_message("assistant"):
        message_placeholder = st.empty()

        response = requests.post(
            "http://localhost:8000/chat",
            json={
                "message": prompt,
                "user_id": "streamlit_user",
                "conversation_id": st.session_state.get("conv_id")
            }
        )

        if response.status_code == 200:
            data = response.json()
            full_response = data["response"]
            st.session_state["conv_id"] = data["conversation_id"]

            message_placeholder.markdown(full_response)

            if data.get("sources"):
                with st.expander("Sources"):
                    for source in data["sources"]:
                        st.write(f"- {source}")
        else:
            st.error("Error calling chatbot API")

    st.session_state.messages.append({"role": "assistant", "content": full_response})

# Sidebar controls
with st.sidebar:
    st.header("Controls")

    if st.button("Clear Conversation"):
        st.session_state.messages = []
        st.session_state.pop("conv_id", None)
        st.rerun()

    st.divider()
    st.caption("Powered by OpenAI GPT-3.5")
```

This is roughly 50 lines of Python, and it produces a fully functional chat interface. `st.session_state` persists data across Streamlit's reruns (Streamlit re-executes the entire script on every interaction). The chat history is stored in session state and re-rendered on each run. The sidebar provides controls like clearing the conversation. The sources expander shows RAG attribution when available.

To run it:

```bash
pip install streamlit
streamlit run app.py
```

Streamlit opens the application in your browser automatically. The interface auto-reloads when you change the code, making the development cycle fast.

## React for Production Interfaces

For user-facing applications that need full control over the experience — custom styling, complex interactions, offline support, accessibility — React is the standard. A React frontend communicates with the same FastAPI backend through HTTP requests, but gives you complete control over how the interface looks and behaves.

Here is a React chat component:

```javascript
import React, { useState, useEffect, useRef } from 'react';
import './ChatInterface.css';

function ChatInterface() {
  const [messages, setMessages] = useState([]);
  const [input, setInput] = useState('');
  const [conversationId, setConversationId] = useState(null);
  const [loading, setLoading] = useState(false);
  const messagesEndRef = useRef(null);

  useEffect(() => {
    messagesEndRef.current?.scrollIntoView({ behavior: "smooth" });
  }, [messages]);

  const sendMessage = async () => {
    if (!input.trim()) return;

    const userMessage = { role: 'user', content: input };
    setMessages(prev => [...prev, userMessage]);
    setInput('');
    setLoading(true);

    try {
      const response = await fetch('http://localhost:8000/chat', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
          message: input,
          user_id: 'react_user',
          conversation_id: conversationId
        })
      });

      const data = await response.json();

      setMessages(prev => [...prev, {
        role: 'assistant',
        content: data.response,
        sources: data.sources
      }]);

      setConversationId(data.conversation_id);
    } catch (error) {
      console.error('Error:', error);
      setMessages(prev => [...prev, {
        role: 'assistant',
        content: 'Sorry, something went wrong.'
      }]);
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="chat-container">
      <div className="messages">
        {messages.map((msg, idx) => (
          <div key={idx} className={`message ${msg.role}`}>
            <div className="message-content">{msg.content}</div>
            {msg.sources && msg.sources.length > 0 && (
              <div className="sources">
                Sources: {msg.sources.join(', ')}
              </div>
            )}
          </div>
        ))}
        {loading && <div className="loading">Thinking...</div>}
        <div ref={messagesEndRef} />
      </div>

      <div className="input-area">
        <input
          type="text"
          value={input}
          onChange={(e) => setInput(e.target.value)}
          onKeyPress={(e) => e.key === 'Enter' && sendMessage()}
          placeholder="Type your message..."
        />
        <button onClick={sendMessage} disabled={loading}>
          Send
        </button>
      </div>
    </div>
  );
}

export default ChatInterface;
```

The component manages its state with React hooks: `useState` for messages, input text, conversation ID, and loading state; `useEffect` for auto-scrolling to the bottom when new messages arrive; and `useRef` for the scroll target element. The `sendMessage` function handles the full lifecycle of a user message: adding it to the display, calling the backend API, and appending the response.

The accompanying CSS provides a clean chat layout:

```css
.chat-container {
  display: flex;
  flex-direction: column;
  height: 100vh;
  max-width: 800px;
  margin: 0 auto;
}

.messages {
  flex: 1;
  overflow-y: auto;
  padding: 20px;
}

.message {
  margin-bottom: 15px;
  padding: 12px;
  border-radius: 8px;
}

.message.user {
  background: #007bff;
  color: white;
  margin-left: 20%;
}

.message.assistant {
  background: #f1f1f1;
  margin-right: 20%;
}

.sources {
  font-size: 0.85em;
  margin-top: 8px;
  opacity: 0.8;
}

.input-area {
  display: flex;
  padding: 20px;
  border-top: 1px solid #ddd;
}

.input-area input {
  flex: 1;
  padding: 12px;
  border: 1px solid #ddd;
  border-radius: 4px;
}

.input-area button {
  margin-left: 10px;
  padding: 12px 24px;
  background: #007bff;
  color: white;
  border: none;
  border-radius: 4px;
  cursor: pointer;
}
```

This is a starting point. A production chat interface would add features like typing indicators, message timestamps, file upload support, markdown rendering in responses, keyboard shortcuts, and accessibility attributes. Libraries like `react-chat-elements` or `stream-chat-react` provide pre-built components for these features.

The key architectural point is that both frontends — Streamlit and React — communicate with the same FastAPI backend through the same HTTP endpoints. The backend doesn't know or care which frontend is calling it. This separation of concerns means you can develop and deploy the frontend and backend independently, swap one frontend for another without changing the backend, and support multiple clients (web, mobile, API integrations) simultaneously.
