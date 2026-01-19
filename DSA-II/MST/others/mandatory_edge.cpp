//Given a graph and one mandatory edge that MUST be included in the spanning tree, find the minimum cost spanning tree.
#include <iostream>
#include <vector>
#include <ctype.h>
#include <queue>
#include <climits>
#include<algorithm>
using namespace std;
class Edge
{
public:
    int from, to;
    int w;
    bool operator<(const Edge &other) const
    {
        return w < other.w;
    }
 int weight(int x , int y){
    
 }
};
class DSU
{

    vector<int> parent;
    vector<int> size;

public:
    DSU(int n)
    {
        parent.resize(n);
        size.resize(n, 1);
        for (int i = 0; i < n; i++)
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
// Alternative: Force mandatory edge first, then build rest of MST
void KRUSKAL_mandatory_simple(vector<Edge> &edges, int n, int mandX, int mandY) {
    DSU dsu(n);
    sort(edges.begin(), edges.end());
    
    vector<Edge> result;
    int totalWeight = 0;
    
    // Step 1: Add mandatory edge first
    for (auto& e : edges) {
        if ((e.from == mandX && e.to == mandY) || (e.from == mandY && e.to == mandX)) {
            result.push_back(e);
            totalWeight += e.w;
            dsu.unite(e.from, e.to);
            break;
        }
    }
    
    // Step 2: Add remaining edges using Kruskal
    for (auto& edge : edges) {
        if (dsu.find(edge.from) != dsu.find(edge.to)) {
            result.push_back(edge);
            totalWeight += edge.w;
            dsu.unite(edge.from, edge.to);
        }
    }
    
    cout << "Total weight " << totalWeight << endl;
    cout << "Mandatory edge " << mandX << " " << mandY << endl;
    for (auto& e : result) {
        cout << e.from << " " << e.to << endl;
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<Edge> edges;
    vector<vector<int> > adj(n+1);
    for (int i = 0; i < m; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        edges.push_back({u, v, w});
        adj[u][v]=w;
    }
    int x,y;
    cin >> x,y;
    KRUSKAL(edges,  n,  x, y,adj);
}

