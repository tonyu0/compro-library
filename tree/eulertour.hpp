#include <algorithm>
#include <vector>

#include "../data-structure/segment_tree.hpp"

// Euler tour:
//  - 頂点をdfs順にたどりながら配列に変換する。部分木に対するクエリなどが可能
// verify:
//  - get_lca: https://atcoder.jp/contests/abc014/submissions/58073831
//  - get_subtree_order: https://judge.yosupo.jp/submission/237633
class EulerTour {
public:
  EulerTour(int n)
    : g(n), in_order(n), out_order(n), cost(n), order2id(n * 2), depth(n * 2) {}

  void add_edge(int from, int to, int cost = 1) {
    g[from].emplace_back(to, cost);
    g[to].emplace_back(from, cost);
  }

  void build() { dfs(); }

  // order series: vertex id to several types of order arranged by Euler tour
  // Usage example: querying/updating values of subtree with segtree
  int get_order(int id) { return in_order[id]; }
  // v's subtree is [in_order[v], out_order[v])
  std::pair<int, int> get_subtree_order(int id) {
    return std::make_pair(in_order[id], out_order[id]);
  }

  int get_lca(int id0, int id1) {
    if (in_order[id0] > in_order[id1]) std::swap(id0, id1);
    // LCA is minimum depth vertex while Euler tour from id0 to id1
    return depth.query(in_order[id0], in_order[id1] + 1).second;
  }

  int get_length(int x, int y) {
    return cost[x] + cost[y] - cost[get_lca(x, y)] * 2;
  }

private:
  void dfs(int v = 0, int p = -1, int d = 0, int c = 0) {
    in_order[v] = cnt;
    cost[v] = c;
    depth.update(cnt, std::make_pair(d, v));
    order2id[cnt] = v;
    ++cnt;

    for (auto &[nv, nc] : g[v]) {
      if (nv == p) continue;
      dfs(nv, v, d + 1, c + nc);
      depth.update(cnt, std::make_pair(d, v));
      order2id[cnt] = v;
      ++cnt;
    }
    out_order[v] = cnt;
  }

  int N, cnt = 0;
  std::vector<std::vector<std::pair<int, int>>> g;
  std::vector<int> in_order, out_order, cost, order2id;

  // segtree setting for lca query, pair: (depth, id)
  static std::pair<int, int> id() { return std::make_pair(0x3f3f3f3f, -1); }
  static std::pair<int, int> op(std::pair<int, int> a, std::pair<int, int> b) {
    return std::min(a, b);
  }
  segment_tree<std::pair<int, int>, op, id> depth;
};