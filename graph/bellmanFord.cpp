// TODO: https://atcoder.jp/contests/abc061/tasks/abc061_d

// O(|V||E|)
bool bellman_ford(int s, Graph &g, vector<long long> &dist) {
  int n = g.size();
  dist.assign(n, INF);
  dist[s] = 0;

  for(int i = 0; i < n*2; ++i) {
    bool update = false;
    for(int v = 0; v < n; ++v) {
      for(Edge e : g[v]) {
        if(dist[e.src] != INF && dist[e.dst] > dist[e.src] + e.cost) {
          dist[e.dst] = dist[e.src] + e.cost;
          update = true;

          if(i >= n && e.dst == n-1) {
            // negative cycle
            return true;
          }
        }
      }
    }
    if(!update) break;
  }
  return false;
}
