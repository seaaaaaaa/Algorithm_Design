#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

int main(){
    int n;
    cin >> n;
    int x = 16777216; // 2^24
    int count = 0;
    while(n!=0){
        int s = log2(n);
        //cout << s << endl;
        if(n*2<x){
            while(n!=pow(2,s+1)&&n!=0){
                n = (n+1)%x;
                count++;
            }
            s+=1;
        }    
        //cout << s << endl;
        //cout << n << endl;
        while(s!= 24){
            n = (n*2)%x;
            count++;
            s++;
        }
    }
    cout << count << endl;
    return 0;
}