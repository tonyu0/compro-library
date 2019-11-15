#include <iostream>
#include <queue>
#include <vector>
#include <limits>
using namespace std;

// 説明:
// 最大流問題を解く。
// 高速化ポイント:
// 1. level配列により残余グラフ上の最短経路
// 2. start配列により探索した辺を管理することで同じ辺を何度も探索しない。
// 3. 終点からdfsをすることで始点から終点に辿り着けないパスを探索しない。
// 4. (高速化ではないが)DFSが一回で済むように工夫してある。

// 動的木を用いるとdfsがO(VE) -> O(ElogV)になるらしい。それで全体がO(VElogV)に改善される。

// Dinic O(|E||V|^2)
template <typename T, bool directed>
class Dinic
{
public:
  Dinic(int n) : n(n), capacity(n, vector<T>(n)), edges(n) {}
  void add_edge(int src, int dst, T cap)
  {
    capacity[src][dst] = cap;
    capacity[dst][src] = directed ? 0 : cap;
    edges[src].push_back(dst);
    edges[dst].push_back(src);
  }

  T max_flow(int s, int t)
  {
    T flow = 0;
    while (bfs(s, t))
    {
      // 無駄な辺を何度も調べないために、各頂点に既に調べた辺を持たせる。
      // 各頂点でdfsを続きからできるようにするイメージ
      start.assign(n, 0);
      flow += dfs(t, s, numeric_limits<T>::max());
    }
    return flow;
  }

private:
  int n;
  vector<int> level, start;
  vector<vector<T>> capacity;
  vector<vector<int>> edges;
  // n: 頂点数
  // level: 残余グラフ上の始点からの各点の最短距離
  // start: 各点で探索した辺のインデックス、ステップごとに初期化
  // capacity: 隣接行列で辺の容量を保持

  // 残余グラフ上でsからの最短距離を計算する。tに辿り着けなかったらfalseを返す。
  bool bfs(int s, int t)
  {
    level.assign(n, -1);
    queue<int> que;
    level[s] = 0;
    que.push(s);
    while (!que.empty())
    {
      int v = que.front();
      que.pop();
      for (int nv : edges[v])
        if (capacity[v][nv] > 0 && level[nv] < 0)
        {
          level[nv] = level[v] + 1;
          que.push(nv);
        }
    }
    return level[t] != -1;
  }

  // 逆辺をたどる。
  // limit: 現時点で流せる最大量
  // 1ステップでvから流せる量をresにまとめることで、DFSが一回で済む。
  T dfs(int v, int s, T limit)
  {
    if (v == s)
      return limit;

    T res = 0;
    for (int &i = start[v]; i < (int)edges[v].size(); ++i)
    {
      int nv = edges[v][i];
      // 距離が増加する向きの辺のみ調べる。
      if (capacity[nv][v] > 0 && level[v] == level[nv] + 1)
      {
        // d: 流せた量
        T d = dfs(nv, s, min(limit, capacity[nv][v]));
        if (d > 0)
        {
          capacity[nv][v] -= d;
          capacity[v][nv] += d;
          res += d;
          limit -= d;
          if (limit == 0)
            break;
        }
      }
    }
    return res;
  }
};

// verify
// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_6_A
void GRL6A()
{
  int V, E, u, v, c;
  cin >> V >> E;
  Dinic<int, true> dn(V);
  for (int i = 0; i < E; ++i)
  {
    cin >> u >> v >> c;
    dn.add_edge(u, v, c);
  }
  cout << dn.max_flow(0, V - 1) << endl;
}

// 最小カット = 最大流, 無向グラフ
// https://atcoder.jp/contests/abc010/tasks/abc010_4
void ABC010D()
{
  int N, G, E, p, a, b;
  cin >> N >> G >> E;
  Dinic<int, false> dn(N + 1);
  for (int i = 0; i < G; ++i)
  {
    cin >> p;
    dn.add_edge(p, N, 1);
  }
  for (int i = 0; i < E; ++i)
  {
    cin >> a >> b;
    dn.add_edge(a, b, 1);
  }
  cout << dn.max_flow(0, N) << endl;
}

// 二部グラフの最大マッチング
// https://atcoder.jp/contests/arc092/tasks/arc092_a
void ARC092C()
{
  int N;
  cin >> N;
  int a[100], b[100], c[100], d[100];
  Dinic<int, true> dn(N * 2 + 2); // 二部グラフ + 始点(N*2) + 終点(N*2+1)
  for (int i = 0; i < N; ++i)
    cin >> a[i] >> b[i];
  for (int i = 0; i < N; ++i)
    cin >> c[i] >> d[i];
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      if (a[i] < c[j] && b[i] < d[j])
        dn.add_edge(i, N + j, 1);
  for (int i = 0; i < N; ++i)
  {
    dn.add_edge(N * 2, i, 1);
    dn.add_edge(N + i, N * 2 + 1, 1);
  }
  cout << dn.max_flow(N * 2, N * 2 + 1) << endl;
}

// Project Selection Problem:
// 各要素を2つの集合(選択)に分類する時、上手くグラフを作ると
// 頂点を最適に始点側or終点側へと分離する問題、つまり最小カットに帰着できる。
// https://atcoder.jp/contests/arc085/tasks/arc085_c
void ARC085E()
{
  int N;
  cin >> N;
  long a[111], sum = 0;
  for (int i = 1; i <= N; ++i)
  {
    cin >> a[i];
    if (a[i] > 0)
      sum += a[i];
  }

  Dinic<long, true> dn(N + 3);
  // S(N+1)側: 割る, T(N+2)側: 割らない
  // 全ての要素を、損失を最小化しながら2つの集合に分類することを考える。
  // N+1 ~ iの辺をカットすることはTに分類することに等しい。
  // i ~ N+2の辺をカットすることはSに分類することに等しい。
  for (int i = 1; i <= N; ++i)
    if (a[i] < 0)
      dn.add_edge(N + 1, i, -a[i]); // 割らないことによる損失
    else
      dn.add_edge(i, N + 2, a[i]); // 割ることによる損失

  // jがiの倍数である時、2つは別の集合に分類できない。
  // それを表すために容量∞の辺を張る。
  for (int i = 1; i <= N; ++i)
    for (int j = i * 2; j <= N; j += i)
      dn.add_edge(i, j, numeric_limits<long>::max());
  cout << sum - dn.max_flow(N + 1, N + 2) << endl;
}

int main()
{
  GRL6A();
  // ABC010D();
  // ARC092C();
  // ARC085E();
  return 0;
}