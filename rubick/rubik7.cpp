#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

const int MAXN = 75;
int N;
int board[MAXN][MAXN];
int target[MAXN][MAXN];
bool locked[MAXN][MAXN];

int br, bc; // ตำแหน่งปัจจุบันของช่องว่าง (-1)
string ans = "";

// ฟังก์ชันจำลองการเลื่อนกระเบื้อง พร้อมอัปเดตตำแหน่งช่องว่าง
void apply_move(char m) {
    ans += m;
    int tr = br, tc = bc;
    
    // กฎของเกม: ถ้าสั่ง 'U' แปลว่ากระเบื้องข้างล่างเลื่อนขึ้นมาทับช่องว่าง -> ช่องว่างเลื่อนลง ('D')
    if (m == 'U') tr = br + 1;
    if (m == 'D') tr = br - 1;
    if (m == 'L') tc = bc + 1;
    if (m == 'R') tc = bc - 1;
    
    swap(board[br][bc], board[tr][tc]);
    br = tr;
    bc = tc;
}

void apply_moves(string moves) {
    for (char m : moves) {
        apply_move(m);
    }
}

// BFS หาเส้นทางที่สั้นที่สุดสำหรับ "ช่องว่าง (-1)"
string bfs_blank(int target_r, int target_c, int avoid_r, int avoid_c) {
    if (br == target_r && bc == target_c) return "";
    
    queue<pair<int, int>> q;
    vector<vector<int>> dist(N, vector<int>(N, -1));
    vector<vector<char>> move_from(N, vector<char>(N, ' '));
    vector<vector<pair<int,int>>> parent(N, vector<pair<int,int>>(N, {-1, -1}));

    q.push({br, bc});
    dist[br][bc] = 0;

    // การขยับของช่องว่าง (UP, DOWN, LEFT, RIGHT) -> Action ของกระเบื้องที่สอดคล้องกัน
    int bdr[] = {-1, 1, 0, 0};
    int bdc[] = {0, 0, -1, 1};
    char bact[] = {'D', 'U', 'R', 'L'}; 

    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        if (r == target_r && c == target_c) break;

        for (int i = 0; i < 4; i++) {
            int nr = r + bdr[i], nc = c + bdc[i];
            if (nr >= 0 && nr < N && nc >= 0 && nc < N) {
                if (locked[nr][nc]) continue; // ห้ามเหยียบช่องที่ล็อคแล้ว
                if (nr == avoid_r && nc == avoid_c) continue; // ห้ามเหยียบกระเบื้องเป้าหมายที่เรากำลังจะลาก
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
        curr_r = p.first;
        curr_c = p.second;
    }
    reverse(path.begin(), path.end());
    return path;
}

// BFS หาเส้นทางที่สั้นที่สุดสำหรับ "กระเบื้อง" เพื่อเดินไปยังเป้าหมาย
string bfs_tile(int sr, int sc, int tr, int tc) {
    if (sr == tr && sc == tc) return "";
    
    queue<pair<int, int>> q;
    vector<vector<int>> dist(N, vector<int>(N, -1));
    vector<vector<char>> move_to(N, vector<char>(N, ' '));
    vector<vector<pair<int,int>>> parent(N, vector<pair<int,int>>(N, {-1, -1}));

    q.push({sr, sc});
    dist[sr][sc] = 0;

    int tdr[] = {-1, 1, 0, 0};
    int tdc[] = {0, 0, -1, 1};
    char tact[] = {'U', 'D', 'L', 'R'};

    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        if (r == tr && c == tc) break;

        for (int i = 0; i < 4; i++) {
            int nr = r + tdr[i], nc = c + tdc[i];
            if (nr >= 0 && nr < N && nc >= 0 && nc < N) {
                if (locked[nr][nc]) continue; // ห้ามลากผ่านช่องที่ล็อคแล้ว
                if (dist[nr][nc] == -1) {
                    dist[nr][nc] = dist[r][c] + 1;
                    parent[nr][nc] = {r, c};
                    move_to[nr][nc] = tact[i];
                    q.push({nr, nc});
                }
            }
        }
    }

    if (dist[tr][tc] == -1) return "FAILED";

    string path = "";
    int curr_r = tr, curr_c = tc;
    while (curr_r != sr || curr_c != sc) {
        path += move_to[curr_r][curr_c];
        auto p = parent[curr_r][curr_c];
        curr_r = p.first;
        curr_c = p.second;
    }
    reverse(path.begin(), path.end());
    return path;
}

// ลากกระเบื้องจาก (sr, sc) ไปยัง (tr, tc)
void move_tile_to(int sr, int sc, int tr, int tc) {
    int r = sr, c = sc;
    cerr << "  [DEBUG] Started dragging tile from (" << r << "," << c << ") to (" << tr << "," << tc << ")\n";
    
    while (r != tr || c != tc) {
        string t_path = bfs_tile(r, c, tr, tc);
        if (t_path == "FAILED" || t_path.empty()) {
            cerr << "    [FATAL] Tile BFS Failed! Cannot find route for tile.\n";
            break;
        }
        
        char tile_move = t_path[0]; // หยิบการขยับก้าวแรกของกระเบื้องมาทำก่อน
        int dr = 0, dc = 0;
        if (tile_move == 'U') dr = -1;
        if (tile_move == 'D') dr = 1;
        if (tile_move == 'L') dc = -1;
        if (tile_move == 'R') dc = 1;

        // คำนวณว่าช่องว่าง (-1) ต้องไปดักรอที่ไหน เพื่อให้กระเบื้องไหลเข้าหาเป้าหมายได้
        int blank_target_r = r + dr;
        int blank_target_c = c + dc;

        cerr << "    [DEBUG] Tile wants to move " << tile_move << ". Routing Blank to (" << blank_target_r << "," << blank_target_c << ")...\n";
        
        string b_path = bfs_blank(blank_target_r, blank_target_c, r, c);
        if (b_path == "FAILED") {
            cerr << "    [WARNING] Blank BFS Blocked! Attempting to wiggle blank out of stuck position.\n";
            // Wiggle Logic: ขยับช่องว่างหนีแบบสุ่ม เพื่อแก้บั๊กติดมุม
            int bdr[] = {-1, 1, 0, 0}; int bdc[] = {0, 0, -1, 1}; char b_act[] = {'D', 'U', 'R', 'L'};
            bool wiggled = false;
            for (int i = 0; i < 4; i++) {
                int nbr = br + bdr[i], nbc = bc + bdc[i];
                if (nbr >= 0 && nbr < N && nbc >= 0 && nbc < N && !locked[nbr][nbc] && !(nbr == r && nbc == c)) {
                    apply_move(b_act[i]);
                    wiggled = true;
                    break;
                }
            }
            if (!wiggled) {
                cerr << "    [FATAL] Complete Lockup. Cannot wiggle.\n";
                break;
            }
            continue; // คำนวณ Tile BFS ใหม่อีกรอบหลัง Wiggle
        }
        
        apply_moves(b_path); // ขยับช่องว่างไปรอ
        apply_move(tile_move); // ดึงกระเบื้องเข้าช่องว่าง
        
        r += dr; 
        c += dc;
    }
    cerr << "  [DEBUG] Tile successfully locked at (" << tr << "," << tc << ")\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N)) return 0;

    // อ่านกระดานเริ่มต้น
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> board[i][j];
            if (board[i][j] == -1) {
                br = i; bc = j;
            }
            locked[i][j] = false;
        }
    }

    // อ่านพื้นที่เป้าหมาย
    for (int i = 0; i < N - 2; i++) {
        for (int j = 0; j < N - 2; j++) {
            cin >> target[i][j];
        }
    }

    cerr << "[DEBUG] Board Size: " << N << "x" << N << " | Target Size: " << N-2 << "x" << N-2 << "\n";
    cerr << "[DEBUG] Initial Blank Position: (" << br << "," << bc << ")\n";
    cerr << "========================================================\n";

    // อัลกอริทึมหลัก: ต่อจิ๊กซอว์ทีละชิ้น จากซ้ายไปขวา, บนลงล่าง
    for (int tr = 1; tr <= N - 2; tr++) {
        for (int tc = 1; tc <= N - 2; tc++) {
            int needed_val = target[tr - 1][tc - 1];
            
            cerr << "[DEBUG] --- Solving Target (" << tr << "," << tc << ") | Needs value: " << needed_val << " ---\n";

            // หากระเบื้องที่ต้องการที่ "ใกล้ที่สุด" และยังไม่ถูกล็อค
            int best_dist = 1e9;
            int best_r = -1, best_c = -1;

            for (int r = 0; r < N; r++) {
                for (int c = 0; c < N; c++) {
                    if (board[r][c] == needed_val && !locked[r][c]) {
                        int dist = abs(r - tr) + abs(c - tc);
                        if (dist < best_dist) {
                            best_dist = dist;
                            best_r = r;
                            best_c = c;
                        }
                    }
                }
            }

            if (best_r != -1 && best_c != -1) {
                move_tile_to(best_r, best_c, tr, tc);
                locked[tr][tc] = true; // ล็อคช่องนี้ไว้ ห้ามขยับอีก
            } else {
                cerr << "[FATAL] Cannot find unlocked tile with value " << needed_val << "!\n";
            }
        }
    }

    ans += "S";
    
    cerr << "========================================================\n";
    cerr << "[DEBUG] SOLVED!\n";
    cerr << "[DEBUG] Total Moves Used: " << ans.length() - 1 << "\n"; // -1 เพราะไม่นับ 'S'
    
    cout << ans << "\n";

    return 0;
}