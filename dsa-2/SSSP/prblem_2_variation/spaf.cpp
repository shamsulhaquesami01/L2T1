#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

typedef long long ll;

int main() {
    int n, m, src;
    cin >> n >> m >> src;
    
    vector<vector<pair<int, ll>>> adj(n + 1);
    
    for (int i = 0; i < m; i++) {
        int a, b;
        ll c;
        cin >> a >> b >> c;
        adj[a].push_back({b, c});
    }
    
    vector<ll> dist(n + 1, LLONG_MAX);
    vector<int> cnt(n + 1, 0); // Count of relaxations
    vector<int> parent(n + 1, -1);
    vector<bool> in_queue(n + 1, false);
    queue<int> q;
    
    dist[src] = 0;
    q.push(src);
    in_queue[src] = true;
    
    int cycle_node = -1;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        in_queue[u] = false;
        
        for (auto &[v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                
                if (!in_queue[v]) {
                    q.push(v);
                    in_queue[v] = true;
                    cnt[v]++;
                    
                    // If a node is relaxed n times, negative cycle exists
                    if (cnt[v] >= n) {
                        cycle_node = v;
                        break;
                    }
                }
            }
        }
        
        if (cycle_node != -1) break;
    }
    
    if (cycle_node == -1) {
        cout << "No negative cycle" << endl;
        cout << "Distances from source " << src << ":" << endl;
        for (int i = 1; i <= n; i++) {
            if (dist[i] == LLONG_MAX) {
                cout << i << ": INF" << endl;
            } else {
                cout << i << ": " << dist[i] << endl;
            }
        }
    } else {
        cout << "Negative cycle detected!" << endl;
        
        // Trace back to find cycle
        set<int> visited;
        int curr = cycle_node;
        while (visited.find(curr) == visited.end()) {
            visited.insert(curr);
            curr = parent[curr];
        }
        
        // curr is now in the cycle
        vector<int> cycle;
        int start = curr;
        do {
            cycle.push_back(curr);
            curr = parent[curr];
        } while (curr != start);
        cycle.push_back(start);
        
        reverse(cycle.begin(), cycle.end());
        
        cout << "Cycle: ";
        for (int node : cycle) {
            cout << node << " ";
        }
        cout << endl;
    }
    
    return 0;
}
