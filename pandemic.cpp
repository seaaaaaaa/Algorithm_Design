#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// Structure to hold grid coordinates and the day the cell was infected
struct Cell {
    int r, c, day;
};

int main() {
    // Optimize standard I/O operations for competitive programming
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int r, c, T;
    if (!(cin >> r >> c >> T)) return 0;

    vector<vector<int>> grid(r, vector<int>(c));
    queue<Cell> q;
    
    int infected_count = 0;

    // Read the grid and initialize the BFS queue with day 0 infections
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            cin >> grid[i][j];
            if (grid[i][j] == 1) {
                q.push({i, j, 0});
                infected_count++;
            }
        }
    }

    // Direction arrays for moving up, down, left, right
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    // Process the spread of the virus using BFS
    while (!q.empty()) {
        Cell current = q.front();
        q.pop();

        // If the current infected cell was infected on day T, it cannot spread further within the time limit
        if (current.day >= T) {
            continue; 
        }

        // Check all 4 adjacent neighbors
        for (int i = 0; i < 4; ++i) {
            int nr = current.r + dr[i];
            int nc = current.c + dc[i];

            // Check if neighbor is within bounds and is an uninfected, non-mountain cell
            if (nr >= 0 && nr < r && nc >= 0 && nc < c) {
                if (grid[nr][nc] == 0) {
                    grid[nr][nc] = 1; // Mark as infected to prevent revisiting
                    infected_count++;
                    q.push({nr, nc, current.day + 1});
                }
            }
        }
    }

    // Output the final count of infected provinces
    cout << infected_count << "\n";

    return 0;
}
