#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
#include <algorithm>

using namespace std;

// --- TEMPLATE ---
struct Edge { int to, capacity, flow, rev; };
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
        q.push(s); parent[s] = {s, -1};
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

void solve(int t_case) {
    int N_nodes;
    cin >> N_nodes;
    // Node Splitting Mapping:
    // Node i (1-based) becomes:
    //   In-node:  i
    //   Out-node: i + N_nodes
    // Capacity logic handles the node limit.
    int MAX_ID = 2 * N_nodes + 2;
    int S = 0;
    int T = MAX_ID - 1;

    MaxFlow mf(MAX_ID);

    // 1. Node Capacities
    for (int i = 1; i <= N_nodes; i++) {
        int cap;
        cin >> cap;
        mf.addEdge(i, i + N_nodes, cap);
    }

    // 2. Edges
    int M;
    cin >> M;
    for (int i = 0; i < M; i++) {
        int u, v, c;
        cin >> u >> v >> c;
        // Connect u_out -> v_in
        mf.addEdge(u + N_nodes, v, c);
    }

    // 3. Sources and Sinks
    int B, D;
    cin >> B >> D;
    
    // Connect Super Source to provided Sources
    for(int i=0; i<B; i++){
        int u; cin >> u;
        mf.addEdge(S, u, INT_MAX); 
    }

    // Connect provided Sinks to Super Sink
    for(int i=0; i<D; i++){
        int v; cin >> v;
        mf.addEdge(v + N_nodes, T, INT_MAX);
    }

    cout << "Case " << t_case << ": " << mf.edmondsKarp(S, T) << endl;
}

int main() {
    int t;
    cin >> t;
    for (int i = 1; i <= t; i++) {
        solve(i);
    }
}