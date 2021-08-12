#include <complex>
#include <iostream>
#include <vector>
using namespace std;

namespace fft {
using C = complex<double>;
const double PI = 3.14159265358979323846;

void fft(vector<C>& f, int n, int sgn = 1) {
  if (n == 1) return;

  vector<C> f0(n / 2), f1(n / 2);
  for (int i = 0; i < n / 2; ++i) {
    f0[i] = f[i * 2];
    f1[i] = f[i * 2 + 1];
  }
  fft(f0, n / 2, sgn);
  fft(f1, n / 2, sgn);

  C zeta = {cos(2.0 * PI / (double)n), sin(2.0 * PI / (double)n) * sgn};
  C now = {1.0, 0.0};

  for (int i = 0; i < n; ++i) {
    f[i] = f0[i % (n / 2)] + now * f1[i % (n / 2)];
    now *= zeta;
  }
}

template <typename T>
vector<T> multiply(vector<T>& a, vector<T>& b) {
  int n = (int)a.size() + (int)b.size() - 1;
  int nn = 1;
  while (nn < n) nn <<= 1;

  vector<C> f(nn), g(nn);
  for (int i = 0; i < (int)a.size(); ++i) f[i] = {(double)a[i], 0.0};
  for (int i = 0; i < (int)b.size(); ++i) g[i] = {(double)b[i], 0.0};

  fft(f, nn);
  fft(g, nn);
  for (int i = 0; i < nn; ++i) f[i] = f[i] * g[i];

  // inverse
  fft(f, nn, -1);

  vector<T> c(n);
  for (int i = 0; i < n; ++i) c[i] = (T)(f[i].real() / (double)nn + 0.5);
  return c;
}
} // namespace fft