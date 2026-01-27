#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
#include <algorithm>
#include <map>
#include <string>

using namespace std;

// --- STANDARD MAXFLOW TEMPLATE ---
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

// Helper to map string sizes to integer IDs (1 to 6)
int getSizeID(string s) {
    if (s == "XXL") return 1;
    if (s == "XL") return 2;
    if (s == "L") return 3;
    if (s == "M") return 4;
    if (s == "S") return 5;
    if (s == "XS") return 6;
    return 0;
}

void solve(int t) {
    int N, M; // N = shirts per size (capacity), M = number of contestants
    cin >> N >> M;

    // Node Mapping:
    // Source = 0
    // Contestants = 1 to M
    // Sizes = M+1 to M+6
    // Sink = M+7
    
    int S = 0;
    int T = M + 7;
    MaxFlow mf(T + 1);

    // 1. Connect Source to Contestants
    for(int i = 1; i <= M; i++) {
        mf.addEdge(S, i, 1);
    }

    // 2. Connect Contestants to Preferred Sizes
    for(int i = 1; i <= M; i++) {
        string s1, s2;
        cin >> s1 >> s2;
        int size1 = getSizeID(s1);
        int size2 = getSizeID(s2);

        // Contestant i -> SizeNode (Offset by M)
        mf.addEdge(i, M + size1, 1);
        mf.addEdge(i, M + size2, 1);
    }

    // 3. Connect Sizes to Sink with Capacity N
    // (Since there are N colors, there are N shirts of EACH size)
    for(int i = 1; i <= 6; i++) {
        mf.addEdge(M + i, T, N);
    }

    // 4. Check if Max Flow == M (everyone got a shirt)
    if(mf.edmondsKarp(S, T) == M) {
        cout << "Case " << t << ": YES" << endl;
    } else {
        cout << "Case " << t << ": NO" << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    if (cin >> t) {
        for(int i = 1; i <= t; i++) {
            solve(i);
        }
    }
    return 0;
}
