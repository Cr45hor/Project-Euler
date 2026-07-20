// print(sum(1 for n in range(1, 2**30) if n ^ (n * 2) ^ (n * 3) == 0))

#include <iostream>
#include <cstdint>

using namespace std;

int main() {
    int64_t count = 0;
    for (int64_t n = 1; n <= (1LL << 30); ++n) {
        if ((n ^ (n * 2) ^ (n * 3)) == 0) {
            ++count;
        }
    }
    cout << count << '\n';
    return 0;
}