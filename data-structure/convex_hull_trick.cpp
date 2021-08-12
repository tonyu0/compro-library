#include <deque>

//  Convex Hull Trick
//  最小の考え方
//  xを固定すると、必要な直線のみある場合、直線のとる値は下に凸になるため、二分探索ができる。

//  不必要の条件
//  前から傾き単調非減少の直線が3つ(f1,f2,f3)あるとき、真ん中の直線がいらない条件
//  f1,f2のx座標>=f2,f3のx座標
//  ▽(不必要) or △(必要)
//  交点を求めると上の条件は(b3-b2)*(a2-a1) >= (b2-b1)*(a3-a2)

//  基本方針はこれで、あとは傾き単調とかxが単調とかで計算量が落ちたりする。
//  与えられるxが昇順になる場合、一度最小値を取らなくなった直線は再び最小値を取らない
//  なので傾きでかいほうからふたつずつ最小値を見ていって、最小じゃなかったら削除。

// 蟻本p305の同値は、平行な直線がたくさんあると嘘？理由:

template <typename T>
class convex_hull_trick {
  struct Line {
    T a, b;
    Line(T _a, T _b) : a(_a), b(_b) {}
    T get(T x) { return a * x + b; }
  };
  std::deque<Line> deq;

  // ここの判定を変えると最小から最大にできたり、など
  // f1.a >= f2.a >= f3.aを仮定
  // (a, b)で直線管理
  // true=いらない
  bool check(Line f1, Line f2, Line f3) {
    return (f2.a - f1.a) * (f3.b - f2.b) >= (f2.b - f1.b) * (f3.a - f2.a);
  }

public:
  convex_hull_trick() = default;
  void insert(T a, T b) {
    Line line(a, b);
    // fの傾きは必ず最小なので、後ろから見てると削れる。
    while (deq.size() >= 2 && check(*(deq.end() - 2), *(deq.end() - 1), line))
      deq.pop_back();
    deq.push_back(line);
  }

  // min value of a * x + b
  // xは小さい順に入れられることを仮定
  T query(T x) {
    while (deq.size() >= 2 && deq[0].get(x) >= deq[1].get(x)) deq.pop_front();
    return deq.front().get(x);
  }
};