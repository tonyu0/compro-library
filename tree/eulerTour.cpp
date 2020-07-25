#include <iostream>
#include <vector>
using namespace std;

struct Edge
{
  int dst, cost;
  Edge(int d, int c) : dst(d), cost(c) {}
};

using Edges = vector<Edge>;
using Graph = vector<Edges>;

struct EulerTour
{
  int N, cnt = 0;
  Graph g;
  vector<int> cost, rnk, ord;
  vector<pair<int, int>> depth;

  EulerTour(int n, Graph &g) : g(g), rnk(n), cost(n), ord(n * 2)
  {
    for (N = 1; N < n * 2; N <<= 1)
      ;
    depth.resize(N * 2 - 1);

    dfs();

    for (int i = N - 2; i >= 0; --i)
      depth[i] = min(depth[i * 2 + 1], depth[i * 2 + 2]);
  }

  void dfs(int v = 0, int p = -1, int d = 0, int c = 0)
  {
    rnk[v] = cnt, cost[v] = c;
    depth[cnt + N - 1] = make_pair(d, cnt), ord[cnt] = v;
    ++cnt;

    for (Edge e : g[v])
    {
      if (e.dst == p)
        continue;
      dfs(e.dst, v, d + 1, c + e.cost);
      depth[cnt + N - 1] = make_pair(d, cnt), ord[cnt] = v;
      ++cnt;
    }
  }

  pair<int, int> rmq(int a, int b, int i = 0, int l = 0, int r = -1)
  {
    if (r == -1)
      r = N;
    if (r <= a || b <= l)
      return make_pair(1 << 30, 0);
    if (a <= l && r <= b)
      return depth[i];

    auto L = rmq(a, b, i * 2 + 1, l, (l + r) / 2);
    auto R = rmq(a, b, i * 2 + 2, (l + r) / 2, r);
    return min(L, R);
  }

  int lca(int x, int y) { return ord[rmq(min(rnk[x], rnk[y]), max(rnk[x], rnk[y]) + 1).second]; }
  int dist(int x, int y) { return cost[x] + cost[y] - cost[lca(x, y)] * 2; }
};
