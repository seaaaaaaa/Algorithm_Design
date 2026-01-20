#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;

int main(){
  int n;
  cin >> n;
  set<pair<int,int>> w;
  for(int i = 0; i < n; i++){
    int l,r;
    cin >> l >> r;
    if (l>r) continue;

    auto it = w.lower_bound({l,-2e9});

    if(it != w.begin()){
      it--;
      if(it->second < l-1) {
        it++;
      }
    }
    while (it!=w.end()&&it->first <= r+1) {
      l = std::min(l, it->first);
      r = std::max(r, it->second);

      it = w.erase(it);
    }
    w.insert({l,r});
  }

  for(auto &p : w) cout << p.first << " " << p.second<<" ";

  //cout << endl << "=="<< w.size() << "==";
}

     
      
