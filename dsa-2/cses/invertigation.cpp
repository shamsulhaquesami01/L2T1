#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

// Infinity constant larger than any possible path cost
const long long INF = 1e18;
const int MOD = 1e9 + 7;

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    // Adjacency list: adj[u] stores pairs {v, weight}
    vector<vector<pair<int, int>>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
    }

    // Arrays to sto re the 4 required values
    vector<long long> dist(n + 1, INF);
    vector<long long> count(n + 1, 0);
    vector<int> min_flights(n + 1, 0);
    vector<int> max_flights(n + 1, 0);

    // Dijkstra's Priority Queue (Min-Heap): stores {distance, vertex}
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;

    // Initialization for the start node (Syrjälä = 1)
    dist[1] = 0;
    count[1] = 1; // One way to be at start: do nothing
    min_flights[1] = 0;
    max_flights[1] = 0;
    pq.push({0, 1});

    while (!pq.empty()) {
        long long d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        // If we found a shorter way to u previously, skip this stale entry
        if (d > dist[u]) continue;

        for (auto& edge : adj[u]) {
            int v = edge.first;
            int w = edge.second;

            // Case 1: Found a strictly shorter path
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                
                // Reset stats for v based on u
                count[v] = count[u];
                min_flights[v] = min_flights[u] + 1;
                max_flights[v] = max_flights[u] + 1;
                
                pq.push({dist[v], v});
            } 
            // Case 2: Found another path with the same shortest distance
            else if (dist[u] + w == dist[v]) {
                // Accumulate count
                count[v] = (count[v] + count[u]) % MOD;
                // Update min/max edges
                min_flights[v] = min(min_flights[v], min_flights[u] + 1);
                max_flights[v] = max(max_flights[v], max_flights[u] + 1);
            }
        }
    }

    // Output the results for the destination node (Lehmälä = n)
    cout << dist[n] << " " 
         << count[n] << " " 
         << min_flights[n] << " " 
         << max_flights[n] << endl;

    return 0;
}