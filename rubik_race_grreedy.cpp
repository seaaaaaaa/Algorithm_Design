#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

using namespace std;

int N;
int board[70][70], target[70][70];
bool locked[70][70];
int br, bc;
string res = "";

bool is_safe(int r, int c) { return r >= 0 && r < N && c >= 0 && c < N; }

// กฎหน้า 2: U = แผ่นล่างขึ้น(BR+1), D = แผ่นบนลง(BR-1), L = แผ่นขวามาซ้าย(BC+1), R = แผ่นซ้ายมาขวา(BC-1)
void move_tile(char m) {
    int nr = br, nc = bc;
    if (m == 'U') nr = br + 1;
    else if (m == 'D') nr = br - 1;
    else if (m == 'L') nc = bc + 1;
    else if (m == 'R') nc = bc - 1;

    if (is_safe(nr, nc)) {
        swap(board[br][bc], board[nr][nc]);
        br = nr; bc = nc;
        res += m;
    }
}

void path_blank(int tr, int tc, int ar, int ac) {
    if (br == tr && bc == tc) return;
    queue<pair<int, int>> q; q.push({br, bc});
    pair<int, int> parent[70][70]; char move_dir[70][70];
    for(int i=0; i<N; i++) for(int j=0; j<N; j++) parent[i][j] = {-1, -1};
    parent[br][bc] = {br, bc};
    
    int dr[] = {1, -1, 0, 0}, dc[] = {0, 0, 1, -1};
    char mc[] = {'U', 'D', 'L', 'R'}; 

    bool found = false;
    while(!q.empty()){
        pair<int,int> cur = q.front(); q.pop();
        if(cur.first == tr && cur.second == tc) { found = true; break; }
        for(int i=0; i<4; i++){
            int nr = cur.first+dr[i], nc = cur.second+dc[i];
            if(is_safe(nr, nc) && !locked[nr][nc] && !(nr==ar && nc==ac)){
                if(parent[nr][nc].first == -1){
                    parent[nr][nc] = cur; move_dir[nr][nc] = mc[i];
                    q.push({nr, nc});
                }
            }
        }
    }
    // หากหาทางปกติไม่ได้ (โดนขัง) ให้เดินทับ Locked ได้เพื่อแก้ WA/Illegal Move
    if(!found){
        q = queue<pair<int, int>>(); q.push({br, bc});
        for(int i=0; i<N; i++) for(int j=0; j<N; j++) parent[i][j] = {-1, -1};
        parent[br][bc] = {br, bc};
        while(!q.empty()){
            pair<int,int> cur = q.front(); q.pop();
            if(cur.first == tr && cur.second == tc) { found = true; break; }
            for(int i=0; i<4; i++){
                int nr = cur.first+dr[i], nc = cur.second+dc[i];
                if(is_safe(nr, nc) && !(nr==ar && nc==ac)){
                    if(parent[nr][nc].first == -1){
                        parent[nr][nc] = cur; move_dir[nr][nc] = mc[i];
                        q.push({nr, nc});
                    }
                }
            }
        }
    }
    if(found){
        string p = ""; int cr = tr, cc = tc;
        while(cr != br || cc != bc){
            p += move_dir[cr][cc];
            pair<int,int> prev = parent[cr][cc]; cr = prev.first; cc = prev.second;
        }
        reverse(p.begin(), p.end());
        for(char c : p) move_tile(c);
    }
}

void solve(int tr, int tc, int val) {
    int sr = -1, sc = -1, best_d = 1000;
    for(int r=0; r<N; r++) for(int c=0; c<N; c++)
        if(!locked[r][c] && board[r][c] == val)
            if(abs(r-tr)+abs(c-tc) < best_d) { best_d = abs(r-tr)+abs(c-tc); sr = r; sc = c; }
    
    while(sr != tr || sc != tc){
        int nr = sr, nc = sc;
        if(sc > tc) nc--; else if(sc < tc) nc++;
        else if(sr > tr) nr--; else if(sr < tr) nr++;
        path_blank(nr, nc, sr, sc);
        if(br == sr+1) move_tile('U'); else if(br == sr-1) move_tile('D');
        else if(bc == sc+1) move_tile('L'); else if(bc == sc-1) move_tile('R');
        sr = nr; sc = nc;
    }
    locked[tr][tc] = true;
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    if(!(cin >> N)) return 0;
    for(int i=0; i<N; i++) for(int j=0; j<N; j++){
        cin >> board[i][j]; if(board[i][j] == -1) { br = i; bc = j; }
    }
    for(int i=0; i<N-2; i++) for(int j=0; j<N-2; j++) cin >> target[i][j];

    // Shell solving (ทำแถว k แล้วตามด้วยคอลัมน์ k) เพื่อเปิดพื้นที่
    for (int k = 0; k < N-2; k++) {
        // แก้แถวที่ k+1 (เริ่มจากคอลัมน์ k+1 ไปทางขวา)
        for (int c = 1 + k; c <= N-2; c++) solve(1 + k, c, target[k][c-1]);
        // แก้คอลัมน์ที่ k+1 (เริ่มจากแถว k+2 ลงมา)
        for (int r = 2 + k; r <= N-2; r++) solve(r, 1 + k, target[r-1][k]);
    }

    // ลบ move ที่สวนทางกันเพื่อลดจำนวนก้าว
    string fin = "";
    for(char c : res) {
        if(!fin.empty() && ((fin.back()=='U'&&c=='D')||(fin.back()=='D'&&c=='U')||(fin.back()=='L'&&c=='R')||(fin.back()=='R'&&c=='L')))
            { fin.pop_back(); continue; }
        fin += c;
    }
    cout << fin << "S" << endl;
    return 0;
}
