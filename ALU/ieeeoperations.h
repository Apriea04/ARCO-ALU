#ifndef IEEEOPERATIONS_H
#define IEEEOPERATIONS_H

#include <string>
#include "unioncode.h"

using namespace std;

class IEEEOperations
{
public:
    IEEEOperations(union Code op1, union Code op2);
    ~IEEEOperations();
    string translateIEEE(union Code op);
    string translateHex(union Code op);
    void add();
    void multiply();
    void divide();
    union Code getResult();

private:
    union Code op1;
    union Code op2;
    union Code *result;

    unsigned int mantisaA : 24;
    unsigned int mantisaB : 24;

    union Code salida;

    unsigned int signoA;
    unsigned int exponenteA;
    unsigned int signoB;
    unsigned int exponenteB;
    unsigned int complementoDos(unsigned int n);
    unsigned int complementoUno(unsigned int n);
    unsigned int multiplyWithoutSign();
    bool checkOverflow();
    bool checkUnderflow();
    bool operandosOpuestos();
    bool esOp1Denormal();
    bool esOp2Denormal();

};



#endif // IEEEOPERATIONS_H
