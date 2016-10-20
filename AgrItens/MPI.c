#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#define CHFOR '.'
#define MSG_TAG 0

typedef struct{
    int x, y, estado, raio, lastm;
    char id;
}Formiga;

void sendSquare(char **mat, int size ,int n){
    int i,j,k,position=0;
    
    for (i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            void *buf = malloc(sizeof(char)*n*n/size/size);
           
            for(k = 0; k < n/size; k++){
                MPI_Pack(&mat[i*n/size + k][j*n/size], n/size, MPI_CHAR, buf, sizeof(char)*n*n/size/size, &position, MPI_COMM_WORLD);
            }
            MPI_Send(buf, n*n/size/size, MPI_PACKED, i*size+j,MSG_TAG, MPI_COMM_WORLD);
         }
    }
}

void initMat(char **mat, int n){
    int i,j;
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            mat[i][j] = ' ';
        }
    }
}

void preencherMat(char **mat, int itens, int n){
    int i,j,k;
    
    for(k = 0; k < itens;){
        i = rand()%n;
        j = rand()%n;
    
        if(mat[i][j] != CHFOR){
            mat[i][j] = CHFOR;
            k++;
        }
    }
}

int main(int argc, char **argv){
    
    int rank, size;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(argc != 6){
        printf("Argumentos: Tamanho da matriz, Quantidade de itens, Quantidade de vivas, Raid de visão, iterações\n");
        return 0;
    }

    int n = atoi(argv[1]), itens = atoi(argv[2]), raio = atoi(argv[4]), it = atoi(argv[5]), vivas = atoi(argv[3]);
    
    if (rank == 0){
        char **mat = (char**)malloc(sizeof(char*)*n);
        char *data = (char *)malloc(sizeof(char*)*(n*n));
        
        int i,j,k,position=0;
    
        for (i = 0; i < n; i++) {
            mat[i] = &data[i * n];
        }
        
        initMat(mat, n);
        srand(time(NULL));
        preencherMat(mat, itens, n);
        //sendSquare(mat, size, n);

        for (i = 0; i < size; i++){
            for(j = 0; j < size; j++){
                void *buf = malloc(sizeof(char)*n*n/size/size);
               
                for(k = 0; k < n/size; k++){
                    MPI_Pack(&mat[i*n/size + k][j*n/size], n/size, MPI_CHAR, buf, sizeof(char)*n*n/size/size, &position, MPI_COMM_WORLD);
                }
                MPI_Send(buf, n*n/size/size, MPI_PACKED, i*size+j,MSG_TAG, MPI_COMM_WORLD);
             }
        }

    } 

    MPI_Status status;
    void *inbuf = malloc(sizeof(char)*n*n/size/size);
    MPI_Recv(inbuf, sizeof(char)*n*n/size/size, MPI_PACKED, 0, MSG_TAG, MPI_COMM_WORLD, &status);

    MPI_Finalize();
    return 0;
}
