// Suboptimal solution. Better approach is to calculate 
// for each length of input the list of possible operations.
// Also using DP might be a possibility (not sure)
#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

typedef unsigned long long ull;

template <typename T> void print_vect(vector<T> &v) {
  for (auto elem : v) {
    cout << elem << " ";
  }
  cout << '\n';
}

pair<unsigned long long, vector<ull>> parse_in(string &s) {
  string temp = "";
  vector<ull> v;
  int iter = 0;
  while (s[iter] != ':') {
    temp += s[iter];
    iter++;
  }
  iter++;
  iter++;

  unsigned long long l = stoll(temp);

  temp = "";
  while (iter < s.size()) {
    if (s[iter] == ' ') {
      v.push_back(stoi(temp));
      temp = "";
    } else {
      temp += s[iter];
    }

    iter++;
  }

  v.push_back(stoi(temp));

  return {l, v};
}

const string OP = "+*";
const string OP2 = "+*@";

void get_ops(vector<string> &ops, string curr_string, int size,
             const string OP) {
  if (curr_string.size() == size) {
    ops.push_back(curr_string);
    return;
  }

  for (int i = 0; i < OP.size(); i++) {
    get_ops(ops, curr_string + OP[i], size, OP);
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  string s;
  ull ans_first = 0;
  ull ans_second = 0;
  while (getline(cin, s)) {
    pair<ull, vector<ull>> in = parse_in(s);
    vector<ull> in_v = in.second;

    auto solve_first = [&]() {
      vector<string> ops;
      get_ops(ops, "", in_v.size() - 1, OP);

      for (string &s : ops) {
        vector<ull> temp_v = in_v;
        int current = 0;
        int current_ops_idx = 0;
        ull temp = temp_v[current];

        for (int i = 1; i < temp_v.size(); i++) {
          if (s[current_ops_idx] == '*') {
            temp_v[i] = temp_v[i - 1] * temp_v[i];
          } else {
            temp_v[i] = temp_v[i - 1] + temp_v[i];
          }

          current_ops_idx++;
        }

        if (temp_v[temp_v.size() - 1] == in.first) {
          ans_first += in.first;
          break;
        }
      }
    };

    auto solve_second = [&]() {
      vector<string> ops;
      get_ops(ops, "", in_v.size() - 1, OP2);

      for (string &s : ops) {
        vector<ull> temp_v = in_v;
        int current = 0;
        int current_ops_idx = 0;
        ull temp = temp_v[current];

        for (int i = 1; i < temp_v.size(); i++) {
          if (s[current_ops_idx] == '*') {
            temp_v[i] = temp_v[i - 1] * temp_v[i];
          } else if (s[current_ops_idx] == '@') {
            string temp;
            string a = to_string(temp_v[i - 1]);
            string b = to_string(temp_v[i]);
            temp += a;
            temp += b;

            temp_v[i] = stoll(temp);
          } else {
            temp_v[i] = temp_v[i - 1] + temp_v[i];
          }

          current_ops_idx++;
        }

        if (temp_v[temp_v.size() - 1] == in.first) {
          ans_second += in.first;
          break;
        }
      }
    };

    // pick one to run (will fix this)
    /*solve_first();*/
    solve_second();
  }

  cout << "first solution: \n" << ans_first << "\n";
  cout << "second solution: \n" << ans_second << "\n";

  return 0;
}
