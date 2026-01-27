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

    // SPFA Algorithm (Replaces BFS) to find path with Min Cost
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
                
                // Cost calculation: Flow * Cost
                // Note: We use the cost of the edge we traversed
                // Since SPFA minimizes total cost, we add (pathFlow * unitCost)
                // However, simpler is just adding pathFlow * dist[t] 
                // because dist[t] is the cost per unit flow along this path.
            }
            
            maxFlow += pathFlow;
            minCost += (long long)pathFlow * dist[t];
        }
        return {maxFlow, minCost};
    }
};