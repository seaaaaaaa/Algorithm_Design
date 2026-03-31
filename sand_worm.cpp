#include <iostream>
#include <vector>
#include <set>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

// Direction arrays to easily move Up, Down, Left, Right
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int R, C, K;
    if (!(cin >> R >> C >> K)) return 0;

    // Read the Northern Hemisphere Map
    vector<vector<char>> gridN(R, vector<char>(C));
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            cin >> gridN[i][j];
        }
    }

    // Read the Southern Hemisphere Map
    vector<vector<char>> gridS(R, vector<char>(C));
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            cin >> gridS[i][j];
        }
    }

    // ==========================================
    // STEP 1: Find all reachable safe cells in the North
    // ==========================================
    vector<vector<bool>> visitedN(R, vector<bool>(C, false));
    queue<pair<int, int>> qN;
    int north_size = 0;

    // Somchai always starts at (1, 1) - which is (0, 0) in 0-based index
    if (gridN[0][0] == '0') {
        qN.push({0, 0});
        visitedN[0][0] = true;
        north_size++;
    }

    while (!qN.empty()) {
        int r = qN.front().first;
        int c = qN.front().second;
        qN.pop();

        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            // Check boundaries and if it's an unvisited safe cell ('0')
            if (nr >= 0 && nr < R && nc >= 0 && nc < C) {
                if (gridN[nr][nc] == '0' && !visitedN[nr][nc]) {
                    visitedN[nr][nc] = true;
                    north_size++;
                    qN.push({nr, nc});
                }
            }
        }
    }

    // ==========================================
    // STEP 2: Precalculate sizes of all safe zones in the South
    // ==========================================
    vector<vector<bool>> visitedS(R, vector<bool>(C, false));
    vector<vector<int>> comp_size(R, vector<int>(C, 0));

    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            // If we find an unvisited safe cell, discover its entire component
            if (gridS[i][j] == '0' && !visitedS[i][j]) {
                queue<pair<int, int>> qS;
                vector<pair<int, int>> comp_cells;

                qS.push({i, j});
                visitedS[i][j] = true;
                comp_cells.push_back({i, j});

                while (!qS.empty()) {
                    int r = qS.front().first;
                    int c = qS.front().second;
                    qS.pop();

                    for (int d = 0; d < 4; d++) {
                        int nr = r + dr[d];
                        int nc = c + dc[d];

                        if (nr >= 0 && nr < R && nc >= 0 && nc < C) {
                            if (gridS[nr][nc] == '0' && !visitedS[nr][nc]) {
                                visitedS[nr][nc] = true;
                                qS.push({nr, nc});
                                comp_cells.push_back({nr, nc});
                            }
                        }
                    }
                }

                // Record the size of this safe zone for all cells inside it
                int current_comp_size = comp_cells.size();
                for (auto& cell : comp_cells) {
                    comp_size[cell.first][cell.second] = current_comp_size;
                }
            }
        }
    }

    // ==========================================
    // STEP 3: Check all K Sand Worms and pick the best one
    // ==========================================
    int max_south = 0;
    for (int i = 0; i < K; i++) {
        int r, c;
        cin >> r >> c;
        
        // Convert input 1-based indices to 0-based indices for our vectors
        r--; 
        c--; 

        // Somchai can only ride this worm if he can reach it in the North!
        if (visitedN[r][c]) {
            if (comp_size[r][c] > max_south) {
                max_south = comp_size[r][c];
            }
        }
    }

    // The answer is the cells he explored in the North + the best component in the South
    cout << north_size + max_south << "\n";

    return 0;
}
