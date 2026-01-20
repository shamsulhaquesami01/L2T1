#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

class Edge {
public:
    int u, v, w;
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
        return parent[x] == x ? x : parent[x] = find(parent[x]);
    }
    
    bool unite(int x, int y) {
        x = find(x); y = find(y);
        if (x == y) return false;
        if (size[x] < size[y]) swap(x, y);
        parent[y] = x;
        size[x] += size[y];
        return true;
    }
};

int main() {
    int n, m;
    cin >> n >> m;
    
    vector<Edge> allEdges(m);
    for (int i = 0; i < m; i++) {
        cin >> allEdges[i].u >> allEdges[i].v >> allEdges[i].w;
    }
    
    // Build original MST
    vector<Edge> sortedEdges = allEdges;
    sort(sortedEdges.begin(), sortedEdges.end());
    
    DSU dsu(n);
    vector<Edge> mstEdges;
    int originalCost = 0;
    
    for (auto& e : sortedEdges) {
        if (dsu.unite(e.u, e.v)) {
            mstEdges.push_back(e);
            originalCost += e.w;
        }
    }
    
    cout << "Original MST cost: " << originalCost << endl;
    cout << "Original MST edges:" << endl;
    for (auto& e : mstEdges) {
        cout << e.u << " " << e.v << " (weight: " << e.w << ")" << endl;
    }
    
    // Read vertex to remove
    int removeVertex;
    cout << "\nEnter vertex to remove: ";
    cin >> removeVertex;
    
    // Build new graph without this vertex
    vector<Edge> validEdges;
    for (auto& e : allEdges) {
        if (e.u != removeVertex && e.v != removeVertex) {
            validEdges.push_back(e);
        }
    }
    
    // Build new MST
    sort(validEdges.begin(), validEdges.end());
    
    DSU newDsu(n);
    vector<Edge> newMstEdges;
    int newCost = 0;
    int edgeCount = 0;
    int newN = n - 1; // Number of vertices after removal
    
    for (auto& e : validEdges) {
        if (newDsu.unite(e.u, e.v)) {
            newMstEdges.push_back(e);
            newCost += e.w;
            edgeCount++;
            if (edgeCount == newN - 1) {
                break;
            }
        }
    }
    
    cout << "\nAfter removing vertex " << removeVertex << ":" << endl;
    
    if (edgeCount == newN - 1) {
        cout << "New MST cost: " << newCost << endl;
        cout << "New MST edges:" << endl;
        for (auto& e : newMstEdges) {
            cout << e.u << " " << e.v << " (weight: " << e.w << ")" << endl;
        }
    } else {
        cout << "Graph becomes disconnected!" << endl;
    }
    
    return 0;
}
// ```

// **Sample Input:**
// ```
// 5 7
// 0 1 2
// 0 3 6
// 1 2 3
// 1 3 8
// 1 4 5
// 2 4 7
// 3 4 9
// 1