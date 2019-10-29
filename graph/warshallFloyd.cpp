// 説明: 全点間最短距離, もう少し早くしたJohnsonというものもある。
// TODO: https://atcoder.jp/contests/abc079/tasks/abc079_d  https://atcoder.jp/contests/abc143/tasks/abc143_e

// O(V^3)
void warshall_floyd(int n, vector<vector<int> > &g) {
  for(int k = 0; k < n; ++k)
    for(int i = 0; i < n; ++i)
      for(int j = 0; j < n; ++j) g[i][j] = min(g[i][j], g[i][k] + g[k][j]);
}
