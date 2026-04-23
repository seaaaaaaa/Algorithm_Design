#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <unordered_map>
#include <string>
#include <algorithm>

using namespace std;

int N;
vector<vector<int>> grid;
vector<vector<int>> target;
vector<vector<bool>> locked;

// A* State representation
struct State {
    int tr, tc, er, ec;
    bool operator==(const State& o) const {
        return tr == o.tr && tc == o.tc && er == o.er && ec == o.ec;
    }
};

// Custom hash to pack the 4 coordinates into a 32-bit integer for extremely fast hashing
struct StateHash {
    size_t operator()(const State& s) const {
        return (s.tr << 24) | (s.tc << 16) | (s.er << 8) | s.ec;
    }
};

struct ParentInfo {
    State prev;
    char move;
};

// Directions for Empty Space (E):
// UP -> D, DOWN -> U, LEFT -> R, RIGHT -> L
int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
char move_chars[4] = {'D', 'U', 'R', 'L'};

int heuristic(int tr, int tc, int er, int ec, int dest_r, int dest_c) {
    int dt = abs(tr - dest_r) + abs(tc - dest_c);
    if (dt == 0) return 0;
    int de = abs(er - tr) + abs(ec - tc);
    return dt * 4 + de;
}

char get_move_char(pair<int,int> from, pair<int,int> to) {
    int dr = to.first - from.first;
    int dc = to.second - from.second;
    if (dr == -1) return 'D';
    if (dr == 1) return 'U';
    if (dc == -1) return 'R';
    if (dc == 1) return 'L';
    return ' ';
}

// ---------------------------------------------------------
// 1. A* Pathfinding (For lowest macro-move counts)
// ---------------------------------------------------------
bool a_star_move_tile(int t_start_r, int t_start_c, int e_start_r, int e_start_c, 
                      int dest_r, int dest_c, string& out_path, pair<int,int>& out_e) {
    if (t_start_r == dest_r && t_start_c == dest_c) {
        out_e = {e_start_r, e_start_c};
        return true;
    }

    typedef pair<int, int> pii; // f, g
    typedef pair<pii, State> Node;

    auto comp = [](const Node& a, const Node& b) { return a.first.first > b.first.first; };
    priority_queue<Node, vector<Node>, decltype(comp)> pq(comp);

    unordered_map<State, int, StateHash> best_g;
    unordered_map<State, ParentInfo, StateHash> parent;

    State start_state = {t_start_r, t_start_c, e_start_r, e_start_c};
    best_g[start_state] = 0;
    parent[start_state] = {{-1, -1, -1, -1}, ' '};
    
    pq.push({{heuristic(t_start_r, t_start_c, e_start_r, e_start_c, dest_r, dest_c), 0}, start_state});

    int expansions = 0;
    int max_expansions = 15000; // Cap to prevent stalling

    while (!pq.empty()) {
        auto [fg, state] = pq.top();
        int f = fg.first;
        int g = fg.second;
        pq.pop();

        if (best_g.count(state) && best_g[state] < g) continue;

        if (state.tr == dest_r && state.tc == dest_c) {
            string path = "";
            State curr = state;
            while (parent[curr].prev.tr != -1) {
                path += parent[curr].move;
                curr = parent[curr].prev;
            }
            reverse(path.begin(), path.end());
            out_path = path;
            out_e = {state.er, state.ec};
            return true;
        }

        expansions++;
        if (expansions > max_expansions) return false;

        for (int i = 0; i < 4; ++i) {
            int ner = state.er + dirs[i][0];
            int nec = state.ec + dirs[i][1];
            if (ner >= 0 && ner < N && nec >= 0 && nec < N && !locked[ner][nec]) {
                int ntr = state.tr;
                int ntc = state.tc;
                if (ner == state.tr && nec == state.tc) {
                    ntr = state.er;
                    ntc = state.ec;
                }
                State nstate = {ntr, ntc, ner, nec};
                int ng = g + 1;
                if (!best_g.count(nstate) || ng < best_g[nstate]) {
                    best_g[nstate] = ng;
                    parent[nstate] = {state, move_chars[i]};
                    int nf = ng + heuristic(ntr, ntc, ner, nec, dest_r, dest_c);
                    pq.push({{nf, ng}, nstate});
                }
            }
        }
    }
    return false;
}

// ---------------------------------------------------------
// 2. Guaranteed Fallback Pathfinding
// ---------------------------------------------------------
void fallback_move_tile(pair<int,int> T_start, pair<int,int> dest, pair<int,int> E_start, 
                        string& out_path, pair<int,int>& out_E) {
    out_path = "";
    pair<int,int> curr_T = T_start;
    pair<int,int> curr_E = E_start;

    while (curr_T != dest) {
        // Step A: Find shortest path for Tile to dest
        queue<pair<int,int>> q;
        q.push(curr_T);
        vector<vector<pair<int,int>>> parent_T(N, vector<pair<int,int>>(N, {-2, -2}));
        parent_T[curr_T.first][curr_T.second] = {-1, -1};

        bool found = false;
        while (!q.empty() && !found) {
            auto curr = q.front(); q.pop();
            if (curr == dest) break;
            for (int i = 0; i < 4; i++) {
                int nr = curr.first + dirs[i][0];
                int nc = curr.second + dirs[i][1];
                if (nr >= 0 && nr < N && nc >= 0 && nc < N && !locked[nr][nc]) {
                    if (parent_T[nr][nc].first == -2) {
                        parent_T[nr][nc] = curr;
                        q.push({nr, nc});
                        if (nr == dest.first && nc == dest.second) { found = true; break; }
                    }
                }
            }
        }

        pair<int,int> step = dest;
        while (parent_T[step.first][step.second] != curr_T) {
            step = parent_T[step.first][step.second];
        }
        pair<int,int> next_T = step;

        // Step B: Route Empty space to next_T avoiding curr_T
        locked[curr_T.first][curr_T.second] = true;
        queue<pair<int,int>> qE;
        qE.push(curr_E);
        vector<vector<pair<int,int>>> parent_E(N, vector<pair<int,int>>(N, {-2, -2}));
        parent_E[curr_E.first][curr_E.second] = {-1, -1};

        found = false;
        while (!qE.empty() && !found) {
            auto curr = qE.front(); qE.pop();
            if (curr == next_T) break;
            for(int i = 0; i < 4; i++) {
                int nr = curr.first + dirs[i][0];
                int nc = curr.second + dirs[i][1];
                if (nr >= 0 && nr < N && nc >= 0 && nc < N && !locked[nr][nc]) {
                    if (parent_E[nr][nc].first == -2) {
                        parent_E[nr][nc] = curr;
                        qE.push({nr, nc});
                        if (nr == next_T.first && nc == next_T.second) { found = true; break; }
                    }
                }
            }
        }
        locked[curr_T.first][curr_T.second] = false;

        string path_E = "";
        pair<int,int> curr = next_T;
        while (parent_E[curr.first][curr.second].first != -1) {
            pair<int,int> prev = parent_E[curr.first][curr.second];
            path_E += get_move_char(prev, curr);
            curr = prev;
        }
        reverse(path_E.begin(), path_E.end());
        out_path += path_E;

        // Step C: Swap E and T
        curr_E = next_T;
        out_path += get_move_char(curr_E, curr_T);
        curr_E = curr_T;
        curr_T = next_T;
    }
    out_E = curr_E;
}

// Applies moves to internal grid to maintain accurate state
void apply_moves(const string& moves, pair<int,int>& E_pos) {
    for (char m : moves) {
        int dr = 0, dc = 0;
        if (m == 'D') dr = -1;
        else if (m == 'U') dr = 1;
        else if (m == 'R') dc = -1;
        else if (m == 'L') dc = 1;
        
        int nr = E_pos.first + dr;
        int nc = E_pos.second + dc;
        swap(grid[E_pos.first][E_pos.second], grid[nr][nc]);
        E_pos = {nr, nc};
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N)) return 0;

    grid.assign(N, vector<int>(N, 0));
    locked.assign(N, vector<bool>(N, false));
    pair<int,int> empty_pos;

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            cin >> grid[r][c];
            if (grid[r][c] == -1) {
                empty_pos = {r, c};
            }
        }
    }

    target.assign(N - 2, vector<int>(N - 2, 0));
    for (int r = 0; r < N - 2; r++) {
        for (int c = 0; c < N - 2; c++) {
            cin >> target[r][c];
        }
    }

    string final_moves = "";

    // Process Target from Top to Bottom, Left to Right
    for (int r = 1; r < N - 1; r++) {
        for (int c = 1; c < N - 1; c++) {
            int req_color = target[r - 1][c - 1];

            // Lock if already perfectly placed
            if (grid[r][c] == req_color) {
                locked[r][c] = true;
                continue;
            }

            // Greedy evaluation: Find the best unlocked tile
            pair<int,int> best_cand = {-1, -1};
            int best_score = 1e9;
            for (int ir = 0; ir < N; ir++) {
                for (int ic = 0; ic < N; ic++) {
                    if (!locked[ir][ic] && grid[ir][ic] == req_color) {
                        int score = heuristic(ir, ic, empty_pos.first, empty_pos.second, r, c);
                        if (score < best_score) {
                            best_score = score;
                            best_cand = {ir, ic};
                        }
                    }
                }
            }

            string path = "";
            pair<int,int> new_E;
            
            // Attempt optimal A* Path
            bool success = a_star_move_tile(best_cand.first, best_cand.second, empty_pos.first, empty_pos.second, r, c, path, new_E);
            
            // Fallback to BFS planner if A* takes too long
            if (!success) {
                fallback_move_tile(best_cand, {r, c}, empty_pos, path, new_E);
            }

            apply_moves(path, empty_pos);
            locked[r][c] = true;
            final_moves += path;
        }
    }

    // Must end with S to confirm submission
    final_moves += "S";
    cout << final_moves << "\n";

    return 0;
}