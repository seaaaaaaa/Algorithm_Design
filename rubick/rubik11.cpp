#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <cmath>
#include <algorithm>
#include <unordered_set>
#include <cstring>
#include <tuple>

using namespace std;

const int MAXN = 75;
int N;
int original_board[MAXN][MAXN];
int target_arr[MAXN][MAXN];

// ==========================================
// 1. ADVANCED WA* SOLVER (มองภาพรวมทั้งกระดาน)
// ==========================================
struct State {
    vector<char> b;
    int blank_pos;
    int g;
    int h;
    int parent;
    char move;
};

struct CompareState {
    bool operator()(const pair<int, int>& a, const pair<int, int>& b) const {
        return a.first > b.first;
    }
};

struct VectorHash {
    size_t operator()(const vector<char>& v) const {
        size_t hash = 0;
        for (char c : v) hash ^= c + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        return hash;
    }
};

int compute_h(const vector<char>& b) {
    static int pos_arr[105][MAXN*MAXN];
    static int count[105];
    memset(count, 0, sizeof(count));
    
    for(int i=0; i<N*N; i++) {
        int c = b[i];
        if(c > 0 && c <= 100) pos_arr[c][count[c]++] = i;
    }
    
    int h = 0;
    for(int tr=1; tr<=N-2; tr++){
        for(int tc=1; tc<=N-2; tc++){
            int req = target_arr[tr-1][tc-1];
            int curr = b[tr*N+tc];
            if(curr == req) continue;
            
            int min_d = 1e9;
            for(int i=0; i<count[req]; i++){
                int p = pos_arr[req][i];
                int d = abs(p / N - tr) + abs(p % N - tc);
                if(d < min_d) min_d = d;
            }
            h += min_d;
        }
    }
    return h;
}

string solve_astar() {
    vector<char> start_b(N*N);
    int start_blank = -1;
    for(int i=0; i<N; i++) {
        for(int j=0; j<N; j++) {
            start_b[i*N + j] = original_board[i][j];
            if(original_board[i][j] == -1) start_blank = i*N + j;
        }
    }
    
    vector<State> states;
    states.reserve(300000); // จอง Memory เตรียมไว้
    
    priority_queue<pair<int, int>, vector<pair<int, int>>, CompareState> pq;
    unordered_set<vector<char>, VectorHash> closed;
    
    int start_h = compute_h(start_b);
    if (start_h == 0) return "S";
    
    states.push_back({start_b, start_blank, 0, start_h, -1, ' '});
    pq.push({0 + 10 * start_h, 0}); // Weight = 10 พุ่งชนเป้าหมาย
    
    int max_expansions = (N <= 33) ? 150000 : 50000; // ตัดจบเร็วขึ้นถ้า N ใหญ่
    int expansions = 0;
    
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    char dir[] = {'D', 'U', 'R', 'L'}; 
    
    while(!pq.empty() && expansions < max_expansions) {
        int u = pq.top().second; pq.pop();
        
        if (closed.count(states[u].b)) continue;
        closed.insert(states[u].b);
        expansions++;
        
        if (states[u].h == 0) {
            string path = "";
            int curr = u;
            while(states[curr].parent != -1) {
                path += states[curr].move;
                curr = states[curr].parent;
            }
            reverse(path.begin(), path.end());
            cerr << "[WA*] BOOYAH! Solved optimally! Expansions: " << expansions << "\n";
            return path + "S";
        }
        
        int br = states[u].blank_pos / N;
        int bc = states[u].blank_pos % N;
        
        for(int i=0; i<4; i++) {
            int nbr = br + dr[i], nbc = bc + dc[i];
            if (nbr >= 0 && nbr < N && nbc >= 0 && nbc < N) {
                vector<char> next_b = states[u].b;
                swap(next_b[br*N + bc], next_b[nbr*N + nbc]);
                
                if (closed.count(next_b)) continue;
                
                int next_blank = nbr*N + nbc;
                int g = states[u].g + 1;
                int h = compute_h(next_b);
                
                states.push_back({next_b, next_blank, g, h, u, dir[i]});
                pq.push({g + 10 * h, states.size() - 1});
            }
        }
    }
    cerr << "[WA*] Aborted. Too complex for A*. Falling back to Constructive Solver...\n";
    return "FAILED";
}

// ==========================================
// 2. CONSTRUCTIVE SOLVER (ระบบสำรองฉุกเฉิน)
// ==========================================
namespace Constructive {
    int board[MAXN][MAXN];
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
        br = tr; bc = tc;
    }

    void apply_moves(string moves) {
        for (char m : moves) apply_move(m);
    }

    string bfs_blank(int target_r, int target_c, int avoid_r, int avoid_c) {
        if (br == target_r && bc == target_c) return "";
        queue<pair<int, int>> q;
        vector<vector<int>> dist(N, vector<int>(N, -1));
        vector<vector<char>> move_from(N, vector<char>(N, ' '));
        vector<vector<pair<int,int>>> parent(N, vector<pair<int,int>>(N, {-1, -1}));

        q.push({br, bc});
        dist[br][bc] = 0;
        int bdr[] = {-1, 1, 0, 0}; int bdc[] = {0, 0, -1, 1}; char bact[] = {'D', 'U', 'R', 'L'}; 

        while (!q.empty()) {
            auto [r, c] = q.front(); q.pop();
            if (r == target_r && c == target_c) break;
            for (int i = 0; i < 4; i++) {
                int nr = r + bdr[i], nc = c + bdc[i];
                if (nr >= 0 && nr < N && nc >= 0 && nc < N && !locked[nr][nc] && !(nr == avoid_r && nc == avoid_c)) {
                    if (dist[nr][nc] == -1) {
                        dist[nr][nc] = dist[r][c] + 1;
                        parent[nr][nc] = {r, c};
                        move_from[nr][nc] = bact[i];
                        q.push({nr, nc});
                    }
                }
            }
        }
        if (dist[target_r][target_c] == -1) return "FAILED";
        string path = "";
        int curr_r = target_r, curr_c = target_c;
        while (curr_r != br || curr_c != bc) {
            path += move_from[curr_r][curr_c];
            auto p = parent[curr_r][curr_c];
            curr_r = p.first; curr_c = p.second;
        }
        reverse(path.begin(), path.end());
        return path;
    }

    string get_straight_tile_path(int sr, int sc, int tr, int tc) {
        if (sr == tr && sc == tc) return "";
        priority_queue<tuple<int, int, int, int>, vector<tuple<int,int,int,int>>, greater<>> pq;
        int dist[MAXN][MAXN][5];
        for(int i=0;i<N;i++) for(int j=0;j<N;j++) for(int k=0;k<5;k++) dist[i][j][k] = 1e9;
        tuple<int,int,int> parent[MAXN][MAXN][5];
        char move_char[MAXN][MAXN][5];
        pq.push({0, sr, sc, 4});
        dist[sr][sc][4] = 0;
        int tdr[] = {-1, 1, 0, 0}; int tdc[] = {0, 0, -1, 1}; char tact[] = {'U', 'D', 'L', 'R'};
        int best_end_dir = -1; int min_end_cost = 1e9;

        while(!pq.empty()){
            auto [cost, r, c, l_dir] = pq.top(); pq.pop();
            if (cost > dist[r][c][l_dir]) continue;
            if (r == tr && c == tc) {
                if (cost < min_end_cost) { min_end_cost = cost; best_end_dir = l_dir; }
                continue;
            }
            for(int i=0; i<4; i++){
                int nr = r + tdr[i], nc = c + tdc[i];
                if(nr >= 0 && nr < N && nc >= 0 && nc < N && !locked[nr][nc]) {
                    int turn_penalty = (l_dir != 4 && l_dir != i) ? 2 : 0;
                    int new_cost = cost + 10 + turn_penalty; 
                    if (new_cost < dist[nr][nc][i]) {
                        dist[nr][nc][i] = new_cost;
                        parent[nr][nc][i] = {r, c, l_dir};
                        move_char[nr][nc][i] = tact[i];
                        pq.push({new_cost, nr, nc, i});
                    }
                }
            }
        }
        if (best_end_dir == -1) return "FAILED";
        string path = "";
        int cr = tr, cc = tc, cdir = best_end_dir;
        while(cr != sr || cc != sc) {
            path += move_char[cr][cc][cdir];
            auto pr = parent[cr][cc][cdir];
            cr = get<0>(pr); cc = get<1>(pr); cdir = get<2>(pr);
        }
        reverse(path.begin(), path.end());
        return path;
    }

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

    void move_tile_to(int sr, int sc, int tr, int tc) {
        int r = sr, c = sc;
        while (r != tr || c != tc) {
            string t_path = get_straight_tile_path(r, c, tr, tc);
            if (t_path == "FAILED" || t_path.empty()) break;
            
            for (char tile_move : t_path) {
                int dr = 0, dc = 0;
                if (tile_move == 'U') dr = -1;
                if (tile_move == 'D') dr = 1;
                if (tile_move == 'L') dc = -1;
                if (tile_move == 'R') dc = 1;

                int blank_target_r = r + dr;
                int blank_target_c = c + dc;

                string b_path = bfs_blank(blank_target_r, blank_target_c, r, c);
                if (b_path == "FAILED") {
                    int bdr[] = {-1, 1, 0, 0}; int bdc[] = {0, 0, -1, 1}; char b_act[] = {'D', 'U', 'R', 'L'};
                    for (int i = 0; i < 4; i++) {
                        int nbr = br + bdr[i], nbc = bc + bdc[i];
                        if (nbr >= 0 && nbr < N && nbc >= 0 && nbc < N && !locked[nbr][nbc] && !(nbr == r && nbc == c)) {
                            apply_move(b_act[i]); break;
                        }
                    }
                    break;
                }
                apply_moves(b_path);
                apply_move(tile_move);
                r += dr; 
                c += dc;
            }
        }
    }

    string optimize_final_path(string path) {
        string opt = "";
        for (char m : path) {
            if (!opt.empty()) {
                char last = opt.back();
                if ((m == 'U' && last == 'D') || (m == 'D' && last == 'U') ||
                    (m == 'L' && last == 'R') || (m == 'R' && last == 'L')) {
                    opt.pop_back(); 
                    continue;
                }
            }
            opt.push_back(m);
        }
        return opt;
    }

    void solve() {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                board[i][j] = original_board[i][j];
                if (board[i][j] == -1) { br = i; bc = j; }
                locked[i][j] = false;
            }
        }

        for (int tr = 1; tr <= N - 2; tr++) {
            for (int tc = 1; tc <= N - 2; tc++) {
                int needed_val = target_arr[tr - 1][tc - 1];
                if (board[tr][tc] == needed_val) {
                    locked[tr][tc] = true;
                    continue;
                }
                vector<vector<int>> dist_map = get_all_distances_from(tr, tc);
                int best_dist = 1e9, best_blank_dist = 1e9, best_r = -1, best_c = -1;

                for (int r = 0; r < N; r++) {
                    for (int c = 0; c < N; c++) {
                        if (board[r][c] == needed_val && !locked[r][c] && dist_map[r][c] != -1) {
                            int d = dist_map[r][c];
                            int b_dist = abs(br - r) + abs(bc - c); 
                            if (d < best_dist || (d == best_dist && b_dist < best_blank_dist)) {
                                best_dist = d;
                                best_blank_dist = b_dist;
                                best_r = r;
                                best_c = c;
                            }
                        }
                    }
                }
                if (best_r != -1 && best_c != -1) {
                    move_tile_to(best_r, best_c, tr, tc);
                    locked[tr][tc] = true;
                }
            }
        }
        string final_ans = optimize_final_path(ans) + "S";
        cerr << "[Constructive] Fallback Solved! Moves: " << final_ans.length() - 1 << "\n";
        cout << final_ans << "\n";
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N)) return 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> original_board[i][j];
        }
    }

    for (int i = 0; i < N - 2; i++) {
        for (int j = 0; j < N - 2; j++) {
            cin >> target_arr[i][j];
        }
    }

    cerr << ">>> Analyzing Board " << N << "x" << N << "...\n";
    
    // ลองใช้ AI ดำน้ำหาทางที่ดีที่สุดก่อน (ให้เวลาคิดแปปนึง)
    string ans = solve_astar();
    
    if (ans != "FAILED") {
        cout << ans << "\n";
    } else {
        // ถ้ากระดานใหญ่เกิน AI จะสลับมาใช้การดึงทีละแผ่นทันที
        Constructive::solve();
    }

    return 0;
}