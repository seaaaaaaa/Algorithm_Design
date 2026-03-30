#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;

int main(){
  int n;
  cin >> n;
  vector<set<int>> preq(n);
  vector<set<int>> adj(n);
  set<int> nsuc;
  vector<bool> suc(n,false);
  vector<int> res;
  for(int i=0;i<n;i++){
    nsuc.insert(i);
    int m;
    cin >> m;
    for(int j=0;j<m;j++){
      int x;
      cin >> x;
      preq[i].insert(x);
      adj[x].insert(i);
    }
  }
/*
  for(auto &x:adj){
    for(auto &y:x){
      cout << y << " ";
    }
  }
  cout << endl;

  for(auto &x:preq){
    for(auto &y:x){
      cout << y << " ";
    }
    cout << endl;
  }
*/
  while(!nsuc.empty()){
    bool found = false;
    for(auto&x:nsuc){
      if(!suc[x]&&preq[x].empty()){
        suc[x]=true;
        res.push_back(x);
        for(auto &y:adj[x]){
          preq[y].erase(x);
        }
        found = true;
        nsuc.erase(x);
        break;
      }
    }

    if(!found) break;
  }

  for(auto &x: res)cout << x << " ";
}
