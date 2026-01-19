#include <iostream>
#include <vector>
#include <ctype.h>
#include <queue>
#include <climits>
#include<algorithm>
using namespace std;
using ll = long long;
class Edge
{
public:
    int from, to;
    ll w;
    bool operator<(const Edge &other) const
    {
        return w < other.w;
    }
};
class DSU
{

    vector<int> parent;
    vector<int> size;

public:
    DSU(int n)
    {
        parent.resize(n+1);
        size.resize(n+1, 1);
        for (int i = 1; i <= n; i++)
        {
            parent[i] = i;
        }
    }

    int find(int x)
    {
        if (parent[x] != x)
        {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unite(int x, int y)
    {
        int rootX = find(x);
        int rootY = find(y);

        if (rootX == rootY)
            return;

        if (size[rootX] < size[rootY])
        {
            parent[rootX] = rootY;
            size[rootY] += size[rootX];
        }
        else
        {
            parent[rootY] = rootX;
            size[rootX] += size[rootY];
        }
    }

    bool connected(int x, int y)
    {
        return find(x) == find(y);
    }

    int getSize(int x)
    {
        return size[find(x)];
    }
};

ll KRUSKAL(vector<Edge> &edges, int n)
{
    ll total=0;
    DSU dsu(n);
    sort(edges.begin(), edges.end());
    for (auto [u, v, w] : edges)
    {
        if (dsu.find(u) != dsu.find(v))
        {
            total+=w;
            dsu.unite(u, v);
        }
    }
    return total;
}
ll kruskal_forced(vector<Edge> &edges, int n, int x, int y)
{
    ll total=0;
    DSU dsu(n);
    sort(edges.begin(), edges.end());
    for (auto [u, v, w] : edges)
    {
        if(u==x && v==y || u==y && v==x){
        if (dsu.find(u) != dsu.find(v))
        {
            total+=w;
            dsu.unite(u, v);
        }
    }
    }
    for (auto [u, v, w] : edges)
    {
        if (dsu.find(u) != dsu.find(v))
        {
            total+=w;
            dsu.unite(u, v);
        }
    }
    return total;
}
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<Edge> edges;
    for (int i = 0; i < m; i++)
    {
        int u, v;
        ll w;
        cin >> u >> v >> w;
        edges.push_back({u, v, w});
    }
    vector<Edge> input_order = edges;
    ll pre =KRUSKAL(edges, n);
    for(auto e : input_order){
      ll final = kruskal_forced(edges,n, e.from, e.to);
      if(final==pre) cout<<"YES"<<endl;
      else cout<<"NO"<<endl;
    }

}

/*
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

const int MAXN = 200005;
const int LOGN = 20; // Sufficient for 2*10^5

struct Edge {
    int u, v, w, id;
    bool operator<(const Edge& other) const {
        return w < other.w;
    }
};

struct DSU {
    vector<int> parent;
    DSU(int n) {
        parent.resize(n + 1);
        for (int i = 1; i <= n; i++) parent[i] = i;
    }
    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }
    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) parent[root_i] = root_j;
    }
};

// Tree storage
vector<pair<int, int>> adj[MAXN];
// Binary Lifting Tables
int up[MAXN][LOGN];
int max_w[MAXN][LOGN];
int depth[MAXN];
bool inMST[MAXN]; // To track which original edges are in MST

void dfs(int u, int p, int w, int d) {
    depth[u] = d;
    up[u][0] = p;
    max_w[u][0] = w;
    
    // Standard Binary Lifting Precomputation
    for (int i = 1; i < LOGN; i++) {
        up[u][i] = up[up[u][i - 1]][i - 1];
        max_w[u][i] = max(max_w[u][i - 1], max_w[up[u][i - 1]][i - 1]);
    }
    
    for (auto& edge : adj[u]) {
        int v = edge.first;
        int weight = edge.second;
        if (v != p) {
            dfs(v, u, weight, d + 1);
        }
    }
}

// Function to get max weight on path between u and v
int getMaxOnPath(int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);
    
    int ret = 0;
    // 1. Lift u to the same depth as v
    for (int i = LOGN - 1; i >= 0; i--) {
        if (depth[u] - (1 << i) >= depth[v]) {
            ret = max(ret, max_w[u][i]);
            u = up[u][i];
        }
    }
    
    if (u == v) return ret;
    
    // 2. Lift both until they are just below LCA
    for (int i = LOGN - 1; i >= 0; i--) {
        if (up[u][i] != up[v][i]) {
            ret = max(ret, max_w[u][i]);
            ret = max(ret, max_w[v][i]);
            u = up[u][i];
            v = up[v][i];
        }
    }
    
    // 3. Final step to LCA
    ret = max(ret, max_w[u][0]);
    ret = max(ret, max_w[v][0]);
    return ret;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    
    vector<Edge> edges;
    vector<Edge> original_edges; // Keep original order for output
    
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        edges.push_back({u, v, w, i});
        original_edges.push_back({u, v, w, i});
    }

    // 1. Build MST
    sort(edges.begin(), edges.end());
    DSU dsu(n);
    vector<bool> is_mst_edge(m, false);
    
    for (auto& e : edges) {
        if (dsu.find(e.u) != dsu.find(e.v)) {
            dsu.unite(e.u, e.v);
            adj[e.u].push_back({e.v, e.w});
            adj[e.v].push_back({e.u, e.w});
            is_mst_edge[e.id] = true;
        }
    }

    // Check connectivity (optional based on problem constraints)
    // If graph is not connected, you might need to run DFS on each component.
    // Assuming connected for now (or handle loop 1 to N).
    
    // 2. Precompute LCA
    // '1' is root, parent of root is 1, weight to parent is 0
    dfs(1, 1, 0, 0); 

    // 3. Answer Queries (Check original edges)
    for (auto& e : original_edges) {
        if (is_mst_edge[e.id]) {
            cout << "YES" << endl;
        } else {
            // Check if this edge can replace a path edge
            int path_max = getMaxOnPath(e.u, e.v);
            if (e.w == path_max) {
                cout << "YES" << endl;
            } else {
                cout << "NO" << endl;
            }
        }
    }

    return 0;
}
*/