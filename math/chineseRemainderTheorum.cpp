#include <iostream>
#include <vector>
using namespace std;
// https://qiita.com/drken/items/b97ff231e43bce50199a

// a * x + b * y = gcd(a, b)を満たすx, yを取得
// 最終的に
// a0 = gcd(a, b), b0 = 0, x = 1, y = 0の状態を作り出す。
// そこから来た道を辿り、a0, b0からa, bを復元する過程でx, yを求めるイメージ。
int extgcd(int a, int b, int &x, int &y)
{
  if (b == 0)
  {
    x = 1;
    y = 0;
    return a;
  }
  int g = extgcd(b, a % b, y, x);
  y -= (a / b) * x;
  return g;
}

// 連立線形合同式を解く。拡張ユークリッドの誤除法を用いる。
// 解は無数にあるので、解全体を表す x ≡ b (mod m)のbとmのペアを返す。
// x ≡ b1 (mod m1), x ≡ b2 (mod m2), ...
// m1, m2 ...が互いに素なら、xが一意に定まる。
pair<int, int> linear_congruence(const vector<int> &A, const vector<int> &B, const vector<int> &M)
{
  // 各合同式を解きながら更新していく。
  int x = 0, m = 1;
  for (int i = 0; i < (int)B.size(); ++i)
  {
    int p, q;
    int d = extgcd(m, M[i], p, q);
    // Mp ≡ d (mod m[i])
    // Mp - m[i]q = d
    // p is inv of M/d (mod. m[i]/d)
    // Mとm[i]は互いに素じゃなくても、b[i] - rがgcd(M, m[i])で割り切れれば良い。
    if ((B[i] - x) % d != 0)
      return make_pair(0, -1);
    int tmp = (B[i] - r) / d * p % (M[i] / d);
    r += m * tmp;
    M *= M[i] / d;
  }
  return make_pair((x + M) % M, M);
}
