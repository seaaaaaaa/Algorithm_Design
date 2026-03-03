#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    // ปรับแต่งความเร็ว I/O ของ C++
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    // รับค่าจำนวนสมาชิก N
    if (!(cin >> n)) return 0;

    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    // กรณีที่ไม่มีข้อมูล
    if (n == 0) {
        cout << 0 << "\n";
        return 0;
    }

    // vector สำหรับเก็บตัวสุดท้ายของ Subsequence ความยาวต่างๆ
    vector<int> lis;

    for (int i = 0; i < n; ++i) {
        // หาตำแหน่งแรกใน lis ที่มีค่า >= a[i]
        auto it = lower_bound(lis.begin(), lis.end(), a[i]);

        // ถ้าหาไม่เจอ แปลว่า a[i] มากกว่าทุกค่าใน lis ให้เอาไปต่อท้าย
        if (it == lis.end()) {
            lis.push_back(a[i]);
        } 
        // ถ้าหาเจอ ให้ทับค่านั้นด้วย a[i] (เพื่อให้ค่าปลายหางมีค่าน้อยที่สุดเท่าที่จะทำได้)
        else {
            *it = a[i];
        }
    }

    // ขนาดของ lis คือความยาวของ Longest Increasing Subsequence
    cout << lis.size() << "\n";

    return 0;
}
