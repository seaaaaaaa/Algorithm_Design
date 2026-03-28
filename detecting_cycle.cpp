#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;


bool dfs(int i,vector<set<int>> &adj, vector<bool> &visited, int parent){
  visited[i]= true;
  for(auto &x:adj[i]){
    if(visited[x]==false){
      if(dfs(x,adj,visited,i))return true;
    } else if(x!=parent){
      return true;
    }
  }
  return false;
}

void circuit_detect(vector<set<int>> &adj){
  vector<bool> visited(adj.size(),false);
  bool has_cycle = false;
  for(int i=0;i<adj.size();i++){
    if(visited[i]==false){
      if(dfs(i,adj,visited,-1)){
        has_cycle = true;
        break;
      }
    }
  }
  if(has_cycle){
    cout << "YES" << endl;
  } else {
    cout << "NO" << endl;
  }
}

int main(){
  int t;
  cin >> t;
  for(int i=0;i<t;i++){
    int n,e;
    cin >> n >> e;
    vector<set<int>> adj(n);
    for(int j=0;j<e;j++){
      int a,b;
      cin >> a >> b;
      adj[a].insert(b);
      adj[b].insert(a);
    }
    circuit_detect(adj);
  }
}
