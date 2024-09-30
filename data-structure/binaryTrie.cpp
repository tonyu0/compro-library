#pragma once

// データ管理方法は普通のTrieと同じで、数字を2進展開したものを管理する。
// その性質から、主に数字の大小に関わる、色々な操作ができる。
// 今のところ形になりそうなのは各要素にxorしたうえで何かを求めるやつ

// https://judge.yosupo.jp/submission/64573

// 二進展開した左側から処理
template <int B = 32>
class BinaryTrie {
public:
  BinaryTrie() : root(nullptr) {}
  int size() { return (root ? root->num : 0); }
  bool empty() { return !root; }
  void insert(int val) { root = add(root, val); }
  void erase(int val) { root = remove(root, val); }
  // k番目に小さい値を出します
  int query(int k) { return get(root, k); }
  // 木全体にxor_valをxorした時の最小・最大を求めます
  // 「xor_valでxorを取ると考えた際に最小値を取る実際の値」を返します
  int min_element(int xor_val = 0) { return get_xor_min(root, xor_val); }
  int max_element(int xor_val = 0) { return get_xor_min(root, ~xor_val); }
  // k未満の値を数えます
  int lower_bound(int k) { return search(root, k); }

private:
  struct Node {
    int num; // このNodeを使っている数字の数
    Node *left, *right;
    Node() : num(0), left(nullptr), right(nullptr) {}
  };

  Node *root;

  // ノードを追加 既にある場合はカウント追加のみ　右からb桁目が0 or
  // 1で左か右にノードを作るか決定
  Node *add(Node *n, int val, int b = B - 1) {
    if (n == nullptr) { n = new Node; }
    n->num++;
    if (b < 0) { return n; }

    if (val >> b & 1) {
      n->right = add(n->right, val, b - 1);
    } else {
      n->left = add(n->left, val, b - 1);
    }
    return n;
  }

  Node *remove(Node *n, int val, int b = B - 1) {
    if (n->num > 0) { n->num--; }
    if (n->num == 0) { return nullptr; }
    if (b < 0) { return n; }

    if (val >> b & 1) {
      n->right = remove(n->right, val, b - 1);
    } else {
      n->left = remove(n->left, val, b - 1);
    }
    return n;
  }

  // get k th smallest number (0-indexed)
  int get(Node *n, int k, int b = B - 1) {
    if (b < 0) { return 0; }

    int tmp = n->left ? n->left->num : 0;
    // b桁目が0である要素の数がkより少ないなら、そのままleftへ。そうでないなら、leftの要素数を除いたうえでrightへ
    return (k < tmp ? get(n->left, k, b - 1)
                    : get(n->right, k - tmp, b - 1) | (1 << b));
  }

  int get_xor_min(Node *n, int val, int b = B - 1) {
    if (b < 0) { return 0; }
    // valの右からb桁目が立ってる
    if (val >> b & 1) {
      if (n->right == nullptr) {
        return get_xor_min(n->left, val, b - 1);
      } else {
        return get_xor_min(n->right, val, b - 1) | (1 << b);
      }
    }
    // 立ってない
    if (n->left == nullptr) {
      return get_xor_min(n->right, val, b - 1) | (1 << b);
    }
    return get_xor_min(n->left, val, b - 1);
  }

  // count the num less than val
  int search(Node *n, int val, int b = B - 1) {
    if (n == nullptr || b < 0) { return 0; }
    if (val >> b & 1) {
      return search(n->right, val, b - 1) + (n->left ? n->left->num : 0);
    }
    return search(n->left, val, b - 1);
  }
};