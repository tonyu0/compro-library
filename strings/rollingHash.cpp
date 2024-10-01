#include <chrono>
#include <iostream>
#include <vector>
using namespace std;

// ハッシュ値を使用したO(1)の同値性チェック
// https://en.wikipedia.org/wiki/Rabin_fingerprint
//
// 主にハッシュ値の衝突が話題になる
// https://yukicoder.me/problems/no/3015
// https://yukicoder.me/problems/804

const int bases[64] = {257, 262, 266, 275, 276, 281, 285, 290, 296, 302, 306,
                       310, 311, 313, 323, 333, 344, 345, 350, 357, 367, 370,
                       373, 402, 423, 425, 431, 440, 442, 443, 454, 457, 458,
                       462, 471, 478, 481, 487, 489, 492, 499, 501, 502, 503,
                       506, 514, 524, 532, 535, 541, 550, 552, 557, 559, 562,
                       563, 567, 570, 571, 580, 592, 597, 604, 612};
const unsigned long long base =
  bases[chrono::duration_cast<chrono::microseconds>(
          chrono::system_clock::now().time_since_epoch())
          .count() &
        63];

class rolling_hash {
  using u64 = unsigned long long;
  vector<u64> hash, power;
  rolling_hash(const string &s) {
    size_t n = s.size();
    hash.assign(n + 1, 0);
    power.assign(n + 1, 1);

    for (size_t i = 0; i < n; ++i) {
      power[i + 1] = mul(power[i], base);
      hash[i + 1] = mul(hash[i], base) + s[i];
      if (hash[i + 1] >= mod) hash[i + 1] -= mod;
    }
  }

  inline u64 mul(u64 a, u64 b) {
    // mod 2^61-1:  https://qiita.com/keymoon/items/11fac5627672a6d6a9f6
    // モンゴメリ乗算?(未習)
    // __uint128_t res = __uint128_t(a) * b;
    u64 au = a >> 31;
    u64 ad = a & mask31;
    u64 bu = b >> 31;
    u64 bd = b & mask31;
    u64 mid = ad * bu + au * bd;
    u64 midu = mid >> 30;
    u64 midd = mid & mask30;
    u64 res = au * bu * 2 + midu + (midd << 31) + ad * bd;
    res = (res >> 61) + (res & mask61);
    if (res >= mod) return res - mod;
    return res;
  }

  // [l, r)
  // s[0..7]="abcdef", s[0..3]="abc"とする
  // hash(s[3..7])=hash(s[0..7])-hash(s[0..3])*10^4
  // h = {0, a*s1, a*a*s1 + a*s2, a*a*a*s1 + a*a*s2 + a*s3,...}からもわかる
  u64 get(int l, int r) {
    u64 res = (mod + hash[r] - mul(hash[l], power[r - l]));
    if (res >= mod) return res - mod;
    return res;
  }
  static constexpr u64 mod = 0x1fffffffffffffff;
  static constexpr u64 base = 10007;
  static constexpr u64 mask30 = 1ull << 30 - 1;
  static constexpr u64 mask31 = 1ull << 31 - 1;
  static constexpr u64 mask61 = 1ull << 61 - 1;
};
