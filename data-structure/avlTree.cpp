#pragma once
// T keyは大小関係が定義できる必要がある。
template <class T>
class avl_tree {
public:
  struct node {
    T key;
    int size, height;
    node *child[2];
    node(const T &key) : key(key), size(1), height(1) {
      child[0] = child[1] = nullptr;
    }
  } *root;
  T null;

  avl_tree(T null = 0) : root(nullptr), null(null) {}
  // find/insert/erase key: O(logN)
  T find(const T &key) {
    node *res = find(root, key);
    return res ? res->key : null;
  }
  void insert(const T &key) { root = insert(root, new node(key)); }
  void erase(const T &key) { root = erase(root, key); }
  // get k-th smallest number (0-indexed): O(logN)
  T rank(int k) const {
    node *res = rank(root, k);
    return res ? res->key : null;
  }
  // TODO: order_of_keyを実装したい
  // setを検索して、それが下から何番目か知りたくなる時がある。
  // node *order_of_key(int x) const {}
  // TODO: lower_bound, upper_boundを実装する

private:
  node *find(node *t, const T &key) {
    if (t == nullptr) { return nullptr; }

    if (key < t->key) {
      return find(t->child[0], key);
    } else if (key > t->key) {
      return find(t->child[1], key);
    }
    return t;
  }

  node *insert(node *t, node *x) {
    if (t == nullptr) { return x; } // insert x here

    if (x->key <= t->key) {
      t->child[0] = insert(t->child[0], x);
    } else {
      t->child[1] = insert(t->child[1], x);
    }
    t->size += 1;
    return balance(t);
  }

  node *erase(node *t, const T &x) {
    if (t == nullptr) { return nullptr; }

    // erase this node and move_down
    if (x == t->key) { return move_down(t->child[0], t->child[1]); }

    if (x < t->key) {
      t->child[0] = erase(t->child[0], x);
    } else {
      t->child[1] = erase(t->child[1], x);
    }
    t->size -= 1;
    return balance(t);
  }

  node *move_down(node *t, node *rhs) {
    if (t == nullptr) { return rhs; }

    t->child[1] = move_down(t->child[1], rhs);
    return balance(t);
  }

  node *rotate(node *t, int l, int r) {
    node *s = t->child[r];
    t->child[r] = s->child[l];
    s->child[l] = balance(t);

    _update_subtree_size(t);
    _update_subtree_size(s);
    return balance(s);
  }

  node *balance(node *t) {
    for (int i = 0; i <= 1; ++i) {
      if (_get_subtree_height(t->child[i ^ 1]) <
          _get_subtree_height(t->child[i]) - 1) {
        if (_get_subtree_height(t->child[i]->child[i ^ 1]) >
            _get_subtree_height(t->child[i]->child[i])) {
          t->child[i] = rotate(t->child[i], i, i ^ 1);
        }
        return rotate(t, i ^ 1, i);
      }
    }

    _update_subtree_height(t);
    _update_subtree_size(t);
    return t;
  }

  node *rank(node *t, int k) const {
    if (t == nullptr) { return nullptr; }

    int m = t->child[0] ? t->child[0]->size : 0; // left -> right
    if (k < m) {
      return rank(t->child[0], k);
    } else if (k > m) {
      return rank(t->child[1], k - m - 1);
    }
    return t; // this node is k-th smallest
  }

  /* binary search
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

  // helper
  void _update_subtree_size(node *t) {
    if (t != nullptr) {
      t->size = (t->child[0] ? t->child[0]->size : 0) +
                (t->child[1] ? t->child[1]->size : 0) + 1;
    }
  }
  void _update_subtree_height(node *t) {
    if (t != nullptr) {
      t->height = max((t->child[0] ? t->child[0]->height : 0),
                      (t->child[1] ? t->child[1]->height : 0)) +
                  1;
    }
  }
  int _get_subtree_height(node *t) { return (t ? t->height : 0); }
};