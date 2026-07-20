import math

def T(N: int) -> int:
    """
    Parameterization of the formula p^2 - pq + q^2 = r^2
    Let:
    p = n^2 - m^2
    q = 2mn - m^2
    r = n^2 - nm + m^2
    for 0 < m < n and gcd(m, n) = 1
    The condition for primitive tuples (p, q, r) (means gcd(p, q, r) = 1) is m + n != 0 mod 3. To prove this condition, suppose there is a common prime factor of p and q. Then:
        n^2 = m^2 mod d
        2mn = m^2 mod d
        Noticed that n^2 - m^2 = (n - m)(n + m) mod d. Since d is a prime number, it means either n - m = 0 mod d or m + n = 0 mod d
        Suppose n - m = 0 mod d, then q = 2mn - m^2 = m^2 = 0 mod d, implying that d | m, which leads to d | n => gcd(m, n) = d, contradicting the assumption that gcd(m, n) = 1. Therefore, m + n = 0 mod d. However:
        (m + n)^2 = m^2 + 2mn + n^2 = 3n^2 = 0 mod d, which implies that d | 3 (because gcd(m, n) = 1). Therefore d = 3. This means that if m + n = 0 mod 3, then p and q are not coprime. Therefore, the condition for primitive tuples is gcd(m, n) = 1 and m + n != 0 mod 3.
    The above proofs also show that if gcd(p, q, r) > 1, then gcd(p, q, r) = 3. Which means (p, q, r) need to be scaled by a factor of 3 to get a primitive tuple. Therefore, the tuples can be divided into two families:
    Family 1: (p, q, r) = (n^2 - m^2, 2mn - m^2, n^2 - nm + m^2) for 0 < m < n, gcd(m, n) = 1, m + n != 0 mod 3
    Family 2: (p, q, r) = (n^2 - m^2, 2mn - m^2, n^2 - nm + m^2)/3 for 0 < m < n, gcd(m, n) = 1, m + n = 0 mod 3

    The main condition for p, q, r is that pq/(p + q + r) * sqrt(3)/2 <= N. Given that pq/(p + q + r) = m(n - m), the problem reduces to m(n - m) < 2N/sqrt(3).
    To count number of valid (p, q, r), more conditions are needed:
    - p < q => n^2 < 2mn => n < 2m. Therefore m < n < 2m
    - Since the parameterization only generate primitive tuples, we need another scaling factor k such that k * m(n - m) < 2N/sqrt(3). 
    Let g = n - m. For fixed m and L = floor(2N/sqrt(3)), the conditions are:
    - 0 < m < n < 2m => 0 < g < m
    - gcd(m, g) = gcd(m, n - m) = gcd(m, n) = 1
    - kmg <= L => k <= L/(mg) and g^2 < mg <= L means g < sqrt(L).
    But, remember that gcd(p, q, r) can be 1 or 3. When gcd(p, q, r) = 3, we divide the tuple by 3, resulting in k * m(n - m)/3 < 2N/sqrt(3) => k * mg <= 6N/sqrt(3). The constraints are the same as above, except that L = floor(6N/sqrt(3)). Therefore, it is better to let L1 = floor(2N/sqrt(3)) and L2 = floor(6N/sqrt(3)), and let L = max(L1, L2) = L2 the maximum value of mg. The final constraints are:
    - 0 < g < m < L/g
    - gcd(m, g) = 1
    - k * mg <= L1 if m + n != 0 mod 3, else k * mg <= L2
    - g < sqrt(L) = sqrt(L2)
    """

    L1 = math.floor(2 * N / math.sqrt(3))
    L2 = math.floor(6 * N / math.sqrt(3))
    MAXL = L2
    MAXG = math.floor(math.sqrt(L2))
    
    cnt = 0
    for g in range(1, MAXG + 1):
        for m in range(g + 1, math.floor(MAXL / g) + 1):
            if math.gcd(m, g) == 1:
                if (m - g) % 3 != 0:
                    cnt += math.floor(L1 / (m * g))
                else:
                    cnt += math.floor(L2 / (m * g))
    return cnt

if __name__ == "__main__":
    N = 1053779
    print(T(N))