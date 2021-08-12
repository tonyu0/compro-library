#include <iostream>
#include <vector>
using namespace std;

// using kmp algorithm, search given pattern sequence from reference sequence in
// O(N).
template <typename T>
void search(const vector<T>& s, const vector<T>& pattern) {
  vector<int> table(pattern.size() + 1);
  // table[i] := when pattern[i] is not matched, next start from
  // pattern[table[i]]
  // or
  // table[i] := maximum value x where s[0..x] == s[i-1-x..i-1]
  // 考え方のコツは、prefixとsuffixが一致してる分、suffix+1からマッチを開始できうるという考え。
  table[0] = -1;
  for (int i = 0, len = -1; i < (int)pattern.size(); ++i) {
    while (len >= 0 && pattern[i] != pattern[len]) len = table[len];
    ++len;
    table[i + 1] = len;
  }

  vector<int> pos;
  for (int i = 0, len = 0; i < (int)s.size(); ++i) {
    // 今見ている文字がマッチするまで、while loop.
    // 全くマッチしない場合は0からスタート.
    while (len >= 0 && s[i] != pattern[len]) len = table[len];
    ++len;
    if (len == pattern.size()) {
      // pos.push_back(i - len + 1);
      len = table[len];
    }
  }
  // return pos;
}

// 文字列検索や回文検索用のテーブルを作る。
// 自分自身が一致するのはもちろん計算に入れない
// そのため、例えばi==1は絶対0、i==2はs[0]==s[1]なら1, otherwise 0
vector<int>& makeTable(const string& s) {
  int n = (int)s.size();
  vector<int> ret(n);
  ret[0] = -1;
  for (int i = 0, j = -1; i < n; ++i) {
    // 不一致だったらペナルティとしてjを左に戻すイメージ。
    //　jには今までマッチしてきた数が入ってる。jは最悪-1(スタート地点)まで戻る。
    while (j >= 0 && s[i] != s[j]) j = ret[j];

    // j<i+1より、ret[j]は計算済み
    if (s[i + 1] == s[j])
      ret[i + 1] = ret[j]; // knuthパワー
    else
      ret[i] = ++j;
  }
  return ret;
}

// strの中からwordを検索する。
vector<int> kmp(const string& str, const string& word) {
  vector<int> table = makeTable(word), ret;
  int m = 0, i = 0, n = str.size();
  while (m + i < n) {
    if (word[i] == str[m + i]) {
      if (++i == (int)(word.size())) {
        ret.push_back(m);
        m = m + i - table[i];
        i = table[i];
      }
    } else {
      m = m + i - table[i];
      if (i > 0) i = table[i];
    }
  }
  return ret;
}