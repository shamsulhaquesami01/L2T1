#include <iostream>
#include <vector>
#include <tuple>
#include <set>
#include <algorithm>

using namespace std;

typedef long long ll;

class NegativeCycleCounter {
private:
    int n;
    vector<tuple<int, int, ll>> edges;
    vector<vector<pair<int, ll>>> adj;
    set<set<int>> cycles; // Store unique cycles as sets
    
    // Extract cycle starting from a node known to be in a cycle
    set<int> extractCycle(int start_node, vector<int>& parent) {
        // Trace back n steps to ensure we're in the cycle
        int curr = start_node;
        for (int i = 0; i < n; i++) {
            curr = parent[curr];
        }
        
        // Now curr is definitely in a cycle
        // Extract all nodes in this cycle
        set<int> cycle;
        int cycle_start = curr;
        do {
            cycle.insert(curr);
            curr = parent[curr];
        } while (curr != cycle_start);
        
        return cycle;
    }
    
    // Check if a set of nodes forms a negative cycle
    bool isNegativeCycle(const set<int>& nodes) {
        // Calculate total weight of cycle
        ll total_weight = 0;
        
        for (int u : nodes) {
            for (auto& [v, w] : adj[u]) {
                if (nodes.count(v)) {
                    total_weight += w;
                    break; // Only one edge per node in simple cycle
                }
            }
        }
        
        return total_weight < 0;
    }
    
public:
    NegativeCycleCounter(int n) : n(n), adj(n + 1) {}
    
    void addEdge(int a, int b, ll c) {
        edges.push_back({a, b, c});
        adj[a].push_back({b, c});
    }
    
    int countNegativeCycles() {
        // Run Bellman-Ford and collect all nodes that get updated in nth iteration
        vector<ll> dist(n + 1, 0);
        vector<int> parent(n + 1, -1);
        
        // Standard n-1 iterations
        for (int i = 0; i < n - 1; i++) {
            for (auto& [u, v, w] : edges) {
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    parent[v] = u;
                }
            }
        }
        
        // Collect all nodes that can still be relaxed (in negative cycles)
        set<int> cycle_nodes;
        for (int i = 0; i < n; i++) {
            for (auto& [u, v, w] : edges) {
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    parent[v] = u;
                    cycle_nodes.insert(v);
                }
            }
        }
        
        if (cycle_nodes.empty()) return 0;
        
        // For each node in a negative cycle, extract the cycle
        set<int> processed;
        
        for (int node : cycle_nodes) {
            if (processed.count(node)) continue;
            
            set<int> cycle = extractCycle(node, parent);
            
            // Mark all nodes in this cycle as processed
            for (int v : cycle) {
                processed.insert(v);
            }
            
            // Add this unique cycle
            cycles.insert(cycle);
        }
        
        return cycles.size();
    }
    
    void printCycles() {
        cout << "Total negative cycles: " << cycles.size() << endl;
        int idx = 1;
        for (const auto& cycle : cycles) {
            cout << "Cycle " << idx++ << ": ";
            for (int node : cycle) {
                cout << node << " ";
            }
            cout << endl;
        }
    }
};

int main() {
    int n, m;
    cin >> n >> m;
    
    NegativeCycleCounter counter(n);
    
    for (int i = 0; i < m; i++) {
        int a, b;
        ll c;
        cin >> a >> b >> c;
        counter.addEdge(a, b, c);
    }
    
    int count = counter.countNegativeCycles();
    
    if (count == 0) {
        cout << "No negative cycles found" << endl;
    } else {
        counter.printCycles();
    }
    
    return 0;
}

/* 
Test Input 1: (2 separate negative cycles)
8 10
1 2 3
2 3 4
1 4 2
2 4 5
4 5 -10
5 2 3
6 7 1
7 8 1
8 6 -4
3 1 1

Expected: 2 cycles
Cycle 1: 2 4 5
Cycle 2: 6 7 8

Test Input 2: (1 negative cycle)
6 7
1 2 3
2 3 4
1 4 2
2 4 5
4 5 -10
6 3 -2
5 2 3

Expected: 1 cycle
Cycle 1: 2 4 5

Test Input 3: (No negative cycle)
4 4
1 2 1
2 3 1
3 4 1
4 1 1

Expected: 0 cycles
*/