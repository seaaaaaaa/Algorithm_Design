#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

const int INF = 1e9;
int N;
vector<vector<int>> target_area;

// Movement definitions: U, D, L, R
// U: Tile below moves UP -> Blank moves DOWN (+N)
// D: Tile above moves DOWN -> Blank moves UP (-N)
// L: Tile right moves LEFT -> Blank moves RIGHT (+1)
// R: Tile left moves RIGHT -> Blank moves LEFT (-1)
int dRow[] = {1, -1, 0, 0};
int dCol[] = {0, 0, 1, -1};
char moveChar[] = {'U', 'D', 'L', 'R'};
char inverseMove[] = {'D', 'U', 'R', 'L'}; // To prevent immediate backtracking

// --- HEURISTIC FUNCTION ---
// Calculates misplaced tiles ONLY in the (N-2) x (N-2) target area.
int calculate_heuristic(const vector<int>& board) {
    int h = 0;
    for (int r = 1; r < N - 1; ++r) {
        for (int c = 1; c < N - 1; ++c) {
            int current_color = board[r * N + c];
            int target_color = target_area[r - 1][c - 1];
            if (current_color != target_color) {
                h++; 
            }
        }
    }
    return h;
}

// --- DEBUG FUNCTION ---
void debug_print_board(const vector<int>& board, int g, int h, char last_move) {
    cerr << "\n" << string(g * 2, ' ') << "=== Depth " << g << " | Move: " << last_move << " | Heuristic: " << h << " ===" << "\n";
    for (int i = 0; i < N; ++i) {
        cerr << string(g * 2, ' ');
        for (int j = 0; j < N; ++j) {
            int val = board[i * N + j];
            if (val == -1) cerr << "[  -1] ";
            else cerr << "[" << string(4 - to_string(val).length(), ' ') << val << "] ";
        }
        cerr << "\n";
    }
}

// --- IDA* SEARCH FUNCTION ---
int search(vector<int>& board, int blank_pos, int g, int bound, string& path, char last_move, bool debug_mode) {
    int h = calculate_heuristic(board);
    int f = g + h;

    if (debug_mode && g <= 3) { // Limit deep logging to top 3 levels to avoid gigabytes of text
        debug_print_board(board, g, h, last_move);
        cerr << string(g * 2, ' ') << "f-score: " << f << " (bound: " << bound << ")\n";
    }

    if (f > bound) return f;
    if (h == 0) return -1; // -1 indicates FOUND

    int min_bound = INF;
    int blank_r = blank_pos / N;
    int blank_c = blank_pos % N;

    for (int i = 0; i < 4; ++i) {
        if (last_move == inverseMove[i]) continue; // Prevent moving back and forth

        int new_r = blank_r + dRow[i];
        int new_c = blank_c + dCol[i];

        if (new_r >= 0 && new_r < N && new_c >= 0 && new_c < N) {
            int new_blank_pos = new_r * N + new_c;
            
            // Apply move (Swap blank with target tile)
            swap(board[blank_pos], board[new_blank_pos]);
            path.push_back(moveChar[i]);

            int t = search(board, new_blank_pos, g + 1, bound, path, moveChar[i], debug_mode);
            
            if (t == -1) return -1; // FOUND
            if (t < min_bound) min_bound = t;

            // Undo move (Backtrack)
            path.pop_back();
            swap(board[blank_pos], board[new_blank_pos]);
        }
    }
    return min_bound;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N)) return 0;

    vector<int> initial_board(N * N);
    int blank_pos = -1;

    for (int i = 0; i < N * N; ++i) {
        cin >> initial_board[i];
        if (initial_board[i] == -1) {
            blank_pos = i;
        }
    }

    target_area.assign(N - 2, vector<int>(N - 2));
    for (int i = 0; i < N - 2; ++i) {
        for (int j = 0; j < N - 2; ++j) {
            cin >> target_area[i][j];
        }
    }

    // Toggle this to 'false' if you want to turn off the deep cerr logs
    bool debug_mode = true; 

    if (debug_mode) cerr << ">>> Starting IDA* Search...\n";

    int bound = calculate_heuristic(initial_board);
    string path = "";

    // If it's already solved
    if (bound == 0) {
        cout << "S\n";
        return 0;
    }

    // IDA* Main Loop
    while (true) {
        if (debug_mode) cerr << "\n==============================\n>>> NEW IDA* ITERATION | BOUND: " << bound << "\n==============================\n";
        
        int t = search(initial_board, blank_pos, 0, bound, path, 'X', debug_mode);
        
        if (t == -1) { // Found
            if (debug_mode) cerr << "\n>>> PATH FOUND! Length: " << path.length() << "\n";
            cout << path << "S\n";
            break;
        }
        if (t == INF) {
            if (debug_mode) cerr << ">>> NO SOLUTION EXISTS.\n";
            break; 
        }
        bound = t; // Increase the bound to the minimum f-score that exceeded the previous bound
    }

    return 0;
}