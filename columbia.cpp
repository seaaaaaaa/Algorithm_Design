#include <iostream>
#include <vector>
#include <climits>
#include <queue>
using namespace std;

int dr[]={1,-1,0,0};
int dc[]={0,0,1,-1};

void bfs(int x, int y,vector<vector<int>> &fee,vector<vector<int>> &m){
  queue<pair<int,int>> q;
  m[0][0] = 0;
  q.push({x,y});
  while(!q.empty()){
    for(int i=0;i<4;i++){
      int nr = q.front().first + dr[i];
      int nc = q.front().second + dc[i];
      if(nr>=0&&nr<m.size()&&nc>=0&&nc<m[0].size()){
        if(m[nr][nc]>m[q.front().first][q.front().second]+fee[nr][nc]){
          m[nr][nc] = m[q.front().first][q.front().second]+fee[nr][nc];
          q.push({nr,nc});
        }
      }
    }
    q.pop();
  }
}

int main(){
  int r,c;
  cin >> r >> c;
  vector<vector<int>> fee(r,vector<int>(c));
  for(int i=0;i<r;i++){
    for(int j=0;j<c;j++){
      cin >> fee[i][j];
    }
  }
  vector<vector<int>> m(r,vector<int>(c,INT_MAX));
  bfs(0,0,fee,m);
  for(auto &x:m){
    for(auto &y:x){
      cout << y << " ";
    }
    cout << endl;
  }
}
