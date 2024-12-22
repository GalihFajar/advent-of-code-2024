#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;
typedef vector<string> vs;

vs parse_towels(string &s) {
  vs out;
  string temp;

  for (auto c : s) {
    if (c == ',') {
      out.push_back(temp);
      temp = "";
      continue;
    } else if (c == ' ') {
      continue;
    }

    temp += c;
  }
  out.push_back(temp);

  return out;
}

bool is_match(string &a, string &b) {
  int i = 0, j = 0;
  while (i < a.size() && j < b.size()) {
    if (a[i] != b[j])
      return false;
    i++;
    j++;
  }

  return true;
}

bool permute(vs towels, string &target, string current,
             unordered_map<string, bool> &dp) {
  if (dp.count(current)) {
    return dp[current];
  }

  if (current.size() > target.size() || !is_match(current, target)) {
    return false;
  }
  
  if (current == target) {
    return true;
  }
  bool ans = false;
  for (int i = 0; i < towels.size(); i++) {
    ans = ans || permute(towels, target, current + towels[i], dp);
    dp[current + towels[i]] = ans;

    if (ans)
      return ans;
  }

  return ans;
}

long long permute_ways(vs towels, string &target, string current,
                       unordered_map<string, long long> &dp) {
  if (dp.count(current)) {
    return dp[current];
  }

  if (current.size() > target.size() || !is_match(current, target)) {
    return 0;
  }

  if (current == target) {
    return 1;
  }

  long long valid_ways = 0;
  for (int i = 0; i < towels.size(); i++) {
    long long temp = permute_ways(towels, target, current + towels[i], dp);
    valid_ways += temp;
    dp[current + towels[i]] = temp;
  }

  return valid_ways;
}

void solve_1(vs towels, vs combs) {
  int cnt = 0;
  for (auto c : combs) {
    unordered_map<string, bool> map;
    if (permute(towels, c, "", map)) {
      cnt++;
    }
  }

  cout << "Solution 1: " << cnt << endl;
}

void solve_2(vs towels, vs combs) {
  long out = 0;
  for (auto c : combs) {
    unordered_map<string, long long> map;
    unordered_map<string, bool> visited;
    long ways = permute_ways(towels, c, "", map);
    /*cout << "c: " << c << " ways: " << ways;*/
    out += ways;
  }

  cout << "Solution 2: " << out << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);
  vs towels;
  vs combs;
  string s;
  getline(cin, s);
  cin.ignore(1000, '\n');
  towels = parse_towels(s);

  while (cin >> s) {
    combs.push_back(s);
  }

  solve_1(towels, combs);
  solve_2(towels, combs);

  return 0;
}
