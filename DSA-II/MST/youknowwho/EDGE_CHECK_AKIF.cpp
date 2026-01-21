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
    int id; // Added ID to track original input order

    // Operator for sorting by weight
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
        parent.resize(n + 1); // +1 safety for 1-based indexing
        size.resize(n + 1, 1);
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

void KRUSKAL(vector<Edge> &edges, int n, int m)
{
    // Vector to store answers ordered by original edge ID
    vector<string> results(m); 
    DSU dsu(n);

    // 1. Sort edges by weight
    sort(edges.begin(), edges.end());

    // 2. Process edges in batches of equal weights
    int i = 0;
    while (i < edges.size())
    {
        int j = i;
        // Find the range [i, j) containing all edges of the same weight
        while (j < edges.size() && edges[j].w == edges[i].w)
        {
            j++;
        }

        // Phase 1: Check connectivity (Look Ahead)
        // For every edge in this batch, check if it bridges two DIFFERENT components
        // formed by strictly lighter edges.
        for (int k = i; k < j; k++)
        {
            if (dsu.find(edges[k].from) != dsu.find(edges[k].to))
            {
                results[edges[k].id] = "YES";
            }
            else
            {
                results[edges[k].id] = "NO";
            }
        }

        // Phase 2: Unite (Commit)
        // Now actually merge the components to prepare for the heavier edges
        for (int k = i; k < j; k++)
        {
            if (dsu.find(edges[k].from) != dsu.find(edges[k].to))
            {
                dsu.unite(edges[k].from, edges[k].to);
            }
        }

        // Move to the next batch
        i = j;
    }

    // Output results in the original input order
    for (const string &res : results)
    {
        cout << res << endl;
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (cin >> n >> m) {
        vector<Edge> edges;
        for (int i = 0; i < m; i++)
        {
            int u, v, w;
            cin >> u >> v >> w;
            // Store 'i' as the ID so we remember this was the 0th, 1st, 2nd... edge
            edges.push_back({u, v, w, i});
        }
        
        // Root isn't typically needed for this specific problem, 
        // but reading it to match your template structure
        int root; 
        if (cin >> root) {
             KRUSKAL(edges, n, m);
        } else {
             // Handle case if root is optional or not present
             KRUSKAL(edges, n, m);
        }
    }
    return 0;
}