#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#include <climits>

using namespace std;

const long long INF = 1e18;

// Edge structure
struct Edge {
    int v;
    long long w;
};

// State structure for Priority Queue
struct State {
    long long cost;
    int u;
    int used50; // 0 = false, 1 = true (50% off coupon)
    int used25; // 0 = false, 1 = true (25% off coupon)

    // Operator overloading for Min-Priority Queue
    bool operator>(const State& other) const {
        return cost > other.cost;
    }
};

void solve() {
    int n, m;
    if (!(cin >> n >> m)) return;

    vector<vector<Edge>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
    }

    // dist[node][used50][used25]
    // Dimensions: (N+1) x 2 x 2
    vector<vector<vector<long long>>> dist(n + 1, vector<vector<long long>>(2, vector<long long>(2, INF)));

    // Min-Priority Queue
    priority_queue<State, vector<State>, greater<State>> pq;

    // Initial state: Start at 1, Cost 0, No coupons used (0, 0)
    dist[1][0][0] = 0;
    pq.push({0, 1, 0, 0});

    while (!pq.empty()) {
        auto [d, u, u50, u25] = pq.top();
        pq.pop();

        // Skip stale states if we found a better path already
        if (d > dist[u][u50][u25]) continue;

        for (const auto& edge : adj[u]) {
            int v = edge.v;
            long long w = edge.w;

            // --- OPTION 1: Normal Travel (Don't use any new coupon) ---
            if (d + w < dist[v][u50][u25]) {
                dist[v][u50][u25] = d + w;
                pq.push({dist[v][u50][u25], v, u50, u25});
            }

            // --- OPTION 2: Use 50% Coupon (if not used yet) ---
            if (u50 == 0) {
                long long newCost = d + (w / 2);
                if (newCost < dist[v][1][u25]) {
                    dist[v][1][u25] = newCost;
                    pq.push({dist[v][1][u25], v, 1, u25});
                }
            }

            // --- OPTION 3: Use 25% Coupon (if not used yet) ---
            if (u25 == 0) {
                // 25% off means you pay 75% of the price -> (w * 3) / 4
                long long newCost = d + (w * 3) / 4;
                if (newCost < dist[v][u50][1]) {
                    dist[v][u50][1] = newCost;
                    pq.push({dist[v][u50][1], v, u50, 1});
                }
            }
        }
    }

    // Answer is the minimum cost to reach Node N in ANY state
    long long ans = INF;
    for (int i = 0; i <= 1; i++) {
        for (int j = 0; j <= 1; j++) {
            ans = min(ans, dist[n][i][j]);
        }
    }

    cout << ans << endl;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}