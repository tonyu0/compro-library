#include <iostream>
#include <vector>
using namespace std;

struct Edge {
  int dst, cost;
  Edge(int d, int c) : dst(d), cost(c) {}
};
using Edges = vector<Edge>;
using Graph = vector<Edges>;

struct hld {
  int n, id;
  Graph g;
  vector<int> size, head, par, vid, cost;

  hld(int n, Graph &g)
    : n(n), g(g), size(n, 1), head(n), par(n), vid(n), cost(n) {
    dfs(0, -1, 0);
    id = 0;
    decompose(0, -1, 0);
  }

  void dfs(int v, int p, int c) {
    par[v] = p;
    cost[v] = c;

    for (Edge e : g[v]) {
      if (e.dst == p) continue;
      dfs(e.dst, v, c + e.cost);
      size[v] += size[e.dst];
    }
  }

  void decompose(int v, int p, int h) {
    vid[v] = id++;
    head[v] = h;

    pair<int, int> heavy = make_pair(-1, -1);
    for (Edge e : g[v]) {
      if (e.dst == p) continue;
      heavy = max(heavy, make_pair(size[e.dst], e.dst));
    }

    if (heavy.second != -1) decompose(heavy.second, v, h);
    for (Edge e : g[v]) {
      if (e.dst == p || e.dst == heavy.second) continue;
      decompose(e.dst, v, e.dst);
    }
  }

  int lca(int x, int y) {
    while (true) {
      if (vid[x] > vid[y]) swap(x, y);
      if (head[x] == head[y]) return x;
      y = par[head[y]];
    }
  }

  int dist(int x, int y) {
    int z = lca(x, y);
    return cost[x] + cost[y] - cost[z] * 2;
  }
};