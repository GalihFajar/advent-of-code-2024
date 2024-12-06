#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

typedef unordered_set<int> usi;

typedef struct Page {
  int number;
  usi left;
  usi right;
} Page;

template <typename T> bool is_exist(unordered_set<T> &u, T elem) {
  if (u.find(elem) == u.end()) {
    return false;
  }

  return true;
}

pair<int, int> parse(string &s) {
  pair<int, int> out;
  string temp;
  for (auto c : s) {
    if (c == '|') {
      out.first = stoi(temp);
      temp = "";
      continue;
    }

    temp += c;
  }

  out.second = stoi(temp);

  return out;
}

vector<int> parse_to_v(string &s) {
  vector<int> out;
  string temp = "";
  for (char c : s) {
    if (c == ',') {
      out.push_back(stoi(temp));
      temp.clear();
      continue;
    }

    temp += c;
  }

  out.push_back(stoi(temp));

  return out;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);
  unordered_map<int, Page> page_dict;

  string s;
  while (getline(cin, s)) {
    if (s.empty())
      break;
    pair<int, int> pii = parse(s);
    int a = pii.first, b = pii.second;

    if (!page_dict.count(a)) {
      page_dict[a] = Page{a, usi(), usi()};
    }

    if (!page_dict.count(b)) {
      page_dict[b] = Page{b, usi(), usi()};
    }

    Page &page_a = page_dict[a];
    Page &page_b = page_dict[b];

    page_a.right.insert(b);
    page_b.left.insert(a);
  }

  int ans = 0;
  int second_ans = 0;
  while (getline(cin, s)) {
    vector<int> v = parse_to_v(s);
    auto is_valid = [&]() -> bool {
      for (int k = 0; k < v.size(); k++) {
        Page p = page_dict[v[k]];
        int l = 0;
        while (l < k) {
          if (is_exist(p.right, v[l]))
            return false;
          l++;
        }

        int r = v.size() - 1;
        while (k < r) {
          if (is_exist(p.left, v[r]))
            return false;
          r--;
        }
      }

      return true;
    };

    if (is_valid()) {
      ans += v[((int)v.size() - 1) / 2];
    } else {
      // assume that the input always is fixable
      vector<Page> new_pages;
      for (int k = 0; k < v.size(); k++) {
        Page new_page = Page{v[k], usi(), usi()};
        for (int i = 0; i < v.size(); i++) {
          if (i == k)
            continue;
          int elem = v[i];
          if (is_exist(page_dict[v[k]].left, elem)) {
            new_page.left.insert(elem);
          }
          if (is_exist(page_dict[v[k]].right, elem)) {

            new_page.right.insert(elem);
          }
        }

        new_pages.push_back(new_page);
      }

      sort(new_pages.begin(), new_pages.end(), [](Page &p1, Page &p2) -> bool {
        return p1.left.size() < p2.left.size();
      });
      second_ans += new_pages[(new_pages.size() - 1) / 2].number;
    }
  }

  cout << "first ans: " << ans << '\n';
  cout << "second ans: " << second_ans << '\n';

  return 0;
}
