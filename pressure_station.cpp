#include <iostream>
#include <algorithm>
#include <vector>
#include <climits>
using namespace std;

int main(){
  int n,k;
  cin >> n >> k;
  vector<int> p(n+1);
  for(int i = 1; i<=n; i++) cin >> p[i];
  vector<int> dp(n+1,INT_MAX);
  dp[0] =0;
  for(int i = 1; i<=n;i++){
    if(dp[i-1]==INT_MAX)continue;
    for(int j = max(1,i-k);j<=min(n,i+k);j++){
      int lastCovered = min(n,j+k);
      if(dp[i-1]!= INT_MAX){
        dp[lastCovered] = min(dp[lastCovered],dp[i-1]+p[j]);
      }
    }
  }
  cout << dp[n] << endl;
}
