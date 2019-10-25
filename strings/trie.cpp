#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>
#include <map>
using namespace std;

// 説明: 
// Trie木への文字の追加はO(|S|)、Trie木への文字列検索はO(|S|)で行える。
// マッチングに失敗した時の遷移をうまくやるとAho Corasick法になる。
// TODO: 英小文字以外にも対応する。問題の整理をする。

class Trie {
  public:
    Trie(){ root = makeNode(); }

    void insert(string s) {
      Node* now = root;
      for(int i = 0; i < (int)s.size(); ++i) {
        int next = s[i] - 'a';
        if(now->child[next] == nullptr) now->child[next] = makeNode();
        now = now->child[next];
      }
      now->end = true;
    }

    bool search(string s) {
      Node* now = root;
      for(int i = 0; i < (int)s.size(); ++i) {
        int next = s[i] - 'a';
        if(now->child[next] == nullptr) return false;
        now = now->child[next];
      }
      return now->end;
    }
  private:
    struct Node {
      Node* child[26];
      bool end;
    };

    Node* root;
    Node* makeNode() {
      Node* node = new Node;
      node->end = false;
      for(int i = 0; i < 26; ++i) node->child[i] = nullptr;
      return node;
    }
};

int n;
string S, s[50505];
 
// JOI2010 DNA Synthesizer
int main() {
  cin.tie(0); ios_base::sync_with_stdio(false);
  cin >> n >> S;
  Trie trie;
  m['A'] = 0, m['T'] = 1, m['C'] = 2, m['G'] = 3;
  for(int i = 0; i < n; ++i) {
    cin >> s[i];
    reverse(s[i].begin(), s[i].end());
    trie.insert(s[i]);
  }
 
  int N = (int)S.size();
  vector<int> dp(N+1, 1<<30);
  dp[0] = 0;
  for(int i = 1; i < N; ++i) {
    for(int j = 1; j < 21; ++j) {
      if(i - j < 0) continue;
      string target = S.substr(i-j, j+1);
      reverse(target.begin(), target.end());
      if(trie.search(target)) {
        for(int k = i-j; k < i; ++k)
          dp[i] = min(dp[i], dp[k] + 1);
      }
    }
  } 
  cout << dp[N-1] << endl;
  return 0;
}
