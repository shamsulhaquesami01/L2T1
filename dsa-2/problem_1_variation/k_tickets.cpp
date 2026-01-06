#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

const long long INF = 1e18;

struct State {
    long long cost;
    int u;
    int coupons_used;

    bool operator>(const State& other) const {
        return cost > other.cost;
    }
};

void solve() {
    int n, m, k;
    cin >> n >> m >> k; // k is number of coupons

    vector<vector<pair<int, int>>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
    }

    // dist[u][j] = min cost to reach u using exactly j coupons
    vector<vector<long long>> dist(n + 1, vector<long long>(k + 1, INF));
    priority_queue<State, vector<State>, greater<State>> pq;

    dist[1][0] = 0;
    pq.push({0, 1, 0});

    while (!pq.empty()) {
        auto [d, u, used] = pq.top();
        pq.pop();

        if (d > dist[u][used]) continue;

        for (auto& edge : adj[u]) {
            int v = edge.first;
            int w = edge.second;

            // Option 1: Don't use a coupon
            if (d + w < dist[v][used]) {
                dist[v][used] = d + w;
                pq.push({dist[v][used], v, used});
            }

            // Option 2: Use a coupon (if we have any left)
            if (used < k) {
                int new_w = w / 2;
                if (d + new_w < dist[v][used + 1]) {
                    dist[v][used + 1] = d + new_w;
                    pq.push({dist[v][used + 1], v, used + 1});
                }
            }
        }
    }

    // The answer is the min cost to reach node n using ANY number of coupons (0 to k)
    long long ans = INF;
    for (int i = 0; i <= k; ++i) {
        ans = min(ans, dist[n][i]);
    }
    cout << ans << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}