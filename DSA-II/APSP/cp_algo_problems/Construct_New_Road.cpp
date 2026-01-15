/*
In this problem you will be given a map of Dhaka in terms of an undirected graph. Each node will
denote a station and edge will represent a road. Nodes will be simple 2-D geometric points and the cost
to travel a road is proportional to the Euclidian distance between two nodes. Now, you will suggest
to construct one road temporarily. You have fixed the criterion to select the two nodes between which
new road will be constructed.
If there is a road between (u, v) then this pair will not be considered.
Otherwise Cuv = Sum(P reCostij− CurCostij ) where CurCostij is the shortest cost path from i
to j if the road between (u, v) exists. And P reCostij is the shortest cost route before constructing the
road between u and v.
The (u, v) pair with maximum Cuv will be selected.
*/


#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

const double INF = 1e18;

// Structure to store coordinates
struct Point {
    double x, y;
};

// Euclidean Distance
double getDist(Point p1, Point p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

void solve() {
    int n, m;
    while (cin >> n >> m && (n != 0 || m != 0)) {
        vector<Point> nodes(n + 1);
        // Reading coordinates
        for (int i = 1; i <= n; i++) {
            cin >> nodes[i].x >> nodes[i].y;
        }

        // Distance matrix (weight if an edge exists)
        vector<vector<double>> geoDist(n + 1, vector<double>(n + 1));
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                geoDist[i][j] = getDist(nodes[i], nodes[j]);
            }
        }

        // Adjacency matrix for Floyd-Warshall
        vector<vector<double>> adj(n + 1, vector<double>(n + 1, INF));
        vector<vector<bool>> edgeExists(n + 1, vector<bool>(n + 1, false));

        for (int i = 1; i <= n; i++) adj[i][i] = 0;

        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            adj[u][v] = geoDist[u][v];
            adj[v][u] = geoDist[u][v];
            edgeExists[u][v] = true;
            edgeExists[v][u] = true;
        }

        // 1. Initial Floyd-Warshall (PreCost)
        vector<vector<double>> preCost = adj;
        for (int k = 1; k <= n; k++) {
            for (int i = 1; i <= n; i++) {
                for (int j = 1; j <= n; j++) {
                    preCost[i][j] = min(preCost[i][j], preCost[i][k] + preCost[k][j]);
                }
            }
        }

        double maxBenefit = 1.0; // Benefit must be > 1.0 to count
        int bestU = -1, bestV = -1;
        double minRoadLength = INF;

        // 2. Iterate all possible new roads
        for (int u = 1; u <= n; u++) {
            for (int v = u + 1; v <= n; v++) {
                if (edgeExists[u][v]) continue; // Skip existing roads

                double currentBenefit = 0;
                double newEdgeWeight = geoDist[u][v];

                // Calculate benefit without running full FW again
                // Check every pair (i, j) to see if u-v helps
                for (int i = 1; i <= n; i++) {
                    for (int j = i + 1; j <= n; j++) {
                        double originalDist = preCost[i][j];
                        
                        // Try going via u-v: i -> u -> v -> j
                        double path1 = preCost[i][u] + newEdgeWeight + preCost[v][j];
                        // Try going via v-u: i -> v -> u -> j
                        double path2 = preCost[i][v] + newEdgeWeight + preCost[u][j];

                        double newDist = min(originalDist, min(path1, path2));
                        currentBenefit += (originalDist - newDist);
                    }
                }

                // 3. Selection Logic (Tie Breaking)
                if (currentBenefit > maxBenefit) {
                    maxBenefit = currentBenefit;
                    bestU = u;
                    bestV = v;
                    minRoadLength = newEdgeWeight;
                } 
                else if (abs(currentBenefit - maxBenefit) < 1e-9) { // Ties
                    // Secondary criteria: shortest new road length
                    if (newEdgeWeight < minRoadLength) {
                        bestU = u;
                        bestV = v;
                        minRoadLength = newEdgeWeight;
                    }
                    // Tertiary criteria: smallest indices
                    // (Implicitly handled by the loop order: u increases, then v increases)
                }
            }
        }

        if (bestU != -1) {
            cout << bestU << " " << bestV << endl;
        } else {
            cout << "No road required" << endl;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}