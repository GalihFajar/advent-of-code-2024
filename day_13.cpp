#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;


long long calculate(pair<int, int> p1, pair<int, int> p2,
               pair<long long, long long> target) {
  long long Y = target.second;
  long long X = target.first;

  long long x1 = p1.first;
  long long y1 = p1.second;

  long long x2 = p2.first;
  long long y2 = p2.second;

  long long b = (Y * x1 - X * y1) / (x1 * y2 - x2 * y1);
  long long a = (X - b * x2) / x1;

  if ((x1 * a + x2 * b) == target.first && (y1 * a + y2 * b) == target.second) {
    return 3 * a + b;
  }

  return 0;
}

void solve_1() {
  auto parse = [](vector<string> a, int idx) -> int {
    string temp = a[idx];
    string x = "";
    for (auto c : temp) {
      if (c >= '0' && c <= '9') {
        x += c;
      }
    }

    return stoi(x);
  };

  int total = 0;

  while (1) {
    int x = 2;
    vector<string> A, B, XY;
    pair<int, int> A_pair, B_pair, XY_pair;
    string a, b, c, d, e, f, g;
    while (x--) {
      cin >> a;
      if (cin.eof()) {
        break;
      }
      cin >> b >> c >> d;
      if (x)
        A = {a, b, c, d};
      else
        B = {a, b, c, d};
    }
    if (a.empty() || b.empty() || c.empty()) {
      break;
    }
    cin >> a >> b >> c;
    XY = {a, b, c};

    A_pair = {parse(A, 2), parse(A, 3)};
    B_pair = {parse(B, 2), parse(B, 3)};
    XY_pair = {parse(XY, 1), parse(XY, 2)};

    total += calculate(A_pair, B_pair, XY_pair);
  }

  cout << "Solution 1: " << total << endl;
}

void solve_2() {
  auto parse = [](vector<string> a, int idx) -> int {
    string temp = a[idx];
    string x = "";
    for (auto c : temp) {
      if (c >= '0' && c <= '9') {
        x += c;
      }
    }

    return stoll(x);
  };

  int long long total = 0;
  int count = 0;

  while (1) {
    int x = 2;
    vector<string> A, B, XY;
    pair<int, int> A_pair, B_pair;
    pair<long long, long long> XY_pair;
    string a, b, c, d, e, f, g;
    while (x--) {
      cin >> a;
      if (cin.eof()) {
        break;
      }
      cin >> b >> c >> d;
      if (x)
        A = {a, b, c, d};
      else
        B = {a, b, c, d};
    }
    if (a.empty() || b.empty() || c.empty()) {
      break;
    }
    cin >> a >> b >> c;
    XY = {a, b, c};

    A_pair = {parse(A, 2), parse(A, 3)};
    B_pair = {parse(B, 2), parse(B, 3)};
    XY_pair = {parse(XY, 1) + 10000000000000, parse(XY, 2) + 10000000000000};

    total += calculate(A_pair, B_pair, XY_pair);
  }

  cout << "Solution 2: " << total << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  // run one of each at a time
  /*solve_1();*/
  solve_2();

  return 0;
}
