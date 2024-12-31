#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

typedef vector<string> vs;

vector<vector<int>> keys;
vector<vector<int>> locks;

void parse_input(vector<string> &vs) {
  if (vs[0][0] == '#') {
    vector<int> o;
    for (int j = 0; j < vs[0].size(); j++) {
      int cnt = 0;
      for (int i = 1; i < vs.size() - 1; i++) {
        if (vs[i][j] == '#')
          cnt++;
      }
      o.push_back(cnt);
    }

    locks.push_back(o);
  } else {
    vector<int> o;
    for (int j = 0; j < vs[0].size(); j++) {
      int cnt = 0;
      for (int i = vs.size() - 2; i > 0; i--) {
        if (vs[i][j] == '#')
          cnt++;
      }

      o.push_back(cnt);
    }

    keys.push_back(o);
  }
}

void solve_1() {
  int ans = 0;
  for (auto k : keys) {
    for (auto l : locks) {
      auto calc = [&]() -> int {
        for (int i = 0; i < k.size(); i++) {
          if (k[i] + l[i] > 5)
            return 0;
        }

        return 1;
      };

      ans += calc();
    }
  }

  cout << "Solution 1: " << ans << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);
  string s;
  vector<vector<string>> input;
  vector<string> temp;
  while (1) {
    cin >> s;
    if (cin.eof())
      break;
    temp.push_back(s);
    if (temp.size() == 7) {
      input.push_back(temp);
      parse_input(temp);
      temp.clear();
    }
  }

  solve_1();

  return 0;
}
