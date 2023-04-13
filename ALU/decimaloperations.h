#ifndef DECIMALOPERATIONS_H
#define DECIMALOPERATIONS_H


class DecimalOperations
{
public:
    DecimalOperations(long op1, long op2, long *result);
    ~DecimalOperations();
    void add();
    void multiply();
    void divide();
private:
    long op1;
    long op2;
    long *result;
};

#endif // DECIMALOPERATIONS_H
