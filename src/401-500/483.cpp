#include <iostream>
#include <algorithm>
#include <numeric>
#include <cstdint>
#include <cassert>
#include <array>
#include <vector>
#include <iomanip>

using namespace std;

using Real = double;

//---------------------------------------------------------------------------------------------------------

const int N = 350;
const int SMALL_PRIME_COUNT = 7;
const int small_prime[SMALL_PRIME_COUNT] = {2, 3, 5, 7, 11, 13, 17}; // primes not exceeding sqrt(N)
const int max_powers[SMALL_PRIME_COUNT] = {8, 5, 3, 3, 2, 2, 2}; // max power of small_prime[i] <= N
const int MASK_MAX = 23328; 

//---------------------------------------------------------------------------------------------------------

int power(int base, int exp) {
    int res = 1;
    for (int i = 0; i < exp; i++) {
        res *= base;
    }
    return res;
}

//---------------------------------------------------------------------------------------------------------

int base[SMALL_PRIME_COUNT + 1];
int16_t saved_merge_mask[MASK_MAX][MASK_MAX]{};

int prefix(int n) {
    int res = 1;
    for (int p: small_prime) {
        while(n % p == 0) {
            res *= p;
            n /= p;
        }
    }
    return res;
}

array<int, SMALL_PRIME_COUNT> reduced(int n, bool is_prefix = false) {
    if (!is_prefix) {
        n = prefix(n);
    }
    array<int, SMALL_PRIME_COUNT> expo{};
    for (int i = 0; i < SMALL_PRIME_COUNT; i++) {
        while(n % small_prime[i] == 0) {
            expo[i]++;
            n /= small_prime[i];
        }
    }
    return expo;
}

int get_mask(const array<int, SMALL_PRIME_COUNT>& expo) {
    int mask = 0;
    for (int i = 0; i < SMALL_PRIME_COUNT; i++) {
        mask += base[i] * expo[i];
    }
    assert(mask < MASK_MAX);
    return mask;
}

array<int, SMALL_PRIME_COUNT> reverse_mask(int mask) {
    assert(mask < MASK_MAX);
    array<int, SMALL_PRIME_COUNT> expo{};
    for (int i = SMALL_PRIME_COUNT - 1; i >= 0; i--) {
        expo[i] = mask / base[i];
        mask -= expo[i] * base[i];
    }
    assert(mask == 0);
    return expo;
}

int merge_mask(int mask_a, int mask_b) {
    int res = 0;
    for (int i = 0; i < SMALL_PRIME_COUNT; i++) {
        int radix = max_powers[i] + 1;
        int a = mask_a % radix;
        int b = mask_b % radix;
        res += (a > b ? a : b) * base[i];
        mask_a /= radix;
        mask_b /= radix;
    }
    return res;
}

int64_t lcm_mask(int mask) {
    assert(mask < MASK_MAX);
    array<int, SMALL_PRIME_COUNT> expo = reverse_mask(mask);
    int64_t lcm = 1;
    for (int i = 0; i < SMALL_PRIME_COUNT; i++) {
        if (expo[i] > 0) {
            lcm *= power(small_prime[i], expo[i]);
        }
    }
    return lcm;
}

void precompute_base() {
    base[0] = 1;
    for (int i = 0; i < SMALL_PRIME_COUNT; i++) {
        base[i + 1] = base[i] * (max_powers[i] + 1);
    }
    assert(base[SMALL_PRIME_COUNT] == MASK_MAX);

    for (int mask1 = 0; mask1 < MASK_MAX; mask1++) {
        for (int mask2 = 0; mask2 < MASK_MAX; mask2++) {
            saved_merge_mask[mask1][mask2] = merge_mask(mask1, mask2);
        }
    }
}

//---------------------------------------------------------------------------------------------------------

Real P[N + 1][N + 1];

void precompute_probs() {
    // P[a][k] = 1/(k! * a^k)
    // P[a][k] = P[a][k - 1] / (a * k)
    for (int a = 1; a <= N; a++) {
        P[a][0] = 1;
        for (int k = 1; a * k <= N; k++) {
            P[a][k] = P[a][k - 1] / (a * k);
        }
    }
}

//---------------------------------------------------------------------------------------------------------

void precompute() {
    precompute_base();
    precompute_probs();
}

//---------------------------------------------------------------------------------------------------------

bool visited[N + 1];

Real dp[N + 1][MASK_MAX];
Real dp_temp[N + 1][MASK_MAX];
vector<int> active_masks[N + 1];
bool seen_mask[N + 1][MASK_MAX] = {false};

struct Transition {
    int sum;
    int mask;
    Real prob;
    Transition(int __sum = 0, int __mask = 0, Real __prob = 1.0) : sum(__sum), mask(__mask), prob(__prob) {}
};

vector<Transition> transition_rules;

// Given a set of cycle lengths {l1, l2, ..., lk}
// Generate all possible non-negative linear combinations of these cycle lengths
// And for each combination, compute the corresponding sum, mask, and probability
void generate_transition_rules(const vector<int>& cycle_lengths, int idx = 0, int curr_sum = 0, int curr_mask = 0, Real curr_prob = 1.0) {
    if (idx == (int)cycle_lengths.size()) {
        transition_rules.emplace_back(curr_sum, curr_mask, curr_prob);
        return;
    }

    int len = cycle_lengths[idx];
    int len_mask = get_mask(reduced(len));
    
    generate_transition_rules(cycle_lengths, idx + 1, curr_sum, curr_mask, curr_prob);

    // Include this cycle length k times
    int next_mask = saved_merge_mask[curr_mask][len_mask];
    for (int k = 1; curr_sum + k * len <= N; ++k) {
        Real next_prob = curr_prob * P[len][k];
        generate_transition_rules(cycle_lengths, idx + 1, curr_sum + k * len, next_mask, next_prob);
    }
}

void update(vector<int> cycle_lengths, int multiplier = 1) {
    transition_rules.clear(); 
    generate_transition_rules(cycle_lengths, 0, 0, 0, 1.0);

    for(int i = 0; i <= N; i++) {
        fill(dp_temp[i], dp_temp[i] + MASK_MAX, 0.0);
    }
    
    vector<int> temp_active_masks[N + 1];

    for (auto &[sum, trans_mask, prob]: transition_rules) {
        Real actual_prob = prob;
        if (sum > 0) {
            actual_prob *= multiplier * multiplier; 
        }
        
        // Knapsack DP
        for (int w = 0; w <= N - sum; w++) {
            for (int dp_mask : active_masks[w]) { 
                int new_mask = saved_merge_mask[dp_mask][trans_mask];
                dp_temp[w + sum][new_mask] += dp[w][dp_mask] * actual_prob;
                
                if (!seen_mask[w + sum][new_mask]) {
                    seen_mask[w + sum][new_mask] = true;
                    temp_active_masks[w + sum].push_back(new_mask);
                }
            }
        }
    }

    for (int w = 0; w <= N; w++) {
        active_masks[w] = move(temp_active_masks[w]);
        
        for (int m : active_masks[w]) {
            seen_mask[w][m] = false;
        }
        for (int m = 0; m < MASK_MAX; m++) {
            dp[w][m] = dp_temp[w][m];
        }
    }
}

//---------------------------------------------------------------------------------------------------------

Real pipeline() {
    auto is_smooth = [](int n) {
        return prefix(n) == n;
    };

    dp[0][0] = 1.0;
    active_masks[0].push_back(0);
    for (int n = 1; n <= N; n++) {
        if (visited[n]) {
            continue;
        }
        if (is_smooth(n)) {
            update({n}, 1);
        }
        else {
            int big_prime = n;
            assert(big_prime * big_prime > N);
            vector<int> big_cycle_lengths;
            for (int k = 1; k * big_prime <= N; k++) {
                visited[k * big_prime] = true;
                big_cycle_lengths.push_back(k * big_prime);
            }
            update(big_cycle_lengths, big_prime); 
        }
    }

    Real expected = 0.0;
    for (int mask = 0; mask < MASK_MAX; mask++) {
        int64_t small_lcm = lcm_mask(mask);
        expected += dp[N][mask] * small_lcm * small_lcm;
    }
    return expected;
}

//---------------------------------------------------------------------------------------------------------

int main() {
    precompute();
    cout << scientific << setprecision(9) << pipeline() << fixed << '\n';
    return 0;
}