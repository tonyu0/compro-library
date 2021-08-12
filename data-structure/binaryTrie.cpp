#pragma once



// min_element / max_element: エラーをはいて動かない
// lower_bound: 0しか出力されない
constexpr int B = 64;
class BinaryTrie {
public:
    BinaryTrie() : root(nullptr) {}
    // use
    int  size() { return (root ? root->num : 0); }
    bool empty() { return !root; }
    void insert(int val) { root = add(root, val); }
    void erase(int val) { root = remove(root, val); }
    int  query(int k) { return get(root, k); }
    int  min_element(int bias = 0) { return get_xor_min(root, bias); }
    int  max_element(int bias = 0) { return get_xor_min(root, ~bias); }
    int  lower_bound(int k) { return search(root, k); }
private:
    // node of trie
    struct node {
        int num;
        node* left, * right;
        node() : num(0), left(nullptr), right(nullptr) {}
    };

    // root pointer
    node* root;

    // add val to trie
    node* add(node* n, int val, int b = B - 1) {
        if (!n) n = new node;
        n->num++;
        if (b < 0) return n;

        if (val & (1 << b)) n->right = add(n->right, val, b - 1);
        else             n->left = add(n->left, val, b - 1);
        return n;
    }

    // remove val from trie
    node* remove(node* n, int val, int b = B - 1) {
        if (n->num == 1) return nullptr;
        n->num--;
        if (b < 0) return n;

        if (val & (1 << b)) n->right = remove(n->right, val, b - 1);
        else             n->left = remove(n->left, val, b - 1);
        return n;
    }

    // get k th smallest number (0-indexed)
    int get(node* n, int k, int b = B - 1) {
        if (b < 0) return 0;

        int tmp = n->left ? n->left->num : 0;
        return (k < tmp ? get(n->left, k, b - 1) : get(n->right, k - tmp, b - 1) | (1 << b));
    }

    int get_xor_min(node* n, int val, int b = B - 1) {
        if (b < 0) return 0;
        bool f = (val & (1 << b));
        if (f) {
            f ^= !n->right;
            return get_xor_min(n->right, val, b - 1) | (f << b);
        }
        f ^= !n->left;
        return get_xor_min(n->left, val, b - 1) | (f << b);
    }

    // count the num less than val
    int search(node* n, int val, int b = B - 1) {
        if (!n || b < 0) return 0;
        bool f = (val >> b) & 1;
        if (f) return search(n->right, val, b - 1) + (n->left ? n->left->num : 0);
        else  return search(n->left, val, b - 1);
    }

};
