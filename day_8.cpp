#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;
typedef vector<string> vs;

template <typename T> bool check_if_exists(unordered_set<T> &u, T elem) {
  if (u.find(elem) == u.end()) {
    return false;
  }

  return true;
}

void print_vs(vector<string> &vs) {
  for (auto s : vs) {
    cout << s << endl;
  }
}

bool is_out_of_bound(int row, int col, vs &input) {
  return row < 0 || col < 0 || row >= (int)input.size() ||
         col >= (int)input[0].size();
}

void solve_1(vs &input) {
  unordered_map<char, vector<pair<int, int>>> char_pos;
  unordered_set<string> antinodes;
  vector<string> temp;

  for (int i = 0; i < input.size(); i++) {
    for (int j = 0; j < input[i].size(); j++) {
      if (input[i][j] == '.')
        continue;
      char_pos[input[i][j]].push_back({i, j});
    }
  }

  for (auto chr : char_pos) {
    vector<pair<int, int>> &pos = chr.second;
    for (int i = 0; i < pos.size(); i++) {
      for (int j = 0; j < pos.size(); j++) {
        if (i == j)
          continue;
        int row_diff = pos[j].first - pos[i].first;
        int col_diff = pos[j].second - pos[i].second;
        int a = row_diff * 2 + pos[i].first;
        int b = col_diff * 2 + pos[i].second;
        if (is_out_of_bound(a, b, input))
          continue;

        string t = "";
        t += to_string(a);
        t += '#';
        t += to_string(b);

        antinodes.insert(t);
      }
    }
  }

  cout << "first solution: " << (int)antinodes.size() << endl;
}

void solve_2(vs &input) {
  unordered_map<char, vector<pair<int, int>>> char_pos;
  unordered_set<string> antinodes;
  vector<string> temp;

  for (int i = 0; i < input.size(); i++) {
    for (int j = 0; j < input[i].size(); j++) {
      if (input[i][j] == '.')
        continue;
      char_pos[input[i][j]].push_back({i, j});
    }
  }

  for (auto chr : char_pos) {
    vector<pair<int, int>> &pos = chr.second;
    for (int i = 0; i < pos.size(); i++) {
      for (int j = 0; j < pos.size(); j++) {
        if (i == j)
          continue;
        int row_diff = pos[j].first - pos[i].first;
        int col_diff = pos[j].second - pos[i].second;
        int a;
        int b;
        string t = "";
        int factor = 1;

        do {
          a = row_diff * factor + pos[i].first;
          b = col_diff * factor + pos[i].second;
          string t = "";
          t += to_string(a);
          t += '#';
          t += to_string(b);
          if (!is_out_of_bound(a, b, input)) {
            antinodes.insert(t);
          }

          factor++;
        } while (!is_out_of_bound(a, b, input));
      }
    }
  }

  cout << "second solution: " << (int)antinodes.size() << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);
  string s;
  vector<string> input;

  while (getline(cin, s)) {
    input.push_back(s);
  }

  solve_1(input);
  solve_2(input);

  return 0;
}
