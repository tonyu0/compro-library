#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// TODO: 多項式同士の演算として書く
// templateの部分コンパイル時定数にできる？
template <long long mod = 998244353>
class modFFT {
public:
  modFFT() {}

  vector<long long> multiply(vector<long long> a, vector<long long> b) {
    int an = a.size();
    int bn = b.size();
    int need = an + bn - 1;
    int nn = 1;
    while (nn < 2 * an || nn < 2 * bn) nn <<= 1;
    a.resize(nn);
    b.resize(nn);
    fft(a);
    fft(b);
    for (int i = 0; i < nn; i++) a[i] = a[i] * b[i] % mod;
    reverse(++a.begin(), a.end());
    fft(a);
    long long inv = pow(nn, mod - 2);
    for (int i = 0; i < nn; i++) a[i] = a[i] * inv % mod;
    a.resize(need);
    return a;
  }
  vector<long long> pow(vector<long long> a) {
    size_t n = a.size();
    if (n == 1) return {1, a[0] - 1};

    vector<long long> b(a.begin(), a.begin() + n / 2);
    vector<long long> c(a.begin() + n / 2, a.end());
    return multiply(pow(b), pow(c));
  }

private:
  long long pow(long long a, long long b) {
    long long x = 1, step = 1 << 30;
    while (step > 0) {
      x = x * x % mod;
      if (step & b) x = x * a % mod;
      step >>= 1;
    }
    return x;
  }

  void fft(vector<long long>& a) {
    int n = a.size();
    for (int i = 0; i < n; i++) {
      int j = 0;
      int x = i, y = n - 1;
      while (y > 0) {
        j = (j << 1) + (x & 1);
        x >>= 1;
        y >>= 1;
      }
      if (i < j) swap(a[i], a[j]);
    }
    for (int len = 1; len < n; len *= 2) {
      long long root = pow(5, (mod - 1) / (2 * len));
      for (int i = 0; i < n; i += 2 * len) {
        long long w = 1;
        for (int j = 0; j < len; j++) {
          long long u = a[i + j];
          long long v = a[i + j + len] * w % mod;
          a[i + j] = (u + v) % mod;
          a[i + j + len] = (mod + u - v) % mod;
          w = w * root % mod;
        }
      }
    }
  }
};

int main() {
  cin.tie(0)->sync_with_stdio(0);
  int n, q, b;
  cin >> n >> q;
  vector<ll> a(n);
  rep(i, 0, n) cin >> a[i];
  modFFT fft = modFFT();
  vector<ll> c = fft.pow(a);

  reverse(c.begin(), c.end());
  rep(i, 0, q) {
    cin >> b;
    cout << c[b] << '\n';
  }
  return 0;
}
