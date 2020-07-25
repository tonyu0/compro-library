#include <iostream>
#include <cmath>
#include <unordered_map>
using namespace std;
using ll = long long;
// 説明: g^x ≡ y (mod p)をO(√p)で解く。
// Baby step:
// m = √p
// x^0, x^1, x^2, ... (baby step)
// それぞれのbaby stepを
// n^e (mod p)

ll mod_pow(ll n, ll e, ll p)
{
  ll res = 1;
  while (e > 0)
  {
    if (e & 1)
      res = res * n % p;
    n = n * n % p;
    e >>= 1;
  }
  return res;
}

// pを法としたgの冪乗には長さp-1の周期性がある。
// g^x % p = hを満たすxを求める。
// x = i*m + j (m = √p)として上式を変形すると
// g^(i*m+j) ≡ h (mod p) pは素数なので両辺にg^((-m)*i)をかけて
// g^j ≡ h(g^(-m))^i
// baby stepでj, giant stepで(g^(-m))^iを総当たり
int babystep_giantstep(int g, int h, int p)
{
  int m = ceil(sqrt(p));
  unordered_map<int, int> table;

  // baby step
  // g^i % p(i = 0 ~ √p)を求める
  ll baby = 1;
  for (int j = 0; j < m; ++j)
  {
    table[baby] = j;
    baby = (baby * g) % p;
  }

  // giant step
  ll gm = mod_pow(g, p - 1 - m, p);
  ll giant = h;
  for (int i = 0; i < m; ++i)
  {
    auto itr = table.find(giant);
    if (itr != table.end())
      return i * m + itr->second;
    giant = (giant * gm) % p;
  }
  return -1;
}

// https://atcoder.jp/contests/arc042/tasks/arc042_d
int main()
{
  ll X, P, A, B;
  cin >> X >> P >> A >> B;
  // X^i = Y(mod P)を考える。
  // iを全探索すると間に合わないのでYを固定して離散対数問題を解く。
  // Yは周期P-1なので、Pがでかいと結局間に合わないので、工夫する。
  if (B - A + 1 > P - 1)
  {
    cout << 1;
  }
  else if (B - A < 100000) // 試すiが小さいとBSGSがTLEするのでここは愚直にやる。
  {
    int ans = P;
    // X^A (mod P)
    int t = mod_pow(X, A, P);
    for (int i = 0; i < B - A + 1; ++i)
    {
      ans = min(ans, t);
      t = t * X % P;
    }
    cout << ans;
  }
  else
  {
    A %= P - 1;
    B %= P - 1;
    // BSGSを使う。
    // X^i = h(mod P)
    for (int h = 0; h < P; ++h)
    {
      int res = babystep_giantstep(X, h, P);
      if (A <= res && res <= B)
      {
        cout << h;
        break;
      }
    }
  }
  cout << '\n';
  return 0;
}