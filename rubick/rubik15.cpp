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

// 🥈 The Silver Ratio: สมดุลระหว่างหาทางลัด (H=1) กับการเดินเป็นระเบียบ (H=6)
const int H_WEIGHT = 2; 

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
// 1. เรดาร์ BFS
// ==========================================
vector<vector<int>> get_all_distances_from(int start_r, int start_c) {
    vector<vector<int>> dist(N, vector<int>(N, -1));
    queue<pair<int, int>> q;
    q.push({start_r, start_c});
    dist[start_r][start_c] = 0;
    int dr[] = {-1, 1, 0, 0}; int dc[] = {0, 0, -1, 1};
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
// 2. Micro BFS
// ==========================================
short b_visited[MAXN][MAXN];
short b_visit_id = 0;
int b_q_r[MAXN * MAXN], b_q_c[MAXN * MAXN];
pair<short, short> b_parent[MAXN][MAXN];
char b_move_from[MAXN][MAXN];

string fast_bfs_blank(int sr, int sc, int tr, int tc, int avoid_r, int avoid_c) {
    if (sr == tr && sc == tc) return "";
    b_visit_id++;
    
    int head = 0, tail = 0;
    b_q_r[tail] = sr; b_q_c[tail] = sc; tail++;
    b_visited[sr][sc] = b_visit_id;
    
    int b_dr[] = {1, -1, 0, 0}; int b_dc[] = {0, 0, 1, -1};
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

// ==========================================
// 3. MACRO A* V9: The Silver Ratio + Turn Stabilizer
// ==========================================
struct MacroState { short r, c, bd; };
MacroState parent_macro[MAXN][MAXN][4];
string move_macro[MAXN][MAXN][4];
int dist_macro[MAXN][MAXN][4];
int macro_visited[MAXN][MAXN][4];
int macro_search_id = 0;

struct MacroNode {
    int f, g, turns; short r, c, bd;
    bool operator>(const MacroNode& o) const {
        if (f != o.f) return f > o.f;                   // 1. หาเส้นทางที่สั้นที่สุด
        if (turns != o.turns) return turns > o.turns;   // 2. ถ้าก้าวเท่ากัน เลือกทางที่เลี้ยว "น้อยกว่า" (รักษาระเบียบกระดาน)
        return g < o.g;                                 // 3. เดินเข้าใกล้เป้าหมายมากที่สุด
    }
};

string macro_a_star(int sr, int sc, int tar_r, int tar_c, const vector<vector<int>>& dist_map, int best_so_far) {
    macro_search_id++;
    priority_queue<MacroNode, vector<MacroNode>, greater<MacroNode>> pq;
    
    int t_dr[] = {-1, 1, 0, 0}; int t_dc[] = {0, 0, -1, 1}; char t_char[] = {'U', 'D', 'L', 'R'};
    
    for (int i=0; i<4; i++) {
        int nbr = sr + t_dr[i], nbc = sc + t_dc[i];
        if (nbr >= 0 && nbr < N && nbc >= 0 && nbc < N && !locked[nbr][nbc]) {
            string b_path = fast_bfs_blank(br, bc, nbr, nbc, sr, sc);
            if (b_path != "FAILED") {
                int g = b_path.length();
                int true_h = dist_map[sr][sc];
                
                if (g + true_h >= best_so_far) continue; // Pruning ด้วยก้าวจริง 100%
                
                int f_score = g + (true_h * H_WEIGHT);
                
                macro_visited[sr][sc][i] = macro_search_id;
                dist_macro[sr][sc][i] = g;
                parent_macro[sr][sc][i] = {-1, -1, -1};
                move_macro[sr][sc][i] = b_path;
                
                pq.push({f_score, g, 0, (short)sr, (short)sc, (short)i});
            }
        }
    }
    
    while(!pq.empty()) {
        auto curr = pq.top(); pq.pop();
        int g = curr.g; short r = curr.r, c = curr.c, bd = curr.bd, turns = curr.turns;
        
        if (g + dist_map[r][c] >= best_so_far) continue; 
        if (g > dist_macro[r][c][bd]) continue;
        
        if (r == tar_r && c == tar_c) {
            string full_path = "";
            short cr = r, cc = c, cbd = bd;
            while(cr != sr || cc != sc) {
                full_path = move_macro[cr][cc][cbd] + full_path;
                auto p = parent_macro[cr][cc][cbd];
                cr = p.r; cc = p.c; cbd = p.bd;
            }
            full_path = move_macro[sr][sc][cbd] + full_path;
            return full_path;
        }
        
        for (int i=0; i<4; i++) {
            int nr = r + t_dr[i], nc = c + t_dc[i];
            if (nr >= 0 && nr < N && nc >= 0 && nc < N && !locked[nr][nc]) {
                int target_br = nr, target_bc = nc; 
                int current_br = r + t_dr[bd], current_bc = c + t_dc[bd];
                
                string b_path = fast_bfs_blank(current_br, current_bc, target_br, target_bc, r, c);
                if (b_path != "FAILED") {
                    int step_cost = b_path.length() + 1;
                    int new_g = g + step_cost;
                    int true_h = dist_map[nr][nc];
                    
                    if (new_g + true_h >= best_so_far) continue; 
                    
                    short new_bd = i ^ 1; 
                    int new_turns = turns + (bd != new_bd ? 1 : 0);
                    
                    if (macro_visited[nr][nc][new_bd] != macro_search_id || new_g < dist_macro[nr][nc][new_bd]) {
                        macro_visited[nr][nc][new_bd] = macro_search_id;
                        dist_macro[nr][nc][new_bd] = new_g;
                        parent_macro[nr][nc][new_bd] = {r, c, bd};
                        move_macro[nr][nc][new_bd] = b_path + t_char[i];
                        
                        int f_score = new_g + (true_h * H_WEIGHT);
                        pq.push({f_score, new_g, new_turns, (short)nr, (short)nc, new_bd});
                    }
                }
            }
        }
    }
    return "FAILED";
}

// ==========================================
// 4. Optimize ขั้นสุดท้าย 
// ==========================================
string optimize_final_path(string path) {
    string opt = "";
    for (char m : path) {
        if (!opt.empty()) {
            char last = opt.back();
            if ((m == 'U' && last == 'D') || (m == 'D' && last == 'U') ||
                (m == 'L' && last == 'R') || (m == 'R' && last == 'L')) { opt.pop_back(); continue; }
        }
        opt.push_back(m);
    }
    return opt;
}

struct Candidate { int r, c, raw_d, b_dist; };

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    memset(b_visited, 0, sizeof(b_visited));
    memset(macro_visited, 0, sizeof(macro_visited));
    
    if (!(cin >> N)) return 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> board[i][j];
            if (board[i][j] == -1) { br = i; bc = j; }
            locked[i][j] = false;
        }
    }
    for (int i = 0; i < N - 2; i++) {
        for (int j = 0; j < N - 2; j++) { cin >> target[i][j]; }
    }

    cerr << ">>> Running MACRO A* V9 (Silver Ratio H*2 + Turn Stabilizer)...\n";

    for (int tr = 1; tr <= N - 2; tr++) {
        for (int tc = 1; tc <= N - 2; tc++) {
            int needed_val = target[tr - 1][tc - 1];
            
            if (board[tr][tc] == needed_val) {
                locked[tr][tc] = true;
                continue;
            }

            vector<vector<int>> dist_map = get_all_distances_from(tr, tc);
            
            vector<Candidate> cands;
            for (int r = 0; r < N; r++) {
                for (int c = 0; c < N; c++) {
                    if (board[r][c] == needed_val && !locked[r][c] && dist_map[r][c] != -1) {
                        int b_dist = abs(br - r) + abs(bc - c);
                        cands.push_back({r, c, dist_map[r][c], b_dist});
                    }
                }
            }
            
            // เรียงลำดับตัวเลือกเพื่อเร่งความเร็วในการ Pruning
            sort(cands.begin(), cands.end(), [](const Candidate& a, const Candidate& b){
                return (a.raw_d * 4 + a.b_dist) < (b.raw_d * 4 + b.b_dist);
            });

            string best_path = "FAILED";
            int min_moves = 1e9; 

            for (auto& cand : cands) {
                if (cand.raw_d >= min_moves) continue; 
                
                string path = macro_a_star(cand.r, cand.c, tr, tc, dist_map, min_moves);
                
                if (path != "FAILED" && path.length() < min_moves) {
                    min_moves = path.length();
                    best_path = path;
                }
            }

            if (best_path != "FAILED") {
                apply_moves(best_path);
            } else {
                cerr << "    [FATAL] Macro A* Failed at (" << tr << "," << tc << ").\n";
            }
            locked[tr][tc] = true;
        }
    }

    string final_ans = optimize_final_path(ans) + "S";
    cerr << ">>> MACRO A* SOLVED!\n>>> Final Moves: " << final_ans.length() - 1 << "\n";
    cout << final_ans << "\n";

    return 0;
}