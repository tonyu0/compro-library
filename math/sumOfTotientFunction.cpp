#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

long long sum_of_totient_function(long long N) {
  // S(n) = n * (n + 1) / 2 - \sum_{i=2}^{i=n} S(n / i)
  // where S(x) = \sum_{i=1}^{i=x} φ(i)

  long long mod = 998244353;
  int K = (int)pow((double)N, 2. / 3.);
  // lower[i] := \sum_{n=1}^{n=i} φ(i), use sieve and cumsum
  vector<long long> lower(K);
  iota(lower.begin(), lower.end(), 0);
  for (int i = 2; i < K; ++i)
    if (lower[i] == i)
      for (int j = i; j < K; j += i) lower[j] -= lower[j] / i;
  for (int i = 0; i < K - 1; ++i) (lower[i + 1] += lower[i]) %= mod;

  // upper[i] := \sum_{n=1}^{n=[N/i]} φ(n)
  // upper[i] = S(N / i)
  vector<long long> upper(N / K + 1);
  for (int i = 1; i < upper.size(); ++i)
    upper[i] = N / i % mod * (N / i % mod + 1) / 2 % mod;

  for (int i = upper.size() - 1; i >= 1; --i) {
    long long n;
    for (n = 2; N / (n * i) != N / ((n + 1) * i); ++n)
      (upper[i] -= n * i < upper.size() ? upper[n * i] : lower[N / (n * i)]) %=
        mod;

    for (long long m = N / (n * i); m >= 1; --m)
      (upper[i] -= (N / (m * i) - (N / ((m + 1) * i))) *
                   (m < K ? lower[m] : upper[N / m])) %= mod;
    upper[i] = (upper[i] + mod) % mod;
  }
  return upper[1];
}