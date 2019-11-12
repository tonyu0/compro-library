#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;
// 説明: nに含まれるサイズkの部分集合を列挙する。
// 2の補数表現より-x == (~x) + 1なので、最下位ビットは x & -xで取れる。

// 部分集合を投げると次の部分集合を返してくれる関数
int next_combination(int comb)
{
  int x = comb & -comb;              //最下位の1が連続している区間(0101110 -> 1001111)
  int y = comb + x;                  // 最下位の1が連続している区間の1つ上のビットを1にして、それ以下のビットを0
  return ((comb & ~y) / x >> 1) | y; // 立ってるビットが1減るまでxを右シフト、それをyとorする
}

// 使用例
// https://atcoder.jp/contests/abc018/tasks/abc018_4
void ABC018D()
{
  int N, M, P, Q, R, x, y, z, happy[325][325];
  cin >> N >> M >> P >> Q >> R;
  for (int i = 0; i < R; ++i)
  {
    cin >> x >> y >> z;
    --x, --y;
    happy[x][y] = z;
  }

  // 上記の関数で片方の組み合わせは全列挙できるので、もう片方の最適な選び方を速めに決める。
  int ans = 0;
  for (int comb = (1 << P) - 1; comb < (1 << N); comb = next_combination(comb))
  {
    vector<int> otoko(M);
    for (int i = 0; i < N; ++i)
      if (comb >> i & 1)
        for (int j = 0; j < M; ++j)
          otoko[j] += happy[i][j];
    sort(otoko.rbegin(), otoko.rend());

    int tmp = 0;
    for (int i = 0; i < Q; ++i)
      tmp += otoko[i];
    ans = max(ans, tmp);
  }
  cout << ans << '\n';
}

int main()
{
  ABC018D();
  return 0;
}