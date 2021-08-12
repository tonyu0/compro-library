#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <vector>
using namespace std;
using ll = long long;

// TODO:
// どこらへんで最長共通接尾辞を判断してるのか。。理解が曖昧なので理解する。
class AhoCorasick {
public:
  // ノード数がroot(=0)のみの状態からスタート
  AhoCorasick(const vector<string> &ss) : nodes(1) {
    for (auto s : ss) add(0, s, 0); // 渡された全ての文字列を木に追加

    queue<int> q;
    q.push(0);
    idx.push_back(0);
    // bfsによりsuffix linkを構築
    // idxには探索順に頂点番号を入れる。
    while (q.size()) {
      int v = q.front();
      q.pop();
      for (auto edge : nodes[v].child) {
        // nv = 子要素の節番号、f = 親方向への枝のようなもの
        int nv = edge.second, f = nodes[v].fail;
        // マッチに失敗した時に、最長共通接尾辞であるNodeにsuffix linkを結ぶ。
        // suffix linkをたどれるだけたどる。
        while (f != -1 && nodes[f].child.count(edge.first) == 0)
          f = nodes[f].fail;

        // nvのsuffix linkを確定させる。
        // 辿った先に共通接尾辞があった。(よくわかってない)
        if (f != -1) nodes[nv].fail = nodes[f].child[edge.first];
        // なかったので根にsuffix link
        else
          nodes[nv].fail = 0;
        q.push(nv);
        idx.push_back(nv);
      }
    }
  }

  // ある要素sを再帰的に木に追加
  void add(int cur, const string &s, int i) {
    if (i == (int)s.size()) return;
    // 子要素にs[i]がなかったら追加
    if (!nodes[cur].child.count(s[i])) {
      // この深さの節の子要素の値は、現時点での節の数
      nodes[cur].child[s[i]] = nodes.size();
      // 空のnodeを追加
      nodes.emplace_back();
    }
    add(nodes[cur].child[s[i]], s, i + 1);
  }
  bool hasChild(int cur, char c) { return nodes[cur].child.count(c) > 0; }
  int nextState(int cur, char c) { return nodes[cur].child[c]; }
  int suffixLink(int cur) { return nodes[cur].fail; }

  vector<int> &getIdx() { return idx; }
  int size() { return (int)nodes.size(); }

private:
  // 辺に文字を持たせる
  struct Node {
    // 子供 - charで繋がった先の節は、全体でint番目の節
    map<char, int> child;
    int fail;
    Node() : fail(-1) {}
  };
  vector<int> idx;
  vector<Node> nodes;
};

void JSC2019FINAL_E() {
  ll N, Q, X, Y;
  cin >> N >> Q >> X >> Y;
  vector<string> S(N), T(Q);
  for (int i = 0; i < N; ++i) cin >> S[i];
  for (int i = 0; i < Q; ++i) cin >> T[i];

  AhoCorasick aho(S);

  // まず形成した木から各ノードについて最小スコアを計算
  // 全てのS[i]についてすべてのprefixを調べ、節curでの最小値を更新していく。
  vector<ll> dp(aho.size(), 1LL << 62);
  for (int i = 0; i < N; ++i) {
    ll cur = 0, n = (ll)S[i].size();
    for (ll j = 0; j < n; ++j) {
      // 木上を辿りながら
      // prefixを全て調べる。prefixを伸ばすほどコストXは減る。
      // Sのうちのどれかと合わせるのが目的・・なので
      // dp[cur]はS[i]に合わせるのに必要な最低コスト、それを全てのSについて更新
      dp[cur] = min(dp[cur], -j * X + (n - j) * Y);
      cur = aho.nextState(cur, S[i][j]);
    }
    dp[cur] = min(dp[cur], -n * X);
  }

  // 全ての頂点からsuffix linkを辿りdpデーブルを更新
  // idxは根からの探索順。なぜかこれ通りに更新しないとWAを食らった。
  // なんとなくわかる気がする。。
  auto idx = aho.getIdx();
  for (int i : idx) {
    int f = aho.suffixLink(i);
    if (f != -1) dp[i] = min(dp[i], dp[f]);
  }

  // クエリに答える。
  // 後ろから削る・・・T[i]の各接尾辞を調べることで解決。
  // 前から削る・・・現在地からsuffix linkをたどることで必要な分は全部削れる。
  // 後ろから追加・・・木の子孫のうち、現在地から最も近い子孫を見つければ良い。
  for (int i = 0; i < Q; ++i) {
    ll cur = 0, ans = 1LL << 62, n = T[i].size();
    for (ll j = 0; j < n; ++j) {
      ans = min(ans, X * n + dp[cur]);
      while (cur != -1 && !aho.hasChild(cur, T[i][j]))
        cur = aho.suffixLink(cur);
      // failure linkがあったらたどる、なかったら根に戻る。
      // T[i]の先頭の文字を削ることに値する。中途半端な削り方はこの処理で同時にskipされる。
      if (cur == -1)
        cur = 0;
      else
        cur = aho.nextState(cur, T[i][j]);
    }
    cout << min(ans, X * n + dp[cur]) << '\n';
  }
}

int main() {
  JSC2019FINAL_E();
  return 0;
}