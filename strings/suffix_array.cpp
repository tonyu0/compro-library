#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

// https://atcoder.jp/contests/abc141/submissions/15840890

class suffix_array {
  int n;
  std::string s;
  std::vector<int> suf;
  std::vector<int> rank;
  std::vector<int> lcp;

public:
  suffix_array() = default;
  suffix_array(const std::string& _s)
    : n(_s.length()), s(_s), suf(n), rank(n), lcp(n) {
    std::iota(suf.begin(), suf.end(), 0);
    for (int i = 0; i < n; ++i) rank[i] = s[i];
    int itr = 1;
    auto cmp = [&](int a, int b) {
      if (rank[a] != rank[b]) return rank[a] < rank[b];
      int l = a + itr < n ? rank[a + itr] : -1;
      int r = b + itr < n ? rank[b + itr] : -1;
      return l < r;
    };
    std::vector<int> buff(n);
    for (; itr < n; itr <<= 1) {
      std::sort(suf.begin(), suf.end(), cmp);
      buff[suf[0]] = 0;
      for (int i = 1; i < n; ++i)
        buff[suf[i]] = buff[suf[i - 1]] + cmp(suf[i - 1], suf[i]);

      buff.swap(rank);
    }
    for (int i = 0; i < n; ++i) rank[suf[i]] = i;

    int now = 0;
    for (int i = 0; i < n; ++i) {
      int x = rank[i];
      if (x == n - 1) continue;
      int y = x + 1;
      int j = suf[y];
      if (0 < now) now--;
      for (; now + std::max(i, j) < n; now++)
        if (s[i + now] != s[j + now]) break;

      lcp[x] = now;
    }
  }

  bool find(std::string& t) {
    int ng = -1, ok = (int)s.length() - 1;
    while (ok - ng > 1) {
      int mid = (ok + ng) / 2;
      if (s.substr(suf[mid], t.length()) < t)
        ng = mid;
      else
        ok = mid;
    }
    return s.substr(suf[ok], t.length()) == t;
  }

  std::vector<int> get_suf() const { return suf; }
  std::vector<int> get_lcp() const { return lcp; }
};