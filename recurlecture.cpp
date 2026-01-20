int test(int n,int current) {
  if (current == n ) return n;
  else{
    return  current + test(n ,current+1 );
  }
}
