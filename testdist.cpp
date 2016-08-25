#include <iostream>
#include <random>
#include <functional>
using namespace std;
int main(){
    default_random_engine gerador(time(NULL));
    normal_distribution<double> distribuicao(4.5,5);
    auto normal = bind(distribuicao, gerador);
    int x,aux;
    cin>>x;
    int s[8] = {0};
    for (int i=0;i<x;i++){
        do{
            aux = round(normal());
        }while(aux<1 || aux>8);
        s[aux-1]++;
    }
    for (int i=0;i<8;i++){
        cout<<i+1<<": "<<s[i]<<endl;
    }
    return 0;
}
