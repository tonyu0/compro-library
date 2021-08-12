#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

vector<int> enumerate_primes(int n) {
  vector<bool> is_prime(n + 1, 1);
  vector<int> primes;
  for (int i = 3; i * i <= n; i += 2)
    if (is_prime[i])
      for (int j = i * i; j <= n; j += i * 2) is_prime[j] = 0;

  if (n > 1) primes.push_back(2);
  for (int i = 3; i <= n; i += 2)
    if (is_prime[i]) primes.push_back(i);
  return primes;
}

vector<int> enumerate_primes(int n) {
  vector<int> primes{2};
  vector<bool> is_prime(n + 1);
  for (int i = 3; i <= n; i += 2) is_prime[i] = 1;
  for (long long i = 3; i <= n; i += 2) {
    if (is_prime[i]) {
      primes.push_back(i);
      for (long long j = i * i; j <= n; j += i * 2) is_prime[j] = 0;
    }
  }
  return primes;
}
// O(N) ただしメモリをくう。10^8で1024MBくらい？でもleast prime
// factorが全て取れるのは嬉しい。
vector<int> enumerate_primes(int n) {
  vector<int> primes;
  vector<int> least_prime(n + 1);
  for (int i = 2; i <= n; ++i) {
    if (least_prime[i] == 0) {
      least_prime[i] = i;
      primes.push_back(i);
    }
    for (int j = 0;
         j < primes.size() && primes[j] <= least_prime[i] && i * primes[j] <= n;
         ++j) {
      least_prime[i * primes[j]] = primes[j];
    }
  }
  return primes;
}

long long count_primes(long long n) {
  long long m = sqrt(n);
  vector<long long> nums(m); // store all n / i (1 <= i <= n) (O(N^(1/2)))
  for (long long i = 0; i < m; ++i) nums[i] = n / (i + 1);
  for (long long i = n / m - 1; i > 0; --i) nums.push_back(i);
  reverse(nums.begin(), nums.end());

  vector<long long> count(nums);
  // count(m, p) := m以下で、p番目以下の全ての素数でふるった後に残ってる数
  // count(m, p) = count(m, p - 1) - (s(m / p, p - 1) - s(p - 1, p - 1))
  vector<int> primes = enumerate_primes(m);
  // table for compression
  vector<int> table(m + 1);
  for (int i = 0; i < nums.size(); ++i)
    if (nums[i] <= m) table[nums[i]] = i;

  // count[x] := the number of primes less than or equal to x
  for (long long p : primes) {
    for (int i = nums.size() - 1; i >= 0; --i) {
      if (nums[i] < p * p) break;
      long long x = nums[i] / p;
      int j = (x <= m ? table[x] : nums.size() - (table[n / x] + 1));
      count[i] -= count[j] - count[table[p - 1]];
    }
  }
  return count.back() - 1;
}

// 5x slower compeared to above one, but simple implement
// long long count_primes(long long n) {
//   long long m = sqrt(n);
//   vector<long long> nums(m); // store all n / i(1 <= i <= n) (O(N^(1/2)))
//   unordered_map<long long, long long> count;
//   for (long long i = 0; i < m; ++i) nums[i] = n / (i + 1);
//   for (long long i = n / m - 1; i > 0; --i) nums.push_back(i);
//   for (long long e : nums) count[e] = e;

//   // count(m, p) := m以下で、p番目以下の全ての素数でふるった後に残ってる数
//   // count(m, p) = count(m, p - 1) - (s(m / p, p - 1) - s(p - 1, p - 1))
//   vector<int> primes = enumerate_primes(m);

//   // count[x] := the number of primes less than or equal to x
//   for (long long p : primes) {
//     for (long long e : nums) {
//       if (e < p * p) break;
//       count[e] -= count[e / p] - count[p - 1];
//     }
//   }
//   return count[n] - 1;
// }