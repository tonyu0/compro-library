#include <algorithm>
#include <cmath>
#include <vector>

class Mo {
public:
  Mo(int n, int q) : bucket_size((int)sqrt(n)), query_idx(q), in(n) {
    for (int i = 0; i < q; ++i) query_idx[i] = i;
  }

  // [l, r)
  void push(int l, int r) {
    left.emplace_back(l);
    right.emplace_back(r);
  }

  void run() {
    std::sort(begin(query_idx), end(query_idx), [&](int i, int j) {
      if (left[i] / bucket_size != left[j] / bucket_size)
        return left[i] < left[j]; // 違うバケットにいたら
      else if (left[i] / bucket_size % 2 == 0)
        return right[i] < right[j]; // 同じバケットにいたら
      return right[i] > right[j];
    });
    int nl = 0, nr = 0;
    for (int idx : query_idx) {
      while (nl > left[idx]) move(--nl);
      while (nr < right[idx]) move(nr++);
      while (nl < left[idx]) move(nl++);
      while (nr > right[idx]) move(--nr);
      solve(idx);
    }
  }

private:
  int bucket_size;
  std::vector<int> left, right, query_idx;
  std::vector<bool> in;
  void move(int idx) {
    in[idx].flip();
    if (in[idx])
      insert(idx);
    else
      erase(idx);
  }

  // 問題ごとに変える
  void insert(int idx);
  void erase(int idx);
  void solve(int idx);
};

/* 例: ABC174
// ---- mo 用 ----
vector<int> cnt(500001), a, ans;
int num_kind = 0;

void Mo::insert(int idx) {
  if (cnt[a[idx]] == 0) ++num_kind;
  ++cnt[a[idx]];
}

void Mo::erase(int idx) {
  if (cnt[a[idx]] == 1) --num_kind;
  --cnt[a[idx]];
}

void Mo::solve(int idx) { ans[idx] = num_kind; }
// ----
*/