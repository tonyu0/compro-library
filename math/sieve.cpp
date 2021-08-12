#include <vector>
using namespace std;

const int MAX = 200000;
void sieve_least_divisor(vector<int> &lst) {
  lst.assign(MAX, 0);
  for (int i = 2; i < MAX; ++i) lst[i] = i;
  for (int i = 2; i < MAX; ++i)
    for (long long j = 1ll * i * i; j < MAX; j += i)
      if (lst[j] == j) lst[j] = i;
}