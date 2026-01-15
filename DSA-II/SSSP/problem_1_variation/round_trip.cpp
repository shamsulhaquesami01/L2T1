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
    int used; // 0 = false, 1 = true

    bool operator>(const State& other) const {
        return cost > other.cost;
    }
};

// Helper function to run Dijkstra from a specific source 'src'
// Returns the dist table: dist[node][0] and dist[node][1]
vector<vector<long long>> runDijkstra(int n, int src, const vector<vector<Edge>>& adj) {
    // dist[node][used]
    vector<vector<long long>> dist(n + 1, vector<long long>(2, INF));
    priority_queue<State, vector<State>, greater<State>> pq;

    dist[src][0] = 0;
    pq.push({0, src, 0});

    while (!pq.empty()) {
        auto [d, u, used] = pq.top();
        pq.pop();

        if (d > dist[u][used]) continue;

        for (const auto& edge : adj[u]) {
            int v = edge.v;
            long long w = edge.w;

            // --- OPTION 1: Normal Travel ---
            if (d + w < dist[v][used]) {
                dist[v][used] = d + w;
                pq.push({dist[v][used], v, used});
            }

            // --- OPTION 2: Use Coupon (if not used yet) ---
            if (used == 0) {
                long long newCost = d + (w / 2);
                if (newCost < dist[v][1]) {
                    dist[v][1] = newCost;
                    pq.push({dist[v][1], v, 1});
                }
            }
        }
    }
    return dist;
}

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

    // 1. Run Dijkstra for Outbound Trip (1 -> N)
    // We pass the adjacency list as-is because we are following directed edges
    vector<vector<long long>> distFromStart = runDijkstra(n, 1, adj);

    // 2. Run Dijkstra for Return Trip (N -> 1)
    // We still use the same adjacency list because the problem implies standard directed flights
    // (If the return trip uses REVERSED edges, we would need to build a reverse graph. 
    // But usually "Round Trip" means flying N->1 using valid N->...->1 flights.)
    vector<vector<long long>> distFromEnd = runDijkstra(n, n, adj);

    // 3. Retrieve Costs
    long long there_normal = distFromStart[n][0];
    long long there_discount = distFromStart[n][1];
    
    long long back_normal = distFromEnd[1][0];
    long long back_discount = distFromEnd[1][1];

    // Check reachability
    bool there_possible = (there_normal != INF); // If normal is possible, discount is also implicitly possible/derived
    bool back_possible = (back_normal != INF);

    if (!there_possible || !back_possible) {
        cout << -1 << endl;
        return;
    }

    // 4. Calculate Minimum Total
    // Scenario A: Discount on the way there, Normal on the way back
    long long ans1 = there_discount + back_normal;

    // Scenario B: Normal on the way there, Discount on the way back
    long long ans2 = there_normal + back_discount;

    // Scenario C: (Optional check) Maybe using NO coupon is somehow better? 
    // (Mathematically impossible with positive weights and floor division, but safe to check)
    // long long ans3 = there_normal + back_normal;

    cout << min(ans1, ans2) << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}
