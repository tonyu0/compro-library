#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <queue>
#include <vector>
using namespace std;

// 文字列追加・検索 O(|S|)
// マッチングに失敗した時の遷移をうまくやるとAho Corasick法になる。
template <int set_size = 26, char base_char = 'a'>
class trie {
  struct Node {
    Node *next[set_size] = {nullptr};
    // int count = 0; // if need node dup count
    bool isleaf = false;
  };

public:
  trie() : nodes(1, new Node) {} // add the root node
  bool search(const std::string &s) {
    Node *now = nodes[0];
    for (const char &c : s) {
      int i = c - base_char;
      if (!now->next[i]) { return false; }
      now = now->next[i];
    }
    return now->isleaf;
  }
  void insert(const std::string &s) {
    Node *now = nodes[0];
    for (const char &c : s) {
      int i = c - base_char;
      if (!now->next[i]) {
        nodes.push_back(new Node);
        now->next[i] = nodes.back();
      }
      // now->next[c]->count++;
      now = now->next[i];
    }
    now->isleaf = true;
  }

private:
  std::vector<Node *> nodes;
};

// https://atcoder.jp/contests/joisc2010/tasks/joisc2010_dna
void JOI2010DNA() {
  int n;
  string S, s[50505];
  cin >> n >> S;
  for (char &c : S) {
    if (c == 'G')
      c = 'B';
    else if (c == 'T')
      c = 'D';
  }
  trie<4, 'A'> trie;
  for (int i = 0; i < n; ++i) {
    cin >> s[i];
    for (char &c : s[i]) {
      if (c == 'G')
        c = 'B';
      else if (c == 'T')
        c = 'D';
    }
    reverse(s[i].begin(), s[i].end());
    trie.insert(s[i]);
  }

  int N = (int)S.size();
  vector<int> dp(N + 1, 1 << 30);
  dp[0] = 0;
  for (int i = 1; i < N; ++i) {
    for (int j = 1; j < 21; ++j) {
      if (i - j < 0) continue;
      string target = S.substr(i - j, j + 1);
      reverse(target.begin(), target.end());
      if (trie.search(target)) {
        for (int k = i - j; k < i; ++k) dp[i] = min(dp[i], dp[k] + 1);
      }
    }
  }
  cout << dp[N - 1] << endl;
}

// https://yukicoder.me/problems/no/430