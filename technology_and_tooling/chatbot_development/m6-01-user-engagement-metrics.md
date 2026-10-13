---
name: User Engagement Metrics
dependsOn:
  - technology_and_tooling.chatbot_development.m5-05-authentication-and-security
tags: [chatbots, analytics, engagement, metrics, optional]
learningOutcomes:
  - Define and calculate key engagement metrics for chatbot applications.
  - Implement an engagement tracking system that logs messages and sessions.
  - Calculate retention rates using cohort analysis.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    image: https://github.com/OxfordCompetencyCenters.png
    license: CC-BY-4.0
---

Your chatbot is deployed and users are interacting with it. But how is it actually performing? Are users finding value? Do they return? Where do they drop off? Without analytics, you're operating blind — making changes based on intuition rather than evidence, unable to tell whether an update improved the experience or degraded it.

Analytics separates good chatbots from great ones. The metrics covered in this module give you a quantitative understanding of user behaviour, conversation quality, system performance, and the impact of changes. By the end, every decision you make about your chatbot will be grounded in data.

## Key Engagement Metrics

Engagement metrics answer the fundamental question: are people using your chatbot, and do they find it useful enough to come back?

**Active users** measure the size and consistency of your user base. Daily Active Users (DAU) counts unique users per day. Weekly and Monthly Active Users (WAU, MAU) provide broader views. The ratio DAU/MAU — called "stickiness" — reveals how frequently users return. A stickiness ratio of 0.5 means the average monthly user engages on 15 out of 30 days, which is exceptionally high. For most chatbots, a stickiness ratio of 0.1–0.2 is healthy.

**Session metrics** describe the depth of individual interactions. Session duration measures how long users engage in a single conversation. Messages per session measures how many exchanges occur. Sessions per user measures how often users return. Together, these paint a picture of usage patterns: are users having brief, transactional exchanges (1–2 messages) or extended, substantive conversations (10+ messages)?

**Retention** is the most important engagement metric. It measures whether users come back. Day 1 retention is the percentage of users who return the next day. Day 7 and Day 30 retention track weekly and monthly return rates. For customer support chatbots, good benchmarks are 30–40% day-1 retention, 15–25% day-7 retention, and average sessions of 6–10 messages lasting 3–5 minutes. If your day-1 retention is below 15% or more than half of sessions consist of a single message, users aren't finding enough value to continue the conversation.

## Implementing Engagement Tracking

The foundation of engagement analytics is logging every message to a database where it can be queried later. The `EngagementTracker` class provides the core tracking and calculation capabilities:

```python
from datetime import datetime, timedelta
from collections import defaultdict

class EngagementTracker:
    def __init__(self, db_connection):
        self.db = db_connection

    def log_message(self, user_id, session_id, role, content, timestamp=None):
        """Log every message for analytics."""
        if timestamp is None:
            timestamp = datetime.now()

        self.db.execute("""
            INSERT INTO messages (user_id, session_id, role, content, timestamp)
            VALUES (?, ?, ?, ?, ?)
        """, (user_id, session_id, role, content, timestamp))

    def get_daily_active_users(self, date):
        """Count unique users who sent messages on a date."""
        result = self.db.execute("""
            SELECT COUNT(DISTINCT user_id)
            FROM messages
            WHERE DATE(timestamp) = ?
        """, (date,))
        return result.fetchone()[0]

    def get_retention(self, cohort_date, days_later):
        """Calculate retention for a cohort of users.

        Args:
            cohort_date: The date to analyse (e.g., "2024-01-01")
            days_later: Days after cohort_date to check (e.g., 1, 7, 30)
        """
        cohort_users = self.db.execute("""
            SELECT DISTINCT user_id
            FROM messages
            WHERE DATE(timestamp) = ?
        """, (cohort_date,)).fetchall()

        cohort_size = len(cohort_users)
        if cohort_size == 0:
            return 0

        target_date = (datetime.strptime(cohort_date, "%Y-%m-%d") +
                      timedelta(days=days_later)).strftime("%Y-%m-%d")

        returned_users = self.db.execute("""
            SELECT COUNT(DISTINCT user_id)
            FROM messages
            WHERE user_id IN ({})
              AND DATE(timestamp) = ?
        """.format(','.join('?' * cohort_size)),
        tuple([u[0] for u in cohort_users] + [target_date])).fetchone()[0]

        return (returned_users / cohort_size) * 100

    def get_session_stats(self, session_id):
        """Get statistics for a conversation session."""
        messages = self.db.execute("""
            SELECT role, content, timestamp
            FROM messages
            WHERE session_id = ?
            ORDER BY timestamp
        """, (session_id,)).fetchall()

        if not messages:
            return None

        first_msg = datetime.fromisoformat(messages[0][2])
        last_msg = datetime.fromisoformat(messages[-1][2])
        duration = (last_msg - first_msg).total_seconds()

        return {
            "message_count": len(messages),
            "duration_seconds": duration,
            "user_messages": sum(1 for m in messages if m[0] == 'user'),
            "bot_messages": sum(1 for m in messages if m[0] == 'assistant')
        }

    def get_engagement_report(self, start_date, end_date):
        """Generate a comprehensive engagement report."""
        dau = self.get_daily_active_users(end_date)

        sessions = self.db.execute("""
            SELECT session_id FROM messages
            WHERE DATE(timestamp) BETWEEN ? AND ?
            GROUP BY session_id
        """, (start_date, end_date)).fetchall()

        session_stats = [self.get_session_stats(s[0]) for s in sessions]
        avg_duration = sum(s['duration_seconds'] for s in session_stats) / len(session_stats)
        avg_messages = sum(s['message_count'] for s in session_stats) / len(session_stats)

        return {
            "dau": dau,
            "total_sessions": len(sessions),
            "avg_session_duration": avg_duration,
            "avg_messages_per_session": avg_messages
        }
```

Integrating the tracker into your API is straightforward — log every message that passes through the chat endpoint:

```python
tracker = EngagementTracker(db_connection)

@app.post("/chat")
async def chat(request: ChatRequest):
    # ... generate response ...

    tracker.log_message(request.user_id, session_id, "user", request.message)
    tracker.log_message(request.user_id, session_id, "assistant", response)

    return response
```

The principle is to log everything at the message level, then calculate metrics from the raw data. This gives you maximum flexibility — you can compute new metrics later without modifying the tracking code. In a production system, you would use a dedicated analytics database (ClickHouse, BigQuery) or an analytics service (Mixpanel, Amplitude) rather than querying the application database directly. The retention calculation is particularly valuable: it identifies which acquisition channels, features, or time periods produce users who stick around, and that insight directly informs where to invest your effort.
