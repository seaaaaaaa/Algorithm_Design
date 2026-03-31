#include<iostream>
#include<vector>
#include<set>
#include<queue>
#include<algorithm>
#include<climits>
using namespace std;

int main(){
  int n,m,k;
  cin >> n >> m >>k;
  int f;
  cin >> f;
  vector<int> s(k);
  for(int i=0;i<k;i++){
    cin >> s[i];
  }
  vector<pair<int,pair<int,int>>> e(m);
  for(int i=0;i<k;i++){
    e.push_back({0,{n,s[i]}});
  }
  for(int i=0;i<m;i++){
    int a,b,w;
    cin >> a >> b >> w;
    e.push_back({w,{a,b}});
  }
  vector<int> dist(n+1,INT_MAX);
  dist[n]=0;
  for(int i=0; i<n;i++){
    for(auto &x: e){
      int w = x.first;
      int a = x.second.first;
      int b = x.second.second;
      if(dist[a]!=INT_MAX&&dist[b]>dist[a]+w){
        dist[b] = dist[a]+w;
      }
    }
  }


  cout << dist[f] << endl;
}
