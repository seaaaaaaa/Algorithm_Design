#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <cmath>

using namespace std;

// Global variables to store the board size and target grid
int N;
vector<int> target_grid; 

// Struct to represent a state in the Priority Queue
struct State {
    int f_cost;         // f(n) = g(n) + h(n)
    int g_cost;         // Number of moves so far
    string moves;       // Sequence of moves
    vector<int> board;  // Flattened 1D board
    int blank_r;        // Row index of the empty space (-1)
    int blank_c;        // Column index of the empty space (-1)

    // Min-heap comparator (lower f_cost has higher priority)
    bool operator>(const State& other) const {
        return f_cost > other.f_cost;
    }
};

// Heuristic function: Manhattan Distance
int get_manhattan_distance(const vector<int>& board) {
    int cost = 0;
    
    // Store positions of each color to optimize the search
    // Assuming tile numbers are between 1 and 100 based on the problem description
    vector<pair<int, int>> positions[105];
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            int val = board[r * N + c];
            if (val != -1 && val <= 100) {
                positions[val].push_back({r, c});
            }
        }
    }

    // Calculate distance for the target center (N-2) x (N-2)
    for (int tr = 1; tr < N - 1; ++tr) {
        for (int tc = 1; tc < N - 1; ++tc) {
            int target_val = target_grid[(tr - 1) * (N - 2) + (tc - 1)];
            int min_dist = 1e9; // Infinity
            
            // Find the closest tile of the same color
            for (const auto& p : positions[target_val]) {
                int dist = abs(p.first - tr) + abs(p.second - tc);
                if (dist < min_dist) {
                    min_dist = dist;
                }
            }
            if (min_dist != 1e9) {
                cost += min_dist;
            }
        }
    }
    return cost;
}

// Check if the center of the board matches the target grid
bool is_goal(const vector<int>& board) {
    for (int r = 1; r < N - 1; ++r) {
        for (int c = 1; c < N - 1; ++c) {
            if (board[r * N + c] != target_grid[(r - 1) * (N - 2) + (c - 1)]) {
                return false;
            }
        }
    }
    return true;
}

string solve_rubiks_race(const vector<int>& initial_board) {
    int start_r = -1, start_c = -1;
    
    // Find the starting position of the empty space (-1)
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            if (initial_board[r * N + c] == -1) {
                start_r = r;
                start_c = c;
                break;
            }
        }
    }

    // Priority queue for A* search
    priority_queue<State, vector<State>, greater<State>> pq;
    // Set to keep track of visited states to avoid loops
    set<vector<int>> visited;

    int initial_h = get_manhattan_distance(initial_board);
    pq.push({initial_h, 0, "", initial_board, start_r, start_c});
    visited.insert(initial_board);

    // Movement directions based on problem rules
    // U: Bottom tile moves up -> Empty space (-1) moves DOWN (r+1)
    // D: Top tile moves down -> Empty space (-1) moves UP (r-1)
    // L: Right tile moves left -> Empty space (-1) moves RIGHT (c+1)
    // R: Left tile moves right -> Empty space (-1) moves LEFT (c-1)
    char move_chars[] = {'U', 'D', 'L', 'R'};
    int dr[] = {1, -1, 0, 0};
    int dc[] = {0, 0, 1, -1};

    while (!pq.empty()) {
        State curr = pq.top();
        pq.pop();

        // If goal is reached, append 'S' (Submit) and return
        if (is_goal(curr.board)) {
            return curr.moves + "S";
        }

        for (int i = 0; i < 4; ++i) {
            int new_r = curr.blank_r + dr[i];
            int new_c = curr.blank_c + dc[i];

            // Check boundaries
            if (new_r >= 0 && new_r < N && new_c >= 0 && new_c < N) {
                vector<int> new_board = curr.board;
                // Swap empty space with the target tile
                swap(new_board[curr.blank_r * N + curr.blank_c], new_board[new_r * N + new_c]);

                // If this state hasn't been visited yet
                if (visited.find(new_board) == visited.end()) {
                    visited.insert(new_board);
                    
                    string new_moves = curr.moves + move_chars[i];
                    int g_cost = curr.g_cost + 1;
                    int h_cost = get_manhattan_distance(new_board);
                    int f_cost = g_cost + h_cost;

                    pq.push({f_cost, g_cost, new_moves, new_board, new_r, new_c});
                }
            }
        }
    }

    return "No solution found";
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N)) return 0;

    vector<int> initial_board(N * N);
    for (int i = 0; i < N * N; ++i) {
        cin >> initial_board[i];
    }

    target_grid.resize((N - 2) * (N - 2));
    for (int i = 0; i < (N - 2) * (N - 2); ++i) {
        cin >> target_grid[i];
    }

    string result = solve_rubiks_race(initial_board);
    cout << result << "\n";

    return 0;
}
