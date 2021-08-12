#include <algorithm>
#include <complex>
#include <vector>
using namespace std;

constexpr double EPS = 1e-10;
using Point = complex<double>;
using Line = pair<Point, Point>;
using Polygon = vector<Point>;
bool compare(const Point &a, const Point &b) { // x ascending order
  if (a.real() != b.real()) return a.real() < b.real();
  return a.imag() < b.imag();
};

double dot(Point p, Point q) { return (conj(p) * q).real(); }
double cross(Point p, Point q) { return (conj(p) * q).imag(); }
double slope(Line l) { return tan(arg(l.second - l.first)); }

Point project(Line l, Point p) { // project p onto line (s,t)
  return l.first + (l.second - l.first) * dot(p - l.first, l.second - l.first) /
                     norm(l.second - l.first);
}

Point reflect(Line l, Point p) {
  return l.first +
         conj((p - l.first) / (l.second - l.first)) * (l.second - l.first);
}

int ccw(Point a, Point b, Point c) {
  b -= a;
  c -= a;
  if (cross(b, c) > EPS) return +1;     // counter-clockwise
  if (cross(b, c) < -EPS) return -1;    // clockwise
  if (dot(b, c) < -EPS) return +2;      // c--a--b
  if (abs(b) + EPS < abs(c)) return -2; // a--b--c
  return 0;                             // a--c--b
}

bool intersect(Line a, Line b) {
  return ccw(a.first, a.second, b.first) * ccw(a.first, a.second, b.second) <=
           0 &&
         ccw(b.first, b.second, a.first) * ccw(b.first, b.second, a.second) <=
           0;
}

Point cross_point(Line a, Line b) {
  Point base = b.second - b.first;
  double area1 = abs(cross(base, a.first - b.first));
  double area2 = abs(cross(base, a.second - b.first));
  double t = area1 / (area1 + area2);
  return a.first + (a.second - a.first) * t;
}

// dist line and p
double distance(Point p, Line l) {
  return abs(cross(l.second - l.first, p - l.first) / abs(l.second - l.first));
}
double distance(Line s, Point p) {
  Point a = s.first, b = s.second;

  if (dot(b - a, p - a) < EPS) return abs(p - a);
  if (dot(a - b, p - b) < EPS) return abs(p - b);
  return distance(p, s);
}
double distance(Line s1, Line s2) {
  if (intersect(s1, s2)) return 0.0;
  return min(min(distance(s1, s2.first), distance(s1, s2.second)),
             min(distance(s2, s1.first), distance(s2, s1.second)));
}

double area(Polygon &ps) {
  double res = 0.0;
  for (int i = 0; i < (int)ps.size(); ++i)
    res += cross(ps[i], ps[(i + 1) % (int)ps.size()]);
  return abs(res) / 2.0;
}

Polygon convex_hull(Polygon &ps) {
  sort(ps.begin(), ps.end(), compare);
  int n = (int)ps.size(), k = 0;
  Polygon res(n * 2);

  // use "< -EPS" if include corner or boundary, otherwise, use "< EPS"
  for (int i = 0; i < n; res[k++] = ps[i++])
    while (k >= 2 && cross(res[k - 1] - res[k - 2], ps[i] - res[k - 1]) < EPS)
      --k;

  for (int i = n - 2, t = k + 1; i >= 0; res[k++] = ps[i--])
    while (k >= t && cross(res[k - 1] - res[k - 2], ps[i] - res[k - 1]) < EPS)
      --k;

  res.resize(k - 1);
  return res;
}

double caliper(Polygon &ps) {
  ps = convex_hull(ps);
  int n = (int)ps.size();
  if (n == 2) { return abs(ps[0] - ps[1]); }

  int i = 0, j = 0;
  // j --> (x asc order) --> i
  for (int k = 0; k < n; ++k) {
    if (compare(ps[i], ps[k])) i = k;
    if (compare(ps[k], ps[j])) j = k;
  }

  int si = i, sj = j;
  double res = 0.0;
  // rotate 180 degrees
  while (i != sj || j != si) {
    res = max(res, abs(ps[i] - ps[j]));
    if (cross(ps[(i + 1) % n] - ps[i], ps[(j + 1) % n] - ps[j]) < 0) {
      i = (i + 1) % n;
    } else {
      j = (j + 1) % n;
    }
  }

  return res;
}
