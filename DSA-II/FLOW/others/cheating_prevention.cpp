#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
#include <algorithm>

using namespace std;

// --- STANDARD MAXFLOW TEMPLATE ---
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

void solve() {
    int N, G, E;
    // Input format: N G E
    if (!(cin >> N >> G >> E)) return;

    vector<int> targets(G);
    for(int i=0; i<G; i++) {
        cin >> targets[i];
    }

    // Graph Construction
    // Nodes 0..N-1
    // Split: u_in = u, u_out = u + N
    // Super Source (Virtual) -> 0_in (Already handled by starting at 0)
    // Super Sink = 2*N
    
    int S = 0;       // Start at Takahashi's In-node (0)
    int T = 2 * N;   // Super Sink
    
    MaxFlow mf(T + 1);

    // 1. Node Capacities (Internal Edges)
    for(int i=0; i<N; i++) {
        int u_in = i;
        int u_out = i + N;
        
        if (i == 0) {
            // Takahashi cannot be removed -> Infinite Capacity
            mf.addEdge(u_in, u_out, INT_MAX);
        } else {
            // Everyone else costs 1 to remove
            mf.addEdge(u_in, u_out, 1);
        }
    }

    // 2. Friendship Edges
    for(int i=0; i<E; i++) {
        int u, v;
        cin >> u >> v;
        
        // u_out -> v_in (Cap 1)
        mf.addEdge(u + N, v, 1);
        // v_out -> u_in (Cap 1)
        mf.addEdge(v + N, u, 1);
    }

    // 3. Connect Targets to Super Sink
    for(int p : targets) {
        // If message reaches p_out, she has seen it.
        // Connect p_out -> T with infinite capacity
        mf.addEdge(p + N, T, INT_MAX);
    }

    // Min Cost = Max Flow
    cout << mf.edmondsKarp(S, T) << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}