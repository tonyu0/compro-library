#include <vector>

// Binary Indexed Tree
// https://ioinformatics.org/journal/v9_2015_39_44.pdf
// update(i, x): update i-th element to x
// query(i): returns a result of applying operations to [1, i] (1-indexed)
// O(logn) each

// Points:
// 各数字のLSBは区間の長さを表す。LSBはi&-iみたいに簡単に取れるので左端から　すべて埋められる
// 更新時は、被覆される要素すべて更新　たかだかlogn　単純に区間のサイズを足し引きして必要なところを更新している。
// -i = ~i + 1

// TODO:
// 抽象化 任意の区間演算ならアーベル群　sum xor など
//    [1,i]だけならmin,maxもいける(こいつらは逆操作できない)、だがBIT2本でmin,maxも行ける模様?
// BIT2本で区間加算するやつ　→　クエリをバラしてやるところが遅延評価セグ木っぽい？
// 必要なところだけ作るBIT　→　辞書配列に持たせる　indexでかくても各要素へのアクセスが少ないパターン
// 多次元BITのverify

// verify:
//  - lower_bound(): https://atcoder.jp/contests/arc033/submissions/58037272

template <typename T, bool zero_indexed = true>
class fenwick_tree {
  int size, depth;
  std::vector<T> data;

public:
  fenwick_tree() = default;
  explicit fenwick_tree(int n) : size(n), depth(0), data(n) {
    while ((1 << depth) < (int)size) { ++depth; }
  }
  // prefix sum [0, i) (0-indexed)
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

  std::pair<int, T> lower_bound(T x) {
    // 累積和がx以上になる最小のindexと、その直前までの累積和
    // 二分木上の二分探索と同じ
    T sum = 0;
    int pos = 0;
    for (int i = depth; i >= 0; --i) {
      int j = pos + (1 << i);
      if (j <= size && sum + data[j - zero_indexed] < x) {
        // 答えは右側にある
        sum += data[j - zero_indexed]; // ←↑抽象化ポイント?
        pos += 1 << i;
      }
    }
    return std::make_pair(pos + 1 - zero_indexed, sum);
  }
};

template <typename T>
class fenwick_tree_2d {
  int height, width;
  std::vector<std::vector<T>> data;

public:
  fenwick_tree_2d() = default;
  explicit fenwick_tree_2d(int h, int w) : height(h), width(w) {
    data.assign(height, std::vector<T>(width));
  }

  void update(int h, int w, T x) {
    for (int i = h; i < height; i += i & -i) {
      for (int j = w; j < width; j += j & -j) { data[i][j] += x; }
    }
  }
  // [1, h] and [1, w]
  T query(int h, int w) {
    T result = 0;
    for (int i = h; i > 0; i -= i & -i) {
      for (int j = w; j > 0; j -= j & -j) { result += data[i][j]; }
    }
    return result;
  }
};