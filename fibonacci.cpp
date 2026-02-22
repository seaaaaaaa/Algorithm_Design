#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;



int fibo(int n){
  if(n==0) return 1 
  if(n==1) return 1;
  if(n==2) return 2;
  return fibo(n-1)+fibo(n-2);
}

int main(){
  int n;
  cin >> n;
  cout << fibo(n-1) << endl;
}
