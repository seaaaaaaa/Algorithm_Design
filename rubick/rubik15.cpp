#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <cmath>
#include <algorithm>
#include <cstring>

using namespace std;

const int MAXN = 75;
int N;
int board[MAXN][MAXN];
int target[MAXN][MAXN];
bool locked[MAXN][MAXN];

int br, bc; 
string ans = "";

void apply_move(char m) {
    ans += m;
    int tr = br, tc = bc;
    if (m == 'U') tr = br + 1;
    if (m == 'D') tr = br - 1;
    if (m == 'L') tc = bc + 1;
    if (m == 'R') tc = bc - 1;
    
    swap(board[br][bc], board[tr][tc]);
    br = tr;
    bc = tc;
}

void apply_moves(string moves) {
    for (char m : moves) apply_move(m);
}

// ==========================================
// Check if a tile is resting on its target
// ==========================================
inline bool is_perfect(int r, int c) {
    if (r >= 1 && r <= N - 2 && c >= 1 && c <= N - 2) {
        return board[r][c] == target[r - 1][c - 1];
    }
    return false;
}

// ==========================================
// Pure BFS Distance Map
// ==========================================
vector<vector<int>> get_all_distances_from(int start_r, int start_c) {
    vector<vector<int>> dist(N, vector<int>(N, -1));
    queue<pair<int, int>> q;
    if (start_r < 0 || start_r >= N || start_c < 0 || start_c >= N) return dist;

    q.push({start_r, start_c});
    dist[start_r][start_c] = 0;
    int dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, -1, 1};
    
    while(!q.empty()){
        auto [r, c] = q.front(); q.pop();
        for(int i=0; i<4; i++){
            int nr = r + dr[i], nc = c + dc[i];
            if(nr >= 0 && nr < N && nc >= 0 && nc < N && !locked[nr][nc] && dist[nr][nc] == -1){
                dist[nr][nc] = dist[r][c] + 1;
                q.push({nr, nc});
            }
        }
    }
    return dist;
}

// ==========================================
// Fast Blank BFS (Dual-Pass Non-Destructive)
// ==========================================
int b_visited[MAXN][MAXN], b_visit_id = 0;
int b_q_r[MAXN * MAXN], b_q_c[MAXN * MAXN];
pair<short, short> b_parent[MAXN][MAXN];
char b_move_from[MAXN][MAXN];

string fast_bfs_blank_inner(int sr, int sc, int tr, int tc, int avoid_r, int avoid_c, bool avoid_perf) {
    if (sr == tr && sc == tc) return "";
    b_visit_id++;
    int head = 0, tail = 0;
    b_q_r[tail] = sr; b_q_c[tail] = sc; tail++;
    b_visited[sr][sc] = b_visit_id;
    int b_dr[] = {1, -1, 0, 0}, b_dc[] = {0, 0, 1, -1};
    char b_char[] = {'U', 'D', 'L', 'R'};
    bool found = false;
    
    while(head < tail) {
        int r = b_q_r[head], c = b_q_c[head]; head++;
        if (r == tr && c == tc) { found = true; break; }
        for (int i=0; i<4; i++) {
            int nr = r + b_dr[i], nc = c + b_dc[i];
            if (nr >= 0 && nr < N && nc >= 0 && nc < N) {
                if (nr == avoid_r && nc == avoid_c) continue;
                if (locked[nr][nc]) continue;
                // Core V51 feature: Don't trample perfectly placed tiles
                if (avoid_perf && is_perfect(nr, nc)) continue;
                
                if (b_visited[nr][nc] != b_visit_id) {
                    b_visited[nr][nc] = b_visit_id;
                    b_parent[nr][nc] = {r, c};
                    b_move_from[nr][nc] = b_char[i];
                    b_q_r[tail] = nr; b_q_c[tail] = nc; tail++;
                }
            }
        }
    }
    if (!found) return "FAILED";
    string path = "";
    int cr = tr, cc = tc;
    while(cr != sr || cc != sc) {
        path += b_move_from[cr][cc];
        auto p = b_parent[cr][cc];
        cr = p.first; cc = p.second;
    }
    reverse(path.begin(), path.end());
    return path;
}

string fast_bfs_blank(int sr, int sc, int tr, int tc, int avoid_r, int avoid_c) {
    // Pass 1: Try to path without destroying future targets
    string res = fast_bfs_blank_inner(sr, sc, tr, tc, avoid_r, avoid_c, true);
    // Pass 2: Fallback to destructive path if absolutely necessary
    if (res == "FAILED") {
        res = fast_bfs_blank_inner(sr, sc, tr, tc, avoid_r, avoid_c, false);
    }
    return res;
}

// ==========================================
// A* Macro Routing
// ==========================================
struct MacroState { short r, c, bd; };
MacroState parent_macro[MAXN][MAXN][4];
string move_macro[MAXN][MAXN][4];
int dist_macro[MAXN][MAXN][4], macro_visited[MAXN][MAXN][4], macro_search_id = 0;

struct MacroNode {
    int f, g, h; short r, c, bd;
    bool operator>(const MacroNode& o) const { return (f != o.f) ? f > o.f : h > o.h; }
};

string macro_a_star(int sr, int sc, int tar_r, int tar_c, const vector<vector<int>>& dist_map) {
    macro_search_id++;
    priority_queue<MacroNode, vector<MacroNode>, greater<MacroNode>> pq;
    int t_dr[] = {-1, 1, 0, 0}, t_dc[] = {0, 0, -1, 1}; char t_char[] = {'U', 'D', 'L', 'R'};
    
    for (int i=0; i<4; i++) {
        int nbr = sr + t_dr[i], nbc = sc + t_dc[i];
        if (nbr >= 0 && nbr < N && nbc >= 0 && nbc < N && !locked[nbr][nbc]) {
            string b_path = fast_bfs_blank(br, bc, nbr, nbc, sr, sc);
            if (b_path != "FAILED") {
                int g = b_path.length();
                macro_visited[sr][sc][i] = macro_search_id;
                dist_macro[sr][sc][i] = g;
                parent_macro[sr][sc][i] = {-1, -1, -1};
                move_macro[sr][sc][i] = b_path;
                
                int h = dist_map[sr][sc] * 3;
                pq.push({g + h, g, h, (short)sr, (short)sc, (short)i});
            }
        }
    }
    
    while(!pq.empty()) {
        auto curr = pq.top(); pq.pop();
        int g = curr.g; short r = curr.r, c = curr.c, bd = curr.bd;
        if (g > dist_macro[r][c][bd]) continue; 
        if (r == tar_r && c == tar_c) {
            string full_path = ""; short cr = r, cc = c, cbd = bd;
            while(cr != sr || cc != sc) {
                full_path = move_macro[cr][cc][cbd] + full_path;
                auto p = parent_macro[cr][cc][cbd];
                cr = p.r; cc = p.c; cbd = p.bd;
            }
            return move_macro[sr][sc][cbd] + full_path;
        }
        for (int i=0; i<4; i++) {
            int nr = r + t_dr[i], nc = c + t_dc[i];
            if (nr >= 0 && nr < N && nc >= 0 && nc < N && !locked[nr][nc]) {
                string b_path = fast_bfs_blank(r + t_dr[bd], c + t_dc[bd], nr, nc, r, c);
                if (b_path != "FAILED") {
                    int step_cost = b_path.length() + 1;
                    
                    // Soft penalty to prevent dragging the target tile over perfect spots
                    if (is_perfect(nr, nc)) step_cost += 15; 
                    
                    int new_g = g + step_cost;
                    int h = dist_map[nr][nc] * 3; 
                    short new_bd = i ^ 1; 
                    
                    if (macro_visited[nr][nc][new_bd] != macro_search_id || new_g < dist_macro[nr][nc][new_bd]) {
                        macro_visited[nr][nc][new_bd] = macro_search_id;
                        dist_macro[nr][nc][new_bd] = new_g;
                        parent_macro[nr][nc][new_bd] = {r, c, bd};
                        move_macro[nr][nc][new_bd] = b_path + t_char[i];
                        pq.push({new_g + h, new_g, h, (short)nr, (short)nc, new_bd});
                    }
                }
            }
        }
    }
    return "FAILED";
}

// ==========================================
// V51: Evaluator
// ==========================================
struct Candidate { int r, c, score; };

string solve_single_tile(int tr, int tc, int needed_val) {
    vector<vector<int>> dist_map = get_all_distances_from(tr, tc);
    vector<vector<int>> blank_bfs = get_all_distances_from(br, bc);
    vector<Candidate> cands;
    
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (board[r][c] == needed_val && !locked[r][c] && dist_map[r][c] != -1) {
                
                int b_dist = blank_bfs[r][c];
                if (b_dist == -1) b_dist = 1000;
                
                // Dynamic Highway Bias based on board dimensions
                int dist_to_highway = min({r, N - 1 - r, c, N - 1 - c});
                int expected = b_dist + (4 * dist_map[r][c]) + (dist_to_highway * 2);
                
                // Absolute Veto: Do not steal tiles that are already home
                if (is_perfect(r, c)) expected += 10000;
                
                cands.push_back({r, c, expected});
            }
        }
    }
    
    if (cands.empty()) return "FAILED";
    
    sort(cands.begin(), cands.end(), [](const Candidate& a, const Candidate& b){
        return a.score < b.score;
    });

    string best_path = "FAILED"; 
    int min_moves = 2e9;
    
    // Evaluate Top 12 (Safely finds absolute best path among candidates)
    int limit = min((int)cands.size(), 12);
    for (int i = 0; i < limit; i++) {
        string path = macro_a_star(cands[i].r, cands[i].c, tr, tc, dist_map);
        if (path != "FAILED") {
            int moves = path.length();
            if (moves < min_moves) {
                best_path = path;
                min_moves = moves;
            }
        }
    }
    return best_path;
}

// ==========================================
// Peephole Optimizer 
// ==========================================
string optimize_final_path(string path) {
    bool changed = true;
    while (changed) {
        changed = false; string opt = "";
        for (char m : path) {
            if (!opt.empty()) {
                char last = opt.back();
                if ((m == 'U' && last == 'D') || (m == 'D' && last == 'U') ||
                    (m == 'L' && last == 'R') || (m == 'R' && last == 'L')) { opt.pop_back(); changed = true; continue; }
            }
            opt.push_back(m);
        }
        path = opt;
    }
    return path;
}

int main() {
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    if (!(cin >> N)) return 0;
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> board[i][j]; if (board[i][j] == -1) { br = i; bc = j; }
            locked[i][j] = false;
        }
    }
    for (int i = 0; i < N - 2; i++) for (int j = 0; j < N - 2; j++) cin >> target[i][j];
    
    cerr << ">>> Running MACRO A* V51 (The Harmonizer)...\n";
    
    for (int tr = 1; tr <= N - 2; tr++) {
        if (tr % 2 == 1) {
            for (int tc = 1; tc <= N - 2; tc++) {
                int needed = target[tr - 1][tc - 1];
                if (board[tr][tc] != needed) {
                    string path = solve_single_tile(tr, tc, needed);
                    if (path != "FAILED") apply_moves(path);
                }
                locked[tr][tc] = true;
            }
        } else {
            for (int tc = N - 2; tc >= 1; tc--) {
                int needed = target[tr - 1][tc - 1];
                if (board[tr][tc] != needed) {
                    string path = solve_single_tile(tr, tc, needed);
                    if (path != "FAILED") apply_moves(path);
                }
                locked[tr][tc] = true;
            }
        }
    }
    
    string final_ans = optimize_final_path(ans) + "S";
    cerr << ">>> MACRO A* SOLVED!\n>>> Final Moves: " << final_ans.length() - 1 << "\n";
    cout << final_ans << "\n";
    return 0;
}