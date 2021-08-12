#include <climits>
#include <iostream>
#include <random>
using namespace std;

class xor128 {
public:
  static constexpr unsigned min() { return 0; }
  static constexpr unsigned max() { return 1e9; }
  unsigned operator()() { return random(); }
  xor128() {}

private:
  unsigned x = 123456789, y = 362436069, z = 521288629, w = 88675123;
  unsigned random() {
    unsigned t;
    t = x ^ (x << 11);
    x = y;
    y = z;
    z = w;
    return w = (w ^ (x >> 19)) ^ (t ^ (t >> 8));
  }
};

template <class T>
class Treap {
  xor128 rnd;
  struct Node {
    T key;
    int priority;
    Node *l, *r;
    Node(T key, int priority)
      : key(key), priority(priority), l(nullptr), r(nullptr) {}
  }* root = nullptr;
  using Tree = Node*;

  void split(Tree t, T key, Tree& l, Tree& r) {
    if (!t)
      l = r = nullptr;
    else if (key < t->key)
      split(t->l, key, l, t->l), r = t;
    else
      split(t->r, key, t->r, r), l = t;
  }

  void merge(Tree& t, Tree l, Tree r) {
    if (!l || !r)
      t = l ? l : r;
    else if (l->priority > r->priority)
      merge(l->r, l->r, r), t = l;
    else
      merge(r->l, l, r->l), t = r;
  }

  void insert(Tree& t, Tree item) {
    if (!t)
      t = item;
    else if (item->priority > t->priority)
      split(t, item->key, item->l, item->r), t = item;
    else
      insert(item->key < t->key ? t->l : t->r, item);
  }

  void erase(Tree& t, T key) {
    if (t->key == key)
      merge(t, t->l, t->r);
    else
      erase(key < t->key ? t->l : t->r, key);
  }

  bool find(Tree& t, T key) {
    if (!t)
      return false;
    else if (t->key == key)
      return true;
    return find(key < t->key ? t->l : t->r, key);
  }

public:
  void insert(T key) { insert(root, new Node(key, rnd())); }
  void erase(T key) { erase(root, key); }
  bool find(T key) { return find(root, key); }
};

int main() {
  Treap<int> tp;
  int a, b;
  cin >> a >> b;
  tp.insert(a);
  tp.insert(b);
  cout << tp.find(a) << endl;
  tp.erase(a);
  cout << tp.find(a) << endl;
  return 0;
}
