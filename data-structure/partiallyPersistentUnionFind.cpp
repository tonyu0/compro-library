
// 説明: 永続データ構造の一つ。任意の過去の状態を保つUnionFind。
// TODO: https://atcoder.jp/contests/agc002/tasks/agc002_d

struct PartiallyPersistentUnionFind {
  vector<int> par, time;
  vector<vector<pair<int, int>>> history;

  PartiallyPersistentUnionFind(int size) : par(size, -1), time(size, 1e9), history(size) {
    for(auto &tmp : history) tmp.emplace_back(-1, -1);
  }

  bool unite(int t, int x, int y) {
    x = find(t, x); y = find(t, y);
    if (x == y) return false;
    if (par[x] > par[y]) swap(x, y);
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
    return -prev(lower_bound(history[x].begin(), history[x].end(), make_pair(t, 0)))->second;
  }

  bool same(int t, int x, int y) {
    return find(t, x) == find(t, y);
  }
};
