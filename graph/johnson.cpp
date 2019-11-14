#include <iostream>
#include <vector>
#include <queue>
#include <limits>
using namespace std;
// 説明:
// 全点対最短距離。十分にスパースなグラフならwarshall-floydより計算量が落ちる。
// ベルマンフォードをして負の辺を消去するポテンシャルを得て、ダイクストラを各頂点からする。
// 計算量:
// ベルマンフォードでのポテンシャル計算 -- O(VE)
// ダイクストラを各頂点からやる -- O(V * ElogV)
// 備考:
// ワーシャルフロイドが想定解の問題では頂点数 << 辺数なので普通は使わない。

template <typename T, bool directed>
class Johnson
{
public:
  Johnson(int n) : n(n), graph(n) {}
  void add_edge(int src, int dst, T cost)
  {
    graph[src].emplace_back(dst, cost);
    if (!directed)
      graph[dst].emplace_back(src, cost);
  }
  bool build()
  {
    if (!bellman_ford())
      return false;
    dist.assign(n, vector<T>(n, numeric_limits<T>::max() / 2));
    // n回ダイクストラ
    for (int s = 0; s < n; ++s)
      dijkstra(s);
    return true;
  }
  inline T get(int s, int t) const { return dist[s][t]; }

private:
  int n;
  struct Edge
  {
    int dst;
    T cost;
    Edge(int dst, T cost) : dst(dst), cost(cost) {}
  };
  vector<T> h;
  vector<vector<T>> dist;
  vector<vector<Edge>> graph;

  bool bellman_ford()
  {
    // ベルマンフォードによりポテンシャルを導入
    h.assign(n, 0);
    for (int i = 0; i < n; ++i)
      for (int v = 0; v < n; ++v)
        for (Edge e : graph[v])
          if (h[e.dst] > h[v] + e.cost)
          {
            h[e.dst] = h[v] + e.cost;
            if (i == n - 1)
              return false;
          }
    return true;
  }
  void dijkstra(int s)
  {
    priority_queue<pair<T, int>, vector<pair<T, int>>, greater<pair<T, int>>> pq;
    dist[s][s] = 0;
    pq.emplace(0, s);
    while (!pq.empty())
    {
      T c = pq.top().first;
      int v = pq.top().second;
      pq.pop();
      for (Edge e : graph[v])
        if (dist[s][e.dst] > dist[s][v] + e.cost)
        {
          dist[s][e.dst] = dist[s][v] + e.cost;
          pq.emplace(dist[s][e.dst], e.dst);
        }
    }
    for (int i = 0; i < n; ++i)
      dist[s][i] += h[i] - h[s];
  }
};

// https://atcoder.jp/contests/abc143/tasks/abc143_e
void ABC143E()
{
  long N, M, L, A, B, C, Q, s, t;
  cin >> N >> M >> L;
  Johnson<long, false> js1(N);
  for (int i = 0; i < M; ++i)
  {
    cin >> A >> B >> C;
    js1.add_edge(A - 1, B - 1, C);
  }
  js1.build();

  Johnson<long, false> js2(N);
  for (int i = 0; i < N; ++i)
  {
    for (int j = i + 1; j < N; ++j)
      if (js1.get(i, j) <= L)
        js2.add_edge(i, j, 1);
  }
  js2.build();
  cin >> Q;
  for (int i = 0; i < Q; ++i)
  {
    cin >> s >> t;
    long ans = js2.get(s - 1, t - 1);
    if (ans == numeric_limits<long>::max() / 2)
      cout << -1;
    else
      cout << ans - 1;
    cout << '\n';
  }
}

int main()
{
  ABC143E();
  return 0;
}