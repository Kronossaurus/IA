#include <cstdio>
#include <string>
#include <bitset>
#include <cstdlib>
#include <cmath>
#include <omp.h>
#define BSIZE 250
#define T0 100
#define TN 1
#define N 100000
#define IT 10
using namespace std;
int v,nc;
typedef struct cl{
    bool b[3];
    unsigned p[3];
}cl;
int foo(bitset<BSIZE> r, cl *vet){
    int i;
    int cont=0;
#pragma omp parallel for private(i) shared(vet,nc) reduction(+:cont)
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
    FILE *f = fopen(argv[1],"r"), *out = fopen("data.txt","w");
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

    for(int i=0;i<nc;i++){
        int aux[3];
        fscanf(f,"%d %d %d 0", &aux[0], &aux[1], &aux[2]);
        //printf("%d %d %d\n",aux[0],aux[1],aux[2]);
        for(int j=0;j<3;j++){
            if(aux[j]<0){
                vet[i].b[j] = false;
                vet[i].p[j] = -aux[j]-1;
            }
            else{
                vet[i].b[j] = true;
                vet[i].p[j] = aux[j]-1;
            }
        }
    }
    int resps[IT],vits[IT],score[N];
    double temp[N];
    double media=.0,dp=.0,its=.0,dpits=.0;
    for(int k=0; k<IT; k++){
        for(int i=0; i<v; i++){
            if(rand()%2 == 1)
                r.set(i);
            else
                r.reset(i);
        }
        maior = r;
        int smaior = foo(r,vet);
        temp[0]=T0;
        int i=0;
        for(i=0; i<N && score[i-1] != nc; i++){
            score[i] = foo(r,vet);
            if(score[i]>=smaior){
                maior = r;
                smaior = score[i];
            }
            int j = rand()%v;
            r.flip(j);
            int newscore = foo(r,vet);
            int delta = score[i] - newscore;
            //temp[i+1] = temp[i]*.9;
            //temp[i+1] = T0 - (double)i*(T0-TN)/N;//0
            //temp[i+1]=(T0-TN)/(1.0+exp(0.3*(i - (double)N/2))) + TN;//4
            temp[i+1]=exp(-(double)i/N*log((double)T0/TN))*T0;//8
            //temp[i+1]=T0*exp(-(double)1/(N*N)*log((double)T0/TN)*i*i);//9
            //temp[i+1]=exp(-(double)20*i/N*log((double)T0/TN))*T0;//minha formula
            if(exp((double)-delta/temp[i])<(rand()%1001)/1000.0)
                r.flip(j);
        }
        printf("Iterações: %d\tCláusulas satisfeitas: %d\n",i,resps[k]=smaior);
        vits[k] = i-1;
        printBit(maior);
        media+=resps[k];
        its+= vits[k];
    }
    media/=IT;
    its/=IT;
    for(int i=0; i<IT; i++){
        dp+= (resps[i]-media)*(resps[i]-media);
        dpits+=(vits[i]-its)*(vits[i]-its);
    }
    dp/=IT;
    dpits/=IT;
    dp = sqrt(dp);
    dpits = sqrt(dpits);
    printf("Média de cláusulas satisfeitas: %lf\tDesvio Padrão: %lf\n",media,dp);
    printf("Média de iterações: %lf\tDesvio Padrão: %lf\n",its,dpits);

    for(int i=0; i<=vits[IT-1]; i++){
        fprintf(out,"%d %lf %d\n",i,temp[i],score[i]);
    }
    fclose(f);
    fclose(out);
    return 0;
}
