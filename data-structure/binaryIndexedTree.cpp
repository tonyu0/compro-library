// update, query O(logn)
struct BinaryIndexedTree {
  vector<int> data;
  int n;
  BinaryIndexedTree(int n) : data(n+1), n(n+1) {}

  void update(int i,int x){
    for(; i <= n; i += i & -i) data[i] += x;
  }

  long query(int i){
    long ret = 0;
    for(; i > 0; i -= i & -i) ret += data[i];
    return ret;
  }
};
