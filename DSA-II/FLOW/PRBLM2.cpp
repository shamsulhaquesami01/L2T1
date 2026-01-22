#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
#include <algorithm>

using namespace std;

// --- Reuse your Exact Edge Struct and MaxFlow Class ---
struct Edge {
    int to;
    int capacity;
    int flow;
    int rev; 
};

class MaxFlow {
    int N;
    vector<vector<Edge>> graph;

public:
    MaxFlow(int n) : N(n), graph(n) {}

    int addEdge(int u, int v, int cap) {
        Edge a = {v, cap, 0, (int)graph[v].size()};
        Edge b = {u, 0, 0, (int)graph[u].size()}; 
        graph[u].push_back(a);
        graph[v].push_back(b);
        return (int)graph[u].size() - 1;
    }

    int bfs(int s, int t, vector<pair<int,int>>& parent) {
        fill(parent.begin(), parent.end(), make_pair(-1, -1));
        queue<int> q;
        q.push(s);
        parent[s] = {s, -1};

        while (!q.empty()) {
            int u = q.front(); q.pop();
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
        vector<pair<int,int>> parent(N);

        while (bfs(s, t, parent)) {
            int pathFlow = INT_MAX;

            for (int v = t; v != s; ) {
                auto [u, idx] = parent[v];
                pathFlow = min(pathFlow, graph[u][idx].capacity - graph[u][idx].flow);
                v = u;
            }

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

    Edge getEdge(int u, int idx) {
        return graph[u][idx];
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, K, M;
    if (!(cin >> N >> K >> M)) return 0;

    // We need two extra nodes for Source and Sink
    // Let Source = N, Sink = N + 1
    int s = N;
    int t = N + 1;
    
    // Create MaxFlow object with N + 2 nodes
    MaxFlow mf(N + 2);

    // 1. Connect Source (s) to all Officers (0 to K-1)
    for (int i = 0; i < K; i++) {
        mf.addEdge(s, i, 1);
    }

    // 2. Connect all Agents (K to N-1) to Sink (t)
    for (int i = K; i < N; i++) {
        mf.addEdge(i, t, 1);
    }

    // 3. Read Compatibility Edges (Officer -> Agent)
    // We store these to verify which ones get flow later
    vector<tuple<int, int, int>> potential_matches;

    for (int i = 0; i < M; i++) {
        int u, v;
        cin >> u >> v;
        
        // Add edge with Capacity 1
        int idx = mf.addEdge(u, v, 1);
        
        // Store u, v, and the specific edge index
        potential_matches.emplace_back(u, v, idx);
    }

    // 4. Calculate Max Flow
    cout << mf.edmondsKarp(s, t) << endl;

    // 5. Print the pairs
    // Check flow on the "middle" edges (Officer -> Agent)
    for (const auto& [u, v, idx] : potential_matches) {
        Edge e = mf.getEdge(u, idx);
        
        // If flow is 1, this officer is matched with this agent
        if (e.flow == 1) {
            cout << u << " " << v << endl;
        }
    }

    return 0;
}