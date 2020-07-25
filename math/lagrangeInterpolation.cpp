#include <iostream>
#include <vector>
using namespace std;
// 説明:
// n個の点情報(x, y)を受け取り、n-1次のラグランジュ多項式上でのx = tの値を返す関数
// 計算量: O(n^2)

// x, yの値を渡すとラグランジュ補間
template <typename T>
T LagrangeInterpolation(vector<T> &x, vector<T> &y, T t)
{
  T sum = 0;
  int n = (int)x.size();

  for (int i = 0; i < n; i++)
  {
    T num = y[i], denom = 1;
    for (int j = 0; j < n; j++)
    {
      if (i == j)
        continue;
      num = num * (t - x[j]);
      denom = denom * (x[i] - x[j]);
    }
    T prod = num / denom;
    sum += prod;
  }

  return sum;
}

// 解けてない
// https://atcoder.jp/contests/arc033/tasks/arc033_4
void ARC033D()
{
  int n;
  cin >> n;
  long p, t;
  vector<long> x(n + 1), y(n + 1);
  for (int i = 0; i <= n; ++i)
  {
    cin >> p;
    x[i] = i, y[i] = p;
  }
  cin >> t;
  cout << LagrangeInterpolation<long>(x, y, t) << '\n';
}

int main()
{
  ARC033D();
  return 0;
}