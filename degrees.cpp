#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

int main(){
  std::ios_base::sync_with_stdio(false); std::cin.tie(0);
  int n;
  cin >> n;
  map<int,int> m;
  int k = 0;
  for(int i=0; i<n; i++){
    int degree = 0;
    for(int j=0; j<n; j++){
      int x;
      cin >> x;
      degree+=x;
    }
    if (k<degree) k=degree;
    m[degree]+=1;
  }

  for(int i=0; i<k; i++){
    if(m.find(i)==m.end())m[i]=0;
  }
  for(auto &x:m) cout << x.second <<" ";
}
