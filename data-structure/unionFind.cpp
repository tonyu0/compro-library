#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

// 説明: 集合の大きさ・重みを取れる実装

struct UnionFind
{
  vector<int> par, weight;
  UnionFind(int n) : par(n, -1), weight(n) {}
  void unite(int x, int y, int w = 0)
  {
    // w += get_weight(x); w -= get_weight(y);
    x = find(x);
    y = find(y);
    if (x == y)
      return;
    if (par[x] > par[y])
      swap(x, y), w = -w;
    par[x] += par[y];
    par[y] = x;
    // weight[y] = w;
    return;
  }
  bool same(int x, int y) { return find(x) == find(y); }
  int find(int x)
  {
    if (par[x] < 0)
      return x;
    int r = find(par[x]);
    // weight[x] += get_weight(par[x]);
    return par[x] = r;
  }
  int sizeOf(int x) { return -par[find(x)]; }
  // int get_weight(int x) { find(x); return weight[x]; }
  // int diff(int x, int y) { return get_weight(y) - get_weight(x); }
};

int n, q, t, x, y;
// http://poj.org/problem?id=3723
// 矛盾性の判定

int main()
{
  scanf("%d %d", &n, &q);
  UnionFind uf(n * 3);
  int ans = 0;
  for (int i = 0; i < q; ++i)
  {
    scanf("%d %d %d", &t, &x, &y);
    --x, --y;
    // invalid query
    if (x < 0 || n <= x || y < 0 || n <= y)
    {
      ++ans;
      continue;
    }

    if (t == 1)
    {
      // x -> yもしくはy -> xの関係が起こっていないか。
      if (uf.same(x, n + y) || uf.same(x, y + n * 2))
      {
        ++ans;
      }
      else
      {
        uf.unite(x, y);                 // x & y are A
        uf.unite(x + n, y + n);         // x & y are B
        uf.unite(x + n * 2, y + n * 2); // x & y are C
      }
    }
    else
    {
      // xとyが同じ関係、 y -> xと言う逆方向の関係が起こっていないか。
      if (uf.same(x, y) || uf.same(x, y + n * 2))
      {
        ++ans;
      }
      else
      {
        uf.unite(x, y + n);         // x -> y, A -> B
        uf.unite(x + n, y + n * 2); // x -> y, B -> C
        uf.unite(x + n * 2, y);     // x -> y, C -> A
      }
    }
  }
  printf("%d\n", ans);
  return 0;
}
