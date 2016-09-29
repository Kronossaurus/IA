#include <cstdio>
#include <vector>
#include <list>
#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <unistd.h>
#include <omp.h>
#define SLEEP 6e4
using namespace std;
int x_0,y_0,x_1,y_1, cont, size;

typedef struct r{
    pair<int,int> ant;
    int custo;
}resp;
class Star{
    public:
    int x,y,custo,xant,yant;
    Star(int x, int y, int custo, int xant, int yant) : x(x), y(y), custo(custo), xant(xant), yant(yant){};
};
void printResp(vector<vector<int> > v, vector<vector<resp> > r){
    for(int i=0;i<11;i++)
        printf("%d - %d\n",r[0][i].ant.first,r[0][i].ant.second);
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
    cont = 1;
    if(x_0 == x_1 && y_0 == y_1){
        printf("Custo total: 0\nVértices visitados: 1\n");
        return 0;
    }
    r[x_0][y_0].custo = custo[x_0][y_0];
    r[x_0][y_0].ant = {-1,-1};
    mem.push_back({x_0,y_0});
    if(x_0>0){
        v.push_back({x_0-1,y_0});
        r[x_0-1][y_0].custo = r[x_0][y_0].custo + custo[x_0-1][y_0];
        r[x_0-1][y_0].ant = {x_0,y_0};
    }
    if(y_0<size-1){
        v.push_back({x_0,y_0+1});
        r[x_0][y_0+1].custo = r[x_0][y_0].custo + custo[x_0][y_0+1];
        r[x_0][y_0+1].ant = {x_0,y_0};
    }
    if(x_0<size-1){
        v.push_back({x_0+1,y_0});
        r[x_0+1][y_0].custo = r[x_0][y_0].custo + custo[x_0+1][y_0];
        r[x_0+1][y_0].ant = {x_0,y_0};
    }
    if(y_0>0){
        v.push_back({x_0,y_0-1});
        r[x_0][y_0-1].custo = r[x_0][y_0].custo + custo[x_0][y_0-1];
        r[x_0][y_0-1].ant = {x_0,y_0};
    }
    while(!v.empty()){
        printExp(mem);
        usleep(SLEEP);
        pair<int,int> aux (v.front().first,v.front().second);
        mem.push_back(aux);
        v.pop_front();
        cont++;
        //printf("%d\n",cont);
        if(aux == destino){
            //r.clear();
            //r.shrink_to_fit();
            //mem.clear();
            //mem.shrink_to_fit();
            //v.clear();
            printResp(custo,r);
            printf("Custo Total: %d\nVértices visitados: %d\n",r[x_1][y_1].custo, cont);
            return 0;
        }
        if(aux.second > 0){
            pair<int,int> oeste(aux.first,aux.second-1);
            if(find(mem.begin(),mem.end(),oeste) == mem.end() && find(v.begin(),v.end(),oeste)==v.end()){
                v.push_front(oeste);
                r[oeste.first][oeste.second].custo = r[aux.first][aux.second].custo + custo[oeste.first][oeste.second];
                r[oeste.first][oeste.second].ant = {aux.first,aux.second};
            }
        }
        if(aux.first < size-1){
            pair<int,int> sul(aux.first+1,aux.second);
            if(find(mem.begin(),mem.end(),sul) == mem.end() && find(v.begin(),v.end(),sul)==v.end()){
                v.push_front(sul);
                r[sul.first][sul.second].custo = r[aux.first][aux.second].custo + custo[sul.first][sul.second];
                r[sul.first][sul.second].ant = {aux.first,aux.second};
            }
        }
        if(aux.second < size-1){
            pair<int,int> leste(aux.first,aux.second+1);
            if(find(mem.begin(),mem.end(),leste) == mem.end() && find(v.begin(),v.end(),leste)==v.end()){
                v.push_front(leste);
                r[leste.first][leste.second].custo = r[aux.first][aux.second].custo + custo[leste.first][leste.second];
                r[leste.first][leste.second].ant = {aux.first,aux.second};
            }
        }
        if(aux.first > 0){
            pair<int,int> norte(aux.first-1,aux.second);
            if(find(mem.begin(),mem.end(),norte) == mem.end() && find(v.begin(),v.end(),norte)==v.end()){
                v.push_front(norte);
                r[norte.first][norte.second].custo = r[aux.first][aux.second].custo + custo[norte.first][norte.second];
                r[norte.first][norte.second].ant = {aux.first,aux.second};
            }
        }
    }
    return 0;
}
int BFS(vector<vector<int> > custo){
    list<pair<int,int> > v;
    vector<pair<int,int> > mem;

    vector<vector<resp> > r;
    r.resize(size);
#pragma omp parallel for
    for(int i=0;i<size;i++){
        r[i].resize(size);
    }

    pair<int,int> destino(x_1,y_1);
    cont = 1;
    if(x_0 == x_1 && y_0 == y_1){
        printf("Custo total: 0\nVértices visitados: 1\n");
        return 0;
    }
    r[x_0][y_0].custo = custo[x_0][y_0];
    r[x_0][y_0].ant = {-1,-1};
    mem.push_back({x_0,y_0});
    if(x_0>0){
        v.push_back({x_0-1,y_0});
        r[x_0-1][y_0].custo = r[x_0][y_0].custo + custo[x_0-1][y_0];
        r[x_0-1][y_0].ant = {x_0,y_0};
    }
    if(y_0<size-1){
        v.push_back({x_0,y_0+1});
        r[x_0][y_0+1].custo = r[x_0][y_0].custo + custo[x_0][y_0+1];
        r[x_0][y_0+1].ant = {x_0,y_0};
    }
    if(x_0<size-1){
        v.push_back({x_0+1,y_0});
        r[x_0+1][y_0].custo = r[x_0][y_0].custo + custo[x_0+1][y_0];
        r[x_0+1][y_0].ant = {x_0,y_0};
    }
    if(y_0>0){
        v.push_back({x_0,y_0-1});
        r[x_0][y_0-1].custo = r[x_0][y_0].custo + custo[x_0][y_0-1];
        r[x_0][y_0-1].ant = {x_0,y_0};
    }
    while(!v.empty()){
        printExp(mem);
        usleep(SLEEP);
        pair<int,int> aux (v.front().first,v.front().second);
        mem.push_back(aux);
        v.pop_front();
        cont++;
        //printf("%d\n",cont);
        if(aux == destino){
            //r.clear();
            //r.shrink_to_fit();
            //mem.clear();
            //mem.shrink_to_fit();
            //v.clear();
            printResp(custo,r);
            printf("Custo Total: %d\nVértices visitados: %d\n",r[x_1][y_1].custo, cont);
            return 0;
        }
        if(aux.first > 0){
            pair<int,int> norte(aux.first-1,aux.second);
            if(find(mem.begin(),mem.end(),norte) == mem.end() && find(v.begin(),v.end(),norte)==v.end()){
                v.push_back(norte);
                r[norte.first][norte.second].custo = r[aux.first][aux.second].custo + custo[norte.first][norte.second];
                r[norte.first][norte.second].ant = {aux.first,aux.second};
            }
        }
        if(aux.second < size-1){
            pair<int,int> leste(aux.first,aux.second+1);
            if(find(mem.begin(),mem.end(),leste) == mem.end() && find(v.begin(),v.end(),leste)==v.end()){
                v.push_back(leste);
                r[leste.first][leste.second].custo = r[aux.first][aux.second].custo + custo[leste.first][leste.second];
                r[leste.first][leste.second].ant = {aux.first,aux.second};
            }
        }
        if(aux.first < size-1){
            pair<int,int> sul(aux.first+1,aux.second);
            if(find(mem.begin(),mem.end(),sul) == mem.end() && find(v.begin(),v.end(),sul)==v.end()){
                v.push_back(sul);
                r[sul.first][sul.second].custo = r[aux.first][aux.second].custo + custo[sul.first][sul.second];
                r[sul.first][sul.second].ant = {aux.first,aux.second};
            }
        }
        if(aux.second > 0){
            pair<int,int> oeste(aux.first,aux.second-1);
            if(find(mem.begin(),mem.end(),oeste) == mem.end() && find(v.begin(),v.end(),oeste)==v.end()){
                v.push_back(oeste);
                r[oeste.first][oeste.second].custo = r[aux.first][aux.second].custo + custo[oeste.first][oeste.second];
                r[oeste.first][oeste.second].ant = {aux.first,aux.second};
            }
        }
    }
    return 0;
}
bool compare (Star p1, Star p2){
    return p1.custo <= p2.custo;
}
int BCU(vector<vector<int> > custo){
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
            printResp(custo,r);
            printf("Custo Total: %d\nVértices visitados: %d\n",r[x_1][y_1].custo, cont);
            return 0;
        }
        if(aux.first > 0){
            pair<int,int> norte(aux.first-1,aux.second);
            if(find(mem.begin(),mem.end(),norte) == mem.end()){
                //printf("%d %d\n",norte.first,norte.second);
                Star vnorte(norte.first,norte.second,r[aux.first][aux.second].custo + custo[norte.first][norte.second],aux.first,aux.second);
                v.push_back(vnorte);
            }
        }
        if(aux.second < size-1){
            pair<int,int> leste(aux.first,aux.second+1);
            if(find(mem.begin(),mem.end(),leste) == mem.end()){
                //printf("%d %d\n",leste.first,leste.second);
                Star vleste(leste.first,leste.second,r[aux.first][aux.second].custo + custo[leste.first][leste.second],aux.first,aux.second);
                v.push_back(vleste);
            }
        }
        if(aux.first < size-1){
            pair<int,int> sul(aux.first+1,aux.second);
            if(find(mem.begin(),mem.end(),sul) == mem.end()){
                //printf("%d %d\n",sul.first,sul.second);
                Star vsul(sul.first,sul.second,r[aux.first][aux.second].custo + custo[sul.first][sul.second],aux.first,aux.second);
                v.push_back(vsul);
            }
        }
        if(aux.second > 0){
            pair<int,int> oeste(aux.first,aux.second-1);
            if(find(mem.begin(),mem.end(),oeste) == mem.end()){
                //printf("%d %d\n",oeste.first,oeste.second);
                Star voeste(oeste.first,oeste.second,r[aux.first][aux.second].custo + custo[oeste.first][oeste.second],aux.first,aux.second);
                v.push_back(voeste);
            }
        }
        v.sort(compare);
    }
    return 0;
}
bool compareAst (Star p1, Star p2){
    return p1.custo + abs(p1.x-x_1) + abs(p1.y-y_1) <= p2.custo + abs(p2.x-x_1) + abs(p2.y-y_1);
}
bool compareAst2 (Star p1, Star p2){
    int dist1 = sqrt(abs(p1.x-x_1) + abs(p1.y-y_1));
    int dist2 = sqrt(abs(p2.x-x_1) + abs(p2.y-y_1));
    //printf("%d %d\n",dist1,dist2);
    return p1.custo + dist1 <= p2.custo + dist2;
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
            printResp(custo,r);
            printf("Custo Total: %d\nVértices visitados: %d\n",r[x_1][y_1].custo, cont);
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
    if(metodo == 'p')
        DFS(custo);
    else if(metodo == 'l')
        BFS(custo);
    else if(metodo == 'u')
        BCU(custo);
    else if(metodo == 'a')
        AStar(custo);
    return 0;
}
