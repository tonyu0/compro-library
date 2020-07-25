#include <iostream>
#include <vector>
using namespace std;

// 説明: セグメントツリーを２本用意することで遅延評価を実現する。
// TODO: 抽象化する

struct LazySegmentTree
{
  vector<long long> data, lazy;
  int n;

  LazySegmentTree(int n_)
  {
    for (n = 1; n < n_; n *= 2)
      ;
    data.assign(n * 2 - 1, 0);
    lazy.assign(n * 2 - 1, 0);
  }

  void propagate(int i, int l, int r)
  {

    if (lazy[i] != 0)
    {
      data[i] += lazy[i];

      if (r - l > 1)
      {
        lazy[i * 2 + 1] += lazy[i] / 2;
        lazy[i * 2 + 2] += lazy[i] / 2;
      }
    }

    lazy[i] = 0;
  }

  void update(int a, int b, long long x, int i = 0, int l = 0, int r = -1)
  {
    if (r < 0)
      r = n;

    propagate(i, l, r);

    if (b <= l || r <= a)
      return;
    if (a <= l && r <= b)
    {
      lazy[i] += (r - l) * x;
      propagate(i, l, r);
    }
    else
    {
      update(a, b, x, i * 2 + 1, l, (l + r) / 2);
      update(a, b, x, i * 2 + 2, (l + r) / 2, r);
      data[i] = data[i * 2 + 1] + data[i * 2 + 2];
    }
  }

  long long query(int a, int b, int i = 0, int l = 0, int r = -1)
  {
    if (r < 0)
      r = n;
    if (b <= l || r <= a)
      return 0;

    propagate(i, l, r);
    if (a <= l && r <= b)
      return data[i];
    long long vl = query(a, b, i * 2 + 1, l, (l + r) / 2);
    long long vr = query(a, b, i * 2 + 2, (l + r) / 2, r);
    return vl + vr;
  }
};
