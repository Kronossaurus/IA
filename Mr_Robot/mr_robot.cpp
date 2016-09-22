#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <omp.h>
using namespace std;
int x_0,y_0,x_1,y_1;

int DFS(vector<vector<int> > custo){
    vector<pair<int,int> > v;
    vector<pair<int,int> > mem;
    int size = custo.size();
    if(x_0>0)
        v.push_back({x_0-1,y_0});
    if(y_0<=size)
        v.push_back({x_0,y_0+1});
    if(x_0<=size)
        v.push_back({x_0+1,y_0});
    if(y0>0)
        v.push_back({x_0,y_0-1});
    while(!v.empty()){
        pair<int,int> aux (v.front().first,v.front().second);
        mem.push_back(aux);
        v.erase(v.begin());
        if(aux.second > 0){
        }
    }
    return 0;
}
int main(int argc, char **argv){
    if(argc != 2){
        printf("Argumento:\n1 - Busca em Profundidade\n2 - Busca em Largura\n3 - Busca com Custo Unitário\n");
        return 0;
    }
    int metodo = atoi(argv[1]);
    int SIZE;
    scanf("%d",&SIZE);
    scanf("%d %d",&x_0,&y_0);
    scanf("%d %d",&x_1,&y_1);
    vector<vector<int> > mat, custo;
    mat.reserve(SIZE);
    custo.reserve(SIZE);
#pragma omp parallel for
    for(int i=0;i<SIZE;i++){
        for(int j=0;j<SIZE;j++){
            scanf("%d",&mat[i][j]);
            if(mat[i][j] == 0)
                custo[i][j] = 1;
            else if(mat[i][j] == 1)
                custo[i][j] = 5;
            else if(mat[i][j] == 2)
                custo[i][j] = 10;
            else
                custo[i][j] =15;
        }
    }
    if(metodo == 1)
        DFS(custo);
    return 0;
}
