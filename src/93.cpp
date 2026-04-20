#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <cmath>

using namespace std;

double add(double a, double b) {
    return a + b;
}

double sub(double a, double b) {
    return a - b;
}

double mul(double a, double b) {
    return a * b;
}

double divide(double a, double b) {
    if (abs(b) < 1e-9) {
        b = 0;
    }
    return a / b;
}

double (*ops[])(double, double) = {add, sub, mul, divide};

set<double> generate(const vector<double>& nums) {
    if (nums.size() == 1) {
        return {nums[0]};
    }
    
    set<double> res;
    for (int i = 0; i < nums.size(); i++) {
        for (int j = 0; j < nums.size(); j++) {
            if (i == j) continue;

            vector<double> nums_minus;
            for (int k = 0; k < nums.size(); k++) {
                if (k != i && k != j) {
                    nums_minus.push_back(nums[k]);
                }
            }

            for (auto op: ops) {
                double val = op(nums[i], nums[j]);
                if (!isfinite(val)) {
                    continue;
                }
                nums_minus.push_back(val);
                set<double> sub_res = generate(nums_minus);
                res.insert(sub_res.begin(), sub_res.end());
                nums_minus.pop_back();
            }
        }
    }
    return res;
}

int count_consecutive(const set<double>& res) {
    int cnt = 0;
    set<int> filtered;
    for (double x: res) {
        if (x < 1) {
            continue;
        }
        if (abs(x - round(x)) < 1e-9) {
            filtered.insert((int)round(x));
        }
    }
    for (int i = 1; ; i++) {
        if (filtered.count(i) == 0) {
            break;
        }
        cnt++;
    }
    return cnt;
}

int main() {
    vector<double> nums;
    pair<int, vector<double>> ans;
    for (int a = 0; a <= 9; a++) {
        nums.push_back(a);
        for (int b = a + 1; b <= 9; b++) {
            nums.push_back(b);
            for (int c = b + 1; c <= 9; c++) {
                nums.push_back(c);
                for (int d = c + 1; d <= 9; d++) {
                    nums.push_back(d);
                    set<double> res = generate(nums);
                    int cnt = count_consecutive(res);
                    ans = max(ans, make_pair(cnt, nums));
                    nums.pop_back();
                }
                nums.pop_back();
            }
            nums.pop_back();
        }
        nums.pop_back();
    }
    for (double x: ans.second) {
        cout << x;
    }
    cout << endl;

    return 0;
}