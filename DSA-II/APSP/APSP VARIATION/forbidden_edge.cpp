#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const long long INF = 1e18;

void solve() {
    int n, m;
    if (!(cin >> n >> m)) return;

    // 1. Initialize Distance Matrix
    vector<vector<long long>> dist(n + 1, vector<long long>(n + 1, INF));
    
    // Distance to self is always 0
    for (int i = 1; i <= n; i++) dist[i][i] = 0;

    // 2. Read the Forbidden Edge Specification
    // (Assume inputs are 1-based indices)
    int badU, badV;
    cin >> badU >> badV; 

    // 3. Read Edges and Build Graph
    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;

        // --- THE LOGIC ---
        // If this edge is the forbidden one, we pretend it doesn't exist.
        if (u == badU && v == badV) {
            continue; 
        }

        // Otherwise, add it normally
        // (Handle multiple edges by keeping the minimum)
        if (dist[u][v] > w) {
            dist[u][v] = w;
        }
    }

    // 4. Standard Floyd-Warshall
    // No changes needed here! The algorithm just works on the "broken" graph.
    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }

    // 5. Output Queries
    int q;
    cin >> q;
    cout << "Shortest paths avoiding edge " << badU << "->" << badV << ":" << endl;
    
    while (q--) {
        int s, t;
        cin >> s >> t;

        if (dist[s][t] == INF) {
            cout << -1 << endl;
        } else {
            cout << dist[s][t] << endl;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}