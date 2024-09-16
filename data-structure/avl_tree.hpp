#pragma once
// T needs ==, >, <. maybe can use vector?
// this avl_tree is only for procon, because this skips delete to speed up
// when use map ver, remove comment out of /* */

// verify:
// - set ver(with rank())
//    https://atcoder.jp/contests/arc033/submissions/57849054
// - map ver
//    https://judge.yosupo.jp/submission/236074

/*template <typename T, typename U>*/
template <typename T>
class avl_tree {
public:
  struct node {
    T key;
    /* U value; */
    int size, height; // subtree size / height
    node *child[2];   // 0: small, 1: large
    /* node(const T &key,const U
     * &value):key(key),value(value),size(1),height(1){ */
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
  /* std::pair<T, U> find(const T &key) {
    node *res = find(root, key);
    return res ? std::make_pair(res->key, res->value)
               : std::make_pair(null, (U)0);
  } */

  void insert(const T &key) { root = insert(root, new node(key)); }
  /* void insert(const T &key, const U &value) {
    root = insert(root, new node(key, value));
  } */

  void erase(const T &key) { root = erase(root, key); }
  // get k-th smallest number (0-indexed): O(logN)
  T rank(int k) const {
    node *res = rank(root, k);
    return res ? res->key : null;
  }
  // T order_of_key(int x) const {} // TODO
  // T lower_bound(int l, int r) const {} // TODO

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
  // returns child after insert
  node *insert(node *t, node *x) {
    if (t == nullptr) { return x; } // insert x here

    if (x->key == t->key) {
      // key duplicate, reject inserting x
      /* t->value = x->value; */
      return t;
    }

    if (x->key < t->key) {
      t->child[0] = insert(t->child[0], x);
    } else {
      t->child[1] = insert(t->child[1], x);
    }
    t->size += 1;
    return balance(t);
  }
  // returns child after erase
  node *erase(node *t, const T &x) {
    if (t == nullptr) { return nullptr; }

    // erase this node and move_down
    if (x == t->key) { return move_down(t->child[0], t->child[1]); }

    if (x < t->key) {
      t->child[0] = erase(t->child[0], x);
    } else {
      t->child[1] = erase(t->child[1], x);
    }
    t->size -= 1; // can erase multiple values?
    return balance(t);
  }
  // helper used from erase(),
  //  t        .       l
  // l r  ->  l r  ->   ..r (most descendant right of l)
  node *move_down(node *t, node *rhs) {
    if (t == nullptr) { return rhs; }

    t->child[1] = move_down(t->child[1], rhs);
    return balance(t);
  }
  //   t      r
  //  l r -> t
  //    l'  l l' (an example of l<r case)
  // (r to subtree root, if l<r, ccw rotate, if l>r, cw rotate)
  node *rotate(node *t, int l, int r) {
    // in both of l<r and r>l cases, this rotation is valid. hard to explain
    node *s = t->child[r];
    t->child[r] = s->child[l];
    s->child[l] = balance(t);
    update(t);
    update(s);
    return balance(s);
  }
  // returns subtree root of t after rotation
  node *balance(node *t) {
    for (int i = 0; i <= 1; ++i) {
      // balance factor(bf) := left height - right height
      if (balance_factor(t->child[!i], t->child[i]) < -1) {
        // if bf < -1 or bf > 1, then need rotate. in this case, it means i's
        // subtree is too large. improve this situation by rotation.
        if (balance_factor(t->child[i]->child[!i], t->child[i]->child[i]) > 0) {
          // in this case, above bf cannot be positive(in short, it leads to be
          // a bad rotation), then first rotate i's child.
          t->child[i] = rotate(t->child[i], i, !i);
        }
        // need make i the root of t's subtree
        return rotate(t, !i, i);
      }
    }

    update(t);
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

  // helper
  void update(node *t) {
    if (t != nullptr) {
      t->size = (t->child[0] ? t->child[0]->size : 0) +
                (t->child[1] ? t->child[1]->size : 0) + 1;
      t->height = max(t->child[0] ? t->child[0]->height : 0,
                      t->child[1] ? t->child[1]->height : 0) +
                  1;
    }
  }
  int balance_factor(node *l, node *r) {
    return (l ? l->height : 0) - (r ? r->height : 0);
  }
};