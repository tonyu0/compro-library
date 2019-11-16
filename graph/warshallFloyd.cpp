#include <iostream>
#include <vector>
using namespace std;
// 説明: 全点間最短距離。

// O(V^3)
void warshall_floyd(int n, vector<vector<int>> &g)
{
  for (int k = 0; k < n; ++k)
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j)
        g[i][j] = min(g[i][j], g[i][k] + g[k][j]);
}
