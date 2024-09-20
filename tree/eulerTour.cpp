#include <vector>
using namespace std;

// 木を列に置き換えて、区間に対する操作を可能にする。
// パスに対して加算、部分木に対して加算、とか
// 区間と実際の木の対応を考える必要がある
// 基本的にコストは辺に乗っていることが多いので、辺でin/outたどったほうがわかりやすいかもしれない
// 区間加算区間取得ができるとパスとか部分木に対して操作できる

struct EulerTour {
  int N, cnt = 0;
  vector<vector<pair<int, int>>> g;
  vector<int> cost, in, out, ord;
  vector<pair<int, int>> depth;

  EulerTour(int n) : g(n), in(n), out(n), cost(n), ord(n * 2) {
    for (N = 1; N < n * 2; N <<= 1);
    depth.resize(N * 2 - 1);
  }

  void add_edge(int from, int to, int cost) {
    g[from].push_back(make_pair(to, cost));
    g[to].push_back(make_pair(from, cost));
  }

  void build() {
    dfs();

    for (int i = N - 2; i >= 0; --i)
      depth[i] = min(depth[i * 2 + 1], depth[i * 2 + 2]);
  }

  void dfs(int v = 0, int p = -1, int d = 0, int c = 0) {
    // rnk: preorder
    in[v] = cnt;
    cost[v] = c;
    depth[cnt + N - 1] = make_pair(d, cnt);
    ord[cnt] = v;
    ++cnt;

    for (auto& [nv, nc] : g[v]) {
      if (nv == p) continue;
      dfs(nv, v, d + 1, c + nc);
      depth[cnt + N - 1] = make_pair(d, cnt);
      ord[cnt] = v;
      ++cnt;
    }

    out[v] = cnt;
  }

  std::pair<int, int> get_subtree(int v, bool skip_root = false) {
    return make_pair(in[v] + (int)skip_root, out[v]);
  }

  std::pair<int, int> rmq(int a, int b, int i = 0, int l = 0, int r = -1) {
    if (r == -1) r = N;
    if (r <= a || b <= l) return make_pair(1 << 30, 0);
    if (a <= l && r <= b) return depth[i];

    auto L = rmq(a, b, i * 2 + 1, l, (l + r) / 2);
    auto R = rmq(a, b, i * 2 + 2, (l + r) / 2, r);
    return min(L, R);
  }

  int lca(int x, int y) {
    return ord[rmq(min(in[x], in[y]), max(in[x], in[y]) + 1).second];
  }
  int dist(int x, int y) { return cost[x] + cost[y] - cost[lca(x, y)] * 2; }
};
