#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

#include <iostream>
#include <vector>
#include <array>
#include <chrono>

const long long MOD = 1e9 + 7;
using CountType = long long;

// Known exact size from previous run bounds
const int EXACT_STATES = 29739270;

struct FastHash {
    inline std::size_t operator()(uint64_t x) const {
        x ^= x >> 30; x *= 0xbf58476d1ce4e5b9ULL;
        x ^= x >> 27; x *= 0x94d049bb133111ebULL;
        x ^= x >> 31; return x;
    }
};

// Custom Open-Addressing Hash Map (Zero node allocations)
class FlatHashMap {
    // 2^26 = 67,108,864 (Power of 2 for fast bitwise modulo)
    static constexpr size_t CAPACITY = 1 << 26; 
    static constexpr size_t MASK = CAPACITY - 1;
    
    std::vector<uint64_t> keys;
    std::vector<int> vals;

public:
    FlatHashMap() {
        keys.assign(CAPACITY, UINT64_MAX); // UINT64_MAX represents an empty slot
        vals.resize(CAPACITY);
    }

    // Returns {id, true if newly inserted}
    inline std::pair<int, bool> get_or_insert(uint64_t key, int next_id) {
        size_t idx = FastHash{}(key) & MASK;
        
        // Linear probing
        while (keys[idx] != UINT64_MAX && keys[idx] != key) {
            idx = (idx + 1) & MASK;
        }
        
        if (keys[idx] == key) {
            return {vals[idx], false}; // Found existing
        }
        
        // Insert new
        keys[idx] = key;
        vals[idx] = next_id;
        return {next_id, true};
    }
};

inline uint64_t next_state(uint64_t state, int x) {
    uint64_t P = state >> 10;
    int D = state & 0x3FF;
    
    int mask = D & ((1 << (x + 1)) - 1);
    if (mask > 0) {
        int y = 31 - __builtin_clz(mask);
        D ^= (1 << y);
    }
    D |= (1 << x);
    
    bool updated = false;
    for (int i = 0; i < 10; ++i) {
        int val = (P >> (i * 4)) & 0xF;
        if (val > x) {
            P &= ~(0xFULL << (i * 4));
            P |= (static_cast<uint64_t>(x) << (i * 4));
            updated = true;
            break;
        }
    }
    
    if (!updated) return UINT64_MAX; 
    return (P << 10) | D;
}

inline bool is_balanced(uint64_t state) {
    uint64_t P = state >> 10;
    int D = state & 0x3FF;
    
    int lenP = 0;
    for (int i = 0; i < 10; ++i) {
        if (((P >> (i * 4)) & 0xF) != 15) lenP++;
        else break;
    }
    
    return lenP == __builtin_popcount(D);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    auto start_time = std::chrono::high_resolution_clock::now();

    // --- Phase 1: Build the DFA ---
    FlatHashMap state_map;
    
    // Pre-allocate memory to completely prevent reallocation overhead
    std::vector<uint64_t> id_to_state;
    id_to_state.reserve(EXACT_STATES);
    
    std::vector<std::array<int, 10>> trans;
    trans.reserve(EXACT_STATES);
    
    std::vector<bool> is_bal;
    is_bal.reserve(EXACT_STATES);

    std::vector<int> initial_states;

    for (int d = 1; d <= 9; ++d) {
        uint64_t P = 0xFFFFFFFFFFULL;
        P &= ~0xFULL; P |= d;         
        int D = (1 << d);             
        uint64_t s = (P << 10) | D;
        
        int id = id_to_state.size();
        state_map.get_or_insert(s, id);
        
        id_to_state.push_back(s);
        trans.push_back({});
        is_bal.push_back(is_balanced(s));
        initial_states.push_back(id);
    }

    int head = 0;
    while (head < id_to_state.size()) {
        uint64_t u = id_to_state[head];
        for (int d = 0; d <= 9; ++d) {
            uint64_t v = next_state(u, d);
            if (v != UINT64_MAX) {
                int next_id = id_to_state.size();
                auto [id, inserted] = state_map.get_or_insert(v, next_id);
                
                if (inserted) {
                    id_to_state.push_back(v);
                    trans.push_back({});
                    is_bal.push_back(is_balanced(v));
                }
                trans[head][d] = id;
            } else {
                trans[head][d] = -1; 
            }
        }
        head++;
    }
    
    int S = id_to_state.size();
    std::cout << "DFA Built. Total states: " << S << "\n";

    // --- Phase 2: DP over the flat DFA ---
    std::vector<CountType> dp(S, 0);
    CountType total_balanced = 0;

    for (int id : initial_states) {
        dp[id] += 1;
    }

    for (int length = 1; length <= 100; ++length) {
        CountType len_balanced = 0;
        for (int i = 0; i < S; ++i) {
            if (dp[i] > 0 && is_bal[i]) {
                len_balanced = (len_balanced + dp[i]) % MOD;
            }
        }
        total_balanced = (total_balanced + len_balanced) % MOD;

        if (length == 100) break; 

        std::vector<CountType> next_dp(S, 0);
        for (int u = 0; u < S; ++u) {
            if (dp[u] == 0) continue;
            for (int d = 0; d <= 9; ++d) {
                int v = trans[u][d];
                if (v != -1) {
                    next_dp[v] = (next_dp[v] + dp[u]) % MOD;
                }
            }
        }
        dp = std::move(next_dp);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "Total balanced numbers (mod " << MOD << "): " << total_balanced << "\n";
    std::cout << "Execution time: " << elapsed.count() << " seconds\n";
    
    return 0;
}