#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const long long INF = 1e18; // Use a large number for infinity

void solve() {
    int n, m;
    if (!(cin >> n >> m)) return;

    // 1. Initialize Matrices
    // dist: Standard weights
    // dist_disc: Discounted weights (w - 1)
    vector<vector<long long>> dist(n, vector<long long>(n, INF));
    vector<vector<long long>> dist_disc(n, vector<long long>(n, INF));

    // Distance to self is 0
    for (int i = 0; i < n; i++) {
        dist[i][i] = 0;
        dist_disc[i][i] = 0;
    }

    // 2. Read Edges
    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        
        // Store the minimum edge weight if multiple edges exist
        if (w < dist[u][v]) {
            dist[u][v] = w;
        }
        // Discounted weight is w - 1
        // Constraint says w >= 1, so w - 1 >= 0. No negative edges.
        if ((w - 1) < dist_disc[u][v]) {
            dist_disc[u][v] = w - 1;
        }
    }

    int V; 
    cin >> V; // Read the Discount City V

    // 3. Run Floyd-Warshall on BOTH matrices
    
    // Pass 1: Standard APSP
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }

    // Pass 2: Discounted APSP
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist_disc[i][k] != INF && dist_disc[k][j] != INF) {
                    dist_disc[i][j] = min(dist_disc[i][j], dist_disc[i][k] + dist_disc[k][j]);
                }
            }
        }
    }

    // 4. Process Queries
    int q;
    cin >> q;
    while (q--) {
        int s, t;
        cin >> s >> t;

        // Calculate cost if we bypass V (Standard path)
        long long standard_cost = dist[s][t];

        // Calculate cost if we go through V (Discounted path)
        // Path: s -> V -> t (using discounted weights for the whole chain)
        long long discount_cost = INF;
        if (dist_disc[s][V] != INF && dist_disc[V][t] != INF) {
            discount_cost = dist_disc[s][V] + dist_disc[V][t];
        }

        long long ans = min(standard_cost, discount_cost);

        if (ans == INF) cout << -1 << endl;
        else cout << ans << endl;
    }
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}