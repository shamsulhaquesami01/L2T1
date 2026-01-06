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
    int used; // 0 = false, 1 = true (coupon used)

    // Operator overloading for Min-Priority Queue
    bool operator>(const State& other) const {
        return cost > other.cost;
    }
};

void solve() {
    int n, m;
    long long budget;
    // Reading inputs
    if (!(cin >> n >> m >> budget)) return;

    vector<vector<Edge>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
    }

    // dist[node][used]
    // Dimensions: (N+1) x 2
    vector<vector<long long>> dist(n + 1, vector<long long>(2, INF));

    // Min-Priority Queue
    priority_queue<State, vector<State>, greater<State>> pq;

    // Initial state: Start at 1, Cost 0, Coupon NOT used (0)
    dist[1][0] = 0;
    pq.push({0, 1, 0});

    while (!pq.empty()) {
        auto [d, u, used] = pq.top();
        pq.pop();

        // Skip stale states
        if (d > dist[u][used]) continue;
        
        // Skip if current cost already exceeds budget (Pruning)
        if (d > budget) continue;

        for (const auto& edge : adj[u]) {
            int v = edge.v;
            long long w = edge.w;

            // --- OPTION 1: Normal Travel (Don't use coupon here) ---
            long long newCost = d + w;
            if (newCost <= budget && newCost < dist[v][used]) {
                dist[v][used] = newCost;
                pq.push({dist[v][used], v, used});
            }

            // --- OPTION 2: Use Coupon (if not used yet) ---
            if (used == 0) {
                long long discountedW = w / 2;
                long long newCostDisc = d + discountedW;
                
                if (newCostDisc <= budget && newCostDisc < dist[v][1]) {
                    dist[v][1] = newCostDisc;
                    pq.push({dist[v][1], v, 1});
                }
            }
        }
    }

    // Answer is min cost to reach N
    long long ans = min(dist[n][0], dist[n][1]);

    if (ans == INF || ans > budget) {
        cout << "Not possible within budget" << endl;
    } else {
        cout << ans << endl;
    }
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}