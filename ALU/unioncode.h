
#ifndef UNIONCODE_H
#define UNIONCODE_H

/*
Pasar de real a IEEE754

Rellenamos los bits a la derecha hasta obtener el número
con 24 bits y desplazamos tantos bits a la izquierda como
necesitemos para que nos quede la parte entera
únicamente con un 1 y multiplicamos por 2 elevado a
tantas unidades como hemos desplazado el número.
*/

/*
Pasar de IEEE754 a real

Desplazamos la mantisa a la izquierda o a la derecha en
función del valor del exponente.

Eliminamos los ceros a la derecha

Pasamos de binario a decimal
*/

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
