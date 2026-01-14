#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const long long INF = 1e18;

void solve() {
    int n, m;
    if (!(cin >> n >> m)) return;

    // 1. Initialize Matrices
    vector<vector<long long>> dist(n + 1, vector<long long>(n + 1, INF));
    vector<vector<int>> next_node(n + 1, vector<int>(n + 1, -1));

    // Distance to self is 0, Next is self (or -1, distinct choice doesn't matter much for self)
    for (int i = 1; i <= n; i++) {
        dist[i][i] = 0;
        next_node[i][i] = i;
    }

    // 2. Read Edges
    for (int i = 0; i < m; i++) {
        int u, v; 
        long long w;
        cin >> u >> v >> w;
        // Handle multi-edges: keep the shortest one
        if (w < dist[u][v]) {
            dist[u][v] = w;
            next_node[u][v] = v; // To go u->v, go directly to v
        }
    }

    // 3. Floyd-Warshall with Path Tracking
    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                        // CRITICAL: The next step from i to j is the same as the next step from i to k
                        next_node[i][j] = next_node[i][k];
                    }
                }
            }
        }
    }

    // 4. Query Processing
    int q;
    cin >> q;
    while (q--) {
        int s, t;
        cin >> s >> t;

        if (dist[s][t] == INF) {
            cout << "No Path" << endl;
            continue;
        }

        // --- Path Reconstruction Logic ---
        vector<int> path;
        int curr = s;
        
        // Follow the breadcrumbs until we hit the destination
        while (curr != t) {
            path.push_back(curr);
            curr = next_node[curr][t];
            
            // Safety check for impossible paths (should be caught by INF check, but good practice)
            if (curr == -1) {
                path.clear();
                break;
            }
        }
        if (!path.empty() || s == t) path.push_back(t); // Add destination to path

        // --- Output ---
        cout << "Cost: " << dist[s][t] << endl;
        cout << "Path: ";
        for (size_t i = 0; i < path.size(); i++) {
            cout << path[i] << (i == path.size() - 1 ? "" : " -> ");
        }
        cout << endl;
        
        cout << "Nodes in path: " << path.size() << endl;
        // Edge count is always Nodes - 1 (unless it's a single node 0-cost path)
        cout << "Edges in path: " << (path.size() > 0 ? path.size() - 1 : 0) << endl;
        cout << "-----------------" << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}