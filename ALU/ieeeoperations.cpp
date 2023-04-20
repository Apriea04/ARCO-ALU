#include "ieeeoperations.h"
#include <iostream>
#include <bitset>
#include <sstream>


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
    //printf("Signo: %u \n",a.bitfield.sign);
    //printf("Exponente: %u \n",a.bitfield.expo);
    //printf("Parte Fraccionaria : %u \n", a.bitfield.partFrac);

    //Transforma a bits el exponente
    std::bitset<8> exponenteBin(a.bitfield.expo);
    //Transforma a bits la parte fraccionaria
    std::bitset<23> fraccionariaBin(a.bitfield.partFrac);

    //Devuelve el numero en binario IEEE754
    return std::to_string(a.bitfield.sign) + exponenteBin.to_string() + fraccionariaBin.to_string();
}

//Metodo que le pasas un string en binario y te devuelve un string en hexadecimal
string IEEEOperations::translateBinaryToHex(string bin)
{
    // Convertir el string binario a un objeto bitset
    bitset<64> bitSet(bin);

    // Convertir el objeto bitset a un número hexadecimal en un stringstream
    stringstream ss;
    ss << hex << uppercase << bitSet.to_ullong();
    string hexString = ss.str();
    return hexString;
}

void IEEEOperations::binaryTransform(){
    union Code a;

    a.numero = op1;


    signoA = a.bitfield.sign;
    exponenteA = a.bitfield.expo;
    mantisaA = a.bitfield.partFrac;



    union Code b;
    b.numero = op2;

    signoB = b.bitfield.sign;
    exponenteB = b.bitfield.expo;
    mantisaB = b.bitfield.partFrac;
}

//Metodo para calcular el complemento a 2 (Tengo serias dudas de que funcione bien)
int IEEEOperations::complementoDos(int n) {
    //Según ChatGPT, podemos negar un número con (~variable), luego el complemento a 2 es
    return (~n) + 1;
}


//Metodo de la suma
void IEEEOperations::add()
{
    //Metodo que saca mantisa, signo y exponente de A y B
    binaryTransform();

    //Paso 1
    int g = 0;
    int r = 0;
    int st = 0;
    int n = 24;
    bool operandos_intercambiados = false;
    bool complementado_P = false;


    //Paso 2
    if(exponenteA < exponenteB){
        //Intercambiamos los operandos
        unsigned int signoTemp = signoA;
        unsigned int exponenteTemp = exponenteA;
        unsigned int mantisaTemp = mantisaA;

        signoA = signoB;
        exponenteA = exponenteB;
        mantisaA = mantisaB;

        signoB = signoTemp;
        exponenteB = exponenteTemp;
        mantisaB = mantisaTemp;
        operandos_intercambiados = true;

    }

    //Paso 3

    unsigned int exponenteSuma = exponenteA;
    unsigned int d = exponenteA - exponenteB;

    //Paso 4

    if(exponenteA != exponenteB){
        mantisaB = complementoDos(mantisaB);
    }


    //Paso 5

    unsigned int p = mantisaB;

    //Paso 6



    //Test
    cout<<" Signo A: "<<signoA<<" Exponente A: "<<exponenteA<<" Mantisa A: "<<mantisaA<<endl;
    cout<<" Signo B: "<<signoB<<" Exponente B: "<<exponenteB<<" Mantisa B: "<<mantisaB<<endl;

    //Numero para tests
    *result = 69;
}






//Metodo de la multiplicacion
void IEEEOperations::multiply()
{
    //Metodo que saca mantisa, signo y exponente de A y B
    binaryTransform();

    //Test
    cout<<" Signo A: "<<signoA<<" Exponente A: "<<exponenteA<<" Mantisa A: "<<mantisaA<<endl;
    cout<<" Signo B: "<<signoB<<" Exponente B: "<<exponenteB<<" Mantisa B: "<<mantisaB<<endl;

    //Numero para tests
    *result = 777;
}


//Metodo de la division
void IEEEOperations::divide()
{

    //Metodo que saca mantisa, signo y exponente de A y B
    binaryTransform();

    //Test
    cout<<" Signo A: "<<signoA<<" Exponente A: "<<exponenteA<<" Mantisa A: "<<mantisaA<<endl;
    cout<<" Signo B: "<<signoB<<" Exponente B: "<<exponenteB<<" Mantisa B: "<<mantisaB<<endl;

    //Numero para tests
    *result = 3.14;
}
