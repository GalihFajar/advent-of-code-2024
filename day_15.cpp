#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

pair<int, int> next_coord(pair<int, int> &curr, char c) {
  if (c == '<') {
    return {curr.first, curr.second - 1};
  }

  if (c == '^') {
    return {curr.first - 1, curr.second};
  }

  if (c == '>') {
    return {curr.first, curr.second + 1};
  }

  if (c == 'v') {
    return {curr.first + 1, curr.second};
  }

  return {-1, -1};
};

bool push(pair<int, int> curr_pos, char comm, vector<string> &coord) {
  int i = curr_pos.first, j = curr_pos.second;
  if (coord[i][j] == '.') {
    coord[i][j] = 'O';
    return true;
  }

  if (coord[i][j] == '#') {
    return false;
  }

  pair<int, int> next_pos = next_coord(curr_pos, comm);
  coord[i][j] = 'O';

  return push(next_pos, comm, coord);
}

void print_vs(vector<string> &vs) {
  for (auto s : vs) {
    cout << s << endl;
  }
}

void solve_1(vector<string> coord, vector<string> comms) {
  pair<int, int> sub_pos;

  for (int i = 0; i < coord.size(); i++) {
    for (int j = 0; j < coord[i].size(); j++) {
      if (coord[i][j] == '@') {
        sub_pos = {i, j};
        break;
      }
    }
  }

  for (int i = 0; i < comms.size(); i++) {
    for (int j = 0; j < comms[i].size(); j++) {
      pair<int, int> next = next_coord(sub_pos, comms[i][j]);
      int i_curr, j_curr, i_next, j_next;
      tie(i_curr, j_curr) = sub_pos;
      tie(i_next, j_next) = next;

      if (coord[i_next][j_next] == '.') {
        coord[i_curr][j_curr] = '.';
        coord[i_next][j_next] = '@';
        sub_pos = next;
      } else if (coord[i_next][j_next] == 'O') {
        if (push(next, comms[i][j], coord)) {
          coord[next.first][next.second] = '@';
          coord[i_curr][j_curr] = '.';
          sub_pos = next;
        }
      }
    }
  }
  int ans = 0;

  for (int i = 0; i < coord.size(); i++) {
    for (int j = 0; j < coord[i].size(); j++) {
      if (coord[i][j] == 'O') {
        ans += i * 100 + j;
      }
    }
  }


  cout << "Solution 1: " << ans << endl;
}

string idx_key(int i, int j, char c) {
  string s = "";
  s += to_string(i);
  s += "#";
  s += to_string(j);
  s += c;

  return s;
}

pair<int, int> parse_idx_key(string k) {
  string temp = "";
  int a, b;
  int *ptr = &a;
  for (auto c : k) {
    if (c == ']' || c == '[')
      break;
    if (c == '#') {
      a = stoi(temp);
      temp = "";
      continue;
    }
    temp += c;
  }
  b = stoi(temp);

  return {a, b};
}

char parse_idx_key_bracket(string k) {
  for (auto c : k) {
    if (c == ']' || c == '[')
      return c;
  }

  return '!';
}

bool push_2_h(pair<int, int> curr_pos, char comm, vector<string> &coord,
              unordered_map<string, pair<int, int>> &able_to_push) {
  int i = curr_pos.first, j = curr_pos.second;
  bool can_push = true;
  pair<int, int> next = next_coord(curr_pos, comm);

  if (coord[i][j] == '.') {
    return true;
  } else if (coord[i][j] == '#') {
    return false;
  }

  can_push = can_push && push_2_h(next, comm, coord, able_to_push);

  if (can_push) {
    able_to_push[idx_key(i, j, coord[i][j])] = next;
  }
  return can_push;
}

bool push_2_v(pair<int, int> curr_pos, char comm, vector<string> &coord,
              unordered_map<string, pair<int, int>> &able_to_push, unordered_map<string, int>& visited) {
  int i = curr_pos.first, j = curr_pos.second;
  if (visited[idx_key(i, j, '|')]) return true;
  visited[idx_key(i, j, '|')] = true;
  bool can_push = true;
  pair<int, int> next = next_coord(curr_pos, comm);

  if (coord[i][j] == '.') {
    return true;
  } else if (coord[i][j] == '#') {
    return false;
  }

  // recurse to side as well
  if (coord[i][j] == '[') {
    can_push = can_push && push_2_v({i, j + 1}, comm, coord, able_to_push, visited);
  } else if (coord[i][j] == ']') {
    can_push = can_push && push_2_v({i, j - 1}, comm, coord, able_to_push, visited);
  }

  int i_next = next.first, j_next = next.second;
  can_push = can_push && push_2_v(next, comm, coord, able_to_push, visited);

  if (can_push)
    able_to_push[idx_key(i, j, coord[i][j])] = next;
  return can_push;
}

void solve_2(vector<string> coord, vector<string> comms) {
  pair<int, int> sub_pos;
  vector<string> temp = coord;
  coord = vector<string>(coord.size());

  for (int i = 0; i < temp.size(); i++) {
    for (int j = 0; j < temp[i].size(); j++) {
      if (temp[i][j] == '#') {
        coord[i] += "##";
      } 
      if (temp[i][j] == 'O') {
        coord[i] += "[]";
      } 
      if (temp[i][j] == '.') {
        coord[i] += "..";
      } 
      if (temp[i][j] == '@') {
        coord[i] += "@.";
      } 
    }
  }

  for (int i = 0; i < coord.size(); i++) {
    for (int j = 0; j < coord[i].size(); j++) {
      if (coord[i][j] == '@') {
        sub_pos = {i, j};
        break;
      }
    }
  }

  for (int i = 0; i < comms.size(); i++) {
    for (int j = 0; j < comms[i].size(); j++) {
      pair<int, int> next = next_coord(sub_pos, comms[i][j]);
      int i_curr, j_curr, i_next, j_next;
      tie(i_curr, j_curr) = sub_pos;
      tie(i_next, j_next) = next;

      if (coord[i_next][j_next] == '.') {
        coord[i_curr][j_curr] = '.';
        coord[i_next][j_next] = '@';
        sub_pos = next;
      } else if (coord[i_next][j_next] == '[' || coord[i_next][j_next] == ']') {
        unordered_map<string, pair<int, int>> able_to_push;
        unordered_map<string, int> visited;

        if (comms[i][j] == '^' || comms[i][j] == 'v') {
          if (push_2_v(next, comms[i][j], coord, able_to_push, visited)) {
            unordered_map<string, bool> processed;
            for (auto x : able_to_push) {
              // NOTE: push based on the map
              pair<int, int> before = parse_idx_key(x.first);
              char c = parse_idx_key_bracket(x.first);
              pair<int, int> after = x.second;

              coord[after.first][after.second] = c;
              processed[idx_key(after.first, after.second, '|')] = true;
              if (!processed[idx_key(before.first, before.second, '|')])
                coord[before.first][before.second] = '.';
            }

            coord[i_next][j_next] = '@';
            coord[i_curr][j_curr] = '.';
            sub_pos = next;
          }
        } else if (comms[i][j] == '<' || comms[i][j] == '>') {
          if (push_2_h(next, comms[i][j], coord, able_to_push)) {
            unordered_map<string, bool> processed;
            for (auto x : able_to_push) {
              // NOTE: push based on the map
              pair<int, int> before = parse_idx_key(x.first);
              char c = parse_idx_key_bracket(x.first);
              pair<int, int> after = x.second;

              coord[after.first][after.second] = c;
              processed[idx_key(after.first, after.second, '|')] = true;
              if (!processed[idx_key(before.first, before.second, '|')])
                coord[before.first][before.second] = '.';
            }

            coord[i_next][j_next] = '@';
            coord[i_curr][j_curr] = '.';
            sub_pos = next;
          }
        }
      }

      // NOTE: To debug
      /*cout << "command: " << comms[i][j] << endl;*/
      /*print_vs(coord);*/
      /*printf("done: [%d, %d]\n", i, j);*/
      /*cout << endl;*/
    }
  }
  int ans = 0;

  for (int i = 0; i < coord.size(); i++) {
    for (int j = 0; j < coord[i].size(); j++) {
      if (coord[i][j] == '[') {
        ans += i * 100 + j;
      }
    }
  }

  cout << endl;

  cout << "Solution 2: " << ans << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  vector<string> coordinates;
  vector<string> commands;

  string s;
  while (cin >> s) {
    if (s[0] == '#')
      coordinates.push_back(s);
    else
      commands.push_back(s);
  }

  solve_1(coordinates, commands);
  solve_2(coordinates, commands);

  return 0;
}
