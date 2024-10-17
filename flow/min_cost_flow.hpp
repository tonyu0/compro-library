#include <limits>
#include <queue>
#include <vector>
// 説明:
// 最小費用流(始点から終点に、特定の流量dを流すときの最小コスト)を求める。
// コストは、"辺のコスト*辺に流した流量"の全ての辺に対する総和。
// 最小コスト最大流とも捉えることができる。

// 計算量: O(F|V|^2)

// ポイント:
// 各ステップでは残余グラフ上でs-t最短経路を求め、その経路に流せるだけ流す。
// 流したらその経路上の容量を減らしてく。そのときに経路復元する必要がある。
// 負の辺を除去することでベルマンフォードではなくダイクストラを使用でき計算量が改善する

// verify:
// 流量n以下が欲しい時は、余分な流量を流すためにs-tに容量無限コスト0の辺を張り、実際に流すパスのコストは負になるように調整する。(実際のパスに優先的に流れるように)
// https://atcoder.jp/contests/practice2/submissions/58865170
// https://atcoder.jp/contests/abc137/submissions/58865370
// (仕事-報酬を受け取る日　の割り当て問題.これはTLEするが高速化すると行けるらしい)
// https://atcoder.jp/contests/pakencamp-2018-day3/submissions/8520640
// (宿題-やる日　の割り当て問題)
// https://atcoder.jp/contests/abc004/submissions/8441193
// (発展形だが1対1でなくても属性を管理する頂点を作ればいける)

// TODO: http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2293

template <typename T>
class min_cost_flow {
  using Edge = std::tuple<int, size_t, T, T>;

public:
  min_cost_flow(int n) : n(n), edges(n), prev(n) {}

  void add_edge(int from, int to, T cap, T cost) {
    edges[from].emplace_back(to, edges[to].size(), cap, cost);
    edges[to].emplace_back(from, edges[from].size() - 1, 0, -cost);
  }

  const std::vector<std::vector<Edge>> &get_edges() { return edges; }

  T flow(int s, int t, T f) {
    T res = 0;
    h.assign(n, 0);
    while (f > 0) {
      // 残余グラフ上で辿り着けなかったら無理
      if (!dijkstra(s, t)) { return -1; }
      // 負辺除去のためのhを更新
      for (int i = 0; i < n; ++i) { h[i] += dist[i]; }

      T d = f;
      for (int v = t; v != s; v = prev[v].first) {
        d = std::min(d, std::get<2>(edges[prev[v].first][prev[v].second]));
      }
      // 今のステップではd流せるので、流す。
      f -= d;
      res += d * h[t];
      // 残余グラフを編集
      for (int v = t; v != s; v = prev[v].first) {
        auto &[_v, revidx, cap, _cost] = edges[prev[v].first][prev[v].second];
        auto &[_rv, _rrevidx, rcap, _rcost] = edges[v][revidx];
        cap -= d;
        rcap += d;
      }
    }
    return res;
  }

private:
  int n;
  T inf = std::numeric_limits<T>::max() / 3;
  std::vector<std::vector<Edge>> edges;
  std::vector<T> dist, h;
  std::vector<std::pair<int, int>> prev;
  std::priority_queue<std::pair<T, int>, std::vector<std::pair<T, int>>,
                      std::greater<std::pair<T, int>>>
    pq;

  // 残余グラフ上最短経路
  bool dijkstra(int s, int t) {
    dist.assign(n, inf);
    dist[s] = 0;
    pq.emplace(0, s);
    while (!pq.empty()) {
      auto [cur_dist, cur] = pq.top();
      pq.pop();
      if (dist[cur] < cur_dist) { continue; }
      for (int i = 0; i < static_cast<int>(edges[cur].size()); ++i) {
        const auto &[nxt, _, cap, cost] = edges[cur][i];
        if (cap > 0 && dist[nxt] > dist[cur] + cost + h[cur] - h[nxt]) {
          dist[nxt] = dist[cur] + cost + h[cur] - h[nxt];
          pq.emplace(dist[nxt], nxt);

          prev[nxt] = std::make_pair(cur, i);
        }
      }
    }
    return dist[t] != inf;
  }
};
