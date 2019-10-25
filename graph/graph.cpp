// 説明: ここのライブラリのグラフは大体これで構築したものを使う。


struct Edge {
  int src, dst, cost;
  Edge(int s, int d, int c = 1) : src(s), dst(d), cost(c) {}
  bool operator<(const Edge &e){ return cost < e.cost;}
};
using Edges = vector<Edge>;
using Graph = vector<Edges>;
