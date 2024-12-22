#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
using namespace std;

typedef vector<string> vs;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<bool> vb;
typedef vector<vb> vvb;
typedef pair<int, int> pii;

void print_in(vs &v) {
  for (auto s : v) {
    cout << s << endl;
  }
}

void parse_in(string &s, vs &in) {
  string t = "";
  pii p;
  for (auto c : s) {
    if (c == ',') {
      p.first = stoi(t);
      t = "";
      continue;
    }
    t += c;
  }

  p.second = stoi(t);

  in[p.second][p.first] = '#';
}

void solve_1(vs input) {
  priority_queue<pair<int, pii>> q;
  vvi distance = vvi(input.size(), vi(input.size(), INT_MAX));
  vvb processed = vvb(input.size(), vb(input.size(), false));
  const pii START = {0, 0};
  const pii END = {input.size() - 1, input.size() - 1};

  distance[START.first][START.second] = 0;
  q.push({0, START});

  while (!q.empty()) {
    pii a = q.top().second;
    q.pop();
    int i, j;
    tie(i, j) = a;
    if (processed[i][j])
      continue;
    processed[i][j] = true;
    const vector<pii> NEXT = {{i + 1, j}, {i - 1, j}, {i, j + 1}, {i, j - 1}};
    for (auto b : NEXT) {
      int i_next, j_next;
      tie(i_next, j_next) = b;
      if (i_next < 0 || j_next < 0 || i_next >= input.size() ||
          j_next >= input[i_next].size())
        continue;
      if (input[i_next][j_next] == '#')
        continue;

      if (distance[i][j] + 1 < distance[i_next][j_next]) {
        distance[i_next][j_next] = distance[i][j] + 1;
        q.push({-distance[i_next][j_next], {i_next, j_next}});
      }
    }
  }

  cout << "Solution 1: " << distance[END.first][END.second] << endl;
}

void solve_2(vs input, vs bytes) {
  for (auto s : bytes) {
    parse_in(s, input);
    priority_queue<pair<int, pii>> q;
    vvi distance = vvi(input.size(), vi(input.size(), INT_MAX));
    vvb processed = vvb(input.size(), vb(input.size(), false));
    const pii START = {0, 0};
    const pii END = {input.size() - 1, input.size() - 1};

    distance[START.first][START.second] = 0;
    q.push({0, START});

    while (!q.empty()) {
      pii a = q.top().second;
      q.pop();
      int i, j;
      tie(i, j) = a;
      if (processed[i][j])
        continue;
      processed[i][j] = true;
      const vector<pii> NEXT = {{i + 1, j}, {i - 1, j}, {i, j + 1}, {i, j - 1}};
      for (auto b : NEXT) {
        int i_next, j_next;
        tie(i_next, j_next) = b;
        if (i_next < 0 || j_next < 0 || i_next >= input.size() ||
            j_next >= input[i_next].size())
          continue;
        if (input[i_next][j_next] == '#')
          continue;

        if (distance[i][j] + 1 < distance[i_next][j_next]) {
          distance[i_next][j_next] = distance[i][j] + 1;
          q.push({-distance[i_next][j_next], {i_next, j_next}});
        }
      }
    }

    if (distance[END.first][END.second] == INT_MAX) {
      cout << "Solution 2: " << s << endl;
      return;
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  vector<string> bytes;
  vs input = vs(71, string(71, '.'));
  vs input_2 = input;
  string s;
  const int LIMIT = 1024;
  int count = 0;
  while (cin >> s) {
    bytes.push_back(s);
    if (count == LIMIT)
      continue;
    parse_in(s, input);
    count++;
  }

  solve_1(input);
  solve_2(input_2, bytes);

  return 0;
}
