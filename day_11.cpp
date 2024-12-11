#include <climits>
#include <ios>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

void print_vs(vector<string> &vs) {
  for (auto s : vs) {
    cout << s << " ";
  }
  cout << endl;
}

unordered_map<string, long long> dict;
unordered_map<string, long long> memo;

long long resolve_stone_neo(string elem, int times) {
  if (times == 0) {
    return 1;
  }

  string current_elem_depth = "";
  current_elem_depth += elem;
  current_elem_depth += "#";
  current_elem_depth += to_string(times);


  if (dict.count(current_elem_depth)) return dict[current_elem_depth];

  int elem_size = elem.size();

  if (elem == "0") {
    long long ans = resolve_stone_neo("1", times - 1);
    dict[current_elem_depth] = ans;
    return ans;
  } else if (elem_size % 2 == 0) {
    int i = 0;
    int end = elem_size - 1;
    int half = end / 2;
    string a = "";
    string b = "";

    while (i <= end) {

      if (i <= half) {
        a += elem[i];
      } else {
        b += elem[i];
      }

      i++;
    }

    if (b.size() != 1 && b[0] == '0') {
      string c = "";
      int start = [&]() -> int {
        int j = 0;
        while (j < b.size() && b[j] == '0') {
          j++;
        }

        return j;
      }();

      while (start < b.size()) {
        c += b[start++];
      }
      b = c;

      if (b.empty()) b = "0";
    }

    long long ans =
        resolve_stone_neo(a, times - 1) + resolve_stone_neo(b, times - 1);

    dict[current_elem_depth] = ans;
    return ans;
  } else {
    unsigned long long t = stoll(elem);
    t *= 2024;
    string temp = to_string(t);
    long long ans = resolve_stone_neo(temp, times - 1);
    dict[current_elem_depth] = ans;
    return ans;
  }

  return 1;
}


void solve(vector<string> in) {

  long long ans_1 = 0;
  for (int i = 0; i < in.size(); i++) {
    vector<string> temp = {in[i]};
    ans_1 += resolve_stone_neo(in[i], 25);
  }

  cout << "Solution 1: " << ans_1 << endl;

  long long ans_2 = 0;
  for (int i = 0; i < in.size(); i++) {
    vector<string> temp = {in[i]};
    ans_2 += resolve_stone_neo(in[i], 75);
  }

  cout << "Solution 2: " << ans_2 << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  vector<string> in;
  string s;

  while (cin >> s) {
    in.push_back(s);
  }

  /*solve_1(in);*/
  solve(in);

  return 0;
}
