#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
#include <algorithm>

using namespace std;

// --- TEMPLATE ---
struct conditions{
    int exp,age,crime,origin,weapon,trust,language;
};
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
};

void solve(int t) {
    int n, m;
    cin >> n >>m;
    vector< conditions > A;
    for(int i=0; i<n; i++){
        int a,b,c,d,e,f,g;
        cin>>a>>b>>c>>d>>e>>f>>g;
        A.push_back({a,b,c,d,e,f,g});
    }
    
    vector< conditions > B;
    for(int i=0; i<m; i++) {
       int a,b,c,d,e,f,g;
        cin>>a>>b>>c>>d>>e>>f>>g;
        B.push_back({a,b,c,d,e,f,g});
    }

    // Nodes: Source(0), Sink(n+m+1), A(1..n), B(n+1..n+m)
    int S = 0, T = n + m + 1;
    MaxFlow mf(T + 1);

    for(int i=0; i<n; i++) mf.addEdge(S, i+1, 1);
    for(int j=0; j<m; j++) mf.addEdge(n+1+j, T, 1);


    //shorto onujayi edge banano just
    for(int i=0; i<n; i++) {
         auto [exp,age,crime,origin,weapon,trust,language]=A[i];
        for(int j=0; j<m; j++) {
            auto [a,b,c,d,e,f,g]=B[j];
            if(abs(exp-a)<=12 && abs(age-b) <=5 &&( (crime==0 && c==0)|| (crime==1 && c==1))
              && ( (origin==0 && d==0)|| (origin==1 && d==1)) && (weapon||e) && (trust+f >=10)
              && (language&g)
        )
            mf.addEdge(i+1,n+1+j,1);
        }
    }

    cout << "Case " << t << ": " << mf.edmondsKarp(S, T) << endl;
}
int main() {
    int t;
    cin >> t;
    for (int i = 1; i <= t; i++) {
        solve(i);
    }
}