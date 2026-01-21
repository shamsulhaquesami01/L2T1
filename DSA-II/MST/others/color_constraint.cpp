#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;

// 0 = Red, 1 = Blue
struct Edge {
    int u, v;
    int w;
    int color; // 1 for Blue, 0 for Red
    int id;    // Original index to print solution
};

// DSU Class (Standard)
struct DSU {
    vector<int> parent;
    DSU(int n) {
        parent.resize(n + 1);
        for(int i=0; i<=n; i++) parent[i] = i;
    }
    int find(int i) {
        if(parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }
    bool unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if(root_i != root_j) {
            parent[root_i] = root_j;
            return true;
        }
        return false;
    }
};

int n, m, K;
vector<Edge> all_edges;
vector<pair<int,int>> best_mst_edges; // To store the result path

// Returns {Total Cost (with penalty), Number of Blue Edges Used}
pair<long long, int> runKruskal(int penalty) {
    DSU dsu(n);
    long long current_weight = 0;
    int blue_count = 0;
    best_mst_edges.clear();

    // 1. Create a temporary list of edges with modified weights
    vector<pair<int, Edge*>> sorted_edges;
    for(auto& e : all_edges) {
        int effective_weight = e.w;
        if(e.color == 1) effective_weight += penalty;
        sorted_edges.push_back({effective_weight, &e});
    }
    sort(sorted_edges.begin(), sorted_edges.end(), [](const auto& a, const auto& b) {
        if(a.first != b.first) return a.first < b.first;
        return a.second->color < b.second->color; // Pick Red (0) before Blue (1) if tie
    });

    // 3. Build MST
    int edges_count = 0;
    for(auto& p : sorted_edges) {
        int weight = p.first;
        Edge* e = p.second;
        
        if(dsu.unite(e->u, e->v)) {
            current_weight += weight;
            if(e->color == 1) blue_count++;
            best_mst_edges.push_back({e->u, e->v});
            edges_count++;
        }
    }

    if(edges_count < n - 1) return {-1, -1}; // Disconnected
    return {current_weight, blue_count};
}

void solve() {
    cin >> n >> m >> K; // K is Max Blue Edges allowed
    for(int i = 0; i < m; i++) {
        int u, v, w, c;
        cin >> u >> v >> w >> c; // c: 1=Blue, 0=Red
        all_edges.push_back({u, v, w, c, i});
    }
    int low = -100005, high = 100005;
    long long final_ans = -1;
    int final_penalty = 0;

    while(low <= high) {
        int mid = low + (high - low) / 2;
        
        pair<long long, int> result = runKruskal(mid);
        
        if(result.second == -1) {
            // Graph disconnected even with best efforts?
            // Should not happen unless graph is disconnected initially
            cout << -1 << endl; 
            return;
        }

        if(result.second <= K) {
 
            
            final_ans = result.first - (1LL * mid * result.second);
            final_penalty = mid;
            
            // To get closer to exactly K (if needed) or just minimize, 
            // we usually try to lower the penalty to see if we can still hold constraint?
            // If we have 3 blues and limit is 5, lowering penalty might give us 4 blues (cheaper).
            high = mid - 1; 
        } else {
            // Too many Blue edges! Need higher penalty to discourage them.
            low = mid + 1;
        }
    }

    if(final_ans != -1) {.
        
        cout << "Minimum Cost with at most " << K << " Blue edges: " << final_ans << endl;
        // Print edges if needed...
    } else {
        cout << "Impossible" << endl;
    }
}

int main() {
    solve();
    return 0;
}