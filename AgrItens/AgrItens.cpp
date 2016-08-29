#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <random>
#include <thread>
#include <mutex>
#include <unistd.h>
#define CHFOR '.'
#define SLEEP 1e5
#define MAXTH 200
using namespace std;
mutex mtx[100][100];
//RAND_MAX = 2147483647
class Formiga{
    public:
        int x, y, estado, raio, lastm;
	char id;
};
FILE *of;
default_random_engine gerador;
normal_distribution<double> distribuicao(4.5,5);

void initMat(char **mat, int n){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            mat[i][j] = ' ';
        }
    }
}
void preencherMat(char **mat, int itens, int n){
    int i,j;
    for(int k = 0; k < itens;){
        i = rand()%n;
        j = rand()%n;
        if(mat[i][j] != CHFOR){
            mat[i][j] = CHFOR;
            k++;
        }
    }
}
void fprintMat(char **mat, int n){
    for(int i=0; i<n+2; i++){
        fprintf(of, "\n");
        for(int j=0; j<n+2; j++){
            if(i==0){
                fprintf(of," -");
            }
            else if(i==n+1){
                fprintf(of," -");
            }
            else{
                if(j==0 || j==n+1)
                    fprintf(of," |");
                else
                    fprintf(of, " %c",mat[i-1][j-1]);
            }
        }
    }
}
void printMat(char **mat, int n, Formiga *f, int vivas){
    //system("clear");
    for(int i=0;i<vivas;i++){
        if(f[i].estado == 1){
            mat[f[i].x][f[i].y] = '@';
        }
        else{
            //cout<<f[i].id<<" "<<f[i].x<<" "<<f[i].y<<endl;
            mat[f[i].x][f[i].y] = f[i].id;
        }
    }
    for(int i=0; i<n+2; i++){
        for(int j=0; j<n+2; j++){
            if(i==0){
                printf(" -");
            }
            else if(i==n+1){
                printf(" -");
                }
            else{
            if(j==0 || j==n+1)
                printf(" |");
            else
                cout<<" "<<mat[i-1][j-1];
            }
        }
        cout<<endl;
    }
}
void simular(Formiga *f, int n, char **mat){
    //movimento
    int m, refazer;
    do{
        refazer = 0;
        do{
            m = round(distribuicao(gerador));
        }while(m<1 || m>8 || m == 9-f->lastm);
        if(m==1 && f->x>0 && f->y>0){
            f->x--;
            f->y--;
            f->lastm = 1;
        }
        else if(m==2 && f->x>0){
            f->x--;
            f->lastm = 2;
        }
        else if(m==3 && f->x>0 && f->y<n-1){
            f->x--;
            f->y++;
            f->lastm = 3;
        }
        else if(m==4 && f->y>0){
            f->y--;
            f->lastm = 4;
        }
        else if(m==5 && f->y<n-1){
            f->y++;
            f->lastm = 5;
        }
        else if(m==6 && f->x<n-1 && f->y>0){
            f->x++;
            f->y--;
            f->lastm = 6;
        }
        else if(m==7 && f->x<n-1){
            f->x++;
            f->lastm = 7;
        }
        else if(m==8 && f->x<n-1 && f->y<n-1){
            f->x++;
            f->y++;
            f->lastm = 8;
        }
        else
            refazer = 1;
    }while(refazer==1);
    //decisao
    mtx[f->x][f->y].lock();
    int cont=0, cell=0, x, y;
    if(f->estado == 0 && mat[f->x][f->y] == CHFOR){
        for(int i=0;i<f->raio*2+1;i++){
            for(int j=0;j<f->raio*2+1;j++){
                if(i==f->raio && j==f->raio)
                    continue;
                x = f->x+i-f->raio;
                y = f->y+j-f->raio;
                if(x>=0 && y>=0 && x<n && y<n){
                    cell++;
                    if(mat[x][y] == CHFOR)
                        cont++;
                }
            }
        }
        int p = 1+ rand()%99;
        //cout<<f->x<<" "<<f->y<<" "<<cell<<endl;
        if(p > cont*100/cell){
            f->estado = 1;
            mat[f->x][f->y] = ' ';
        }
    }
    else if(f->estado == 1 && mat[f->x][f->y] == ' '){
        for(int i=0;i<f->raio*2+1;i++){
            for(int j=0;j<f->raio*2+1;j++){
                if(i==f->raio && j==f->raio)
                    continue;
                x = f->x+i-f->raio;
                y = f->y+j-f->raio;
                if(x>=0 && y>=0 && x<n && y<n){
                    cell++;
                    if(mat[x][y] == CHFOR)
                        cont++;
                }
            }
        }
        int p = 1+ rand()%99;
        //cout<<f->x<<" "<<f->y<<" "<<cell<<endl;
        if(p < cont*100/cell){
            f->estado = 0;
            mat[f->x][f->y] = CHFOR;
        }
    }
    mtx[f->x][f->y].unlock();
}
int main(int argc, char **argv){
    //Restrições iniciais
    if(argc != 6){
        cout<<"Argumentos: Tamanho da matriz, Quantidade de itens, Quantidade de vivas, Raio de visão, iterações\n";
        return 0;
    }
    int n = atoi(argv[1]), itens = atoi(argv[2]), vivas = atoi(argv[3]), raio = atoi(argv[4]), it = atoi(argv[5]);
    if(n <= 0){
        cout<<"Tamanho da matriz invalido\n";
        return 0;
    }
    if(itens <= 0 || itens > n*n){
        cout<<"Quantidade invalida de itens\n";
        return 0;
    }
    if(vivas <= 0){
        cout<<"Quantidade invalida de formigas vivas\n";
        return 0;
    }
    if(raio <= 0){
        cout<<"Raio de visao invalido\n";
        return 0;
    }
    ///
    char **mat = (char**)malloc(sizeof(char*)*n);
    for(int i=0;i<n;i++)
        mat[i] = (char*)malloc(sizeof(char)*n);

    initMat(mat,n);
    srand(time(NULL));
    preencherMat(mat, itens, n);
    Formiga f[MAXTH];
    char **aux = (char**)malloc(sizeof(char*)*n);
    for (int i=0;i<n;i++){
        aux[i] = (char*)malloc(sizeof(char)*n);
        for(int j=0;j<n;j++){
            aux[i][j] = mat[i][j];
        }
    }

    of = fopen("output.txt","w");
    for(int i=0;i<vivas;i++){
        f[i].x = rand()%n;
        f[i].y = rand()%n;
        f[i].raio = raio;
        f[i].estado = 0;
        f[i].id = 'o';
        f[i].lastm = 0;
    }
    printMat(mat, n,f,vivas);
    fprintf(of, "Matriz inicial:\n");
    fprintMat(mat,n);
    for (int i=0; i<vivas; i++)//gambiarra
        mat[f[i].x][f[i].y] = ' ';
    thread th[MAXTH];
    for(int h=0; h < it; h++){
        //cout<<h+1<<endl;
        for(int i=0;i<vivas;i++){
            th[i] = thread(simular,&f[i],n,mat);
        }
        for(int i=0;i<vivas;i++){
            th[i].join();
        }

        for (int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                aux[i][j] = mat[i][j];
            }
        }
        printMat(aux,n,f,vivas);
        usleep(SLEEP);
    }
    int h;
    do{
        h=0;
        for(int i=0;i<vivas;i++){
            if(f[i].estado == 1){
                h=1;
                th[i] = thread(simular,&f[i],n,mat);
            }
        }

        for(int i=0;i<vivas;i++){
            if(th[i].joinable())
                th[i].join();
        }

        for (int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                aux[i][j] = mat[i][j];
            }
        }
        printMat(aux,n,f,vivas);
        usleep(SLEEP);
    }while(h==1);
    fprintf(of,"\nResultado final:\n");
    fprintMat(mat,n);
    for(int i=0;i<n;i++)
        free(mat[i]);
    free(mat);
    return 0;
}
