all:
	g++ AgrItens.cpp -Wall -o AgrItens -std=c++11 -pthread
	g++ AgrItensOpenMP.cpp -Wall -o AgrItensOpenMP -std=c++11 -pthread -fopenmp
semprint:
	g++ AgrItens.cpp -Wall -o AgrItens -std=c++11 -pthread -D semprint
	g++ AgrItensOpenMP.cpp -Wall -o AgrItensOpenMP -std=c++11 -pthread -fopenmp -D semprint
