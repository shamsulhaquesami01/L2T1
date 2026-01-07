#include <iostream>
#include <vector>
#include <queue>
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
    int used;           // 0, 1, or 2
    int last_was_coupon; // 0 = false, 1 = true (Restriction flag)

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

    // dist[node][used_count][last_was_coupon]
    // Dimensions: (N+1) x 3 x 2
    vector<vector<vector<long long>>> dist(n + 1, vector<vector<long long>>(3, vector<long long>(2, INF)));
    
    // Min-Priority Queue
    priority_queue<State, vector<State>, greater<State>> pq;

    // Initial State: Node 1, Cost 0, Used 0, Last Was Normal (0)
    dist[1][0][0] = 0;
    pq.push({0, 1, 0, 0});

    while (!pq.empty()) {
        auto [d, u, used, lastWasCoupon] = pq.top();
        pq.pop();

        // Skip stale states
        if (d > dist[u][used][lastWasCoupon]) continue;

        for (const auto& edge : adj[u]) {
            int v = edge.v;
            long long w = edge.w;

            // --- OPTION 1: Normal Flight ---
            // Always allowed. Resets the "last_was_coupon" flag to 0.
            if (d + w < dist[v][used][0]) {
                dist[v][used][0] = d + w;
                pq.push({dist[v][used][0], v, used, 0});
            }

            // --- OPTION 2: Use Coupon ---
            // Allowed ONLY if:
            // 1. We have coupons left (used < 2)
            // 2. We did NOT use a coupon on the immediate previous step (lastWasCoupon == 0)
            if (used < 2 && lastWasCoupon == 0) {
                long long newCost = d + (w / 2);
                if (newCost < dist[v][used + 1][1]) {
                    dist[v][used + 1][1] = newCost;
                    pq.push({dist[v][used + 1][1], v, used + 1, 1});
                }
            }
        }
    }

    // Answer is min cost to reach N with any valid state
    long long ans = INF;
    for (int i = 0; i <= 2; i++) {       // Check used 0, 1, or 2
        for (int j = 0; j <= 1; j++) {   // Check ending on normal or coupon
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