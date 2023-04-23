
#ifndef UNIONCODE_H
#define UNIONCODE_H

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


#endif // UNIONCODE_H
