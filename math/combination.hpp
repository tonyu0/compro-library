#pragma once
#include <vector>

// templateにしてmodintを渡せるようにするか、modintにcominationを追加する(配列持つのは容量食うので、このクラスに渡せるようにするといい)
class combination {
public:
  combination(int n, uint64_t m)
    : mod(m), fact(n + 1), inv(n + 1), finv(n + 1) {
    fact[0] = finv[0] = inv[1] = 1;
    // i * inv[i] = 1 mod p.
    // p = p / i * i + p % i
    // => p / i * i + p % i = 0 mod p
    // => (p / i * -inv[p % i]) * i = 1 mod p
    for (int i = 2; i <= n; ++i) inv[i] = mod - inv[mod % i] * (mod / i) % mod;
    for (int i = 1; i <= n; ++i) {
      fact[i] = fact[i - 1] * i % mod;
      finv[i] = finv[i - 1] * inv[i] % mod;
    }
  }
  uint64_t operator()(int n, int k) const {
    if (n < k || n < 0 || k < 0) return 0;
    return fact[n] * finv[n - k] % mod * finv[k] % mod;
  }
  uint64_t modpow(uint64_t x, uint64_t e) {
    uint64_t res = 1;
    while (e > 0) {
      if (e & 1) res = res * x % mod;
      x = x * x % mod;
      e >>= 1;
    }
    return res;
  }

private:
  uint64_t mod;
  std::vector<uint64_t> fact, inv, finv;
};