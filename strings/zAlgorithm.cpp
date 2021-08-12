#include <iostream>
#include <numeric>
#include <vector>
using namespace std;
template <typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T>& a) {
  os << "{";
  for (size_t i = 0; i < a.size(); ++i) os << (i > 0 ? "," : "") << a[i];
  return os << "}";
}

// pattern + '$' + text
// みたいにしてからz配列作ると、文字列検索ができる。
// 原理忘れたらここ
// https://personal.utdallas.edu/~besp/demo/John2010/z-algorithm.htm
// お気持ち: [l, r]のみ保持しておけば、基本的には[l,
// r]となる文字列に対する小問題
vector<int> z_algorithm(const string& s) {
  int n = s.size();
  vector<int> z(n);
  z[0] = n;
  int l = 0, r = 0;
  // prefixである区間[l,
  // r]のうち、rが最もでかいのを保持。iを進めていき、区間内なら計算をさぼれる。

  for (int i = 1; i < n; ++i) {
    int k = 0;
    // iが区間に含まれている場合:
    // とりあえずz[i-l]は計算済みなので見てみる。i + z[i-l] >=
    // rの場合は再計算が必要だが置いておいて、 i + z[i-l] <
    // rの場合、答えはmin(z[i-l], r - i)になる。
    if (i <= r) k = min(z[i - l], r - i);
    // i +
    // z[i-l]がrを超えてしまう場合は、それ以上伸ばしても一致しないことが分かっているので、min(r-i)で良い。
    // 再計算がいらない場合はここで終わり

    // iが区間に入っていない or
    // 上のminでr-iが選ばれたら、区間を右に伸ばす(ここはまだ見てない)
    // この時、マッチはkから行う(始点はiということだが)
    if (r < i || i + k == r)
      while (i + k < n && s[k] == s[i + k]) ++k;
    z[i] = k;
    if (0 < k && r < i + k) l = i, r = i + k;
  }
  return z;
}

vector<int> zAlgorithm(const string& s) {
  int i = 1, j = 0, n = (int)s.size();
  vector<int> ret(n);
  ret[0] = n;
  while (i < n) {
    // jを使うことで先頭から、iから、の接頭辞の一致を見ている。
    // 愚直にやるなら毎回j=0とする。が、それだとO(|S|^2)になる。
    while (i + j < n && s[j] == s[i + j]) ++j;
    ret[i] = j;   // ここでi番目先頭の場合の値は確定
    if (j == 0) { // jが進んでなかったら
      ++i;
      continue;
    }
    // jはマッチしてるので、追加で・・
    // 必ず i > kとなるので、ret[k]は更新済み。
    // s[0]~s[j] と s[i]~s[i+j]がマッチしている => k + kでの半径 < jなら、
    // i+kの半径==kの半径が言える。
    int k = 1;
    while (i + k < n && k + ret[k] < j) ret[i + k] = ret[k], ++k;
    i += k, j -= k;
  }
  return ret;
}

// kmp
// iまで見たときのシフト数
// j + zj - 1 = iとなる最小のjを探す。
// 文字列Sから文字列Tを探そうとする。i+1でマッチに失敗した。
// この時、マッチで失敗した部分の中に、同じprefixがあったら
// 先頭以外、最も左のprefixの位置に、現在の先頭のprefixを合わせてマッチを始めないといけない。
// この位置をjとする。(1<=j<i)
// j+z_j-1とは、(Tの)prefixと
// マッチしたTの中にprefixと共通する部分が2つ以上ある。
// -> それのみを考慮することは必要十分だろうか
// インデックス+prefixのマッチ、うーん、これって最小というより1つだけな気がする
// "j+z_j-1=i, iの時点でどのprefixとも一致しないなら、i-1になる。"
// ababcのとき、i=3として、i+1=4, cでマッチに失敗。
// この時、j+z_j-1=iとなる最小のjはj=2. 2右にスライドすればいいね。
// abcdefのとき、i=3として、i+1=4, eでマッチに失敗。
// j+z_j-1=iとなる最小のjはj=4. 4右にずらせばいい。
// このあと、更にprefixの有無でマッチングのスキップをするが、とりあえずここまで。
// マッチに失敗したときにどれくらい飛ばせるかを前計算する。iでマッチ失敗。
// ＊T内に[0..i]prefixと[k..i]prefixが同じ場所があれば、k進めて、k~iのマッチをさぼれる
// -> jはk進めて、iは変化なし

void ABC141E() {
  int N;
  string S;
  cin >> N >> S;
  int ans = 0;
  for (int i = 0; i < N; ++i) {
    auto z = zAlgorithm(S.substr(i, N - i));
    for (int j = 0; j < (int)z.size(); ++j) {
      if (z[j] <= j) ans = max(ans, z[j]);
    }
  }
  cout << ans;
}

int main() {
  // ABC141E();
  return 0;
}