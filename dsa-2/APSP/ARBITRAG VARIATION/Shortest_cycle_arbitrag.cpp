#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iomanip>

using namespace std;

void solve() {
    int n;
    while (cin >> n) {
        map<string, int> mapStrToInt;
        for (int i = 0; i < n; i++) {
            string s; cin >> s;
            mapStrToInt[s] = i;
        }

        int m; cin >> m;
        // adj[i][j] stores the direct exchange rate from i to j
        vector<vector<double>> adj(n, vector<double>(n, 0.0));
        
        for (int i = 0; i < m; i++) {
            string u_str, v_str;
            double rate;
            cin >> u_str >> rate >> v_str;
            int u = mapStrToInt[u_str];
            int v = mapStrToInt[v_str];
            // If multiple rates exist, keep the best one
            adj[u][v] = max(adj[u][v], rate);
        }

        // dp[i][j] stores the max profit from i to j using 'steps' number of trades
        // Initially (step 1), it's just the direct adjacency matrix
        vector<vector<double>> dp = adj;

        int found_steps = -1;
        int found_node = -1;

        // Try cycle lengths from 2 up to N
        for (int steps = 2; steps <= n; steps++) {
            vector<vector<double>> next_dp(n, vector<double>(n, 0.0));
            bool cycle_found = false;

            for (int i = 0; i < n; i++) {       // Start Node
                for (int j = 0; j < n; j++) {   // End Node
                    for (int k = 0; k < n; k++) { // Intermediate Node
                        // Logic: Path(i->k) of length (steps-1) + Edge(k->j)
                        if (dp[i][k] > 0.0 && adj[k][j] > 0.0) {
                            next_dp[i][j] = max(next_dp[i][j], dp[i][k] * adj[k][j]);
                        }
                    }
                }
            }

            // Check if any node has profited from a cycle of this specific length
            for (int i = 0; i < n; i++) {
                if (next_dp[i][i] > 1.0) {
                    cycle_found = true;
                    found_node = i;
                    break;
                }
            }

            if (cycle_found) {
                found_steps = steps;
                break; 
            }

            // Prepare for next iteration
            dp = next_dp;
        }

        if (found_steps != -1) {
            cout << "Smallest Arbitrage Cycle Length: " << found_steps << " trades." << endl;
            // Note: Printing the specific path for this method requires storing 
            // parent pointers for every step layer (parent[step][node]), which uses O(N^3) memory.
        } else {
            cout << "No Arbitrage exists." << endl;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}