// 説明: 直線を管理するデータ構造。ある値xに対して最大値or最小値をとる直線の値を高速に求める。


struct CHT {
  struct Line {
    long long a, b;
    Line(long long a, long long b) : a(a), b(b) {}
    long long get(long long x) { return a * x + b; }
  };
  deque<Line> deq;

  CHT(){}

  bool check(Line f1, Line f2, Line f3) {
    return (f2.a - f1.a) * (f3.b - f2.b) >= (f2.b - f1.b) * (f3.a - f2.a);
  }

  void insert(long long a, long long b) {
    Line line(a, b);
    while(deq.size() >= 2 && check(*(deq.end()-2), *(deq.end()-1), line)) deq.pop_back();
    deq.push_back(line);
  }

  long long query(long long x) {
    while(deq.size() >= 2 && deq[0].get(x) >= deq[1].get(x)) deq.pop_front();
    return deq.front().get(x);
  }
};

