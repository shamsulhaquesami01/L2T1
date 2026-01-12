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
        vector<string> mapIntToStr(n);
        for (int i = 0; i < n; i++) {
            string s; cin >> s;
            mapStrToInt[s] = i;
            mapIntToStr[i] = s;
        }

        int m; cin >> m;
        vector<vector<double>> dist(n, vector<double>(n, 0.0));
        vector<vector<int>> next_node(n, vector<int>(n, -1));

        for (int i = 0; i < n; i++) {
            dist[i][i] = 1.0;
            next_node[i][i] = i;
        }

        for (int i = 0; i < m; i++) {
            string u_str, v_str;
            double rate;
            cin >> u_str >> rate >> v_str;
            int u = mapStrToInt[u_str];
            int v = mapStrToInt[v_str];
            dist[u][v] = rate;
            next_node[u][v] = v;
        }

        // Floyd-Warshall with Path Reconstruction
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (dist[i][k] * dist[k][j] > dist[i][j]) {
                        dist[i][j] = dist[i][k] * dist[k][j];
                        next_node[i][j] = next_node[i][k]; // Update path pointer
                    }
                }
            }
        }

        // Find Best Profit
        double maxProfit = 1.0;
        int bestStart = -1;

        for (int i = 0; i < n; i++) {
            if (dist[i][i] > maxProfit) {
                maxProfit = dist[i][i];
                bestStart = i;
            }
        }

        if (bestStart != -1) {
            cout << "Best Arbitrage Profit: " << fixed << setprecision(4) << maxProfit << endl;
            cout << "Path: ";
            
            // Reconstruct Cycle
            int curr = bestStart;
            vector<int> path;
            
            // Simple loop detection to avoid infinite printing if logic loops
            for(int k=0; k<=n; k++) { 
                cout << mapIntToStr[curr] << " ";
                curr = next_node[curr][bestStart];
                if (curr == bestStart) break;
            }
            cout << mapIntToStr[bestStart] << endl;
        } else {
            cout << "No Arbitrage exists." << endl;
        }
    }
}

int main() {
    solve();
    return 0;
}