#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct Command {
    int orientation;
    int x, y;
};

vector<Command> results;

/**
 * orientation logic based on which quadrant is "missing":
 * 0: missing top-left (tile covers top-right, bottom-left, bottom-right)
 * 1: missing top-right (tile covers top-left, bottom-left, bottom-right)
 * 2: missing bottom-left (tile covers top-left, top-right, bottom-right)
 * 3: missing bottom-right (tile covers top-left, top-right, bottom-left)
 */

void solve(int size, int x, int y, int holeX, int holeY) {
    if (size == 1) return;

    int half = size / 2;
    int centerX = x + half;
    int centerY = y + half;

    // Determine which quadrant the hole is in
    // 0: Top-Left, 1: Top-Right, 2: Bottom-Left, 3: Bottom-Right
    int quad;
    if (holeX < centerX && holeY < centerY) quad = 0;
    else if (holeX >= centerX && holeY < centerY) quad = 1;
    else if (holeX < centerX && holeY >= centerY) quad = 2;
    else quad = 3;

    // Place the center tile based on the hole quadrant
    // Orientation is defined by the 2x2 area's missing corner
    results.push_back({quad, x + half - 1, y + half - 1}); 

    // Recursive calls for each quadrant
    // Top-Left
    solve(half, x, y, (quad == 0 ? holeX : centerX - 1), (quad == 0 ? holeY : centerY - 1));
    // Top-Right
    solve(half, centerX, y, (quad == 1 ? holeX : centerX), (quad == 1 ? holeY : centerY - 1));
    // Bottom-Left
    solve(half, x, centerY, (quad == 2 ? holeX : centerX - 1), (quad == 2 ? holeY : centerY));
    // Bottom-Right
    solve(half, centerX, centerY, (quad == 3 ? holeX : centerX), (quad == 3 ? holeY : centerY));
}

int main() {
    int L, X, Y;
    if (!(cin >> L >> X >> Y)) return 0; 

    // The problem uses (X,Y) as (Column, Row) where (0,0) is North-West
    solve(L, 0, 0, X, Y); 

    cout << results.size() << endl; 
    for (const auto& cmd : results) {
        cout << cmd.orientation << " " << cmd.x << " " << cmd.y << endl; 
    }

    return 0;
}
