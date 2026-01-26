#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
#include <algorithm>

using namespace std;

struct Edge {
    int to;
    int capacity;
    int flow;
    int rev; // Index of the reverse edge ph[to]in gra
};

class MaxFlow {
    int N;
    // graph[u] stores all edges starting from u
    vector<vector<Edge>> graph;

public:
    MaxFlow(int n) : N(n), graph(n) {}

    // Returns the index of the added edge in graph[u]
    int addEdge(int u, int v, int cap) {
        // Forward edge: u -> v
        Edge a = {v, cap, 0, (int)graph[v].size()};
        // Backward edge: v -> u (Capacity 0)
        Edge b = {u, 0, 0, (int)graph[u].size()};
        
        graph[u].push_back(a);
        graph[v].push_back(b);
        
        // Return index of the forward edge (it's the last one added to u)
        return (int)graph[u].size() - 1;
    }

    // BFS to find augmenting path
    int bfs(int s, int t, vector<pair<int,int>>& parent) {
        fill(parent.begin(), parent.end(), make_pair(-1, -1));
        queue<int> q;
        q.push(s);
        parent[s] = {s, -1};

        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int i = 0; i < graph[u].size(); i++) {
                Edge &e = graph[u][i];
                // Check residual capacity (capacity - flow > 0)
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
        vector<pair<int,int>> parent(N);

        while (bfs(s, t, parent)) {
            int pathFlow = INT_MAX;

            // Find bottleneck capacity
            for (int v = t; v != s; ) {
                auto [u, idx] = parent[v];
                pathFlow = min(pathFlow, graph[u][idx].capacity - graph[u][idx].flow);
                v = u;
            }

            // Update residual capacities
            for (int v = t; v != s; ) {
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

    // Helper to get specific edge info
    Edge getEdge(int u, int idx) {
        return graph[u][idx];
    }
};

int main() {
    // Optimize I/O operations
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    MaxFlow mf(N);
    
    // Store: u, v, capacity, edge_index
    vector<tuple<int, int, int, int>> edges;

    for (int i = 0; i < M; i++) {
        int u, v, c;
        cin >> u >> v >> c;
        
        // Add edge and capture the specific index for this edge
        int idx = mf.addEdge(u, v, c);
        edges.emplace_back(u, v, c, idx);
    }

    int s, t;
    cin >> s >> t;

    cout << mf.edmondsKarp(s, t) << "\n";

    // Output flows using the stored indices
    for (const auto& [u, v, c, idx] : edges) {
        // Access the specific edge directly using the saved index
        Edge e = mf.getEdge(u, idx);
        
        // For simple graphs, e.flow is sufficient.
        // Even if there are back-flows, e.flow tracks the net flow on this specific pipe.
        cout << u << " " << v << " " << e.flow << "/" << c << "\n";
    }

    return 0;
}