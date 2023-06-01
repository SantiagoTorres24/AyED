#include <iostream>
using namespace std;

float CalcularPi () {
    float pi=0;
    float t=1;
    int s=1;

    while (abs(4*pi-3.141592) >=0.0000001) // Para 6 decimales 
    {
        pi+=s*(1/t); // Se van sumando valores a pi
        s*= -1; // Se va cambiando el signo
        t+= 2; // Se va sumando de a 2 el denominador, manteniendo la imparidad del mismo
        }
    return 4*pi;
    }

    int main () {
        float pi= CalcularPi();
        cout.precision (7);
        cout<<"El valor de pi es:"<<pi<<endl;
        return 0;
    }
