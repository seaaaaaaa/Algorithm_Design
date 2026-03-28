#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;

int main(){
  int n;
  cin >> n;
  vector<set<int>> adj(n);
  for(int i=0;i<n;i++){
    int x,y;
    cin >> x >> y;
    adj[x].insert(y);
    adj[y].insert(x);
  }
  while(true){
    for(int i=0;i<n;i++){
      if(adj[i].size()==1){
        int j=*(adj[i].begin());
        adj[i].erase(j);
        adj[j].erase(i);
      }
    }
    bool c=true;
    for(int i=0;i<n;i++){
      if(adj[i].size()==1) c=false;
    }
    if(c) break;
  }
  int res=0;
  for(int i=0;i<n;i++){
    if(adj[i].size()==2)res++;
  }
  cout << res << endl;
} 
