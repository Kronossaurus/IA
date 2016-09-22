#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <omp.h>
using namespace std;
int x_0,y_0,x_1,y_1, cont;

int DFS(vector<vector<int> > custo){
    vector<pair<pair<int,int>,int> > v;
    vector<pair<int,int> > mem;
    pair<int,int> destino(x_1,y_1);
    int size = custo.size();
    cont = 1;
    if(x_0 == x_1 && y_0 == y_1){
        printf("Custo total: 0\nVértices visitados: 1\n");
        return 0;
    }
    if(x_0>0)
        v.push_back({{x_0-1,y_0}, custo[x_0][y_0]});
    if(y_0<=size)
        v.push_back({{x_0,y_0+1}, custo[x_0][y_0]});
    if(x_0<=size)
        v.push_back({{x_0+1,y_0}, custo[x_0][y_0]});
    if(y0>0)
        v.push_back({{x_0,y_0-1}, custo[x_0][y_0]});
    while(!v.empty()){
        pair<int,int> aux (v.front().first.first,v.front().first.second);
        mem.push_back(aux);
        v.erase(v.begin());
        cont++;
        if(aux == destino){
            printf("Custo Total: %d\nVértices visitados: %d\n",aux.second, cont);
            return 0;
        }
        if(aux.second > 0){
            pair<int,int> oeste(aux.first,aux.second-1);
            if(find(mem.begin(),mem.end(),oeste) == mem.end())
                v.insert(v.begin(),{oeste,aux.second+custo[oeste.first][oeste.second]});
        }
        if(aux.first <= size){
            pair<int,int> sul(aux.first+1,aux.second);
            if(find(mem.begin(),mem.end(),sul) == mem.end())
                v.insert(v.begin(),{sul,aux.second+custo[sul.first][sul.second]});
        }
        if(aux.second <= size){
            pair<int,int> leste(aux.first,aux.second+1);
            if(find(mem.begin(),mem.end(),leste) == mem.end())
                v.insert(v.begin(),{leste,aux.second+custo[leste.first][leste.second]});
        }
        if(aux.first > 0){
            pair<int,int> norte(aux.first-1,aux.second);
            if(find(mem.begin(),mem.end(),norte) == mem.end())
                v.insert(v.begin(),{norte,aux.second+custo[norte.first][norte.second]});
        }
    }
    return 0;
}
int main(int argc, char **argv){
    if(argc != 3){
        printf("Argumentos:\n1 - Arquivo de ambiente\n2 - Método de busca:\n\t'p' - Busca em Profundidade\n\t'l' - Busca em Largura\n\t'c' - Busca com Custo Unitário\n");
        return 0;
    }
    FILE *f = fopen(argv[1], "r");
    char metodo = argv[2][0];
    int SIZE;
    fscanf(f,"%d",&SIZE);
    fscanf(f,"%d %d",&x_0,&y_0);
    fscanf(f,"%d %d",&x_1,&y_1);
    vector<vector<int> > mat, custo;
    mat.resize(SIZE);
    custo.resize(SIZE);
#pragma omp parallel for
    for(int i=0;i<SIZE;i++){
        mat[i].resize(SIZE);
        custo[i].resize(SIZE);
        for(int j=0;j<SIZE;j++){
            fscanf(f,"%d",&mat[i][j]);
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
    if(metodo == 'p')
        DFS(custo);
    return 0;
}
