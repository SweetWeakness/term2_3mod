#include <limits.h>
#include "iostream"
#include "vector"
#include "algorithm"
#include "set"
using namespace std;


struct DSU{
    vector<int> parent;
    vector<int> rank;

    explicit DSU(int n) : parent(n), rank(n) {}

    void make_set (int v) {
        parent[v] = v;
        rank[v] = 0;
    }

    int find_set (int v) {
        if (v == parent[v])
            return v;
        return parent[v] = find_set (parent[v]);
    }

    void union_sets (int a, int b) {
        a = find_set (a);
        b = find_set (b);
        if (a != b) {
            if (rank[a] < rank[b])
                swap (a, b);
            parent[b] = a;
            if (rank[a] == rank[b])
                ++rank[a];
        }
    }
};



struct Edge{
    int from;
    int to;
    int weight;
    int num;

    bool operator<(const Edge& one) const{
        if(weight == one.weight) return num < one.num;
        return weight < one.weight;
    }

    explicit Edge(int f, int t, int w, int n) : from(f), to(t), weight(w), num(n) {}
};


int alg_Boruvki(set<Edge>& edges, int n, int m) {
    int WEIGHT = 0;
    int comps = n;


    DSU components(n);
    for (int i = 0; i < n; i++) components.make_set(i);

    vector<int> AllComps;


    for (int i = 0; i < n; i++) {
        AllComps.push_back(i);
    }

    vector<pair<int,int>> minedges(n,pair<int,int>(INT_MAX,0));
    //начало алгоритма
    while (comps > 1) {
        //cout << comps <<" "<<edges.size() <<'\n';



        for (auto i= edges.begin();i!=edges.end();i++) { //поиск мин ребер для компонент
            Edge curEdge = *i;
            //cout << curEdge.weight << " ";

            int fromComp = components.find_set(curEdge.from);
            int toComp = components.find_set(curEdge.to);
            if (fromComp != toComp ) {
                if(curEdge.weight < minedges[fromComp].first){
                    minedges[fromComp].first = curEdge.weight;
                    minedges[fromComp].second = curEdge.to;
                }
                if(curEdge.weight < minedges[toComp].first){
                    minedges[toComp].first = curEdge.weight;
                    minedges[toComp].second = curEdge.from;
                }
            }
        }

        //cout << edges.size();

        //cout <<"k";

        for(auto j=0;j<AllComps.size();j++){// объединение компонент
            int curComp = AllComps[j];
            int new_comp;

            if(minedges[curComp].first!=INT_MAX && curComp!=-1) {
                int to = minedges[curComp].second;
                int otherComp = components.find_set(to);
                WEIGHT += minedges[curComp].first;

                if (components.find_set(minedges[otherComp].second) == curComp) {
                    components.union_sets(curComp, to);

                    new_comp = components.find_set(to);
                    minedges[new_comp].first = INT_MAX;

                } else {
                    pair<int, int> tmp = minedges[otherComp];
                    components.union_sets(curComp, to);
                    new_comp = components.find_set(to);
                    minedges[new_comp] = tmp;
                }

                if(new_comp == curComp){
                    AllComps[otherComp] = -1;
                }else{
                    AllComps[curComp] =-1;
                }
                comps--;
            }
        }
    }

    //cout << "ky";
    return WEIGHT;
}


void floyd(vector<vector<int>>& d, int n){
    for( int i = 0; i < n; i++ ) {
        for (int u = 0; u < n; u++) {
            for (int v = 0; v < n; v++) {
                if ( d[u][i] + d[i][v] < d[u][v] && d[u][i] != INT_MAX && d[i][v] != INT_MAX) {
                    d[u][v] = d[u][i] + d[i][v];
                }
            }
        }
    }
}


void swap(int *a, int i, int j)
{
    int s = a[i];
    a[i] = a[j];
    a[j] = s;
}

bool NextSet(int *a, int n)
{
    int j = n - 2;
    while (j != -1 && a[j] >= a[j + 1]) j--;
    if (j == -1)
        return false; // больше перестановок нет
    int k = n - 1;
    while (a[j] >= a[k]) k--;
    swap(a, j, k);
    int l = j + 1, r = n - 1; // сортируем оставшуюся часть последовательности
    while (l<r)
        swap(a, l++, r--);
    return true;
}

int real_distance(set<Edge>& in, int n){
    vector<vector<int>> matrix(n, vector<int>(n, INT_MAX));
    for(auto i = in.begin(); i != in.end(); i++){
        matrix[i->from][i->to] = i->weight;
        matrix[i->to][i->from] = i->weight;
    }

    for(int i = 0; i < n; i++) matrix[i][i] = 0;

    floyd(matrix, n);// нашли матрицу смежности


    int* var = new int[n]; // Наши перестановки вершин
    for(int i = 0; i < n; i++) var[i] = i + 1;

    int min = INT_MAX;

    do{
        int dist = 0;
        for(int i = 0; i < n - 1; i++){
            dist += matrix[var[i] - 1][var[i + 1] - 1];
        }

        dist += matrix[var[0] - 1][var[n - 1] - 1];
        if(min > dist) min = dist;
    }while(NextSet(var, n));

    return min;
}

int main(){
    int n, m, a, b, c;
    cin >> n >> m;
    set<Edge> in;
    for(int i = 0; i < m; i++){
        cin >> a >> b >> c;
        in.insert(Edge(a, b, c, i));
    }

    cout << "Real min path : " << real_distance(in, n) << "\n";
    cout << "MST : " << alg_Boruvki(in, n, m) * 2;

    return 0;
}