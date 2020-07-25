#include <iostream>
#include <vector>
#include <complex>
using namespace std;

using C = complex<double>;
vector<C> dft(vector<C> &f, int n, int sgn = 1)
{
    if (n == 1)
        return f;

    vector<C> f0(n / 2), f1(n / 2);
    for (int i = 0; i < n / 2; ++i)
    {
        f0[i] = f[i * 2 + 0];
        f1[i] = f[i * 2 + 1];
    }
    f0 = dft(f0, n / 2, sgn);
    f1 = dft(f1, n / 2, sgn);

    C zeta(cos(M_PI * 2 / n), sin(M_PI * 2 / n) * sgn);
    C pow_zeta = 1;

    for (int i = 0; i < n; ++i)
    {
        f[i] = f0[i % (n / 2)] + pow_zeta * f1[i % (n / 2)];
        pow_zeta *= zeta;
    }

    return f;
}

vector<C> multiply(vector<C> &g, vector<C> &h)
{
    int n_ = (int)g.size() + (int)h.size() + 1;
    int n = 1;
    while (n < n_)
        n *= 2;
    g.resize(n);
    h.resize(n);

    g = dft(g, n);
    h = dft(h, n);

    vector<C> f(n);
    for (int i = 0; i < n; ++i)
        f[i] = g[i] * h[i];

    // inverse
    f = dft(f, n, -1);
    for (int i = 0; i < n; ++i)
        f[i] /= n;
    return f;
}
