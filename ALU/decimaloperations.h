#ifndef DECIMALOPERATIONS_H
#define DECIMALOPERATIONS_H


class DecimalOperations
{
public:
    DecimalOperations(float op1, float op2, float *result);
    ~DecimalOperations();
    void add();
    void multiply();
    void divide();
private:
    float op1;
    float op2;
    float *result;
};

#endif // DECIMALOPERATIONS_H
