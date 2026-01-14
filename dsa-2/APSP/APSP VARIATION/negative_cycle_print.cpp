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

    // Distance to self is 0
    for (int i = 1; i <= n; i++) {
        dist[i][i] = 0;
        next_node[i][i] = i; // Point to self initially
    }

    // 2. Read Edges
    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        // Keep minimum edge weight
        if (w < dist[u][v]) {
            dist[u][v] = w;
            next_node[u][v] = v; // To go u->v, next step is v
        }
    }

    // 3. Floyd-Warshall with Path Tracking
    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                        // KEY STEP: Update the path pointer
                        // To go i->j via k, the next step is the same as going i->k
                        next_node[i][j] = next_node[i][k];
                    }
                }
            }
        }
    }

    // 4. Check for Negative Cycles
    for (int i = 1; i <= n; i++) {
        if (dist[i][i] < 0) {
            cout << "Negative Cycle Found!" << endl;
            
            // Reconstruct the cycle starting from i
            vector<int> path;
            int curr = i;
            
            // We follow next_node[curr][i] to get back to i
            // We use a visited array to detect the exact loop if the path has a "stem"
            vector<bool> visited(n + 1, false);
            
            while (true) {
                path.push_back(curr);
                
                // If we visit a node twice in this trace, we found the loop closure
                if (visited[curr] && curr == i) break; 
                // Note: Standard FW usually ensures i is IN the loop if dist[i][i] < 0,
                // but just to be safe from infinite loops:
                if (path.size() > n + 1) break; 

                visited[curr] = true;
                curr = next_node[curr][i];
                
                // Safety: if path breaks
                if (curr == -1) {
                    cout << "Error: Broken path logic." << endl;
                    return;
                }
            }

            // Print Cycle
            cout << "Cycle Path: ";
            for (size_t k = 0; k < path.size(); k++) {
                cout << path[k] << (k == path.size() - 1 ? "" : " -> ");
            }
            cout << endl;
            cout << "Total Cycle Weight: " << dist[i][i] << endl;
            return; // Found one, exit
        }
    }

    cout << "No Negative Cycle Detected." << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}