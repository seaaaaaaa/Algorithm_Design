#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    // ปรับความเร็วในการรับส่งข้อมูล
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    // หากต้องการแบ่งกองมากกว่าจำนวนหินที่มี จะแบ่งไม่ได้เลย (จำนวนวิธีเป็น 0)
    if (n < k) {
        cout << 0 << "\n";
        return 0;
    }

    // สร้างตาราง DP 2 มิติ ขนาด (n+1) x (k+1) 
    // ใช้หน่วยความจำนิดเดียว ซึ่งผ่านเกณฑ์ 512MB สบายๆ
    vector<vector<long long>> dp(n + 1, vector<long long>(k + 1, 0));

    // Base Case
    dp[0][0] = 1; 

    // เริ่มคำนวณ Bottom-Up DP
    for (int i = 1; i <= n; ++i) {
        // หิน i ก้อน แบ่งได้มากสุดคือ i กอง (หรือ k กอง ตามโจทย์)
        for (int j = 1; j <= min(i, k); ++j) {
            
            // กรณีแยกเป็นกองใหม่
            long long new_pile = dp[i - 1][j - 1];
            
            // กรณีไปเติมในกองที่มีอยู่แล้ว (คูณ j เพราะเลือกใส่ได้ j กอง)
            long long existing_piles = (j * dp[i - 1][j]) % 1997;

            // นำผลลัพธ์มารวมกันและหาเศษด้วย 1997 ตามกฎ Modulo
            dp[i][j] = (new_pile + existing_piles) % 1997;
        }
    }

    // พิมพ์ผลลัพธ์การแบ่งหิน n ก้อน เป็น k กอง
    cout << dp[n][k] << "\n";

    return 0;
}
