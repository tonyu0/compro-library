#include <vector>

template <long long mod = 1000000007>
class modint {
public:
  long long x;
  constexpr modint(const long long x = 0) noexcept : x(x % mod) {}
  constexpr long long &value() noexcept { return x; }
  constexpr const long long &value() const noexcept { return x; }
  constexpr modint operator+(const modint rhs) const noexcept {
    return modint(*this) += rhs;
  }
  constexpr modint operator-(const modint rhs) const noexcept {
    return modint(*this) -= rhs;
  }
  constexpr modint operator*(const modint rhs) const noexcept {
    return modint(*this) *= rhs;
  }
  constexpr modint operator/(const modint rhs) const noexcept {
    return modint(*this) /= rhs;
  }
  constexpr modint &operator+=(const modint rhs) noexcept {
    if ((x += rhs.x) >= mod) x -= mod;
    return *this;
  }
  constexpr modint &operator-=(const modint rhs) noexcept {
    if ((x += mod - rhs.x) >= mod) x -= mod;
    return *this;
  }
  constexpr modint &operator*=(const modint rhs) noexcept {
    x = x * rhs.x % mod;
    return *this;
  }
  constexpr modint &operator/=(const modint rhs) noexcept {
    *this *= rhs.inverse();
    return *this;
  }
  constexpr bool operator==(const modint &rhs) const { return x == rhs.x; }
  constexpr bool operator!=(const modint &rhs) const { return x != rhs.x; }
  modint inverse() const {
    long long a = x, b = mod, u = 1, v = 0, t;
    while (b > 0) {
      t = a / b;
      swap(a -= t * b, b);
      swap(u -= t * v, v);
    }
    return modint(u + mod);
  }

  modint pow(long long exp) const {
    modint res(1), mul(x);
    while (exp > 0) {
      if (exp & 1) res *= mul;
      mul *= mul;
      exp >>= 1;
    }
    return res;
  }
  operator long long() const { return x; }
  friend ostream &operator<<(ostream &os, const modint &rhs) {
    return os << rhs.x;
  }
  friend istream &operator>>(istream &is, modint &a) {
    long long t;
    is >> t;
    a = modint<mod>(t);
    return is;
  }
};

template <long long mod = 1000000007>
class combination {
  using mint = modint<mod>;

public:
  constexpr combination(size_t n) : fac(n + 1), inv(n + 1), inv_fac(n + 1) {
    fac[0] = 1;
    inv_fac[0] = 1;
    inv[1] = 1;
    // i * inv[i] = 1 mod p.
    // p = p / i * i + p % i
    // => p / i * i + p % i = 0 mod p
    // => (p / i * -inv[p % i]) * i = 1 mod p
    for (size_t i = 2; i <= n; ++i) { inv[i] -= inv[mod % i] * mint(mod / i); }
    for (size_t i = 1; i <= n; ++i) {
      fac[i] = fac[i - 1] * mint(i);
      inv_fac[i] = inv_fac[i - 1] * inv[i];
    }
  }

  mint operator()(size_t n, size_t k) {
    if (n < k) { return 0; }
    return fac[n] * inv_fac[n - k] * inv_fac[k];
  }
  mint factrial(size_t n) { return fac[n]; }

private:
  std::vector<mint> fac, inv, inv_fac;
};