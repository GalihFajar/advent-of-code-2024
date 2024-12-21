#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

typedef long long ll;

vector<int> parse_input(string &in) {
  string s = "";
  vector<int> out;
  for (auto c : in) {
    if (c == ',') {
      out.push_back(stoll(s));
      s = "";
      continue;
    }
    s += c;
  }
  out.push_back(stoi(s));

  return out;
}

void print_out(vector<int> &output) {
  for (int i = 0; i < output.size(); i++) {
    if (i == 0) {
      cout << output[i];
    } else {
      cout << "," << output[i];
    }
  }
}

void print_dec_oct(ll x) {
  cout << "dec: " << dec << x;
  cout << " oct: " << oct << x;
}

void solve_1(long long reg_a, long long reg_b, long long reg_c,
             vector<int> input) {

  vector<int> output;
  int ptr = 0;
  auto resolve_operand = [&](long long operand) -> long long {
    if (operand == 4)
      return reg_a;
    if (operand == 5)
      return reg_b;
    if (operand == 6)
      return reg_c;

    return operand;
  };
  // duplicate (too lazy to refactor)
  auto process = [&]() {
    int instruction = input[ptr];
    switch (instruction) {
    case 0:
      reg_a = reg_a / pow(2, resolve_operand(input[ptr + 1]));
      ptr += 2;
      break;
    case 1:
      reg_b = reg_b ^ input[ptr + 1];
      ptr += 2;
      break;
    case 2:
      reg_b = resolve_operand(input[ptr + 1]) % 8;
      ptr += 2;
      break;
    case 3:
      if (reg_a == 0) {
        ptr += 2;
      } else {
        ptr = input[ptr + 1];
      }
      break;
    case 4:
      reg_b = reg_b ^ reg_c;
      ptr += 2;
      break;
    case 5:
      output.push_back(resolve_operand(input[ptr + 1]) % 8);
      ptr += 2;
      break;
    case 6:
      reg_b = reg_a / pow(2, resolve_operand(input[ptr + 1]));
      ptr += 2;
      break;
    case 7:
      reg_c = reg_a / pow(2, resolve_operand(input[ptr + 1]));
      ptr += 2;
      break;
    }
  };

  while (ptr < input.size()) {
    process();
  }

  cout << "Solution 1: ";
  print_out(output);
  cout << endl;
}

bool same(vector<int> &a, vector<int> &b) {
  if (a.size() != b.size())
    return false;

  for (int i = 0; i < a.size(); i++) {
    if (a[i] != b[i])
      return false;
  }

  return true;
}

void proc(ll &reg_a, ll &reg_b, ll &reg_c, vector<int> &input,
          vector<int> &output, int &ptr) {
  auto resolve_operand = [&](long long operand) -> long long {
    if (operand == 4)
      return reg_a;
    if (operand == 5)
      return reg_b;
    if (operand == 6)
      return reg_c;

    return operand;
  };

  int instruction = input[ptr];
  switch (instruction) {
  case 0:
    reg_a = reg_a / pow(2, resolve_operand(input[ptr + 1]));
    ptr += 2;
    break;
  case 1:
    reg_b = reg_b ^ input[ptr + 1];
    ptr += 2;
    break;
  case 2:
    reg_b = resolve_operand(input[ptr + 1]) % 8;
    ptr += 2;
    break;
  case 3:
    if (reg_a == 0) {
      ptr += 2;
    } else {
      ptr = input[ptr + 1];
    }
    break;
  case 4:
    reg_b = reg_b ^ reg_c;
    ptr += 2;
    break;
  case 5:
    output.push_back(resolve_operand(input[ptr + 1]) % 8);
    ptr += 2;
    break;
  case 6:
    reg_b = reg_a / pow(2, resolve_operand(input[ptr + 1]));
    ptr += 2;
    break;
  case 7:
    reg_c = reg_a / pow(2, resolve_operand(input[ptr + 1]));
    ptr += 2;
    break;
  }
}

template <typename T> bool compare(vector<T> &a, vector<T> &b) {
  int i = a.size() - 1, j = b.size() - 1;
  while (i >= 0 && j >= 0) {
    if (a[i] != b[j])
      return false;
    i--; j--;
  }

  return true;
}

void backtrack(vector<int> &input, ll current, vector<ll>& answers) {
  for (int i = 0; i < 8; i++) {
    ll reg_a = (current << 3) + i, b = 0, c = 0;
    int ptr = 0;
    vector<int> output;
    while (ptr < input.size()) {
      proc(reg_a, b, c, input, output, ptr);
    }

    if (compare(input, output)) {
      if (output.size() == 16) {
        answers.push_back((current << 3) + i);
        return;
      }
      backtrack(input, (current << 3) + i, answers);
    }
  }
}

void solve_2(long long reg_a, long long reg_b, long long reg_c,
             vector<int> input) {



  // for hypotheses
  auto hyp = [&]() {
    for (ll i = 0; i < 128; i++) {
      vector<int> output;
      int ptr = 0;
      ll a = i, b = 0, c = 0;
      print_dec_oct(a);
      cout << " ";
      while (ptr < input.size()) {
        proc(a, b, c, input, output, ptr);
      }
      print_out(output);
      cout << endl;
    }

    cout << "----\n";

    for (ll i = 0; i < 8; i++) {
      vector<int> output;
      int ptr = 0;
      ll a = (6 << 3) + i, b = 0, c = 0; // register A is always shifted 3 bits to right
                                         // hence, we can spot the next output based on every 3 bits
                                         // Let's say number 6 is equals 0 (the last bit), if we want to 
                                         // preserve it, then it must be on the first 3 msbs, hence 
                                         // we shift it 3 bytes to the left to preserve it.
                                         //
                                         // Do it until the desired program input size
      print_dec_oct(a);
      cout << " ";
      while (ptr < input.size()) {
        proc(a, b, c, input, output, ptr);
      }
      print_out(output);
      cout << endl;
    }
  };
  /*hyp();*/
  vector<ll> answers;
  backtrack(input, 0, answers);
  sort(answers.begin(), answers.end());

  cout << "Solution 2: " << answers[0] << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);
  long long reg_a, reg_b, reg_c;
  long long a, b, c;
  string temp, input;

  cin >> temp >> temp >> reg_a;
  cin >> temp >> temp >> reg_b;
  cin >> temp >> temp >> reg_c;
  cin >> temp >> input;

  solve_1(reg_a, reg_b, reg_c, parse_input(input));
  solve_2(reg_a, reg_b, reg_c, parse_input(input));

  return 0;
}
