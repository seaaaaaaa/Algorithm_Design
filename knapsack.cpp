#include <iostream>
#include <vector>

using namespace std;

int main() {
    // ปรับแต่งความเร็ว I/O ของ C++
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    // รับค่าจำนวนสิ่งของ (n) และความจุกระเป๋า (m)
    if (!(cin >> n >> m)) return 0;

    vector<int> v(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> v[i];
    }

    vector<int> w(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> w[i];
    }

    // รับค่าตาราง V(a, b) ที่โจทย์คำนวณมาให้แล้ว
    // ขนาดของตารางคือ (n + 1) x (m + 1)
    vector<vector<int>> V(n + 1, vector<int>(m + 1));
    for (int i = 0; i <= n; ++i) {
        for (int j = 0; j <= m; ++j) {
            cin >> V[i][j];
        }
    }

    vector<int> selected_items;
    
    // เริ่มย้อนรอยจากตำแหน่ง n และความจุ m
    int a = n;
    int b = m;

    while (a > 0 && b > 0) {
        // ถ้ายอดรวมมูลค่าต่างจากตอนที่ไม่มีของชิ้นที่ a แสดงว่าของชิ้นนี้ถูกเลือก
        if (V[a][b] != V[a - 1][b]) {
            selected_items.push_back(a);
            b -= w[a]; // หักลบน้ำหนักของชิ้นนี้ออกจากความจุที่เหลือ
        }
        a--; // ถอยไปพิจารณาของชิ้นก่อนหน้า
    }

    // พิมพ์จำนวนสิ่งของที่ถูกเลือก
    cout << selected_items.size() << "\n";
    
    // พิมพ์หมายเลขของสิ่งของที่ถูกเลือก
    for (int i = 0; i < selected_items.size(); ++i) {
        cout << selected_items[i] << (i == selected_items.size() - 1 ? "" : " ");
    }
    cout << "\n";

    return 0;
}
