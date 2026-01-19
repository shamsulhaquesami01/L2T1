// ... [Insert DSU/Edge Classes] ...

struct MSTEdge { int to, w; };
vector<MSTEdge> tree_adj[1005];
int max_w_on_path;

// DFS to find the max weight edge on the path between u and v in the MST
bool findMaxOnPath(int u, int target, int p, int current_max) {
    if (u == target) {
        max_w_on_path = current_max;
        return true;
    }
    for (auto& edge : tree_adj[u]) {
        if (edge.to != p) { // Don't go back to parent
            if (findMaxOnPath(edge.to, target, u, max(current_max, edge.w))) return true;
        }
    }
    return false;
}

void solveSecondBestMST(int n, vector<Edge>& edges) {
    sort(edges.begin(), edges.end());
    DSU dsu(n);
    vector<Edge> mst_edges;
    vector<Edge> non_mst_edges;
    long long mst_weight = 0;

    // 1. Build MST
    for (auto& edge : edges) {
        if (dsu.find(edge.from) != dsu.find(edge.to)) {
            dsu.unite(edge.from, edge.to);
            mst_weight += edge.w;
            tree_adj[edge.from].push_back({edge.to, edge.w});
            tree_adj[edge.to].push_back({edge.from, edge.w});
            mst_edges.push_back(edge);
        } else {
            non_mst_edges.push_back(edge);
        }
    }

    long long second_best_cost = -1;
    long long min_diff = 1e18; // Stores smallest positive increase

    // 2. Try adding every non-MST edge
    for (auto& edge : non_mst_edges) {
        max_w_on_path = 0;
        findMaxOnPath(edge.from, edge.to, -1, 0);
        
        // Cost change: +new_edge - max_cycle_edge
        // We want strictly second best, so new_edge.w MUST be > max_w_on_path
        // If they are equal, the cost doesn't change (not strictly second best)
        if (edge.w > max_w_on_path) {
            long long diff = edge.w - max_w_on_path;
            if (diff < min_diff) {
                min_diff = diff;
                second_best_cost = mst_weight + diff;
            }
        }
    }

    if (second_best_cost != -1)
        cout << "Strictly Second Best MST Cost: " << second_best_cost << endl;
    else
        cout << "No Strictly Second Best MST exists" << endl;
}