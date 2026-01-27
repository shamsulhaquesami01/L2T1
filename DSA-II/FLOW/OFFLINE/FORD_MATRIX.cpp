#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
#include <cstring>
#include <algorithm>

using namespace std;

const int MAXN = 505; // Adjust based on problem constraints
const long long INF = 1e18;

class MaxFlowMatrix {
    int N;
    long long capacity[MAXN][MAXN]; // Stores Residual Capacity
    vector<int> adj[MAXN];          // Adjacency list to keep BFS fast
    int parent[MAXN];
    int parent_edge_idx[MAXN];      // Not needed for matrix, but parent node is

public:
    MaxFlowMatrix(int n) : N(n) {
        memset(capacity, 0, sizeof(capacity));
        for(int i=0; i<n; i++) adj[i].clear();
    }

    void addEdge(int u, int v, long long cap) {
        // Add forward and backward edges to adjacency list for traversal
        adj[u].push_back(v);
        adj[v].push_back(u); 
        
        // Update capacity matrix
        capacity[u][v] += cap; 
        // capacity[v][u] remains 0 initially (unless bidirectional)
    }

    bool bfs(int s, int t) {
        memset(parent, -1, sizeof(parent));
        queue<int> q;
        q.push(s);
        parent[s] = -2;

        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                // If not visited AND has residual capacity
                if (parent[v] == -1 && capacity[u][v] > 0) {
                    parent[v] = u;
                    if (v == t) return true;
                    q.push(v);
                }
            }
        }
        return false;
    }

    long long edmondsKarp(int s, int t) {
        long long maxFlow = 0;

        while (bfs(s, t)) {
            long long pathFlow = INF;

            // 1. Find bottleneck
            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                pathFlow = min(pathFlow, capacity[u][v]);
            }

            // 2. Update residual capacities
            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                capacity[u][v] -= pathFlow; // Reduce forward
                capacity[v][u] += pathFlow; // Increase backward
            }

            maxFlow += pathFlow;
        }
        return maxFlow;
    }
};