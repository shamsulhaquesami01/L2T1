

//CHECK EDGES IF IN ANY MST OR ALL OR NONE


#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Edge
{
public:
    int from, to, w, id; // Added 'id' to track original index
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
        parent.resize(n + 1);
        size.resize(n + 1, 1);
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
};

// Returns MST weight. Returns 1e9 if disconnected.
int runKruskal(int n, vector<Edge> &edges, int forbidden_index, int force_index)
{
    DSU dsu(n);
    int weight = 0;
    int edges_count = 0;

    // 1. Force specific edge first (if force_index is valid)
    if (force_index != -1)
    {
        dsu.unite(edges[force_index].from, edges[force_index].to);
        weight += edges[force_index].w;
        edges_count++;
    }

    // 2. Add rest normally
    for (int i = 0; i < edges.size(); i++)
    {
        // Skip the forbidden edge or the one we already forced
        if (i == forbidden_index || i == force_index)
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
    // Store verdicts mapped by original ID
    vector<string> results(edges.size());

    // Sort is required for Kruskal's greedy logic
    sort(edges.begin(), edges.end());

    // Calculate standard MST cost first
    int original_mst_cost = runKruskal(n, edges, -1, -1);

    for (int i = 0; i < edges.size(); i++)
    {
        // 1. Check Critical: What is MST cost WITHOUT this edge?
        // Note: We pass 'i' which is the current index in the SORTED vector
        int cost_without = runKruskal(n, edges, i, -1);

        if (cost_without > original_mst_cost)
        {
            // If cost increases (or graph becomes disconnected), it is Critical
            results[edges[i].id] = "any";
        }
        else
        {
            // 2. Check Pseudo-Critical: Force this edge to be used
            int cost_forced = runKruskal(n, edges, -1, i);
            
            if (cost_forced == original_mst_cost)
            {
                // If we can force it and still get the same optimal cost, it fits in SOME MST
                results[edges[i].id] = "at least one";
            }
            else
            {
                // If forcing it makes the cost worse, it is in NO MST
                results[edges[i].id] = "none";
            }
        }
    }

    // Print results in the original input order
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
            // Store 'i' as the original ID
            edges.push_back({u, v, w, i});
        }
        solveCriticalEdges(n, edges);
    }
    return 0;
}