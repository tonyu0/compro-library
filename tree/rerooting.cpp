#include <functional>
#include <vector>

// https://atcoder.jp/contests/abc222/submissions/26478032
// 子のマージ、親への値の伝播の操作がモノイドになる状態で、それの操作の結果を各根に対して求める。
template <typename T>
class rerooting {
public:
  rerooting(int n, T id) : tree(n), dp(n), id(id) {}
  void add_edge(int u, int v, T c = 1) {
    tree[u].emplace_back(v, c);
    tree[v].emplace_back(u, c);
  }
  void build() {
    dfs1(0, -1);
    dfs2(0, -1, id);
  }

  T get(int v) {
    T res = id;
    for (int i = 0; i < (int)tree[v].size(); ++i) {
      res = merge(res, dp[v][i]);
    }
    return res;
  }

private:
  T dfs1(int v, int p) {
    dp[v].resize(tree[v].size());

    T res = id;
    for (int i = 0; i < (int)tree[v].size(); ++i) {
      if (tree[v][i].first == p) { continue; }
      dp[v][i] = dfs1(tree[v][i].first, v);
      res = merge(res, dp[v][i]);
    }
    return to_parent(res, v);
  }
  void dfs2(int v, int p, T from_parent) {
    for (int i = 0; i < (int)tree[v].size(); ++i) {
      if (tree[v][i].first == p) {
        dp[v][i] = from_parent;
        break;
      }
    }

    std::vector<T> left(tree[v].size() + 1, id), right(tree[v].size() + 1, id);
    for (int i = (int)tree[v].size(); i > 0; --i) {
      right[i - 1] = merge(right[i], dp[v][i - 1]);
    }
    for (int i = 0; i < (int)tree[v].size(); ++i) {
      left[i + 1] = merge(left[i], dp[v][i]);
    }
    for (int i = 0; i < (int)tree[v].size(); ++i) {
      if (tree[v][i].first == p) { continue; }
      T val = merge(left[i], right[i + 1]);
      dfs2(tree[v][i].first, v, to_parent(val, v));
    }
  }

  // 問題ごとに換える
  // 関数を渡す感じでもいいが、引数とかスコープも変わってきて整合性を取るのが大変だった

  // merge:
  // ある頂点の子の情報に対して左右からmergeを適用するため、Tとmergeはモノイドになる必要がある。
  // ここは引数も確定
  T merge(T lhs, T rhs) { return max(lhs, rhs); }
  // to_parent:
  // 親へ情報を渡すときの操作、色々やりたいこともあるので、引数の数も変わることが多い
  // 「根であること」を子に伝播するために渡す必要のある情報
  T to_parent(T i, T j) { return i; }

  std::vector<vector<pair<int, T>>> tree;
  std::vector<vector<T>> dp;
  T id;
};