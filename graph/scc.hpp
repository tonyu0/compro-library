#include <vector>
using namespace std;

// 強連結成分分解:
// 有向グラフをDAGにするようなものだが、グラフを変換する機能はまだ。つぶせる頂点グループを列挙するのとつぶした後のDAGの頂点数を出す程度。
// verify: https://atcoder.jp/contests/practice2/submissions/57493307
class StronglyConnectedComponents {
public:
  StronglyConnectedComponents(int n)
    : n(n), g(n), rg(n), used(n), components(n) {}

  void add_edge(int from, int to) { g[from].push_back(to); }

  void build() {
    for (int from = 0; from < n; ++from) {
      for (int to : g[from]) { rg[to].push_back(from); }
    }

    // dfs1
    for (int v = 0; v < n; ++v) {
      if (used[v] == 0) { dfs1(v); }
    }
    // dfs2
    reverse(order.begin(), order.end());
    for (int v : order) {
      if (used[v] == 1) { dfs2(v, k++); }
    }
    // collecting result
    scc_result.resize(k);
    for (int v = 0; v < n; ++v) { scc_result[components[v]].push_back(v); }
  }
  int size() { return k; }
  int component_of(int x) { return components[x]; } // topological sorted
  bool same(int x, int y) { return components[x] == components[y]; }
  const vector<vector<int>> &scc() { return scc_result; }

private:
  int n, k = 0;
  vector<vector<int>> g, rg;
  vector<int> used, order, components;
  // used: 0: totally unused, 1: used in dfs1, 2: used in dfs2
  vector<vector<int>> scc_result;

  // dfs for finding post order of graph
  void dfs1(int v) {
    used[v] = 1;
    for (int nv : g[v]) {
      if (used[nv] == 0) { dfs1(nv); }
    }
    order.push_back(v);
  }
  // dfs for reversed graph
  void dfs2(int v, int k) {
    used[v] = 2;
    components[v] = k;
    for (int nv : rg[v]) {
      if (used[nv] == 1) { dfs2(nv, k); }
    }
  }
};

// 2sat: (a ∨ b) ⋀ (c ∨ d) ⋀ ... みたいな∨で結ばれた項が高々2項であるもの
// (a ∨ b) → (￢a => b) ⋀ (￢b => a)　(つまり(￢a⋀￢b)が偽)
// と変換でき、2satの制約から全体を⇒と⋀で表現できる。
// ⇒は有向グラフとして考えられ、⋀は⇒がすべて真となる条件を考えることで取り除けるため、後は有向グラフ上で矛盾(a⇒￢a)が起きないなら充足可能.
// その場合、構成は一つ簡単にできる。各要素の真偽のうち、トポロジカルソート順で後になる方を採用していくと、それが一つの構成となる。order後ろから決めてくときれいに決まるやつ
// (辺の追加は制約を加えるイメージ。a⇒bが真のときにも、これは￢aについては何も制約を課していないので、￢a関連の辺の追加は不要、のように)

// verify: https://atcoder.jp/contests/practice2/submissions/57495267
class TwoSAT {
public:
  TwoSAT(int n) : n(n), scc(2 * n) {}
  // add a ∨ b (→ (￢a ⇒ b) ⋀ (￢b ⇒ a))
  void add_clause(int a, bool is_a_true, int b, bool is_b_true) {
    //
    add_implication(a, !is_a_true, b, is_b_true);
    add_implication(b, !is_b_true, a, is_a_true);
  }
  // add a ⇒ b
  void add_implication(int a, bool is_a_true, int b, bool is_b_true) {
    // a: a, ￢a: a+n
    scc.add_edge(a + (is_a_true ? 0 : n), b + (is_b_true ? 0 : n));
  }

  bool satisfiable() {
    scc.build();
    for (int i = 0; i < n; ++i) {
      if (scc.same(i, i + n)) { return false; }
    }
    return true;
  }

  // build one solution for the 2sat; use only if satisfiable() is true
  const vector<bool> &build() {
    answer.resize(n);
    for (int i = 0; i < n; ++i) {
      // i: i, ￢i: i+n
      answer[i] = scc.component_of(i) > scc.component_of(i + n);
    }
    return answer;
  }

private:
  int n;
  vector<bool> answer;
  StronglyConnectedComponents scc;
};