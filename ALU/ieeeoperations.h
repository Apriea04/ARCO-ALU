#ifndef IEEEOPERATIONS_H
#define IEEEOPERATIONS_H

#include <string>

using namespace std;

class IEEEOperations
{
public:
    IEEEOperations(long op1, long op2, long *result);
    ~IEEEOperations();
    string translateDecToIEEE(long op);
    void add();
    void multiply();
    void divide();

private:
    long op1;
    long op2;
    long *result;

};

#endif // IEEEOPERATIONS_H
