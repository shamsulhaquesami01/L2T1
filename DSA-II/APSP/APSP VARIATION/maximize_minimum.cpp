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
                // Check if paths exist (using -1 as "no path")
                if (dist[i][k] != -1 && dist[k][j] != -1) {
                    
                    // 1. Find the bottleneck (minimum) of this specific path
                    ll path_min = min(dist[i][k], dist[k][j]);
                    
                    // 2. Maximize that bottleneck (we want the "widest" path)
                    dist[i][j] = max(dist[i][j], path_min);
                }
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n, m;
    cin >> n >> m;

    // CHANGE 1: Initialize with -1 (or 0) because we want to MAXIMIZE
    vector<vector<ll>> dist(n + 1, vector<ll>(n + 1, -1));

    // CHANGE 2: Self-distance is Infinite (Infinite capacity to reach yourself)
    for (int i = 1; i <= n; i++)
        dist[i][i] = INF;

    for (int i = 0; i < m; i++) {
        int a, b;
        ll w;
        cin >> a >> b >> w;
        // CHANGE 3: Keep the LARGEST edge if multiple exist
        dist[a][b] = max(dist[a][b], w);
    }

    solve(dist, n);

    int q; 
    cin >> q;
    for (int i = 0; i < q; i++) {
        int a, b;
        cin >> a >> b;
        
        // If dist is still -1, no path exists
        if (dist[a][b] != -1)
            cout << dist[a][b] << endl;
        else
            cout << -1 << endl; // Or however you want to denote "no path"
    }
    return 0;
}