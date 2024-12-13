#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

unordered_map<char, int> areas;

typedef struct Node {
  char value;
  // up right down left
  vector<int> perimeters = {1, 1, 1, 1};
} Node;

bool is_oob(int i, int j, vector<vector<Node>> &mat) {
  return i < 0 || j < 0 || i >= mat.size() || j >= mat[i].size();
}

int dfs(int i, int j, vector<vector<Node>> &mat, vector<vector<bool>> &visited,
        int &area) {
  visited[i][j] = true;
  area++;
  int ans = 0;

  if (!is_oob(i - 1, j, mat) && !visited[i - 1][j] &&
      mat[i][j].value == mat[i - 1][j].value) {
    ans += dfs(i - 1, j, mat, visited, area);
  }

  if (!is_oob(i + 1, j, mat) && !visited[i + 1][j] &&
      mat[i][j].value == mat[i + 1][j].value) {
    ans += dfs(i + 1, j, mat, visited, area);
  }

  if (!is_oob(i, j - 1, mat) && !visited[i][j - 1] &&
      mat[i][j].value == mat[i][j - 1].value) {
    ans += dfs(i, j - 1, mat, visited, area);
  }

  if (!is_oob(i, j + 1, mat) && !visited[i][j + 1] &&
      mat[i][j].value == mat[i][j + 1].value) {
    ans += dfs(i, j + 1, mat, visited, area);
  }

  if (!is_oob(i - 1, j, mat) && mat[i][j].value == mat[i - 1][j].value &&
      visited[i - 1][j]) {
    mat[i][j].perimeters[0] = 0;
    mat[i - 1][j].perimeters[2] = 0;
  }

  if (!is_oob(i + 1, j, mat) && mat[i][j].value == mat[i + 1][j].value &&
      visited[i + 1][j]) {
    mat[i][j].perimeters[2] = 0;
    mat[i + 1][j].perimeters[0] = 0;
  }

  if (!is_oob(i, j - 1, mat) && mat[i][j].value == mat[i][j - 1].value &&
      visited[i][j - 1]) {
    mat[i][j].perimeters[3] = 0;
    mat[i][j - 1].perimeters[1] = 0;
  }

  if (!is_oob(i, j + 1, mat) && mat[i][j].value == mat[i][j + 1].value &&
      visited[i][j + 1]) {
    mat[i][j].perimeters[1] = 0;
    mat[i][j + 1].perimeters[3] = 0;
  }

  ans += mat[i][j].perimeters[0] + mat[i][j].perimeters[1] +
         mat[i][j].perimeters[2] + mat[i][j].perimeters[3];

  return ans;
}

void delete_elem(vector<pair<int, int>> &v, pair<int, int> p) {
  int pos = -1;
  for (int i = 0; i < v.size(); i++) {
    if (v[i].first == p.first && v[i].second == p.second) {
      pos = i;
      break;
    }
  }

  if (pos >= 0)
    v.erase(v.begin() + pos);
}

void dfs_2(int i, int j, vector<vector<Node>> &mat,
           vector<vector<bool>> &visited, vector<vector<pair<int, int>>> &sides,
           int &area) {
  visited[i][j] = true;
  area++;
  sides[0].push_back({i, j});
  sides[1].push_back({i, j});
  sides[2].push_back({i, j});
  sides[3].push_back({i, j});
  int ans = 0;

  if (!is_oob(i - 1, j, mat) && !visited[i - 1][j] &&
      mat[i][j].value == mat[i - 1][j].value) {
    dfs_2(i - 1, j, mat, visited, sides, area);
  }

  if (!is_oob(i + 1, j, mat) && !visited[i + 1][j] &&
      mat[i][j].value == mat[i + 1][j].value) {
    dfs_2(i + 1, j, mat, visited, sides, area);
  }

  if (!is_oob(i, j - 1, mat) && !visited[i][j - 1] &&
      mat[i][j].value == mat[i][j - 1].value) {
    dfs_2(i, j - 1, mat, visited, sides, area);
  }

  if (!is_oob(i, j + 1, mat) && !visited[i][j + 1] &&
      mat[i][j].value == mat[i][j + 1].value) {
    dfs_2(i, j + 1, mat, visited, sides, area);
  }

  if (!is_oob(i - 1, j, mat) && mat[i][j].value == mat[i - 1][j].value &&
      visited[i - 1][j]) {
    mat[i][j].perimeters[0] = 0;
    mat[i - 1][j].perimeters[2] = 0;

    delete_elem(sides[0], {i, j});
    delete_elem(sides[2], {i - 1, j});
  }

  if (!is_oob(i + 1, j, mat) && mat[i][j].value == mat[i + 1][j].value &&
      visited[i + 1][j]) {
    mat[i][j].perimeters[2] = 0;
    mat[i + 1][j].perimeters[0] = 0;

    delete_elem(sides[2], {i, j});
    delete_elem(sides[0], {i + 1, j});
  }

  if (!is_oob(i, j - 1, mat) && mat[i][j].value == mat[i][j - 1].value &&
      visited[i][j - 1]) {
    mat[i][j].perimeters[3] = 0;
    mat[i][j - 1].perimeters[1] = 0;

    delete_elem(sides[3], {i, j});
    delete_elem(sides[1], {i, j - 1});
  }

  if (!is_oob(i, j + 1, mat) && mat[i][j].value == mat[i][j + 1].value &&
      visited[i][j + 1]) {
    mat[i][j].perimeters[1] = 0;
    mat[i][j + 1].perimeters[3] = 0;

    delete_elem(sides[1], {i, j});
    delete_elem(sides[3], {i, j + 1});
  }
}

vector<vector<Node>> parse_input(vector<string> &vs) {
  vector<vector<Node>> out;

  for (auto s : vs) {
    vector<Node> temp;
    for (auto c : s) {
      Node n;
      n.value = c;
      temp.push_back(n);
    }
    out.push_back(temp);
  }

  return out;
}

void solve_1(vector<vector<Node>> input) {
  vector<vector<bool>> visited(input.size(),
                               vector<bool>(input[0].size(), false));
  int ans = 0;

  for (int i = 0; i < input.size(); i++) {
    for (int j = 0; j < input[i].size(); j++) {
      if (visited[i][j])
        continue;
      int area = 0;
      int perim = dfs(i, j, input, visited, area);
      ans += perim * area;
    }
  }

  cout << "Solution 1: " << ans << endl;
}

int process_sides(vector<vector<pair<int, int>>> &sides) {
  auto sort_row = [](pair<int, int>& p1, pair<int, int>& p2) -> bool {
    if (p1.first == p2.first)
      return p1.second < p2.second;

    return p1.first < p2.first;
  };

  auto sort_col = [](pair<int, int>& p1, pair<int, int>& p2) -> bool {
    if (p1.second == p2.second)
      return p1.first < p2.first;

    return p1.second < p2.second;
  };

  sort(sides[0].begin(), sides[0].end(), sort_row);
  sort(sides[1].begin(), sides[1].end(), sort_col);
  sort(sides[2].begin(), sides[2].end(), sort_row);
  sort(sides[3].begin(), sides[3].end(), sort_col);

  int up = 0, right = 0, down = 0, left = 0;

  for (int i = 0; i < sides[0].size(); i++) {
    if (i > 0 && sides[0][i].first == sides[0][i - 1].first && sides[0][i].second - sides[0][i - 1].second == 1)
      continue;
    up++;
  }

  for (int i = 0; i < sides[2].size(); i++) {
    if (i > 0 && sides[2][i].first == sides[2][i - 1].first && sides[2][i].second - sides[2][i - 1].second == 1)
      continue;
    down++;
  }

  for (int i = 0; i < sides[1].size(); i++) {
    if (i > 0 && sides[1][i].second == sides[1][i - 1].second && sides[1][i].first - sides[1][i - 1].first == 1)
      continue;
    right++;
  }

  for (int i = 0; i < sides[3].size(); i++) {
    if (i > 0 && sides[3][i].second == sides[3][i - 1].second && sides[3][i].first - sides[3][i - 1].first == 1)
      continue;
    left++;
  }

  return up + left + right + down;
}

void solve_2(vector<vector<Node>> input) {
  vector<vector<bool>> visited(input.size(),
                               vector<bool>(input[0].size(), false));
  unordered_map<char, int> areas;
  int ans = 0;

  for (int i = 0; i < input.size(); i++) {
    for (int j = 0; j < input[i].size(); j++) {
      if (visited[i][j])
        continue;
      vector<vector<pair<int, int>>> sides = {{}, {}, {}, {}};
      int area = 0;
      dfs_2(i, j, input, visited, sides, area);

      ans += area * process_sides(sides);
   }
  }

  cout << "Solution 2: " << ans << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);
  vector<string> input;
  string s;

  while (cin >> s) {
    input.push_back(s);
  }

  vector<vector<Node>> parsed_input = parse_input(input);
  solve_1(parsed_input);
  solve_2(parsed_input);

  return 0;
}
