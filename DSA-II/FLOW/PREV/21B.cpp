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
// --- TEMPLATE END ---

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int applicants, jobs;
    if (cin >> applicants >> jobs) {
        
        int S = 0;
        int T = applicants + jobs + 1;
        MaxFlow mf(T + 1);

        // 2. Connect Source -> Boys
        for(int i=1; i<=applicants; i++) {
            mf.addEdge(S, i, 1);
        }

        // 3. Connect Girls -> Sink
        for(int i=1; i<=jobs; i++) {
            mf.addEdge(applicants + i, T, 1);
        }
        vector< int > w_streng;
        vector< int > j_streng;
        for (int i = 0; i < applicants; i++) {
            int  v;
            cin >> v;
            w_streng.push_back(v);
        }
           for (int i = 0; i < jobs; i++) {
            int  v;
            cin >> v;
            j_streng.push_back(v);
        }
            for(int i=0; i<applicants; i++){
                for(int j=0; j<jobs; j++){
                    if(w_streng[i]>=j_streng[j]){
                        int v = applicants+j+1;
                        mf.addEdge(i+1,v,1);
                    }
                }
            }
        // 5. Compute Max Matching
        cout << mf.edmondsKarp(S, T) << endl;
    return 0;
}
}