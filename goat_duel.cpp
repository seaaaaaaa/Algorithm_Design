#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <queue>
using namespace std;

int main(){
  long long n;
  cin >> n;
  priority_queue<long long,vector<long long>,greater<long long>> a;
  for(long long i=0;i<n;i++){
    int x;
    cin >> x;
    a.push(x);
  }
  long long x1 = a.top();
  a.pop();
  long long res = 0;
  while(!a.empty()){
    long long x2 = a.top();
    a.pop();
    res +=(x1*x2);
    x1 = x2;
  }
  cout << res;
} 

