#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
using namespace std;

typedef vector<string> vs;
typedef pair<int, int> pii;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<bool> vb;
typedef vector<vb> vvb;
typedef vector<pii> vpii;

struct State {
  pii position;
  int rem_bypass;
  int cost;
  string bypass_ends;
};

void print_out(vs &v) {
  for (auto s : v) {
    cout << s << endl;
  }
}

int oob(int i, int j, vs &v) {
  return i < 0 || j < 0 || i >= (int)v.size() || j >= (int)v[i].size();
}

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

bool is_digit(char c) { return c >= '0' && c <= '9'; }

int djikstra_1(vs &in, pii start, pii end, unordered_map<string, bool> &best) {
  vvi distance = vvi(in.size(), vi(in[0].size(), INT_MAX));
  vvb visited = vvb(in.size(), vb(in[0].size(), false));
  priority_queue<pair<int, pii>> q;
  q.push({0, start});
  distance[start.first][start.second] = 0;

  while (!q.empty()) {
    pii a = q.top().second;
    q.pop();
    int i, j;
    tie(i, j) = a;
    if (visited[i][j])
      continue;
    vpii NEXT = {{i + 1, j}, {i - 1, j}, {i, j + 1}, {i, j - 1}};
    char c = in[i][j];
    if (is_digit(c)) {
      NEXT.clear();

      if (in[i + 1][j] == c + 1) {
        NEXT.push_back({i + 1, j});
      } else if (in[i - 1][j] == c + 1) {
        NEXT.push_back({i - 1, j});
      } else if (in[i][j + 1] == c + 1) {
        NEXT.push_back({i, j + 1});
      } else if (in[i][j - 1] == c + 1) {
        NEXT.push_back({i, j - 1});
      } else {
        return INT_MAX;
      }
    }

    for (auto p : NEXT) {
      int i_next, j_next;
      tie(i_next, j_next) = p;
      if (oob(p.first, p.second, in) || in[i_next][j_next] == '#')
        continue;

      if (distance[i][j] + 1 < distance[i_next][j_next]) {
        distance[i_next][j_next] = distance[i][j] + 1;
        q.push({-distance[i_next][j_next], {i_next, j_next}});
        string s = "";
        s += in[i][j];
        s += in[i_next][j_next];
        best[s] = true;
        s += to_string(i_next);
        s += '#';
        s += to_string(j_next);
      }
    }
  }

  return distance[end.first][end.second];
}

int manhattan(pii p1, pii p2) {
  return abs(p1.first - p2.first) + abs(p1.second - p2.second);
}

vvi djikstra_only(vs &in, pii start, pii end) {
  vvi distance = vvi(in.size(), vi(in[0].size(), INT_MAX));
  vvb visited = vvb(in.size(), vb(in[0].size(), false));
  priority_queue<pair<int, pii>> q;
  q.push({0, start});
  distance[start.first][start.second] = 0;

  while (!q.empty()) {
    pii a = q.top().second;
    q.pop();
    int i, j;
    tie(i, j) = a;
    if (visited[i][j])
      continue;
    vpii NEXT = {{i + 1, j}, {i - 1, j}, {i, j + 1}, {i, j - 1}};
    if (i == end.first && j == end.second) {
      break;
    }

    for (auto p : NEXT) {
      int i_next, j_next;
      tie(i_next, j_next) = p;
      if (oob(p.first, p.second, in) || in[i_next][j_next] == '#')
        continue;

      if (distance[i][j] + 1 < distance[i_next][j_next]) {
        distance[i_next][j_next] = distance[i][j] + 1;
        q.push({-distance[i_next][j_next], {i_next, j_next}});
      }
    }
  }

  return distance;
}

vvi djikstra_gen(vs &in, pii start, pii end,
                 unordered_map<string, bool> &best) {
  vvi distance = vvi(in.size(), vi(in[0].size(), INT_MAX));
  vvb visited = vvb(in.size(), vb(in[0].size(), false));
  priority_queue<pair<int, pii>> q;
  q.push({0, start});
  distance[start.first][start.second] = 0;
  best[key(start.first, start.second)] = true;
  best[key(end.first, end.second)] = true;

  while (!q.empty()) {
    pii a = q.top().second;
    q.pop();
    int i, j;
    tie(i, j) = a;
    if (visited[i][j])
      continue;
    vpii NEXT = {{i + 1, j}, {i - 1, j}, {i, j + 1}, {i, j - 1}};
    if (i == end.first && j == end.second) {
      break;
    }

    for (auto p : NEXT) {
      int i_next, j_next;
      tie(i_next, j_next) = p;
      if (oob(p.first, p.second, in) || in[i_next][j_next] == '#')
        continue;

      if (distance[i][j] + 1 < distance[i_next][j_next]) {
        distance[i_next][j_next] = distance[i][j] + 1;
        q.push({-distance[i_next][j_next], {i_next, j_next}});
        string s = "";
        best[key(i, j)] = true;
      }
    }
  }

  return distance;
}


void solve_1(vs &in) {
  vs clear_map = vs(in.size(), string(in[0].size(), '.'));
  pii start, end;
  for (int i = 0; i < in.size(); i++) {
    for (int j = 0; j < in[i].size(); j++) {
      if (in[i][j] == 'S')
        start = {i, j};
      if (in[i][j] == 'E')
        end = {i, j};
    }
  }

  unordered_map<string, bool> b;
  unordered_map<string, int> saved_new;
  vvi distances = djikstra_gen(in, start, end, b);
  int ans = 0;
  const int LIMIT = 100;
  const int RANGE = 2;

  for (auto x : b) {
    pii curr = parse_key(x.first);
    int i, j;
    tie(i, j) = curr;

    for (int k = -RANGE; k <= RANGE; k++) {
      for (int l = -RANGE; l <= RANGE; l++) {
        if (oob(i + k, j + l, in))
          continue;
        pii new_p = {i + k, j + l};
        if (in[new_p.first][new_p.second] != '#' &&
            manhattan(curr, new_p) == RANGE) {
          int new_to_end = saved_new[key(new_p.first, new_p.second)] ? saved_new[key(new_p.first, new_p.second)] : djikstra_only(in, new_p, end)[end.first][end.second];
          saved_new[key(new_p.first, new_p.second)] = new_to_end;
          int new_dist = distances[i][j] + manhattan(curr, new_p) + new_to_end;
          if (distances[end.first][end.second] - new_dist >= LIMIT) {
            ans++;
          }
        }
      }
    }
  }

  cout << "Solution 1: " << ans << endl;
}

void solve_2(vs &in) {
  vs clear_map = vs(in.size(), string(in[0].size(), '.'));
  pii start, end;
  for (int i = 0; i < in.size(); i++) {
    for (int j = 0; j < in[i].size(); j++) {
      if (in[i][j] == 'S')
        start = {i, j};
      if (in[i][j] == 'E')
        end = {i, j};
    }
  }

  unordered_map<string, bool> b;
  unordered_map<string, int> saved_new;
  vvi distances = djikstra_gen(in, start, end, b);
  int ans = 0;
  const int LIMIT = 100;
  const int RANGE = 20;

  for (auto x : b) {
    pii curr = parse_key(x.first);
    int i, j;
    tie(i, j) = curr;

    for (int k = -RANGE; k <= RANGE; k++) {
      for (int l = -RANGE; l <= RANGE; l++) {
        if (oob(i + k, j + l, in))
          continue;
        pii new_p = {i + k, j + l};
        if (in[new_p.first][new_p.second] != '#' &&
            manhattan(curr, new_p) <= RANGE) {
          int new_to_end = saved_new[key(new_p.first, new_p.second)] ? saved_new[key(new_p.first, new_p.second)] : djikstra_only(in, new_p, end)[end.first][end.second];
          saved_new[key(new_p.first, new_p.second)] = new_to_end;
          int new_dist = distances[i][j] + manhattan(curr, new_p) + new_to_end;
          if (distances[end.first][end.second] - new_dist >= LIMIT) {
            ans++;
          }
        }
      }
    }
  }

  cout << "Solution 2: " << ans << endl;
}

void print_path(vs &v) {
  for (int i = 0; i < v.size(); i++) {
    if (i != v.size() - 1) {
      cout << v[i] << " --> ";
    } else {
      cout << v[i] << "\n";
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  vs input;
  string s;

  while (cin >> s) {
    input.push_back(s);
  }

  solve_1(input);
  solve_2(input);

  return 0;
}
