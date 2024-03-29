#pragma once

// T keyは大小関係が定義できる必要がある。
// usage:
// avl_tree<T> set;
// 値の検索: set.find(x: T) (O(logN)
// 値の追加: set.insert(x: T) (O(logN))
// 値の削除: set.erase(x: T) (O(logN))
// k番目に小さな値の取得: set.rank(k: int) (O(logN))
template <class T>
class avl_tree {
public:
  struct node {
    T key;
    int size, height;
    node *child[2];
    node(const T &key) : key(key), size(1), height(1) {
      child[0] = child[1] = 0;
    }
  } * root;

  avl_tree() { root = NULL; }
  // keyと一致する要素が発見されればその参照を、
  // されなければNullを返す
  node *find(const T &key) { return find(root, key); }
  // 要素の挿入
  void insert(const T &key) { root = insert(root, new node(key)); }
  // 要素の削除
  void erase(const T &key) { root = erase(root, key); }
  // k番目(0-indexed)に小さな値を取得
  // またの名を、find_by_order
  node *rank(int k) const { return rank(root, k); }
  // TODO: order_of_keyを実装したい
  // setを検索して、それが下から何番目か知りたくなる時がある。
  // node *order_of_key(int x) const {}
  // TODO: lower_bound, upper_boundを実装する

private:
  node *find(node *t, const T &key) {
    if (t == NULL) return NULL;
    if (key == t->key)
      return t;
    else if (key < t->key)
      return find(t->child[0], key);
    else
      return find(t->child[1], key);
  }
  node *insert(node *t, node *x) {
    if (t == NULL) return x;
    if (x->key <= t->key)
      t->child[0] = insert(t->child[0], x);
    else
      t->child[1] = insert(t->child[1], x);
    t->size += 1;
    return balance(t);
  }
  node *erase(node *t, const T &x) {
    if (t == NULL) return NULL;
    if (x == t->key) {
      return move_down(t->child[0], t->child[1]);
    } else {
      if (x < t->key)
        t->child[0] = erase(t->child[0], x);
      else
        t->child[1] = erase(t->child[1], x);
      t->size -= 1;
      return balance(t);
    }
  }
  node *move_down(node *t, node *rhs) {
    if (t == NULL) return rhs;
    t->child[1] = move_down(t->child[1], rhs);
    return balance(t);
  }
#define sz(t) (t ? t->size : 0)
#define ht(t) (t ? t->height : 0)
  node *rotate(node *t, int l, int r) {
    node *s = t->child[r];
    t->child[r] = s->child[l];
    s->child[l] = balance(t);
    if (t) t->size = sz(t->child[0]) + sz(t->child[1]) + 1;
    if (s) s->size = sz(s->child[0]) + sz(s->child[1]) + 1;
    return balance(s);
  }
  node *balance(node *t) {
    for (int i = 0; i < 2; ++i) {
      if (ht(t->child[!i]) - ht(t->child[i]) < -1) {
        if (ht(t->child[i]->child[!i]) - ht(t->child[i]->child[i]) > 0)
          t->child[i] = rotate(t->child[i], i, !i);
        return rotate(t, !i, i);
      }
    }
    if (t) t->height = max(ht(t->child[0]), ht(t->child[1])) + 1;
    if (t) t->size = sz(t->child[0]) + sz(t->child[1]) + 1;
    return t;
  }
  // 小さい順にk番目の要素がどんなkeyを持つのかを調べる
  /*
       * 要素数を保持しておいて、木内でi番目の要素が
       * 何なのかが分かるのを利用して二分探索をするとか。
              int a = 目的の値
              int st = -1, ed = tree.size(), mid;        //xxxooo, (st, ed]
              while (ed - st >= 2) {
                  mid = (st + ed) / 2;
                  if ((tree.rank(mid))->key >= a) ed = mid;
                  else st = mid;
              }
              cout << sz - ed << endl;
       */
  node *rank(node *t, int k) const {
    if (!t) return NULL;
    int m = sz(t->child[0]);
    if (k < m) return rank(t->child[0], k);
    if (k == m) return t;
    if (k > m) return rank(t->child[1], k - m - 1);
  }
};
