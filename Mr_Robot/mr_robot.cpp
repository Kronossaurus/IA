#include <cstdio>
#include <vector>
#include <omp.h>
using namespace std;
int i0,j0,i1,j1;

int DFS(int **custo){
    vector<pair<int,int> > v;
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
    scanf("%d %d",&i0,&j0);
    scanf("%d %d",&i1,&j1);
    int mat[SIZE][SIZE], custo[SIZE][SIZE];
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
        DFS(&custo);
    return 0;
}
