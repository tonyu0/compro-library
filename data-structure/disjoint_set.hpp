#include <vector>

class disjoint_set {
public:
  disjoint_set() = default;
  explicit disjoint_set(int n) : par(n, -1) {}
  void merge(int x, int y) {
    x = root(x);
    y = root(y);
    if (x == y) { return; }
    if (par[x] > par[y]) { std::swap(x, y); };
    par[x] += par[y];
    par[y] = x;
  }
  bool same(int x, int y) { return root(x) == root(y); }
  int root(int x) {
    if (par[x] < 0) { return x; }
    return par[x] = root(par[x]);
  }
  int sizeOf(int x) { return -par[root(x)]; }

private:
  std::vector<int> par;
};

class disjoint_set_weighted {
public:
  disjoint_set_weighted() = default;
  explicit disjoint_set_weighted(int n) : par(n, -1), weight(n) {}
  void merge(int x, int y, int w = 0) {
    w += get_weight(x);
    w -= get_weight(y);
    x = root(x);
    y = root(y);
    if (x == y) { return; }
    if (par[x] > par[y]) {
      std::swap(x, y);
      w = -w;
    }
    par[x] += par[y];
    par[y] = x;
    weight[y] = w;
    return;
  }
  bool same(int x, int y) { return root(x) == root(y); }
  int root(int x) {
    if (par[x] < 0) { return x; }
    int r = root(par[x]);
    weight[x] += get_weight(par[x]);
    return par[x] = r;
  }
  int sizeOf(int x) { return -par[root(x)]; }
  int get_weight(int x) {
    root(x);
    return weight[x];
  }
  int diff(int x, int y) { return get_weight(y) - get_weight(x); }

private:
  std::vector<int> par, weight;
};

//
// TODO: https://atcoder.jp/contests/agc002/tasks/agc002_d

// https://atcoder.jp/contests/code-thanks-festival-2017-open/submissions/58277102
// disjoint set

class disjoint_set_partially_persistent {
  std::vector<int> par, time;
  std::vector<std::vector<std::pair<int, int>>> history;

public:
  disjoint_set_partially_persistent(int size)
    : par(size, -1), time(size, 1e9), history(size) {
    for (auto &el : history) { el.emplace_back(-1, -1); }
  }

  bool merge(int t, int x, int y) {
    x = find(t, x);
    y = find(t, y);
    if (x == y) { return false; }
    if (par[x] > par[y]) { std::swap(x, y); }
    par[x] += par[y];
    history[x].emplace_back(t, par[x]);
    par[y] = x;
    time[y] = t;
    return true;
  }

  int find(int t, int x) {
    if (t < time[x]) return x;
    return find(t, par[x]);
  }

  int sizeOf(int t, int x) {
    x = find(t, x);
    return -prev(lower_bound(history[x].begin(), history[x].end(),
                             std::make_pair(t, 0)))
              ->second;
  }

  bool same(int t, int x, int y) { return find(t, x) == find(t, y); }
};
