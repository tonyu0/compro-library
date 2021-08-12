#include <iostream>
#include <vector>
using namespace std;
// s[i]を中心とした回文の半径を記した配列を返す。奇数長の回文のみ。
// 各位置で一回しか更新しない？
vector<int>& manacher(const string& s) {
  int i = 0, j = 0, n = (int)s.size();
  vector<int> ret(n);
  while (i < n) {
    while (i - j >= 0 && i + j < n && s[i - j] == s[i + j]) ++j;
    ret[i] = j;
    int k = 1;
    // iを中心とした半径jの回文がある。
    // 回文の性質として以下のi-k中心とi+k中心の半径は同じ。
    // --------------- i -----------------
    // ----i-k----             ----i+k----
    // ↑ここの半径はjを超えないようにする。
    while (i - k >= 0 && i + k < n && k + ret[i - k] < j)
      ret[i + k] = ret[i - k], ++k; // i-kは必ず計算済み
    // i ~ i + kは記録が終わったので、飛ぶ。
    i += k, j -= k;
  }
  return ret;
}