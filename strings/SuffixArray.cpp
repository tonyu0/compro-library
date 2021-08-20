#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

// https://atcoder.jp/contests/abc141/submissions/15840890

class SuffixArray {

public:
  SuffixArray() = default;
  SuffixArray(const std::string &_s)
    : n((int)_s.length()), s(_s), suf(n), rank(n), lcp(n) {
    // 大小関係が定義された要素の配列に対して使える
    std::iota(suf.begin(), suf.end(), 0);
    // 辞書順にする
    for (int i = 0; i < n; ++i) { rank[i] = (int)s[i]; }

    int powerOfTwo = 1;
    auto CompareUsingRank = [&](int a, int b) {
      // 既に大小関係が決まってたら
      if (rank[a] != rank[b]) { return rank[a] < rank[b]; }
      // a文字目からとb文字目からのsuffixを比較
      int l = a + powerOfTwo < n ? rank[a + powerOfTwo] : -1;
      int r = b + powerOfTwo < n ? rank[b + powerOfTwo] : -1;
      return l < r;
    };
    std::vector<int> tmp(n);
    for (; powerOfTwo < n; powerOfTwo <<= 1) {
      std::sort(suf.begin(), suf.end(), CompareUsingRank);
      tmp[suf[0]] = 0;
      // 2^(i-1)文字目までのsuffix arrayから、2^i文字目までのsaを作る。
      // ランクを導入してあるので、2^(i-1)文字先の2^(i-1)先までのランクが分かる。
      // 現時点のランクが同じ場合は、2^(i-1)文字先のランクを比較することで、2^i文字の比較を行ったことになる。
      for (int i = 1; i < n; ++i) {
        tmp[suf[i]] = tmp[suf[i - 1]] + CompareUsingRank(suf[i - 1], suf[i]);
      }

      tmp.swap(rank); // 一つ前だけあればいい
    }
    // lcpで使うので最終更新
    for (int i = 0; i < n; ++i) { rank[suf[i]] = i; }

    // LCP
    int now = 0;
    // 一つ下のランクのsuffix同士のマッチ数がそれほど変わるわけではないので、今伸ばしている長さを保持
    for (int i = 0; i < n; ++i) {
      int x = rank[i]; // i番目のsuffixの先頭位置
      if (x == n - 1) { continue; }
      int j = suf[x + 1]; // ランク1下の奴のsuf,
      if (0 < now) { now--; }
      for (; now + std::max(i, j) < n; now++) { // 伸ばす, nowは進み続けるので
        if (s[i + now] != s[j + now]) break;
      }

      lcp[x] = now;
    }
  }

  bool find(std::string &t) {
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

  std::vector<int> GetSuf() const { return suf; }
  std::vector<int> GetLcp() const { return lcp; }

private:
  int n;
  std::string s;
  std::vector<int> suf;  // rank -> pos idx
  std::vector<int> rank; // pos idx -> rank
  std::vector<int> lcp;  // pos -> lcp length
};