#include <cstdio>
#include <vector>
#include <list>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <iterator>
#include <unistd.h>
#include <omp.h>
#define SLEEP 6e4
#define PESO_DIST 1

//Ferramentas
#define BATERIA '0'
#define BRACO1  '1'
#define BOMBA   '2'
#define REFRIG  '3'
#define BRACO2  '4'

//Fábricas
#define GENETIC '5'
#define CASCOS  '6'
#define PETROL  '7'
#define FUNDIC  '8'
#define VIGAS   '9'
using namespace std;
int x_0,y_0, cont, size;
class Item{
    char tipo;
    int x,y;
    Item(int x, int y, char tipo) : tipo(tipo), x(x), y(y){};
};
typedef struct{
    pair<int,int> ant;
    int custo;
}resp;
class Star{
    public:
    int x,y,custo,xant,yant;
    Star(int x, int y, int custo, int xant, int yant) : x(x), y(y), custo(custo), xant(xant), yant(yant){};
};
void printResp(vector<vector<int> > v, vector<vector<resp> > r){
    //for(int i=0;i<11;i++)
        //printf("%d - %d\n",r[0][i].ant.first,r[0][i].ant.second);
    char mat[size][size];
    for(int i=0; i<size; i++)
        for(int j=0; j<size; j++)
            if(v[i][j] == 1)
                mat[i][j] = ' ';
            else if(v[i][j] == 5)
                mat[i][j] = '.';
            else if(v[i][j] == 10)
                mat[i][j] = ';';
            else
                mat[i][j] = 'j';
    mat[x_1][y_1] = 'D';
    list<pair<int,int> > listr;
    pair<int,int> fim(-1,-1),aux(x_1,y_1);
    while((aux = r[aux.first][aux.second].ant) != fim){
        //printf("%d - %d\n",aux.first,aux.second);
        listr.push_front(aux);
    }
    for(list<pair<int,int> >::iterator it=listr.begin();it!=listr.end();it++){
        for(int i=0; i<size+2; i++){
            for(int j=0; j<size+2; j++){
                if(i==0 || i==size+1){
                    printf(" -");
                }
                else if(j==size+1 || j==0){
                    printf(" |");
                }
                else if(i==it->first+1 && j==it->second+1)
                    printf(" R");
                else
                    printf(" %c",mat[i-1][j-1]);
            }
            printf("\n");
        }
        usleep(SLEEP);
    }
    //for(auto it = listr.cbegin(); it!=listr.cend();it++)
        //printf("(%d-%d)\n", it->first,it->second);
}
void printFinal(vector<vector<int> > v, vector<vector<resp> > r){
    char mat[size][size];
    for(int i=0; i<size; i++)
        for(int j=0; j<size; j++)
            if(v[i][j] == 1)
                mat[i][j] = ' ';
            else if(v[i][j] == 5)
                mat[i][j] = '.';
            else if(v[i][j] == 10)
                mat[i][j] = ';';
            else
                mat[i][j] = 'j';
    mat[x_1][y_1] = 'D';
    list<pair<int,int> > listr;
    pair<int,int> fim(-1,-1),aux(x_1,y_1);
    while((aux = r[aux.first][aux.second].ant) != fim){
        //printf("%d - %d\n",aux.first,aux.second);
        listr.push_front(aux);
    }
    for(list<pair<int,int> >::iterator it=listr.begin();it!=listr.end();it++){
        if(it->first == x_0 && it->second ==y_0)
            mat[it->first][it->second] = 'R';
        else if(it->first == x_1 && it->second ==y_1)
            mat[it->first][it->second] = 'D';
        else
            mat[it->first][it->second] = 'x';
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
}
void printExp(vector<pair<int,int> > v){
    char mat[size][size];
    for(int i=0; i<size; i++)
        for(int j=0; j<size; j++)
            mat[i][j] = ' ';
    for (vector<pair<int,int> >::iterator i = v.begin(); i != v.end(); ++i) {
        mat[i->first][i->second] = 'x';
    }
    mat[x_0][y_0] = 'R';
    mat[x_1][y_1] = 'D';
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
bool compareAst (Star p1, Star p2){
    return p1.custo + (abs(p1.x-x_1) + abs(p1.y-y_1))*PESO_DIST <= p2.custo + (abs(p2.x-x_1) + abs(p2.y-y_1))*PESO_DIST;
}
int AStar(vector<vector<int> > custo){
    list<Star> v;
    vector<pair<int,int> > mem;

    vector<vector<resp> > r;
    r.resize(size);
#pragma omp parallel for
    for(int i=0;i<size;i++){
        r[i].resize(size);
    }

    pair<int,int> destino(x_1,y_1);
    cont = 0;
    if(x_0 == x_1 && y_0 == y_1){
        printf("Custo total: 0\nVértices visitados: 1\n");
        return 0;
    }
    Star saux (x_0,y_0,custo[x_0][y_0],-1,-1);
    v.push_back(saux);
    r[x_0][y_0].ant = {-1,-1};
    while(!v.empty()){
        printExp(mem);
        usleep(SLEEP);
        pair<int,int> aux (v.front().x,v.front().y);
        int caux = v.front().custo;
        mem.push_back(aux);
        cont++;
        r[aux.first][aux.second].ant = {v.front().xant,v.front().yant};
        v.pop_front();

        auto it = v.begin();
        while(it!=v.end()){
            //printf("%d %d\n", it->first.first, it->first.second);
            if(it->x == aux.first && it->y == aux.second){
                //printf("!\n");
                it = v.erase(it);
            }
            else
                it++;
        }

        r[aux.first][aux.second].custo = caux;
        printf("%d\n",cont);
        if(aux == destino){
            getchar();
            printResp(custo,r);
            printf("Custo Total: %d\nVértices visitados: %d\n",r[x_1][y_1].custo, cont);
            printFinal(custo,r);
            return 0;
        }
        if(aux.first > 0){
            Star vnorte(aux.first-1,aux.second,custo[aux.first-1][aux.second]+r[aux.first][aux.second].custo,aux.first,aux.second);
            pair<int,int> norte(aux.first-1,aux.second);
            if(find(mem.begin(),mem.end(),norte) == mem.end()){
                //printf("%d %d\n",norte.first,norte.second);
                v.push_back(vnorte);
            }
        }
        if(aux.second < size-1){
            Star vleste(aux.first,aux.second+1,custo[aux.first][aux.second+1]+r[aux.first][aux.second].custo,aux.first,aux.second);
            pair<int,int> leste(aux.first,aux.second+1);
            if(find(mem.begin(),mem.end(),leste) == mem.end()){
                //printf("%d %d\n",leste.first,leste.second);
                v.push_back(vleste);
            }
        }
        if(aux.first < size-1){
            Star vsul(aux.first+1,aux.second,custo[aux.first+1][aux.second]+r[aux.first][aux.second].custo,aux.first,aux.second);
            pair<int,int> sul(aux.first+1,aux.second);
            if(find(mem.begin(),mem.end(),sul) == mem.end()){
                //printf("%d %d\n",sul.first,sul.second);
                v.push_back(vsul);
            }
        }
        if(aux.second > 0){
            Star voeste(aux.first,aux.second-1,custo[aux.first][aux.second-1]+r[aux.first][aux.second].custo,aux.first,aux.second);
            pair<int,int> oeste(aux.first,aux.second-1);
            if(find(mem.begin(),mem.end(),oeste) == mem.end()){
                //printf("%d %d\n",oeste.first,oeste.second);
                v.push_back(voeste);
            }
        }
        v.sort(compareAst);
        //v.sort(compareAst2);
    }
    return 0;
}
int main(int argc, char **argv){
    if(argc != 2){
        printf("Argumentos:\n1 - Arquivo de ambiente\n");
        return 0;
    }
    FILE *f = fopen(argv[1], "r");
    fscanf(f,"%d",&size);
    fscanf(f,"%d %d",&x_0,&y_0);
    vector<vector<int> > mat, custo;
    mat.resize(size);
    custo.resize(size);
//#pragma omp parallel for shared(custo,mat)
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
                custo[i][j] = 15;
        }
    }
    List<Item> itens;
    List<Item> fabricas;
    for(int i=0; i<5;){
        Item aux;
        aux.x = rand()%size;
        aux.y = rand()%size;
    }
    AStar(custo);
    return 0;
}
