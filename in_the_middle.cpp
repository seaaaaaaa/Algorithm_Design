#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
#include <climits>

using namespace std;

int main(){
  int n;
  cin >> n;
  vector<int> t;
  int t1,t2,t3;
  cin >> t1 >> t2 >> t3;
  t.push_back(t1-1);
  t.push_back(t2-1);
  t.push_back(t3-1);
  vector<set<int>> adj(n);
  for(int i=0;i<n;i++){
    int k;
    cin >> k;
    for(int j=0;j<k;j++){
      int x;
      cin >> x;
      adj[i].insert(x-1);
    }
  }
  vector<vector<int>> dist(3,vector<int>(n,INT_MAX));

  for(int i=0;i<3;i++){
    dist[i][t[i]]=0;
    queue<int> q;
    q.push(t[i]);
    vector<bool> visited(n,false);
    visited[t[i]]=true;
    while(!q.empty()){
      for(auto &x :adj[q.front()]){
        if(visited[x]==false){
          visited[x]=true;
          dist[i][x]=dist[i][q.front()]+1;
          q.push(x);
        }
      }
      q.pop();
    }

  }
  int res = INT_MAX;
  for(int i=0;i<n;i++){
    if(dist[0][i]!=INT_MAX&&dist[1][i]!=INT_MAX&&dist[2][i]!=INT_MAX){
      int m = max(dist[0][i],max(dist[1][i],dist[2][i]));
      if(m<res)res=m;
    }
  }
  cout << res << endl;
}
