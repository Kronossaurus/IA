#include <stdlib.h>
#include <stdio.h>
#include <mpi/mpi.h> //prestar atenção nesse cara aqui
#include <math.h>
#include <time.h>
#include <omp.h>
#define CHFOR '.'
#define MSG_TAG 0

omp_lock_t mtx[200][200];
typedef struct{
    int x, y, estado, raio, lastm;
    char id;
}Formiga;

int rank, size;
void sendSquare(char **mat, int size ,int n){
    int i,j,k,position=0;
    for (i = 0; i < sqrt(size); i++){
        for(j = 0; j < sqrt(size); j++){
            void *buf = malloc(sizeof(char)*n*n); //n = dim
            for(k = 0; k < n; k++){//size = num comp
                MPI_Pack(&mat[i*n + k][j*n], n, MPI_CHAR, buf, sizeof(char)*n, &position, MPI_COMM_WORLD);
            }
            MPI_Send(buf, n, MPI_PACKED, i*sqrt(size)+j, MSG_TAG, MPI_COMM_WORLD);
            free(buf);
         }
    }
}

char** recvSquare(int n, int size){
    char **mat = (char**)malloc(sizeof(char*)*n);
    int i=0;
    for(;i<n;i++){
        mat[i] = (char*)malloc(sizeof(char));
    }
    void *inbuf = malloc(sizeof(char)*n*n);
    MPI_Status status;
    int position = 0;
    MPI_Recv(inbuf, sizeof(char)*n*n, MPI_PACKED, 0, MSG_TAG, MPI_COMM_WORLD, &status);
//for para desempacotar
    MPI_Unpack(inbuf, sizeof(char)*n, &position, &mat[position][0], n, MPI_CHAR, MPI_COMM_WORLD);
    return mat;
}

void sendFormiga(Formiga *f, int n, int send, int *vivas){
    int position;
    void *buf = malloc(sizeof(char)+sizeof(int)*5);

    MPI_Pack(&f[n].x, 1, MPI_INT, buf, sizeof(int), &position, MPI_COMM_WORLD);
    MPI_Pack(&f[n].y, 1, MPI_INT, buf, sizeof(int), &position, MPI_COMM_WORLD);
    MPI_Pack(&f[n].estado, 1, MPI_INT, buf, sizeof(int), &position, MPI_COMM_WORLD);
    MPI_Pack(&f[n].raio, 1, MPI_INT, buf, sizeof(int), &position, MPI_COMM_WORLD);
    MPI_Pack(&f[n].lastm, 1, MPI_INT, buf, sizeof(int), &position, MPI_COMM_WORLD);
    MPI_Pack(&f[n].id, 1, MPI_CHAR, buf, sizeof(char), &position, MPI_COMM_WORLD);

    MPI_Send(buf, 1, MPI_PACKED, send,MSG_TAG, MPI_COMM_WORLD);

    f[n] = f[--(*vivas)];
    f = (Formiga*)realloc(f, sizeof(Formiga)*(*vivas));
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
void simular(Formiga *f, int n, char **mat, int nf, int z, int *vivas){
    //movimento
    int m, flag;
    MPI_Request request;
    MPI_Status status;
    void *inbuf = malloc(sizeof(char)+sizeof(int)*5);
    for(int p=z;p<nf+z && p<*vivas;p++){
    	MPI_Irecv(inbuf, 1, MPI_PACKED, MPI_ANY_SOURCE, MSG_TAG, MPI_COMM_WORLD, &request);
    	MPI_Test(&request, &flag, &status);
    	if(flag == 1){//recvFormiga
            f = (Formiga*)realloc(f, sizeof(Formiga)*++(*vivas));
            int position;
            MPI_Unpack(inbuf, sizeof(int), &position, &f[*vivas-1].x, 1, MPI_INT, MPI_COMM_WORLD);
            MPI_Unpack(inbuf, sizeof(int), &position, &f[*vivas-1].y, 1, MPI_INT, MPI_COMM_WORLD);
            MPI_Unpack(inbuf, sizeof(int), &position, &f[*vivas-1].estado, 1, MPI_INT, MPI_COMM_WORLD);
            MPI_Unpack(inbuf, sizeof(int), &position, &f[*vivas-1].raio, 1, MPI_INT, MPI_COMM_WORLD);
            MPI_Unpack(inbuf, sizeof(int), &position, &f[*vivas-1].lastm, 1, MPI_INT, MPI_COMM_WORLD);
            MPI_Unpack(inbuf, sizeof(char), &position, &f[*vivas-1].id, 1, MPI_CHAR, MPI_COMM_WORLD);
        }
        m = 1+rand()%8;
            if(m==1){
	            if(f[p].x>0 && f[p].y>0){
	                f[p].x--;
	                f[p].y--;
	                f[p].lastm = 1;
	            }
	            else{
                        if(rank-sqrt(size) < 0)
                            sendFormiga(f,n,rank+sqrt(size)*(sqrt(size)-1),vivas);
                        else
                            sendFormiga(f,n,rank-sqrt(size),vivas);

                        if((rank)%(int)sqrt(size) == 0)
                            sendFormiga(f,n,rank+sqrt(size),vivas);
                        else
                            sendFormiga(f,n,rank-1,vivas);
	            	//mensagem
	            }
            }
            else if(m==2){
	            if (f[p].x>0){
	                f[p].x--;
	                f[p].lastm = 2;
	            }
	            else{
                        if(rank-sqrt(size) < 0)
                            sendFormiga(f,n,rank+sqrt(size)*(sqrt(size)-1),vivas);
                        else
                            sendFormiga(f,n,rank-sqrt(size),vivas);
	            }
	        }
            else if(m==3){
	            if (f[p].x>0 && f[p].y<n-1){
	                f[p].x--;
	                f[p].y++;
	                f[p].lastm = 3;
	            }
	            else{
                        if(rank-sqrt(size) < 0)
                            sendFormiga(f,n,rank+sqrt(size)*(sqrt(size)-1),vivas);
                        else
                            sendFormiga(f,n,rank-sqrt(size),vivas);
	            	//mensagem
                        if((rank+1)%(int)sqrt(size) == 0)
                            sendFormiga(f,n,rank-sqrt(size),vivas);
                        else
                            sendFormiga(f,n,rank+1,vivas);
	            }
	        }
            else if(m==4){
	            if (f[p].y>0){
	                f[p].y--;
	                f[p].lastm = 4;
	            }
	            else{
                        if((rank)%(int)sqrt(size) == 0)
                            sendFormiga(f,n,rank+sqrt(size),vivas);
                        else
                            sendFormiga(f,n,rank-1,vivas);
	            }
	        }
            else if(m==5){
	            if (f[p].y<n-1){
	                f[p].y++;
	                f[p].lastm = 5;
	            }
	            else{
                        if((rank+1)%(int)sqrt(size) == 0)
                            sendFormiga(f,n,rank-sqrt(size),vivas);
                        else
                            sendFormiga(f,n,rank+1,vivas);
	            }
	        }
            else if(m==6){
	            if (f[p].x<n-1 && f[p].y>0){
	                f[p].x++;
	                f[p].y--;
	                f[p].lastm = 6;
	            }
	            else{
                        if(rank+sqrt(size) > size-1)
                            sendFormiga(f,n,rank-sqrt(size)*(sqrt(size)-1),vivas);
                        else
                            sendFormiga(f,n,rank+sqrt(size),vivas);
	            	//mensagem
                        if((rank)%(int)sqrt(size) == 0)
                            sendFormiga(f,n,rank+sqrt(size),vivas);
                        else
                            sendFormiga(f,n,rank-1,vivas);
	            }
            }
            else if(m==7){
	            if (f[p].x<n-1){
	                f[p].x++;
	                f[p].lastm = 7;
	            }
	            else{
                        if(rank+sqrt(size) > size-1)
                            sendFormiga(f,n,rank-sqrt(size)*(sqrt(size)-1),vivas);
                        else
                            sendFormiga(f,n,rank+sqrt(size),vivas);
	            }
	        }
            else if(m==8){
	            if (f[p].x<n-1 && f[p].y<n-1){
	                f[p].x++;
	                f[p].y++;
	                f[p].lastm = 8;
	            }
	            else{
                        if(rank+sqrt(size) > size-1)
                            sendFormiga(f,n,rank-sqrt(size)*(sqrt(size)-1),vivas);
                        else
                            sendFormiga(f,n,rank+sqrt(size),vivas);
	            	//mensagem
                        if((rank+1)%(int)sqrt(size) == 0)
                            sendFormiga(f,n,rank-sqrt(size),vivas);
                        else
                            sendFormiga(f,n,rank+1,vivas);
	            }
	        }

        //decisao
        omp_set_lock(&mtx[f[p].x][f[p].y]);
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
        omp_unset_lock(&mtx[f[p].x][f[p].y]);
    }
    free(inbuf);
}
int main(int argc, char **argv){
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(argc != 6){
        printf("Argumentos: Tamanho da matriz de cada computador, Quantidade de itens, Quantidade de vivas, Raid de visão, iterações\n");
        return 0;
    }

    int n = atoi(argv[1]), itens = atoi(argv[2]), raio = atoi(argv[4]), it = atoi(argv[5]), vivas = atoi(argv[3]);
    if (rank == 0){
        char **mat = (char**)malloc(sizeof(char*)*n*sqrt(size));
        char *data = (char *)malloc(sizeof(char*)*(n*n*size));
        int i,j,k,position=0;
        for (i = 0; i < n*sqrt(size); i++) {
            mat[i] = &data[i * n*(int)sqrt(size)];
        }
        initMat(mat, n*sqrt(size));
        srand(time(NULL));
        preencherMat(mat, itens, n*sqrt(size));
        sendSquare(mat, size, n*sqrt(size));

        /*for (i = 0; i < size; i++){
            for(j = 0; j < size; j++){
                void *buf = malloc(sizeof(char)*n*n/size/size);
                for(k = 0; k < n/size; k++){
                    MPI_Pack(&mat[i*n/size + k][j*n/size], n/size, MPI_CHAR, buf, sizeof(char)*n*n/size/size, &position, MPI_COMM_WORLD);
                }
                MPI_Send(buf, n*n/size, MPI_PACKED, i*size+j,MSG_TAG, MPI_COMM_WORLD);

             }
        }*/
        free(mat);
        free(data);
    }

    recvSquare(n, size);
    char **mat = recvSquare(n,size);
    int j, i;
    Formiga *formigas = (Formiga*)malloc(sizeof(Formiga)*vivas);
    for(i = 0; i < vivas; i++){
        formigas[i].x = rand()%n;
        formigas[i].y = rand()%n;
        formigas[i].estado = 0;
        formigas[i].raio = raio;
        formigas[i].lastm = -1;
    }

    for(i = 0; i < it; i++){
    	#pragma omp parallel for shared(formigas, n, mat)
    	for (j = 0; j < 8; ++j)
    	{
            simular(formigas, n, mat, vivas, ceil(vivas/8.0), &vivas);
    	}
    }

    free(formigas);
    free(mat);

    MPI_Finalize();
    return 0;
}
