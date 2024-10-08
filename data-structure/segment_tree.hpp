#include <vector>

template <typename T, T (*operation)(T, T), T (*identity)()>
class segment_tree {
public:
  segment_tree(size_t n) {
    for (size = 1; size < n; size <<= 1) {}
    data.assign(size * 2, identity());
  }

  // make segtree from vector
  segment_tree(size_t n, const std::vector<T> &a) {
    for (size = 1; size < n; size <<= 1) {}
    data.assign(size * 2, identity());
    for (size_t i = 0; i < n; ++i) { data[i + size] = a[i]; }
    for (int i = size - 1; i > 0; --i) {
      data[i] = operation(data[i * 2], data[i * 2 + 1]);
    }
  }

  void update(size_t i, T value, bool override = false) {
    i += size;
    data[i] = override ? value : operation(data[i], value);

    for (; i >>= 1;) { data[i] = operation(data[i * 2], data[i * 2 + 1]); }
  }
  // [a,b)
  T query(size_t l, size_t r) {
    T resL = identity();
    T resR = identity();
    for (l += size, r += size; l < r; l >>= 1, r >>= 1) {
      if (l & 1) resL = operation(resL, data[l++]);
      if (r & 1) resR = operation(data[--r], resR);
    }
    return operation(resL, resR);
  }
  // TODO: セグ木上二分探索
  // max_right:
  // 左端のindex(=l)とTを返すbool関数(=f)を受け取り、f(l..r)を満たす最大のrを返却
  // min_left: 右端rと関数fを受け取り、f(l..r)を満たす最小のlを返却

private:
  size_t size;         // num of leaves
  std::vector<T> data; // root index is 1
};