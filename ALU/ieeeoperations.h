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
    string translateDecToIEEE(union Code op);
    string translateBinaryToHex(string bin);
    void add();
    void multiply();
    void divide();
    union Code getResult();
    string getIEEEResult();

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
    void binaryTransform();
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
