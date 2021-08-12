#include <iostream>
#include <random>
#include <set>
#include <vector>
using ll = int64_t;
using namespace std;
#define rep(i, j, n) for (int i = j; i < (n); ++i)
#define rrep(i, j, n) for (int i = (n)-1; j <= i; --i)

template <typename T>
std::ostream &operator<<(std::ostream &os, std::vector<T> &a) {
  os << "{";
  for (size_t i = 0; i < a.size(); ++i) os << (i > 0 ? "," : "") << a[i];
  return os << "}";
}

[[maybe_unused]] constexpr ll MOD = 1e9 + 7;
[[maybe_unused]] constexpr int INF = 0x3f3f3f3f;
[[maybe_unused]] constexpr ll INFL = 0x3f3f3f3f3f3f3f3fLL;

string solve1(int n, vector<int> &a) {
  vector<vector<int>> pos(n);
  rep(i, 0, n) {
    --a[i];
    pos[a[i]].push_back(i);
  }
  string ans(n, '0');
  set<int> unko;
  unko.insert(-1);
  unko.insert(n);
  rep(k, 0, n) {
    // n - k th
    if (pos[k].size() == 0) break;
    if (pos[k].size() == 1) {
      int c = pos[k][0];
      unko.insert(c);
      auto itr = unko.find(c);
      int prev = *--itr;
      int next = *++++itr;
      if (next - prev - 1 >= n - k) { ans[n - k - 1] = '1'; }
    } else {
      bool ok = false;
      for (int c : pos[k]) {
        unko.insert(c);
        auto itr = unko.find(c);
        int prev = *--itr;
        int next = *++++itr;
        ok |= next - prev - 1 >= n - k;
        unko.erase(c);
      }
      if (ok) ans[n - k - 1] = '1';
      break;
    }
  }
  return ans;
}

string solve2(int n, vector<int> &a) {
  vector<int> m(n + 1);
  rep(i, 0, n) m[i + 1] = min(m[i], a[i]);

  string ans(n, '0');
  rep(k, 0, n) {
    vector<int> tmp;
    rep(i, 0, n - k) {
      int mn = a[i];
      rep(j, i + 1, i + k + 1) mn = min(mn, a[j]);
      tmp.push_back(mn);
    }
    bool ok = true;
    sort(tmp.begin(), tmp.end());
    rep(i, 0, tmp.size()) { ok &= i == tmp[i]; }
    if (ok) ans[k] = '1';
  }
  return ans;
}

int main() {
  random_device seed_gen;
  mt19937_64 engine(seed_gen());
  rep(_, 0, 1000) {
    int n = engine() % 100 + 1;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) a[i] = engine() % n + 1;
    cout << "n: " << n << " a: " << a;
    cout << "checking input:" << endl;
    string ans1 = solve1(n, a);
    string ans2 = solve2(n, a);
    if (ans1 == ans2)
      cout << "OK!" << endl;
    else {
      cout << "NG!" << endl;
      cout << "SOLVE1: " << ans1 << endl;
      cout << "SOLVE2: " << ans2 << endl;
      break;
    }
  }
}