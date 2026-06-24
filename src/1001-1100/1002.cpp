#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <queue>
#include <unordered_map>

using namespace std;

const int INF = 1e9;

// Segment Tree 1: Finds intervals with L inside range, R > threshold
struct MaxSegTree {
    int n;
    vector<int> tree;
    vector<int> pos_to_id;

    MaxSegTree(int n) : n(n) {
        tree.assign(4 * n, -1);
        pos_to_id.assign(n, -1);
    }

    void update(int id, int l, int r, int pos, int val, int interval_id) {
        if (l == r) {
            tree[id] = val;
            pos_to_id[l] = interval_id;
            return;
        }
        int mid = (l + r) / 2;
        if (pos <= mid) update(2 * id, l, mid, pos, val, interval_id);
        else update(2 * id + 1, mid + 1, r, pos, val, interval_id);
        tree[id] = max(tree[2 * id], tree[2 * id + 1]);
    }

    void extract(int id, int l, int r, int ql, int qr, int threshold, vector<int>& res) {
        if (l > qr || r < ql || tree[id] <= threshold) return;
        if (l == r) {
            res.push_back(pos_to_id[l]);
            tree[id] = -1; // Remove it
            return;
        }
        int mid = (l + r) / 2;
        extract(2 * id, l, mid, ql, qr, threshold, res);
        extract(2 * id + 1, mid + 1, r, ql, qr, threshold, res);
        tree[id] = max(tree[2 * id], tree[2 * id + 1]);
    }
};

// Segment Tree 2: Finds intervals with R inside range, L < threshold
struct MinSegTree {
    int n;
    vector<int> tree;
    vector<int> pos_to_id;

    MinSegTree(int n) : n(n) {
        tree.assign(4 * n, INF);
        pos_to_id.assign(n, -1);
    }

    void update(int id, int l, int r, int pos, int val, int interval_id) {
        if (l == r) {
            tree[id] = val;
            pos_to_id[l] = interval_id;
            return;
        }
        int mid = (l + r) / 2;
        if (pos <= mid) update(2 * id, l, mid, pos, val, interval_id);
        else update(2 * id + 1, mid + 1, r, pos, val, interval_id);
        tree[id] = min(tree[2 * id], tree[2 * id + 1]);
    }

    void extract(int id, int l, int r, int ql, int qr, int threshold, vector<int>& res) {
        if (l > qr || r < ql || tree[id] >= threshold) return;
        if (l == r) {
            res.push_back(pos_to_id[l]);
            tree[id] = INF; // Remove it
            return;
        }
        int mid = (l + r) / 2;
        extract(2 * id, l, mid, ql, qr, threshold, res);
        extract(2 * id + 1, mid + 1, r, ql, qr, threshold, res);
        tree[id] = min(tree[2 * id], tree[2 * id + 1]);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ifstream fin("1002_input.txt");
    string line;
    if (!getline(fin, line)) return 0;

    vector<int> arr;
    stringstream ss(line);
    string item;
    while (getline(ss, item, ',')) {
        if (!item.empty()) arr.push_back(stoi(item));
    }

    int total_elements = arr.size();
    int N = total_elements / 2;

    unordered_map<int, int> val_to_id;
    int id_counter = 0;
    
    vector<int> L(N, -1), R(N, -1);

    for (int i = 0; i < total_elements; ++i) {
        int v = arr[i];
        if (val_to_id.find(v) == val_to_id.end()) {
            val_to_id[v] = id_counter;
            L[id_counter] = i;
            id_counter++;
        } else {
            R[val_to_id[v]] = i;
        }
    }

    MaxSegTree st1(total_elements);
    MinSegTree st2(total_elements);

    for (int i = 0; i < N; ++i) {
        st1.update(1, 0, total_elements - 1, L[i], R[i], i);
        st2.update(1, 0, total_elements - 1, R[i], L[i], i);
    }

    vector<int> color(N, -1);
    int max_above = 0;

    // BFS to find bipartite connected components
    for (int i = 0; i < N; ++i) {
        if (color[i] != -1) continue;

        queue<int> q;
        q.push(i);
        color[i] = 0;
        
        // Remove the starting node from both trees to avoid processing it again
        st1.update(1, 0, total_elements - 1, L[i], -1, -1);
        st2.update(1, 0, total_elements - 1, R[i], INF, -1);

        int count0 = 0, count1 = 0;

        while (!q.empty()) {
            int curr = q.front();
            q.pop();

            if (color[curr] == 0) count0++;
            else count1++;

            vector<int> neighbors;
            
            // Query 1: Find all j where L_j is inside curr, but R_j > R_curr
            st1.extract(1, 0, total_elements - 1, L[curr] + 1, R[curr] - 1, R[curr], neighbors);
            
            // Query 2: Find all j where R_j is inside curr, but L_j < L_curr
            st2.extract(1, 0, total_elements - 1, L[curr] + 1, R[curr] - 1, L[curr], neighbors);

            for (int nxt : neighbors) {
                if (color[nxt] == -1) {
                    color[nxt] = color[curr] ^ 1;
                    q.push(nxt);
                    
                    // Immediately delete found neighbors from the other tree to keep them synced
                    st1.update(1, 0, total_elements - 1, L[nxt], -1, -1);
                    st2.update(1, 0, total_elements - 1, R[nxt], INF, -1);
                }
            }
        }
        max_above += max(count0, count1);
    }

    cout << max_above << '\n';
    return 0;
}