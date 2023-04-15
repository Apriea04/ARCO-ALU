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


    int signoA = a.bitfield.sign;
    int exponenteA = a.bitfield.expo;
    int mantisaA = a.bitfield.partFrac;



    union Code b;
    b.numero = op2;

    int signoB = b.bitfield.sign;
    int exponenteB = b.bitfield.expo;
    int mantisaB = b.bitfield.partFrac;

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
