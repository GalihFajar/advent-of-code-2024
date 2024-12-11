#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;
typedef vector<string> vs;

void dfs(vector<string> &in, int i, int j, int &score, vector<vector<bool>>& visited, bool unique) {
  if (!visited[i][j] && in[i][j] == '9') {
    score++;
    if (unique)
      visited[i][j] = true;
    return;
  }

  if (i + 1 < in.size() && (in[i + 1][j] - in[i][j]) == 1) {
    dfs(in, i + 1, j, score, visited, unique);
  }

  if (i - 1 >= 0 && (in[i - 1][j] - in[i][j]) == 1) {
    dfs(in, i - 1, j, score, visited, unique);
  }

  if (j + 1 < in[i].size() && (in[i][j + 1] - in[i][j]) == 1) {
    dfs(in, i, j + 1, score, visited, unique);
  }

  if (j - 1 >= 0 && (in[i][j - 1] - in[i][j]) == 1) {
    dfs(in, i, j - 1, score, visited, unique);
  }

}

void solve_1(vs input) {
  int total_score = 0;

  for (int i = 0; i < input.size(); i++) {
    int score = 0;
    for (int j = 0; j < input[i].size(); j++) {
    vector<vector<bool>> visited(input.size(), vector<bool>(input[0].size(), false)); 
      if (input[i][j] == '0') {
        dfs(input, i, j, score, visited, true);
      }
    }
    total_score += score;
  }

  cout << "first solution: " << total_score << endl;
}

void solve_2(vs input) {
  int total_score = 0;

  for (int i = 0; i < input.size(); i++) {
    int score = 0;
    for (int j = 0; j < input[i].size(); j++) {
    vector<vector<bool>> visited(input.size(), vector<bool>(input[0].size(), false)); 
      if (input[i][j] == '0') {
        dfs(input, i, j, score, visited, false);
      }
    }
    total_score += score;
  }

  cout << "second solution: " << total_score << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);
  vector<string> input;
  string s;
  while (cin >> s) {
    input.push_back(s);
  }

  solve_1(input);
  solve_2(input);

  return 0;
}
