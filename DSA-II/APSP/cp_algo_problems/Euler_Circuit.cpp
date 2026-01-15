#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>

using namespace std;

const int INF = 1e9;
const int MAX_CHARS = 26; // 'a' through 'z'

// Global variables to maintain state across the loop
int dist[MAX_CHARS][MAX_CHARS];
int degree[MAX_CHARS];
int total_weight = 0;

void reset_case() {
    total_weight = 0;
    for (int i = 0; i < MAX_CHARS; i++) {
        degree[i] = 0;
        for (int j = 0; j < MAX_CHARS; j++) {
            if (i == j) dist[i][j] = 0;
            else dist[i][j] = INF;
        }
    }
}

void solve() {
    // 1. Identify nodes with Odd Degrees
    vector<int> odd_nodes;
    for (int i = 0; i < MAX_CHARS; i++) {
        if (degree[i] % 2 != 0) {
            odd_nodes.push_back(i);
        }
    }

    int extra_cost = 0;

    // 2. If there are odd nodes (Problem guarantees 0 or 2)
    if (odd_nodes.size() == 2) {
        int u = odd_nodes[0];
        int v = odd_nodes[1];

        // Run Floyd-Warshall to find shortest path between u and v
        // (Since N is small (26), O(N^3) is instant)
        for (int k = 0; k < MAX_CHARS; k++) {
            for (int i = 0; i < MAX_CHARS; i++) {
                for (int j = 0; j < MAX_CHARS; j++) {
                    if (dist[i][k] != INF && dist[k][j] != INF) {
                        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                    }
                }
            }
        }
        
        // The extra cost is the shortest path to reconnect these two nodes
        extra_cost = dist[u][v];
    }

    cout << total_weight + extra_cost << endl;
}

int main() {
    // Optimize I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    reset_case();

    string s;
    // Read string by string
    while (cin >> s) {
        if (s == "deadend") {
            solve();
            reset_case(); // Prepare for next test case
        } else {
            // Process the street
            int u = s.front() - 'a';
            int v = s.back() - 'a';
            int w = s.length();

            total_weight += w;
            degree[u]++;
            degree[v]++;

            // Update graph weights
            // Note: Keep the smallest weight if multiple streets connect same nodes
            // (Though problem implies at most one direct connection)
            dist[u][v] = min(dist[u][v], w);
            dist[v][u] = min(dist[v][u], w);
        }
    }

    return 0;
}