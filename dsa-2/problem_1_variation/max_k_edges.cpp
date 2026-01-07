#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

const long long INF = 1e18;

struct Edge {
    int v;
    long long w;
};

struct State {
    long long cost;
    int u;
    int used;  // 0 or 1
    int steps; // Edges used so far

    bool operator>(const State& other) const {
        return cost > other.cost;
    }
};

void solve() {
    int n, m, k;
    if (!(cin >> n >> m >> k)) return; // k is max edges allowed

    vector<vector<Edge>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v; 
        long long w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
    }

    // dist[node][used][steps]
    // Dimensions: (N+1) x 2 x (K+1)
    vector<vector<vector<long long>>> dist(n + 1, vector<vector<long long>>(2, vector<long long>(k + 1, INF)));
    
    priority_queue<State, vector<State>, greater<State>> pq;

    // Start: Node 1, Cost 0, Coupon 0, Steps 0
    dist[1][0][0] = 0;
    pq.push({0, 1, 0, 0});

    while (!pq.empty()) {
        auto [d, u, used, steps] = pq.top();
        pq.pop();

        if (d > dist[u][used][steps]) continue;
        if (steps >= k) continue; // Cannot take more edges

        for (const auto& edge : adj[u]) {
            int v = edge.v;
            long long w = edge.w;
            int next_steps = steps + 1;

            // --- OPTION 1: Normal Travel ---
            if (d + w < dist[v][used][next_steps]) {
                dist[v][used][next_steps] = d + w;
                pq.push({dist[v][used][next_steps], v, used, next_steps});
            }

            // --- OPTION 2: Use Coupon ---
            if (used == 0) {
                long long newCost = d + (w / 2);
                if (newCost < dist[v][1][next_steps]) {
                    dist[v][1][next_steps] = newCost;
                    pq.push({dist[v][1][next_steps], v, 1, next_steps});
                }
            }
        }
    }

    // Answer: Min cost to reach N using ANY number of steps <= K
    long long ans = INF;
    for (int s = 0; s <= k; s++) {
        ans = min(ans, min(dist[n][0][s], dist[n][1][s]));
    }

    if (ans == INF) cout << -1 << endl;
    else cout << ans << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}