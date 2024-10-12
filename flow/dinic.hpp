#include <limits>
#include <queue>
#include <vector>

// 説明:
// 最大流問題を解く。
// 高速化ポイント:
// 1. dist配列により残余グラフ上の最短経路
// 2. start配列により探索した辺を管理することで同じ辺を何度も探索しない。
// 3. 終点からdfsをすることで始点から終点に辿り着けないパスを探索しない。
// 4. (高速化ではないが)DFSが一回で済むように工夫してある。

// 動的木を用いるとdfsがO(VE) ->
// O(ElogV)になるらしい。それで全体がO(VElogV)に改善される。

// 使用例:
// - bipartite matching1:  https://judge.yosupo.jp/submission/241259
//    -> TODO: Hopcroft-Karp
// - bipartite matching2: https://atcoder.jp/contests/arc092/submissions/8404542
// - 最小カット=最大流: https://atcoder.jp/contests/abc010/submissions/8441351
// - ProjectSelectionProblem:
//    各要素を2つの集合(選択)に分類する時、上手くグラフを作ると頂点を最適に始点側or終点側へと分離する問題、つまり最小カットに帰着できる。
//    https://atcoder.jp/contests/arc085/submissions/58625726

// Dinic O(|E||V|^2)
template <typename T, bool directed = true>
class Dinic {
  using Edge = std::tuple<int, size_t, T>;

public:
  Dinic(int n) : n(n), edges(n) {}
  void add_edge(int from, int to, T cap) {
    // 逆に流し戻すために逆辺の情報を持つ必要があるが、隣接リストで持つために逆辺を示すインデックスをいい感じに持たせる
    edges[from].emplace_back(to, edges[to].size(), cap);
    edges[to].emplace_back(from, edges[from].size() - 1, directed ? 0 : cap);
  }

  T max_flow(int s, int t) {
    T flow = 0;
    while (bfs(s, t)) {
      start.assign(n, 0);
      flow += dfs(t, s, std::numeric_limits<T>::max());
    }
    return flow;
  }

  // can check capacity after processing if use after max_flow()
  const std::vector<std::vector<Edge>> &get_edges() const { return edges; }

private:
  int n;
  std::vector<int> dist, start;
  std::vector<std::vector<Edge>> edges;
  // dist: 残余グラフ上の始点からの各点の最短距離
  // start: 各点で探索した辺のインデックス、ステップごとに初期化

  // O(V)
  // 残余グラフ上でsからの最短距離を計算する。tに辿り着けなかったらfalseを返す。
  bool bfs(int s, int t) {
    dist.assign(n, -1);
    std::queue<int> que;
    dist[s] = 0;
    que.push(s);
    while (!que.empty()) {
      int v = que.front();
      que.pop();
      for (auto &[nv, _, nc] : edges[v])
        if (nc > 0 && dist[nv] < 0) {
          dist[nv] = dist[v] + 1;
          que.push(nv);
        }
    }
    return dist[t] != -1;
  }

  // 逆辺をたどる。
  // limit: 現時点で流せる最大量
  // 1ステップでvから流せる量をresにまとめることで、DFSが一回で済む。
  T dfs(int to, int s, T limit) {
    if (to == s) { return limit; }
    // s - ... - from - to - ... - t

    T res = 0;
    // 無駄な辺を何度も調べないために、各頂点に既に調べた辺を持たせる。(start)
    // 各頂点でdfsを続きからできるようにするイメージ
    for (int &i = start[to]; i < static_cast<int>(edges[to].size()); ++i) {
      auto &[from, revidx, revcap] = edges[to][i];
      auto &[_to, _revidx, cap] = edges[from][revidx];
      // 距離が増加する向きの辺のみ調べる。
      if (cap > 0 && dist[from] == dist[to] - 1) {
        if (T d = dfs(from, s, min(limit, cap)); d > 0) {
          cap -= d;
          revcap += d;
          res += d;
          limit -= d;
          // limitは1頂点から複数頂点へ流す場合があるので必要。(dfsで流しているので流せた場合にはパスが構築され残余グラフも更新されるため、複数頂点へ流してもOK)
          if (limit == 0) { break; }
        }
      }
    }
    return res;
  }

  // 参考: FordFulkerson
  // 貪欲法+逆辺による押し戻しにより最大流問題を解くアルゴリズム O(max_flow * E)
  // std::vector<int> used;
  // int check = 1;
  // T dfs(int from, int t, T flow) {
  //   if (from == t) { return flow; }
  //   used[from] = check;
  //   for (int i = 0; i < static_cast<int>(edges[from].size()); ++i) {
  //     auto &[to, revidx, cap] = edges[from][i];
  //     auto &[_from, _revidx, revcap] = edges[to][revidx];
  //     if (cap > 0 && used[to] != check) {
  //       T d = dfs(to, t, min(flow, cap));
  //       if (d > 0) {
  //         // 流せるなら流す
  //         // 流した場合、辺の容量は減り、逆辺の容量は増える
  //         cap -= d;
  //         revcap += d;
  //         return d;
  //       }
  //     }
  //   }
  //   return 0;
  // }
  // T max_flow(int s, int t) {
  //   T flow = 0;
  //   while (true) {
  //     T f = dfs(s, t, std::numeric_limits<T>::max());
  //     if (f == 0) break;
  //     flow += f;
  //     ++check;
  //   }
  //   return flow;
  // }
};
