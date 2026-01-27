//sticker matching

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>

using namespace std;

// --- STANDARD MAX FLOW TEMPLATE ---
struct Edge {
    int to;
    int capacity;
    int flow;
    int rev;
};

class MaxFlow {
public:
    int N;
    vector<vector<Edge>> graph;

    MaxFlow(int n) : N(n), graph(n) {}

    void addEdge(int u, int v, int cap) {
        Edge a = {v, cap, 0, (int)graph[v].size()};
        Edge b = {u, 0, 0, (int)graph[u].size()};
        graph[u].push_back(a);
        graph[v].push_back(b);
    }

    int bfs(int s, int t, vector<pair<int,int>>& parent) {
        fill(parent.begin(), parent.end(), make_pair(-1, -1));
        queue<int> q;
        q.push(s);
        parent[s] = {s, -1};

        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int i = 0; i < graph[u].size(); i++) {
                Edge &e = graph[u][i];
                if (parent[e.to].first == -1 && e.capacity > e.flow) {
                    parent[e.to] = {u, i};
                    if (e.to == t) return 1;
                    q.push(e.to);
                }
            }
        }
        return 0;
    }

    int edmondsKarp(int s, int t) {
        int maxFlow = 0;
        vector<pair<int,int>> parent(N);
        while (bfs(s, t, parent)) {
            int pathFlow = 1e9;
            for (int v = t; v != s; ) {
                auto [u, idx] = parent[v];
                pathFlow = min(pathFlow, graph[u][idx].capacity - graph[u][idx].flow);
                v = u;
            }
            for (int v = t; v != s; ) {
                auto [u, idx] = parent[v];
                graph[u][idx].flow += pathFlow;
                int rev_idx = graph[u][idx].rev;
                graph[v][rev_idx].flow -= pathFlow;
                v = u;
            }
            maxFlow += pathFlow;
        }
        return maxFlow;
    }
};

struct Trade {
    int friend_idx;
    int give_sticker;
    int get_sticker;
};

void solve() {
    int n, m; // n friends, m sticker types
    if (!(cin >> n >> m)) return;

    // 1. Parse "My" Stickers
    int my_k; cin >> my_k;
    map<int, int> my_counts;
    int my_initial_distinct = 0;
    for(int i=0; i<my_k; i++) {
        int s; cin >> s;
        if (my_counts[s] == 0) my_initial_distinct++;
        my_counts[s]++;
    }

    // 2. Parse Friends' Stickers
    vector<map<int, int>> friend_counts(n);
    for(int i=0; i<n; i++) {
        int k; cin >> k;
        for(int j=0; j<k; j++) {
            int s; cin >> s;
            friend_counts[i][s]++;
        }
    }

    // Nodes: S=0, Stickers=1..m, Friends=m+1..m+n, T=m+n+1
    int S = 0;
    int T = m + n + 1;
    MaxFlow mf(T + 1);

    // 3. Build Graph

    // S -> Sticker (My Duplicates)
    for(int s=1; s<=m; s++) {
        if(my_counts[s] > 1) {
            mf.addEdge(S, s, my_counts[s] - 1);
        }
    }

    // Sticker -> T (My Needs)
    for(int s=1; s<=m; s++) {
        if(my_counts[s] == 0) {
            mf.addEdge(s, T, 1);
        }
    }

    // Interactions with Friends
    for(int i=0; i<n; i++) {
        int friend_node = m + 1 + i;
        
        // Sticker -> Friend (Friend Needs)
        for(int s=1; s<=m; s++) {
            if(friend_counts[i][s] == 0) {
                // Capacity 1 because friend only accepts one copy of what they don't have
                mf.addEdge(s, friend_node, 1);
            }
        }

        // Friend -> Sticker (Friend Offers)
        for(int s=1; s<=m; s++) {
            if(friend_counts[i][s] > 1) {
                // Capacity is number of duplicates friend has
                mf.addEdge(friend_node, s, friend_counts[i][s] - 1);
            }
        }
    }

    // 4. Calculate Max Flow
    int new_stickers = mf.edmondsKarp(S, T);
    cout << (my_initial_distinct + new_stickers) << endl;

    // 5. Decompose Flow into Trades
    vector<Trade> pending_trades;
    
    for(int i=0; i<n; i++) {
        int friend_node = m + 1 + i;
        vector<int> give_list; // What I give to friend (Sticker -> Friend)
        vector<int> get_list;  // What I get from friend (Friend -> Sticker)

        // Find incoming flow (Sticker -> Friend)
        // We need to look at edges in graph[sticker] pointing to friend
        // Or look at graph[friend] reverse edges? 
        // Better: iterate stickers
        for(int s=1; s<=m; s++) {
            // Find edge s -> friend_node
            for(const auto& e : mf.graph[s]) {
                if(e.to == friend_node && e.flow == 1) { // Forward edge with flow
                    give_list.push_back(s);
                }
            }
        }

        // Find outgoing flow (Friend -> Sticker)
        for(const auto& e : mf.graph[friend_node]) {
            if(e.to >= 1 && e.to <= m && e.flow > 0) { // Forward edge with flow
                for(int k=0; k<e.flow; k++) { // If flow > 1, add multiple times
                    get_list.push_back(e.to);
                }
            }
        }

        // Pair them up
        for(size_t k=0; k<give_list.size(); k++) {
            pending_trades.push_back({i + 1, give_list[k], get_list[k]});
        }
    }

    // 6. Simulate Trades to Print in Valid Order
    // We execute a trade only if we currently have >1 of the 'give' sticker
    vector<bool> executed(pending_trades.size(), false);
    int executed_count = 0;

    while(executed_count < pending_trades.size()) {
        bool progress = false;
        for(size_t i=0; i<pending_trades.size(); i++) {
            if(!executed[i]) {
                Trade& t = pending_trades[i];
                if(my_counts[t.give_sticker] > 1) {
                    // Valid trade
                    cout << "Exchange a sticker with number " << t.give_sticker 
                         << " against a sticker with number " << t.get_sticker 
                         << " of friend " << t.friend_idx << endl;
                    
                    my_counts[t.give_sticker]--;
                    my_counts[t.get_sticker]++;
                    executed[i] = true;
                    executed_count++;
                    progress = true;
                }
            }
        }
        if(!progress) break; // Should not happen given valid max flow
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}