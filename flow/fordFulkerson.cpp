// 説明: 貪欲法+逆辺による押し戻しにより最大流問題を解くアルゴリズム

#include <iostream>
#include <vector>
using namespace std;
constexpr int INF = 1 << 30;

struct Edge
{
    int dst, cap, rev;
    Edge(int d, int c, int r, int w) : dst(d), cap(c), rev(r) {}
};

using Edges = vector<Edge>;
using Graph = vector<Edges>;

template <typename T, bool directed>
class FordFulkerson
{
public:
    int n;       // 頂点数
    Graph graph; // グラフ本体
    vector<bool> used;

    FordFulkerson(int n) : n(n), graph(n) {}
    // 容量0の逆辺も張る
    void add_edge(int src, int dst, T cap)
    {
        graph[src].emplace_back(dst, cap, graph[dst].size()); // directed
        graph[dst].emplace_back(src, directed ? 0 : cap, graph[src].size() - 1);
    }

    T dfs(int v, int t, T flow)
    {
        if (v == t)
            return flow;
        used[v] = true;
        for (auto &edge : graph[v])
        {
            if (!used[edge.dst] && edge.cap > 0)
            {
                T d = dfs(edge.dst, t, min(flow, edge.cap));
                if (d > 0)
                {
                    // 流せるなら流す
                    // 流した場合、辺の容量は減り、逆辺の容量は増える
                    edge.cap -= d;
                    graph[edge.dst][edge.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }

    T max_flow(int s, int t)
    {
        T flow = 0;
        while (true)
        {
            used.assign(n, false);
            T f = dfs(s, t, INF);
            if (f == 0)
                break;
            flow += f;
        }
        return flow;
    }
};

int main()
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
    return 0;
}