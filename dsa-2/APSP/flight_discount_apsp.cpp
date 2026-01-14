#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip> // For setprecision

using namespace std;

const double INF = 1e18;

void solve() {
    int n, m;
    if (!(cin >> n >> m)) return;

    // We use a "Flattened" 2*N graph
    // Nodes 1..N:     State 0 (Coupon Available)
    // Nodes N+1..2N:  State 1 (Coupon Used)
    int size = 2 * n + 1;
    vector<vector<double>> dist(size, vector<double>(size, INF));

    // Initialize diagonals
    for (int i = 1; i < size; i++) dist[i][i] = 0;

    for (int i = 0; i < m; i++) {
        int u, v;
        double w;
        cin >> u >> v >> w;

        // 1. Normal Path (Keep Coupon): u -> v (Layer 0)
        dist[u][v] = min(dist[u][v], w);

        // 2. Normal Path (Already Used Coupon): u+n -> v+n (Layer 1)
        dist[u + n][v + n] = min(dist[u + n][v + n], w);

        // 3. USE COUPON (Transition): u -> v+n (Cross Layers)
        // This is the specific edge where we pay half price
        dist[u][v + n] = min(dist[u][v + n], w / 2.0);
    }

    // Standard Floyd-Warshall on the 2N graph
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

        // The answer is the minimum of:
        // 1. Not using the coupon at all: dist[s][t]
        // 2. Using the coupon somewhere: dist[s][t + n]
        //    (This path starts at 's' in Layer 0 and ends at 't' in Layer 1)
        
        double ans = min(dist[s][t], dist[s][t + n]);

        if (ans == INF) cout << -1 << endl;
        else cout << fixed << setprecision(2) << ans << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}