#include <iostream>
#include <vector>
using namespace std;
constexpr int INF = 1e9;

struct Edge {
    int to, cap, rev, weight;
    Edge(int t, int c, int r, int w) : to(t), cap(c), rev(r), weight(w){}
};

using Edges = vector<Edge>;
using Graph = vector<Edges>;



class Flow {
    public:
        Flow(int v){
            mGraph.resize(v);
            mUsed.assign(v,false);
            mVert = v;
        }
        void add_edge(int from, int to, int cap, int weight = 1){
            mGraph[from].emplace_back(to,cap,mGraph[to].size(), weight); // directed
            mGraph[to].emplace_back(from,0,mGraph[from].size()-1, -weight);
        }

        // use after add all edges
        int bipartite_matching(int x, int y){
            int start = max(x,y)*2;
            int end = max(x,y)*2 + 1 ;
            for(int i = 0; i < x; ++i) add_edge(start, i, 1);
            for(int i = 0; i < y; ++i) add_edge(i+x, end, 1);
            return max_flow(start,end);
        }
        // verify : GRL_7_A

        // ford-fulkerson
        int dfs(int v, int t, int f){
            if(v==t) return f;
            mUsed[v] = true;
            for(auto &e : mGraph[v]){
                if(!mUsed[e.to] && e.cap > 0){
                    int d = dfs(e.to,t,min(f,e.cap));
                    if(d > 0){
                        e.cap -= d;
                        mGraph[e.to][e.rev].cap += d;
                        return d;
                    }
                }
            }
            return 0;
        }

        int max_flow(int s,int t){
            int flow = 0;
            while(true){
                mUsed.assign(mVert,false);
                int f = dfs(s,t,INF);
                if(f==0) break;
                flow += f;
            }
            return flow;
        }
        // verify : GRL_6_A

        // min_cost_flow
        int min_cost_flow(int s, int t, int f){
            int res = 0;
            vector<int> prevv(mVert);
            vector<int> preve(mVert);
            while(f > 0){
                // bellman_ford
                vector<int> dst(mVert,INF);
                dst[s] = 0;
                while(true){
                    bool update = false;
                    for(int i = 0; i < mVert; ++i){
                        if(dst[i] == INF) continue;
                        for(int j = 0; j < mGraph[i].size(); ++j){
                            auto e = mGraph[i][j];
                            if(e.cap > 0 && dst[e.to] > dst[i]+e.weight){
                                dst[e.to] = dst[i] + e.weight;
                                prevv[e.to] = i;
                                preve[e.to] = j;
                                update = true;
                            }
                        }
                    }
                    if(!update) break;
                }
                if(dst[t] == INF) return -1;
                // 経路を後ろから辿り、流せる最大容量を確定
                int d = f;
                for(int i = t; i != s; i = prevv[i]){
                    d = min(d, mGraph[prevv[i]][preve[i]].cap);
                }
                f -= d;
                res += d * dst[t];
                // 流す
                for(int i = t; i != s; i = prevv[i]){
                    Edge &e = mGraph[prevv[i]][preve[i]];
                    e.cap -= d;
                    mGraph[i][e.rev].cap += d;
                }
            }
            return res;
        }
        // verify : GRL_6_B


    private:
        Graph mGraph;
        vector<bool> mUsed;
        int mVert;

};

int main(){
    int X,Y,E,u,v;
    cin >> X >> Y >> E;
    Flow f(max(X,Y)*2+2);
    for(int i=0; i < E; ++i){
        cin >> u >> v;
        f.add_edge(u,v+X,1);
    }
    cout << f.bipartite_matching(X,Y) << '\n';
    return 0;
}


