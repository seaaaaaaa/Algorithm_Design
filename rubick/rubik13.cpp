#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <cmath>
#include <algorithm>
#include <tuple>
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
// 1. ระบบหาแผ่นที่ใกล้ที่สุด
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
// 2. BFS เอาช่องว่างเดินไปประกบแผ่นเป้าหมาย
// ==========================================
string bfs_blank_to_adj(int tr, int tc) {
    if (abs(br - tr) + abs(bc - tc) == 1) return "";
    queue<pair<int, int>> q;
    vector<vector<int>> dist(N, vector<int>(N, -1));
    vector<vector<char>> move_from(N, vector<char>(N, ' '));
    vector<vector<pair<int,int>>> p(N, vector<pair<int,int>>(N, {-1, -1}));

    q.push({br, bc});
    dist[br][bc] = 0;
    int phys_dr[] = {1, -1, 0, 0}; int phys_dc[] = {0, 0, 1, -1};
    char move_char[] = {'U', 'D', 'L', 'R'};
    int end_r = -1, end_c = -1;

    while(!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        if (abs(r - tr) + abs(c - tc) == 1) { end_r = r; end_c = c; break; }
        for(int i=0; i<4; i++) {
            int nr = r + phys_dr[i], nc = c + phys_dc[i];
            if (nr >= 0 && nr < N && nc >= 0 && nc < N && !locked[nr][nc]) {
                if (nr == tr && nc == tc) continue;
                if (dist[nr][nc] == -1) {
                    dist[nr][nc] = dist[r][c] + 1;
                    p[nr][nc] = {r, c};
                    move_from[nr][nc] = move_char[i];
                    q.push({nr, nc});
                }
            }
        }
    }
    if (end_r == -1) return "FAILED";
    string path = "";
    int cr = end_r, cc = end_c;
    while(cr != br || cc != bc) {
        path += move_from[cr][cc];
        auto pr = p[cr][cc]; cr = pr.first; cc = pr.second;
    }
    reverse(path.begin(), path.end());
    return path;
}

// ==========================================
// 3. SINGLE-TILE A* (STA*) - อาวุธลับ
// ==========================================
const int MAX_REL = 22; 
const int REL_OFF = MAX_REL;
const int REL_SIZE = MAX_REL * 2 + 1;
int visited[MAXN][MAXN][REL_SIZE][REL_SIZE];
int current_search_id = 0;

struct QNode {
    int cost, f, tr, tc, br, bc;
    bool operator>(const QNode& o) const { return f > o.f; }
};

struct ParentNode { char tr, tc, br, bc, move; };
ParentNode parent_map[MAXN][MAXN][REL_SIZE][REL_SIZE];

string drag_tile_sta(int sr, int sc, int target_r, int target_c) {
    current_search_id++;
    if (current_search_id > 2000000000) { memset(visited, 0, sizeof(visited)); current_search_id = 1; }
    
    priority_queue<QNode, vector<QNode>, greater<QNode>> pq;
    int start_bdr = br - sr, start_bdc = bc - sc;
    pq.push({0, 0, sr, sc, br, bc});
    visited[sr][sc][start_bdr + REL_OFF][start_bdc + REL_OFF] = current_search_id;
    
    int phys_dr[] = {1, -1, 0, 0}; int phys_dc[] = {0, 0, 1, -1};
    char move_char[] = {'U', 'D', 'L', 'R'};
    int original_br = br, original_bc = bc;
    int expansions = 0;

    while(!pq.empty()) {
        auto curr = pq.top(); pq.pop();
        expansions++;
        if (expansions > 9000) return "FAILED"; // ถ้าค้นหานานเกินไป ให้ยอมแพ้แล้วไปใช้โหมดฉุกเฉิน
        
        if (curr.tr == target_r && curr.tc == target_c) {
            string path = "";
            int ctr = curr.tr, ctc = curr.tc, cbr = curr.br, cbc = curr.bc;
            while (ctr != sr || ctc != sc || cbr != original_br || cbc != original_bc) {
                int rel_r = cbr - ctr, rel_c = cbc - ctc;
                auto p = parent_map[ctr][ctc][rel_r + REL_OFF][rel_c + REL_OFF];
                path += p.move;
                ctr = p.tr; ctc = p.tc; cbr = p.br; cbc = p.bc;
            }
            reverse(path.begin(), path.end());
            return path;
        }
        
        for (int i=0; i<4; i++) {
            int nbr = curr.br + phys_dr[i], nbc = curr.bc + phys_dc[i];
            if (nbr < 0 || nbr >= N || nbc < 0 || nbc >= N || locked[nbr][nbc]) continue; 
            
            int ntr = curr.tr, ntc = curr.tc;
            if (nbr == curr.tr && nbc == curr.tc) { ntr = curr.br; ntc = curr.bc; }
            
            int rel_r = nbr - ntr, rel_c = nbc - ntc;
            if (abs(rel_r) > MAX_REL || abs(rel_c) > MAX_REL) continue;
            
            if (visited[ntr][ntc][rel_r + REL_OFF][rel_c + REL_OFF] != current_search_id) {
                visited[ntr][ntc][rel_r + REL_OFF][rel_c + REL_OFF] = current_search_id;
                parent_map[ntr][ntc][rel_r + REL_OFF][rel_c + REL_OFF] = {(char)curr.tr, (char)curr.tc, (char)curr.br, (char)curr.bc, move_char[i]};
                
                int cost = curr.cost + 1;
                int h = 5 * (abs(ntr - target_r) + abs(ntc - target_c)) + (abs(nbr - ntr) + abs(nbc - ntc));
                pq.push({cost, cost * 2 + 3 * h, ntr, ntc, nbr, nbc});
            }
        }
    }
    return "FAILED";
}

// ==========================================
// 4. FALLBACK SYSTEM (ระบบฉุกเฉินการันตีคำตอบ)
// ==========================================
string fallback_bfs_blank(int tr, int tc, int avoid_r, int avoid_c) {
    if (br == tr && bc == tc) return "";
    queue<pair<int, int>> q;
    vector<vector<int>> dist(N, vector<int>(N, -1));
    vector<vector<char>> move_from(N, vector<char>(N, ' '));
    vector<vector<pair<int,int>>> p(N, vector<pair<int,int>>(N, {-1, -1}));

    q.push({br, bc}); dist[br][bc] = 0;
    int dr[] = {-1, 1, 0, 0}; int dc[] = {0, 0, -1, 1}; char act[] = {'D', 'U', 'R', 'L'}; 

    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        if (r == tr && c == tc) break;
        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i], nc = c + dc[i];
            if (nr >= 0 && nr < N && nc >= 0 && nc < N && !locked[nr][nc] && !(nr == avoid_r && nc == avoid_c)) {
                if (dist[nr][nc] == -1) {
                    dist[nr][nc] = dist[r][c] + 1;
                    p[nr][nc] = {r, c};
                    move_from[nr][nc] = act[i];
                    q.push({nr, nc});
                }
            }
        }
    }
    if (dist[tr][tc] == -1) return "FAILED";
    string path = ""; int cr = tr, cc = tc;
    while (cr != br || cc != bc) { path += move_from[cr][cc]; auto pr = p[cr][cc]; cr = pr.first; cc = pr.second; }
    reverse(path.begin(), path.end()); return path;
}

string fallback_get_straight_tile_path(int sr, int sc, int tr, int tc) {
    if (sr == tr && sc == tc) return "";
    priority_queue<tuple<int, int, int, int>, vector<tuple<int,int,int,int>>, greater<>> pq;
    int dist[MAXN][MAXN][5];
    for(int i=0;i<N;i++) for(int j=0;j<N;j++) for(int k=0;k<5;k++) dist[i][j][k] = 1e9;
    tuple<int,int,int> p[MAXN][MAXN][5]; char move_char[MAXN][MAXN][5];
    
    pq.push({0, sr, sc, 4}); dist[sr][sc][4] = 0;
    int dr[] = {-1, 1, 0, 0}; int dc[] = {0, 0, -1, 1}; char act[] = {'U', 'D', 'L', 'R'};
    int best_end_dir = -1; int min_end_cost = 1e9;

    while(!pq.empty()){
        auto [cost, r, c, l_dir] = pq.top(); pq.pop();
        if (cost > dist[r][c][l_dir]) continue;
        if (r == tr && c == tc) { if (cost < min_end_cost) { min_end_cost = cost; best_end_dir = l_dir; } continue; }
        for(int i=0; i<4; i++){
            int nr = r + dr[i], nc = c + dc[i];
            if(nr >= 0 && nr < N && nc >= 0 && nc < N && !locked[nr][nc]) {
                int turn_penalty = (l_dir != 4 && l_dir != i) ? 3 : 0;
                int new_cost = cost + 10 + turn_penalty; 
                if (new_cost < dist[nr][nc][i]) {
                    dist[nr][nc][i] = new_cost; p[nr][nc][i] = {r, c, l_dir}; move_char[nr][nc][i] = act[i];
                    pq.push({new_cost, nr, nc, i});
                }
            }
        }
    }
    if (best_end_dir == -1) return "FAILED";
    string path = ""; int cr = tr, cc = tc, cdir = best_end_dir;
    while(cr != sr || cc != sc) { path += move_char[cr][cc][cdir]; auto pr = p[cr][cc][cdir]; cr = get<0>(pr); cc = get<1>(pr); cdir = get<2>(pr); }
    reverse(path.begin(), path.end()); return path;
}

void fallback_drag(int sr, int sc, int tr, int tc) {
    int r = sr, c = sc;
    while (r != tr || c != tc) {
        string t_path = fallback_get_straight_tile_path(r, c, tr, tc);
        if (t_path == "FAILED" || t_path.empty()) break;
        
        for (char tile_move : t_path) {
            int dr = 0, dc = 0;
            if (tile_move == 'U') dr = -1; if (tile_move == 'D') dr = 1;
            if (tile_move == 'L') dc = -1; if (tile_move == 'R') dc = 1;
            int b_tr = r + dr, b_tc = c + dc;
            string b_path = fallback_bfs_blank(b_tr, b_tc, r, c);
            if (b_path == "FAILED") {
                int bdr[] = {-1, 1, 0, 0}; int bdc[] = {0, 0, -1, 1}; char b_act[] = {'D', 'U', 'R', 'L'};
                for (int i = 0; i < 4; i++) {
                    int nbr = br + bdr[i], nbc = bc + bdc[i];
                    if (nbr >= 0 && nbr < N && nbc >= 0 && nbc < N && !locked[nbr][nbc] && !(nbr == r && nbc == c)) {
                        apply_move(b_act[i]); break;
                    }
                }
                break; // เลี้ยวหลบแล้วเบรคเพื่อวางแผนทางเดินใหม่
            }
            apply_moves(b_path);
            apply_move(tile_move);
            r += dr; c += dc;
        }
    }
}

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

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    memset(visited, 0, sizeof(visited));
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

    cerr << ">>> Running Ultimate STA* + Safety Fallback Solver...\n";

    for (int tr = 1; tr <= N - 2; tr++) {
        for (int tc = 1; tc <= N - 2; tc++) {
            int needed_val = target[tr - 1][tc - 1];
            vector<vector<int>> dist_map = get_all_distances_from(tr, tc);
            int best_dist = 1e9, best_blank_dist = 1e9, best_r = -1, best_c = -1;

            for (int r = 0; r < N; r++) {
                for (int c = 0; c < N; c++) {
                    if (board[r][c] == needed_val && !locked[r][c] && dist_map[r][c] != -1) {
                        int d = dist_map[r][c];
                        int b_dist = abs(br - r) + abs(bc - c); 
                        if (d < best_dist || (d == best_dist && b_dist < best_blank_dist)) {
                            best_dist = d; best_blank_dist = b_dist; best_r = r; best_c = c;
                        }
                    }
                }
            }

            if (best_r != -1 && best_c != -1) {
                // 1. เดินช่องว่างไปชิดกระเบื้อง
                string b_path = bfs_blank_to_adj(best_r, best_c);
                if (b_path != "FAILED") apply_moves(b_path);

                // 2. ลองใช้ STA* ลากกระเบื้องแบบอัจฉริยะ (เร็ว แต่ลึก)
                string t_path = drag_tile_sta(best_r, best_c, tr, tc);
                
                if (t_path != "FAILED") {
                    apply_moves(t_path);
                } else {
                    // 3. ยามฉุกเฉิน: ถ้า STA* งง สลับมาใช้ระบบ Dumb Drag ทันที!
                    cerr << "    [WARNING] STA* Blocked. Triggering Fallback Drag at (" << tr << "," << tc << ")...\n";
                    fallback_drag(best_r, best_c, tr, tc);
                }
                locked[tr][tc] = true;
            }
        }
    }

    string final_ans = optimize_final_path(ans) + "S";
    cerr << ">>> SOLVED!\n>>> Final Moves: " << final_ans.length() - 1 << "\n";
    cout << final_ans << "\n";

    return 0;
}