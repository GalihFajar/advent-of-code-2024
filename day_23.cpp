#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>
using namespace std;

template <typename T> bool is_exist(T elem, set<T> s) {
  if (s.find(elem) != s.end()) {
    return true;
  }

  return false;
}

template <typename T> void print_set(set<T> s) {
  cout << '[';
  for (auto e : s) {
    cout << e << " ";
  }
  cout << ']';
}

pair<string, string> parse_in(string &s) {
  string temp = "";
  pair<string, string> out;
  for (auto c : s) {
    if (c == '-') {
      out.first = temp;
      temp = "";
      continue;
    }

    temp += c;
  }
  out.second = temp;

  return out;
}

string normalize(string &s) {
  string temp = "";
  vector<string> out_v;
  for (auto c : s) {
    if (c == ',') {
      out_v.push_back(temp);
      temp = "";
      continue;
    }

    temp += c;
  }
  out_v.push_back(temp);
  temp = "";
  sort(out_v.begin(), out_v.end());
  for (int i = 0; i < out_v.size(); i++) {
    if (i != out_v.size() - 1) {
      temp += out_v[i] + ',';
    } else {
      temp += out_v[i];
    }
  }

  return temp;
}

void solve_1(vector<string> &input) {
  unordered_map<string, set<string>> adj_map;
  set<string> triad_set;
  for (auto &in : input) {
    string n1, n2;
    tie(n1, n2) = parse_in(in);
    if (adj_map.count(n1)) {
      adj_map[n1].insert(n2);
    } else {
      adj_map[n1] = {n2};
    }

    if (adj_map.count(n2)) {
      adj_map[n2].insert(n1);
    } else {
      adj_map[n2] = {n1};
    }
  }

  vector<string> list;

  for (auto elem : adj_map) {
    set<string> &s = elem.second;
    string a = elem.first;
    for (auto it = s.begin(); it != s.end(); it++) {
      for (auto it2 = s.begin(); it2 != s.end(); it2++) {
        if (it == it2)
          continue;
        string b = *it;
        string c = *it2;

        if (is_exist(c, adj_map[b])) {
          string temp = a + "," + b + "," + c;
          triad_set.insert(normalize(temp));
        }
      }
    }
  }

  int ans = 0;

  for (auto e : triad_set) {
    bool t_avail = false;
    for (int i = 0; i < e.size(); i += 3) {
      if (e[i] == 't') {
        t_avail = true;
        break;
      }
    }

    if (t_avail)
      ans++;
  }

  cout << "Solution 1: " << ans << endl;
}

void bron_kerbosch(set<string> R, set<string> P, set<string> X,
                  const unordered_map<string, set<string>> &adjList,
                  vector<set<string>> &maximalCliques) {
  if (P.empty() && X.empty()) {
    maximalCliques.push_back(R);
    return;
  }

  set<string> P_copy = P;

  for (const string &v : P_copy) {
    set<string> R_new = R;
    R_new.insert(v);

    set<string> P_new, X_new;
    for (const string &u : P) {
      if (adjList.at(v).count(u)) {
        P_new.insert(u);
      }
    }
    for (const string &u : X) {
      if (adjList.at(v).count(u)) {
        X_new.insert(u);
      }
    }

    bron_kerbosch(R_new, P_new, X_new, adjList, maximalCliques);

    P.erase(v);
    X.insert(v);
  }
}

vector<set<string>> find_max_cliques(const unordered_map<string, set<string>> &adjList) {
  set<string> R, P, X;

  for (const auto &pair : adjList) {
    P.insert(pair.first);
  }

  vector<set<string>> maximalCliques;
  bron_kerbosch(R, P, X, adjList, maximalCliques);

  return maximalCliques;
}

void solve_2(vector<string> &input) {
  unordered_map<string, set<string>> adj_map;
  set<string> triad_set;
  for (auto &in : input) {
    string n1, n2;
    tie(n1, n2) = parse_in(in);
    if (adj_map.count(n1)) {
      adj_map[n1].insert(n2);
    } else {
      adj_map[n1] = {n2};
    }

    if (adj_map.count(n2)) {
      adj_map[n2].insert(n1);
    } else {
      adj_map[n2] = {n1};
    }
  }

  vector<set<string>> cliques = find_max_cliques(adj_map);
  sort(cliques.begin(), cliques.end(),
       [](set<string> &s1, set<string> &s2) -> bool {
         return s1.size() > s2.size();
       });
  string out = "";

  for (auto it = cliques[0].begin(); it != cliques[0].end(); it++) {
    if (next(it) == cliques[0].end()) {
      out += *it;
    } else {
      out += *it + ',';
    }
  }

  cout << "Solution 2: " << normalize(out) << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);
  string s;
  vector<string> input;
  while (cin >> s) {
    input.push_back(s);
  }

  solve_1(input);
  solve_2(input);

  return 0;
}
