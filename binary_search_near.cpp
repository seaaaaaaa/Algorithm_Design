#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

template <typename T>
int bsearch(vector<T> &v, T k,int start, int stop) {
  if (start == stop){
    //cout << start << " ";
    if(v[start]<=k) return start;
    else if(start==0)return -1;
    else return start-1;
  }
  int m = (start+stop) >> 1; //bitwise shift right
  if (v[m] >= k) return bsearch(v,k,start,m);
  else return bsearch(v,k,m+1,stop);
}
template <typename T>
int bsearch(vector<T> &v, T k) {
  return bsearch(v,k,0,v.size()-1);
}

int main(){
  int n,m;
  cin >> n >> m;
  vector<int> v(n);
  for(int i=0; i<n; i++){
    cin >> v[i];
  }
  for(int i=0; i<m; i++){
    int x;
    cin >> x;
    int index = bsearch(v,x);
    //cout << index << " ";
    if(index==-1) cout << -1 <<endl;
    else cout << v[index] << endl;
  }
  return 0;
}
