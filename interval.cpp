#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

int main(){
  int n;
  cin >> n;
  map<int,int> m;
  vector<int> s(n);
  vector<int> f(n);
  for(int i = 0; i<n ;i++){
    cin >> s[i];
  }

  for(int i = 0; i<n; i++){
    cin >> f[i];
  }

  vector<pair<int,int>> job(n);
  for(int i = 0;i<n;i++){
    job[i]={f[i],s[i]};
  }
  sort(job.begin(),job.end());
  for(int i = 0;i<n;i++){
    s[i]=job[i].second;
    f[i]=job[i].first;
  }

  int last = -1;
  int res=0;
  for(int i=0; i<n; i++){
    //cout << s[i] << " " <<last<<endl;
    if(s[i]>=last){
      res+=1;
      last = f[i];
    }
  }
  cout << res <<endl;

  
}
