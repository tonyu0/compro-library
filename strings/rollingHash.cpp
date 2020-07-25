#include <iostream>
#include <vector>
using namespace std;

// 説明: ハッシュ値の累積和を取る(O(|S|))ことである区間の文字列比較をO(1)で行う。
// TODO:
// https://yukicoder.me/problems/no/6
// https://yukicoder.me/problems/no/3015
// 備考: これを読んでハッシュを根拠を持って設定する。
// https://qiita.com/keymoon/items/11fac5627672a6d6a9f6#_reference-b0f37d19c7cecde124e3

struct RollingHash
{
  vector<unsigned long long> hash[2], power[2];
  const unsigned int seed = 10007;
  const unsigned int mod[2] = {999999937, 1000000007};

  RollingHash(const string &s)
  {
    int n = (int)s.size();
    for (int m = 0; m < 2; ++m)
    {
      hash[m].assign(n + 1, 0);
      power[m].assign(n + 1, 1);

      for (int i = 0; i < n; ++i)
      {
        hash[m][i + 1] = (hash[m][i] * seed + s[i]) % mod[m];
        power[m][i + 1] = power[m][i] * seed % mod[m];
      }
    }
  }

  // [l, r)
  unsigned long long get_hash(int l, int r, int m = 0)
  {
    unsigned long long ret = (hash[m][r] - hash[m][l] * power[m][r - l] % mod[m] + mod[m]) % mod[m];
    return ret;
  }
};
