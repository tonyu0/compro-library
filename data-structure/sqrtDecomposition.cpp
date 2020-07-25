#include <iostream>
#include <vector>
using namespace std;

struct SqrtDecomposition
{
  int size, bucket_size;
  vector<int> data;
  vector<vector<int>> bucket;
  SqrtDecomposition(int n, vector<int> &v) : size(n), bucket_size(sqrt(n)), data(n), bucket(n)
  {
    for (int i = 0; i < size; ++i)
    {
      bucket[i / bucket_size].push_back(v[i]);
      data[i] = v[i];
    }
    for (int i = 0; i < size / bucket_size; ++i)
      sort(bucket[i].begin(), bucket[i].end());
  }

  // [l, r)
  int query(int l, int r)
  {
    int ret = 1 << 30;
    int start = l / bucket_size + 1;
    int end = r / bucket_size;
    if (start > end)
    {
      for (int i = l; i < r; ++i)
        ret = min(ret, data[i]);
      return ret;
    }
    for (int i = start; i < end; ++i)
      ret = min(ret, bucket[i][0]);
    for (int i = l; i / bucket_size < start; ++i)
      ret = min(ret, data[i]);
    for (int i = r - 1; i / bucket_size >= end; --i)
      ret = min(ret, data[i]);
    return ret;
  }
};
