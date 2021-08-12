#include <vector>
// 参考: https://yosupo.hatenablog.com/entry/2015/03/27/025132
// O(K^2logN)
class Kitamasa {
public:
  // d: coefficient
  // dたちの線形結合でa_nを表したい。
  // 基本方針はダブリング
  // ダブリングの典型通り、まず写像をうまく定義していくつか先の項を表現してみる
  Kitamasa(std::vector<long long> const& d, long long const mod = 1000000007)
    : k(d.size()), d(d), mod(mod) {}
  // k項間漸化式にて,k個の係数をセットとして返す関数を考える。
  // f(k) = {初項k個}
  // f(n) = {x0,x1,x2,...,x_{k-1}} (n項目まで進ませる写像であると考えると良い)
  // こうやって定義すると、f(n)からf(n+1),f(n*2)が求まるので、ダブリングによりn項目が求まる。

  void add1(std::vector<long long>& x) {

    // a_n = x_0 * a_0 + x_1 * a_1 + ... + x_{k-1} * a_{k-1}
    // となるxのセットを考える。(f(n) = {x_0, x_1, ..., x_{k-1}}である)
    // a_{n+1} =
    // {上のaの係数を1ずらす(f(n)が項数を進ませる写像であると考えるとこれでOK)}
    // すると、a_{n+1}のk項目にはa_kが表せるので、初項の条件を使うとa_nでa_{n+1}を表現できる
    // a_{n+1} = (x_{k-1} * d_0) * a_0 + (x_0 * a_1 + x_{k-1} * d_1) * a_2 + ...
    // これはx_{i-1} * a_iって項が出てくるのでxを右にずらすと実装しやすい。
    long long last = x.back();
    x.pop_back();
    x.insert(x.begin(), 0);
    for (size_t i = 0; i < k; ++i) { x[i] = (x[i] + d[i] * last) % mod; }
  }
  void mul2(std::vector<long long>& x) {
    // a_2nを{x_0, x_1,...,x_{k-1}}で表すことが目標。
    // a_2n = x_0 * a_n + x_1 * a_{n+1} + ... + x_{k-1} * a_{n+k-1}だから、
    // a_n ~ a_{n+k-1}, つまりf(n) ~
    // f(n+k-1)が{x_0,x_1,...,x_{k-1}}で表せればよい。
    // incと同じ考えでそれぞれO(k)で出るので、合計O(k^2)でa_2nが求まる。
    std::vector<long long> a_n_i(x);
    std::vector<long long> res(k);
    for (size_t i = 0; i < k; ++i, add1(a_n_i)) {
      for (size_t j = 0; j < k; ++j) {
        res[j] = (res[j] + a_n_i[j] * x[i]) % mod;
      }
    }
    x = std::move(res);
  }

  // calc a_n
  long long calc(long long n) {
    std::vector<long long> res(k);
    res[0] = 1;
    {
      int i;
      for (i = 63; (n >> i & 1) == 0; --i) {}
      for (; i >= 0; --i) {
        mul2(res);
        if ((n >> i & 1) == 1) { add1(res); }
      }
    }

    long long ans = 0;
    for (size_t i = 0; i < k; ++i) { ans = (ans + res[i] * d[i]) % mod; };
    return ans;
  }

private:
  size_t k;
  std::vector<long long> d;
  const long long mod;
};