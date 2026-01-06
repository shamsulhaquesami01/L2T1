#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

const long long INF = 1e18;
const int MOD = 1e9 + 7;

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

    // 1. Data Structures for 2 States (0 and 1)
    // dist[node][state]
    vector<vector<long long>> dist(n + 1, vector<long long>(2, INF));
    vector<vector<long long>> count(n + 1, vector<long long>(2, 0));
    vector<vector<int>> min_flights(n + 1, vector<int>(2, 0));
    vector<vector<int>> max_flights(n + 1, vector<int>(2, 0));

    // Priority Queue
    priority_queue<State, vector<State>, greater<State>> pq;

    // 2. Initialization
    dist[1][0] = 0;
    count[1][0] = 1;
    min_flights[1][0] = 0;
    max_flights[1][0] = 0;
    pq.push({0, 1, 0});

    while (!pq.empty()) {
        auto [d, u, used] = pq.top();
        pq.pop();

        if (d > dist[u][used]) continue;

        for (const auto& edge : adj[u]) {
            int v = edge.v;
            long long w = edge.w;

            // --- OPTION A: Normal Travel (Keep same state) ---
            // Valid for both state 0 -> 0 and state 1 -> 1
            long long costNormal = d + w;
            
            // Strictly Better Path
            if (costNormal < dist[v][used]) {
                dist[v][used] = costNormal;
                count[v][used] = count[u][used];
                min_flights[v][used] = min_flights[u][used] + 1;
                max_flights[v][used] = max_flights[u][used] + 1;
                pq.push({dist[v][used], v, used});
            }
            // Equal Cost Path
            else if (costNormal == dist[v][used]) {
                count[v][used] = (count[v][used] + count[u][used]) % MOD;
                min_flights[v][used] = min(min_flights[v][used], min_flights[u][used] + 1);
                max_flights[v][used] = max(max_flights[v][used], max_flights[u][used] + 1);
            }

            // --- OPTION B: Use Coupon (Transition 0 -> 1) ---
            // Only valid if currently state 0
            if (used == 0) {
                long long costCoupon = d + (w / 2);

                // Strictly Better Path
                if (costCoupon < dist[v][1]) {
                    dist[v][1] = costCoupon;
                    count[v][1] = count[u][0];
                    min_flights[v][1] = min_flights[u][0] + 1;
                    max_flights[v][1] = max_flights[u][0] + 1;
                    pq.push({dist[v][1], v, 1});
                }
                // Equal Cost Path
                else if (costCoupon == dist[v][1]) {
                    count[v][1] = (count[v][1] + count[u][0]) % MOD;
                    min_flights[v][1] = min(min_flights[v][1], min_flights[u][0] + 1);
                    max_flights[v][1] = max(max_flights[v][1], max_flights[u][0] + 1);
                }
            }
        }
    }

    // 3. Final Result Merging
    long long finalCost = INF;
    long long finalCount = 0;
    int finalMinFlights = 0;
    int finalMaxFlights = 0;

    long long cost0 = dist[n][0];
    long long cost1 = dist[n][1];

    if (cost1 < cost0) {
        // Coupon path is strictly better
        finalCost = cost1;
        finalCount = count[n][1];
        finalMinFlights = min_flights[n][1];
        finalMaxFlights = max_flights[n][1];
    } 
    else if (cost0 < cost1) {
        // No-coupon path is strictly better (rare but possible)
        finalCost = cost0;
        finalCount = count[n][0];
        finalMinFlights = min_flights[n][0];
        finalMaxFlights = max_flights[n][0];
    } 
    else {
        // Both paths have equal optimal cost
        finalCost = cost1; // same as cost0
        finalCount = (count[n][0] + count[n][1]) % MOD;
        finalMinFlights = min(min_flights[n][0], min_flights[n][1]);
        finalMaxFlights = max(max_flights[n][0], max_flights[n][1]);
    }

    if (finalCost == INF) {
        // Or specific error output if needed
        cout << -1 << endl;
    } else {
        cout << finalCost << " " 
             << finalCount << " " 
             << finalMinFlights << " " 
             << finalMaxFlights << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}