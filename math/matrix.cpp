#include <iostream>
#include <vector>
#include <functional>
using namespace std;

// 説明:
// 行列演算を行う。二分累乗ができる。
// 加法として定義される二項演算と乗法として定義される二項演算と乗法の単位元を渡す。
// []をオーバーロードしてmat[i][j]みたいに要素の参照・書き換えができるようになってる。

// TODO: N項間線形漸化式により与えられる数式のK項目をO(N^2logK)で求めるきたまさ法なるものがあるらしい。

// 備考: 抽象化したせいかわからないが、する前より5倍くらい遅くなってしまった。速くしたい。
template <typename T, class F = function<T(const T &, const T &)>>
class Matrix
{
public:
    vector<vector<T>> mat;
    T unity;
    F add, mul;
    Matrix(int h, int w, T unity, const F &add, const F &mul)
        : mat(h, vector<T>(w)), unity(unity), add(add), mul(mul) {}

    inline vector<T> &operator[](int i) { return mat[i]; }
    inline int height() const { return (int)mat.size(); }
    inline int width() const { return (int)mat[0].size(); }

    Matrix identity()
    {
        Matrix E(height(), width(), unity, add, mul);
        for (int i = 0; i < height(); ++i)
            E[i][i] = unity;
        return E;
    }

    // O(N^3)
    Matrix &operator*(Matrix &m)
    {
        int h = m.height(), w = m.width();
        vector<vector<T>> tmp(height(), vector<T>(w));
        for (int i = 0; i < height(); ++i)
            for (int j = 0; j < w; ++j)
                for (int k = 0; k < h; ++k)
                    tmp[i][j] = add(tmp[i][j], mul(mat[i][k], m[k][j]));
        mat.swap(tmp);
        return *this;
    }

    // O(N^3logK)
    Matrix operator^(T exponent)
    {
        Matrix res = identity();
        Matrix product = *this;

        while (exponent)
        {
            if (exponent & 1)
                res = res * product;
            product = product * product;
            exponent >>= 1;
        }
        return res;
    }
};

// (整数, +, *)
// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_7_D
void ITP1_7_D()
{
    int n, m, l;
    cin >> n >> m >> l;
    Matrix<long long> A(n, m, 1, [](auto i, auto j) { return i + j; }, [](auto i, auto j) { return i * j; });
    Matrix<long long> B(m, l, 1, [](auto i, auto j) { return i + j; }, [](auto i, auto j) { return i * j; });
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            cin >> A[i][j];
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < l; ++j)
            cin >> B[i][j];
    auto C = A * B;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < l; ++j)
            cout << C[i][j] << " \n"[j == l - 1];
}

// (非負整数, ^, &)
// https://atcoder.jp/contests/abc009/tasks/abc009_4
void ABC009D()
{
    unsigned K, M;
    cin >> K >> M;
    Matrix<unsigned> A(K, 1, ~0U, [](unsigned i, unsigned j) { return i ^ j; }, [](unsigned i, unsigned j) { return i & j; });
    Matrix<unsigned> C(K, K, ~0U, [](unsigned i, unsigned j) { return i ^ j; }, [](unsigned i, unsigned j) { return i & j; });
    for (int i = K - 1; i >= 0; --i)
        cin >> A[i][0];
    for (int i = 0; i < K; ++i)
        cin >> C[0][i];
    for (int i = 0; i < K - 1; ++i)
        C[i + 1][i] = ~0U;

    C = C ^ (M - 1);
    A = C * A;
    cout << A[K - 1][0] << '\n';
}

int main()
{
    // ITP1_7_D();
    ABC009D();
}