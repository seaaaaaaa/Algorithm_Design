#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;




int main() {
  int n;
  cin >> n;
  vector<vector<int>> v(n,vector<int>(n));
  int res = -100000000;
  for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
      cin >> v[i][j];
      if(i-1>=0&&j-1>=0){
        v[i][j]=std::max(v[i-1][j-1]+v[i][j],v[i][j]);
      }
      res = std::max(res,v[i][j]);
    }
  }
  cout << res << endl;
}


  



