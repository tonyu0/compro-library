#include <iostream>
#include <vector>
#include <queue>
#include <limits>
using namespace std;
// 説明:
// 最小費用流(始点から終点に、特定の流量dを流すときの最小コスト)を求める。
// コストは、"辺のコスト*辺に流した流量"の全ての辺に対する総和。
// 最小コスト最大流とも捉えることができる。

// 計算量: O(F|V|^2)

// ポイント:
// 各ステップでは残余グラフ上でs-t最短経路を求め、その経路に流せるだけ流す。
// 流したらその経路上の容量を減らしてく。そのときに経路復元する必要がある。
// 最短経路パートはベルマンフォードでもできるが、ダイクストラを使うためには負の辺を除去する工夫をする。

// TODO:
// https://atcoder.jp/contests/pakencamp-2018-day3/tasks/pakencamp_2018_day3_g
// https://atcoder.jp/contests/abc137/tasks/abc137_d

template <typename T>
class MinCostFlow
{
public:
  MinCostFlow(int n) : n(n), graph(n), prevv(n), preve(n) {}

  void add_edge(int src, int dst, T cap, T cost)
  {
    // 逆辺はインデックスで管理
    graph[src].emplace_back(dst, cap, cost, graph[dst].size());
    graph[dst].emplace_back(src, 0, -cost, graph[src].size() - 1);
  }

  T min_cost_flow(int s, int t, T f)
  {
    T res = 0;
    h.assign(n, 0);
    while (f > 0)
    {
      // 残余グラフ上で辿り着けなかったら無理
      if (!dijkstra(s, t))
        return -1;
      // ポテンシャルを計算する。
      for (int i = 0; i < n; ++i)
        h[i] += min_cost[i];

      T d = f;
      for (int v = t; v != s; v = prevv[v])
        d = min(d, graph[prevv[v]][preve[v]].cap);
      // 今のステップではd流せるので、流す。
      f -= d;
      res += d * h[t];
      // 残余グラフを編集
      for (int v = t; v != s; v = prevv[v])
      {
        // prevv[v] -> vにpreve[v]を使って流したので・・
        Edge &e = graph[prevv[v]][preve[v]];
        e.cap -= d;
        graph[v][e.rev].cap += d;
      }
    }
    return res;
  }

private:
  struct Edge
  {
    int dst, rev;
    T cap, cost;
    Edge(int dst, T cap, T cost, int rev) : dst(dst), cap(cap), cost(cost), rev(rev){};
  };
  int n;
  T inf = numeric_limits<T>::max();
  vector<vector<Edge>> graph;
  vector<T> min_cost, h, prevv, preve;

  // 設定したポテンシャルにより残余グラフ上で最短経路問題を解く。
  bool dijkstra(int s, int t)
  {
    min_cost.assign(n, inf);
    min_cost[s] = 0;
    priority_queue<pair<T, int>, vector<pair<T, int>>, greater<pair<T, int>>> pq;
    pq.emplace(0, s);
    while (!pq.empty())
    {
      int c = pq.top().first;
      int v = pq.top().second;
      pq.pop();
      if (min_cost[v] < c)
        continue;
      for (int i = 0; i < (int)graph[v].size(); ++i)
      {
        Edge e = graph[v][i];
        if (e.cap > 0 && min_cost[e.dst] > min_cost[v] + e.cost + h[v] - h[e.dst])
        {
          min_cost[e.dst] = min_cost[v] + e.cost + h[v] - h[e.dst];
          prevv[e.dst] = v;
          preve[e.dst] = i;
          pq.emplace(min_cost[e.dst], e.dst);
        }
      }
    }
    return min_cost[t] != inf;
  }
};

// verify
// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_6_B
void GRL6B()
{
  int V, E, F, u, v, c, d;
  cin >> V >> E >> F;
  MinCostFlow<int> mcf(V + 1);
  for (int i = 0; i < E; ++i)
  {
    cin >> u >> v >> c >> d;
    mcf.add_edge(u, v, c, d);
  }
  cout << mcf.min_cost_flow(0, V - 1, F) << endl;
}

// https://atcoder.jp/contests/abc004/tasks/abc004_4
void ABC004D()
{
  int r, g, b;
  cin >> r >> g >> b;
  // R+G+B <= 1000
  // 始点-(R,G,B)-(箱1200個くらい用意)-終点
  // Rの位置400, Gの位置500, Bの位置600
  MinCostFlow<int> mcf(1010);
  int S = 1001, T = 1002, R = 1003, G = 1004, B = 1005;

  mcf.add_edge(S, R, r, 0);
  mcf.add_edge(S, G, g, 0);
  mcf.add_edge(S, B, b, 0);
  for (int i = 0; i <= 800; ++i)
    mcf.add_edge(R, i, 1, abs(400 - i));
  for (int i = 100; i <= 900; ++i)
    mcf.add_edge(G, i, 1, abs(500 - i));
  for (int i = 200; i <= 1000; ++i)
    mcf.add_edge(B, i, 1, abs(600 - i));
  for (int i = 0; i <= 1000; ++i)
    mcf.add_edge(i, T, 1, 0);
  cout << mcf.min_cost_flow(S, T, r + b + g) << '\n';
}

int main()
{
  // GRL6B();
  ABC004D();
  return 0;
}