#include <iostream>
#include <vector>
using namespace std;
// https://qiita.com/drken/items/b97ff231e43bce50199a
// 数学系関数その他。
// 拡張ユークリッド、連立合同式解くやつ、floor sum

// a * x + b * y = gcd(a, b)を満たすx, yを取得
// 最終的に
// a0 = gcd(a, b), b0 = 0, x = 1, y = 0の状態を作り出す。
// そこから来た道を辿り、a0, b0からa, bを復元する過程でx, yを求めるイメージ。
/**
 * @brief 簡単な説明（～する関数）
 * @param[in] a(引数名) 引数の説明
 * @param[out] b(引数名) 引数の説明
 * @return bool 戻り値の説明
 * @details 詳細な説明
 */
long long extgcd(long long a, long long b, long long& x, long long& y) {
  if (b == 0) {
    x = 1;
    y = 0;
    return a;
  }
  long long g = extgcd(b, a % b, y, x);
  y -= (a / b) * x;
  return g;
}

// def extgcd(a, b):
//     if b:
//         d, y, x = extgcd(b, a % b)
//         y -= (a // b)*x
//         return d, x, y
//     return a, 1, 0
tuple<long long, long long, long long> ext_gcd(long long a, long long b) {
  if (b > 0) {
    auto [d, y, x] = ext_gcd(b, a % b);
    y -= a / b * x;
    return {d, x, y};
  }
  return {a, 1, 0};
}

// 連立線形合同式を解く。拡張ユークリッドの誤除法を用いる。
// 解は無数にあるので、解全体を表す x ≡ b (mod m)のbとmのペアを返す。
// x ≡ b1 (mod m1), x ≡ b2 (mod m2), ...
// m1, m2 ...が互いに素なら、xが一意に定まる。
pair<long long, long long> linear_congruence(const vector<long long>& B,
                                             const vector<long long>& M) {
  // 各合同式を解きながら更新していく。
  long long x = 0, lcm = 1;
  for (int i = 0; i < (int)B.size(); ++i) {
    long long p, q;
    long long g = extgcd(lcm, M[i], p, q);
    // Mp ≡ d (mod m[i])
    // Mp - m[i]q = d
    // p is inv of M/d (mod. m[i]/d)
    // Mとm[i]は互いに素じゃなくても、b[i] - rがgcd(M, m[i])で割り切れれば良い。
    if ((B[i] - x) % g != 0) return make_pair(0, -1);
    long long tmp = (B[i] - x) / g * p % (M[i] / g);
    x += lcm * tmp;
    lcm *= M[i] / g;
  }
  return make_pair((x + lcm) % lcm, lcm);
}

// floor_sum(n, m, a, b) returns \sum_{x = 0} ^ {n - 1}[(a * x + b) / m]
long long floor_sum(long long n, long long m, long long a, long long b) {
  long long res = 0;
  // a = m * q + r
  // ->[(x *(m * q + r) + b) / m] = q * x +[(r * x + b) / m]
  // \sum_{x = 0 } ^ {n - 1 } q * x =[a / m] * n *(n - 1) / 2
  if (a >= m) {
    res += a / m * n * (n - 1) / 2;
    a %= m;
  }
  if (b >= m) {
    res += b / m * n;
    b %= m;
  }

  // floor((a * x + b) / m) = yとなるxの範囲を考える。
  // ceil, floorは不等式で評価するとよい
  // 最小値xは
  // y <= (a * x + b) / mなので
  // my <= ax + b
  // my - b / a <= x
  // xは整数なので、最小値は左辺の切り上げ(ceil((m * y - b) / a))
  // 最小値x_iが分かると、iになる範囲はx_{i + 1 } - x_i
  // ymaxだけはn - x_ymax
  // (x_2 - x_1) * 1 +(x_3 - x_2) * 2 +... +(x_ymax - x_{ymax - 1}) *(ymax
  // - 1)
  // +(n - x_ymax) * ymax
  // 整理すると、(最後のn * ymaxにより、ymax個の塊にnをぶんぱい可能)
  // \sum_{i = 1 } ^ {ymax }(n - x_i)
  // これを崩してymax - 1までの小問題にできるか考える。
  // (ここでx_max = m * y_max - bと置くとうまくいくっぽいが、仕組みが分からん)
  // \sum_{i = 1 } ^ {ymax}(n - ceil(m * i - b) / a)の和の順番を逆にして
  // \sum_{i = 0 } ^ {ymax - 1}(n - ceil(m *(ymax - i) - b) / a)
  // (0 - ceil(x) = floor(0 - x) を使って)
  //  \sum_{i = 0 } ^ {ymax - 1 }(n + floor((b - m *(ymax - i)) / a)
  // n*ymax + \sum_{i = 0 } ^ {ymax - 1 } floor((m * i + b - m * ymax)/a)
  // 最初の式 : \sum_{i = 0 } ^ {n - 1 } floor((a * i + b) / a)と比較すると
  long long ymax = (a * (n - 1) + b) / m;
  if (ymax == 0) return res;
  if (b < m * ymax) {
    // b >= m * ymaxとなるようにbを調整。
    // floor_sumの2番目、4番目の引数は
    // while b < m* ymax:
    // b += a
    // res -= ymax
    long long diff = m * ymax - b;
    long long add_cnt = (diff + a - 1) / a;
    res -= add_cnt * ymax;
    b += add_cnt * a;
  }

  res += n * ymax + floor_sum(ymax, a, m, b - m * ymax);
  return res;
}