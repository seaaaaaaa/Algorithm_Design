#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

int main(){
  int n,e,s;
  cin >> n >> e >> s;
  vector<vector<pair<int,int>>> adj(n);
  for(int i = 0;i < e; i++){
    int a,b,W;
    cin >> a >> b >> W;
    adj[a].push_back({b,W});
  }

  vector<int> dist(n,INT_MAX);
  dist[s]=0;
  int l;
  vector<int> prev(n,-1);
  for(int i = 1; i<=n-1; i++){
    for(int x=0; x<adj.size();x++){
      if(dist[x]!=INT_MAX){
        for(auto &y:adj[x]){
          if(y.first!=INT_MAX){
            int w = y.second;
            int v = y.first;
            if(dist[v]>dist[x]+w){
              dist[v] = dist[x]+w;
              prev[v] = x;
              l=v;
            }
          }
        } 
      }
    }
  }
  for(int x=0;x<adj.size();x++){
    if(dist[x]!=INT_MAX){
      for(auto &y:adj[x]){
        if(y.first!=INT_MAX){
          if(dist[y.first]>dist[x]+y.second){
            cout << -1 << endl;
            return 0;
          }
        }
      }
    }
  }

  for(int i=0;i<n;i++){
    cout << dist[i] << " ";
  }
  cout <<endl;

}
