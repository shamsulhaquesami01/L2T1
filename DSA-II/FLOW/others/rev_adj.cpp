#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
#include <algorithm>

using namespace std;

// --- TEMPLATE START ---
struct Edge {
    int to;
    int capacity;
    int flow;
    int rev;
};

class MaxFlow {
public:
    int N;
    vector<vector<Edge>> graph;

    MaxFlow(int n) : N(n), graph(n) {}

    int addEdge(int u, int v, int cap) {
        Edge a = {v, cap, 0, (int)graph[v].size()};
        Edge b = {u, 0, 0, (int)graph[u].size()};
        graph[u].push_back(a);
        graph[v].push_back(b);
        return (int)graph[u].size() - 1;
    }

    int bfs(int s, int t, vector<pair<int,int>>& parent) {
        fill(parent.begin(), parent.end(), make_pair(-1, -1));
        queue<int> q;
        q.push(s);
        parent[s] = {s, -1};

        while (!q.empty()) {
            int u = q.front(); q.pop();
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

    long long edmondsKarp(int s, int t) {
        long long maxFlow = 0;
        vector<pair<int,int>> parent(N);
        while (bfs(s, t, parent)) {
            int pathFlow = INT_MAX;
            for (int v = t; v != s; ) {
                auto [u, idx] = parent[v];
                pathFlow = min(pathFlow, graph[u][idx].capacity - graph[u][idx].flow);
                v = u;
            }
            for (int v = t; v != s; ) {
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
// --- TEMPLATE END ---

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    cin >> n >> m;

    MaxFlow mf(n + 1);
    
    // Using simple vector of pairs since we just need u,v
    struct QueryEdge { int u, v, c; }; 
    vector<QueryEdge> queries;

    for (int i = 0; i < m; i++) {
        int u, v, c;
        cin >> u >> v >> c;
        mf.addEdge(u, v, c);
    }

    int x; 
    cin >> x;
    while(x--){
        int u, v, c;
        cin >> u >> v >> c;
        queries.push_back({u, v, c});
    }

    // 1. Calculate Max Flow first!
    mf.edmondsKarp(1, n);

    // 2. Forward BFS: Find all nodes reachable from Source (1)
    vector<bool> vis_s(n + 1, false);
    queue<int> q;
    
    q.push(1);
    vis_s[1] = true;

    while(!q.empty()){
        int u = q.front(); q.pop();
        for(auto &e : mf.graph[u]){
            // If there is residual capacity (cap > flow), we can move forward
            if(!vis_s[e.to] && e.capacity > e.flow){
                vis_s[e.to] = true;
                q.push(e.to);
            }
        }
    }

    // 3. Reverse BFS: Find all nodes that can reach Sink (n)
    // First, build a temporary reverse graph for traversal
    vector<vector<int>> rev_adj(n + 1);
    for(int u = 1; u <= n; u++) {
        for(auto &e : mf.graph[u]) {
            // If u -> v has residual capacity, then in reverse search v -> u is possible
            if(e.capacity > e.flow) {
                rev_adj[e.to].push_back(u);
            }
        }
    }

    vector<bool> vis_t(n + 1, false);
    while(!q.empty()) q.pop(); // Clear queue

    q.push(n); // Start from Sink
    vis_t[n] = true;

    while(!q.empty()){
        int u = q.front(); q.pop();
        // Traverse backwards using the reverse adjacency list
        for(int v : rev_adj[u]){
            if(!vis_t[v]){
                vis_t[v] = true;
                q.push(v);
            }
        }
    }

    // 4. Process Queries
    int idx = 1;
    for(auto qry : queries){
        int u = qry.u;
        int v = qry.v;
        
        // Check if u is reachable from S AND v can reach T
        // This implies edge (u, v) is on a potential path to increase flow
        if(vis_s[u] && vis_t[v]){
           cout << idx << endl; // Or output logic as required (e.g., u, v)
        }
        idx++;
    }

    return 0;
}