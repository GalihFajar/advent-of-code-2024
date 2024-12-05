#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

void print_vs(vector<string> &vs) {
  for (auto s : vs) {
    cout << s << '\n';
  }
  cout << '\n';
}

vector<string> transpose(vector<string> &vs) {
  vector<string> out;
  for (int j = 0; j < vs[0].size(); j++) {
    string t = "";
    for (int i = 0; i < vs.size(); i++) {
      t += vs[i][j];
    }
    out.push_back(t);
  }

  return out;
}

int count_xmas(string &s) {
  int out = 0;
  string xmas = "XMAS";

  for (int i = 0; i < s.size(); i++) {

    if (i + 3 >= s.size())
      break;
    if (s.substr(i, 4) == "XMAS")
      out++;
  }

  return out;
}

// input 3 x 3
int is_xmas(vector<string> &vs) {
  string mas = "MAS";
  bool diag_1 = [&]() -> bool {
    string t;
    int i = 0, j = 0;
    while (i < 3 && j < 3) {
      t += vs[i][j];
      i++; j++;
    }
    if (t == mas)
      return true;
    reverse(t.begin(), t.end());
    if (t == mas)
      return true;

    return false;
  }();

  bool diag_2 = [&]() -> bool {
    int i = 0, j = 2;
    int match = 0;
    string t;
    while (i < 3 && j >= 0) {
      t += vs[i][j];
      i++; j--;
    }

    if (t == mas)
      return true;
    reverse(t.begin(), t.end());
    if (t == mas)
      return true;

    return false;
  }();

  return diag_1 && diag_2;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  vector<string> vs;
  string s;

  while (cin >> s) {
    vs.push_back(s);
  }

  auto solve_1 = [&]() {
    int out = 0;

    // check left to right + right to left
    for (string s_ : vs) {
      out += count_xmas(s_);
      reverse(s_.begin(), s_.end());
      out += count_xmas(s_);
    }

    // check up down + down up
    for (int j = 0; j < vs[0].size(); j++) {
      string s_;
      for (int i = 0; i < vs.size(); i++) {
        s_ += vs[i][j];
      }
      out += count_xmas(s_);
      reverse(s_.begin(), s_.end());
      out += count_xmas(s_);
    }

    auto count_diag_1 = [&]() {
      unordered_map<int, int> visited;

      for (int k = 0; k < vs[0].size(); k++) {
        int j = k, i = 0;
        string s_;
        while (j >= 0 && i >= 0 && j < vs[0].size() && i < vs.size()) {
          visited[i] = j;
          s_ += vs[i][j];
          i++;
          j--;
        }


        out += count_xmas(s_);
        reverse(s_.begin(), s_.end());
        out += count_xmas(s_);
      }

      /*for (int k = vs[0].size() - 1; k >= 1; k--) {*/
      for (int k = 0; k < vs[0].size(); k++) {
        int j = k, i = vs.size() - 1;
        string s_;
        while (j >= 0 && i >= 0 && j < vs[0].size() && i < vs.size()) {
          if (visited[i] == j) {
            break;
          }
          s_ += vs[i][j];
          i--;
          j++;
        }


        out += count_xmas(s_);
        reverse(s_.begin(), s_.end());
        out += count_xmas(s_);
      }
    };

    count_diag_1();

    auto count_diag_2 = [&]() {
      unordered_map<int, int> visited;

      for (int k = 0; k < vs[0].size(); k++) {
        int j = k, i = 0;
        string s_;
        while (j >= 0 && i >= 0 && j < vs[0].size() && i < vs.size()) {
          visited[i] = j;
          s_ += vs[i][j];
          i++;
          j++;
        }

        out += count_xmas(s_);
        reverse(s_.begin(), s_.end());
        out += count_xmas(s_);
      }

      for (int k = 0; k < vs[0].size(); k++) {
        int j = k, i = vs.size() - 1;
        string s_;
        while (j >= 0 && i >= 0 && j < vs[0].size() && i < vs.size()) {
          if (visited[i] == j) {
            break;
          }

          s_ += vs[i][j];
          i--;
          j--;
        }


        out += count_xmas(s_);
        reverse(s_.begin(), s_.end());
        out += count_xmas(s_);
      }
    };
    count_diag_2();
    cout << "first case: " << out << '\n';
  };

  auto solve_2 = [&]() {
    int out = 0;
    for (int i = 0; i < vs.size(); i++) {
      for (int j = 0; j < vs[i].size(); j++) {
        vector<string> vt;
        for (int k = i; k < i + 3 && k < vs.size(); k++) {
          string t = "";
          for (int l = j; l < j + 3 && l < vs[i].size(); l++) {
            t += vs[k][l];
          }
          vt.push_back(t);
        }
        if (vt.size() == 3 && vt.size() == vt[0].size()) {
          out += is_xmas(vt);
        }
      }
    }
    cout << "second case: " << out << '\n';
  };

  solve_1();
  solve_2();

  return 0;
}
