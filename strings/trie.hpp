#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <queue>
#include <vector>
using namespace std;

// 説明:
// Trie木への文字の追加はO(|S|)、Trie木への文字列検索はO(|S|)で行える。
// マッチングに失敗した時の遷移をうまくやるとAho Corasick法になる。
// trie木に渡すデータを数値列にした方がいいと思う
class trie {
public:
  trie(std::size_t max_size) {
    size = 1; // only root
    next.assign(26, std::vector<int>(max_size, -1));
    end.assign(max_size, false);
  }

  bool search(std::string s) {
    int now = 0;
    for (char c : s) {
      if (next[c - 'a'][now] == -1) return false;
      now = next[c - 'a'][now];
    }
    return end[now];
  }

  void insert(std::string s) {
    int now = 0;
    for (char c : s) {
      if (next[c - 'a'][now] == -1) next[c - 'a'][now] = size++;
      now = next[c - 'a'][now];
    }
    end[now] = true;
  }

private:
  std::vector<std::vector<int>> next;
  std::vector<bool> end;
  std::size_t size;
};

// JOI2010 DNA Synthesizer
void JOI2010DNA() {
  int n;
  string S, s[50505];
  cin >> n >> S;
  trie trie('A');
  map<char, int> m;
  m['A'] = 0, m['T'] = 1, m['C'] = 2, m['G'] = 3;
  for (int i = 0; i < n; ++i) {
    cin >> s[i];
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
void YUKI430() {
  string S, T;
  int N;
  cin >> S >> N;
  trie trie('A');
  for (int i = 0; i < N; ++i) {
    cin >> T;
    trie.insert(T);
  }

  int ans = 0;
  for (int i = 0; i < (int)S.size(); ++i) {
    ans += trie.search(S.substr(i, (int)S.size() - i));
  }
  cout << ans << endl;
}

int main() {
  cin.tie(0);
  ios_base::sync_with_stdio(false);
  // JOI2010DNA();
  // YUKI430();
  return 0;
}
