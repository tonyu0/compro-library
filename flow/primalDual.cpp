// 説明: 最小費用流を求める。
// TODO: dinicもだけどグラフを共通化させる。verifyする。

struct Edge { int dst, cap, cost, rev; };
using Edges = vector<Edge>;
using Graph = vector<Edges>;

struct PrimalDual {
  int n;
  Graph g;
  vector<long long> dist;
  vector<int> h, prevv, preve;
  PrimalDual(int n) : n(n), g(n), prevv(n), preve(n) {}

  void add_edge(int src,int dst,int cap,int cost) {
    g[src].push_back((Edge){dst, cap,  cost, (int)g[dst].size()  });
    g[dst].push_back((Edge){src,   0, -cost, (int)g[src].size()-1});
  }

  void dijkstra(int s){
    dist.assign(n, INF);
    dist[s] = 0;
    priority_queue<pair<int,int>, vector<pair<int,int> >, greater<pair<int,int> > > pq;
    pq.emplace(0, s);
    while(!pq.empty()){
      pair<int,int> p = pq.top(); pq.pop();
      int v = p.second;
      if(dist[v] < p.first) continue;
      for(int i=0; i < (int)g[v].size(); ++i) {
        Edge e = g[v][i];
        if(e.cap > 0 && dist[e.dst] > dist[v] + e.cost + h[v] - h[e.dst]){
          dist[e.dst] = dist[v] + e.cost + h[v] - h[e.dst];
          prevv[e.dst] = v;
          preve[e.dst] = i;
          pq.emplace(dist[e.dst], e.dst);
        }
      }
    }
  }

  int min_cost_flow(int s,int t,int f){
    int res = 0;
    h.assign(n, 0);
    while(f > 0){
      dijkstra(s);
      if(dist[t]==INF) return -1;

      for(int i=0; i < n; ++i) h[i] += dist[i];

      int d = f;
      for(int v = t; v != s; v = prevv[v]){
        d = min(d, g[prevv[v]][preve[v]].cap);
      }
      f -= d;
      res += d * h[t];
      for(int v = t; v != s; v = prevv[v]){
        Edge &e = g[prevv[v]][preve[v]];
        e.cap -= d;
        g[v][e.rev].cap += d;
      }
    }
    return res;
  }
};
