#include <vector>

// update, query O(logn)
// 0-indexed
template <typename T>
class fenwick_tree {
  size_t size, depth;
  std::vector<T> data;

public:
  fenwick_tree() = default;
  explicit fenwick_tree(size_t n) : size(n), data(n) {}
  T query(int i) {
    T result = 0;
    for (; i >= 0; i = (i & i + 1) - 1) result += data[i];
    return result;
  }
  void update(int i, T x) {
    for (; i < size; i = i | i + 1) data[i] += x;
  }
};

// update, query O(logn)
// 1-indexed
template <typename T>
class fenwick_tree_one {
  size_t size, depth;
  std::vector<T> data;

public:
  fenwick_tree_one() = default;
  explicit fenwick_tree_one(size_t n)
    : size(n), depth(int(log2(n)) + 1), data(n) {}

  T query(size_t i) {
    T result = 0;
    for (; i > 0; i -= i & -i) result += data[i];
    return result;
  }
  void update(size_t i, T x) {
    for (; i <= size; i += i & -i) data[i] += x;
  }
  std::pair<size_t, T> lower_bound(T x) {
    // 累積和がx以上になる最小のindexと、その直前までの累積和
    T sum = 0;
    size_t pos = 0;
    // 上の階層から、必要か不必要か判定して足し合わせる
    // posのビットを上から埋めていく
    for (int i = depth; i >= 0; --i) {
      size_t j = pos + (1 << i);
      if (j <= size && sum + data[j] < x) {
        // 右側を採用
        sum += data[j];
        pos += 1 << i;
      }
    }
    return std::make_pair(pos + 1, sum);
  }
};