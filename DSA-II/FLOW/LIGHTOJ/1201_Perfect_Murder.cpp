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
    int rev;
};

class MaxFlow {
public:
    int N;
    vector<vector<Edge>> graph;

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
};
// --- TEMPLATE END ---

void solve(int t) {
    int N, M;
    cin >> N >> M;

    // 1. Read Graph and Run 2-Coloring (Bipartite Check)
    vector<vector<int>> adj(N + 1);
    for (int i = 0; i < M; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // color array: 0 = unvisited, 1 = Set A, 2 = Set B
    vector<int> color(N + 1, 0);
    
    // We must handle disconnected components
    for (int i = 1; i <= N; i++) {
        if (color[i] == 0) {
            queue<int> q;
            q.push(i);
            color[i] = 1; // Start with Set A

            while (!q.empty()) {
                int u = q.front(); q.pop();
                for (int v : adj[u]) {
                    if (color[v] == 0) {
                        color[v] = (color[u] == 1) ? 2 : 1; // Flip color
                        q.push(v);
                    }
                }
            }
        }
    }

    // 2. Build Flow Network
    int S = 0, T = N + 1;
    MaxFlow mf(T + 1);

    for (int i = 1; i <= N; i++) {
        if (color[i] == 1) {
            // Source -> Set A
            mf.addEdge(S, i, 1);
            
            // Set A -> Set B (Friendships)
            // We iterate neighbors. If neighbor is Set B, add edge.
            for (int v : adj[i]) {
                // Since graph is undirected, edges appear twice.
                // We only add flow edge from Set A -> Set B to avoid duplicates/cycles.
                if (color[v] == 2) {
                    mf.addEdge(i, v, 1);
                }
            }
        } else {
            // Set B -> Sink
            mf.addEdge(i, T, 1);
        }
    }

    // 3. Calculate Maximum Matching
    int matching = mf.edmondsKarp(S, T);

    // 4. Result = Total Nodes - Matching
    cout << "Case " << t << ": " << (N - matching) << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    if (cin >> t) {
        for (int i = 1; i <= t; i++) {
            solve(i);
        }
    }
    return 0;
}