from collections import Counter

MOD = 10**9 + 7

def I(N: int) -> int:
    if N <= 2:
        return 0
    
    N_msb = N.bit_length() - 1
    M = 1 << N_msb
    
    # Base case for the power of 2 plateaus
    if N == M:
        # return (4**(N_msb - 2)) * (M - 1)
        return (M - 1) * pow(4, N_msb - 2, MOD) % MOD
    
    if N == M * 2 - 1:
        return I(N + 1)
        
    k = N - M
    
    # 1. The MSB cross-product (Your exact offset)
    left_half = (k + 1) // 2
    right_half = (k + 1) - left_half
    # offset = M * left_half * right_half
    offset = M * left_half * right_half % MOD
    
    # 2. Lower-bits interaction bonus
    residual_bonus = (M * k * (k + 1)) // 8
    
    # return I(M) + offset + residual_bonus + I(k)
    return (I(M) + offset + residual_bonus + I(k)) % MOD

def ops(x: int, y: int) -> int:
    return (x**2) ^ (y**2)

def X_brute(N: int) -> int:
    # 1. Build a graph G = (V, E)
    # Where V = {(i, j) | 1 <= i, j <= N}
    # and E = {(u, v) | u = (a, b) in V, v = (b, c) in V, and ops(a, b) < ops(b, c)}

    V = [(ops(i, j), (i, j)) for i in range(1, N + 1) for j in range(1, N + 1) if i != j]
    G = [[] for _ in range(len(V))]
    for u, (w1, (a, b)) in enumerate(V):
        for v, (w2, (c, d)) in enumerate(V):
            if b == c and w1 < w2:
                G[u].append(v)

    # 2. Topologically sort the graph G
    topo = []
    vis = [0] * len(V)

    def dfs(u: int) -> None:
        vis[u] = 1
        for v in G[u]:
            if vis[v] == 0:
                dfs(v)
        topo.append(u)
    
    for u in range(len(V)):
        if vis[u] == 0:
            dfs(u)

    # 3. Dynamic programming to find the longest path in the DAG
    # Weight of a path is sum of weight of nodes in the path, where weight of node u is w(u) = ops(i, j) for node u = (i, j)
    def w(u: int) -> int:
        return V[u][0]

    dp = [-1] * len(V)
    for u in topo:
        dp[u] = max((dp[v] for v in G[u]), default=0) + V[u][0]
        
        weight, coor = V[u]
        print(f"Node: {coor}, Weight: {weight}, DP: {dp[u]}")
    
    return max(dp) % MOD

from collections import defaultdict

def X(N: int) -> int:
    # 1. Group all valid (i, j) pairs by their weight `ops(i, j)`
    weights = defaultdict(list)
    for i in range(1, N + 1):
        for j in range(1, N + 1):
            if i != j:
                w = ops(i, j)
                weights[w].append((i, j))
    
    # 2. Topologically sort by sorting the unique weights
    sorted_weights = sorted(weights.keys())
    
    # max_dp[x] tracks the maximum path weight ending at integer `x`
    # strictly using pairs with weights smaller than the current one.
    max_dp = [0] * (N + 1)
    
    global_max = 0
    
    # 3. Dynamic Programming 
    for w in sorted_weights:
        group = weights[w]
        current_dp = []
        
        # Step 3a: Calculate DP values for the current weight group
        for i, j in group:
            # The max path ending at (i, j) is the weight of (i, j) itself,
            # plus the best path ending at `i` from previous, smaller weights.
            path_weight = w + max_dp[i]
            current_dp.append((j, path_weight))
            
            if path_weight > global_max:
                global_max = path_weight
        
        # Step 3b: Update max_dp state AFTER processing the whole group
        # This guarantees we only form paths where w1 < w2
        for j, path_weight in current_dp:
            if path_weight > max_dp[j]:
                max_dp[j] = path_weight
                
    # Assuming MOD is available in your global scope as in your original snippet
    return global_max % MOD

import functools
import time

def C(N: int) -> int:
    """
    Computes the number of unreachable 3-pile Nim states (a, b, c)
    where 0 <= a, b, c < N in O(log N) time.
    """
    if N <= 0:
        return 0
        
    n_bits = N.bit_length()
    
    @functools.cache
    def dp(i: int, less_a: bool, less_b: bool, less_c: bool, found_k: bool) -> int:
        # Base case: We processed all bits.
        # We only count it if all three numbers are strictly less than N
        # and we successfully established the MSB condition.
        if i == -1:
            return 1 if (less_a and less_b and less_c and found_k) else 0
            
        ans = 0
        bit_n = (N >> i) & 1
        
        # Iterate through all possible bit combinations for a, b, c at position i
        for bit_a in (0, 1):
            if not less_a and bit_a > bit_n: continue
            for bit_b in (0, 1):
                if not less_b and bit_b > bit_n: continue
                for bit_c in (0, 1):
                    if not less_c and bit_c > bit_n: continue
                    
                    # Update strict boundary flags
                    next_la = less_a or (bit_a < bit_n)
                    next_lb = less_b or (bit_b < bit_n)
                    next_lc = less_c or (bit_c < bit_n)
                    
                    if not found_k:
                        # Case 1: We decide this bit is NOT the MSB of the Nim-sum.
                        # The XOR sum at this position must be 0.
                        if (bit_a ^ bit_b ^ bit_c) == 0:
                            ans += dp(i - 1, next_la, next_lb, next_lc, False)
                            
                        # Case 2: We decide this bit IS the MSB of the Nim-sum.
                        # The unreachability rule dictates all three bits must be 1.
                        if bit_a == 1 and bit_b == 1 and bit_c == 1:
                            ans += dp(i - 1, next_la, next_lb, next_lc, True)
                    else:
                        # Case 3: We already found the MSB in a previous higher bit.
                        # Lower bits can be any valid combination.
                        ans += dp(i - 1, next_la, next_lb, next_lc, True)
                        
        return ans
        
    return dp(n_bits - 1, False, False, False, False)

if __name__ == "__main__":
    assert I(10) == 50
    assert X(4) == 71
    assert X(10) == 702
    assert C(10) == 123

    N = 1000
    M = [0] * (N + 1)
    M[0] = I(N)
    M[1] = X(N)
    M[2] = C(N)
    for k in range(3, N + 1):
        M[k] = (M[k - 1] * M[k - 2] * M[k - 3]) % MOD
    assert M[4] == 457587170
    print(M[N])