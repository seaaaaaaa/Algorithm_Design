#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

int main(){
  float W;
  int n;
  cin >> W >>n;
  vector<double> v(n);
  vector<double> w(n);
  vector<pair<double,int>> r;
  for(int i=0;i<n;i++){
    cin >> v[i];
  }
  for(int i=0;i<n;i++){
    cin >> w[i];
    r.push_back({v[i]/w[i],i});
  }
  sort(r.rbegin(),r.rend());
  double res = 0;
  for(auto &x:r){
    if(W>=w[x.second]){
      res+=v[x.second];
      W-=w[x.second];
    } else {
      double want = W/w[x.second];
      res+=want*v[x.second];
      W-=want*w[x.second];
    }
    //cout << x.first<< " "<< x.second << " " << W <<endl;
    if(W==0)break;
  }
  cout << fixed << setprecision(4);
  cout << res << endl;
}

