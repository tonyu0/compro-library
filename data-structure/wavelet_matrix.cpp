#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <vector>
// https: // judge.yosupo.jp/submission/4891

template <class Integral, ::std::size_t Bitlength, Integral None>
class dynamic_wavelet_matrix {
  static_assert(Bitlength > 0, "Bitlength must be greater than 0");

public:
  using value_type = Integral;
  using size_type = ::std::size_t;
  static constexpr value_type none = None;

private:
  class dynamic_fid {
    using u64 = ::std::uint_least64_t;
    using rank_type = ::std::uint_least8_t;
    using size_type = typename dynamic_wavelet_matrix::size_type;
    static ::std::size_t popcount(u64 c) {
#ifdef __has_builtin
      return __builtin_popcountll(c);
#else
      c = (c & 0x5555555555555555ULL) + (c >> 1 & 0x5555555555555555ULL);
      c = (c & 0x3333333333333333ULL) + (c >> 2 & 0x3333333333333333ULL);
      c = (c + (c >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
      return static_cast<::std::size_t>(c * 0x0101010101010101ULL >> 56 & 0x7f);
#endif
    }
    class node_type {
    public:
      using u64 = typename dynamic_fid::u64;
      using rank_type = typename dynamic_fid::rank_type;
      using size_type = typename dynamic_fid::size_type;
      size_type left, right, size;
      u64 value;
      rank_type rank;
      node_type(const size_type s, const u64 f)
        : left(0), right(0), size(s), value(f), rank(0) {}
      u64 sum() const { return rank ? value : dynamic_fid::popcount(value); }
    };

    static u64 mask(const size_type i) noexcept {
      return (static_cast<u64>(1) << i) - 1;
    }

    ::std::vector<node_type> c;
    size_type root;

    void fix(const size_type i) {
      c[i].size = c[c[i].left].size + c[c[i].right].size;
      c[i].value = c[c[i].left].sum() + c[c[i].right].sum();
      c[i].rank = ::std::max(c[c[i].left].rank, c[c[i].right].rank) + 1;
    }

  public:
    typename dynamic_wavelet_matrix::value_type bit;
    dynamic_fid() : c({{0, 0}}), root(0), bit(0) {}
    void adjust_l(size_type &ptr) {
      if (c[c[ptr].left].rank == c[c[ptr].right].rank + 2) {
        size_type temp;
        if (c[c[c[ptr].left].left].rank + 1 != c[c[c[ptr].left].right].rank) {
          temp = c[ptr].left;
        } else {
          temp = c[c[ptr].left].right;
          c[c[ptr].left].right = c[temp].left;
          fix(c[ptr].left);
          c[temp].left = c[ptr].left;
        }
        c[ptr].left = c[temp].right;
        fix(ptr);
        c[temp].right = ptr;
        ptr = temp;
      }
      fix(ptr);
    }
    void adjust_r(size_type &ptr) {
      if (c[c[ptr].right].rank == c[c[ptr].left].rank + 2) {
        size_type temp;
        if (c[c[c[ptr].right].right].rank + 1 != c[c[c[ptr].right].left].rank) {
          temp = c[ptr].right;
        } else {
          temp = c[c[ptr].right].left;
          c[c[ptr].right].left = c[temp].right;
          fix(c[ptr].right);
          c[temp].right = c[ptr].right;
        }
        c[ptr].right = c[temp].left;
        fix(ptr);
        c[temp].left = ptr;
        ptr = temp;
      }
      fix(ptr);
    }
    void split_node(size_type pos) {
      size_type p = root, old = ~static_cast<size_type>(0);
      while (c[p].rank) {
        if (c[c[p].left].size > pos) {
          ::std::swap(old, c[p].left);
        } else {
          pos -= c[c[p].left].size;
          ::std::swap(old, c[p].right);
        }
        ::std::swap(p, old);
      }
      const u64 bits = c[p].value;
      c[p].left = c.size();
      c.emplace_back(32, bits & mask(32));
      c[p].right = c.size();
      c.emplace_back(32, bits >> 32);
      c[p].value = popcount(bits);
      c[p].rank = 1;
      while (::std::swap(p, old), ~p) {
        if (!~c[p].left || c[c[p].left].rank > c[p].rank) {
          ::std::swap(c[p].left, old);
          adjust_l(p);
        } else {
          ::std::swap(c[p].right, old);
          adjust_r(p);
        }
      }
      root = old;
    }
    size_type insert(const size_type pos, const bool bit) {
      assert(pos <= size());
      size_type pos_ = pos, p = root, cnt = 0;
      while (++c[p].size, c[p].rank) {
        c[p].value += bit;
        if (c[c[p].left].size > pos_) {
          p = c[p].left;
        } else {
          pos_ -= c[c[p].left].size;
          cnt += c[c[p].left].sum();
          p = c[p].right;
        }
      }
      cnt += popcount(c[p].value & mask(pos_));
      c[p].value = c[p].value & mask(pos_) | (c[p].value >> pos_ << 1 | bit)
                                               << pos_;
      if (c[p].size == 64) split_node(pos);
      return cnt;
    }
    ::std::pair<bool, size_type> erase(size_type pos) {
      assert(pos < size());
      size_type p = root, cnt = 0;
      const bool bit = operator[](pos);
      while (--c[p].size, c[p].rank) {
        c[p].value -= bit;
        if (c[c[p].left].size > pos) {
          p = c[p].left;
        } else {
          pos -= c[c[p].left].size;
          cnt += c[c[p].left].sum();
          p = c[p].right;
        }
      }
      cnt += popcount(c[p].value & mask(pos));
      c[p].value = c[p].value & mask(pos) | c[p].value >> pos + 1 << pos;
      return {bit, cnt};
    }
    size_type rank(size_type pos) const {
      assert(pos <= size());
      size_type p = root, cnt = 0;
      while (c[p].rank) {
        if (c[c[p].left].size > pos) {
          p = c[p].left;
        } else {
          pos -= c[c[p].left].size;
          cnt += c[c[p].left].sum();
          p = c[p].right;
        }
      }
      return cnt + popcount(c[p].value & mask(pos));
    }
    size_type cnt0() const { return c[root].size - c[root].sum(); }
    size_type size() const { return c[root].size; }
    bool operator[](size_type pos) const {
      assert(pos < size());
      size_type p = root;
      while (c[p].rank) {
        if (c[c[p].left].size > pos) {
          p = c[p].left;
        } else {
          pos -= c[c[p].left].size;
          p = c[p].right;
        }
      }
      return c[p].value >> pos & 1;
    }
  };
  ::std::array<dynamic_fid, Bitlength> matrix;

  size_type at_least(size_type first, size_type last,
                     const value_type value) const {
    size_type ret = 0;
    for (const auto &v : matrix) {
      const size_type l = v.rank(first), r = v.rank(last);
      if (value & v.bit) {
        first = l + v.cnt0();
        last = r + v.cnt0();
      } else {
        ret += r - l;
        first -= l;
        last -= r;
      }
    }
    return ret + last - first;
  }

public:
  dynamic_wavelet_matrix() : matrix() {
    value_type temp = static_cast<value_type>(1) << Bitlength - 1;
    for (auto &v : matrix) v.bit = temp, temp >>= 1;
  }

  size_type size() const { return matrix[0].size(); }
  bool empty() const { return size() == 0; }

  value_type quantile(size_type first, size_type last, size_type k = 0,
                      const value_type upper = none) const {
    if (upper != none) k += rangefreq(first, last, upper, none);
    if (last - first <= k) return none;
    value_type ret = 0;
    for (const auto &v : matrix) {
      const size_type l = v.rank(first), r = v.rank(last);
      if (r - l > k)
        first = l + v.cnt0(), last = r + v.cnt0(), ret |= v.bit;
      else
        first -= l, last -= r, k -= r - l;
    }
    return ret;
  }
  value_type rquantile(const size_type first, const size_type last,
                       size_type k = 0, const value_type lower = none) const {
    if (lower != none) k += rangefreq(first, last, none, lower);
    if (last - first <= k) return none;
    return quantile(first, last, last - first - k - 1, none);
  }
  size_type rangefreq(const size_type first, const size_type last,
                      const value_type lower, const value_type upper) const {
    size_type ret = lower == none ? last - first : at_least(first, last, lower);
    return upper == none ? ret : ret - at_least(first, last, upper);
  }
  void insert(size_type position, const value_type value) {
    for (auto &v : matrix) {
      if (value & v.bit)
        position = v.cnt0() + v.insert(position, 1);
      else
        position -= v.insert(position, 0);
    }
  }
  value_type erase(size_type position) {
    value_type ret = 0;
    for (auto &v : matrix) {
      const auto temp = v.erase(position);
      if (temp.first)
        position = v.cnt0() + temp.second, ret |= v.bit;
      else
        position -= temp.second;
    }
    return ret;
  }
  value_type replace(const size_type index, const value_type value) {
    const value_type ret = erase(index);
    insert(index, value);
    return ret;
  }
};

#include <iostream>

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n, q;
  std::cin >> n >> q;
  dynamic_wavelet_matrix<unsigned, 30, 0> dwm;
  for (int i = 0; i != n; i += 1) {
    int a;
    std::cin >> a;
    dwm.insert(i, a);
  }
  for (int i = 0; i != q; i += 1) {
    int l, r, k;
    std::cin >> l >> r >> k;
    std::cout << dwm.rquantile(l, r, k) << "\n";
  }
}