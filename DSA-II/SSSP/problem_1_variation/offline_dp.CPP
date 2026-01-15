// MAIN PROBLEM USING DP
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


pair<int,int> solve(int start, vector<vector<Edge>>& adj, int n,int dest){
vector<vector<int>> dp(n+1, vector<int>(2, INT_MAX));
    dp[start][0] = 0; // start at Dhaka, coupon unused

    priority_queue<State, vector<State>, greater<State>> pq;
    pq.push({0,start,0});

    while(!pq.empty()) {
        auto [d,u,used] = pq.top(); pq.pop();
        if(d != dp[u][used]) continue;

        for(auto &e: adj[u]) {
            int v = e.to; int w = e.w;
            // case 1: don't use coupon
            if(dp[v][used] > d + w) {
                dp[v][used] = d + w;
                pq.push({dp[v][used], v, used});
            }
            // case 2: use coupon if not used yet
            if(used==0 && dp[v][1] > d + w/2) {
                dp[v][1] = d + floor(w/2);
                pq.push({dp[v][1], v, 1});
            }
        }
    }

    return {dp[dest][0], dp[dest][1]} ;
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<Edge>> adj(n+1);
   vector<vector<Edge>> rev_adj(n+1);
    for(int i=0;i<m;i++) {
        int a,b; int c;
        cin >> a >> b >> c;
        adj[a].push_back({b,c});
        rev_adj[b].push_back({a,c});
    }
  
}
