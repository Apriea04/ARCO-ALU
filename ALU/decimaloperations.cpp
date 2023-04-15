#include "decimaloperations.h"

DecimalOperations::DecimalOperations(float op1, float op2, float *result)
{
    this->op1 = op1;
    this->op2 = op2;
    this->result = result;

}

DecimalOperations::~DecimalOperations()
{

}

void DecimalOperations::add()
{
    *result = op1 + op2;
}

void DecimalOperations::multiply()
{
    *result = op1 * op2;
}

void DecimalOperations::divide()
{
    *result = op1 / op2;
}



