/*

Variation 2: The "Impatient Trader" (Bounded Length)

Concept: Detecting "Negative Cycles" (or here, >1.0 cycles) of a specific length.
 Problem Statement: Arbitrage opportunities disappear quickly. 
 Find if there exists an arbitrage loop that requires exactly K trades (e.g., K=3).
  Loops with K+1 trades are too slow and don't count.

*/



#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

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
        
        // Adjacency Matrix (Rate for 1 step)
        vector<vector<double>> adj(n, vector<double>(n, 0.0));

        for (int i = 0; i < m; i++) {
            string u_str, v_str;
            double rate;
            cin >> u_str >> rate >> v_str;
            int u = mapStrToInt[u_str];
            int v = mapStrToInt[v_str];
            adj[u][v] = max(adj[u][v], rate);
        }

        int K;
        cin >> K; // The required cycle length (e.g., 3)

        // dp[i][j] will store max profit using current number of steps
        // Initially (Step 1), it is just the adjacency matrix
        vector<vector<double>> dp = adj;

        // We iterate K-1 times to extend paths from length 1 to length K
        for (int step = 2; step <= K; step++) {
            vector<vector<double>> next_dp(n, vector<double>(n, 0.0));
            
            // Standard Matrix Multiplication Logic
            // next_dp[i][j] = max(dp[i][x] * adj[x][j])
            for (int i = 0; i < n; i++) {       // Start
                for (int j = 0; j < n; j++) {   // End
                    for (int x = 0; x < n; x++) { // Intermediate
                        if (dp[i][x] > 0.0 && adj[x][j] > 0.0) {
                            next_dp[i][j] = max(next_dp[i][j], dp[i][x] * adj[x][j]);
                        }
                    }
                }
            }
            dp = next_dp; // Update for next iteration
        }

        // After the loop, 'dp' contains max profit for paths of EXACTLY length K
        bool possible = false;
        for (int i = 0; i < n; i++) {
            if (dp[i][i] > 1.0) { // Check diagonal for cycles
                possible = true;
                break;
            }
        }

        if (possible) cout << "Yes (Cycle of length " << K << " exists)" << endl;
        else cout << "No" << endl;
    }
}

int main() {
    solve();
    return 0;
}