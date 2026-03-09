#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <queue>
#include <stack>
using namespace std;

int main(){
  int n,m;
  cin >> n>>m;
  vector<int> task(m);
  for(int i=0;i<m;i++)cin >> task[i];
  sort(task.begin(),task.end());
  double total = 0;
  priority_queue<int,vector<int>,greater<int>> free;
  for (int i = 0; i < n; i++) {
        free.push(0);
  }
  vector<long long> submission_times;
  for(int task_duration: task){
    long long earliest = free.top(); 
    free.pop();
    long long completion = earliest+task_duration;
    submission_times.push_back(completion);
    free.push(completion);
  }
  for(auto &x:submission_times)total+=x;
  double res = total/m;
  cout << fixed << setprecision(4) << res;
}
