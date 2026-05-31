// C++ translation of the original Python script.
// Default N is 100 to match the small-run Python version; an alternate N
// may be supplied on the command line.

#include <bits/stdc++.h>
using namespace std;
using int64 = long long;

static inline int64 mul_mod(int64 a, int64 b, int64 mod) {
	__int128 t = ( __int128 ) a * b;
	t %= mod;
	return (int64)t;
}

static inline int64 pow_mod(int64 base, int64 exp, int64 mod) {
	if (mod == 1) return 0;
	base %= mod;
	int64 res = 1;
	while (exp > 0) {
		if (exp & 1) res = mul_mod(res, base, mod);
		base = mul_mod(base, base, mod);
		exp >>= 1;
	}
	return res;
}

// extended gcd
int64 egcd(int64 a, int64 b, int64 &x, int64 &y) {
	if (a == 0) { x = 0; y = 1; return b; }
	int64 x1, y1;
	int64 g = egcd(b % a, a, x1, y1);
	x = y1 - (b / a) * x1;
	y = x1;
	return g;
}

int64 invmod(int64 a, int64 m) {
	int64 x, y;
	int64 g = egcd(a % m + m, m, x, y);
	if (g != 1) throw runtime_error("Modular inverse does not exist");
	x %= m;
	if (x < 0) x += m;
	return x;
}

int main(int argc, char **argv) {
	int N = 5e8;
	if (argc > 1) N = stoi(argv[1]);

	vector<int> spf(N + 1, 1);
	for (int i = 2; i <= N; ++i) {
		if (spf[i] == 1) {
			spf[i] = i;
			if ((int64)i * i <= N) {
				for (int j = i * i; j <= N; j += i) {
					if (spf[j] == 1) spf[j] = i;
				}
			}
		}
	}

	auto phi = [&](int n) {
		int64 result = n;
		while (n > 1) {
			int p = spf[n];
			result -= result / p;
			while (n % p == 0) n /= p;
		}
		return result;
	};

	auto f = [&](int n) -> int64 {
		if (n == 1) return 1;
		if (n % 2 == 0) {
			return 0;
		} else {
			return phi(n);
		}
	};

	unsigned long long total = 0ULL;
	for (int i = 1; i <= N; ++i) total += (unsigned long long)f(i);
	cout << total << '\n';
	return 0;
}