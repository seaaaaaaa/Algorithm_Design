#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

int check(vector<int> &v, int s, int e){
  if(s+1==e) return v[s]+v[e];
  int m = s+((e-s+1)/2)-1;
  int l = check(v,s,m);
  int r = check(v,m+1,e);
  if(l==-1||r==-1) return -1;
  if(abs(l-r)<= 1) {
    return l+r;
  } else {
    return -1;
  }
}

int main(){
  int n,k;
  cin >> n >> k;
  for(int i = 0 ; i < n; i++){
    vector<int> v(std::pow(2,k));
    for(int j = 0 ; j < std::pow(2,k) ; j++) cin >> v[j];
    if(check(v,0,v.size()-1)==-1) cout << "no" << endl;
    else cout << "yes" << endl;
  }
  return 0;
}

