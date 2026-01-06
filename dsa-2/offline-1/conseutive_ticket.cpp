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
    int node, used,last;
    bool operator>(const State& other) const {
        return dist > other.dist;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m,k;
    cin >> n >> m>>k;
    vector<vector<Edge>> adj(n+1);

    for(int i=0;i<m;i++) {
        int a,b; ll c;
        cin >> a >> b >> c;
        adj[a].push_back({b,c});
    }


vector<vector<vector<ll>>> dp(n+1, vector<vector<ll>>(k+1, vector<ll>(2, INF)));
dp[1][0][0] = 0; // start at Dhaka, no coupon used, lastCoupon=0

priority_queue<State, vector<State>, greater<State>> pq;
pq.push({0,1,0,0}); // dist, node, used, lastCoupon

while(!pq.empty()) {
    auto [d,u,used,last] = pq.top(); pq.pop();
    if(d != dp[u][used][last]) continue;

    for(auto &e: adj[u]) {
        int v = e.to; ll w = e.w;
        // always can travel without coupon
        if(dp[v][used][0] > d + w) {
            dp[v][used][0] = d + w;
            pq.push({dp[v][used][0], v, used, 0});
        }
        // can use coupon only if last==0 and used<k
        if(last==0 && used<k && dp[v][used+1][1] > d + w/2) {
            dp[v][used+1][1] = d + w/2;
            pq.push({dp[v][used+1][1], v, used+1, 1});
        }
    }
}
}