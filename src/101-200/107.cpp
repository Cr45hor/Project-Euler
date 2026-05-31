#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cassert>
#include <fstream>

using namespace std;

int const N = 40;

vector<tuple<int, int, int>> edges;

int par[N];

void init_dsu() {
    for (int u = 0; u < N; u++) {
        par[u] = -1;
    }
}

int root(int u) {
    return (par[u] < 0 ? u : par[u] = root(par[u]));
}

int merge(int u, int v) {
    u = root(u), v = root(v);
    if (u == v) {
        return false;
    }
    if (par[u] > par[v]) {
        swap(u, v);
    }
    par[u] += par[v];
    par[v] = u;
    return true;
}

int main() {
    ifstream fin("0107_network.txt");
    assert(fin.is_open());

    for (int u = 0; u < N; u++) {
        string line;
        getline(fin, line);
        
        for (int i = 0, j = 0, v = 0; i < (int)line.size(); i = j, v++) {
            while(j < (int)line.size() && line[j] != ',') {
                j++;
            }

            string weight = line.substr(i, j - i);
            if (weight != "-") {
                int w = stoi(weight);
                edges.emplace_back(w, u, v);
            }

            if (j + 1 < (int)line.size()) {
                j++;
            }
        }
    }

    sort(edges.begin(), edges.end());
    init_dsu();
    
    int saving = 0;
    for (auto &[w, u, v]: edges) {
        if (u < v && !merge(u, v)) {
            saving += w;
        }
    }
    cout << saving << '\n';
}