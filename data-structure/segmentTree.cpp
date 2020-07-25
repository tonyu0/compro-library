#include <iostream>
#include <vector>
using namespace std;
// 説明:
// build O(n), query O(logn)
// SegmentTree(size, I, getFunc, updateFunc)
template <typename T, class F = function<T(const T &, const T &)>>
class SegmentTree
{
public:
  SegmentTree(int n_, T i, F f, F g) : I(i), get(f), upd(g)
  {
    for (n = 1; n < n_; n *= 2)
      ;
    data.assign(n * 2 - 1, I);
  }

  void update(int i, T x)
  {
    i += n - 1;
    data[i] = upd(data[i], x);
    while (i)
    {
      i = (i - 1) / 2;
      data[i] = get(data[i * 2 + 1], data[i * 2 + 2]);
    }
  }
  // [a,b)
  T query(int a, int b)
  {
    return query(a, b, 0, 0, n);
  }

private:
  int n;
  vector<T> data;
  T I;
  F get, upd;

  T query(int a, int b, int i, int l, int r)
  {
    if (b <= l || r <= a)
      return I;
    if (a <= l && r <= b)
      return data[i];
    int vl = query(a, b, i * 2 + 1, l, (l + r) / 2);
    int vr = query(a, b, i * 2 + 2, (l + r) / 2, r);
    return get(vl, vr);
  }
};
