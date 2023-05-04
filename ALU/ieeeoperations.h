#ifndef IEEEOPERATIONS_H
#define IEEEOPERATIONS_H

#include <string>
#include "unioncode.h"
#include <cmath>
#include <iostream>
#include <bitset>
#include <sstream>

using namespace std;

class IEEEOperations
{
public:
    IEEEOperations(union Code op1, union Code op2);
    ~IEEEOperations();
    string translateIEEE(union Code op);
    string translateHex(union Code op);
    void add();
    float addVals(float a, float b);
    void multiply();
    float multiplyVals(float a, float b, bool *resultDenormal);
    void divide();
    union Code getResult();
    void operations(string n);
    bool getMulti();

private:
    union Code op1;
    union Code op2;
    union Code result;

    unsigned int mantisaA : 24;
    unsigned int mantisaB : 24;

    union Code salida;

    unsigned int signoA;
    unsigned int exponenteA;
    unsigned int signoB;
    unsigned int exponenteB;
    unsigned int complementoDos(unsigned int n);
    unsigned int complementoUno(unsigned int n);
    unsigned int multiplyWithoutSign(bitset<24> *MA, bitset<24> MB);
    bool checkOverflow(int exponent);
    bool checkUnderflow(int exponent);
    bool operandosOpuestos(float a, float b);
    bool esOp1Denormal();
    bool esOp2Denormal();
    bool esOpDenormal(Code num);
    bool multi;

    //Metodos para debug
    string intBinario(unsigned int op);
    string intHex(unsigned int  op);


};



#endif // IEEEOPERATIONS_H
