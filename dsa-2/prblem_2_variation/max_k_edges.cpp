#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

using namespace std;

const long long INF = 1e18;

// Edge structure
struct Edge {
    int u, v;
    long long w;
};

void solve() {
    int n, m, k;
    if (!(cin >> n >> m >> k)) return; // k is max edges allowed

    vector<Edge> edges;
    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        edges.push_back({u, v, w});
    }

    // dist[i] stores shortest distance to i using *at most* current iteration's edges
    vector<long long> dist(n + 1, INF);
    dist[1] = 0;

    // Run exactly K times
    for (int i = 1; i <= k; i++) {
        vector<long long> temp_dist = dist; // Copy previous state
        
        for (const auto& edge : edges) {
            // Only relax if we reached 'u' in the previous step (i-1 edges)
            if (dist[edge.u] != INF) {
                if (dist[edge.u] + edge.w < temp_dist[edge.v]) {
                    temp_dist[edge.v] = dist[edge.u] + edge.w;
                }
            }
        }
        // Update the main distance array for the next iteration
        dist = temp_dist;
    }

    if (dist[n] == INF) {
        cout << -1 << endl;
    } else {
        cout << dist[n] << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}