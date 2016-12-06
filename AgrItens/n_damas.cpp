#include<stdio.h>
#include<math.h>
#include<mpi/mpi.h>
#include<stdlib.h>

#define RECUO 4
int tabuleiro[300],cont;


// Coloca as damas no tabuleiro retorna 0 para possição errada ou 1 para posição certa
int colocar(int casa, int coluna){

	int i;
	for(i=1;i<=casa-1;++i){
            if(tabuleiro[i]==coluna){
                return 0;
            }else{
                if(abs(tabuleiro[i]-coluna)==abs(i-casa)){
                    return 0;
                }
            }
	}
	return 1;
}

//função recursiva que testa por backtracking a posição das damas
void rainha(int casa,int n){

    int coluna;
    for(coluna=1;coluna<=n;++coluna){
        if(colocar(casa, coluna)){
            tabuleiro[casa]=coluna;
            if(casa==n){
                //printf("test \n");
                cont++;
            }
            else{
                rainha(casa+1,n);
            }
        }
    }
}



int main(int argc, char** argv){

        MPI_Init(&argc, &argv);
	int n;
        int rank, size;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        n=atoi(argv[1]);
 	rainha(1,rank + RECUO);
 	printf("%d Numero de respostas: %d\n",rank + RECUO, cont);
        MPI_Finalize();
 	return 0;

}

