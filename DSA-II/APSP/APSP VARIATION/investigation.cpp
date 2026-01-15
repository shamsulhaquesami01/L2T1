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
    vector<vector<long long>> paths(n + 1, vector<long long>(n + 1, 0));
    vector<vector<int>> min_edges(n + 1, vector<int>(n + 1, 1e9));
    vector<vector<int>> max_edges(n + 1, vector<int>(n + 1, 0));

    // Base Case: Distance to self
    for (int i = 1; i <= n; i++) {
        dist[i][i] = 0;
        paths[i][i] = 1;
        min_edges[i][i] = 0;
        max_edges[i][i] = 0;
    }

    // 2. Read Edges (Handle Multiple Edges carefully!)
    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;

        if (w < dist[u][v]) {
            // Found a strictly better direct edge
            dist[u][v] = w;
            paths[u][v] = 1;
            min_edges[u][v] = 1;
            max_edges[u][v] = 1;
        } 
        else if (w == dist[u][v]) {
            // Found another direct edge with the SAME cost
            paths[u][v]++;
            // min/max edges for a direct edge is always 1, so no change needed
        }
    }

    // 3. The "All-in-One" Floyd-Warshall
    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                // Check if a path exists via k
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    
                    long long newDist = dist[i][k] + dist[k][j];
                    long long newWays = paths[i][k] * paths[k][j];
                    int newMinE = min_edges[i][k] + min_edges[k][j];
                    int newMaxE = max_edges[i][k] + max_edges[k][j];

                    if (newDist < dist[i][j]) {
                        // CASE 1: Strictly Better Path Found
                        dist[i][j] = newDist;
                        paths[i][j] = newWays;
                        min_edges[i][j] = newMinE;
                        max_edges[i][j] = newMaxE;
                    } 
                    else if (newDist == dist[i][j]) {
                        // CASE 2: Equal Cost Path Found
                        paths[i][j] += newWays;
                        min_edges[i][j] = min(min_edges[i][j], newMinE);
                        max_edges[i][j] = max(max_edges[i][j], newMaxE);
                    }
                }
            }
        }
    }

    // 4. Output Query Results
    int q;
    cin >> q;
    while (q--) {
        int s, t;
        cin >> s >> t;

        if (dist[s][t] == INF) {
            cout << "No Path" << endl;
        } else {
            cout << "Shortest Cost: " << dist[s][t] << endl;
            cout << "Number of Shortest Paths: " << paths[s][t] << endl;
            cout << "Min Edges: " << min_edges[s][t] << endl;
            cout << "Max Edges: " << max_edges[s][t] << endl;
            cout << "-----------------" << endl;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}