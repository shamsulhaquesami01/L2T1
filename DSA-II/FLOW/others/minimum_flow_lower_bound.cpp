#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
#include <algorithm>

using namespace std;

// --- YOUR STANDARD MAXFLOW TEMPLATE ---
struct Edge {
    int to;
    long long capacity;
    long long flow;
    int rev; // Index of the reverse edge
};

class MaxFlow {
public: // Made public so we can modify edges in Phase 2
    int N;
    vector<vector<Edge>> graph;

    MaxFlow(int n) : N(n), graph(n) {}

    // Returns the index of the added forward edge
    int addEdge(int u, int v, long long cap) {
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
        vector<pair<int, int>> parent(N);

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

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    // Nodes: 0 to N-1 (User Inputs)
    // Super Source (SS) = N
    // Super Sink (TT) = N + 1
    int SS = N;
    int TT = N + 1;
    
    MaxFlow mf(N + 2);
    vector<long long> balance(N, 0);

    // 1. Read Edges with Lower Bounds
    for (int i = 0; i < M; i++) {
        int u, v;
        long long l, c;
        cin >> u >> v >> l >> c; // u, v, lower_bound, capacity
        
        // Add edge with capacity (Capacity - LowerBound)
        mf.addEdge(u, v, c - l);
        
        // Update Demand/Supply balances
        balance[u] -= l;
        balance[v] += l;
    }

    int s, t;
    cin >> s >> t; // Original Source and Sink

    // 2. Add Infinite Edge from t -> s to create a Circulation
    // We store the index to access it later
    int t_to_s_idx = mf.addEdge(t, s, 1e18); // Infinite capacity

    // 3. Connect Super-Source and Super-Sink based on Balance
    long long total_supply_needed = 0;

    for (int i = 0; i < N; i++) {
        if (balance[i] > 0) {
            // Need flow: SS -> i
            mf.addEdge(SS, i, balance[i]);
            total_supply_needed += balance[i];
        } else if (balance[i] < 0) {
            // Excess flow: i -> TT
            mf.addEdge(i, TT, -balance[i]);
        }
    }

    // 4. Phase 1: Check Feasibility (SS -> TT)
    long long flow_phase1 = mf.edmondsKarp(SS, TT);

    if (flow_phase1 != total_supply_needed) {
        cout << "Impossible" << endl;
        return 0;
    }

    // The current valid flow from s to t is the flow passing through t->s
    long long current_flow = mf.graph[t][t_to_s_idx].flow;

    // 5. Phase 2: Minimization (Push flow back from t -> s)
    
    // A. Remove the t->s infinite edge (set capacity and flow to 0)
    mf.graph[t][t_to_s_idx].capacity = 0;
    mf.graph[t][t_to_s_idx].flow = 0;
    
    // Also remove the reverse edge s->t (ghost edge created by addEdge)
    int s_rev_idx = mf.graph[t][t_to_s_idx].rev;
    mf.graph[s][s_rev_idx].capacity = 0;
    mf.graph[s][s_rev_idx].flow = 0;

    // B. Run Max Flow from t to s
    // This pushes flow "backwards" to cancel out as much s->t flow as possible
    long long cancel_flow = mf.edmondsKarp(t, s);

    // 6. Result
    // Min Flow = Initial Valid Flow - Max Possible Cancellation
    cout << current_flow - cancel_flow << endl;

    return 0;
}