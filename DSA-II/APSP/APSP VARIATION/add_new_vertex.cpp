#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;
using ll = long long;
const ll INF = 1e18;

void solve(vector<vector<ll>>& dist, int n) {
    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    // Read existing shortest path matrix
    vector<vector<ll>> dist(n + 2, vector<ll>(n + 2, INF));
    
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            cin >> dist[i][j];
        }
    }
    
    // Read new city connections
    int num_new_roads;
    cin >> num_new_roads;
    
    int new_city = n + 1;
    vector<pair<int, ll>> new_edges;
    
    // Initialize new city
    for (int i = 1; i <= n; i++) {
        dist[new_city][i] = INF;
        dist[i][new_city] = INF;
    }
    dist[new_city][new_city] = 0;
    
    // Read direct edges to new city
    for (int i = 0; i < num_new_roads; i++) {
        int u;
        ll w;
        cin >> u >> w;
        new_edges.push_back({u, w});
        
        // Bidirectional edge (take min if multiple)
        dist[new_city][u] = min(dist[new_city][u], w);
        dist[u][new_city] = min(dist[u][new_city], w);
    }
    
    // PHASE 1: Calculate shortest paths to/from new city via its neighbors
    for (int i = 1; i <= n; i++) {
        for (auto [u, w] : new_edges) {
            // From new_city to i via neighbor u
            if (dist[u][i] != INF) {
                dist[new_city][i] = min(dist[new_city][i], w + dist[u][i]);
            }
            // From i to new_city via neighbor u
            if (dist[i][u] != INF) {
                dist[i][new_city] = min(dist[i][new_city], dist[i][u] + w);
            }
        }
    }
    
    // PHASE 2: Use new city as pivot to update all pairs
    ll total_sum = 0;
    for (int i = 1; i <= n + 1; i++) {
        for (int j = 1; j <= n + 1; j++) {
            // Relax using new city as intermediate
            if (dist[i][new_city] != INF && dist[new_city][j] != INF) {
                dist[i][j] = min(dist[i][j], dist[i][new_city] + dist[new_city][j]);
            }
            
            // Sum only upper triangle to avoid counting twice
            if (i < j && dist[i][j] != INF) {
                total_sum += dist[i][j];
            }
        }
    }
    
    cout << total_sum << endl;
    
    return 0;
}


// ### Input:
// ```
// 3
// 0 5 10
// 5 0 3
// 10 3 0
// 2
// 1 2
// 3 4
// ```

// ### Output:
// ```
// 34