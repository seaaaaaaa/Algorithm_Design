#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main() {
    // ปรับแต่งความเร็ว I/O ของ C++
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int R, C;
    // รับค่าจำนวนแถว (R) และคอลัมน์ (C)
    if (!(cin >> R >> C)) return 0;

    // รับข้อมูลตาราง
    vector<string> grid(R);
    for (int i = 0; i < R; ++i) {
        cin >> grid[i];
    }

    // สร้างตาราง DP ขนาด R x C และกำหนดค่าเริ่มต้นเป็น 0
    vector<vector<int>> dp(R, vector<int>(C, 0));
    
    int max_k = 0; // ตัวแปรเก็บขนาดความยาวด้านที่มากที่สุด

    // เริ่มคำนวณ DP จากซ้ายไปขวา บนลงล่าง
    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < C; ++j) {
            
            // ถ้าช่องปัจจุบันเป็น '1' ค่อยนำมาคำนวณ
            if (grid[i][j] == '1') {
                
                // Base case: ถ้าอยู่ขอบซ้ายสุดหรือบนสุด ขนาดมากสุดคือ 1
                if (i == 0 || j == 0) {
                    dp[i][j] = 1;
                } else {
                    // หาค่าต่ำสุดจาก 3 ช่อง (บน, ซ้าย, ขอบบนซ้าย) แล้วบวก 1
                    dp[i][j] = min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]}) + 1;
                }
                
                // อัปเดตขนาดสี่เหลี่ยมจัตุรัสที่ใหญ่ที่สุดที่เคยเจอ
                max_k = max(max_k, dp[i][j]);
            }
        }
    }

    // พิมพ์ผลลัพธ์คือขนาดของสี่เหลี่ยมจัตุรัสที่ใหญ่ที่สุด
    cout << max_k << "\n";

    return 0;
}
