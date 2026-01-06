#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;
using ll = long long;
const ll INF = 1e18;

struct Edge {
    int to;
    ll w;
};

struct State {
    ll dist;
    int node, used;
    bool operator>(const State& other) const {
        return dist > other.dist;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<vector<Edge>> adj(n+1);

    for(int i=0;i<m;i++) {
        int a,b; ll c;
        cin >> a >> b >> c;
        adj[a].push_back({b,c});
    }

    vector<vector<ll>> dp(n+1, vector<ll>(3, INF));
    dp[1][0] = 0; // start at Dhaka, coupon unused

    priority_queue<State, vector<State>, greater<State>> pq;
    pq.push({0,1,0});

    while(!pq.empty()) {
        auto [d,u,used] = pq.top(); pq.pop();
        if(d != dp[u][used]) continue;

        for(auto &e: adj[u]) {
            int v = e.to; ll w = e.w;
            // case 1: don't use coupon
            if(dp[v][used] > d + w) {
                dp[v][used] = d + w;
                pq.push({dp[v][used], v, used});
            }
            // case 2: use coupon if not used yet
            if(used==0 && dp[v][1] > d + w/2) {
                dp[v][1] = d + w/2;
                pq.push({dp[v][1], v, 1});
            }
            if(used==1 && dp[v][2] > d + w/2) {
                dp[v][2] = d + w/2;
                pq.push({dp[v][2], v, 2});
            }
        }
    }

    cout << min(dp[n][2],min(dp[n][0], dp[n][1]) )<< "\n";
}
