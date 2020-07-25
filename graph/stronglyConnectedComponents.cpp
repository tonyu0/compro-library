// 説明: 強連結成分分解をする。無向グラフは木に、有向グラフはDAGになる。

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

class StronglyConnectedComponents
{
public:
  StronglyConnectedComponents(int n, Graph g) : n(n), k(0), g(g), used(n, -1), components(n)
  {
    rg.resize(n);
    for (int i = 0; i < n; ++i)
      for (Edge e : g[i])
        rg[e.dst].emplace_back(i, e.cost);

    for (int i = 0; i < n; ++i)
      if (used[i] == -1)
        dfs1(i);
    for (int i = (int)order.size() - 1; i >= 0; --i)
      if (!used[order[i]])
        dfs2(order[i], k++);
  }
  int size() { return k; }
  bool same(int x, int y) { return components[x] == components[y]; }

private:
  int n, k;
  vector<int> used, order, components;
  Graph g, rg;

  void dfs1(int v)
  {
    used[v] = 0;
    for (Edge e : g[v])
      if (!!used[e.dst])
        dfs1(e.dst);
    order.push_back(v);
  }
  void dfs2(int v, int k)
  {
    used[v] = 1;
    components[v] = k;
    for (Edge e : rg[v])
      if (!used[e.dst])
        dfs2(e.dst, k);
  }
};

int main()
{
  return 0;
}