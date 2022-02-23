#include <vector>

// 20211003 : verify : https://judge.yosupo.jp/submission/62290

template <typename T = int>
class Tree {
public:
  Tree(int size) : tree(size) {}

  void add_edge(int from, int to, T cost = 1, bool directed = false) {
    tree[from].emplace_back(to, cost);
    if (!directed) { tree[to].emplace_back(from, cost); }
  }

  tuple<T, int, int> get_diameter() {
    auto [_, end1] = get_diameter_inner(0, -1);
    auto [diameter, end2] = get_diameter_inner(end1, -1);
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

    for (auto &[nv, _] : tree[from]) {
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
  vector<T> shortest_path_bfs(int s) {
    vector<T> dist(tree.size(), tree.size());
    queue<int> q;
    q.push(s);
    dist[s] = 0;
    while (q.size() > 0) {
      int v = q.front();
      q.pop();
      for (auto [nv, nc] : tree[v]) {
        if (dist[nv] > dist[v] + nc) {
          dist[nv] = dist[v] + nc;
          q.push(nv);
        }
      }
    }
    return dist;
  }
  void shortest_path_dfs(int v, int p, vector<T> &dist) {
    T res = 0;
    for (auto [nv, nc] : tree[v]) {
      if (nv == p) { continue; }
      shortest_path_dfs(nv, v, dist);
      res = max(res, dist[nv] + 1);
    }
    dist[v] = res;
  }

private:
  pair<T, int> get_diameter_inner(int cur, int parent) {
    pair<T, int> res = {0, cur};
    for (auto &[nv, nc] : tree[cur]) {
      if (nv == parent) { continue; }
      auto [child_cost, child_vert] = get_diameter_inner(nv, cur);
      if (res.first < child_cost + nc) {
        res.first = child_cost + nc;
        res.second = child_vert;
      }
    }
    return res;
  }

  std::vector<std::vector<pair<int, T>>> tree;
};