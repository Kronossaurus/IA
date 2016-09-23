#include <cstdio>
#include <vector>
#include <list>
#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <unistd.h>
#include <omp.h>
using namespace std;
int x_0,y_0,x_1,y_1, cont, size;

typedef struct r{
    pair<int,int> ant;
    int custo;
}resp;
void printExp(vector<pair<int,int> > v){
    char mat[size][size];
    for(int i=0; i<size; i++)
        for(int j=0; j<size; j++)
            mat[i][j] = ' ';
    mat[x_0][y_0] = 'R';
    mat[x_1][y_1] = 'D';
    for (vector<pair<int,int> >::iterator i = v.begin(); i != v.end(); ++i) {
        mat[i->first][i->second] = 'x';
    }
    for(int i=0; i<size+2; i++){
        for(int j=0; j<size+2; j++){
            if(i==0 || i==size+1){
                printf(" -");
            }
            else if(j==size+1 || j==0){
                printf(" |");
            }
            else
                printf(" %c",mat[i-1][j-1]);
        }
        printf("\n");
    }
    return;
}
int DFS(vector<vector<int> > custo){
    list<pair<int,int> > v;
    vector<pair<int,int> > mem;

    vector<vector<resp> > r;
    r.resize(size);
#pragma omp parallel for
    for(int i=0;i<size;i++){
        r[i].resize(size);
    }

    pair<int,int> destino(x_1,y_1);
    int size = custo.size();
    cont = 1;
    if(x_0 == x_1 && y_0 == y_1){
        printf("Custo total: 0\nVértices visitados: 1\n");
        return 0;
    }
    r[x_0][y_0].custo = custo[x_0][y_0];
    r[x_0][y_0].ant = {-1,-1};
    if(x_0>0){
        v.push_back({x_0-1,y_0});
        r[x_0-1][y_0].custo = r[x_0][y_0].custo + custo[x_0-1][y_0];
        r[x_0-1][y_0].ant = {x_0,y_0};
    }
    if(y_0<=size){
        v.push_back({x_0,y_0+1});
        r[x_0][y_0+1].custo = r[x_0][y_0].custo + custo[x_0][y_0+1];
        r[x_0][y_0+1].ant = {x_0,y_0};
    }
    if(x_0<=size){
        v.push_back({x_0+1,y_0});
        r[x_0+1][y_0].custo = r[x_0][y_0].custo + custo[x_0+1][y_0];
        r[x_0+1][y_0].ant = {x_0,y_0};
    }
    if(y0>0){
        v.push_back({x_0,y_0-1});
        r[x_0][y_0-1].custo = r[x_0][y_0].custo + custo[x_0][y_0-1];
        r[x_0][y_0-1].ant = {x_0,y_0};
    }
    while(!v.empty()){
        printExp(mem);
        usleep(1e5);
        pair<int,int> aux (v.front().first,v.front().second);
        mem.push_back(aux);
        v.pop_front();
        cont++;
        //printf("%d\n",cont);
        if(aux == destino){
            printf("Custo Total: %d\nVértices visitados: %d\n",r[x_1][y_1].custo, cont);
            //r.clear();
            //r.shrink_to_fit();
            //mem.clear();
            //mem.shrink_to_fit();
            //v.clear();
            return 0;
        }
        if(aux.second > 0){
            pair<int,int> oeste(aux.first,aux.second-1);
            if(find(mem.begin(),mem.end(),oeste) == mem.end() && find(v.begin(),v.end(),oeste)==v.end())
                v.push_front(oeste);
            r[oeste.first][oeste.second].custo = r[aux.first][aux.second].custo + custo[oeste.first][oeste.second];
            r[oeste.first][oeste.second].ant = {aux.first,aux.second};
        }
        if(aux.first < size-1){
            pair<int,int> sul(aux.first+1,aux.second);
            if(find(mem.begin(),mem.end(),sul) == mem.end() && find(v.begin(),v.end(),sul)==v.end())
                v.push_front(sul);
            r[sul.first][sul.second].custo = r[aux.first][aux.second].custo + custo[sul.first][sul.second];
            r[sul.first][sul.second].ant = {aux.first,aux.second};
        }
        if(aux.second < size-1){
            pair<int,int> leste(aux.first,aux.second+1);
            if(find(mem.begin(),mem.end(),leste) == mem.end() && find(v.begin(),v.end(),leste)==v.end())
                v.push_front(leste);
            r[leste.first][leste.second].custo = r[aux.first][aux.second].custo + custo[leste.first][leste.second];
            r[leste.first][leste.second].ant = {aux.first,aux.second};
        }
        if(aux.first > 0){
            pair<int,int> norte(aux.first-1,aux.second);
            if(find(mem.begin(),mem.end(),norte) == mem.end() && find(v.begin(),v.end(),norte)==v.end())
                v.push_front(norte);
            r[norte.first][norte.second].custo = r[aux.first][aux.second].custo + custo[norte.first][norte.second];
            r[norte.first][norte.second].ant = {aux.first,aux.second};
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
    fscanf(f,"%d",&size);
    fscanf(f,"%d %d",&x_0,&y_0);
    fscanf(f,"%d %d",&x_1,&y_1);
    vector<vector<int> > mat, custo;
    mat.resize(size);
    custo.resize(size);
#pragma omp parallel for
    for(int i=0;i<size;i++){
        mat[i].resize(size);
        custo[i].resize(size);
        for(int j=0;j<size;j++){
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
