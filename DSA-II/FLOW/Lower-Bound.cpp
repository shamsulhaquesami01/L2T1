#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
#include <algorithm>

using namespace std;

// --- STANDARD TEMPLATE START ---
struct Edge {
    int to;
    long long capacity; // Use long long for safety
    long long flow;
    int rev;
};

class MaxFlow {
public:
    int N;
    vector<vector<Edge>> graph;

    MaxFlow(int n) : N(n), graph(n) {}

    int addEdge(int u, int v, long long cap) {
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

    long long edmondsKarp(int s, int t) {
        long long maxFlow = 0;
        vector<pair<int,int>> parent(N);
        while (bfs(s, t, parent)) {
            long long pathFlow = LLONG_MAX;
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
// --- STANDARD TEMPLATE END ---

struct InputEdge {
    int u, v;
    long long lower_bound, capacity;
    int edge_index; // Index in graph[u]
};

void solve() {
    int n, m;
    if (!(cin >> n >> m)) return;

    // We need extra nodes for Super Source (SS) and Super Sink (TT)
    // Let's use 0-based indexing for internal logic
    // Nodes 0..n-1 are original nodes.
    // SS = n, TT = n + 1
    int SS = n;
    int TT = n + 1;

    MaxFlow mf(n + 2);
    vector<long long> demand(n, 0);
    vector<InputEdge> input_edges;

    // 1. Read Edges and Transform Constraints
    for (int i = 0; i < m; i++) {
        int u, v;
        long long l, c;
        cin >> u >> v >> l >> c;
        // Adjust for 0-based indexing if input is 1-based. Assuming input is 0-based based on prompt context.
        // If input is 1-based (1..N), use u-1, v-1.
        // Let's assume standard CP format 1..N often, but your previous code used 0-based.
        // Wait, the Problem image says "Line 1: N M", typical for 1-based graphs.
        // Let's assume 1-based for safety and adjust.
        u--; v--; 

        // Add edge with capacity (c - l)
        int idx = mf.addEdge(u, v, c - l);
        
        // Update Demands
        demand[u] -= l;
        demand[v] += l;

        // Store info to print later
        input_edges.push_back({u, v, l, c, idx});
    }

    int s, t;
    cin >> s >> t;
    s--; t--;

    // 2. Setup Super Source and Super Sink based on Demands
    long long total_demand_needed = 0;
    for (int i = 0; i < n; i++) {
        if (demand[i] > 0) {
            mf.addEdge(SS, i, demand[i]);
            total_demand_needed += demand[i];
        } else if (demand[i] < 0) {
            mf.addEdge(i, TT, -demand[i]);
        }
    }

    // 3. Add Infinite Edge from t -> s to close the cycle
    int infinity_edge_idx = mf.addEdge(t, s, LLONG_MAX);

    // 4. Run Max Flow for Feasibility (SS -> TT)
    long long flow_ss_tt = mf.edmondsKarp(SS, TT);

    if (flow_ss_tt != total_demand_needed) {
        cout << "IMPOSSIBLE" << endl;
        return;
    }

    // 5. If Feasible, calculate Max Flow (s -> t)
    // The current flow on the infinity edge (t->s) is the valid "base flow".
    long long base_flow = mf.graph[t][infinity_edge_idx].flow;

    // Disconnect t->s edge to prevent looping back during maximization
    mf.graph[t][infinity_edge_idx].capacity = 0;
    mf.graph[t][infinity_edge_idx].flow = 0;
    // Also clear the reverse edge (s->t) just in case
    int rev_idx = mf.graph[t][infinity_edge_idx].rev;
    mf.graph[s][rev_idx].capacity = 0;
    mf.graph[s][rev_idx].flow = 0;

    // Run Max Flow again on the residual graph
    long long extra_flow = mf.edmondsKarp(s, t);

    cout << (base_flow + extra_flow) << endl;

    // 6. Print Flow on each edge
    for (const auto& edge : input_edges) {
        // Actual Flow = Flow in transformed graph + Lower Bound
        long long current_flow = mf.graph[edge.u][edge.edge_index].flow;
        cout << (current_flow + edge.lower_bound) << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}
