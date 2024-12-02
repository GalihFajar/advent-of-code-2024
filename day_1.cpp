#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);
  int a, b;
  vector<int> l1, l2;
  unordered_map<int, int> total_appear;

  while (cin >> a) {
    cin >> b;

    l1.push_back(a);
    l2.push_back(b);
    total_appear[b]++;
  }

  sort(l1.begin(), l1.end());
  sort(l2.begin(), l2.end());
  int ans_p1 = 0;
  int ans_p2 = 0;

  for (int i = 0; i < l1.size(); i++) {
    ans_p1 += abs(l1[i] - l2[i]);
    ans_p2 += l1[i] * total_appear[l1[i]];
  }

  cout << ans_p1 << '\n';
  cout << ans_p2 << '\n';

  return 0;
}
