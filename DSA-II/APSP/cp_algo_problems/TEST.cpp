#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <iomanip> // For setprecision

using namespace std;

const int INF = 1e9;
const int MAXN = 105; // Page numbers are 1-100

void solve(int first_u, int first_v, int caseNum) {
    // 1. Initialization
    // dist[i][j] stores the shortest path from i to j
    int dist[MAXN][MAXN];

    // Initialize matrix: 0 for diagonal, INF for others
    for (int i = 0; i < MAXN; i++) {
        for (int j = 0; j < MAXN; j++) {
            if (i == j) dist[i][j] = 0;
            else dist[i][j] = INF;
        }
    }

    // Set to keep track of active page numbers in this test case
    set<int> pages;

    // Process the first edge passed from main
    dist[first_u][first_v] = 1;
    pages.insert(first_u);
    pages.insert(first_v);

    // 2. Read the rest of the edges for this case
    int u, v;
    while (cin >> u >> v && (u != 0 || v != 0)) {
        dist[u][v] = 1;
        pages.insert(u);
        pages.insert(v);
    }

    // 3. Floyd-Warshall Algorithm
    // Iterate through all possible intermediate nodes k
    // (We iterate 1 to 100 because page IDs are in that range)
    for (int k = 1; k <= 100; k++) {
        for (int i = 1; i <= 100; i++) {
            for (int j = 1; j <= 100; j++) {
                // Optimization: Don't try to improve paths involving INF
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }

    // 4. Calculate Average
    double totalPathLength = 0;
    int pairCount = 0;

    // We only iterate over pages that actually exist in our set
    for (int i : pages) {
        for (int j : pages) {
            if (i == j) continue; // Skip self-loops

            // If a path exists, add it to the total
            if (dist[i][j] != INF) {
                totalPathLength += dist[i][j];
                pairCount++;
            }
        }
    }

    double average = 0.0;
    if (pairCount > 0) {
        average = totalPathLength / pairCount;
    }

    // 5. Output
    cout << "Case " << caseNum << ": average length between pages = " 
         << fixed << setprecision(3) << average << " clicks" << endl;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int u, v;
    int caseNum = 1;

    // The input structure is unique:
    // We read the first pair of a case. If it's 0 0, we terminate.
    // Otherwise, we pass that pair to solve() which reads the rest.
    while (cin >> u >> v && (u != 0 || v != 0)) {
        solve(u, v, caseNum++);
    }

    return 0;
    //jhi
}