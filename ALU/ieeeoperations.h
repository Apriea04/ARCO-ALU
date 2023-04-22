#ifndef IEEEOPERATIONS_H
#define IEEEOPERATIONS_H

#include <string>

using namespace std;

class IEEEOperations
{
public:
    IEEEOperations(float op1, float op2);
    ~IEEEOperations();
    string translateDecToIEEE(float op);
    string translateBinaryToHex(string bin);
    void add();
    void multiply();
    void divide();
    float getResult();
    string getIEEEResult();

private:
    float op1;
    float op2;
    union Code *result;

    unsigned int mantisaA : 24;
    unsigned int mantisaB : 24;

    float salida;

    unsigned int signoA;
    unsigned int exponenteA;
    unsigned int signoB;
    unsigned int exponenteB;
    void binaryTransform();
    unsigned int complementoDos(unsigned int n);
    unsigned int complementoUno(unsigned int n);
    unsigned int multiplyWithoutSign(unsigned int a, unsigned int b, unsigned int *p);
    bool checkOverflow(unsigned int *p);
    bool checkUnderflow(unsigned int *p);
    bool operandosOpuestos();
    bool esOp1Denormal();
    bool esOp2Denormal();

};

//Union code para sacar el exponente, parte fraccionaria y mantisa de cada float (Sacado del .c de clase)
union Code {

    struct{
        unsigned int partFrac : 23;
        unsigned int expo : 8;
        unsigned int sign : 1;
    }bitfield;

    float numero;
    unsigned int numerox;
};

#endif // IEEEOPERATIONS_H
