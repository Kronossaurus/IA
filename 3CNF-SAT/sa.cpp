#include <cstdio>
#include <string>
#include <bitset>
#include <cstdlib>
#include <cmath>
#include <omp.h>
#define BSIZE 250
#define T0 500000
#define TN 0
#define N 500000
#define STAG 100
using namespace std;
int v,nc;
typedef struct cl{
    bool b[3];
    unsigned p[3];
}cl;
int foo(bitset<BSIZE> r, cl *vet){
    int cont = 0,i;
//#pragma omp parallel for private(i) shared(vet,cont,nc)
    for(i=0; i<nc; i++){
        if(r[vet[i].p[0]] == vet[0].b[0] || r[vet[i].p[1]] == vet[1].b[1] || r[vet[i].p[2]] == vet[2].b[2])
            cont++;
    }
    return cont;
}
void printBit(bitset<BSIZE> r){
    for(int i=0; i<v; i++){
        printf("%d ",(int)r[i]);
    }
    printf("\n");
}
int main(int argc, char **argv){
    if(argc != 2){
        printf("Argumentos: 1-Nome do arquivo\n");
        return 0;
    }
    srand(time(NULL));
    FILE *f = fopen(argv[1],"r");
    char c = 'c';
    while(c == 'c'){
        c = getc(f);
        if(c == 'c'){
            fscanf(f, "%*[^\n]");
        }
        getc(f);
    }

    fscanf(f,"cnf %d %d", &v, &nc);
    //printf("v = %d nc = %d\n", v, nc);
    bitset<BSIZE> r, maior;
    cl vet[nc];

    for(int i=0; i<v; i++){
        if(rand()%2 == 1)
            r.set(i);
    }
    maior = r;

    for(int i=0;i<nc;i++){
        int aux[3];
        fscanf(f,"%d %d %d 0", &aux[0], &aux[1], &aux[2]);
        //printf("%d %d %d\n",aux[0],aux[1],aux[2]);
        for(int j=0;j<3;j++){
            if(aux[j]<0){
                vet[i].b[j] = false;
                vet[i].p[j] = -aux[j];
            }
            else{
                vet[i].b[j] = true;
                vet[i].p[j] = aux[j];
            }
        }
    }
    int smaior = foo(r,vet);
    double temp = T0;
    int stag = 0, i;
    for(i=0; i<N && stag<STAG; i++){
        int score = foo(r,vet);
        if(score>=smaior){
            maior = r;
            smaior = score;
        }
        int j = rand()%v;
        r.flip(j);
        int newscore = foo(r,vet);
        int delta = score - newscore;
        if(newscore>score){
            stag=0;
            continue;
        }
        stag++;
        if(exp(-delta/temp)<(rand()%1001)/1000.0)
            r.flip(j);
        temp=(T0-TN)/(1+exp(.3*(i - N/2))) + TN;
    }

    printf("Iterações:%d Cláusulas satisfeitas:%d\n",i,foo(r,vet));
    printBit(maior);
    fclose(f);
    return 0;
}
