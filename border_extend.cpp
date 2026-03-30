#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// Direction arrays for moving up, down, left, and right
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

int main() {
    // Optimize standard I/O operations for faster execution
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int R, C, k;
    if (!(cin >> R >> C >> k)) return 0; // Read dimensions and max distance

    vector<vector<char> > grid(R, vector<char>(C));
    queue<pair<int, int> > q; 
    vector<vector<int> > dist(R, vector<int>(C, -1));

    // Read the grid character by character (this safely ignores spaces/newlines in input)
    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < C; ++j) {
            cin >> grid[i][j]; 
            if (grid[i][j] == '1') {
                q.push(make_pair(i, j)); // Push initial land cells into the queue
                dist[i][j] = 0; // Distance to itself is 0
            }
        }
    }

    // Perform Multi-Source BFS
    while (!q.empty()) {
        int r = q.front().first;
        int c = q.front().second;
        q.pop();

        // Stop extending if the distance reaches the limit k
        if (dist[r][c] == k) continue; 

        // Explore the 4 adjacent neighbors
        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            // Check if the neighbor is within grid boundaries
            if (nr >= 0 && nr < R && nc >= 0 && nc < C) {
                // If it is water ('0') and has not been visited yet
                if (grid[nr][nc] == '0' && dist[nr][nc] == -1) {
                    dist[nr][nc] = dist[r][c] + 1;
                    grid[nr][nc] = '2'; // Mark as extended land
                    q.push(make_pair(nr, nc)); // Push the new land cell into the queue
                }
            }
        }
    }

    // Output the final modified grid with spaces between numbers
    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < C; ++j) {
            cout << grid[i][j];
            // Print a space after every character except the last one in the row
            if (j < C - 1) {
                cout << " ";
            }
        }
        cout << "\n"; // Significantly faster output
    }

    return 0;
}
