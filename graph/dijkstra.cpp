// TODO: 問題を追加する。

// O(|V|+|E|)log|V|)
void dijkstra(int s, Graph &g, vector<long long> &dist) {
  using Info = pair<long long, int>;
  // init
  dist.assign(g.size(), INF);
  dist[s] = 0;
  priority_queue<Info, vector<Info>, greater<Info> > pq;
  pq.emplace(0, s);
  //
  // main
  while(!pq.empty()){
    long long d = pq.top().first;
    int v = pq.top().second;
    pq.pop();
    if(dist[v] < d) continue;
    for(Edge e : g[v]){
      if(dist[e.dst] > dist[v] + e.cost){
        dist[e.dst] = dist[v] + e.cost;
        pq.emplace(dist[e.dst], e.dst);
      }
    }
  }
  //
}

int main() {
  return 0;
}
