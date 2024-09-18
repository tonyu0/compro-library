#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <random>
#include <set>
#include <vector>
using namespace std;
using ll = long long;

// https://yukicoder.me/problems/no/836
// [0, R]でdivで割った余りがmodである個数
ll countMultiple(ll R, ll div, ll mod) {
  if (R == 0) return 0;

  ll res = R / div;
  if (mod <= R % div && 0 < mod) res++;
  return res;
}
ll countMultiple(ll L, ll R, ll div, ll mod) { // [L,R] and x % div == mod
  return countMultiple(R, div, mod) - countMultiple(L - 1, div, mod);
}

// ランレングス圧縮
vector<pair<char, int>> RLE(string s) {
  vector<pair<char, int>> ret;
  for (int i = 0; i < (int)s.size();) {
    int cnt = 0;
    while (s[i] == s[i + cnt]) ++cnt;
    ret.emplace_back(s[i], cnt);
    i += cnt;
  }
  return ret;
}

// 座標圧縮
template <typename T>
class CoordinateCompression {
  vector<T> compressed;
  CoordinateCompression(const vector<T>& to_compress)
    : compressed(to_compress) {
    sort(compressed.begin(), compressed.end());
    compressed.erase(unique(compressed.begin(), compressed.end()),
                     compressed.end());
  }
  int index(T val) {
    return lower_bound(compressed.begin(), compressed.end(), val) -
           compressed.begin();
  }
};

/*
A からスタートして、D を足していくとき、個数 mod B の最大は，g = gcd(B, D)
として B−g+(A mod g) となり，これを C と比較すればよいです。
個数 mod gが常に一定である(個数はAにB引いてD足す繰り返しなので)
ことを考えるとこれが上界であるこ とは言え、また，(B/g − 1) ×
inv(D/g, B/g) 回 D を足したときにこの上界が達成できます (inv(X, Y ) は， X × inv
= 1 mod Y なる値とします)。
*/

// Dの倍数とBの剰余について考えるときは、とりあえずgcdで思考範囲を減らしておこう。
// 因みにDの倍数のmod Bの周期はB / gcd(B, D), mod Bの最大値はB - gcd(B, D)
// (あんま理解できてない)

// offsetからスタート、addを足していくときのmod modの最大値
ll add_max_mod(ll offset, ll add, ll mod) {
  ll g = gcd(add, mod);
  return mod - g + offset % g;
}

const int INF = 1 << 30;
ll saturating_mul(ll a, ll b) {
  if (a > INF / b) return INF;
  return a * b;
}

// 集合を表す数Sから各要素の状態を取る。b=要素数
void get_state(int S, int b, vector<int>& state) {
  for (int i = 0; i < state.size(); ++i) {
    state[i] = S % b;
    S /= b;
  }
}

struct custom_hash {
  static uint64_t splitmix64(uint64_t x) {
    // http://xorshift.di.unimi.it/splitmix64.c
    x += 0x9e3779b97f4a7c15;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x ^ (x >> 31);
  }

  size_t operator()(uint64_t x) const {
    static const uint64_t FIXED_RANDOM =
      chrono::steady_clock::now().time_since_epoch().count();
    return splitmix64(x + FIXED_RANDOM);
  }
};

// unordered_map<int, int, Hash> mpみたいにする
int base = random_device()();

struct Hash {
  int operator()(const int& x) const {
    int h = hash<int>{}(x);
    int seed = base;
    seed ^= h + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return seed;
  }
};

// しゃくとりをバグらすので、抽象化したい
// 条件を満たす区間の数えアゲ
// sumの初期値と演算を与えればよさそう？
int two_pointers(int n, vector<int>& a, int x) {
  ll res = 0;
  auto f = [](int l, int r) { return r - l; };
  for (int l = 0, r = 0; l < n; ++l) {
    while (r < n && f(l, r)) r++;
    // aaaa
  }
  return res;
}

vector<int> divide(int bit, int n) {
  vector<int> nums;
  int ten = 10;
  while (bit) {
    if (bit & 1) {
      nums.push_back(n % ten);
      n /= ten;
      ten = 10;
    } else
      ten *= 10;
    bit >>= 1;
  }
  nums.push_back(n);
  return nums;
}

// https://atcoder.jp/contests/abc141/submissions/19485384
// HashSet, 入れたいところ、値を指定。binの数は14以外にも指定.
struct HashSet {
  vector<pair<ll, ll>> data[1 << 14];
  ll get(ll at, ll val) {
    for (auto& i : data[at & (1 << 14) - 1])
      if (i.first == at) return val - i.second;
    data[at & (1 << 14) - 1].emplace_back(at, val);
    return 0;
  }
};

uint64_t gcd(uint64_t a, uint64_t b) {
  if (a == 0) return b;
  if (b == 0) return a;
  int shift = __builtin_ctzll(a | b);
  a >>= __builtin_ctzll(a);
  do {
    b >>= __builtin_ctzll(b);
    if (a > b) std::swap(a, b);
    b -= a;
  } while (b);
  return (a << shift);
}

template <typename T>
int popcount0(T x) {
  int res = 0;
  while (x) {
    x &= x - 1;
    ++res;
  }
  return res;
}

int popcount1(long long x) {
  x -= ((x >> 1) & 0x5555555555555555);
  x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333);
  x = (x + (x >> 4)) & 0x0f0f0f0f0f0f0f0f;
  x += x >> 8;
  x += x >> 16;
  x += x >> 32;
  return x & 0x7f;
}

// calc a[n](1 <= n <= N).
// fix some number k(1 <= k <= n - 1), there are two cases:
// 1. n番目の数字がk -> このnとkを交換することでa[n-2]の状態と同じ
// 2. n番目の数字がkでない -> nとkを同一視できるのでa[n-1]
// それぞれkはn-1通りあるのでa[n] = (n-1)(a[n-1]+a[n-2])
// ＊順列なので、特定の交換により状態が減る。特にn番目と何かを交換した時の状態の変化を見る。

vector<long long> calc_montmort_number(int n) {
  vector<long long> a(n + 1);
  a[0] = 1;
  for (long long i = 2; i <= n; ++i) a[i] = (a[i - 1] + a[i - 2]) * (i - 1);
  return a;
}

void stirling_number_second(int n) {
  //
  vector<vector<ll>> S(n + 1, vector<u64>(n + 1));
  S[0][0] = 1;
  for (int i = 1; i <= n; ++i) {
    for (int j = 0; j <= n; ++j) {
      // ball 1 belongs to some group
      S[i][j] += j * S[i - 1][j];
      // ball 1 makes one group
      if (j > 0) S[i][j] += S[i - 1][j - 1];
    }
  }
  for (int i = 0; i <= n; ++i) cout << S[n][i] << ' ';
  cout << endl;
}

void SegmentSet() {
  const ll INFL = 1LL << 60;
  set<pair<ll, ll>> st;
  st.insert(make_pair(-INFL, -INFL));
  st.insert(make_pair(INFL, INFL));

  rep(i, 0, n) {
    ll l, c;
    cin >> l >> c;
    ll r = l;
    // [l, r)
    auto itr = st.lower_bound(make_pair(l, r));

    // 前の区間がつなげるならつなぐ
    --itr;
    if (itr->first <= l && l <= itr->second) {
      l = min(l, itr->first);
      r = max(r, itr->second);
      st.erase(itr);
    }

    itr = st.lower_bound(make_pair(l, r));
    while (l <= itr->first && itr->first < r + c) {
      c -= max(0LL, itr->first - r);
      r = max(r, itr->second);
      itr = st.erase(itr);
    }
    r += c;
    cout << r - 1 << '\n';
    st.insert(make_pair(l, r));
  }
}