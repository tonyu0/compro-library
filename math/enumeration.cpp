#include <iostream>
#include <vector>
using namespace std;

// 説明: 数え上げ
// https://yukicoder.me/problems/no/117

constexpr int MOD = 1e9 + 7;
constexpr int MAX = 100010;
long fac[MAX], inv[MAX], finv[MAX];
void init()
{
    fac[0] = inv[0] = finv[0] = 1;
    fac[1] = inv[1] = finv[1] = 1;
    for (int i = 2; i < MAX; ++i)
    {
        fac[i] = fac[i - 1] * i % MOD;
        inv[i] = MOD - inv[MOD % i] * (MOD / i) % MOD;
        finv[i] = finv[i - 1] * inv[i] % MOD;
    }
}

// 区別できるnこの中からkこ選ぶときの順列の総数
long P(int n, int k)
{
    if (n < 0 || k < 0 || n < k)
        return 0;
    return fac[n] * finv[n - k] % MOD;
}

// 区別できるnこの中からkこ選ぶときの組み合わせの総数
long C(int n, int k)
{
    if (n < 0 || k < 0 || n < k)
        return 0;
    return (fac[n] * finv[n - k] % MOD) * finv[k] % MOD;
}

// 区別できるnこの中から重複を許してkこ選ぶときの組み合わせの総数
long H(int n, int k)
{
    return C(n + k - 1, k);
}