// Byteland has n cities, and m roads between them. The goal is to construct new roads so that there is a route between any two cities.
// Your task is to find out the minimum number of roads required, and also determine which roads should be built


#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class DSU
{
    vector<int> parent;
    vector<int> size;

public:
    DSU(int n)
    {
        parent.resize(n + 1);
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
};
// ----------------------------------

void solve()
{
    int n, m;
    if (!(cin >> n >> m)) return;

    DSU dsu(n);
    // This builds the initial connected components
    for (int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;
        dsu.unite(u, v);
    }


    vector<int> component_leaders;
    
    for (int i = 1; i <= n; i++)
    {
        if (dsu.find(i) == i) 
        {
            component_leaders.push_back(i);
        }
    }

    // 4. Output Results
    int new_roads_needed = component_leaders.size() - 1;
    cout << new_roads_needed << endl;

    // Connect the first leader to the second, second to third, etc.
    for (size_t i = 0; i < component_leaders.size() - 1; i++)
    {
        cout << component_leaders[i] << " " << component_leaders[i + 1] << endl;
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    solve();
    
    return 0;
}


//cd DSA-II/MST/OFFLINE/
// chmod +x test_all.sh
// ./test_all.sh