#include <bits/stdc++.h>

using namespace std;

int main() {
    srand(time(0));
    int t = 1 + rand() % 10;
    int z = 1 + rand() % 100;
    int l = z + rand() % 100;
    cout << t << '\n';
    cout << z << '\n';
    cout << l << '\n';
    int x;
    for (int i = 0; i < z; i++) {
        for (int j = 0; j < z; j++) {
            x = rand() % 10;
            cout << x << " \n"[j == z-1];
        }
    }
    for (int i = 0; i < l; i++) {
        for (int j = 0; j < l; j++) {
            x = (i == j ? 0 : 1 + rand() % 10);
            cout << x << " \n"[j == l-1];
        }
    }
    return 0;
}
