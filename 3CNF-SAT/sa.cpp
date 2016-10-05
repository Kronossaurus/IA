#include <cstdio>
#include <string>
using namespace std;
int v,nc;
typedef struct cl{
    bool b1,b2,b3;
    unsigned int p1,p2,p3;
}cl;
int f(bitset r, cl *vet){
    int cont = 0;
    for(int i=0; i<nc; i++){
    }
    return cont;
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
    c = getc(f);

    fscanf("p cnf %d %d", &v, &nc);
    bitset<v> r;
    cl vet[nc];

    for(int i=0, i<v; i++){
        if(rand()%2 == 1)
            r.set(i);
    }

    fclose(f);
    return 0;
}
