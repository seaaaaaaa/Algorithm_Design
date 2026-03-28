#include <iostream>
#include <vector>
#include <set>
using namespace std;

size_t res=0;

void dfs(int i,vector<set<int>>& adj,int k,set<int> &f){
  f.insert(i);
  if(k==0){ 
    return;
  }
  for(auto &x:adj[i]){
    dfs(x,adj,k-1,f);
  }
}

int main(){
  int n,e,k;
  cin >> n >> e >> k;
  vector<set<int>> adj(n);
  for(int i=0;i<e;i++){
    int a,b;
    cin >> a >> b;
    adj[a].insert(b);
    adj[b].insert(a);
  }
  for(int i=0;i<n;i++){
    set<int> f;
    dfs(i,adj,k,f);
    res = max(res,f.size());
  }
  cout << res << endl;
}


    


