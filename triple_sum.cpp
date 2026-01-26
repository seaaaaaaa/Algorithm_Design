#include <iostream>
#include <vector>
4 5
-2 1 5 6
1 3 5 6 7#include <algorithm>
#include <set>
using namespace std;

bool check(int m,vector<int> v){
  
  int n = v.size();
  for(int i=0;i<n-2;i++){
    int l=i+1;
    int r=n-1;
    while(l<r){
      int res = v[i]+v[l]+v[r];
      if(res<m) l++;
      else if(res > m) r--;
      else return true;
    }
  }
  return false;
}

int main(){
  int n,m;
  cin >> n >> m ;
  vector<int> v(n);
  for(int i = 0; i < n; i++){
    int x;
    cin >> v[i];
  }
  for(int i = 0; i < m; i++){
    int y;
    cin >> y;
    if(check(y,v)) cout << "YES" << endl;
    else cout << "NO" << endl;
  }
}
