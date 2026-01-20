#include <iostream>
#include <vector>
#include <ctype.h>
#include <queue>
#include <climits>
#include <algorithm>
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
};

class DSU
{
    vector<int> parent;
    vector<int> size;

public:
    DSU(int n)
    {
        // CHANGE 1: Resize to n + 1
        parent.resize(n + 1);
        size.resize(n + 1, 1);
        
        // CHANGE 2: Initialize up to <= n
        for (int i = 0; i <= n; i++)
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

void KRUSKAL(vector<Edge> &edges, int n)
{
    vector<pair<int, int>> result;
    int total = 0;
    DSU dsu(n); // This now creates a DSU of size n+1
    
    sort(edges.begin(), edges.end());
    
    for (auto [u, v, w] : edges)
    {
        if (dsu.find(u) != dsu.find(v))
        {
            result.push_back({u, v});
            total += w;
            dsu.unite(u, v);
        }
    }
    cout << "Total weight " << total << endl;
    for (auto [u, v] : result)
    {
        cout << u << " " << v << endl;
    }
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
        int u, v, w;
        cin >> u >> v >> w;
        edges.push_back({u, v, w});
    }
    int root;
    cin >> root; // Root isn't strictly needed for standard Kruskal's, but kept for consistency
    KRUSKAL(edges, n);
}