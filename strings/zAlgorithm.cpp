#include <iostream>
#include <vector>
using namespace std;

vector<int> zAlgorithm(const string &s) {
  int i = 1, j = 0, n = (int)s.size();
  vector<int> ret(n);
  ret[0] = n;
  while(i < n) {
    // jを使うことで先頭から、iから、の接頭辞の一致を見ている。
    // 愚直にやるなら毎回j=0とする。が、それだとO(|S|^2)になる。
    while(i + j < n && s[j] == s[i+j]) ++j;
    ret[i] = j; // ここでi番目先頭の場合の値は確定
    if(j == 0) { // jが進んでなかったら
      ++i; continue;
    }
    // jはマッチしてるので、追加で・・
    // 必ず i > kとなるので、ret[k]は更新済み。
    // s[0]~s[j] と s[i]~s[i+j]がマッチしている => k + kでの半径 < jなら、 i+kの半径==kの半径が言える。
    int k = 1;
    while(i + k < n && k + ret[k] < j) ret[i+k] = ret[k], ++k;
    i += k, j -= k;
  }
  return ret;
}

void ABC141E() {
  int N; string S;
  cin >> N >> S;
  int ans = 0;
  for(int i = 0; i < N; ++i) {
    auto z = zAlgorithm(S.substr(i, N-i));
    for(int j = 0; j < (int)z.size(); ++j) {
      if(z[j] <= j) ans = max(ans, z[j]);
    }
  }
  cout << ans;
}

int main() {
  ABC141E();
  return 0;
}