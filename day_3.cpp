#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

pair<int, int> parse_mul(string& s, pair<int, int> start_end_idx) {
  int start_idx = start_end_idx.first;
  int end_idx = start_end_idx.second;

  if (start_idx >= s.size() || end_idx >= s.size()) return {-9999, -9999};

  string temp = s.substr(start_idx, 4);
  if (s[end_idx] != ')' || temp != "mul(") return {-9999, -9999};

  int idx = start_idx + 4;
  string first, second;
  while (idx < end_idx && s[idx] != ',') {
    first += s[idx];
    idx++;
  }
  idx++;

  while (idx < end_idx && s[idx] != ',') {
    second += s[idx];
    idx++;
  }

  if (first.empty() || second.empty()) return {-9999, -9999};

  return {stoi(first), stoi(second)};
}

bool is_do(string& s, int begin) {
  int end = begin + 3;
  if (end >= s.size()) return false;

  return s.substr(begin, 4) == "do()";
}

bool is_dont(string& s, int begin) {
  int end = begin + 6;
  if (end >= s.size()) return false;

  return s.substr(begin, 7) == "don't()";
}


int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);
  string s;

  int first_out = 0;
  int second_out = 0;
  bool do_op = true;
  while(getline(cin, s)) {
    for (int i = 0; i < s.size(); i++) {
      if (is_do(s, i)) {
        do_op = true;
      } else if (is_dont(s, i)) {
        do_op = false;
      }

      pair<int, int> t;
      int basic_end = i + 7;
      for (int j = 0; j <= 4; j++) {
        t = parse_mul(s, {i, basic_end + j});
        if (t.first != -9999 && t.second != -9999)
          break;
      }

      if (t.first != -9999 && t.second != -9999) {
        first_out += t.first * t.second;
        if (do_op) {
          second_out += t.first * t.second;
        }
        continue;
      }
    }
  }

  cout << first_out << '\n';
  cout << second_out << '\n';

  return 0;
}
