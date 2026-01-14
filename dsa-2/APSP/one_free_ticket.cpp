#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
const long long INF = 1e18;

void solve() {
    int n, m; cin >> n >> m;
    // We create a graph of size 2*N + 1
    // Indices 1..n are State 0 (Ticket unused)
    // Indices n+1..2n are State 1 (Ticket used)
    int size = 2 * n + 1;
    vector<vector<long long>> dist(size, vector<long long>(size, INF));

    for (int i = 1; i < size; i++) dist[i][i] = 0;

    for (int i = 0; i < m; i++) {
        int u, v; long long w;
        cin >> u >> v >> w; // 1-based input assumed

        // 1. Normal path in State 0
        dist[u][v] = min(dist[u][v], w);
        // 2. Normal path in State 1
        dist[u + n][v + n] = min(dist[u + n][v + n], w);
        // 3. USING THE TICKET (Transition 0 -> 1) with weight 0
        dist[u][v + n] = min(dist[u][v + n], 0LL); 
    }

    // Run FW on the 2N x 2N matrix
    for (int k = 1; k < size; k++) {
        for (int i = 1; i < size; i++) {
            for (int j = 1; j < size; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF)
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }

    int q; cin >> q;
    while(q--) {
        int s, t; cin >> s >> t;
        // Result is min of:
        // 1. Path without using ticket (s -> t)
        // 2. Path using ticket (s -> t + n)
        long long ans = min(dist[s][t], dist[s][t + n]);
        cout << (ans == INF ? -1 : ans) << endl;
    }
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}