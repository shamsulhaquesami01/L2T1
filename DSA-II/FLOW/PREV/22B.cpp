#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
#include <algorithm>

using namespace std;
struct position{
    int x,y,idx;
};
double calcDistance(position a, position b){
    return sqrt(abs(pow(a.x-b.x,2)+pow(a.y-b.y,2)));
}
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

    int mice, holes;
    double dist;
    if (cin >> mice >> holes>>dist) {
        
        
        int S = 0;
        int T = mice + holes + 1;
        MaxFlow mf(T + 1);

        // 2. Connect Source -> Boys
        vector<position> mice_pos;
        vector<position>hole_pos;
        for(int i=1; i<=mice; i++) {
            int x,y; cin>>x>>y;
            mice_pos.push_back({x,y,i});
            mf.addEdge(S, i, 1);
        }

        // 3. Connect Girls -> Sink
        for(int i=1; i<=holes; i++) {
             int x,y,c; cin>>x>>y>>c;
            hole_pos.push_back({x,y,i});
            mf.addEdge(mice + i, T, c);
        }

        vector<pair<int, int>> potential_matches; 

        for(position idur:mice_pos){
            for(position gorto:hole_pos){
                if(calcDistance(idur,gorto)<=dist){
                    mf.addEdge(idur.idx,gorto.idx+mice,1);
                }
            }
        }
        // 5. Compute Max Matching
        cout << mf.edmondsKarp(S, T) << endl;

    }
    return 0;
}