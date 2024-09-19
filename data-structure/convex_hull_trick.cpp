#include <deque>

// f(x)=ax+bという形の直線をたくさん管理して、任意のxを入れたときの最小値を取得できる
// 最小値をとる直線だけをなぞってみると、下に凸となり、これは傾き降順に並べたものとなる。
// verify: https://atcoder.jp/contests/colopl2018-final-open/submissions/6618008
// (蟻本p305の同値は、平行な直線がたくさんあると嘘？)

// TODO:
// 最大値クエリに対応
// クエリに単調性なくても動くもの書く
// LiChaoTree? 履修

template <typename T>
class convex_hull_trick {
  struct Line {
    T a, b;
    Line(T _a, T _b) : a(_a), b(_b) {}
    T get(T x) { return a * x + b; }
  };
  std::deque<Line> deq;

  // f1.a >= f2.a >= f3.aを仮定 (／:f1, ￣: f2, ＼: f3)
  // ／￣＼ : 3つともいる(どこかで最小値とりうる)
  //  ＿
  // ＼／ : 上の1ついらない(どこでも最小値とらない)
  // 下の図の状況で交点の条件を考える。
  // それぞれの交点はy=ax+bを等式で結んで解くとわかる。
  // f2がいらない条件: x12 >= x23 <=> (b2-b1)/(a1-a2) >= (b3-b2)/(a2-a3)
  bool check(Line f1, Line f2, Line f3) {
    return (f2.a - f1.a) * (f3.b - f2.b) >= (f2.b - f1.b) * (f3.a - f2.a);
  }

public:
  convex_hull_trick() = default;
  // insert(a,b):
  // 新しい直線(ax+b)を、傾きが単調(a1>=a2>=a3>=...)、不必要な直線が入らないように(挿入の際check())追加
  // 入れる位置を傾きで二分探索して挿入する位置を決める必要があるが、
  // 傾きを単調に入れられる場合は後ろのチェックだけでよい(以下の実装、O(1))
  void insert(T a, T b) {
    Line line(a, b);
    // a1 >= a2 >= a3 >= ... でinsertされることを仮定
    while (deq.size() >= 2 && check(*(deq.end() - 2), *(deq.end() - 1), line))
      deq.pop_back();
    deq.push_back(line);
  }

  // query(x): a * x + bが最小となるような直線のxでの値
  // 傾きで二分探索して最小値をとる位置を調べる必要があるが、xを昇順(x1<=x2<=x3<=...)に入れる場合は簡略化できる
  T query(T x) {
    // x1 <= x2 <= x3 <= ... でqueryされることを仮定
    // するとdeqには傾き降順でinsertされているため、
    // f0 >= f1である場合はxの条件からf0はもう最小値は取りえないため削除してよい
    while (deq.size() >= 2 && deq[0].get(x) >= deq[1].get(x)) deq.pop_front();
    return deq.front().get(x);
  }
};