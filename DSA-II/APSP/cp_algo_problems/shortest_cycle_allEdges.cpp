//undirected graph
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <numeric>

using namespace std;

const int INF = 1e9;

// Memoization table for DP
// Max N is 15, so max odd vertices is 14 (must be even). 2^14 is safe.
int memo[1 << 16];
vector<int> odds;
int dist[20][20];

// Recursive DP function to find Min Weight Perfect Matching
int solve_matching(int mask) {
    // Base case: If mask is 0, all vertices are matched, cost is 0
    if (mask == 0) return 0;
    
    // Check memoization
    if (memo[mask] != -1) return memo[mask];

    int res = INF;

    // Find the first bit set in the mask (first available odd-degree vertex)
    // __builtin_ctz returns the number of trailing zeros (index of first 1)
    int i = __builtin_ctz(mask);
    
    // We must match odds[i] with some other odds[j]
    // Iterate through all other bits j > i
    for (int j = i + 1; j < odds.size(); j++) {
        if (mask & (1 << j)) {
            // Try matching i and j
            // Cost is distance between them + result of matching the REST
            int new_mask = mask ^ (1 << i) ^ (1 << j);
            res = min(res, dist[odds[i]][odds[j]] + solve_matching(new_mask));
        }
    }

    return memo[mask] = res;
}

void solve() {
    int n, m;
    if (!(cin >> n >> m)) return;

    // 1. Initialization
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (i == j) dist[i][j] = 0;
            else dist[i][j] = INF;
        }
    }

    vector<int> degree(n + 1, 0);
    int total_weight = 0;

    // 2. Read Edges
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        total_weight += w;
        degree[u]++;
        degree[v]++;
        
        // Handle multiple edges: keep the smallest weight for travel purposes
        dist[u][v] = min(dist[u][v], w);
        dist[v][u] = min(dist[v][u], w);
    }

    // 3. Floyd-Warshall (N <= 15, O(N^3))
    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }

    // 4. Check Connectivity
    // We must start at 1. If there is any vertex with edges (degree > 0)
    // that cannot be reached from 1, the task is impossible.
    for (int i = 1; i <= n; i++) {
        if (degree[i] > 0 && dist[1][i] == INF) {
            cout << -1 << endl;
            return;
        }
    }
    
    // Edge Case: If M > 0 but node 1 has degree 0 (and somehow satisfied the check above?),
    // it implies the edges form a component isolated from 1. 
    // The previous loop handles this, but let's be explicit if needed.
    // If M=0, answer is 0.
    if (m == 0) {
        cout << 0 << endl;
        return;
    }

    // 5. Identify Odd Degree Vertices
    odds.clear();
    for (int i = 1; i <= n; i++) {
        if (degree[i] % 2 != 0) {
            odds.push_back(i);
        }
    }

    // 6. Bitmask DP for Matching
    // Initialize memo table with -1
    // Size of mask is 2^(number of odd vertices)
    int num_odds = odds.size();
    fill(memo, memo + (1 << num_odds), -1);

    int extra_cost = solve_matching((1 << num_odds) - 1);

    if (extra_cost >= INF) {
        // Should theoretically not happen if connectivity check passes
        cout << -1 << endl;
    } else {
        cout << total_weight + extra_cost << endl;
    }
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}