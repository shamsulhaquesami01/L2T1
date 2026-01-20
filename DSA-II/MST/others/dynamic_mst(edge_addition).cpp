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

// Find max weight edge on path from u to v using DFS
bool findMaxOnPath(int curr, int target, vector<vector<pair<int,int>>>& adj,
                   vector<bool>& visited, int& maxWeight) {
    if (curr == target) return true;
    
    visited[curr] = true;
    
    for (auto [neighbor, weight] : adj[curr]) {
        if (!visited[neighbor]) {
            int prevMax = maxWeight;
            maxWeight = max(maxWeight, weight);
            
            if (findMaxOnPath(neighbor, target, adj, visited, maxWeight)) {
                return true;
            }
            
            maxWeight = prevMax;
        }
    }
    
    return false;
}

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
    
    // Add new edge
    int newU, newV, newW;
    cout << "\nEnter new edge to add (u v w): ";
    cin >> newU >> newV >> newW;
    
    // Build adjacency list from current MST
    vector<vector<pair<int,int>>> adj(n);
    for (auto& e : mstEdges) {
        adj[e.u].push_back({e.v, e.w});
        adj[e.v].push_back({e.u, e.w});
    }
    
    // Check if endpoints are already connected
    DSU checkDsu(n);
    for (auto& e : mstEdges) {
        checkDsu.unite(e.u, e.v);
    }
    
    if (checkDsu.find(newU) != checkDsu.find(newV)) {
        // Endpoints not connected - adding edge reduces components
        cout << "\nEndpoints not connected in MST!" << endl;
        cout << "Adding edge connects two components." << endl;
        cout << "New MST cost: " << originalCost + newW << endl;
        return 0;
    }
    
    // Find max weight edge on path from newU to newV
    vector<bool> visited(n, false);
    int maxWeight = 0;
    findMaxOnPath(newU, newV, adj, visited, maxWeight);
    
    cout << "\nMax weight edge on path " << newU << " to " << newV << ": " << maxWeight << endl;
    
    if (newW < maxWeight) {
        cout << "New edge (" << newW << ") is lighter than max edge (" << maxWeight << ")" << endl;
        cout << "Replace max edge with new edge." << endl;
        cout << "New MST cost: " << originalCost - maxWeight + newW << endl;
    } else {
        cout << "New edge (" << newW << ") is not lighter than max edge (" << maxWeight << ")" << endl;
        cout << "MST remains unchanged." << endl;
        cout << "MST cost: " << originalCost << endl;
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
// 2 4 4