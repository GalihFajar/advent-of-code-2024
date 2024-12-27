#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
using namespace std;

typedef vector<string> vs;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int, int> pii;
typedef vector<bool> vb;
typedef vector<vb> vvb;
typedef vector<pii> vp;
typedef vector<vp> vvp;
typedef long long ll;

vector<string> NUMPAD = {"789", "456", "123", "_0A"};
vector<string> ARROW_KEY = {"_^A", "<v>"};
unordered_map<char, pii> NUMPAD_MAP = {
    {'7', {0, 0}}, {'8', {0, 1}}, {'9', {0, 2}}, {'4', {1, 0}},
    {'5', {1, 1}}, {'6', {1, 2}}, {'1', {2, 0}}, {'2', {2, 1}},
    {'3', {2, 2}}, {'0', {3, 1}}, {'A', {3, 2}}, {'_', {3, 0}}};

unordered_map<char, pii> ARROW_KEY_MAP = {
    {'^', {0, 1}}, {'A', {0, 2}}, {'<', {1, 0}},
    {'v', {1, 1}}, {'>', {1, 2}}, {'_', {0, 0}},
};
vector<vector<vvp>> paths = vector<vector<vvp>>(200, vector<vvp>(200, {}));

string key(int i, int j) {
  string s = "";
  s += to_string(i);
  s += '#';
  s += to_string(j);

  return s;
}

pii parse_key(string s) {
  pii out;
  string temp = "";
  for (auto c : s) {
    if (c == '#') {
      out.first = stoi(temp);
      temp = "";
      continue;
    }

    temp += c;
  }

  out.second = stoi(temp);

  return out;
}

template <typename T> bool is_oob(int i, int j, const vector<T> &v) {
  return i < 0 || j < 0 || i >= v.size() || j >= v[0].size();
}

template <typename T> void print_pair(pair<T, T> p) {
  cout << "[";
  cout << p.first << " " << p.second;
  cout << "]";
}

template <typename T> void print_v(vector<T> &v) {
  for (auto x : v) {
    cout << x << endl;
  }
}

void bfs(pii start, pii end, vs &map) {
  queue<pair<pii, vp>> q;
  vvb visited = vvb(10, vb(10, false));
  int si, sj, ei, ej;
  tie(si, sj) = start;
  tie(ei, ej) = end;
  q.push({start, {}});

  while (!q.empty()) {
    pii a = q.front().first;
    vp path = q.front().second;
    q.pop();
    int i, j;
    tie(i, j) = a;

    visited[i][j] = true;
    path.push_back(a);

    if (i == ei && j == ej) {
      paths[map[si][sj]][map[ei][ej]].push_back(path);
      continue;
    }

    vector<pii> next = {{i + 1, j}, {i - 1, j}, {i, j + 1}, {i, j - 1}};

    for (auto n : next) {
      int i_next, j_next;
      tie(i_next, j_next) = n;

      if (is_oob(n.first, n.second, map) || map[i_next][j_next] == '_' ||
          visited[i_next][j_next]) {
        continue;
      }

      q.push({n, path});
    }
  }
}

void init() {
  string n = "0123456789A";
  string arr = "<^>vA";

  for (auto a : n) {
    for (auto b : n) {
      bfs(NUMPAD_MAP[a], NUMPAD_MAP[b], NUMPAD);
    }
  }

  for (auto a : arr) {
    for (auto b : arr) {
      bfs(ARROW_KEY_MAP[a], ARROW_KEY_MAP[b], ARROW_KEY);
    }
  }
}

string translate_path(vp &path) {
  if (path.empty())
    return "!";
  string out = "";

  for (int i = 0; i < path.size() - 1; i++) {
    pii p1 = path[i], p2 = path[i + 1];
    int i1, j1, i2, j2;
    tie(i1, j1) = p1;
    tie(i2, j2) = p2;

    if (i2 > i1) {
      out += 'v';
    } else if (i2 < i1) {
      out += '^';
    } else if (j2 > j1) {
      out += '>';
    } else if (j2 < j1) {
      out += '<';
    }
  }

  out += 'A';

  return out;
}

void get_base_paths(string command, int idx, string curr_path, vs &bp) {

  if (idx == command.size()) {
    bp.push_back(curr_path);
    return;
  }

  for (auto path : paths[idx - 1 < 0 ? 'A' : command[idx - 1]][command[idx]]) {
    get_base_paths(command, idx + 1, curr_path + translate_path(path), bp);
  }
}

unordered_map<string, ll> dp;

string key_2(string path, int depth) { return path + '#' + to_string(depth); }

void put_map(string key, ll value) {
  if (!dp.count(key)) {
    dp[key] = value;
  } else {
    if (dp[key] > value) {
      dp[key] = value;
    }
  }
}

ll get_min_path_len(string path, int depth, int max_depth) {
  if (dp.count(key_2(path, depth))) {
    return dp[key_2(path, depth)];
  }

  if (depth == max_depth) {
    put_map(key_2(path, depth), path.size());
    return path.size();
  }

  ll ret = 0;
  for (int i = 0; i < path.size(); i++) {
    ll sub_ret = LLONG_MAX;
    for (auto path : paths[i - 1 < 0 ? 'A' : path[i - 1]][path[i]]) {
      string next_path = translate_path(path);
      auto x = get_min_path_len(next_path, depth + 1, max_depth);
      sub_ret = min(sub_ret, x);
    }
    ret += sub_ret;
  }

  put_map(key_2(path, depth), ret);
  return ret;
}

int command_value(string &comm) {
  string temp = "";
  for (auto c : comm) {
    if (c != 'A') {
      temp += c;
    }
  }

  return stoi(temp);
}

void solve_1(vs input) {
  dp.clear();
  ll ans = 0;
  for (auto c : input) {
    vs bp;
    get_base_paths(c, 0, "", bp);
    ll best = LLONG_MAX;
    for (auto s : bp) {
      best = min(best, get_min_path_len(s, 1, 3));
    }
    ans += best * command_value(c);
  }

  cout << "Solution 1: " << ans << endl;
}

void solve_2(vs input) {
  dp.clear();
  ll ans = 0;
  for (auto c : input) {
    vs bp;
    get_base_paths(c, 0, "", bp);
    ll best = LLONG_MAX;
    for (auto s : bp) {
      best = min(best, get_min_path_len(s, 1, 26));
    }
    ans += best * command_value(c);
  }

  cout << "Solution 2: " << ans << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);
  init();
  string s;
  vs input;
  while (cin >> s) {
    input.push_back(s);
  }

  solve_1(input);
  solve_2(input);

  return 0;
}
