#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

void print_vec(vector<int> &v) {
  for (int i = 0; i < v.size(); i++)
    cout << v[i] << " ";
  cout << "\n";
}

vector<int> to_arr_int(string &s) {
  string in;
  vector<int> out;

  for (int i = 0; i < s.size(); i++) {
    if (s[i] != ' ') {
      in += s[i];
    } else {
      if (!in.empty()) {
        out.push_back(stoi(in));
        in = "";
      }
    }
  }

  if (!in.empty())
    out.push_back(stoi(in));

  return out;
}

bool is_decreasing(vector<int>& v){
  bool b = true;
  int diff;
  for (int i = 1; i < v.size(); i++) {
    diff = abs(v[i] - v[i - 1]);
    b = b && (v[i] < v[i - 1]) && diff >= 1 && diff <= 3;
  }

  return b;
};

bool is_increasing(vector<int>& v) {
  bool b = true;
  int diff;
  for (int i = 1; i < v.size(); i++) {
    diff = abs(v[i] - v[i - 1]);
    b = b && (v[i] > v[i - 1]) && diff >= 1 && diff <= 3;
  }

  return b;
};

bool is_safe(vector<int> &v) { return is_decreasing(v) || is_increasing(v); }

bool is_safe_with_rm(vector<int> &v) {
  if (is_increasing(v) || is_decreasing(v)) return true;

  bool is_increasing_ = [&]() -> bool {
    vector<bool> rem_idx(v.size(), false);
    vector<int> temp;

    for (int i = 0; i < v.size(); i++) {
      rem_idx[i] = true;
      for (int j = 0; j < v.size(); j++){
        if (rem_idx[j]) continue;
        temp.push_back(v[j]);
      }

      if (is_increasing(temp)) return true;
      rem_idx[i] = false;
      temp.clear();
    }

    return false;
  }();

  bool is_decreasing_ = [&]() -> bool {
    vector<bool> rem_idx(v.size(), false);
    vector<int> temp;

    for (int i = 0; i < v.size(); i++) {
      rem_idx[i] = true;
      for (int j = 0; j < v.size(); j++){
        if (rem_idx[j]) continue;
        temp.push_back(v[j]);
      }

      if (is_decreasing(temp)) return true;
      rem_idx[i] = false;
      temp.clear();
    }

    return false;
  }();

  return is_increasing_ || is_decreasing_;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  string s;
  int safe = 0;
  int safe_modified = 0;
  while (getline(cin, s)) {
    vector<int> v = to_arr_int(s);
    safe += is_safe(v);
    safe_modified += is_safe_with_rm(v);
  }

  cout << "safe: " << safe << "\n";
  cout << "safe modified: " << safe_modified << "\n";

  return 0;
}
