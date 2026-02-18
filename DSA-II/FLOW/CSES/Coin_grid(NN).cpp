#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
#include <algorithm>
#include <string>

using namespace std;
//sami
// --- STANDARD MAXFLOW TEMPLATE START ---
struct Edge {
    int to;
    int capacity;
    int flow;
    int rev; // Index of the reverse edge
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

    int bfs(int s, int t, vector<pair<int, int>> &parent) {
        fill(parent.begin(), parent.end(), make_pair(-1, -1));
        queue<int> q;
        q.push(s);
        parent[s] = {s, -1};

        while (!q.empty()) {
            int u = q.front();
            q.pop();
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
        vector<pair<int, int>> parent(N);

        while (bfs(s, t, parent)) {
            int pathFlow = INT_MAX;
            for (int v = t; v != s;) {
                auto [u, idx] = parent[v];
                pathFlow = min(pathFlow, graph[u][idx].capacity - graph[u][idx].flow);
                v = u;
            }
            for (int v = t; v != s;) {
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
// --- STANDARD MAXFLOW TEMPLATE END ---

int n;
vector<int> match_col_of_row; // match_col_of_row[r] = c means Row r matched with Col c
vector<int> match_row_of_col; // match_row_of_col[c] = r means Col c matched with Row r
vector<bool> visited_left;    // Rows visited in DFS
vector<bool> visited_right;   // Cols visited in DFS
vector<vector<int>> adj_bipartite; // Original bipartite edges (Row -> Cols)

// DFS for Konig's Theorem
void dfs(int u) {
    visited_left[u] = true;
    for (int v : adj_bipartite[u]) {
        if (!visited_right[v]) {
            visited_right[v] = true;
            int next_row = match_row_of_col[v];
            if (next_row != -1 && !visited_left[next_row]) {
                dfs(next_row);
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> n)) return 0;

    // Nodes: S=0, Rows=1..N, Cols=N+1..2N, T=2N+1
    int S = 0;
    int T = 2 * n + 1;
    MaxFlow mf(T + 1);

    adj_bipartite.resize(n + 1);

    for (int i = 1; i <= n; i++) {
        string row;
        cin >> row;
        
        // Connect Source -> Row
        mf.addEdge(S, i, 1);

        for (int j = 0; j < n; j++) {
            if (row[j] == 'o') {
                int col_idx = j + 1;
                // Connect Row -> Col
                mf.addEdge(i, n + col_idx, 1);
                
                // Save for DFS later
                adj_bipartite[i].push_back(col_idx); 
            }
        }
    }

    // Connect Cols -> Sink
    for (int j = 1; j <= n; j++) {
        mf.addEdge(n + j, T, 1);
    }

    // 1. Calculate Max Matching
    int matching_size = mf.edmondsKarp(S, T);

    // 2. Extract Matching Information
    match_col_of_row.assign(n + 1, -1);
    match_row_of_col.assign(n + 1, -1);

    for (int i = 1; i <= n; i++) {
        for (const auto &e : mf.graph[i]) {
            // If flow is 1 and it goes to a Column Node
            if (e.flow == 1 && e.to > n && e.to <= 2 * n) {
                int col_idx = e.to - n;
                match_col_of_row[i] = col_idx;
                match_row_of_col[col_idx] = i;
                break;
            }
        }
    }

    // 3. Construct Vertex Cover (Konig's Theorem)
    visited_left.assign(n + 1, false);
    visited_right.assign(n + 1, false);

    // Start DFS from all UNMATCHED Rows
    for (int i = 1; i <= n; i++) {
        if (match_col_of_row[i] == -1) {
            dfs(i);
        }
    }

    // Vertex Cover = (Rows NOT visited) U (Cols visited)
    cout << matching_size << "\n";
    
    // Print Rows
    for (int i = 1; i <= n; i++) {
        if (!visited_left[i]) {
            cout << "1 " << i << "\n";
        }
    }
    // Print Cols
    for (int j = 1; j <= n; j++) {
        if (visited_right[j]) {
            cout << "2 " << j << "\n";
        }
    }

    return 0;
}