#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

const long long INF = 1e18;

struct Flight {
    int to;
    int dep; // Departure Time
    int dur; // Duration
};

void solve() {
    int n, m;
    if (!(cin >> n >> m)) return;

    vector<vector<Flight>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v, dep, dur;
        cin >> u >> v >> dep >> dur;
        adj[u].push_back({v, dep, dur});
    }

    int s, t, t0;
    cin >> s >> t >> t0;

    // dist[i] stores the earliest arrival time at node i
    vector<long long> dist(n + 1, INF);
    
    // Priority Queue: {arrival_time, u} (Min-Heap)
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;

    // Initial State: We are at source 's' at time 't0'
    dist[s] = t0;
    pq.push({t0, s});

    while (!pq.empty()) {
        long long curr_time = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        // If we found a faster way to 'u' already, skip this stale state
        if (curr_time > dist[u]) continue;

        // If we reached the target, we don't stop immediately in generic Dijkstra,
        // but for simple shortest path, the first time we extract 't', it's optimal.
        if (u == t) {
            cout << curr_time << endl;
            return;
        }

        for (const auto& flight : adj[u]) {
            // CHECK: Can we catch this flight?
            // We must arrive at 'u' (curr_time) BEFORE or AT departure (flight.dep)
            if (curr_time <= flight.dep) {
                long long arrival_at_dest = flight.dep + flight.dur;

                // RELAXATION: Is this flight getting us to 'v' earlier than before?
                if (arrival_at_dest < dist[flight.to]) {
                    dist[flight.to] = arrival_at_dest;
                    pq.push({dist[flight.to], flight.to});
                }
            }
        }
    }

    // If reachable, we returned above. If queue empty and t not reached:
    cout << -1 << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}
