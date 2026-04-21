#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int res = 0;
int n;
vector<bool> col(12,false);
vector<int> r(12,-1);



bool save(int row, int i){
  for(int j=0;j<row;j++){
    if( i == r[j]|| row - i == j - r[j] || row+i == j+r[j] ){
      return false;
    }
  }
  return true;
}

void solve(int row){
  if(row == n) res++;
  for(int i=0;i<n;i++){
    if((!col[i]) && save(row,i)){
      r[row] = i;
      col[i] = true;
      solve(row+1);
      col[i] = false;
    }
  }
}


int main(){
  cin >> n;
  solve(0);
  cout << res;
}
