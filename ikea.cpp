#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;

int main(){
  int n,e;
  cin >> n >> e;
  vector<set<int>> adj(n);
  for(int i=0;i<e;i++){
    int a,b;
    cin >> a >> b;
    adj[b-1].insert(a-1);
  }
  for(int i=0;i<5;i++){
    vector<bool> done(n,false);
    bool valid = true;
    for(int j=0;j<n;j++){
      int s;
      cin >> s;
      bool ready = true;
      for(auto&x:adj[s-1]){
        if(done[x]==false){
          ready = false;
          valid =false;
          break;
        }
      }
      if(ready){
        done[s-1] = true;
      }
    }
    if(valid){
      cout << "SUCCESS" << endl;
    } else {
      cout << "FAIL" << endl;
    }


  }
}
