
// 説明: 集合の大きさ・重みを取れる実装

struct UnionFind {
  vector<int> par, weight;
  UnionFind(int n) : par(n,-1), weight(n) {}
  void unite(int x, int y, int w = 0) {
    // w += get_weight(x); w -= get_weight(y);
    x = find(x); y = find(y);
    if (x == y) return;
    if (par[x] > par[y]) swap(x, y), w = -w;
    par[x] += par[y];
    par[y] = x;
    // weight[y] = w;
    return;
  }
  bool same(int x, int y) { return find(x) == find(y); }
  int find(int x) {
    if(par[x] < 0) return x;
    int r = find(par[x]);
    // weight[x] += get_weight(par[x]);
    return par[x] = r;
  }
  int sizeOf(int x) { return -par[find(x)]; }
  // int get_weight(int x) { find(x); return weight[x]; }
  // int diff(int x, int y) { return get_weight(y) - get_weight(x); }
};
