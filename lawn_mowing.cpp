#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main(){
  ios_base::sync_with_stdio(false); cin.tie(NULL);
  long n,m,k;
  cin >> n >> m >> k;
  vector<long> v(n);
  for(long i=0; i<n; i++){
    long x;
    cin >> x;
    if(i==0){
      v[i]=x;
      continue;
    }

    v[i] = x+v[i-1];

  }
  for(auto &x:v) cout << x << " ";
  for(long i=0; i<m; i++){
    long a,b;
    cin >> a >> b;
    long adj=v[a-1];
    for(long i=0;i<n;i++){
      v[i]=v[i]-adj+((i-a+1)*k);
      cout << v[i] << " ";
    }
    cout << endl;
    long l = upper_bound(v.begin()+a,v.end(),b)-v.begin()-1;
    cout << l << endl;

    if(l<0) cout << 0 << endl;
    long res = r[i];
    if(a!=0) res = r[i]-r[a-1];
    cout << res << endl;
  }
}
