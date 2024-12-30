#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <set>
#include <sstream>
#include <unordered_map>
#include <vector>
using namespace std;

struct Input {
  string a;
  string op;
  string b;
  string c;

  bool operator<(const Input &other) const {
    return std::tie(a, op, b, c) <
           std::tie(other.a, other.op, other.b, other.c);
  }
};

template <typename K, typename T> void print_pair_v(vector<pair<K, T>> &v) {
  for (auto e : v) {
    /*if (e.first[0] == 'x' || e.first[0] == 'y')*/
    cout << e.first << ": " << e.second << endl;
  }
}

int do_op(int a, int b, string op) {
  if (op == "AND") {
    return a && b;
  } else if (op == "OR") {
    return a || b;
  }
  return a ^ b;
}

void solve_1(set<Input> inputs, unordered_map<string, int> memory) {
  auto print_input = [&]() {
    for (Input i : inputs) {
      cout << i.a << " " << i.op << " " << i.b << " --> " << i.c << endl;
      ;
    }
  };

  auto print_memory = [&]() {
    for (auto m : memory) {
      cout << m.first << ": " << m.second << endl;
    }
  };

  int init_size = inputs.size();

  set<string> erased;

  while (erased.size() != inputs.size()) {
    for (Input i : inputs) {
      if ((memory.count(i.a) && memory.count(i.b)) &&
          !erased.count(i.a + i.op + i.b + i.c)) {
        memory[i.c] = do_op(memory[i.a], memory[i.b], i.op);
        erased.insert(i.a + i.op + i.b + i.c);
      }
    }
  }

  vector<string> zs;
  for (auto m : memory) {
    if (m.first[0] == 'z') {
      zs.push_back(m.first);
    }
  }

  sort(zs.rbegin(), zs.rend());
  string out = "";
  for (auto z : zs) {
    out += to_string(memory[z]);
  }

  cout << "Solution 1: " << stoll(out, nullptr, 2) << endl;
}

void print_memory(unordered_map<string, int> &memory) {
  for (auto m : memory) {
    cout << m.first << ": " << m.second << endl;
  }
};

template <typename T> vector<T> to_vector(set<T> s) {
  vector<T> out;
  for (auto e : s) {
    out.push_back(e);
  }

  return out;
}

void solve_2(set<Input> inputs, unordered_map<string, int> memory) {
  vector<Input> inputs_v = to_vector(inputs);

  auto print_single_input = [](Input &i) {
    cout << i.a << " " << i.op << " " << i.b << " --> " << i.c << endl;
  };

  set<string> faulty_gates;
  for (auto i : inputs_v) {
    if (i.c[0] == 'z' && i.c[1] != '4' && i.c[2] != '5') {
      if (i.op != "XOR") {
        /*print_single_input(i);*/
        faulty_gates.insert(i.c);
      }
    }

    if (i.c[0] != 'z') {
      if (i.op == "XOR" &&
          !(i.a[0] == 'x' || i.a[0] == 'y' || i.b[0] == 'x' || i.b[0] == 'y')) {
        /*print_single_input(i);*/
        faulty_gates.insert(i.c);
      }
    }

    if ((i.a[0] == 'x' || i.a[0] == 'y') && (i.b[0] == 'x' || i.b[0] == 'y')) {
      if (!(i.a[1] == '0' && i.a[2] == '0') &&
          !(i.b[1] == '0' && i.b[2] == '0')) {
        if (i.op == "XOR") {
          bool used = false;
          for (auto j : inputs_v) {
            if ((i.c == j.a || i.c == j.b) && j.op == "XOR") {
              used = true;
            }
          }

          if (!used) {
            /*print_single_input(i);*/
            faulty_gates.insert(i.c);
          }
        }
      }

      if (!(i.a[1] == '0' && i.a[2] == '0') &&
          !(i.b[1] == '0' && i.b[2] == '0')) {
        if (i.op == "AND") {
          bool used = false;
          for (auto j : inputs_v) {
            if ((i.c == j.a || i.c == j.b) && j.op == "OR") {
              used = true;
            }
          }

          if (!used) {
            /*print_single_input(i);*/
            faulty_gates.insert(i.c);
          }
        }
      }
    }
  }

  vector<string> out = [&]() -> vector<string> {
    vector<string> out_;
    for (auto g: faulty_gates) {
      out_.push_back(g);
    }

    return out_;
  }();

  sort(out.begin(), out.end());
  cout << "Solution 2: ";
  for (int i = 0; i < out.size(); i++) {
    if (i == out.size() - 1) {
      cout << out[i] << endl;
    } else {
      cout << out[i] << ",";
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);
  string a, b, c, d, e;
  unordered_map<string, int> memory;

  set<Input> inputs;
  string line;
  while (getline(cin, line) && !line.empty()) {
    istringstream iss(line);
    if (iss >> a >> b) {
      memory[a.substr(0, a.size() - 1)] = std::stoi(b);
    }
  }

  while (std::getline(std::cin, line)) {
    istringstream iss(line);
    if (iss >> a >> b >> c >> d >> e) {
      inputs.insert({a, b, c, e});
    }
  }

  solve_1(inputs, memory);
  solve_2(inputs, memory);

  return 0;
}
