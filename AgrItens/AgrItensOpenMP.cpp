#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <random>
#include <mutex>
#include <unistd.h>
#define CHFOR '.'
#define SLEEP 1e5
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
unsigned seedp;
int vivas;
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
void simular(Formiga *f, int n, char **mat, int nf, int z){
    //movimento
    int m, refazer;
    for(int p=z;p<nf+z && p<vivas;p++){
        do{
            refazer = 0;
            do{
                m = round(distribuicao(gerador));
            }while(m<1 || m>8 || m == 9-f[p].lastm);
            if(m==1 && f[p].x>0 && f[p].y>0){
                f[p].x--;
                f[p].y--;
                f[p].lastm = 1;
            }
            else if(m==2 && f[p].x>0){
                f[p].x--;
                f[p].lastm = 2;
            }
            else if(m==3 && f[p].x>0 && f[p].y<n-1){
                f[p].x--;
                f[p].y++;
                f[p].lastm = 3;
            }
            else if(m==4 && f[p].y>0){
                f[p].y--;
                f[p].lastm = 4;
            }
            else if(m==5 && f[p].y<n-1){
                f[p].y++;
                f[p].lastm = 5;
            }
            else if(m==6 && f[p].x<n-1 && f[p].y>0){
                f[p].x++;
                f[p].y--;
                f[p].lastm = 6;
            }
            else if(m==7 && f[p].x<n-1){
                f[p].x++;
                f[p].lastm = 7;
            }
            else if(m==8 && f[p].x<n-1 && f[p].y<n-1){
                f[p].x++;
                f[p].y++;
                f[p].lastm = 8;
            }
            else
                refazer = 1;
        }while(refazer==1);
        //decisao
        mtx[f[p].x][f[p].y].lock();
        int cont=0, cell=0, x, y;
        if(f[p].estado == 0 && mat[f[p].x][f[p].y] == CHFOR){
            for(int i=0;i<f[p].raio*2+1;i++){
                for(int j=0;j<f[p].raio*2+1;j++){
                    if(i==f[p].raio && j==f[p].raio)
                        continue;
                    x = f[p].x+i-f[p].raio;
                    y = f[p].y+j-f[p].raio;
                    if(x>=0 && y>=0 && x<n && y<n){
                        cell++;
                        if(mat[x][y] == CHFOR)
                            cont++;
                    }
                }
            }
            int p = 1+ rand()%99;
            //cout<<f[p].x<<" "<<f[p].y<<" "<<cell<<endl;
            if(p > cont*100/cell){
                f[p].estado = 1;
                mat[f[p].x][f[p].y] = ' ';
            }
        }
        else if(f[p].estado == 1 && mat[f[p].x][f[p].y] == ' '){
            for(int i=0;i<f[p].raio*2+1;i++){
                for(int j=0;j<f[p].raio*2+1;j++){
                    if(i==f[p].raio && j==f[p].raio)
                        continue;
                    x = f[p].x+i-f[p].raio;
                    y = f[p].y+j-f[p].raio;
                    if(x>=0 && y>=0 && x<n && y<n){
                        cell++;
                        if(mat[x][y] == CHFOR)
                            cont++;
                    }
                }
            }
            int p = 1+ rand()%99;
            //cout<<f[p].x<<" "<<f[p].y<<" "<<cell<<endl;
            if(p < cont*100/cell){
                f[p].estado = 0;
                mat[f[p].x][f[p].y] = CHFOR;
            }
        }
        mtx[f[p].x][f[p].y].unlock();
    }
}
int main(int argc, char **argv){
    //Restrições iniciais
    if(argc != 7){
        cout<<"Argumentos: Tamanho da matriz, Quantidade de itens, Quantidade de vivas, Raio de visão, iterações, Número de Threads\n";
        return 0;
    }
    int n = atoi(argv[1]), itens = atoi(argv[2]), raio = atoi(argv[4]), it = atoi(argv[5]), nt = atoi(argv[6]);
    vivas = atoi(argv[3]);
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
    Formiga f[vivas];
    char **aux = (char**)malloc(sizeof(char*)*n);
#pragma omp parallel for num_threads(nt) shared(aux,mat)
    for (int i=0;i<n;i++){
        aux[i] = (char*)malloc(sizeof(char)*n);
        for(int j=0;j<n;j++){
            aux[i][j] = mat[i][j];
        }
    }

    of = fopen("output.txt","w");
#pragma omp parallel for shared(f)
    for(int i=0;i<vivas;i++){
        f[i].x = rand_r(&seedp)%n;
        f[i].y = rand_r(&seedp)%n;
        f[i].raio = raio;
        f[i].estado = 0;
        f[i].id = 'o';
        f[i].lastm = 0;
    }
#ifndef semprint 
    printMat(mat, n,f,vivas);
#endif
    for (int i=0; i<vivas; i++)//gambiarra
        mat[f[i].x][f[i].y] = ' ';
    fprintf(of, "Matriz inicial:\n");
    fprintMat(mat,n);
    int cont = ceil(vivas/nt*1.0);
    for(int h=0; h < it; h++){
        //cout<<h+1<<endl;
#pragma omp parallel for num_threads(nt) shared(f, n, mat, cont)
        for(int i=0;i<nt;i++){
            simular(&f[0],n,mat,cont, cont*i);
        }

#ifndef semprint 
        for (int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                aux[i][j] = mat[i][j];
            }
        }
        printMat(aux,n,f,vivas);
        usleep(SLEEP);
#endif
    }
    //int h;
    //do{
        //h=0;
        //for(int i=0;i<vivas;i++){
            //if(f[i].estado == 1){
                //h=1;
                //th[i] = thread(simular,&f[i],n,mat,1);
            //}
        //}

        //for(int i=0;i<nt;i++){
            //if(th[i].joinable())
                //th[i].join();
        //}

//#ifndef semprint 
        //for (int i=0;i<n;i++){
            //for(int j=0;j<n;j++){
                //aux[i][j] = mat[i][j];
            //}
        //}
        //printMat(aux,n,f,vivas);
        //usleep(SLEEP);
//#endif
    //}while(h==1);
    fprintf(of,"\nResultado final:\n");
    fprintMat(mat,n);
    for(int i=0;i<n;i++)
        free(mat[i]);
    free(mat);
    return 0;
}
