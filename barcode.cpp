#include <iostream>
#include <vector>

using namespace std;

int main() {
    // ปรับแต่งความเร็ว I/O ของ C++
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m, k;
    if (!(cin >> n >> m >> k)) return 0;

    // การเปลี่ยนสี K ครั้ง เท่ากับมีกลุ่มสีทั้งหมด K + 1 กลุ่ม
    int target_blocks = k + 1;

    // สร้างตาราง DP 2 มิติ ขนาด (n + 1) x (target_blocks + 1)
    // ใช้ long long เผื่อกรณีผลลัพธ์มีค่ามาก
    vector<vector<long long>> dp(n + 1, vector<long long>(target_blocks + 1, 0));

    // Base case
    dp[0][0] = 1;

    // คำนวณ DP แบบ Bottom-Up
    for (int j = 1; j <= target_blocks; ++j) {           // ลูปจำนวนกลุ่มที่ใช้
        for (int i = 1; i <= n; ++i) {                   // ลูปความยาวบาร์โค้ดรวม
            for (int x = 1; x <= m; ++x) {               // ลูปขนาดของกลุ่มสีล่าสุดที่จะนำมาต่อ (ไม่เกิน M)
                if (i >= x) {
                    dp[i][j] += dp[i - x][j - 1];
                }
            }
        }
    }

    // คำตอบคือ จำนวนวิธีสร้างความยาว n โดยใช้ k + 1 กลุ่ม
    cout << dp[n][target_blocks] << "\n";

    return 0;
}
