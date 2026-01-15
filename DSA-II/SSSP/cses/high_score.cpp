#include<iostream>
#include<vector>
#include<ctype.h>
#include<queue>
#include <climits>
using namespace std;

class Edge {
    public:
    long long int u, v, w;
    Edge(long long int u, long long int v, long long int w){
        this->u=u;
        this->v=v;
        this->w=w;
    }
};
vector<bool> can_reach_n;
vector<vector<int>> adj_rev; 

void find_reachable_from_n(int n) {
    queue<int> q;
    q.push(n);
    can_reach_n[n] = true;

    while (!q.empty()) {
        int curr = q.front();
        q.pop();

        for (int prev : adj_rev[curr]) {
            if (!can_reach_n[prev]) {
                can_reach_n[prev] = true;
                q.push(prev);
            }
        }
    }
}
void bellmanFord(long long int startNode, long long int N, long long int E, vector<Edge>& edges) {
    vector<long long int> dist(N+1, LONG_MAX);
    dist[startNode] = 0;

    // Relaxation Step (Run N-1 times)
    for (long long int i = 1; i <= N - 1; i++) {
        for (const auto& edge : edges) {
            long long int u = edge.u;
            long long int v = edge.v;
            long long int w = edge.w;
            if (dist[u] !=  LONG_MAX  && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w; 
            }
        }
    }

    for (const auto& edge : edges) {
        long long int u = edge.u;
        long long int v = edge.v;
        long long int w = edge.w;
        if (dist[u] != LONG_MAX && dist[u] + w < dist[v]) {
            if (can_reach_n[v]) {
                cout << -1 << endl;
                return;
            }
        }
    }
    cout<<-dist[N]<<endl;
}

 int main() {
   int n,m;
    cin>>n>>m;
    vector<Edge> edges;
 can_reach_n.assign(n + 1, false);
    adj_rev.resize(n + 1);

    for (int i = 0; i < m; i++) {
        long long int u, v, w;
        cin >> u >> v >> w;
        edges.push_back(Edge(u, v, -w));
        adj_rev[v].push_back(u);
    }
    find_reachable_from_n(n);
    bellmanFord(1, n, m, edges);
    return 0;
}