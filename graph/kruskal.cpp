#include <iostream>
#include <vector>
using namespace std;

// O(|E|log|E|)
int kruskal(Edges &es, int n)
{
    UnionFind uf(n);
    sort(es.begin(), es.end());
    int min_cost = 0;
    // Edges mst;

    for (Edge e : es)
    {
        if (!uf.same(e.src, e.dst))
        {
            uf.unite(e.src, e.dst);
            // mst.push_back(e);
            min_cost += e.cost;
        }
    }
    return min_cost;
}
