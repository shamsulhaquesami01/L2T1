#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
#include <algorithm>

using namespace std;

const int INF = 1e9;

// --- STANDARD MAXFLOW TEMPLATE ---
struct Edge {
    int to;
    int capacity;
    int flow;
    int rev; // Index of the reverse edge
};

class MaxFlow {
public:
    int N;
    vector<vector<Edge>> graph;

    MaxFlow(int n) : N(n), graph(n) {}

    // Add edge and return its index in the adjacency list
    int addEdge(int u, int v, int cap) {
        Edge a = {v, cap, 0, (int)graph[v].size()};
        Edge b = {u, 0, 0, (int)graph[u].size()};
        graph[u].push_back(a);
        graph[v].push_back(b);
        return (int)graph[u].size() - 1;
    }

    int bfs(int s, int t, vector<pair<int, int>> &parent) {
        fill(parent.begin(), parent.end(), make_pair(-1, -1));
        queue<int> q;
        q.push(s);
        parent[s] = {s, -1};

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int i = 0; i < graph[u].size(); i++) {
                Edge &e = graph[u][i];
                if (parent[e.to].first == -1 && e.capacity > e.flow) {
                    parent[e.to] = {u, i};
                    if (e.to == t) return 1;
                    q.push(e.to);
                }
            }
        }
        return 0;
    }

    int edmondsKarp(int s, int t) {
        int maxFlow = 0;
        vector<pair<int, int>> parent(N);

        while (bfs(s, t, parent)) {
            int pathFlow = INT_MAX;
            for (int v = t; v != s;) {
                auto [u, idx] = parent[v];
                pathFlow = min(pathFlow, graph[u][idx].capacity - graph[u][idx].flow);
                v = u;
            }
            for (int v = t; v != s;) {
                auto [u, idx] = parent[v];
                graph[u][idx].flow += pathFlow;
                int rev_idx = graph[u][idx].rev;
                graph[v][rev_idx].flow -= pathFlow;
                v = u;
            }
            maxFlow += pathFlow;
        }
        return maxFlow;
    }
};

struct InputEdge {
    int u, v, id;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    MaxFlow mf(n + 1);
    vector<InputEdge> edges;

    for (int i = 0; i < m; i++) {
        int u, v, c;
        cin >> u >> v >> c;
        int idx = mf.addEdge(u, v, c);
        edges.push_back({u, v, idx});
    }

    int s, t;
    cin >> s >> t;

    // 1. Compute Initial Max Flow
    int base_flow = mf.edmondsKarp(s, t);
    int max_possible_flow = base_flow;

    // 2. Find nodes reachable from S in Residual Graph (S-Set)
    vector<bool> vis_s(n + 1, false);
    queue<int> q;
    q.push(s);
    vis_s[s] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (auto &e : mf.graph[u]) {
            if (!vis_s[e.to] && e.capacity > e.flow) {
                vis_s[e.to] = true;
                q.push(e.to);
            }
        }
    }

    // 3. Find nodes that can reach T in Residual Graph (T-Set)
    // We do this by BFSing backwards from T on residual edges
    vector<bool> vis_t(n + 1, false);
    while(!q.empty()) q.pop();
    q.push(t);
    vis_t[t] = true;
    
    // Build reverse lookup for BFS traversal
    // Note: A forward residual edge u->v exists if cap > flow.
    // To traverse backwards, we check if u->v exists when standing at v.
    vector<vector<int>> rev_adj(n + 1);
    for(int u = 1; u <= n; u++) {
        for(auto &e : mf.graph[u]) {
            if(e.capacity > e.flow) {
                rev_adj[e.to].push_back(u);
            }
        }
    }

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : rev_adj[u]) {
            if (!vis_t[v]) {
                vis_t[v] = true;
                q.push(v);
            }
        }
    }

    // 4. Test each candidate edge
    // Save the state of the graph to restore after each test
    vector<vector<Edge>> saved_graph = mf.graph;

    for (const auto &edge : edges) {
        int u = edge.u;
        int v = edge.v;
        int idx = edge.id;

        // Condition: Edge must be saturated (part of a cut)
        // AND u must be reachable from S, and v must be able to reach T
        if (mf.graph[u][idx].capacity == mf.graph[u][idx].flow && vis_s[u] && vis_t[v]) {
            
            // A. Increase Capacity to Infinity (or a large number)
            mf.graph[u][idx].capacity += INF;

            // B. Run Max Flow again (incrementally)
            int extra_flow = mf.edmondsKarp(s, t);
            
            // C. Update Maximum
            max_possible_flow = max(max_possible_flow, base_flow + extra_flow);

            // D. Restore Graph State for next iteration
            mf.graph = saved_graph; 
        }
    }

    cout << "Initial Max Flow: " << base_flow << endl;
    cout << "Max Flow after extending one edge: " << max_possible_flow << endl;

    return 0;
}