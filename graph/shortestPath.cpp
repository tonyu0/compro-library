#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <queue>
#include <set>
#include <vector>
using namespace std;
using ll = int64_t;
#define rep(i, j, n) for (int i = j; i < (int)n; ++i)
#define rrep(i, j, n) for (int i = (int)n - 1; j <= i; --i)

#define dbg(x) cerr << __LINE__ << ": " << #x << " = " << (x) << '\n'
template <typename T>
ostream& operator<<(ostream& os, vector<T>& a) {
  for (int i = 0; i < (int)a.size(); ++i)
    os << a[i] << " \n"[i == (int)a.size() - 1];
  return os;
}

constexpr ll MOD = 1000000007;
constexpr int INF = 0x3f3f3f3f;
constexpr ll INFL = 0x3f3f3f3f3f3f3f3fLL;

template <bool directed = false>
class shortest_path {
  using P = pair<int, ll>;

  int n;
  vector<vector<P>> graph;

public:
  shortest_path(int n) : n(n), graph(n) {}
  void add_edge(int u, int v, ll c) {
    graph[u].emplace_back(v, c);
    if (!directed) graph[v].emplace_back(u, c);
  }

  bool bellman_ford(int s, vector<long long>& dist) {
    int n = graph.size();
    dist.assign(n, INF);
    dist[s] = 0;

    for (int i = 0; i < n; ++i) {
      bool update = false;
      for (int v = 0; v < n; ++v) {
        for (auto [nv, cost] : graph[v]) {
          if (dist[v] != INF && dist[nv] > dist[v] + cost) {
            dist[nv] = dist[v] + cost;
            update = true;

            if (i == n - 1) {
              // negative cycle
              return true;
            }
          }
        }
      }
      if (!update) break;
    }
    return false;
  }

  vector<ll> dijkstra(int s) {
    vector<ll> dist(n, 1LL << 60);
    dist[s] = 0;
    priority_queue<P, vector<P>, greater<P>> pq;
    pq.emplace(0, s);
    while (!pq.empty()) {
      auto [d, v] = pq.top();
      pq.pop();
      if (dist[v] < d) continue;
      for (auto [nv, cost] : graph[v]) {
        if (dist[nv] > dist[v] + cost) {
          dist[nv] = dist[v] + cost;
          pq.emplace(dist[nv], nv);
        }
      }
    }
    return dist;
  }
};