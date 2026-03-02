#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;





int main(){
  
  
  string s1,s2;
  cin >> s1 >> s2;
  vector<vector<int>> v(s1.length()+1,vector<int>(s2.length()+1));
  for(int i = 1; i<=s1.length();i++){
    for(int j = 1; j<=s2.length();j++){
      if(i==0||j==0) v[i][j]=0; 
      if(i>0&&j>0&&s1[i-1]==s2[j-1]){
        v[i][j] = v[i-1][j-1]+1;
        continue;
      }
      if(i>0&&j>0&&s1[i-1]!=s2[j-1]) v[i][j] = max(v[i-1][j],v[i][j-1]);
    }
  }
  
  for(auto& x :v) {
    for(auto & y : x) cout << y << " ";
    cout << endl;
  }
  

  cout << v[s1.length()][s2.length()];
}
