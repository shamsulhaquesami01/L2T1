#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const long long INF = 1e18;

void solve() {
    int n, m;
    if (!(cin >> n >> m)) return;

    // We need 2*N nodes.
    // Indices 1 to N: Even Parity State
    // Indices N+1 to 2N: Odd Parity State
    int size = 2 * n + 1;
    vector<vector<long long>> dist(size, vector<long long>(size, INF));

    // Initialize diagonals
    // Distance from a node to itself with 0 steps (Even) is 0
    for (int i = 1; i < size; i++) {
        dist[i][i] = 0;
    }

    // Read Edges
    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;

        // An edge flips the parity state
        
        // 1. Even u -> Odd v
        // Map v to its odd counterpart: v + n
        dist[u][v + n] = min(dist[u][v + n], w);

        // 2. Odd u -> Even v
        // Map u to its odd counterpart: u + n
        dist[u + n][v] = min(dist[u + n][v], w);
    }

    // Run Standard Floyd-Warshall on the expanded 2N graph
    for (int k = 1; k < size; k++) {
        for (int i = 1; i < size; i++) {
            for (int j = 1; j < size; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }

    // Process Queries
    int q;
    cin >> q;
    while (q--) {
        int s, t;
        cin >> s >> t;

        // We want the shortest path from s to t with an EVEN number of edges.
        // This corresponds to starting at s (Even layer) and ending at t (Even layer).
        // (If we wanted ODD, we would query dist[s][t + n])
        
        long long ans = dist[s][t];

        if (ans == INF) cout << -1 << endl;
        else cout << ans << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}