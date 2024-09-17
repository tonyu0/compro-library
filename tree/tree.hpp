#include <queue>
#include <vector>

// 20211003 : verify : https://judge.yosupo.jp/submission/62290

// basic algorithms for tree graph
// lca: build O(NlogN), query O(logN)
template <typename T = int>
class Tree {
public:
  Tree(int size) : tree(size) {}

  void add_edge(int from, int to, T cost = 1) {
    tree[from].emplace_back(to, cost);
    tree[to].emplace_back(from, cost);
  }

  std::tuple<T, int, int> get_diameter() {
    const auto &[_, end1] = get_diameter_inner(0, -1);
    const auto &[diameter, end2] = get_diameter_inner(end1, -1);
    return {diameter, end1, end2};
  }
  // Construct the path between given endpoint
  void construct_path(int from, int &to, int parent, std::vector<int> &path) {
    // 端に辿り着き、辿り着いたフラグを立てる
    if (from == to) {
      path.push_back(to);
      to = -1;
      return;
    }

    for (const auto &[nv, _] : tree[from]) {
      if (nv == parent) { continue; }
      construct_path(nv, to, from, path);
      // 潜った先にtoがあるなら今見てるのはパス中である
      if (to == -1) {
        path.push_back(from);
        return;
      }
    }
  }
  // 単一始点 to 全点
  std::vector<T> shortest_path_bfs(int s) {
    std::vector<T> dist(tree.size(), tree.size());
    std::queue<int> q;
    q.push(s);
    dist[s] = 0;
    while (q.size() > 0) {
      int v = q.front();
      q.pop();
      for (const auto &[nv, nc] : tree[v]) {
        if (dist[nv] > dist[v] + nc) {
          dist[nv] = dist[v] + nc;
          q.push(nv);
        }
      }
    }
    return dist;
  }
  void shortest_path_dfs(int v, int p, std::vector<T> &dist) {
    T res = 0;
    for (const auto &[nv, nc] : tree[v]) {
      if (nv == p) { continue; }
      shortest_path_dfs(nv, v, dist);
      res = max(res, dist[nv] + 1);
    }
    dist[v] = res;
  }

  /* find LCA using binary lifting */
  void build_lca() {
    int n = tree.size();
    cost.resize(n);
    depth.resize(n);
    logn = 0;
    while ((1 << logn) < n) ++logn;
    par.resize(n + 1, std::vector<int>(logn));

    calc_params_dfs();

    for (int i = 0; i < logn - 1; ++i) {
      for (int v = 0; v < (int)tree.size(); ++v) {
        par[v][i + 1] = par[par[v][i]][i];
      }
    }
  }

  /* call build_lca() before use */
  int get_lca(int x, int y) {
    if (depth[x] > depth[y]) swap(x, y);

    for (int i = logn - 1; i >= 0; --i) {
      if (((depth[y] - depth[x]) >> i) & 1) y = par[y][i];
    }
    if (x == y) return y;

    for (int i = logn - 1; i >= 0; --i) {
      int nx = par[x][i], ny = par[y][i];
      if (nx != ny) x = nx, y = ny;
    }
    return par[y][0];
  }

  /* call build_lca() before use */
  T get_length(int x, int y) {
    int z = get_lca(x, y);
    return cost[x] + cost[y] - cost[z] * 2;
  }

private:
  std::pair<T, int> get_diameter_inner(int cur, int parent) {
    std::pair<T, int> res = {0, cur};
    for (const auto &[nv, nc] : tree[cur]) {
      if (nv == parent) { continue; }
      const auto &[child_cost, child_vert] = get_diameter_inner(nv, cur);
      if (res.first < child_cost + nc) {
        res.first = child_cost + nc;
        res.second = child_vert;
      }
    }
    return res;
  }

  // calc depth and cost from root(0th node)
  void calc_params_dfs(int v = 0, int p = 0, int d = 0, T c = 0) {
    par[v][0] = p;
    depth[v] = d;
    cost[v] = c;

    for (const auto &[nv, nc] : tree[v]) {
      if (nv == p) continue;
      calc_params_dfs(nv, v, d + 1, cost[v] + nc);
    }
  }

  int logn;
  std::vector<int> depth;
  std::vector<T> cost;
  std::vector<std::vector<int>> par;
  std::vector<std::vector<std::pair<int, T>>> tree;
};