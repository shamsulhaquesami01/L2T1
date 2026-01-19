#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class DSU
{

public:
    vector<int> parent;
    vector<int> size;
    int numberOfComponents;
    int maxComponentSize;

    DSU(int n)
    {
        parent.resize(n + 1);
        size.resize(n + 1, 1);
        numberOfComponents=n;
        maxComponentSize=1;
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
        numberOfComponents--;
        if (size[rootX] < size[rootY])
        {
            parent[rootX] = rootY;
            size[rootY] += size[rootX];
            maxComponentSize= max(maxComponentSize,size[rootY]);
        }
        else
        {
            parent[rootY] = rootX;
            size[rootX] += size[rootY];
             maxComponentSize= max(maxComponentSize,size[rootX]);
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
        cout<<dsu.numberOfComponents<<" "<<dsu.maxComponentSize<<endl;
    }


}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    solve();
    
    return 0;
}