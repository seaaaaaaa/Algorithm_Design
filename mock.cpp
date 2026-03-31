#include <iostream>
#include <vector>
#include <queue>
#include <string>

using namespace std;

const int INF = 1e9; // A very large number representing infinity

// Direction arrays for walking up, down, left, and right
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

int main() {
    // Optimize standard I/O operations for faster execution
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0; // Read dimensions

    vector<string> grid(N);
    int start_r = -1, start_c = -1;
    int target_r = -1, target_c = -1;

    // Read the grid and locate the Goat (S) and Horse (T)
    for (int i = 0; i < N; ++i) {
        cin >> grid[i];
        for (int j = 0; j < M; ++j) {
            if (grid[i][j] == 'S') {
                start_r = i;
                start_c = j;
            } else if (grid[i][j] == 'T') {
                target_r = i;
                target_c = j;
            }
        }
    }

    // dist array tracks the minimum time to reach each cell
    vector<vector<int> > dist(N, vector<int>(M, INF));
    queue<pair<int, int> > q;
    
    // Track which rows and columns have already been blasted by a rocket
    vector<bool> row_activated(N, false);
    vector<bool> col_activated(M, false);

    // Initialize the starting position
    q.push(make_pair(start_r, start_c));
    dist[start_r][start_c] = 0;

    while (!q.empty()) {
        int r = q.front().first;
        int c = q.front().second;
        q.pop();

        // If we reached the Horse, we can output the answer immediately
        if (r == target_r && c == target_c) {
            cout << dist[r][c] << "\n";
            return 0;
        }

        // 1. Normal Walking (Check 4 adjacent neighbors)
        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            // If within bounds and not an obstacle ('X')
            if (nr >= 0 && nr < N && nc >= 0 && nc < M && grid[nr][nc] != 'X') {
                if (dist[nr][nc] > dist[r][c] + 1) {
                    dist[nr][nc] = dist[r][c] + 1;
                    q.push(make_pair(nr, nc));
                }
            }
        }

        // 2. Rocket Jump (If we are standing on a rocket pad '#')
        if (grid[r][c] == '#') {
            
            // Have we used a rocket in this row yet? If not, fire it!
            if (!row_activated[r]) {
                row_activated[r] = true;
                for (int nc = 0; nc < M; ++nc) {
                    // Rockets can fly over 'X', but cannot land on them
                    if (grid[r][nc] != 'X' && dist[r][nc] > dist[r][c] + 1) {
                        dist[r][nc] = dist[r][c] + 1;
                        q.push(make_pair(r, nc));
                    }
                }
            }
            
            // Have we used a rocket in this column yet? If not, fire it!
            if (!col_activated[c]) {
                col_activated[c] = true;
                for (int nr = 0; nr < N; ++nr) {
                    // Rockets can fly over 'X', but cannot land on them
                    if (grid[nr][c] != 'X' && dist[nr][c] > dist[r][c] + 1) {
                        dist[nr][c] = dist[r][c] + 1;
                        q.push(make_pair(nr, c));
                    }
                }
            }
            
        }
    }

    return 0;
}
