/*Given a 2D grid of size n*n, where each cell represents the cost to traverse through that cell, the task is to find the minimum cost to move from the top left cell to the bottom right cell. From a given cell, we can move in 4 directions: left, right, up, down.

Note: It is assumed that negative cost cycles do not exist in input matrix.*/

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
    int rows = grid.size();
    if (rows == 0) return 0;
    int cols = grid[0].size();
    
    // Distance matrix to store min cost to reach each cell
    // Initialize with a large value (Infinity)
    vector<vector<int>> dist(rows, vector<int>(cols, INT_MAX));

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
        if (r == rows - 1 && c == cols - 1) return d;

        // Check all 4 neighbors
        for (int i = 0; i < 4; i++) {
            int nr = r + dx[i];
            int nc = c + dy[i];

            // Check boundary conditions using rows and cols
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                int newDist = d + grid[nr][nc];

                // Relaxation Step
                if (newDist < dist[nr][nc]) {
                    dist[nr][nc] = newDist;
                    pq.push({newDist, nr, nc});
                }
            }
        }
    }

    return dist[rows - 1][cols - 1];
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int m, n;
    // Reading dimensions: m = rows, n = columns
    if (cin >> m >> n) {
        vector<vector<int>> grid(m, vector<int>(n));
        
        // Reading the grid values
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                cin >> grid[i][j];
            }
        }

        // Solve and print
        cout << minimumCostPath(grid) << endl;
    }

    return 0;
}
