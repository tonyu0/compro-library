#include <iostream>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <queue>
#include <map>
#define REP(i, n) for (int i = 0; i < (n); ++i)
using namespace std;
constexpr int INF = 1 << 30;

using ll = long long;

class Item
{
public:
  ll value, weight, index;
  Item() {}
  Item(ll value, ll weight, ll index) : value(value), weight(weight), index(index) {}
};

class BandB
{
public:
  vector<Item> items;
  ll N;
  ll W;
  //現在探索している商品の選び方　used_items[i]=true -> i番目の商品を選択
  vector<bool> used_items;
  //現時点で最適な商品の選び方　opt_items[i]=true -> i番目の商品を選択
  vector<bool> opt_items;
  //現時点で最適な商品の選び方をしたときの価値合計
  ll opt_value = 0;

  BandB(vector<Item> &items, ll W) : items(items), N(items.size()), W(W) {}

  //分枝限定法で探索
  //index=index-1番目の商品までは選択が終わった、index番目からの商品はまだ調べてない
  //value=今のところの価値合計
  //weight=今のところの重さ合計
  void searchbb(int index, ll value, ll weight)
  {
    //全部調べ終わった
    if (index == N || items[index].value <= 0)
    {
      if (opt_value < value)
      {
        opt_items = used_items;
        opt_value = value;
      }
      return;
    }

    //緩和問題を解く
    ll tmp_weight = weight;
    double tmp_value = value;
    for (int i = index; i < N; i++)
    {
      if (tmp_weight + items[i].weight == W)
      {
        //緩和問題の解が元の問題でも実現可能→暫定解として使えるかも
        tmp_value += items[i].value;
        if (opt_value < tmp_value)
        {
          opt_value = tmp_value;
          opt_items = used_items;
          for (int j = index; j <= i; j++)
          {
            opt_items[j] = true;
          }
        }
        return;
      }
      else if (tmp_weight + items[i].weight < W)
      {
        tmp_value += items[i].value;
        tmp_weight += items[i].weight;
      }
      else
      {
        //重さの余りを詰める
        tmp_value += (double)items[i].value * (W - tmp_weight) / items[i].weight;
        tmp_weight = W;
        break;
      }
    }

    //緩和問題ですら暫定解を超えられない→これ以上調査する意味がない
    if (tmp_value < opt_value)
    {
      return;
    }

    //重量制限が許されれば、index番目を選ぶ場合で探索
    if (weight + items[index].weight <= W)
    {
      used_items[index] = true;
      searchbb(index + 1, value + items[index].value, weight + items[index].weight);
    }
    //index番目を選ばない場合で探索
    used_items[index] = false;
    searchbb(index + 1, value, weight);
  }

  ll search()
  {
    //商品をコスパのよい順に並び替え
    sort(items.begin(), items.end(), [](const Item &x, const Item &y) {
      return ((double)x.value / x.weight) > ((double)y.value / y.weight);
    });

    //暫定解として貪欲に選んでみる
    ll tmp_weight = 0, tmp_value = 0;
    vector<bool> tmp_items(N, false);
    for (int i = 0; i < N; ++i)
    {
      if (items[i].value <= 0)
        break;
      if (tmp_weight + items[i].weight <= W)
      {
        tmp_value += items[i].value;
        tmp_weight += items[i].weight;
        tmp_items[i] = true;
      }
      else
      {
        break;
      }
    }
    opt_items = tmp_items;
    opt_value = tmp_value;
    used_items.assign(N, false);
    searchbb(0, 0, 0);
    return opt_value;
  }
};

int main()
{
  cin.tie(0);
  ios_base::sync_with_stdio(false);
  int n, m;
  cin >> n;

  vector<pair<int, int>> vw;
  vw.emplace_back(0, 0);
  for (int i = 1; i <= n; ++i)
  {
    int v, w;
    cin >> v >> w;
    vw.emplace_back(v, w);
  }
  int q;
  cin >> q;
  for (int a = 0; a < q; ++a)
  {
    int v, L;
    cin >> v >> L;
    int N = 0;
    vector<pair<int, int>> p;
    while (v > 0)
    {
      p.push_back(vw[v]);
      v /= 2;
    }
    vector<Item> items(p.size());
    for (int i = 0; i < p.size(); ++i)
    {
      items[i].value = p[i].first;
      items[i].weight = p[i].second;
      items[i].index = i;
    }
    BandB bb(items, L);
    cout << bb.search() << '\n';
  }
  return 0;
}
