#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;
using ll = long long;

// https://yukicoder.me/problems/no/836
// [0, R]でdivで割った余りがmodである個数
ll countMultiple(ll R, ll div, ll mod)
{
    if (R == 0)
        return 0;

    ll res = R / div;
    if (mod <= R % div && 0 < mod)
        res++;
    return res;
}
ll countMultiple(ll L, ll R, ll div, ll mod)
{ // [L,R] and x % div == mod
    return countMultiple(R, div, mod) - countMultiple(L - 1, div, mod);
}

// 座標圧縮
// zip[original] to retrieve compressed value
long long unzip[100010];
map<long long, int> zip;
sort(unzip.begin(), unzip.end());
unzip.erase(unique(unzip.begin(), unzip.end()), unzip.end());
for (int i = 0; i < zip.size(); ++i)
    zip[unzip[i]] = i;

// ランレングス圧縮
vector<pair<char, int>> RLE(string s)
{
    vector<pair<char, int>> ret;
    for (int i = 0; i < (int)s.size();)
    {
        int cnt = 0;
        while (s[i] == s[i + cnt])
            ++cnt;
        ret.emplace_back(s[i], cnt);
        i += cnt;
    }
    return ret;
}
