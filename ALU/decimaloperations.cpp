#include "decimaloperations.h"

DecimalOperations::DecimalOperations(long op1, long op2, long *result)
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



