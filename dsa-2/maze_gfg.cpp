#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>

using namespace std;

// Direction arrays for moving Left, Right, Up, Down
const int dx[] = {0, 0, -1, 1};
const int dy[] = {-1, 1, 0, 0};

// Structure to represent the state in Priority Queue
struct Cell {
    int cost;
    int r;
    int c;

    // Operator overloading for Min-Heap (Lowest cost at top)
    bool operator>(const Cell& other) const {
        return cost > other.cost;
    }
};

int minimumCostPath(vector<vector<int>>& grid) {
    int n = grid.size();
    
    // Distance matrix to store min cost to reach each cell
    // Initialize with a large value (Infinity)
    vector<vector<int>> dist(n, vector<int>(n, INT_MAX));

    // Priority Queue: {cost, r, c}
    priority_queue<Cell, vector<Cell>, greater<Cell>> pq;

    // Initial State: Start at (0,0)
    dist[0][0] = grid[0][0];
    pq.push({grid[0][0], 0, 0});

    while (!pq.empty()) {
        Cell current = pq.top();
        pq.pop();

        int d = current.cost;
        int r = current.r;
        int c = current.c;

        // Optimization: If we found a shorter path to this cell already, skip stale entry
        if (d > dist[r][c]) continue;

        // If we reached the bottom-right cell, return the cost
        // (Optional: can just let it run to fill dist matrix, but usually faster to stop)
        if (r == n - 1 && c == n - 1) return d;

        // Check all 4 neighbors
        for (int i = 0; i < 4; i++) {
            int nr = r + dx[i];
            int nc = c + dy[i];

            // Check boundary conditions
            if (nr >= 0 && nr < n && nc >= 0 && nc < n) {
                int newDist = d + grid[nr][nc];

                // Relaxation Step
                if (newDist < dist[nr][nc]) {
                    dist[nr][nc] = newDist;
                    pq.push({newDist, nr, nc});
                }
            }
        }
    }

    return dist[n - 1][n - 1];
}

int main() {
    // Example Input
    vector<vector<int>> grid = {
        {9, 4, 9, 9},
        {6, 7, 6, 4},
        {8, 3, 3, 7},
        {7, 4, 9, 10}
    };

    cout << minimumCostPath(grid) << endl; // Output: 43

    return 0;
}
