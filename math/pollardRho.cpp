#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <string>
#include <vector>

using namespace std;
using ll = long long;
#define rep(i, j, n) for (int i = j; i < (n); ++i)
#define rrep(i, j, n) for (int i = (n)-1; j <= i; --i)

template <typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T>& a) {
  os << "{";
  for (size_t i = 0; i < a.size(); ++i) os << (i > 0 ? "," : "") << a[i];
  return os << "}";
}

[[maybe_unused]] constexpr ll MOD = 1e9 + 7;
[[maybe_unused]] constexpr int INF = 0x3f3f3f3f;
[[maybe_unused]] constexpr ll INFL = 0x3f3f3f3f3f3f3f3fLL;

long long modpow(long long x, long long e, long long m) {
  long long res = 1;
  while (e > 0) {
    if (e & 1) res = (__uint128_t)res * x % m;
    x = (__uint128_t)x * x % m;
    e >>= 1;
  }
  return res;
}
long long f(long long x, long long c, long long n) {
  return ((__uint128_t)x * x + c) % n;
}

bool is_prime(long long n) {
  if (n == 2) return true;
  if (n == 0 || n == 1 || n % 2 == 0) return false;
  long long d = n - 1;
  d /= d & -d;
  // n - 1 = d * 2^xとなる奇数d

  // vector<long long> a;
  // if (n < (1LL << 32)) a = {2, 7, 61};
  // else a = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};

  for (long long a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
    if (n <= a) break;
    long long t = d;
    long long y = modpow(a, t, n);
    // (1, n-1以外のもの)^2 mod nが1になったら合成数、をしたい
    // 最初から1はskip
    if (y == 1) continue;
    while (y != n - 1) {
      // このループ内に入るのは(y != 1 and y != -1 mod
      // n)かつnが素数ならa^(n-1)=1 mod nであるから t = n-1になる前にy = -1 mod
      // nになる必要がある。
      // それ以外の数字を二乗して直接1になってしまったら、それがmod
      // nの平方根であることになり、nは素数ではないことになる。
      y = (__uint128_t)y * y % n;
      if (y == 1 || t == n - 1) return false;
      t <<= 1;
    }
  }
  return true;
}

long long find_factor(long long n) {
  if (n % 2 == 0) return 2;
  if (is_prime(n)) return n;
  long long m = 1LL << ((64 - __builtin_clzll(n)) / 8);
  for (long long c = 1; c < 99; ++c) {
    long long y = 2, r = 1, q = 1, g = 1, x, ys;
    while (g == 1) {
      x = y;
      for (int i = 0; i < r; ++i) y = f(y, 1, n);
      long long k = 0;
      while (k < r && g == 1) {
        ys = y;
        for (int j = 0; j < min(m, r - k); ++j) {
          y = f(y, c, n);
          q = (__uint128_t)q * abs(x - y) % n;
        }
        g = gcd(q, n);
        k += m;
      }
      r <<= 1;
    }
    if (g == n) {
      g = 1;
      while (g == 1) {
        ys = f(ys, c, n);
        g = gcd(abs(x - ys), n);
      }
    }

    if (g < n) {
      if (is_prime(g))
        return g;
      else if (is_prime(n / g))
        return n / g;
      return find_factor(g);
    }
  }
  return 0;
}

long long find_factor_floyd(long long n, long long c = 1) {
  if (n % 2 == 0) return 2;
  if (is_prime(n)) return n;

  long long x = 2, y = 2, d = 1;
  while (d == 1) {
    x = f(x, c, n);
    y = f(f(y, c, n), c, n);
    d = gcd(abs(x - y), n);
    if (1 < d && d < n) break;
  }
  if (d == n) return find_factor_floyd(n, c + 1);
  return find_factor_floyd(d, c);
}

vector<long long> factorize(long long n) {
  vector<long long> fs;
  while (n > 1) {
    long long d = find_factor(n);
    fs.push_back(d);
    n /= d;
  }
  return fs;
}

// vector<long long> factorize(long long n) {
//   if (n == 1) return {};
//   long long x = find_factor(n);
//   if (x == n) return {x};
//   auto le = factorize(x);
//   auto ri = factorize(n / x);
//   le.insert(le.end(), ri.begin(), ri.end());
//   return le;
// }

int main() {
  cin.tie(0)->sync_with_stdio(0);
  int q;
  cin >> q;
  while (q--) {
    long long a;
    cin >> a;
    auto fs = factorize(a);
    sort(fs.begin(), fs.end());
    cout << fs.size() << " ";
    for (int i = 0; i < (int)fs.size(); ++i) cout << fs[i] << " ";
    cout << '\n';
  }
  return 0;
}
