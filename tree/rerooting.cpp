#include <functional>
#include <vector>

template <typename T, typename F = std::function<T(const T &, const T &)>>
class rerooting {
public:
  // TODO: 辺にコストを持たせられるように
  // f: mergeする値が満たすべき条件
  // g: 親に渡す時の操作
  // merge: 子の値のマージ操作
  std::vector<vector<int>> tree, dp;
  F f, g, merge;
  T id;
  rerooting(int n, F f, F g, F merge, T id)
    : tree(n), dp(n), f(f), g(g), merge(merge), id(id) {}
  void add_edge(int u, int v) {
    tree[u].push_back(v);
    tree[v].push_back(u);
  }

  T dfs1(int v, int p) {
    dp[v].resize(tree[v].size());

    T res = id;
    for (int i = 0; i < (int)tree[v].size(); ++i) {
      if (tree[v][i] == p) continue;
      dp[v][i] = dfs1(tree[v][i], v);
      res = merge(res, f(dp[v][i], 0));
    }
    return g(res, v);
  }

  void dfs2(int v, int p, T from_parent) {
    for (int i = 0; i < (int)tree[v].size(); ++i)
      if (tree[v][i] == p) {
        dp[v][i] = from_parent;
        break;
      }

    std::vector<T> left(tree[v].size() + 1, id), right(tree[v].size() + 1, id);
    for (int i = (int)tree[v].size(); i > 0; --i)
      right[i - 1] = merge(right[i], f(dp[v][i - 1], 0));
    for (int i = 0; i < (int)tree[v].size(); ++i)
      left[i + 1] = merge(left[i], f(dp[v][i], 0));
    for (int i = 0; i < (int)tree[v].size(); ++i) {
      if (tree[v][i] == p) continue;
      T val = merge(left[i], right[i + 1]);
      dfs2(tree[v][i], v, g(val, v));
    }
  }

  void build() {
    dfs1(0, 0);
    dfs2(0, 0, id);
  }

  T get(int v) {
    T res = id;
    for (int i = 0; i < (int)tree[v].size(); ++i)
      res = merge(res, f(dp[v][i], 0));
    return g(res, v);
  }
};
template <typename T, typename F = std::function<T(const T &, const T &)>>
class rerooting {
public:
  // TODO: 辺にコストを持たせられるように
  // move: 親に渡す時の操作
  // merge: 子の値のマージ操作
  std::vector<vector<int>> tree, dp;
  F move, merge;
  T id;
  rerooting(int n, F move, F merge, T id)
    : tree(n), dp(n), move(move), merge(merge), id(id) {}
  void add_edge(int u, int v) {
    tree[u].push_back(v);
    tree[v].push_back(u);
  }

  T dfs1(int v, int p) {
    dp[v].resize(tree[v].size());

    T res = id;
    for (int i = 0; i < (int)tree[v].size(); ++i) {
      if (tree[v][i] == p) continue;
      dp[v][i] = dfs1(tree[v][i], v);
      res = merge(res, dp[v][i]);
    }
    return move(res, v);
  }

  void dfs2(int v, int p, T from_parent) {
    for (int i = 0; i < (int)tree[v].size(); ++i)
      if (tree[v][i] == p) {
        dp[v][i] = from_parent;
        break;
      }

    std::vector<T> left(tree[v].size() + 1, id), right(tree[v].size() + 1, id);
    for (int i = (int)tree[v].size(); i > 0; --i)
      right[i - 1] = merge(right[i], dp[v][i - 1]);
    for (int i = 0; i < (int)tree[v].size(); ++i)
      left[i + 1] = merge(left[i], dp[v][i]);
    for (int i = 0; i < (int)tree[v].size(); ++i) {
      if (tree[v][i] == p) continue;
      T val = merge(left[i], right[i + 1]);
      dfs2(tree[v][i], v, move(val, v));
    }
  }

  void build() {
    dfs1(0, 0);
    dfs2(0, 0, id);
  }

  // getは自分で書く。dp[v][i]にvを根としたときのiを根とした部分木の状態が入ってる。
  bool get(int v) {
    T res = 0;
    for (int i = 0; i < (int)tree[v].size(); ++i) res += dp[v][i] % 2;
    return res >= 2;
  }
};