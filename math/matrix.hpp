#include <functional>
#include <iostream>
#include <vector>
using namespace std;

template <typename T>
class Mat {
public:
  size_t height;
  size_t width;
  std::vector<T> mat;
  // constructor
  Mat(size_t h, size_t w) : height(h), width(w), mat(h * w) {}
  // write
  T& operator()(size_t i, size_t j) { return mat[i * width + j]; }
  // read
  const T& operator()(size_t i, size_t j) const { return mat[i * width + j]; }

  // only square matrix
  static Mat identity(size_t n) {
    Mat E(n, n);
    for (size_t i = 0; i < n; ++i) { E(i, i) = 1; }
    return E;
  }

  // O(N^3)
  Mat& operator*=(const Mat& m) {
    vector<T> res(height * m.width);
    for (size_t i = 0; i < height; ++i) {
      for (size_t k = 0; k < width; ++k) {
        for (size_t j = 0; j < m.width; ++j) {
          res[i * m.width + j] += (*this)(i, k) * m(k, j);
        }
      }
    }
    mat.swap(res);
    return *this;
  }

  Mat& operator*(const Mat& rhs) { return Mat(*this) *= rhs; }

  // O(N^3logK)
  void pow(long long exponent) {
    Mat res = Mat::identity(height);

    while (exponent > 0) {
      if (exponent & 1) { res *= *this; }
      *this *= *this;
      exponent >>= 1;
    }
    swap(*this, res);
  }

  friend std::ostream& operator<<(std::ostream& os, Mat& m) {
    os << "---- Mat ----" << std::endl;
    for (int i = 0; i < m.height; ++i) {
      for (int j = 0; j < m.width; ++j) {
        os << m(i, j) << " \n"[j == m.width - 1];
      }
    }
    os << "---- Mat end ----" << std::endl;
    return os;
  }
};