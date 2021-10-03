#include <vector>

template <typename T>
class cumulative_2d {
public:
  cumulative_2d(size_t h, size_t w) : height(h), width(w), data(h + 1, w + 1){};
  void set(size_t y, size_t x, T val) {
    // assert(0<=h&&h<height&&0<=w&&w<width);
    data[y + 1][x + 1] = val;
  };
  T get(size_t y1, size_t x1, size_t y2, size_t x2) {
    return data[y2][x2] + data[y1][x1] - data[y1][x2] - data[y2][x1];
  }
  build() {
    for (size_t y = 0; y < height; ++y) {
      for (size_t x = 0; x < width; ++x) { data[y][x + 1] += data[y][x]; }
    }
    for (size_t x = 0; x < width; ++x) {
      for (size_t y = 0; y < height; ++y) { data[y + 1][x] += data[y][x]; }
    }
  }

private:
  size_t height, width;
  std::vector<vector<T>> data;
};