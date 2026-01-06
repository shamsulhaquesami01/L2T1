#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
using namespace std;

struct Edge {
    int to;
    int w;
};

struct State {
    int dist;
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
    int x,y,w; cin>>x>>y>>w;
    auto [first_un, first_used]=solve(1,adj,n,x);
    auto [second_unused, second_used]=solve(n,rev_adj,n,y);
    //minimum of these 3 
    cout<<first_un+w+second_used<<endl;
    cout<<first_used + w + second_unused<<endl;
    cout<<first_un+ floor(w/2) +second_unused<<endl;
}
