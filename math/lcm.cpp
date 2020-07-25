#include <iostream>
#include <vector>
using namespace std;

int gcd(int a, int b) { return b == 0 ? a : gcd(b, a % b); }
int lcm(int a, int b) { return a / gcd(a, b) * b; }
int gcd(const vector<int> &v)
{
    int ret = v[0];
    for (int i = 1; i < (int)v.size(); ++i)
        ret = gcd(ret, v[i]);
    return ret;
}
int lcm(const vector<int> &v)
{
    int ret = v[0];
    for (int i = 1; i < (int)v.size(); ++i)
        ret = lcm(ret, v[i]);
    return ret;
}
