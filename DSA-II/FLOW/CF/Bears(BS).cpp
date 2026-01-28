// Problem Summary

// Niwel needs to deliver goods using exactly x bears from Node 1 to Node n.

// Every bear must carry the exact same weight (w).

// Bears act as discrete flow units.

// The total weight on a specific edge cannot exceed its capacity Ci

// If k bears use an edge, the total load is k×w. Therefore, the maximum number of bears allowed on an edge is ⌊Ci/w⌋.
// Goal: Maximize the Total Weight (x×w). Since x is fixed, we just need to maximize the weight per bear (w).

// Approach: This is a Binary Search on Answer combined with Max Flow.

// Binary Search for the value of w (weight per bear).

// Check Function: For a given w, construct a graph where the capacity of each edge is the number of bears it can support: Bear_Capacity=⌊Weight_Capacity/w⌋.

// Run Max Flow on this graph. If the max flow (total bears) ≥x, then this weight w is possible. Try a higher weight. Otherwise, try lower.







#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
#include <algorithm>
#include <iomanip>
#include <cmath>

using namespace std;

// --- YOUR STANDARD TEMPLATE START ---
struct Edge {
    int to;
    int capacity;
    int flow;
    int rev; // Index of the reverse edge
};

class MaxFlow {
    int N;
    vector<vector<Edge>> graph;

public:
    MaxFlow(int n) : N(n), graph(n) {}

    int addEdge(int u, int v, int cap) {
        // Forward edge: u -> v
        Edge a = {v, cap, 0, (int)graph[v].size()};
        // Backward edge: v -> u
        Edge b = {u, 0, 0, (int)graph[u].size()};
        graph[u].push_back(a);
        graph[v].push_back(b);
        return (int)graph[u].size() - 1;
    }

    int bfs(int s, int t, vector<pair<int, int>> &parent) {
        fill(parent.begin(), parent.end(), make_pair(-1, -1));
        queue<int> q;
        q.push(s);
        parent[s] = {s, -1};

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int i = 0; i < graph[u].size(); i++) {
                Edge &e = graph[u][i];
                if (parent[e.to].first == -1 && e.capacity > e.flow) {
                    parent[e.to] = {u, i};
                    if (e.to == t) return 1;
                    q.push(e.to);
                }
            }
        }
        return 0;
    }

    int edmondsKarp(int s, int t) {
        int maxFlow = 0;
        vector<pair<int, int>> parent(N);

        while (bfs(s, t, parent)) {
            int pathFlow = INT_MAX;
            for (int v = t; v != s;) {
                auto [u, idx] = parent[v];
                pathFlow = min(pathFlow, graph[u][idx].capacity - graph[u][idx].flow);
                v = u;
            }
            for (int v = t; v != s;) {
                auto [u, idx] = parent[v];
                graph[u][idx].flow += pathFlow;
                int rev_idx = graph[u][idx].rev;
                graph[v][rev_idx].flow -= pathFlow;
                v = u;
            }
            maxFlow += pathFlow;
        }
        return maxFlow;
    }
};
// --- YOUR TEMPLATE END ---

struct InputEdge {
    int u, v;
    double c;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    long long x; // Number of bears can be up to 100,000
    if (!(cin >> n >> m >> x)) return 0;

    vector<InputEdge> edges;
    for (int i = 0; i < m; i++) {
        int u, v;
        double c;
        cin >> u >> v >> c;
        edges.push_back({u, v, c});
    }

    double low = 0.0, high = 1000000.0 * n; // Safe upper bound
    double ans_w = 0.0;

    // Binary Search for 100 iterations (high precision)
    for (int iter = 0; iter < 100; iter++) {
        double mid = low + (high - low) / 2.0;
        
        // Avoid division by zero
        if (mid < 1e-9) {
            low = mid;
            continue;
        }

        // Build Graph for this weight 'mid'
        MaxFlow mf(n + 1);
        for (const auto& e : edges) {
            // number of bears , k  = floor(capacoty / BearWeight)
            // We clamp it to x because we don't need more than x flow
            long long cap = (long long)(e.c / mid);
            if (cap > x) cap = x; 
            mf.addEdge(e.u, e.v, (int)cap);
        }

        // Check if we can route 'x' bears
        if (mf.edmondsKarp(1, n) >= x) {
            ans_w = mid;
            low = mid; // Try heavier weights
        } else {
            high = mid; // Too heavy, reduce weight
        }
    }

    cout << fixed << setprecision(10) << ans_w * x << endl;

    return 0;
}