#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
#include <algorithm>

using namespace std;

// --- TEMPLATE ---
struct Edge { int to, capacity, flow, rev; };
class MaxFlow {
    int N;
    vector<vector<Edge>> graph;
public:
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
        q.push(s); parent[s] = {s, -1};
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
    int edmondsKarp(int s, int t) {
        int maxFlow = 0;
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
    Edge getEdge(int u, int idx)
    {
        return graph[u][idx];
    }
};

void solve() {
    int n, m;
    cin >> n >>m;
    vector<tuple<int,int,int> > A(n);
    for(int i=0; i<n; i++){
        int a,b,c;
        cin>>a>>b>>c;
        A[i]={a,b,c};
    }
    
    vector<tuple<int,int,int> > B(m);
    for(int i=0; i<m; i++) {
        int a,b,c;
        cin>>a>>b>>c;
        B[i]={a,b,c};
    }

    // Nodes: Source(0), Sink(n+m+1), A(1..n), B(n+1..n+m)
    int S = 0, T = n + m + 1;
    MaxFlow mf(T + 1);

    for(int i=0; i<n; i++) mf.addEdge(S, i+1, 1);
    for(int j=0; j<m; j++) mf.addEdge(n+1+j, T, 1);


    //shorto onujayi edge banano just
    vector<tuple<int, int, int>> matching;
    for(int i=0; i<n; i++) {
         auto [h1,a1,d1]=A[i];
        for(int j=0; j<m; j++) {
            auto [h2,a2,d2]=B[j];
            if(abs(h1-h2)<=10 && abs(a1-a2) <=5 ){
            int idx= mf.addEdge(i+1,n+1+j,1);
            matching.emplace_back(i+1,n+1+j,idx);
            }
        }
    }
    cout<<mf.edmondsKarp(S,T)<<endl;
    for (const auto &[u, v, idx] : matching)
    {
        Edge e = mf.getEdge(u, idx);
        if (e.flow == 1)
        {
            cout <<"Man" <<u << " " <<"Woman"<< v-n << endl;
        }
    }
}
int main() {
    solve();
}