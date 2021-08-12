#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

class UndirectedGraph {
public:
  UndirectedGraph(int n) : graph(n), ord(n), lowlink(n), used(n, false) {}
  void add_edge(int u, int v, int c = 1) {
    graph[u].emplace_back(v, c);
    graph[v].emplace_back(u, c);
  }
  // e(u,v)が橋　<=> ord[u] < low[v]
  // 頂点vが関節点 <=> ord[u] <= low[v]となるvの子uが存在
  // DFS木の子方向から親方向へアクセスできるなら、今見ている辺・頂点を消しても親ー子間の連結は保たれるイメージ
  void dfs(int v, int pv, int &k) {
    used[v] = true;
    ord[v] = lowlink[v] = k++; // 訪れた順
    bool is_articulation = false;
    for (auto [nv, nc] : graph[v]) {
      if (!used[nv]) {
        dfs(nv, v, k);
        lowlink[v] = min(lowlink[v], lowlink[nv]);

        is_articulation |= pv != -1 && ord[v] <= lowlink[nv];
        if (ord[v] < lowlink[nv]) bridges.emplace_back(minmax(v, nv));
      } else if (nv != pv) { // 後退辺だったら
        lowlink[v] = min(lowlink[v], ord[nv]);
      }
    }
    // DFS木の根　かつ　子が2以上
    is_articulation |= pv == -1 && graph[v].size() >= 2;
    if (is_articulation) articulation_points.push_back(v);
  }

  void build() {
    int k = 0;
    for (int i = 0; i < graph.size(); ++i) {
      if (!used[i]) dfs(i, -1, k);
    }
    built = true;
  }

  vector<pair<int, int>> get_bridges() {
    assert(built);
    return bridges;
  }
  vector<int> get_articulation_points() {
    assert(built);
    return articulation_points;
  }

private:
  vector<vector<pair<int, int>>> graph; // pair<int, int> = {to, cost};
  vector<int> ord;
  vector<int> lowlink;
  vector<bool> used;
  bool built = false;
  vector<pair<int, int>> bridges;
  vector<int> articulation_points;
};