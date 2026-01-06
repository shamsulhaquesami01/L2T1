#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

const long long INF = 1e18;

// Edge structure: Destination, Cost, Departure Time, Arrival Time
struct Edge {
    int v;
    long long w;   // Cost (Ticket Price)
    long long dep; // Departure Time
    long long arr; // Arrival Time
};

// State structure for Priority Queue
struct State {
    long long cost; // Money spent (Primary Goal)
    int u;
    int used;       // 0 = coupon not used, 1 = coupon used
    long long time; // Current arrival time at u (Constraint)

    // Operator overloading for Min-Priority Queue (Sort by Cost first)
    bool operator>(const State& other) const {
        if (cost != other.cost)
            return cost > other.cost;
        return time > other.time; // If costs equal, prefer earlier arrival
    }
};

void solve() {
    int n, m;
    // Input: n (cities), m (flights)
    if (!(cin >> n >> m)) return;

    vector<vector<Edge>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v;
        long long w, dep, arr;
        cin >> u >> v >> w >> dep >> arr;
        adj[u].push_back({v, w, dep, arr});
    }

    // "best_states[u][used]" stores a list of {cost, time} pairs that we have reached.
    // We use this to prune "useless" paths.
    vector<pair<long long, long long>> best_states[100005][2];

    // Min-Priority Queue
    priority_queue<State, vector<State>, greater<State>> pq;

    // Initial State: Start at 1, Cost 0, Coupon 0, Time 0
    // We assume the traveler starts at time 0.
    pq.push({0, 1, 0, 0});
    best_states[1][0].push_back({0, 0});

    long long min_cost_to_dest = INF;

    while (!pq.empty()) {
        auto [cost, u, used, time] = pq.top();
        pq.pop();

        // --- PARETO PRUNING ---
        // Check if this state is strictly worse than something we processed already.
        // A state is worse if we already visited 'u' (with same coupon status)
        // with <= cost AND <= time.
        bool dominated = false;
        for (const auto& p : best_states[u][used]) {
            // p.first = saved_cost, p.second = saved_time
            if (p.first <= cost && p.second <= time) {
                // We found a previous path that is cheaper (or equal) AND earlier (or equal).
                // But we must be careful: we are popping from PQ, so 'cost' is >= p.first.
                // The crucial check is strict dominance. If it's exact duplicate, skip.
                if (p.first == cost && p.second == time) {
                    // This is likely the exact entry we just pushed, or a duplicate.
                    // If we are strictly worse, mark dominated.
                } else {
                    dominated = true; 
                    break;
                }
            }
        }
        // Note: In strict Dijkstra, popping ensures minimal cost. 
        // But with time, we might pop {100, time 5} then later {120, time 3}.
        // The check above is a heuristic. For strict correctness in complex graphs:
        // We skip ONLY if there is a state with (cost <= curr && time <= curr) that isn't itself.
        // Simplified check for efficiency:
        // Since we push to 'best_states' BEFORE pushing to PQ (or immediately after),
        // we can just run the relaxation check logic below.

        // Optimization: If we already found a path to N cheaper than this current partial path,
        // and we are AT N, we can stop or record.
        if (u == n) {
            min_cost_to_dest = min(min_cost_to_dest, cost);
            continue; 
        }
        
        // Use the simplified dominance check:
        // If current cost > min_cost_to_dest found so far, we can prune (if we only care about cost)
        if (cost >= min_cost_to_dest) continue; 

        for (const auto& edge : adj[u]) {
            int v = edge.v;
            long long w = edge.w;
            long long dep = edge.dep;
            long long arr = edge.arr;

            // --- CONSTRAINT CHECK ---
            // Can we catch this flight?
            // We must represent at 'u' (current 'time') BEFORE or AT departure ('dep')
            if (time <= dep) {
                
                // --- OPTION 1: Normal Travel (No Coupon Used Here) ---
                long long newCost = cost + w;
                long long newTime = arr;

                // Check if this new state is worth pushing
                bool worth_pushing = true;
                for (const auto& p : best_states[v][used]) {
                    if (p.first <= newCost && p.second <= newTime) {
                        worth_pushing = false;
                        break;
                    }
                }

                if (worth_pushing) {
                    best_states[v][used].push_back({newCost, newTime});
                    pq.push({newCost, v, used, newTime});
                }

                // --- OPTION 2: Use Coupon (if not used yet) ---
                if (used == 0) {
                    long long newCostDisc = cost + (w / 2); // Coupon halves COST
                    // Time remains the same (flight schedule doesn't change)
                    
                    worth_pushing = true;
                    for (const auto& p : best_states[v][1]) {
                        if (p.first <= newCostDisc && p.second <= newTime) {
                            worth_pushing = false;
                            break;
                        }
                    }

                    if (worth_pushing) {
                        best_states[v][1].push_back({newCostDisc, newTime});
                        pq.push({newCostDisc, v, 1, newTime});
                    }
                }
            }
        }
    }

    if (min_cost_to_dest == INF) {
        cout << -1 << endl; // Or "Impossible"
    } else {
        cout << min_cost_to_dest << endl;
    }
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}