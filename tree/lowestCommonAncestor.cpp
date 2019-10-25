// 説明: ダブリングを用いた最小共通祖先の取得。木上の2点間の距離を求めることができる。
// TODO: depthやcostが計算時に必要になることが多いのでgetterを作る。

// 要include graph
// build O(NlogN), query O(logN)
template<typename T>
struct LCA {
  vector<T> cost;
  vector<int> depth;
  vector<vector<int>> par;
  Graph g;
  int logn;

  LCA(int n, Graph g) : cost(n), depth(n), g(g) {
    logn = 0;
    while((1<<logn) < n) ++logn;

    par.assign(n+1, vector<int>(logn, n));
    dfs(0, -1, 0, 0);

    for(int i = 0; i < logn-1; ++i) {
      for(int v = 0; v < n; ++v) {
        par[v][i+1] = par[par[v][i]][i];
      }
    }
  }

  void dfs(int v, int p, int d, T c) {
    if(p != -1) par[v][0] = p;
    depth[v] = d;
    cost[v] = c;

    for(Edge e : g[v]) {
      if(e.dst == p) continue;
      dfs(e.dst, v, d+1, cost[v] + e.cost);
    }
  }

  int get_lca(int x, int y) {
    if(depth[x] > depth[y]) swap(x, y);

    for(int i = logn-1; i >= 0; --i) {
      if(((depth[y] - depth[x])>>i) & 1) y = par[y][i];
    }
    if(x == y) return y;

    for(int i = logn-1; i >= 0; --i) {
      int nx = par[x][i], ny = par[y][i];
      if(nx != ny) x = nx, y = ny; 
    }
    return par[y][0];
  }

  T get_length(int x, int y) {
    int z = get_lca(x, y);
    return cost[x] + cost[y] - cost[z] * 2;
  }
};
