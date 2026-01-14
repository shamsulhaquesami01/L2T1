/*Variation 1: Toll-Free Highway
A delivery network has a special toll-free highway at city H.
 Any path that uses city H gets the maximum edge weight in 
 that path reduced to 0 (simulating a toll-free fast lane).
  Calculate shortest paths for all pairs.*/

#include <iostream>
#include <vector>
#include <queue>
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
    
    ll n, m;
    cin >> n >> m;
    
    vector<vector<ll>> dist(n + 1, vector<ll>(n + 1, INF));
    vector<vector<ll>> maxEdge(n + 1, vector<ll>(n + 1, 0)); // track max edge
    
    for (int i = 1; i <= n; i++)
        dist[i][i] = 0;
    
    for (int i = 0; i < m; i++) {
        int a, b;
        ll w;
        cin >> a >> b >> w;
        if (w < dist[a][b]) {
            dist[a][b] = w;
            maxEdge[a][b] = w;
        }
    }
    
    int H;
    cin >> H;
    
    // Standard APSP
    solve(dist, n);
    
    // Store original distances
    vector<vector<ll>> original = dist;
    
    // Compute max edge on each path through normal APSP
    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    if (dist[i][k] + dist[k][j] == dist[i][j]) {
                        maxEdge[i][j] = max(maxEdge[i][j], max(maxEdge[i][k], maxEdge[k][j]));
                    }
                }
            }
        }
    }
    
    // Now compute paths through H with max edge removed
    vector<vector<ll>> result(n + 1, vector<ll>(n + 1, INF));
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            result[i][j] = original[i][j]; // without using H
            
            // Try path through H
            if (original[i][H] != INF && original[H][j] != INF) {
                ll pathCost = original[i][H] + original[H][j];
                ll maxEdgeOnPath = max(maxEdge[i][H], maxEdge[H][j]);
                ll discountedCost = pathCost - maxEdgeOnPath;
                result[i][j] = min(result[i][j], discountedCost);
            }
        }
    }
    
    int q;
    cin >> q;
    for (int i = 0; i < q; i++) {
        int a, b;
        cin >> a >> b;
        if (result[a][b] != INF)
            cout << result[a][b] << endl;
        else
            cout << -1 << endl;
    }
}