class xor128
{
public:
  static constexpr unsigned min() { return 0; }
  static constexpr unsigned max() { return 1e9; }
  unsigned operator()() { return random(); }
  xor128() {}

private:
  unsigned x = 123456789, y = 362436069, z = 521288629, w = 88675123;
  unsigned random()
  {
    unsigned t;
    t = x ^ (x << 11);
    x = y;
    y = z;
    z = w;
    return w = (w ^ (x >> 19)) ^ (t ^ (t >> 8));
  }
};