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
    int cost; // NEW: Cost per unit flow
    int rev;
};

class MinCostMaxFlow {
public:
    int N;
    vector<vector<Edge>> graph;
    vector<long long> dist;     // Stores min cost to reach node
    vector<int> parent_node;    // To reconstruct path
    vector<int> parent_edge_idx;

    MinCostMaxFlow(int n) : N(n), graph(n), dist(n), parent_node(n), parent_edge_idx(n) {}

    void addEdge(int u, int v, int cap, int cost) {
        // Forward: Cost is +cost
        Edge a = {v, cap, 0, cost, (int)graph[v].size()};
        // Backward: Cost is -cost (refund)
        Edge b = {u, 0, 0, -cost, (int)graph[u].size()}; 
        graph[u].push_back(a);
        graph[v].push_back(b);
    }
    bool spfa(int s, int t) {
        dist.assign(N, 1e18); // Init with INF
        parent_node.assign(N, -1);
        parent_edge_idx.assign(N, -1);
        vector<bool> inQueue(N, false);
        queue<int> q;

        dist[s] = 0;
        q.push(s);
        inQueue[s] = true;

        while (!q.empty()) {
            int u = q.front(); q.pop();
            inQueue[u] = false;

            for (int i = 0; i < graph[u].size(); i++) {
                Edge &e = graph[u][i];
                // Relax edge if Residual Capacity > 0 and Cost is better
                if (e.capacity > e.flow && dist[e.to] > dist[u] + e.cost) {
                    dist[e.to] = dist[u] + e.cost;
                    parent_node[e.to] = u;
                    parent_edge_idx[e.to] = i;
                    
                    if (!inQueue[e.to]) {
                        q.push(e.to);
                        inQueue[e.to] = true;
                    }
                }
            }
        }
        return dist[t] != 1e18;
    }

    pair<int, long long> solve(int s, int t) {
        int maxFlow = 0;
        long long minCost = 0;

        while (spfa(s, t)) {
            int pathFlow = INT_MAX;
            
            // 1. Find bottleneck flow along the SPFA path
            for (int v = t; v != s; v = parent_node[v]) {
                int u = parent_node[v];
                int idx = parent_edge_idx[v];
                pathFlow = min(pathFlow, graph[u][idx].capacity - graph[u][idx].flow);
            }

            // 2. Update flow and cost
            for (int v = t; v != s; v = parent_node[v]) {
                int u = parent_node[v];
                int idx = parent_edge_idx[v];
                
                graph[u][idx].flow += pathFlow;
                int rev_idx = graph[u][idx].rev;
                graph[v][rev_idx].flow -= pathFlow;
            }
            
            maxFlow += pathFlow;
            minCost += (long long)pathFlow * dist[t];
        }
        return {maxFlow, minCost};
    }
};

// Driver for CSES Task Assignment
int main() {
    int n;
    cin >> n;

    int S = 0;
    int T = 2 * n + 1;
    MinCostMaxFlow mcmf(T + 1);

    // 1. Source -> Workers (1..N)
    for (int i = 1; i <= n; i++) {
        mcmf.addEdge(S, i, 1, 0); // Cap 1, Cost 0
    }

    // 2. Workers -> Tasks (N+1 .. 2N)
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            int cost;
            cin >> cost;
            // Edge from Worker i to Task j
            mcmf.addEdge(i, n + j, 1, cost);
        }
    }

    // 3. Tasks -> Sink
    for (int j = 1; j <= n; j++) {
        mcmf.addEdge(n + j, T, 1, 0); // Cap 1, Cost 0
    }

    auto result = mcmf.solve(S, T);
    cout << result.second << "\n"; // Print Min Cost

    // To print the assignment:
    for (int i = 1; i <= n; i++) {
        for (const auto &e : mcmf.graph[i]) {
            // If flow is 1 and it goes to a Task node
            if (e.flow == 1 && e.to > n && e.to <= 2 * n) {
                cout << i << " " << (e.to - n) << "\n";
            }
        }
    }

    return 0;
}