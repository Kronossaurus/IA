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
void simular(Formiga *f, int n, char **mat, int nf, int z){
    //movimento
    int m, refazer;
    for(int p=z;p<nf+z && p<vivas;p++){
    	MPI_Irecv(*f, 1, MPI_PACKED, MPI_ANY, MSG_TAG, MPI_COMM_WORLD, MPI_Request *request)
            m = 1+rand()%8;
            if(m==1){
	            if(f[p].x>0 && f[p].y>0){
	                f[p].x--;
	                f[p].y--;
	                f[p].lastm = 1;
	            }
	            else{
	            	//mensagem
	            }
            }
            else if(m==2){
	            if (f[p].x>0){
	                f[p].x--;
	                f[p].lastm = 2;
	            }
	            else
	            	//mensagem
	        }
            else if(m==3){
	            if (f[p].x>0 && f[p].y<n-1){
	                f[p].x--;
	                f[p].y++;
	                f[p].lastm = 3;
	            }
	            else
	            	//mensagem
	        }
            else if(m==4){
	            if (f[p].y>0){
	                f[p].y--;
	                f[p].lastm = 4;
	            }
	            else
	            	//mensagem
	        }
            else if(m==5){
	            if (f[p].y<n-1){
	                f[p].y++;
	                f[p].lastm = 5;
	            }
	            else
	            	//mensagem
	        }
            else if(m==6){
	            if (f[p].x<n-1 && f[p].y>0){
	                f[p].x++;
	                f[p].y--;
	                f[p].lastm = 6;
	            }
	            else
	            	//mensagem
            }
            else if(m==7){
	            if (f[p].x<n-1){
	                f[p].x++;
	                f[p].lastm = 7;
	            }
	            else
	            	//mensagem
	        }
            else if(m==8){
	            if (f[p].x<n-1 && f[p].y<n-1){
	                f[p].x++;
	                f[p].y++;
	                f[p].lastm = 8;
	            }
	            else
	            	//mensagem
	        }

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
                MPI_Send(buf, n*n/size, MPI_PACKED, i*size+j,MSG_TAG, MPI_COMM_WORLD);
             }
        }

    } 

    MPI_Status status;
    void *inbuf = malloc(sizeof(char)*n*n/size/size);
    MPI_Recv(inbuf, sizeof(char)*n*n/size/size, MPI_PACKED, 0, MSG_TAG, MPI_COMM_WORLD, &status);

	int i;
	Formiga *formigas = (Formiga*)malloc(sizeof(Formiga)*vivas);
	for(i = 0; i < vivas; i++){
		formigas[i]->x = rand()%n/size;
		formigas[i]->y = rand()%n/size;
		formigas[i]->estado = 0;
		formigas[i]->raio = raio;
		formigas[i]->lastm = -1;
    }
    


 	MPI_Send(*f, Formiga*vivas, MPI_INT, i*size+j, MSG_TAG, MPI_COMM_WORLD);
 	MPI_Recv(*f, Formiga*vivas, MPI_INT, MPI_ANY, MSG_TAG, MPI_COMM_WORLD, &status);
    
    MPI_Finalize();
    return 0;
}


