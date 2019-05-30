#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class UnionFind {
    public:
        UnionFind(){}
        explicit UnionFind(int size){
            mPar.resize(size);
            mRnk.assign(size,0);
            for(int i = 0; i < size; ++i) mPar[i] = i;
        }

        int find(int x){
            if(mPar[x]==x) return x;
            return mPar[x] = find(mPar[x]);
        }

        bool same(int x, int y){
            return find(x) == find(y);
        }

        void unite(int x, int y){
            x = find(x);
            y = find(y);
            if(x==y) return;
            if(mRnk[x] < mRnk[y]) swap(x,y);
            mPar[y] = x;
            if(mRnk[x]==mRnk[y]) ++mRnk[x];
        }
    private:
        vector<int> mPar,mRnk;
};

struct Edge{
    int from,to,cost;
    Edge(int f,int t,int c) : from(f), to(t), cost(c){}
    bool operator<(const Edge &e) const {return cost < e.cost;}
};

using Edges = vector<Edge>;

class MinimumSpanningTree {
    public:
        MinimumSpanningTree(){}
        explicit MinimumSpanningTree(int V){
            mUF = UnionFind(V);
        }

        // kruskal
        int kruskal(Edges &es){
            int min_cost = 0;
            sort(es.begin(),es.end());
            for(auto e : es){
                if(!mUF.same(e.from,e.to)){
                    mUF.unite(e.from,e.to);
                    min_cost += e.cost;
                }
            }
            return min_cost;
        }
        //

    private:
        class UnionFind mUF;
};

int main(){
    int V,E,s,t,w;
    cin >> V >> E;
    Edges es;
    for(int i = 0;i < E; ++i){
        cin >> s >> t >> w;
        es.emplace_back(s,t,w);
    }
    MinimumSpanningTree mst(V);
    cout << mst.kruskal(es) << '\n';
    return 0;
}
