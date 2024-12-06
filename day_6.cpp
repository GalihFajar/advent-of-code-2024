#include <algorithm>
#include <climits>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <unordered_map>
#include <vector>
using namespace std;

bool is_exiting(vector<string> &vs, pair<int, int> &curr_pos) {
  int i = curr_pos.first, j = curr_pos.second;
  if (i == 0 && vs[i][j] == '^')
    return true;
  if (i == vs.size() - 1 && vs[i][j] == 'v')
    return true;
  if (j == 0 && vs[i][j] == '<')
    return true;
  if (j == vs[i].size() - 1 && vs[i][j] == '>') {
    return true;
  }

  return false;
}

bool is_oob(vector<string> &vs, pair<int, int> curr_pos) {
  int i = curr_pos.first, j = curr_pos.second;
  return i < 0 || j < 0 || i >= vs.size() || j >= vs[i].size();
}

void print_path(vector<string> &v) {
  for (string &s : v) {
    cout << s << "\n";
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  vector<string> v;
  string s;
  while (cin >> s) {
    v.push_back(s);
  }

  pair<int, int> pos;
  for (int i = 0; i < v.size(); i++) {
    for (int j = 0; j < v[i].size(); j++) {
      if (v[i][j] == '^') {
        pos = {i, j};
      }
    }
  }

  auto solve_first = [&]() {
    vector<string> path = v, temp = v;
    pair<int, int> start_pos;

    start_pos.first = pos.first;
    start_pos.second = pos.second;
    while (1) {
      int &i = pos.first, &j = pos.second;
      vector<string> temp = v;
      if (is_exiting(v, pos)) {
        path[i][j] = 'X';
        break;
      }

      if (v[i][j] == '^') {
        if (!is_oob(v, {i - 1, j})) {
          if (v[i - 1][j] == '#') {
            v[i][j] = '>';
          } else {
            v[i][j] = '.';
            path[i][j] = 'X';
            v[i - 1][j] = '^';
            i--;
          }
        }
      } else if (v[i][j] == 'v') {
        if (!is_oob(v, {i + 1, j})) {
          if (v[i + 1][j] == '#') {
            v[i][j] = '<';
          } else {
            v[i][j] = '.';
            path[i][j] = 'X';
            v[i + 1][j] = 'v';
            i++;
          }
        }
      } else if (v[i][j] == '>') {
        if (!is_oob(v, {i, j + 1})) {
          if (v[i][j + 1] == '#') {
            v[i][j] = 'v';
          } else {
            v[i][j] = '.';
            path[i][j] = 'X';
            v[i][j + 1] = '>';
            j++;
          }
        }
      } else if (v[i][j] == '<') {
        if (!is_oob(v, {i, j - 1})) {
          if (v[i][j - 1] == '#') {
            v[i][j] = '^';
          } else {
            v[i][j] = '.';
            path[i][j] = 'X';
            v[i][j - 1] = '<';
            j--;
          }
        }
      }
    }

    int count = 0;
    for (int i = 0; i < path.size(); i++) {
      for (int j = 0; j < path[i].size(); j++) {
        if (path[i][j] == 'X') {
          count++;
        }
      }
    }

    v = temp;
    pos = start_pos;
    cout << "first solution: " << count << endl;
  };

  auto solve_second = [&]() {
    int ans = 0;
    pair<int, int> start_pos;
    start_pos.first = pos.first;
    start_pos.second = pos.second;
    vector<string> temp = v;

    for (int i = 0; i < v.size(); i++) {
      for (int j = 0; j < v[i].size(); j++) {
        if (v[i][j] == '#' || v[i][j] == '^')
          continue;

        pair<int, int> obs = {i, j};
        v[obs.first][obs.second] = '#';
        bool first = true;
        unordered_map<string, bool> visited;

        while (1) {
          int &k = pos.first, &l = pos.second;
          string t_ = "";
          t_ += k;
          t_ += l;
          t_ += v[k][l];

          if (visited[t_]) {
            ans++;
            break;
          }

          visited[t_] = true;

          if (is_exiting(v, pos)) {
            break;
          }

          if (v[k][l] == '^') {
            if (!is_oob(v, {k - 1, l})) {
              if (v[k - 1][l] == '#') {
                v[k][l] = '>';
              } else {
                v[k][l] = '.';
                v[k - 1][l] = '^';
                k--;
              }
            }
          } else if (v[k][l] == 'v') {
            if (!is_oob(v, {k + 1, l})) {
              if (v[k + 1][l] == '#') {
                v[k][l] = '<';
              } else {
                v[k][l] = '.';
                v[k + 1][l] = 'v';
                k++;
              }
            }
          } else if (v[k][l] == '>') {
            if (!is_oob(v, {k, l + 1})) {
              if (v[k][l + 1] == '#') {
                v[k][l] = 'v';
              } else {
                v[k][l] = '.';
                v[k][l + 1] = '>';
                l++;
              }
            }
          } else if (v[k][l] == '<') {
            if (!is_oob(v, {k, l - 1})) {
              if (v[k][l - 1] == '#') {
                v[k][l] = '^';
              } else {
                v[k][l] = '.';
                v[k][l - 1] = '<';
                l--;
              }
            }
          }
        }

        v = temp;
        pos = start_pos;
      }
    }

    cout << "second solution: " << ans << endl;
  };

  solve_first();
  // Will run for a much longer (roughly O(n^4) complexity)
  solve_second();

  return 0;
}
