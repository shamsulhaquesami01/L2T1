#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

const long long INF = 1e18;
// Use a sufficiently small number for negative infinity to prevent overflow when adding
const long long NEG_INF = -1e18; 

struct Edge {
    int u, v;
    long long w;
};

void solve() {
    int n, m;
    if (!(cin >> n >> m)) return;

    vector<Edge> edges;
    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        edges.push_back({u, v, w});
    }

    vector<long long> dist(n + 1, INF);
    vector<int> parent(n + 1, -1);
    dist[1] = 0;

    // --- PHASE 1: Standard Bellman-Ford (N-1 iterations) ---
    // Finds shortest paths assuming no negative cycles
    for (int i = 0; i < n - 1; i++) {
        for (const auto& edge : edges) {
            if (dist[edge.u] != INF && dist[edge.u] + edge.w < dist[edge.v]) {
                dist[edge.v] = dist[edge.u] + edge.w;
                parent[edge.v] = edge.u;
            }
        }
    }

    // --- PHASE 2: Propagate Negative Cycles (N-1 iterations) ---
    // Instead of BFS, we just run the relaxation loop again.
    // This propagates the "-Infinity" status to all nodes reachable from a cycle.
    for (int i = 0; i < n - 1; i++) {
        for (const auto& edge : edges) {
            if (dist[edge.u] != INF) {
                // If the source is already marked as part of a negative loop (-INF),
                // the destination must also be marked -INF.
                if (dist[edge.u] == NEG_INF) {
                    dist[edge.v] = NEG_INF;
                }
                // Standard relaxation check: if we can still improve, it's a negative cycle
                else if (dist[edge.u] + edge.w < dist[edge.v]) {
                    dist[edge.v] = NEG_INF; 
                    // We don't update parent here because the path is invalid
                }
            }
        }
    }

    // --- PHASE 3: Output ---
    if (dist[n] == NEG_INF) {
        cout << -1 << endl;
    } 
    else if (dist[n] == INF) {
        cout << "No Path" << endl;
    } 
    else {
        cout << "Cost: " << dist[n] << endl;
        
        // Path Reconstruction (Only valid if dist[n] is not NEG_INF)
        vector<int> path;
        int curr = n;
        while (curr != -1) {
            path.push_back(curr);
            curr = parent[curr];
        }
        reverse(path.begin(), path.end());

        cout << "Path: ";
        for (size_t i = 0; i < path.size(); i++) {
            cout << path[i] << (i == path.size() - 1 ? "" : " -> ");
        }
        cout << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}
