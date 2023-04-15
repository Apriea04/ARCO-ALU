#include "ieeeoperations.h"
#include <iostream>
#include <bitset>


IEEEOperations::IEEEOperations(float op1, float op2, float *result)
{
    this->op1 = op1;
    this->op2 = op2;
    this->result = result;
}

IEEEOperations::~IEEEOperations()
{

}

//Metodo que le pasas un float y devuelve el binario ieee754
string IEEEOperations::translateDecToIEEE(float op)
{
    union Code a;

    a.numero = op;
//    printf("Signo: %u \n",a.bitfield.sign);
//    printf("Exponente: %u \n",a.bitfield.expo);
//    printf("Parte Fraccionaria : %u \n", a.bitfield.partFrac);

    std::bitset<8> exponenteBin(a.bitfield.expo);
    std::bitset<23> fraccionariaBin(a.bitfield.partFrac);

    return std::to_string(a.bitfield.sign) + exponenteBin.to_string() + fraccionariaBin.to_string();
}


void IEEEOperations::add()
{
    union Code a;

    a.numero = op1;


    std::bitset<8> exponenteBinA(a.bitfield.expo);
    std::bitset<23> fraccionariaBinA(a.bitfield.partFrac);
    int signoA = a.bitfield.sign;
    string exponenteA = exponenteBinA.to_string();
    string mantisaA = fraccionariaBinA.to_string();


    union Code b;
    b.numero = op2;

    std::bitset<8> exponenteBinB(b.bitfield.expo);
    std::bitset<23> fraccionariaBinB(a.bitfield.partFrac);
    int signoB = b.bitfield.sign;
    string exponenteB = exponenteBinB.to_string();
    string mantisaB = fraccionariaBinB.to_string();
    //Pruebas de conversión
    cout<<" Signo A: "<<signoA<<" Exponente A: "<<exponenteA<<" Mantisa A: "<<mantisaA<<endl;
    cout<<" Signo B: "<<signoB<<" Exponente B: "<<exponenteB<<" Mantisa B: "<<mantisaB<<endl;
    *result = 69;
}

void IEEEOperations::multiply()
{

}

void IEEEOperations::divide()
{

}
