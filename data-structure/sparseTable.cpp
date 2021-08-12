// 説明: 値に変更がない場合の区間の最大や最小を取得する
// TODO: __builtin_clzは最上位ビットから数えて0の連続する個数。

#include <functional>
#include <vector>
using namespace std;

template <typename T, class F = function<T(const T &, const T &)>>
class SparseTable {
public:
  int n;
  vector<vector<T>> mat;
  F func;

  SparseTable(const vector<T> &a, const F &f) : func(f) {
    n = static_cast<int>(a.size());
    int max_log = 32 - __builtin_clz(n);
    mat.resize(max_log);
    mat[0] = a;
    for (int j = 1; j < max_log; j++) {
      mat[j].resize(n - (1 << j) + 1);
      for (int i = 0; i <= n - (1 << j); i++) {
        mat[j][i] = func(mat[j - 1][i], mat[j - 1][i + (1 << (j - 1))]);
      }
    }
  }

  T get(int from, int to) const {
    assert(0 <= from && from <= to && to <= n - 1);
    int lg = 32 - __builtin_clz(to - from + 1) - 1;
    return func(mat[lg][from], mat[lg][to - (1 << lg) + 1]);
  }
};

// template<class T> struct SparseTable {
//   vector<vector<T>> t;
//   SparseTable(const vector<T>& a) : t(1, a) {
//     for (int k = 1, n = a.size(); 1 << k <= n; ++k) {
//       t.emplace_back(n - (1 << k) + 1);
//       for (int i = 0; i + (1 << k) <= n; ++i) {
//         t[k][i] = min(t[k - 1][i], t[k - 1][i + (1 << (k - 1))]);
//       }
//     }
//   }
//   T acc(int l, int r) const {
//     assert(l < r);
//     int k = __lg(r - l);
//     return min(t[k][l], t[k][r - (1 << k)]);
//   }
// };