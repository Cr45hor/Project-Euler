#include <iostream>
#include <cstdint>
#include <cassert>

using namespace std;

const int N = 1e8;
const int M = 1e5;

int spf[N + 1];

void sieve() {
    spf[2] = 2;
    for (int i = 4; i <= N; i += 2) {
        spf[i] = 2;
    }
    for (int i = 1; i <= N; i += 2) {
        spf[i] = i;
    }
    for (int i = 3; i * i <= N; i += 2) {
        if (spf[i] == i) {
            for (int j = i * i; j <= N; j += i * 2) {
                if (spf[j] == j) {
                    spf[j] = i;
                }
            }
        }
    }
}

int d[N + 1];

void precompute() {
    d[1] = 1;
    for (int n = 2; n <= N; n++) {
        int p = spf[n];
        int k = 0;
        int q = n;
        while (q % p == 0) {
            q /= p;
            k++;
        }
        d[n] = (k + 1) * d[q];
    }
}

struct circular_deque {
    int buf[M];
    int begin = 0, end = -1;
    int size = 0;
    
    // Basic operations
    int get_size() const {
        return size;
    }

    int& operator[](int i) {
        return buf[(begin + i) % M];
    }

    int front() const {
        return buf[begin];
    }

    int back() const {
        return buf[end];
    }

    bool empty() const {
        return get_size() == 0;
    }

    // Add/remove elements
    void push_back(int x) {
        end = (end + 1) % M;
        buf[end] = x;
        size++;
    }

    void push_front(int x) {
        begin = (begin - 1 + M) % M;
        buf[begin] = x;
        size++;
    }

    void pop_back() {
        end = (end - 1 + M) % M;
        if (size == 0) {
            begin = 0;
            end = -1;
        }
        size--;
    }

    void pop_front() {
        begin = (begin + 1) % M;
        if (size == 0) {
            begin = 0;
            end = -1;
        }
        size--;
    }
};

int main() {
    sieve();
    precompute();

    circular_deque dq;

    auto add_element = [&](int id) {
        while(!dq.empty() && d[dq.back()] < d[id]) {
            dq.pop_back();
        }
        dq.push_back(id);
    };

    auto remove_element = [&](int lb) {
        while(!dq.empty() && dq.front() < lb) {
            dq.pop_front();
        }
    };

    int64_t S = 0;
    for (int r = 1; r < M; r++) {
        add_element(r);
    }
    for (int r = M; r <= N; r++) {
        int l = r - M + 1;
        add_element(r);
        remove_element(l);
        S += d[dq.front()];
    }
    cout << S << '\n';

    return 0;
}