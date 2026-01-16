#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void perm_kn_replace(int n,vector<int> &sol,int len,int k,int m) {
  if (len < k) {
    for (int i = 0;i<n;i++) {      
      sol[len] = i;
      perm_kn_replace(n,sol,len+1,k,m);
    }
  } else {
    int count=0;
    for(auto &x :sol){
      if(x==1) count++;
    }
    if(count==m){
      for (auto &x : sol) cout << x;
      cout << endl;
    }  
  }
}

int main(){
  int n,m;
  cin >> n >> m;
  vector<int> sol(m);
  perm_kn_replace(2,sol,0,m,n);
  return 0;
}


