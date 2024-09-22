#include <vector>

// Binary Indexed Tree
// 配列への更新、配列の左端からの演算(アーベル群上の演算ならいける)
// Points:
// 各数字のLSBは区間の長さを表す。LSBはi&-iみたいに簡単に取れるので左端から　すべて埋められる
// 更新時は、被覆される要素すべて更新　たかだかlogn　単純に区間のサイズを足し引きして必要なところを更新している。
// 何をもって非福となすか
// 100 なら　010 001

// i & -i is i's LSB, and this leads to node's height
// ex1. 3: 0011 -3: 1101 (-3 = ~3+1)
// ex2. 4: 0100 -4: 1100

// TODO:
// 2本もって区間加算するやつ (あり本にもある)
// 2次元BIT

// update, query O(logn)
// 0-indexed
template <typename T, bool zero_indexed = true>
class fenwick_tree {
  size_t size, depth;
  std::vector<T> data;

public:
  fenwick_tree() = default;
  explicit fenwick_tree(size_t n) : size(n), data(n) {}
  T query(int i) {
    T result = 0;
    // i -= i&-i: i&-i is the LSB of i, so subtract it from i
    // i &= i-1: i-1 is a number where only LSB of i is 0, so take AND
    if constexpr (zero_indexed) {
      for (; i > 0; i &= i - 1) result += data[i - 1];
    } else {
      for (; i > 0; i -= i & -i) result += data[i];
    }
    return result;
  }
  // 3 4 8   0011 0100 1000 (1-indexed)
  // 2 3 7   0010 0011 0111 (0-indexed)
  // 0-indexed transition is filling 1s from the right end
  void update(int i, T x) {
    if constexpr (zero_indexed) {
      for (; i < size; i |= i + 1) data[i] += x;
    } else {
      for (; i <= size; i += i & -i) data[i] += x;
    }
  }

  std::pair<size_t, T> lower_bound(T x) {
    if (depth == 0) {
      while ((1 << (depth + 1)) <= size) ++depth;
    }
    // 累積和がx以上になる最小のindexと、その直前までの累積和
    T sum = 0;
    size_t pos = 0;
    // 上の階層から、必要か不必要か判定して足し合わせる
    // posのビットを上から埋めていく
    for (int i = depth; i >= 0; --i) {
      size_t j = pos + (1 << i);
      if (j <= size && sum + data[j - 1] < x) {
        // 右側を採用
        sum += data[j - 1];
        pos += 1 << i;
      }
    }
    return std::make_pair(pos + 1, sum);
  }
};