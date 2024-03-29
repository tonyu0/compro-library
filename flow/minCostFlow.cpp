#include <iostream>
#include <limits>
#include <queue>
#include <vector>
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

// TODO: http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2293

template <typename T>
class MinCostFlow {
public:
  MinCostFlow(int n)
    : n(n)
    , capacity(n, vector<T>(n))
    , cost(n, vector<T>(n))
    , edges(n)
    , prev(n) {}

  void add_edge(int src, int dst, T cap, T cos) {
    capacity[src][dst] = cap;
    capacity[dst][src] = 0;
    cost[src][dst] = cos;
    cost[dst][src] = -cos;
    edges[src].push_back(dst);
    edges[dst].push_back(src);
  }

  T min_cost_flow(int s, int t, T f) {
    T res = 0;
    h.assign(n, 0);
    while (f > 0) {
      // 残余グラフ上で辿り着けなかったら無理
      if (!dijkstra(s, t)) return -1;
      // ポテンシャルを計算する。
      for (int i = 0; i < n; ++i) h[i] += min_cost[i];

      T d = f;
      for (int v = t; v != s; v = prev[v]) d = min(d, capacity[prev[v]][v]);
      // 今のステップではd流せるので、流す。
      f -= d;
      res += d * h[t];
      // 残余グラフを編集
      for (int v = t; v != s; v = prev[v]) {
        capacity[prev[v]][v] -= d;
        capacity[v][prev[v]] += d;
      }
    }
    return res;
  }

private:
  int n;
  T inf = numeric_limits<T>::max() / 3;
  vector<vector<T>> capacity, cost;
  vector<vector<int>> edges;
  vector<T> min_cost, h;
  vector<int> prev;

  // 設定したポテンシャルにより残余グラフ上で最短経路問題を解く。
  bool dijkstra(int s, int t) {
    min_cost.assign(n, inf);
    min_cost[s] = 0;
    priority_queue<pair<T, int>, vector<pair<T, int>>, greater<pair<T, int>>>
      pq;
    pq.emplace(0, s);
    while (!pq.empty()) {
      int c = pq.top().first;
      int v = pq.top().second;
      pq.pop();
      if (min_cost[v] < c) continue;
      for (int nv : edges[v]) {
        if (capacity[v][nv] > 0 &&
            min_cost[nv] > min_cost[v] + cost[v][nv] + h[v] - h[nv]) {
          min_cost[nv] = min_cost[v] + cost[v][nv] + h[v] - h[nv];
          prev[nv] = v;
          pq.emplace(min_cost[nv], nv);
        }
      }
    }
    return min_cost[t] != inf;
  }
};

// verify
// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_6_B
void GRL6B() {
  int V, E, F, u, v, c, d;
  cin >> V >> E >> F;
  MinCostFlow<int> mcf(V + 1);
  for (int i = 0; i < E; ++i) {
    cin >> u >> v >> c >> d;
    mcf.add_edge(u, v, c, d);
  }
  cout << mcf.min_cost_flow(0, V - 1, F) << endl;
}

// https://atcoder.jp/contests/abc004/tasks/abc004_4
void ABC004D() {
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
  for (int i = 0; i <= 800; ++i) mcf.add_edge(R, i, 1, abs(400 - i));
  for (int i = 100; i <= 900; ++i) mcf.add_edge(G, i, 1, abs(500 - i));
  for (int i = 200; i <= 1000; ++i) mcf.add_edge(B, i, 1, abs(600 - i));
  for (int i = 0; i <= 1000; ++i) mcf.add_edge(i, T, 1, 0);
  cout << mcf.min_cost_flow(S, T, r + b + g) << '\n';
}

// 最大?費用流 + 流量n以下の解が欲しい時はs→tに容量無限コスト0の辺を貼る。
// これはTLEするが高速化すると行けるらしい。➡verifyしてない、本当？
// https://atcoder.jp/contests/abc137/tasks/abc137_d
// ➡(20200918)違うかも, 適当なbaseを与え,
// s→tに容量上限、コスト無限の辺を張るといける
// https://atcoder.jp/contests/practice2/submissions/16811765
void ABC137D() {
  long long n, m, a, b;
  cin >> n >> m;
  MinCostFlow<long long> flow(n + m + 3);
  int s = n + m + 1, t = n + m + 2;
  for (int i = 0; i < n; ++i) {
    cin >> a >> b;
    for (int j = m - a; j >= 0; --j) flow.add_edge(i, n + j, 1, -b);
  }

  for (int i = 0; i < n; ++i) flow.add_edge(s, i, 1, 0);
  for (int i = 0; i < m; ++i) flow.add_edge(n + i, t, 1, 0);
  flow.add_edge(s, t, 1LL << 60, 0);
  cout << -flow.min_cost_flow(s, t, n);
}

// 割り当て問題
// https://atcoder.jp/contests/pakencamp-2018-day3/tasks/pakencamp_2018_day3_g
void other() {
  long long N, M, K, X[1000], S, E;
  cin >> N >> M >> K;
  MinCostFlow<long long> flow(N + M + 2);
  int s = N + M, t = N + M + 1;
  for (int i = 0; i < N; ++i) cin >> X[i];
  for (int i = 0; i < M; ++i) {
    cin >> S >> E;
    --S, --E;
    for (int j = S; j <= E; ++j) flow.add_edge(i, M + j, 1, X[j]);
  }
  for (int i = 0; i < M; ++i) flow.add_edge(s, i, 1, 0);
  for (int i = 0; i < N; ++i) flow.add_edge(M + i, t, 1, 0);
  cout << flow.min_cost_flow(s, t, K);
}

int main() {
  // GRL6B();
  // ABC004D();
  // ABC137D();
  other();
  return 0;
}