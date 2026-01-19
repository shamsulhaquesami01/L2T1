// You can make K edges have zero cost.
//  Find the minimum cost MST after optimally choosing which K edges to make free


#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Edge {
public:
    int from, to, w;
    bool operator<(const Edge &other) const {
        return w < other.w;
    }
};

class DSU {
    vector<int> parent, size;
public:
    DSU(int n) : parent(n), size(n, 1) {
        for (int i = 0; i < n; i++) parent[i] = i;
    }
    
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    
    void unite(int x, int y) {
        int rootX = find(x), rootY = find(y);
        if (rootX == rootY) return;
        if (size[rootX] < size[rootY]) swap(rootX, rootY);
        parent[rootY] = rootX;
        size[rootX] += size[rootY];
    }
};

void KRUSKAL_FREE_K(vector<Edge> &edges, int n, int k) {
    DSU dsu(n);
    sort(edges.begin(), edges.end());
    
    vector<Edge> mstEdges;
    int totalWeight = 0;
    
    // Step 1: Build normal MST
    for (auto& edge : edges) {
        if (dsu.find(edge.from) != dsu.find(edge.to)) {
            mstEdges.push_back(edge);
            totalWeight += edge.w;
            dsu.unite(edge.from, edge.to);
        }
    }
    
    // Step 2: Sort MST edges by weight (descending) to find K heaviest
    sort(mstEdges.begin(), mstEdges.end(), [](const Edge& a, const Edge& b) {
        return a.w > b.w; // descending order
    });
    
    // Step 3: Make K heaviest edges free
    int freeWeight = 0;
    int freeCount = min(k, (int)mstEdges.size());
    
    for (int i = 0; i < freeCount; i++) {
        freeWeight += mstEdges[i].w;
    }
    
    int finalWeight = totalWeight - freeWeight;
    
    // Output
    cout << "Total weight " << finalWeight << endl;
    cout << "Free edges used: " << freeCount << endl;
    
    // Sort back by endpoints for consistent output
    sort(mstEdges.begin(), mstEdges.end(), [](const Edge& a, const Edge& b) {
        if (a.from != b.from) return a.from < b.from;
        return a.to < b.to;
    });
    
    for (auto& e : mstEdges) {
        cout << e.from << " " << e.to << endl;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    vector<Edge> edges;
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        edges.push_back({u, v, w});
    }
    
    int k;
    cin >> k;
    
    KRUSKAL_FREE_K(edges, n, k);
    
    return 0;
}
