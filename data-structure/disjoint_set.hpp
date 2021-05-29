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

class weighted_disjoint_set {
public:
  weighted_disjoint_set() = default;
  explicit weighted_disjoint_set(int n) : par(n, -1), weight(n) {}
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
