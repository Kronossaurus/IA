all:
	g++ AgrItens.cpp -Wall -o AgrItens -std=c++11 -pthread
	g++ AgrItensOpenMP.cpp -Wall -o AgrItensOpenMP -std=c++11 -pthread -fopenmp
	mpicc MPI.c -o AgrItensMPI -fopenmp -std=c99 -lm
semprint:
	g++ AgrItens.cpp -Wall -o AgrItens -std=c++11 -pthread -D semprint
	g++ AgrItensOpenMP.cpp -Wall -o AgrItensOpenMP -std=c++11 -pthread -fopenmp -D semprint
