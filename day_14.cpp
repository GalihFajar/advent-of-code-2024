#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

const int X_LIMIT = 101;
const int Y_LIMIT = 103;

typedef struct Robot {
  pair<int, int> pos;
  pair<int, int> v;
} Robot;

typedef pair<int, int> pii;

Robot parse_input(string &a, string &b) {
  bool second = false;
  Robot out;
  string x, y;
  string *ptr = &x;
  for (auto c : a) {
    if (c == ',') {
      out.pos.first = stoi(*ptr);
      ptr = &y;
    }

    if ((c >= '0' && c <= '9') || c == '-') {
      *ptr += c;
    }
  }

  out.pos.second = stoi(*ptr);
  x.clear();
  y.clear();

  ptr = &x;
  for (auto c : b) {
    if (c == ',') {
      out.v.first = stoi(*ptr);
      ptr = &y;
    }

    if ((c >= '0' && c <= '9') || c == '-') {
      *ptr += c;
    }
  }
  out.v.second = stoi(*ptr);

  return out;
}

int mod(long long a, long long b) { return ((a % b) + b) % b; }

void move(Robot &r, unsigned long long s_elapsed) {
  long long new_x = (r.v.first * s_elapsed + r.pos.first);
  long long new_y = (r.v.second * s_elapsed + r.pos.second);

  r.pos.first = mod(new_x, X_LIMIT);
  r.pos.second = mod(new_y, Y_LIMIT);
}

vector<pair<pii, pii>> quadrants() {
  vector<pair<pii, pii>> out;
  out.push_back({{0, 0}, {X_LIMIT / 2 - 1, Y_LIMIT / 2 - 1}});
  out.push_back({{X_LIMIT / 2 + 1, 0}, {X_LIMIT - 1, Y_LIMIT / 2 - 1}});
  out.push_back(
      {{X_LIMIT / 2 + 1, Y_LIMIT / 2 + 1}, {X_LIMIT - 1, Y_LIMIT - 1}});
  out.push_back({{0, Y_LIMIT / 2 + 1}, {X_LIMIT / 2 - 1, Y_LIMIT - 1}});

  return out;
}

bool in_quadrants(pii a, pair<pii, pii> q) {

  if (a.first >= q.first.first && a.first <= q.second.first &&
      a.second >= q.first.second && a.second <= q.second.second) {
    return true;
  }
  return false;
}

void print_mat(vector<vector<int>> &v) {
  for (auto x : v) {
    for (auto elem : x) {
      if (elem == 0)
        cout << ".";
      else
        cout << "*";
    }
    cout << endl;
  }
}

void solve_1(vector<Robot> input) {
  vector<vector<int>> a(Y_LIMIT, vector<int>(X_LIMIT, 0));

  for (Robot &r : input) {
    move(r, 100);
    a[r.pos.second][r.pos.first]++;
  }
  /*print_mat(a);*/

  vector<pair<pii, pii>> q = quadrants();
  vector<int> total = {0, 0, 0, 0};

  for (auto r : input) {
    for (int i = 0; i < 4; i++) {
      if (in_quadrants(r.pos, q[i])) {
        total[i]++;
      }
    }
  }

  int ans = 1;
  for (auto t : total) {
    ans *= t;
  }

  cout << "Solution 1: " << ans << endl;
}

bool check_tree(vector<vector<int>> &a) {
  int mid_i = -1, mid_j = -1;
  for (int i = 0; i < a.size(); i++) {
    int total = 0;
    int temp;
    for (int j = 0; j < a[i].size(); j++) {
      if (a[i][j] != 0)
        total++;
    }
    if (total == 1) {
      mid_i = i;
      break;
    }
  }
  if (mid_i == -1)
    return false;

  for (int j = 0; j < a[mid_i].size(); j++) {
    if (a[mid_i][j] != 0) {
      mid_j = j;
      break;
    }
  }
  int l = mid_j - 1;
  int r = mid_j + 1;

  while (mid_i < a.size()) {
    int temp_l = l, temp_r = r;
    while (l < r) {
      if (a[mid_i][l] != 0 && a[mid_i][r] != 0) {
        l++;
        r--;
      } else {
        return false;
      }
    }
    mid_i++;
    l = temp_l - 1;
    r = temp_r - 1;
  }

  return true;
};

void solve_2(vector<Robot> input) {
  vector<Robot> temp = input;

  // NOTE: Heuristics -> chose with the most in a row element
  auto calculate_mat_score = [](vector<vector<int>> &a) -> int {
    auto calc_in_a_row = [](vector<int> &x) -> int {
      int ans = 0;

      int l = 0;
      while (l < x.size()) {
        if (x[l] != 0) {
          int current = 0;
          int r = l;
          while (x[r] != 0) {
            current++;
            r++;
          }
          ans = max(ans, current);
          l = r;
        } else {
          l++;
        }
      }

      return ans;
    };

    int score = -1;
    for (auto x : a) {
      score = max(score, calc_in_a_row(x));
    }

    return score;
  };

  long long ans = 0;

  int score_max = -1;
  int sec;

  vector<pair<int, int>> max_sec;

  vector<vector<int>> zz(Y_LIMIT, vector<int>(X_LIMIT, 0));

  for (Robot &r : input) {
    zz[r.pos.second][r.pos.first]++;
  }

  do {
    int seconds = 1;
    ans += seconds;
    for (Robot &r : input) {
      zz[r.pos.second][r.pos.first]--;
      move(r, seconds);
      zz[r.pos.second][r.pos.first]++;
    }
    int q_score = calculate_mat_score(zz);

    if (q_score >= score_max) {
      score_max = q_score;
      sec = ans;
      max_sec.push_back({score_max, sec});
    }
  } while (ans <= 10000);

  sort(max_sec.rbegin(), max_sec.rend());

  // NOTE: For visualizing purposes

  /*for (auto x : max_sec) {*/
  /*  printf("{score: %d, sec: %d}\n", x.first, x.second);*/
  /*  input = temp;*/
  /**/
  /*  vector<vector<int>> a(Y_LIMIT, vector<int>(X_LIMIT, 0));*/
  /*  cout << "max sec: " << x.second << endl;*/
  /*  for (Robot &r : input) {*/
  /*    move(r, x.second);*/
  /*    a[r.pos.second][r.pos.first]++;*/
  /*  }*/
  /**/
  /*  print_mat(a);*/
  /*  cout << "\n";*/
  /*  cout << "\n";*/
  /*  cout << "\n";*/
  /*}*/

  cout << "Solution 2: " << max_sec[0].second << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);
  string a, b;
  vector<Robot> input;

  while (cin >> a >> b) {
    input.push_back(parse_input(a, b));
  }

  solve_1(input);
  solve_2(input);

  return 0;
}
