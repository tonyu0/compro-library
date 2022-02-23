#include <vector>
template <typename T, T (*op)(T, T), T id>
// https://codeforces.com/contest/1301/submission/131480780
class SparseTable2D {
public:
  SparseTable2D() = delete;
  SparseTable2D(const std::vector<std::vector<T>> &in) {
    rows = in.size();
    // assert(rows != 0);
    cols = in[0].size();
    log.resize(max(rows, cols) + 1);
    for (size_t i = 2; i <= max(rows, cols); ++i) { log[i] = log[i / 2] + 1; }
    table.resize(log[rows] + 1);
    for (size_t i = 0; i <= log[rows]; ++i) { table[i].resize(log[cols] + 1); }
    for (size_t i = 0; i <= log[rows]; ++i) {
      for (size_t j = 0; j <= log[cols]; ++j) { table[i][j] = in; }
    }

    for (size_t k = 1; k <= log[rows]; ++k) {
      for (size_t i = 0; i + (1 << k) <= rows; ++i) {
        // i, i+(1<<k-1)に二分できる間
        for (size_t j = 0; j < cols; ++j) {
          table[k][0][i][j] =
            op(table[k - 1][0][i][j], table[k - 1][0][i + (1 << (k - 1))][j]);
        }
      }
    }
    for (size_t l = 1; l <= log[cols]; ++l) {
      for (size_t k = 0; k <= log[rows]; ++k) {
        for (size_t i = 0; i < rows; ++i) {
          for (size_t j = 0; j + (1 << l) <= cols; ++j) {
            table[k][l][i][j] =
              op(table[k][l - 1][i][j], table[k][l - 1][i][j + (1 << (l - 1))]);
          }
        }
      }
    }
  }
  T get(int r1, int c1, int r2, int c2) {
    if (r2 < r1 || c2 < c1 || r1 < 0 || r2 >= rows || c1 < 0 || c2 >= cols)
      return id;
    size_t k = log[r2 - r1 + 1];
    size_t l = log[c2 - c1 + 1];
    return op(table[k][l][r1][c1],
              op(table[k][l][r1][c2 - (1 << l) + 1],
                 op(table[k][l][r2 - (1 << k) + 1][c1],
                    table[k][l][r2 - (1 << k) + 1][c2 - (1 << l) + 1])));
  }

private:
  std::vector<std::vector<std::vector<std::vector<T>>>> table;
  std::vector<size_t> log;
  size_t rows, cols;
};
