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

    // Operator overloading for Min-Priority Queue
    bool operator>(const State& other) const {
        return cost > other.cost;
    }
};

// Helper Function: Runs Dijkstra from a specific source
// Returns the distance table: dist[node][0] (no coupon) and dist[node][1] (coupon used)
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
    // Input format: n, m, then edges, then the mandatory node
    if (!(cin >> n >> m)) return;

    vector<vector<Edge>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
    }

    int mandatoryNode;
    cin >> mandatoryNode; // The vertex we MUST visit

    // 1. Run Dijkstra from Start (1) -> Mandatory Node
    // This gives us the cost to reach 'mandatoryNode' with 0 or 1 coupon used.
    vector<vector<long long>> distFromStart = runDijkstra(n, 1, adj);

    // 2. Run Dijkstra from Mandatory Node -> End (n)
    // This gives us the cost to go from 'mandatoryNode' to 'n' with 0 or 1 coupon used.
    vector<vector<long long>> distFromMandatory = runDijkstra(n, mandatoryNode, adj);

    // 3. Calculate Results
    long long cost1_to_M = distFromStart[mandatoryNode][0]; // S->M (No coupon)
    long long cost2_to_M = distFromStart[mandatoryNode][1]; // S->M (Coupon used)
    
    long long cost1_from_M = distFromMandatory[n][0];       // M->E (No coupon)
    long long cost2_from_M = distFromMandatory[n][1];       // M->E (Coupon used)

    long long finalAns = INF;

    // SCENARIO A: Coupon used in the first leg (S -> M)
    // We arrive at M having used the coupon. We MUST take normal path M -> E.
    if (cost2_to_M != INF && cost1_from_M != INF) {
        finalAns = min(finalAns, cost2_to_M + cost1_from_M);
    }

    // SCENARIO B: Coupon saved for the second leg (M -> E)
    // We arrive at M without using coupon. We use coupon in M -> E.
    if (cost1_to_M != INF && cost2_from_M != INF) {
        finalAns = min(finalAns, cost1_to_M + cost2_from_M);
    }

    // (Scenario C: Coupon never used? Mathematically inferior, but covered by cases above if weights > 0)

    if (finalAns == INF) {
        cout << -1 << endl; // Or "Not possible"
    } else {
        cout << finalAns << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}
