#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <stack>

using namespace std;

int const N = 1e6;

int sum_divisor[N];

void sieve() {
    for (int x = 1; x < N; x++) {
        for (int y = x * 2; y < N; y += x) {
            sum_divisor[y] += x;
        }
    }
}

int succ[N];
vector<int> G[N];

int timeDFS(0);
int low[N], num[N];
stack<int> st;

vector<vector<int>> scc;

// Find strongly connected components using Tarjan's algorithm
void dfs(int u) {
    num[u] = low[u] = ++timeDFS;
    st.push(u);
    
    for (int v: G[u]) {
        if (!num[v]) {
            dfs(v);
            low[u] = min(low[u], low[v]);
        }
        else {
            low[u] = min(low[u], num[v]);
        }
    }

    if (num[u] == low[u]) {
        vector<int> component;
        int v;
        do {
            v = st.top();
            st.pop();
            component.push_back(v);
            num[v] = low[v] = N;
        } while(v != u);
        scc.push_back(component);
    }
}

int main() {
    sieve();

    for (int x = 1; x < N; x++) {
        succ[x] = (sum_divisor[x] < N) ? sum_divisor[x] : 0;
        G[x].push_back(succ[x]);
    }

    for (int x = 0; x < N; x++) {
        if (!num[x]) {
            dfs(x);
        }
    }

    vector<int> biggest_component;
    for (vector<int> component: scc) {
        if (component.size() > 1) {
            if (component.size() > biggest_component.size()) {
                biggest_component = component;
            }
        }
    }

    cout << *min_element(biggest_component.begin(), biggest_component.end()) << '\n';
    return 0;
}