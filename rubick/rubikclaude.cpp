#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <string>
#include <algorithm>
#include <climits>
#include <utility>
using namespace std;

int N;
vector<vector<int>> board;
vector<vector<int>> target;
string moves_output;
set<pair<int,int>> locked;

int bR, bC;

const int cmd_bdr[] = {1, -1, 0, 0};
const int cmd_bdc[] = {0, 0, 1, -1};
const char cmdChar[] = {'U','D','L','R'};

bool inBounds(int r, int c) {
    return r>=0 && r<N && c>=0 && c<N;
}

void applyCmd(int cmd) {
    int nr = bR + cmd_bdr[cmd];
    int nc = bC + cmd_bdc[cmd];
    board[bR][bC] = board[nr][nc];
    board[nr][nc] = -1;
    bR = nr; bC = nc;
    moves_output += cmdChar[cmd];
}

bool moveBlankTo(int tr, int tc, int avoidR=-1, int avoidC=-1) {
    if (bR==tr && bC==tc) return true;
    
    vector<vector<int>> dist(N, vector<int>(N, -1));
    vector<vector<pair<int,int>>> prev(N, vector<pair<int,int>>(N, {-1,-1}));
    vector<vector<int>> prevCmd(N, vector<int>(N, -1));
    
    queue<pair<int,int>> q;
    q.push({bR, bC});
    dist[bR][bC] = 0;
    
    while (!q.empty()) {
        auto [r,c] = q.front(); q.pop();
        if (r==tr && c==tc) break;
        for (int cmd=0; cmd<4; cmd++) {
            int nr = r + cmd_bdr[cmd];
            int nc = c + cmd_bdc[cmd];
            if (!inBounds(nr,nc)) continue;
            if (dist[nr][nc] != -1) continue;
            if (locked.count({nr,nc})) continue;
            if (nr==avoidR && nc==avoidC) continue;
            dist[nr][nc] = dist[r][c]+1;
            prev[nr][nc] = {r,c};
            prevCmd[nr][nc] = cmd;
            q.push({nr,nc});
        }
    }
    
    if (dist[tr][tc] == -1) return false;
    
    vector<int> path;
    int r=tr, c=tc;
    while (prevCmd[r][c] != -1) {
        path.push_back(prevCmd[r][c]);
        auto [pr,pc] = prev[r][c];
        r=pr; c=pc;
    }
    reverse(path.begin(), path.end());
    for (int cmd : path) applyCmd(cmd);
    return true;
}

vector<pair<int,int>> findTilePath(int tileR, int tileC, int destR, int destC) {
    if (tileR==destR && tileC==destC) return {};
    
    vector<vector<int>> dist(N, vector<int>(N, -1));
    vector<vector<pair<int,int>>> prev_pos(N, vector<pair<int,int>>(N, {-1,-1}));
    queue<pair<int,int>> q;
    q.push({tileR, tileC});
    dist[tileR][tileC] = 0;
    
    const int dr_arr[] = {1,-1,0,0};
    const int dc_arr[] = {0,0,1,-1};
    
    while (!q.empty()) {
        auto [r,c] = q.front(); q.pop();
        if (r==destR && c==destC) break;
        for (int d=0; d<4; d++) {
            int nr = r+dr_arr[d];
            int nc = c+dc_arr[d];
            if (!inBounds(nr,nc)) continue;
            if (locked.count({nr,nc})) continue;
            if (dist[nr][nc] != -1) continue;
            dist[nr][nc] = dist[r][c]+1;
            prev_pos[nr][nc] = {r,c};
            q.push({nr,nc});
        }
    }
    
    if (dist[destR][destC] == -1) return {};
    
    vector<pair<int,int>> steps;
    int r=destR, c=destC;
    while (!(r==tileR && c==tileC)) {
        auto [pr,pc] = prev_pos[r][c];
        steps.push_back({r-pr, c-pc});
        r=pr; c=pc;
    }
    reverse(steps.begin(), steps.end());
    return steps;
}

bool moveTileTo(int tileR, int tileC, int destR, int destC) {
    int r = tileR, c = tileC;
    
    auto steps = findTilePath(r, c, destR, destC);
    if (steps.empty() && (r!=destR || c!=destC)) return false;
    
    for (auto [dr, dc] : steps) {
        if (!moveBlankTo(r+dr, c+dc, r, c)) return false;
        
        int pushCmd = -1;
        for (int cmd=0; cmd<4; cmd++) {
            if (cmd_bdr[cmd]==-dr && cmd_bdc[cmd]==-dc) { pushCmd=cmd; break; }
        }
        if (pushCmd==-1) return false;
        
        applyCmd(pushCmd);
        r += dr; c += dc;
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N;
    board.assign(N, vector<int>(N));
    
    for (int i=0; i<N; i++)
        for (int j=0; j<N; j++) {
            cin >> board[i][j];
            if (board[i][j] == -1) { bR=i; bC=j; }
        }
    
    int M = N-2;
    target.assign(M, vector<int>(M));
    for (int i=0; i<M; i++)
        for (int j=0; j<M; j++)
            cin >> target[i][j];
    
    // Check if already solved
    bool solved = true;
    for (int i=0; i<M && solved; i++)
        for (int j=0; j<M && solved; j++)
            if (board[i+1][j+1] != target[i][j]) solved=false;
    
    if (solved) {
        cout << "S\n";
        return 0;
    }
    
    const int dr4[] = {1,-1,0,0};
    const int dc4[] = {0,0,1,-1};
    
    // Process target cells in order
    for (int i=0; i<M; i++) {
        for (int j=0; j<M; j++) {
            int br = i+1, bc = j+1;
            int needed = target[i][j];
            
            if (board[br][bc] == needed) {
                locked.insert({br,bc});
                continue;
            }
            
            // BFS from (br,bc) to find distances to all reachable cells (reverse BFS)
            vector<vector<int>> dist(N, vector<int>(N, INT_MAX/2));
            queue<pair<int,int>> q;
            dist[br][bc] = 0;
            q.push({br,bc});
            while (!q.empty()) {
                auto [r,c] = q.front(); q.pop();
                for (int d=0; d<4; d++) {
                    int nr=r+dr4[d], nc=c+dc4[d];
                    if (!inBounds(nr,nc)) continue;
                    if (locked.count({nr,nc})) continue;
                    if (dist[nr][nc] != INT_MAX/2) continue;
                    dist[nr][nc] = dist[r][c]+1;
                    q.push({nr,nc});
                }
            }
            
            // Find tile with minimum BFS distance
            int bestR=-1, bestC=-1, bestDist=INT_MAX;
            for (int r=0; r<N; r++) {
                for (int c=0; c<N; c++) {
                    if (locked.count({r,c})) continue;
                    if (r==bR && c==bC) continue;
                    if (board[r][c] != needed) continue;
                    if (dist[r][c] < bestDist) {
                        bestDist = dist[r][c];
                        bestR=r; bestC=c;
                    }
                }
            }
            
            if (bestR == -1) continue;
            
            moveTileTo(bestR, bestC, br, bc);
            locked.insert({br,bc});
        }
    }
    
    moves_output += 'S';
    cout << moves_output << "\n";
    
    return 0;
}