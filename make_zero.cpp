#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int main() {
    int x;
    cin >> x;
    
    // กรณีพิเศษ: ถ้า x = 0 อยู่แล้ว ไม่ต้องทำอะไร
    if (x == 0) {
        cout << 0 << endl;
        return 0;
    }
    
    // สร้าง array เก็บระยะทางสั้นสุดไปยังแต่ละสถานะ
    // ใช้ -1 แทนการที่ยังไม่เคยเยี่ยมชม
    const int MOD = 16777216;
    vector<int> dist(MOD, -1);
    
    // สร้าง queue สำหรับ BFS
    queue<int> q;
    
    // เริ่มต้นจากสถานะ x ด้วยระยะทาง 0
    dist[x] = 0;
    q.push(x);
    
    // BFS หาเส้นทางสั้นสุดไปยังสถานะ 0
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        
        // ลอง operation ทั้งสองแบบ
        // Operation 1: (current + 1) mod MOD
        int next1 = (current + 1) % MOD;
        if (dist[next1] == -1) {  // ยังไม่เคยเยี่ยมชม
            dist[next1] = dist[current] + 1;
            
            // ถ้าไปถึงเป้าหมาย (0) แล้ว
            if (next1 == 0) {
                cout << dist[next1] << endl;
                return 0;
            }
            
            q.push(next1);
        }
        
        // Operation 2: (current * 2) mod MOD
        int next2 = (current * 2) % MOD;
        if (dist[next2] == -1) {  // ยังไม่เคยเยี่ยมชม
            dist[next2] = dist[current] + 1;
            
            // ถ้าไปถึงเป้าหมาย (0) แล้ว
            if (next2 == 0) {
                cout << dist[next2] << endl;
                return 0;
            }
            
            q.push(next2);
        }
    }
    
    // ตามทฤษฎีแล้วจะต้องไปถึง 0 ได้เสมอ
    // เพราะ gcd(1, MOD) = 1 และ gcd(2, MOD) = 2
    // สามารถสร้างจำนวนเต็มใดก็ได้ในช่วง [0, MOD-1]
    cout << -1 << endl;  // กรณีที่ไม่น่าจะเกิดขึ้น
    
    return 0;
}