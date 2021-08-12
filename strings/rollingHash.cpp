#include <chrono>
#include <iostream>
#include <vector>
using namespace std;

// 説明:
// ハッシュ値の累積和を取る(O(|S|))ことである区間の文字列比較をO(1)で行う。
// TODO:
// https://yukicoder.me/problems/no/3015
// mod a, base bのもとで、hash値が同じになるような2つの文字列を出力する

// 参考
// https://qiita.com/keymoon/items/11fac5627672a6d6a9f6#_reference-b0f37d19c7cecde124e3
// モンゴメリ乗算？
// i128 t = a * b;
// t = (t + ((t & MOD) << 61) - (t & MOD)) >> 61;
// if(t >= MOD) return t - MOD;

// Hackを防ぐためbaseはランダム(baseが分かったところでhackする方法を知らないが)
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
  rolling_hash(const string& s) {
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
  // hash(s[0..7])=1234567,
  // hash(s[0..3])=123とすると、hash(s[3..7])=hash(s[0..7])-hash(s[0..3])*10^4
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
