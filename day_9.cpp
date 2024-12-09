#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
using namespace std;

void print_vs(vector<string> &vs) {
  for (auto s : vs) {
    cout << s;
  }
  cout << endl;
}

void print_vvs(vector<vector<string>> &vvs) {
  for (auto s : vvs) {
    for (auto s_ : s) {
      cout << s_;
    }
    /*cout << endl;*/
  }
  cout << endl;
}

void fuse_vector(vector<string> &a, vector<string> &b) {
  for (auto x : b) {
    a.push_back(x);
  }
}

int get_empty_space_n(vector<string> &vs) {
  int empty = 0;
  for (string c : vs) {
    if (c == ".")
      empty++;
  }

  return empty;
}

void fill_empty_space(vector<string> &s_empty_space, vector<string> &to_fill) {
  int start = 0;
  for (int i = 0; i < s_empty_space.size(); i++) {
    start = i;
    if (s_empty_space[i] == ".")
      break;
  }

  for (int i = 0; i < to_fill.size(); i++) {
    s_empty_space[start] = to_fill[i];
    to_fill[i] = ".";
    start++;
  }
}

void solve_1(string &s) {
  stringstream ss;
  vector<string> vs;

  int curr = 0;
  bool data = true;

  for (int i = 0; i < s.size(); i++) {
    if (data) {
      string temp;
      for (int j = 0; j < s[i] - '0'; j++) {
        vs.push_back(to_string(curr));
      }
      curr++;
    } else {
      for (int j = 0; j < s[i] - '0'; j++) {
        vs.push_back(".");
      }
    }

    data = !data;
  }

  int l = 0, r = vs.size() - 1;
  while (l < r) {
    if (vs[l] != ".") {
      l++;
      continue;
    }

    if (vs[r] == ".") {
      r--;
      continue;
    }

    swap(vs[l], vs[r]);
    l++;
    r--;
  }

  unsigned long long mult = 0;
  unsigned long long ans = 0;
  for (string s : vs) {
    if (s == ".")
      continue;
    ans += mult * stoi(s);
    mult++;
  }

  cout << "first solution: " << ans << "\n";
}

void solve_2(string &s) {
  stringstream ss;
  vector<vector<string>> vs;
  vector<int> blank_idxs;
  vector<int> data_idxs;

  int curr = 0;
  bool data = true;

  for (int i = 0; i < s.size(); i++) {
    if (data) {
      vector<string> temp_vs;

      for (int j = 0; j < s[i] - '0'; j++) {
        temp_vs.push_back(to_string(curr));
      }
      vs.push_back(temp_vs);
      curr++;
    } else {
      vector<string> temp_vs;
      for (int j = 0; j < s[i] - '0'; j++) {
        temp_vs.push_back(".");
      }
      vs.push_back(temp_vs);
    }

    data = !data;
  }
  vector<vector<string>> temp;
  for (int i = 0; i < vs.size(); i++) {
    if (vs[i].empty())
      continue;
    if (vs[i][0] != ".") {
      temp.push_back(vs[i]);
      continue;
    }

    if (vs[i][0] == ".") {
      vector<string> t = vs[i];
      int j = i;
      /*cout << "vs[j+1][0]: "; cout << vs[j+1][0] << endl;;*/
      while (j + 1 < vs.size() && !vs[j + 1].empty() && vs[j + 1][0] == ".") {
        fuse_vector(t, vs[j + 1]);
        j++;
      }
      i = j;
      temp.push_back(t);
    }
  }

  vs = temp;
  int i_ = 0;
  for (auto &s : vs) {
    if (s[0] == ".") {
      blank_idxs.push_back(i_++);
    } else {
      data_idxs.push_back(i_++);
    }
  }

  for (int r = data_idxs.size() - 1; r > 0; r--) {
    for (int l = 0; l < blank_idxs.size(); l++) {
      if (get_empty_space_n(vs[blank_idxs[l]]) >= vs[data_idxs[r]].size() &&
          (blank_idxs[l] < data_idxs[r])) {
        fill_empty_space(vs[blank_idxs[l]], vs[data_idxs[r]]);
        blank_idxs.push_back(data_idxs[r]);
        sort(blank_idxs.begin(), blank_idxs.end());
        break;
      }
    }
  }

  unsigned long long mult = 0;
  unsigned long long ans = 0;
  for (vector<string> s : vs) {
    for (string c : s) {

      if (c == ".") {
        mult++;
        continue;
      }
      ans += mult * stoi(c);
      mult++;
    }
  }
  cout << "second solution: " << ans << "\n";
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  string s;
  cin >> s;

  solve_1(s);
  solve_2(s);

  return 0;
}
