#include <vector>

#include "../data-structure/segment_tree.hpp"

using namespace std;

// 木を列に置き換えて、区間に対する操作を可能にする。
// パスに対して加算、部分木に対して加算、とか
// 区間と実際の木の対応を考える必要がある
// 基本的にコストは辺に乗っていることが多いので、辺でin/outたどったほうがわかりやすいかもしれない
// 区間加算区間取得ができるとパスとか部分木に対して操作できる

class EulerTour {
public:
  EulerTour(int n) : g(n), in(n), out(n), cost(n), order(n * 2 - 1), depth(n) {}

  void add_edge(int from, int to, int cost = 1) {
    g[from].push_back(make_pair(to, cost));
    g[to].push_back(make_pair(from, cost));
  }

  void build() { dfs(); }

  // v's subtree is [in[v], out[v])
  std::pair<int, int> get_subtree(int v) { return make_pair(in[v], out[v]); }

  int get_lca(int x, int y) {
    return order[depth.query(min(in[x], in[y]), max(in[x], in[y]) + 1).second];
  }
  int get_length(int x, int y) {
    return cost[x] + cost[y] - cost[get_lca(x, y)] * 2;
  }

private:
  void dfs(int v = 0, int p = -1, int d = 0, int c = 0) {
    in[v] = cnt;
    cost[v] = c;
    depth.update(cnt, make_pair(d, cnt));
    order[cnt] = v;
    ++cnt;

    for (auto &[nv, nc] : g[v]) {
      if (nv == p) continue;
      dfs(nv, v, d + 1, c + nc);
      depth.update(cnt, make_pair(d, cnt));
      order[cnt] = v;
      ++cnt;
    }

    out[v] = cnt;
  }

  int N, cnt = 0;
  vector<vector<pair<int, int>>> g;
  vector<int> in, out, cost, order;
  // segtree setting for lca query
  static std::pair<int, int> identity() { return make_pair(0x3f3f3f3f, 0); }
  static std::pair<int, int> pair_min(std::pair<int, int> a,
                                      std::pair<int, int> b) {
    return std::min(a, b);
  }
  segment_tree<std::pair<int, int>, pair_min, identity> depth;
};