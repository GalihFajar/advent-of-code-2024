#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

ll mod(long long a, long long b) { return ((a % b) + b) % b; }

ll mix(ll a, ll b) { return a ^ b; }

ll prune(ll a) { return mod(a, 16777216); }

ll calculate(long long secret_number) {
  ll out = prune(mix(secret_number * 64, secret_number));
  out = prune(mix((out / 32), out));
  out = prune(mix(out * 2048, out));

  return out;
}

ll calculate_n(ll secret_number, int n) {
  int cnt = 0;

  while (cnt < n) {
    secret_number = calculate(secret_number);
    cnt++;
  }

  return secret_number;
}

vector<pii> calculate_n_alt(ll secret_number, int n) {
  int cnt = 0;
  ll prev = secret_number;
  vector<pair<int, int>> changes;

  while (cnt < n) {
    ll temp = calculate(secret_number);
    changes.push_back({temp % 10, temp % 10 - prev % 10});
    secret_number = temp;
    prev = temp;
    cnt++;
  }

  return changes;
}

void insert_map(string key, int value, int idx,
                unordered_map<string, vi> &map) {
  if (map.count(key)) {
    if (map[key][idx] == -10) {
      map[key][idx] = value;
    }
  } else {
    map[key] = vi(1700, -10);
    map[key][idx] = value;
  }
}

void normalize_map(unordered_map<string, vi> &map) {
  for (auto &e : map) {
    for (int i = 0; i < e.second.size(); i++) {
      if (e.second[i] == -10) {
        e.second[i] = 0;
      }
    }
  }
}

string get_key(vector<int> &v) {
  string key = "";
  for (int i = 0; i < v.size(); i++) {
    key += to_string(v[i]);
    if (i != v.size() - 1) {
      key += ',';
    }
  }

  return key;
}

void print_v(vector<int> &v) {
  cout << '[';
  for (auto i : v) {
    cout << i << " ";
  }
  cout << ']';
}

void solve_1(vector<ll> &input) {
  ll ans = 0;
  for (int i = 0; i < input.size(); i++) {
    ans += calculate(input[i]);
    
  }
  cout << "Solution 1: " << ans << endl;
}

void solve_2(vector<ll> &input) {
  unordered_map<string, vector<int>> sequence_map;
  for (int idx = 0; idx < input.size(); idx++) {
    vector<pii> changes = calculate_n_alt(input[idx], 2000);

    for (int i = 0; i < changes.size() - 3; i++) {
      vector<int> temp;
      for (int j = 0; j <= 3; j++) {
        temp.push_back(changes[i + j].second);
      }
      int value = changes[i + 3].first;
      insert_map(get_key(temp), value, idx, sequence_map);
    }
  }

  normalize_map(sequence_map);

  int best = INT_MIN;
  string ans = "";
  for (auto kv : sequence_map) {
    string k;
    vi v;
    tie(k, v) = kv;
    int sum = 0;
    for (auto e : v) {
      sum += e;
    }

    if (sum > best) {
      best = sum;
      ans = k;
    }
  }

  cout << "Solution 2: " << best << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);
  ll in;
  ll ans = 0;
  vector<ll> input;
  while (cin >> in) {
    input.push_back(in);
  }

  solve_1(input);
  solve_2(input);

  return 0;
}
