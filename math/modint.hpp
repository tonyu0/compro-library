
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
    int a = x, b = mod, u = 1, v = 0, t;
    while (b > 0) {
      t = a / b;
      swap(a -= t * b, b);
      swap(u -= t * v, v);
    }
    return modint(u);
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
  // 変換演算子
  operator long long() const { return x; }
  // modintを右オペランドにした<<演算子の非メンバ関数演算子オーバーロード
  // friendによりostreamからmodint.xにアクセス可能
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