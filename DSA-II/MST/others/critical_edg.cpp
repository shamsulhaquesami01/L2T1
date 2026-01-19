// Critical & Pseudo-Critical Edges
// Concept:

// Critical Edge: An edge that exists in every possible MST. (Removing it increases the MST cost).
// Pseudo-Critical Edge: An edge that exists in some MSTs but not all. Logic:
// To check if Edge E is Critical: Run MST without E. If New_Cost > Original_MST_Cost, then E is critical.
// To check if Edge E is Pseudo-Critical: Run MST forcing E to be included
// (start Kruskal's with E already in DSU). If Forced_Cost == Original_MST_Cost, it is pseudo-critical.

// Helper function to run Kruskal's with constraints
// forbidden_id: skip this edge index
// force_id: force this edge index to be in MST
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
int runKruskal(int n, vector<Edge> &edges, int forbidden_id, int force_id)
{
    DSU dsu(n);
    int weight = 0;
    int edges_count = 0;

    // 1. Force specific edge first
    if (force_id != -1)
    {
        dsu.unite(edges[force_id].from, edges[force_id].to);
        weight += edges[force_id].w;
        edges_count++;
    }

    // 2. Add rest normally
    for (int i = 0; i < edges.size(); i++)
    {
        if (i == forbidden_id || i == force_id)
            continue;

        if (dsu.find(edges[i].from) != dsu.find(edges[i].to))
        {
            dsu.unite(edges[i].from, edges[i].to);
            weight += edges[i].w;
            edges_count++;
        }
    }

    // If graph is not connected, return Infinity
    if (edges_count < n - 1)
        return 1e9;
    return weight;
}

void solveCriticalEdges(int n, vector<Edge> &edges)
{
    // Note: Don't sort inside if indices matter, but for Kruskal we usually need sort.
    // For this problem, we sort first, then track indices.
    sort(edges.begin(), edges.end());

    int original_mst_cost = runKruskal(n, edges, -1, -1);

    vector<int> critical, pseudo_critical;

    for (int i = 0; i < edges.size(); i++)
    {
        // Check Critical: MST without this edge
        int cost_without = runKruskal(n, edges, i, -1);

        if (cost_without > original_mst_cost)
        {
            critical.push_back(i); // It's critical
            cout<<"any"<<endl;
        }
        else
        {
            // Check Pseudo-Critical: Force this edge
            int cost_forced = runKruskal(n, edges, -1, i);
            if (cost_forced == original_mst_cost)
            {
                pseudo_critical.push_back(i);
                cout<<"at least one"<<endl;
            }
            else{
                cout<<"none"<<endl;
            }
        }
       

    }

    cout << "Critical Edges Count: " << critical.size() << endl;
    cout << "Pseudo-Critical Edges Count: " << pseudo_critical.size() << endl;
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
    solveCriticalEdges(n,edges);
}