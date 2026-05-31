#include <iostream>
#include <vector>
#include <cstdint>

using namespace std;

// We use __int128 for the total sum because 2e9 elements 
// with values up to 5e7 can easily overflow a 64-bit integer.
typedef __int128_t int128;

struct Node {
    int32_t value;
    int64_t count; // Number of subarrays ending here for which 'value' is the minimum
};

int main() {
    const int N = 2000000000;
    const int MOD = 50515093;
    
    // Initial seed
    int64_t current_s = 290797;
    
    vector<Node> stack;
    int128 total_sum = 0;
    int128 current_contribution = 0;

    for (int i = 1; i <= N; i++) {
        // Generate S[i] on the fly
        current_s = (current_s * current_s) % MOD;
        int32_t val = (int32_t)current_s;

        int64_t count = 1;
        // Pop elements greater than or equal to current value
        // This maintains the "Sum of Subarray Minimums" logic
        while (!stack.empty() && stack.back().value >= val) {
            count += stack.back().count;
            current_contribution -= (int128)stack.back().value * stack.back().count;
            stack.pop_back();
        }

        // Add the new value and its total range of influence
        stack.push_back({val, count});
        current_contribution += (int128)val * count;
        
        total_sum += current_contribution;
    }

    // Helper to print __int128
    string result = "";
    if (total_sum == 0) result = "0";
    while (total_sum > 0) {
        result = (char)((total_sum % 10) + '0') + result;
        total_sum /= 10;
    }
    cout << result << endl;

    return 0;
}