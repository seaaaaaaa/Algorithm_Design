#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <string>
#include <climits>
#include <queue>
using namespace std;

int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};

void bfs(vector<vector<int>> &m, int r, int c) {
  queue<pair<int, int>> q;
  
  // 1. Initialize the start point
  m[0][0] = 0; 
  q.push({0, 0});
  
  while (!q.empty()) {
    int x = q.front().first;
    int y = q.front().second;
    q.pop();
    
    // 2. Check all 4 neighbors
    for (int i = 0; i < 4; i++) {
      int nx = x + dx[i];
      int ny = y + dy[i];
      
      // 3. If neighbor is within bounds, is not a wall (-2), 
      // AND we found a strictly shorter path to it:
      if (nx >= 0 && ny >= 0 && nx < r && ny < c && m[nx][ny] != -2) {
        if (m[nx][ny] > m[x][y] + 1) {
          m[nx][ny] = m[x][y] + 1; // Update distance
          q.push({nx, ny});        // Add to queue to explore its neighbors later
        }
      }
    }
  }
}


int main(){
  int r,c;
  cin >> r >> c;
  vector<vector<int>> m(r,vector<int>(c));
  for(int i=0;i<r;i++){
    string x;
    cin >> x;
    for(int j=0;j<c;j++){
      if (x[j]=='.')m[i][j]=INT_MAX;
      else m[i][j]=-2;
    }
  } 
  bfs(m,r,c);
  if(m[r-1][c-1]!=INT_MAX){
    cout << m[r-1][c-1] << endl;
  } else {
    cout << -1 << endl;
  }
}
