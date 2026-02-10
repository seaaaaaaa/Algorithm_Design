#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>
using namespace std;


long long calculate_min_power(const multiset<int>& avg, int a, int b, int s, int e) {
    //cout << s << " " << e << endl;
    // Count how many Avengers are in the current range [s, e]
    auto it_start = avg.lower_bound(s+1);
    auto it_end = avg.upper_bound(e+1);
    long long count = distance(it_start, it_end);

    // Case 1: No Avengers in this range
    if (count == 0) {
        return a;
    }

    // Power cost for the current range as a whole
    long long current_range_power = (long long)b * count * (e - s+1);

    // Case 2: Base case - range size is 1 and it contains Avengers
    if (s == e) {
        return current_range_power;
    }

    // Case 3: Recursive step - Split the range
    int mid = s + ((e - s+1) / 2)-1;
    long long split_power = calculate_min_power(avg, a, b, s, mid) + 
                            calculate_min_power(avg, a, b, mid + 1, e);

    // Return the minimum of destroying it now or splitting it
    //cout << min(current_range_power,split_power) << endl;
    return min(current_range_power, split_power);
}


int main(){
  int p,k,a,b;
  cin >> p >> k >> a >> b;
  multiset<int> avg;
  for(int i=0;i<k;i++){
    int x;
    cin >> x;
    avg.insert(x);
  }
  cout << calculate_min_power(avg,a,b,0,std::pow(2,p)-1);

}

