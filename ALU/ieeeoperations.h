#ifndef IEEEOPERATIONS_H
#define IEEEOPERATIONS_H

#include <string>

using namespace std;

class IEEEOperations
{
public:
    IEEEOperations(float op1, float op2, float *result);
    ~IEEEOperations();
    string translateDecToIEEE(float op);
    string translateBinaryToHex(string bin);
    void add();
    void multiply();
    void divide();

private:
    float op1;
    float op2;
    float *result;

};

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
