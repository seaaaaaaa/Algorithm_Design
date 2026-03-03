#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main() {
    // ปรับแต่งความเร็ว I/O ของ C++
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    // รับค่าความยาว n และ m
    if (!(cin >> n >> m)) return 0;

    string X, Y;
    cin >> X >> Y;

    // รับค่าตาราง L(i, j) ที่โจทย์คำนวณมาให้แล้ว
    // ขนาดของตารางคือ (n + 1) x (m + 1)
    vector<vector<int>> L(n + 1, vector<int>(m + 1));
    for (int i = 0; i <= n; ++i) {
        for (int j = 0; j <= m; ++j) {
            cin >> L[i][j];
        }
    }

    string ans = "";
    
    // เริ่มย้อนรอยจากตำแหน่ง n และ m
    int i = n;
    int j = m;

    while (i > 0 && j > 0) {
        // หากอักขระตรงกัน (ใช้ i-1 และ j-1 เพราะ string ใน C++ เริ่มนับ index ที่ 0)
        if (X[i - 1] == Y[j - 1]) {
            ans += X[i - 1]; // เก็บอักขระไว้ในคำตอบ
            i--;
            j--;
        } 
        // หากไม่ตรงกัน ให้เดินไปทางช่องที่มีค่า L มากกว่า (เดินขึ้นหรือเดินซ้าย)
        else if (L[i - 1][j] >= L[i][j - 1]) {
            i--; // เดินขึ้น
        } 
        else {
            j--; // เดินซ้าย
        }
    }

    // เนื่องจากเราย้อนรอยจากปลายมาหัว สายอักขระที่ได้จะสลับที่กันอยู่ จึงต้อง reverse กลับให้ถูกต้อง
    reverse(ans.begin(), ans.end());

    // พิมพ์คำตอบ
    cout << ans << "\n";

    return 0;
}
