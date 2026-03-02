#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

int main() {
  
  int n,m;
  cin >> n >> m;
  vector<int> f(n);
  for(int i=0;i<n;i++)cin>>f[i]; 
  for(int i=0;i<m;i++){
    int aflag = 1;
    int sflag = 1;
    int prev = 0;
    map<int,int> m;
    for(int i=0;i<n;i++){
      int x;
      cin >> x;
      if(m.find(f[x-1])!=m.end()){
        if(m[f[x-1]]>=x-1) sflag = 0;
        else m[f[x-1]]=x-1;
      } else {
        m[f[x-1]]=x-1;
      }
      if(prev<=f[x-1]) {
        prev=f[x-1]; 
        //cout << "prev = "<<  prev << endl;
      } else {
        aflag = 0;
        //cout << prev << "-" << f[x-1] << " ";
      } 
    }
    if(!aflag) sflag=0;
    cout << aflag << " " << sflag << endl;
  }
}
