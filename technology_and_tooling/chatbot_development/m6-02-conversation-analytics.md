---
name: Conversation Analytics
dependsOn:
  - technology_and_tooling.chatbot_development.m6-01-user-engagement-metrics
tags: [chatbots, analytics, conversation-quality, intent-analysis, optional]
learningOutcomes:
  - Measure conversation quality using fallback rate, goal completion, and abandonment metrics.
  - Cluster user queries to discover common intents and coverage gaps.
  - Map conversation flows to identify drop-off points and failure patterns.
attribution:
  - citation: >
      Oxford AI Competency Centre (2026). Chatbot Development Course.
    url: https://github.com/OxfordCompetencyCenters/aicc_chatbot_modules
    image: https://github.com/OxfordCompetencyCenters.png
    license: CC-BY-4.0
---

Engagement metrics tell you how much people use your chatbot. Conversation analytics tell you how well the chatbot actually serves them. A bot can have high DAU while consistently failing to answer questions — users keep trying, get frustrated, and eventually leave. Understanding conversation quality requires measuring where conversations succeed, where they fail, and what users are actually asking about.

## Conversation Quality Metrics

Five metrics capture the core dimensions of conversation quality.

**Fallback rate** is the percentage of conversations where the bot produces a response equivalent to "I don't understand" or "I can't help with that." A fallback rate below 10% is good; above 25% is concerning and suggests the bot's knowledge base or retrieval system has significant gaps. Tracking which specific queries trigger fallbacks reveals exactly where to focus improvement efforts.

**Goal completion rate** measures the percentage of conversations where the user achieves their objective — an issue resolved, a product found, a question answered. This requires defining what "completion" means for your use case, which may involve explicit signals (user clicks "resolved"), implicit signals (user doesn't re-open the conversation), or LLM-based assessment. A completion rate above 70% is good; below 40% indicates serious problems.

**Escalation rate** tracks the percentage of conversations handed off to a human agent. A lower rate generally means the bot is handling more queries autonomously, but pushing escalation rate to zero at the cost of user satisfaction is counterproductive. Below 15% is a good target; above 30% suggests the bot isn't capable enough for the queries it receives.

**Response relevance** assesses how well the bot's answers match the user's questions. This can be measured through explicit user feedback (thumbs up/down, star ratings) or automated evaluation using an LLM as a judge. An average rating above 4.0 out of 5 is the target.

**Conversation abandonment** tracks where users stop responding mid-conversation. A user who asks a question, receives an answer, and leaves naturally is fine. A user who asks three questions, gets increasingly irrelevant answers, and gives up represents a failure. Analysing which bot responses precede abandonment reveals the specific answers that are driving users away.

The SQL query to find the most common failure points is revealing:

```sql
SELECT user_message, COUNT(*) as frequency
FROM conversations
WHERE next_message IS NULL
  AND session_messages < 5
GROUP BY user_message
ORDER BY frequency DESC
LIMIT 20
```

This surfaces the messages that most frequently cause users to leave early. Each one is a specific opportunity for improvement: a question the bot can't answer, a response that confuses users, or a topic where the knowledge base is inadequate.

## Intent Coverage Analysis

Understanding what users actually want from your chatbot is essential for prioritising improvements. Intent analysis uses clustering to group similar user queries together, revealing the most common topics and identifying gaps in coverage.

```python
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.cluster import KMeans

class IntentAnalyzer:
    def __init__(self, n_clusters=20):
        self.vectorizer = TfidfVectorizer(max_features=100, stop_words='english')
        self.kmeans = KMeans(n_clusters=n_clusters, random_state=42)

    def analyze_user_queries(self, queries):
        """Cluster user queries to identify common intents."""
        X = self.vectorizer.fit_transform(queries)
        clusters = self.kmeans.fit_predict(X)

        cluster_info = {}
        for cluster_id in range(self.kmeans.n_clusters):
            cluster_queries = [q for i, q in enumerate(queries)
                             if clusters[i] == cluster_id]

            cluster_center = self.kmeans.cluster_centers_[cluster_id]
            top_terms_idx = cluster_center.argsort()[-5:][::-1]
            top_terms = [self.vectorizer.get_feature_names_out()[i]
                        for i in top_terms_idx]

            cluster_info[cluster_id] = {
                "size": len(cluster_queries),
                "top_terms": top_terms,
                "sample_queries": cluster_queries[:5]
            }

        return cluster_info
```

Running this against a month of user queries might produce output like:

```
Cluster 0 (450 queries):
  Topics: password, reset, login, account, forgot
  Examples: ['How do I reset my password?', 'Forgot my login', "Can't access my account"]

Cluster 1 (320 queries):
  Topics: pricing, cost, plan, subscription, upgrade
  Examples: ['What does the pro plan cost?', 'How much is an upgrade?', ...]
```

This is directly actionable. If 30% of queries are about password resets, improving that flow should be the top priority. If 20% ask about pricing but the knowledge base doesn't include pricing information, that's a clear gap to fill. Clusters with low goal completion rates are where improvement effort will have the biggest impact.

For more sophisticated intent analysis, you can use BERT embeddings with HDBSCAN clustering or LLM-based classification. The TF-IDF approach shown here is a solid starting point that works well for initial exploration.

## Conversation Flow Mapping

Journey mapping visualises how users navigate conversations. By building a directed graph of conversation flows — where nodes are intents or message types and edges represent transitions — you can see the most common paths, identify dead ends, and spot loops where users rephrase the same question because the bot didn't answer it.

```python
import networkx as nx
import matplotlib.pyplot as plt

def build_conversation_graph(conversations):
    """Build a graph of conversation flows."""
    graph = nx.DiGraph()

    for conv in conversations:
        messages = conv['messages']

        for i in range(len(messages) - 1):
            current = messages[i]['intent']
            next_msg = messages[i + 1]['intent']

            if graph.has_edge(current, next_msg):
                graph[current][next_msg]['weight'] += 1
            else:
                graph.add_edge(current, next_msg, weight=1)

    return graph

def find_drop_off_points(graph):
    """Identify where users abandon conversations."""
    drop_offs = []

    for node in graph.nodes():
        out_degree = graph.out_degree(node)
        in_degree = graph.in_degree(node)

        if in_degree > 10 and out_degree < 3:
            drop_offs.append({
                "node": node,
                "in_degree": in_degree,
                "out_degree": out_degree,
                "drop_off_rate": 1 - (out_degree / in_degree)
            })

    return sorted(drop_offs, key=lambda x: x['in_degree'], reverse=True)
```

The `find_drop_off_points` function identifies nodes in the graph where many conversations arrive (high in-degree) but few continue (low out-degree). These are the points where users give up. A typical finding might be: "40% of users who ask about refunds don't respond to the next message." That tells you the refund response is inadequate and needs improvement.

Loops in the graph — where users cycle back to the same node repeatedly — indicate frustration. The user is rephrasing their question because the bot didn't understand or didn't provide a useful answer. These loops are high-priority targets for improvement because the user is clearly motivated (they keep trying) but the bot is failing them.

Visualising the graph with the most common paths highlighted gives you a map of the "happy paths" through your chatbot:

```python
def visualize_top_paths(graph, top_n=10):
    """Visualise the most common conversation paths."""
    edges = [(u, v, data['weight']) for u, v, data in graph.edges(data=True)]
    top_edges = sorted(edges, key=lambda x: x[2], reverse=True)[:top_n]

    subgraph = nx.DiGraph()
    for u, v, weight in top_edges:
        subgraph.add_edge(u, v, weight=weight)

    pos = nx.spring_layout(subgraph)
    nx.draw(subgraph, pos, with_labels=True, node_color='lightblue',
            node_size=2000, font_size=8, arrows=True)

    edge_labels = {(u, v): data['weight']
                  for u, v, data in subgraph.edges(data=True)}
    nx.draw_networkx_edge_labels(subgraph, pos, edge_labels)

    plt.title("Top Conversation Paths")
    plt.show()
```

This analysis directly informs product decisions. The most-travelled happy paths should be optimised for speed and clarity. Dead ends need better responses or escalation options. Loops need the underlying question to be answered properly. The resulting improvements are measurable through the same quality metrics, creating a closed feedback loop between analytics and product development.
