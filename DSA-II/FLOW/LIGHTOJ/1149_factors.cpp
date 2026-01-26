// Bipartite Matching The Subtle Logic: 
// The problem asks to pair numbers from Set A with Set B such that Ai
// divides Bj This is a classic Maximum Bipartite Matching.




// You will be given two sets of integers.
//  Let's call them set A and set B. Set A contains n elements and set B contains m elements.
//   You have to remove k1 elements from set A and k2 elements from set B so that of the remaining values,
//    no integer in set B is a multiple of any integer in set A. k1 should be in the range [0, n] and k2 in the range [0, m].
// You have to find the value of (k1 + k2) such that (k1 + k2) is as small as possible.
//  P is a multiple of Q if there is some integer K such that P = K * Q.

// Suppose set A is {2, 3, 4, 5} and set B is {6, 7, 8, 9}. 
// By removing {2, 3} from A and {8} from B, we get the sets {4, 5} and {6, 7, 9}. 
// Here none of the integers 6, 7 or 9 is a multiple of 4 or 5.

// So for this case, the answer is 3 (two from set A and one from set B)


// Direct Max Flow The Subtle Logic: This is the most basic definition of Max Flow. 
// The only "trick" is that there can be multiple cables between two node


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




    ///question e chaynai eta 
    
    void printVertexCover(int n, int m, int S, int T, MaxFlow &mf) {
    // 1. Identify Matched Edges and Unmatched Left Nodes
    vector<bool> isMatchedLeft(n + 1, false);
    vector<vector<int>> adj(n + 1); // Only forward edges for DFS
    vector<int> matchRightToLeft(m + 1, -1);

    // Reconstruct matching from flow
    for (int u = 1; u <= n; u++) {
        for (auto &e : mf.graph[u]) {
            // Check edges going to B (Node IDs > n and < T)
            if (e.to > n && e.to < T) {
                // If flow is 1, it's a matched edge
                if (e.flow == 1) {
                    isMatchedLeft[u] = true;
                    matchRightToLeft[e.to - n] = u; 
                }
                // Store regular adjacency for DFS
                adj[u].push_back(e.to - n); 
            }
        }
    }

    // 2. DFS from Unmatched Left Nodes
    vector<bool> visitedLeft(n + 1, false);
    vector<bool> visitedRight(m + 1, false);
    queue<int> q;

    for (int i = 1; i <= n; i++) {
        if (!isMatchedLeft[i]) {
            q.push(i);
            visitedLeft[i] = true;
        }
    }

    while (!q.empty()) {
        int u = q.front(); q.pop();

        for (int v : adj[u]) {
            if (!visitedRight[v]) {
                // Rule: Left -> Right along NON-MATCHING edges
                // Wait! In residual graph logic, we just follow paths.
                // Standard Konig construction:
                // Visit Right node if connected to visited Left node
                // BUT only if NOT the matched edge? 
                // Actually, simplest logic: 
                // Left->Right (Any Edge) 
                // Right->Left (Only Matched Edge)
                
                // Since 'u' is visited, try to visit v
                // But we can only go to v if (u,v) is NOT the matched edge
                // Actually, if (u,v) IS the matched edge, we can't traverse L->R in alternating path
                // Correct Alternating Path Logic:
                // L->R: Only Unused Edges.
                // R->L: Only Used Edges.
                
                // Let's check flow:
                // If flow(u->v) == 0, we can visit v (Unused edge)
                // If we visit v, we MUST immediately follow the match back to L
                
                int matchedL = matchRightToLeft[v];
                if (matchedL != -1 && matchedL != u) {
                     // Check if this specific u->v is NOT the matching one
                     // Since v is matched to matchedL, u->v is definitely NOT the matching edge
                     visitedRight[v] = true;
                     if (!visitedLeft[matchedL]) {
                         visitedLeft[matchedL] = true;
                         q.push(matchedL);
                     }
                } 
            }
        }
    }

    // 3. Print Minimum Vertex Cover
    // MVC = (Left Unvisited) + (Right Visited)
    cout << "Ideally Remove Left Nodes: ";
    for (int i = 1; i <= n; i++) {
        if (!visitedLeft[i]) cout << i << " "; // Assuming Input Value = ID or map needed
    }
    cout << "\nIdeally Remove Right Nodes: ";
    for (int j = 1; j <= m; j++) {
        if (visitedRight[j]) cout << j << " ";
    }
    cout << endl;
}
};

void solve(int t) {
    int n, m;
    cin >> n;
    vector<int> A(n);
    for(int i=0; i<n; i++) cin >> A[i];
    
    cin >> m;
    vector<int> B(m);
    for(int i=0; i<m; i++) cin >> B[i];

    // Nodes: Source(0), Sink(n+m+1), A(1..n), B(n+1..n+m)
    int S = 0, T = n + m + 1;
    MaxFlow mf(T + 1);

    for(int i=0; i<n; i++) mf.addEdge(S, i+1, 1);
    for(int j=0; j<m; j++) mf.addEdge(n+1+j, T, 1);

    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++) {
            if(B[j] % A[i] == 0) {
                mf.addEdge(i+1, n+1+j, 1);
            }
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