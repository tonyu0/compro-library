#include <iostream>
#include <vector>
using namespace std;

struct SuffixArray
{
    string str;
    vector<int> sa, rnk;

    SuffixArray(string &str) : str(str)
    {
        int n = (int)str.size(), itr = 1;

        auto comp = [&](int a, int b) {
            if (rnk[a] != rnk[b])
                return rnk[a] < rnk[b];
            return (a + itr < n ? rnk[a + itr] : -1) < (b + itr < n ? rnk[b + itr] : -1);
        };

        for (int i = 0; i < n; ++i)
            sa.push_back(i), rnk.push_back(str[i]);

        vector<int> tmp(n);
        for (; itr <= n; itr *= 2)
        {
            sort(sa.begin(), sa.end(), comp);

            tmp[sa[0]] = 0;
            for (int i = 1; i < n; ++i)
                tmp[sa[i]] = tmp[sa[i - 1]] + comp(sa[i - 1], sa[i]);
            rnk = tmp;
        }
    }

    bool find(string &t)
    {
        int ng = -1, ok = (int)str.size() - 1;
        while (ok - ng > 1)
        {
            int mid = (ok + ng) / 2;
            if (str.substr(sa[mid], t.size()) < t)
                ng = mid;
            else
                ok = mid;
        }
        return str.substr(sa[ok], t.size()) == t;
    }
};
