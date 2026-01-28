#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
#include <algorithm>

using namespace std;

// --- YOUR STANDARD TEMPLATE START ---
struct Edge {
    int to;
    int capacity;
    int flow;
    int rev; // Index of the reverse edge
};

class MaxFlow {
    int N;
    vector<vector<Edge>> graph;

public:
    MaxFlow(int n) : N(n), graph(n) {}

    int addEdge(int u, int v, int cap) {
        // Forward edge: u -> v
        Edge a = {v, cap, 0, (int)graph[v].size()};
        // Backward edge: v -> u
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
// --- YOUR TEMPLATE END ---

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    // Store edges to rebuild the graph for each iteration
    vector<pair<int, int>> edges;
    for (int i = 0; i < M; i++) {
        int u, v;
        cin >> u >> v;
        edges.push_back({u, v});
    }

    // Edge Connectivity Calculation
    // We assume the graph is connected initially.
    // If N < 2, connectivity is theoretically undefined or 0.
    if (N < 2) {
        cout << "0" << endl;
        return 0;
    }

    int min_connectivity = INT_MAX;

    // Fix Source = 0
    int S = 0;

    // Try every other node as Sink
    for (int T = 1; T < N; T++) {
        // 1. Create a fresh graph instance
        MaxFlow mf(N);

        // 2. Add edges with Capacity 1
        for (auto p : edges) {
            // Assuming Undirected Graph for standard Edge Connectivity
            // u -> v capacity 1
            mf.addEdge(p.first, p.second, 1);
            // v -> u capacity 1
            mf.addEdge(p.second, p.first, 1);
        }

        // 3. Compute Max Flow (Min Cut) for this pair
        int flow = mf.edmondsKarp(S, T);

        // 4. Update Global Minimum
        min_connectivity = min(min_connectivity, flow);
    }

    cout << min_connectivity << endl;

    return 0;
}