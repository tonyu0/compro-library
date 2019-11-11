// 説明: 貪欲法+逆辺による押し戻しにより最大流問題を解くアルゴリズム

#include <iostream>
#include <vector>
#include <limits>
using namespace std;

// 有向グラフかどうかの情報も渡す
template <typename T, bool directed>
class FordFulkerson
{
public:
    int n;                   // 頂点数
    vector<vector<T>> graph; // グラフ本体(隣接行列)
    vector<int> used;        // checkと等しかったら使用済み
    int check = 1;           // while文の中でusedの初期化をしないための変数

    FordFulkerson(int n) : n(n), graph(n, vector<T>(n)), used(n) {}

    void add_edge(int src, int dst, T cap)
    {
        graph[src][dst] = cap;
        graph[dst][src] = directed ? 0 : cap;
    }

    T dfs(int v, int t, T flow)
    {
        if (v == t)
            return flow;
        used[v] = check;
        for (int nv = 0; nv < n; ++nv)
        {
            if (graph[v][nv] == 0 || used[nv] == check)
                continue;
            T d = dfs(nv, t, min(flow, graph[v][nv]));
            if (d)
            {
                // 流せるなら流す
                // 流した場合、辺の容量は減り、逆辺の容量は増える
                graph[v][nv] -= d;
                graph[nv][v] += d;
                return d;
            }
        }
        return 0;
    }

    T max_flow(int s, int t)
    {
        T flow = 0;
        while (true)
        {
            T f = dfs(s, t, numeric_limits<T>::max());
            if (f == 0)
                break;
            flow += f;
            ++check;
        }
        return flow;
    }
};

// https://atcoder.jp/contests/abc010/tasks/abc010_4
void ABC010D()
{
    int N, G, E, p, a, b;
    cin >> N >> G >> E;
    FordFulkerson<int, false> ff(N + 1);
    for (int i = 0; i < G; ++i)
    {
        cin >> p;
        ff.add_edge(p, N, 1);
    }
    for (int i = 0; i < E; ++i)
    {
        cin >> a >> b;
        ff.add_edge(a, b, 1);
    }
    cout << ff.max_flow(0, N) << endl;
}

int main()
{
    ABC010D();
    return 0;
}