// Quite messy code
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
typedef vector<vector<int>> vvi;
typedef vector<bool> vb;
typedef vector<vb> vvb;
typedef vector<pair<int, int>> vpii;
typedef vector<vpii> vvpii;
typedef priority_queue<pair<int, pair<int, int>>> pq_pipii;

void print_vs(vs &v) {
  for (auto s : v) {
    cout << s << endl;
  }
}

void solve_1(vs input) {
  vvi distance = vvi(input.size(), vi(input[0].size(), INT_MAX));
  vvb processed = vvb(input.size(), vb(input[0].size(), false));
  vvpii best_prev = vvpii(input.size(), vpii(input[0].size(), {}));
  vector<vvpii> best_prev_v =
      vector<vvpii>(input.size(), vvpii(input[0].size(), {}));
  vector<vector<pq_pipii>> best_prev_q = vector<vector<pq_pipii>>(
      input.size(), vector<pq_pipii>(input[0].size(), {}));
  vs directions = input;
  pair<int, int> start_point, end_point;

  for (int i = 0; i < input.size(); i++) {
    for (int j = 0; j < input[i].size(); j++) {
      if (input[i][j] == 'S')
        start_point = {i, j};
      if (input[i][j] == 'E')
        end_point = {i, j};
    }
  }
  int i_start, j_start, i_end, j_end;
  tie(i_start, j_start) = start_point;
  tie(i_end, j_end) = end_point;

  distance[i_start][j_start] = 0;
  priority_queue<pair<int, pair<int, int>>> q;
  q.push({0, {i_start, j_start}});

  auto calculate_distance = [](char c, pair<int, int> current,
                               pair<int, int> next) -> int {
    if (c == '^' || c == 'v') {
      if (abs(current.first - next.first) == 1)
        return 1;
    } else if (c == '<' || c == '>') {
      if (abs(current.second - next.second) == 1)
        return 1;
    }

    return 1000;
  };

  auto det_direction = [](int i_1, int j_1, int i_2, int j_2) -> char {
    if (i_1 < i_2) {
      return 'v';
    } else if (i_1 > i_2) {
      return '^';
    } else if (j_1 < j_2) {
      return '>';
    } else if (j_1 > j_2) {
      return '<';
    }

    return '0';
  };

  char current_c = '^';
  vector<char> cs = {'^', 'v', '<', '>'};
  while (!q.empty()) {
    pair<int, int> a = q.top().second;
    int i = a.first, j = a.second;
    q.pop();
    if (processed[i][j])
      continue;

    processed[i][j] = true;

    if (input[i][j] == '#')
      continue;

    vector<pair<int, int>> next_pos = {
        {i + 1, j}, {i - 1, j}, {i, j + 1}, {i, j - 1}};

    for (auto p : next_pos) {
      int i_next = p.first, j_next = p.second;
      if (input[i_next][j_next] == '#') {
        continue;
      }

      int dist = calculate_distance(directions[i][j], {i, j}, {i_next, j_next});
      if (distance[i][j] + dist < distance[i_next][j_next]) {
        distance[i_next][j_next] = distance[i][j] + dist;
        q.push({-distance[i_next][j_next], {i_next, j_next}});
        best_prev[i_next][j_next] = {i, j};
        best_prev_v[i_next][j_next].push_back({i, j});
        best_prev_q[i_next][j_next].push({-distance[i_next][j_next], {i, j}});
        current_c = det_direction(i, j, i_next, j_next);

        directions[i_next][j_next] = current_c;
      }
    }
  }

  int x = best_prev_q[i_end][j_end].top().second.first,
      y = best_prev_q[i_end][j_end].top().second.second;
  if (best_prev_q[i_end][j_end].size() > 1)
    best_prev_q[i_end][j_end].pop();

  int ans = 0;

  char curr_direction = det_direction(i_end, j_end, x, y);
  int straight = 0, turn = 0;

  while (x != i_start || y != j_start) {
    input[x][y] = curr_direction;
    pair<int, int> next_best = best_prev[x][y];
    char next_direction =
        det_direction(x, y, next_best.first, next_best.second);
    if (next_direction == curr_direction) {
      ans += 1;
      straight++;
    } else {
      ans += 1000;
      ans++;
      straight++;
      turn++;
    }
    curr_direction = next_direction;
    tie(x, y) = next_best;
  }
  straight++;
  ans++;

  if (curr_direction != '<') {
    turn++;
    ans += 1000;
  }

  /*print_vs(input);*/

  /*cout << "distance: " << distance[i_end][j_end] << endl;*/
  /*cout << "straight: " << straight << " turn: " << turn << endl;*/
  cout << "Solution 1: " << ans << endl;
}

string str_key(int i, int j) {
  string s = "";
  s += to_string(i);
  s += '#';
  s += to_string(j);

  return s;
}

void process_node(pair<int, int> node_point, vvpii best_prev,
                  vector<vector<bool>> &processed, int &out, vs &input,
                  unordered_map<string, bool> &best_paths, bool& start) {
  int i, j;
  tie(i, j) = node_point;
  if (best_prev[i][j].first == i && best_prev[i][j].second == j) return;
  /*cout << i << " " << j << endl;*/
  /*processed[i][j] = true;*/
  if (!start) {
    best_paths[str_key(i, j)] = true;
    /*input[i][j] = 'O';*/
    /*out++;*/
  }

  if (start) {
    start = !start;
  }
  process_node(best_prev[i][j], best_prev, processed, out, input, best_paths, start);
}

pair<int, vvpii> djikstra(vs input, pair<int, int> start_point,
                          pair<int, int> end_point) {
  auto calculate_distance = [](char c, pair<int, int> current,
                               pair<int, int> next) -> int {
    if (c == '^' || c == 'v') {
      if (abs(current.first - next.first) == 1)
        return 1;
    } else if (c == '<' || c == '>') {
      if (abs(current.second - next.second) == 1)
        return 1;
    }

    return 1000;
  };

  auto det_direction = [](int i_1, int j_1, int i_2, int j_2) -> char {
    if (i_1 < i_2) {
      return 'v';
    } else if (i_1 > i_2) {
      return '^';
    } else if (j_1 < j_2) {
      return '>';
    } else if (j_1 > j_2) {
      return '<';
    }

    return '0';
  };

  int i_start, j_start, i_end, j_end;
  tie(i_start, j_start) = start_point;
  tie(i_end, j_end) = end_point;

  vs directions = input;
  vvpii best_prev = vvpii(input.size(), vpii(input[0].size(), {}));
  for (int i = 0; i < input.size(); i++) {
    for (int j = 0; j < input[0].size(); j++){
      best_prev[i][j] = {i, j};
    }
  }
  priority_queue<pair<int, pair<int, int>>> q;
  vvb processed = vvb(input.size(), vb(input[0].size(), false));
  vvi distance = vvi(input.size(), vi(input[0].size(), INT_MAX));

  distance[i_start][j_start] = 0;
  q.push({0, {i_start, j_start}});

  char current_c = '^';
  vector<char> cs = {'^', 'v', '<', '>'};
  while (!q.empty()) {
    pair<int, int> a = q.top().second;
    int i = a.first, j = a.second;
    q.pop();
    if (processed[i][j])
      continue;

    processed[i][j] = true;

    if (input[i][j] == '#')
      continue;

    vector<pair<int, int>> next_pos = {
        {i + 1, j}, {i - 1, j}, {i, j + 1}, {i, j - 1}};

    for (auto p : next_pos) {
      int i_next = p.first, j_next = p.second;
      if (input[i_next][j_next] == '#') {
        continue;
      }

      int dist = calculate_distance(directions[i][j], {i, j}, {i_next, j_next});
      if (distance[i][j] + dist < distance[i_next][j_next]) {

        distance[i_next][j_next] = distance[i][j] + dist;
        q.push({-distance[i_next][j_next], {i_next, j_next}});
        best_prev[i_next][j_next] = {i, j};

        current_c = det_direction(i, j, i_next, j_next);
        directions[i_next][j_next] = current_c;
      }
    }
  }

  return {distance[i_end][j_end], best_prev};
}

pair<int, int> parse_key(string s) {
  string temp = "";
  pair<int, int> out;

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

void solve_2(vs input) {
  vs orig_input = input;
  vvi distance = vvi(input.size(), vi(input[0].size(), INT_MAX));
  vvb processed = vvb(input.size(), vb(input[0].size(), false));
  vvpii best_prev;
  vs directions = input;
  pair<int, int> start_point, end_point;
  unordered_map<string, bool> best_paths;

  for (int i = 0; i < input.size(); i++) {
    for (int j = 0; j < input[i].size(); j++) {
      if (input[i][j] == 'S')
        start_point = {i, j};
      if (input[i][j] == 'E')
        end_point = {i, j};
    }
  }

  auto d_res = djikstra(input, start_point, end_point);
  vvpii best_prev_v = d_res.second;

  int best = d_res.first;
  processed = vvb(input.size(), vb(input[0].size(), false));
  bool start = true;
  process_node(end_point, best_prev_v, processed, best, input, best_paths, start);

  // Calculate next n paths with the same distance as best path
  for (auto x: best_paths) {
    int blocked_i = 11,  blocked_j = 2;
    auto p = parse_key(x.first);
    tie(blocked_i, blocked_j) = p;

    input[blocked_i][blocked_j] = '#';
    auto n_dr = djikstra(input, start_point, end_point);
    start = true;
    if (n_dr.first <= best) {
      process_node(end_point, n_dr.second, processed, best, input, best_paths, start);
    }
    input[blocked_i][blocked_j] = '.';
  }

  int ans = 0;

  for (auto x: best_paths) {
    ans++;
    int blocked_i, blocked_j;
    auto p = parse_key(x.first);
    tie(blocked_i, blocked_j) = p;

    input[blocked_i][blocked_j] = 'O';
  }

  print_vs(input);
  cout << endl;
  cout << "Solution 2: " << ans + 2 << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  string s;
  vector<string> input;

  while (cin >> s) {
    input.push_back(s);
  }

  solve_1(input);
  /*solve_2(input);*/

  return 0;
}
