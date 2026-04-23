#include <iostream>
#include <map>
using namespace std;

int main(){
  int n;
  cin >> n;
  map<long long ,long long > m;
  for(int i=0;i<n;i++){
    int x;
    cin >> x;
    m[x]+=1;
  }
  long long res = 0;
  for(auto &x : m){
    if(x.second>1){
      res += x.second*x.second;
    }
  }
  cout <<  res;
}
