// 説明: s-tへの最大流を求める。
// TODO: しっかり理解してそらで書けるようにする。

// Dinic O(|E||V|^2)
struct Edge { int dst; long long cap; int rev; };
using Edges = vector<Edge>;
using Graph = vector<Edges>;

struct Dinic {
  int n, s, t;
  vector<int> level, iter;
  Graph g;

  Dinic(int n) : g(n), n(n) {}
  void add_edge(int src, int dst, long long cap) {
    g[src].push_back((Edge){ dst, cap, (int)g[dst].size()  });
    g[dst].push_back((Edge){ src, 0  , (int)g[src].size()-1});
  }

  void bfs(int s) {
    level.assign(n, -1);
    queue<int> que;
    level[s] = 0;
    que.push(s);
    while(!que.empty()){
      int v = que.front(); que.pop();
      for(Edge e : g[v]) {
        if(e.cap > 0 && level[e.dst] < 0){
          level[e.dst] = level[v] + 1;
          que.push(e.dst);
        }
      }
    }
  }

  long long dfs(int v,int t,long long f){
    if(v==t) return f;
    for(int &i = iter[v]; i < (int)g[v].size(); ++i){
      Edge &e = g[v][i];
      if(e.cap > 0 && level[v] < level[e.dst]){
        long long d = dfs(e.dst, t, min(f, e.cap));
        if(d > 0){
          e.cap -= d;
          g[e.dst][e.rev].cap += d;
          return d;
        }
      }
    }
    return 0;
  }

  long long max_flow(int s,int t) {
    long long flow = 0;
    while(true){
      bfs(s);
      if(level[t] < 0) return flow;
      iter.assign(n,0);
      long long f = 0;
      while((f = dfs(s, t, INF)) > 0){
        flow += f;
      }
    }
  }
};
