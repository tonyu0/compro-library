#include <vector>
using namespace std;

constexpr double EPS = 1e-5;
using Vector = vector<double>;
using Matrix = vector<Vector>;

// Ax = b, make sure A.size() == b.size()
Vector gaussian_elimination(const Matrix &A, const Vector &b) {
  // A is m * n matrix
  int m = A.size();
  int n = A[0].size();

  // make augmented matrix B
  Matrix B(m, Vector(n + 1));
  for (int i = 0; i < m; ++i)
    for (int j = 0; j < n; ++j) B[i][j] = A[i][j];
  for (int i = 0; i < m; ++i) B[i][n] = b[i];

  Vector x(m);
  for (int i = 0; i < m; ++i) {
    int pivot = i;
    for (int j = i + 1; j < m; ++j)
      if (abs(B[j][i]) > abs(B[pivot][i])) pivot = j;
    swap(B[i], B[pivot]);

    if (abs(B[i][i]) < EPS) return Vector(0);

    for (int j = i + 1; j <= n; ++j) B[i][j] /= B[i][i];

    for (int j = 0; j < m; ++j) {
      if (i == j) continue;
      for (int k = i + 1; k <= n; ++k) { B[j][k] -= B[j][i] * B[i][k]; }
    }
  }

  for (int i = 0; i < m; ++i) x[i] = B[i][n];
  return x;
}
