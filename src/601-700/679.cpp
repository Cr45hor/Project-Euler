#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cstdint>
#include <cassert>

using namespace std;

const int N = 30;
const int ALPHABET_SIZE = 26;
const vector<char> ALPHABET = {'A', 'E', 'F', 'R'};
const vector<string> S = {"FREE", "FARE", "AREA", "REEF"};

struct Node {
    int child[ALPHABET_SIZE];
    int mask_keyword;

    int link;
    int to[ALPHABET_SIZE];

    Node(): mask_keyword(0), link(0) {
        fill(begin(child), end(child), -1);
        fill(begin(to), end(to), -1);
    }
};

vector<Node> trie(1);

void add_keyword(string str, int keyword_id) {
    int root = 0;
    for (char ch: str) {
        assert(ch >= 'A' && ch <= 'Z');
        int c = (int)ch - (int)'A';
        if (trie[root].child[c] == -1) {
            trie[root].child[c] = (int)trie.size();
            trie.emplace_back();
        }
        root = trie[root].child[c];
    }
    trie[root].mask_keyword |= (1 << keyword_id);
}

void build() {
    queue<int> Q;
    Q.push(0);

    while(!Q.empty()) {
        int u = Q.front();
        Q.pop();
        int w = trie[u].link;

        for (int c = 0; c < ALPHABET_SIZE; c++) {
            if (trie[u].child[c] == -1) {
                trie[u].to[c] = (u == 0 ? 0 : trie[w].to[c]);
            }
            else {
                int v = trie[u].child[c];
                trie[u].to[c] = v;
                trie[v].link = (u == 0 ? 0 : trie[w].to[c]);
                Q.push(v);
            }
        }
    }
}

int T(int u, int c) {
    return trie[u].to[c];
}

int64_t dp[N + 1][1 << 4][16];
bool vis[N + 1][1 << 4][16];

int64_t solve(int pos, int mask, int u) {
    if (pos >= N) {
        return mask == 15;
    }

    int64_t& res = dp[pos][mask][u];
    if (vis[pos][mask][u]) {
        return res;
    }

    res = 0;
    for (char ch: ALPHABET) {
        int c = (int)ch - (int)'A';
        int v = T(u, c);
        if (!(mask & trie[v].mask_keyword)) {
            res += solve(pos + 1, mask | trie[v].mask_keyword, v);
        }
    }
    vis[pos][mask][u] = true;
    return res;
}

int main() {
    for (int i = 0; i < (int)S.size(); i++) {
        add_keyword(S[i], i);
    }
    build();

    cout << solve(0, 0, 0) << '\n';
}