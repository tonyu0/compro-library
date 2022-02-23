#include <vector>
using namespace std;

const int MAX = 200000;
void sieve_least_divisor(vector<int> &lst) {
  lst.assign(MAX, 0);
  for (int i = 2; i < MAX; ++i) lst[i] = i;
  for (int i = 2; i < MAX; ++i)
    for (long long j = 1ll * i * i; j < MAX; j += i)
      if (lst[j] == j) lst[j] = i;
}

struct Sieve {
  // 篩からのつなぎ技全般
  std::vector<int> primes;
  std::vector<bool> isPrime;
  std::vector<int> mu;
  std::vector<int> minFactor;

  Sieve(int n) : isPrime(n + 1, true), mu(n + 1, 1), minFactor(n + 1, -1) {
    isPrime[0] = isPrime[1] = false;
    minFactor[0] = minFactor[1] = 1;
    for (int i = 2; i <= n; ++i) {
      if (isPrime[i]) {
        primes.push_back(i);
        mu[i] = -1;
        minFactor[i] = i;
        for (int j = i * 2; j <= n; j += i) {
          isPrime[j] = false;
          if (j / i % i == 0) { // if i*j is divided by i^2 (squared number)
            mu[j] = 0;
          } else {
            mu[j] = -mu[j]; // mu sign depend on the number of divisors
          }

          if (minFactor[j] == -1) {
            minFactor[j] = i;
            // the first update
          }
        }
      }
    }
  }

  vector<pair<int, int>> PrimeFactorization(int n) {
    vector<pair<int, int>> res;
    while (n != 1) {
      int p = minFactor[n];
      int e = 0;
      while (n % p == 0) {
        ++e;
        n /= p;
      }
      res.emplace_back(p, e);
    }
    return res;
  }
  vector<int> EnumerateDivisors(int n) {
    vector<int> res({1});
    vector<pair<int, int>> pf = PrimeFactorization(n);
    for (auto &[p, e] : pf) {
      for (int i = 0; i < (int)res.size(); ++i) {
        int v = 1;
        for (int j = 0; j < e; ++j) {
          v *= p;
          res.push_back(res[i] * v);
        }
      }
    }
    return res;
  }

  // 用途: https://atcoder.jp/contests/abc206/submissions/26340980
  template <class T>
  void FastZeta(vector<T> &f) {
    int n = (int)f.size();
    for (int p = 2; p < n; ++p) {
      if (isPrime[p]) {
        for (int k = (n - 1) / p; k >= 1; --k) { f[k] += f[k * p]; }
      }
    }
  }

  template <class T>
  void FastMebius(vector<T> &F) {
    int n = (int)F.size();
    for (int p = 2; p < n; ++p) {
      if (isPrime[p]) {
        for (int k = 1; k * p < n; ++k) { F[k] -= F[k * p]; }
      }
    }
  }

  // 添字 GCD 畳み込み
  template <class T>
  vector<T> GcdConvolution(const vector<T> &f, const vector<T> &g) {
    int n = max(f.size(), g.size());
    vector<T> F(n, 0), G(n, 0), H(n);
    for (int i = 0; i < f.size(); ++i) F[i] = f[i];
    for (int i = 0; i < g.size(); ++i) G[i] = g[i];
    // f[i] := iに対しての何か
    // ↓　ゼータ変換
    // F[i] := iの倍数に対しての何か ←　この情報を畳み込む前に利用も可能
    // ↓　G[i]と各点積
    // H[i] := gcd(x, y)=iの倍数となるものに対しての何か
    // ↓　メビウス変換
    // h[i] := gcd(x, y)=iとなるものに対しての何か
    FastZeta(F);
    FastZeta(G);
    for (int i = 1; i < n; ++i) H[i] = F[i] * G[i];
    FastMebius(H);
    return H;
  }
};
