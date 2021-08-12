#include <functional>
#include <iostream>
#include <vector>
using namespace std;
template <typename S = long long, typename F = long long>
class LazySegmentTree {
public:
  LazySegmentTree(int n_, S e, F id, function<S(S, S)> op,
                  function<S(S, F)> mapping, function<F(F, F)> composition)
    : e(e), id(id), op(op), mapping(mapping), composition(composition) {
    for (n = 1; n < n_; n <<= 1)
      ;
    data.assign(n * 2 - 1, e);
    lazy.assign(n * 2 - 1, id);
  }
  LazySegmentTree(int n_, std::vector<S>& a, S e, F id, function<S(S, S)> op,
                  function<S(S, F)> mapping, function<F(F, F)> composition)
    : e(e), id(id), op(op), mapping(mapping), composition(composition) {
    for (n = 1; n < n_; n <<= 1)
      ;
    data.resize(n * 2 - 1);
    lazy.assign(n * 2 - 1, id);
    std::copy(a.begin(), a.end(), data.begin() + n - 1);
    for (int i = n - 2; i >= 0; --i)
      data[i] = op(data[i * 2 + 1], data[i * 2 + 2]);
  }

  void update(int a, int b, F x, int i = 0, int l = 0, int r = -1) {
    if (r < 0) r = n;

    eval(i);
    if (b <= l || r <= a) return;
    if (a <= l && r <= b) {
      lazy[i] = composition(lazy[i], x);
      eval(i); // ここで評価しないとダメ
    } else {
      update(a, b, x, i * 2 + 1, l, (l + r) / 2);
      update(a, b, x, i * 2 + 2, (l + r) / 2, r);
      data[i] = op(data[i * 2 + 1], data[i * 2 + 2]);
    }
  }

  S query(int a, int b, int i = 0, int l = 0, int r = -1) {
    if (r < 0) r = n;
    if (b <= l || r <= a) return e;

    eval(i);
    if (a <= l && r <= b) return data[i];
    S vl = query(a, b, i * 2 + 1, l, (l + r) / 2);
    S vr = query(a, b, i * 2 + 2, (l + r) / 2, r);
    return op(vl, vr);
  }

private:
  void eval(int i) {

    // if (lazy[i] == id) return; // =を実装しないと通らない
    if (i < n - 1) {
      lazy[i * 2 + 1] = composition(lazy[i * 2 + 1], lazy[i]);
      lazy[i * 2 + 2] = composition(lazy[i * 2 + 2], lazy[i]);
    }

    data[i] = mapping(data[i], lazy[i]);
    lazy[i] = id;
  }
  S e;
  F id;
  function<S(S, S)> op;
  function<S(S, F)> mapping;     // 写像
  function<F(F, F)> composition; // 写像の合成

  vector<S> data;
  vector<F> lazy;
  int n; // leaf count, idx is from n-1 to n*2-1
};

// RAQ/RMQ
struct RARM {
  vector<long long> data, lazy;
  int n;

  RARM(int n_) {
    for (n = 1; n < n_; n *= 2)
      ;
    data.assign(n * 2 - 1, 0);
    lazy.assign(n * 2 - 1, 0);
  }

  void eval(int i, int l, int r) {

    if (lazy[i] != 0) {
      data[i] += lazy[i];

      if (r - l > 1) {
        lazy[i * 2 + 1] += lazy[i] / 2;
        lazy[i * 2 + 2] += lazy[i] / 2;
      }
    }

    lazy[i] = 0;
  }

  void update(int a, int b, long long x, int i = 0, int l = 0, int r = -1) {
    if (r < 0) r = n;

    eval(i, l, r);

    if (b <= l || r <= a) return;
    if (a <= l && r <= b) {
      lazy[i] += (r - l) * x;
      eval(i, l, r); // ここで評価しないとダメ
    } else {
      update(a, b, x, i * 2 + 1, l, (l + r) / 2);
      update(a, b, x, i * 2 + 2, (l + r) / 2, r);
      data[i] = data[i * 2 + 1] + data[i * 2 + 2];
    }
  }

  long long query(int a, int b, int i = 0, int l = 0, int r = -1) {
    if (r < 0) r = n;
    if (b <= l || r <= a) return 0;

    eval(i, l, r);
    if (a <= l && r <= b) return data[i];
    long long vl = query(a, b, i * 2 + 1, l, (l + r) / 2);
    long long vr = query(a, b, i * 2 + 2, (l + r) / 2, r);
    return vl + vr;
  }
};
