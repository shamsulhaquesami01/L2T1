// NORMAL FLOW  TE MIN CUT BER KORA
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
    vector<vector<Edge>> graph; // Made public for Min-Cut DFS access

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
    // Store original edges to print later
    struct InputEdge { int u, v; };
    vector<InputEdge> inputs;

    for (int i = 0; i < m; i++) {
        int u, v,c;
        cin >> u >> v>>c;
        // CSES Police Chase weights are 1 (standard roads) or weighted. 
        // Usually, unweighted cuts = capacity 1. 
        // Problem specific: "roads" implies capacity 1 if unweighted.
        // Assuming capacity 1 for counting edges.
        mf.addEdge(u, v, c);// Roads are bidirectional
    }
    int  x ; cin>>x;
    while(x--){
        int u,v,c;
        cin>>u>>v>>c;
        inputs.push_back({u,v});
    }
    
    vector<bool> visited(n + 1, false);
    queue<int> q;
    q.push(1);
    visited[1] = true;

    while(!q.empty()){
        int u = q.front(); q.pop();
        for(auto &e : mf.graph[u]){
            // If there is remaining capacity, we can visit
            if(!visited[e.to] && e.capacity > e.flow){
                visited[e.to] = true;
                q.push(e.to);
            }
        }
    }

    // 3. Print edges between Reachable (Visited) and Unreachable (Not Visited)
    int idx=1;
    for( auto[u,v]:inputs){
        if(visited[u] && !visited[v] || visited[v] && !visited[u]){
           cout<<idx<<endl;
        
        }
        idx++;
    }

    return 0;
}