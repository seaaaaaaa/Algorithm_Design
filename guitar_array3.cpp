#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

int main() {
    // Optimize standard I/O operations for competitive programming
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    vector<string> grid(N);
    for (int i = 0; i < N; ++i) {
        cin >> grid[i];
    }

    // Extract all empty slots ('.') since X can only be placed on them
    vector<pair<int, int>> dots;
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < M; ++c) {
            if (grid[r][c] == '.') {
                dots.push_back({r, c});
            }
        }
    }

    int num_dots = dots.size();

    // Edge Case: The board has no empty spots
    if (num_dots == 0) {
        for (int r = 0; r < N; ++r) {
            cout << grid[r] << "\n";
        }
        return 0;
    }

    // covers[i] will hold a list of dot IDs that an 'X' at dots[i] would hit.
    vector<vector<int>> covers(num_dots);
    for (int i = 0; i < num_dots; ++i) {
        int r1 = dots[i].first;
        int c1 = dots[i].second;
        for (int j = 0; j < num_dots; ++j) {
            int r2 = dots[j].first;
            int c2 = dots[j].second;
            
            // 1. Horizontal or Vertical (Rook) [cite: 6]
            if (r1 == r2 || c1 == c2) {
                covers[i].push_back(j);
            } 
            // 2. Diagonal (Bishop) [cite: 6]
            else if (abs(r1 - r2) == abs(c1 - c2)) {
                covers[i].push_back(j);
            } 
            // 3. L-Shape (Knight) [cite: 6]
            else if ((abs(r1 - r2) == 1 && abs(c1 - c2) == 2) || 
                     (abs(r1 - r2) == 2 && abs(c1 - c2) == 1)) {
                covers[i].push_back(j);
            }
        }
    }

    // pieces_that_cover[i] holds a list of piece IDs that can cover dot i.
    vector<vector<int>> pieces_that_cover(num_dots);
    for (int i = 0; i < num_dots; ++i) {
        for (int j = 0; j < num_dots; ++j) {
            for(int target : covers[j]) {
                if(target == i) {
                    pieces_that_cover[i].push_back(j);
                    break;
                }
            }
        }
    }

    vector<int> best_solution;
    vector<int> path;
    vector<int> covered_count(num_dots, 0);

    // The Recursive Backtracking Function
    // We added 'last_candidate' to enforce strictly increasing combinations
    auto dfs = [&](auto& self, int pieces_left, int covered_total, int last_candidate) -> bool {
        
        // Base Case: We have successfully covered every dot
        if (covered_total == num_dots) {
            best_solution = path;
            return true;
        }
        
        if (pieces_left == 0) return false;

        // Find the FIRST dot that has a covered count of 0
        int uncovered_idx = -1;
        for (int i = 0; i < num_dots; ++i) {
            if (covered_count[i] == 0) {
                uncovered_idx = i;
                break;
            }
        }

        if (uncovered_idx == -1) return true; 

        // Try placing an 'X' at spots that could possibly cover our missing dot
        for (int candidate : pieces_that_cover[uncovered_idx]) {
            
            // PRUNING MAGIC: Ensure we only check sets in ascending order 
            // If the piece ID is less than or equal to the last placed piece, skip it!
            if (candidate <= last_candidate) continue;
            
            // --- PLACE THE PIECE ---
            int newly_covered = 0;
            for (int target : covers[candidate]) {
                if (covered_count[target] == 0) newly_covered++;
                covered_count[target]++; 
            }
            path.push_back(candidate);
            
            // --- RECURSE DEEPER ---
            // Pass the current 'candidate' as the new 'last_candidate'
            if (self(self, pieces_left - 1, covered_total + newly_covered, candidate)) {
                return true; 
            }
            
            // --- REMOVE THE PIECE (BACKTRACK) ---
            path.pop_back();
            for (int target : covers[candidate]) {
                covered_count[target]--; 
            }
        }
        
        return false; 
    };

    // Iterative Deepening: Try to solve it with 1 piece, then 2 pieces, etc.
    for (int k = 1; k <= num_dots; ++k) {
        // Start the DFS with last_candidate as -1
        if (dfs(dfs, k, 0, -1)) {
            break; 
        }
    }

    // Apply the winning solution back to the 2D grid
    for (int idx : best_solution) {
        int r = dots[idx].first;
        int c = dots[idx].second;
        grid[r][c] = 'X';
    }

    // Output the final grid (leaving '#' exactly as they were!)
    for (int r = 0; r < N; ++r) {
        cout << grid[r] << "\n";
    }

    return 0;
}
