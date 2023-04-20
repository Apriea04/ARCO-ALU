#include "ieeeoperations.h"
#include <iostream>
#include <bitset>
#include <sstream>


IEEEOperations::IEEEOperations(float op1, float op2, float *result)
{
    this->op1 = op1;
    this->op2 = op2;
    this->result = result;
}

IEEEOperations::~IEEEOperations()
{

}

//Metodo que le pasas un float y devuelve el binario ieee754
string IEEEOperations::translateDecToIEEE(float op)
{
    union Code a;

    a.numero = op;
    //printf("Signo: %u \n",a.bitfield.sign);
    //printf("Exponente: %u \n",a.bitfield.expo);
    //printf("Parte Fraccionaria : %u \n", a.bitfield.partFrac);

    //Transforma a bits el exponente
    std::bitset<8> exponenteBin(a.bitfield.expo);
    //Transforma a bits la parte fraccionaria
    std::bitset<23> fraccionariaBin(a.bitfield.partFrac);

    //Devuelve el numero en binario IEEE754
    return std::to_string(a.bitfield.sign) + exponenteBin.to_string() + fraccionariaBin.to_string();
}

//Metodo que le pasas un string en binario y te devuelve un string en hexadecimal
string IEEEOperations::translateBinaryToHex(string bin)
{
    // Convertir el string binario a un objeto bitset
    bitset<64> bitSet(bin);

    // Convertir el objeto bitset a un número hexadecimal en un stringstream
    stringstream ss;
    ss << hex << uppercase << bitSet.to_ullong();
    string hexString = ss.str();
    return hexString;
}

//Este metodo parece que ya no se va a usar mas. TODO borrar si no se usa cuando acabemos
void IEEEOperations::binaryTransform(){
    union Code a;

    a.numero = op1;


    signoA = a.bitfield.sign;
    exponenteA = a.bitfield.expo;
    mantisaA = a.bitfield.partFrac;



    union Code b;
    b.numero = op2;

    signoB = b.bitfield.sign;
    exponenteB = b.bitfield.expo;
    mantisaB = b.bitfield.partFrac;
}

//Metodo para calcular el complemento a 2 (Tengo serias dudas de que funcione bien)
int IEEEOperations::complementoDos(int n) {
    //Según ChatGPT, podemos negar un número con (~variable), luego el complemento a 2 es
    return (~n) + 1;
}


//Metodo de la suma
void IEEEOperations::add()
{
    //Metodo que saca mantisa, signo y exponente de A y B
    //binaryTransform();

    Code a,b, result;
    a.numero = op1;
    b.numero = op2;

    //Añadimos un uno a la izquierda de la parte fraccionaria de la mantisa:
    unsigned int tempVal = 0;
    tempVal |= a.bitfield.partFrac; //Colocamos la parte fraccionaria a la derecha
    tempVal |= (1<<23); //Ponemos un uno a la izquierda
    a.mantisa = tempVal; //Guardamos esa mantisa en a
    tempVal |= b.bitfield.partFrac; //Colocamos la parte fraccionaria de b a la derecha
    b.mantisa = tempVal; //Guardamos esa mantisa en b.

    //Paso 1
    int g = 0;
    int r = 0;
    int st = 0;
    int n = 24;
    bool c= false; //Acarreo
    bool operandos_intercambiados = false;
    bool complementado_P = false;


    //Paso 2
    if(a.bitfield.expo < b.bitfield.expo){
        //Intercambiamos los operandos
        Code tmp = a;
        a = b;
        b = tmp;
        operandos_intercambiados = true;

    }

    //Paso 3

    result.bitfield.expo = a.bitfield.expo;
    unsigned int d = a.bitfield.expo - b.bitfield.expo; //TODO funciona sin decirle que son 8bits?

    //Paso 4

    if(a.bitfield.sign != b.bitfield.sign){
        b.mantisa = complementoDos(b.mantisa);
    }


    //Paso 5

    unsigned int p = b.bitfield.partFrac; //TODO funciona sin decirle que son 24 bits?

    //Paso 6

    g = (p >> (d-1)) & 1;
    r = (p >> (d-2)) & 1;

    unsigned int mask (1u << (d+1));  // Creamos una máscara de bits que tenga 1 en las posiciones 0 a d, inclusive
    unsigned int subset = p & mask;
    st = (subset!=0) ? 1:0;

    //Paso 7

    if (b.bitfield.sign != a.bitfield.sign) {
        unsigned int mask = (1u << (sizeof(unsigned int)*8 - d)) - 1;  // Creamos una máscara de bits que tenga 1 en las posiciones más altas y 0 en las posiciones más bajas
        p = p | (mask << d); // Desplazamos el valor de p d bits a la derecha e insertamos 1s en las posiciones más altas
    } else {
        p = p >> d;
    }

    //Paso 8
    p = p + a.mantisa;
    //¿Se ha producido acarreo?
    if (p & (1u << (sizeof(unsigned int)*8 -1))) {
        //Hubo un acarreo al final de la suma ya que este AND ya que no devuelve 0
        c = true; //c=1
    } else {
        c = false; //c = 0
    }

    //Paso 9

    if (a.bitfield.sign != b.bitfield.sign && ((p>>23 & 1u) == 1) && (c==false)) {
        p = complementoDos(p);
        complementado_P = true;
    }

    //Paso 10
    if (a.bitfield.sign == b.bitfield.sign && c) {
        st = g|r|st;
        r = p & 1u;
        p = p >> 1;
        //TODO continuar
    }

    //Test
    cout<<" Signo A: "<<a.bitfield.sign<<" Exponente A: "<<a.bitfield.expo<<" Mantisa A: "<<a.bitfield.partFrac<<endl;
    cout<<" Signo B: "<<b.bitfield.sign<<" Exponente B: "<<b.bitfield.expo<<" Mantisa B: "<<b.bitfield.partFrac<<endl;
    cout<<" Signo Result: "<<result.bitfield.sign<<" Exponente Result: "<<result.bitfield.expo<<" Mantisa Result: "<<result.bitfield.partFrac<<endl;

    //Numero para tests
    // *result = 69;
}






//Metodo de la multiplicacion
void IEEEOperations::multiply()
{
    //Metodo que saca mantisa, signo y exponente de A y B
    binaryTransform();

    //Test
    cout<<" Signo A: "<<signoA<<" Exponente A: "<<exponenteA<<" Mantisa A: "<<mantisaA<<endl;
    cout<<" Signo B: "<<signoB<<" Exponente B: "<<exponenteB<<" Mantisa B: "<<mantisaB<<endl;

    //Numero para tests
    *result = 777;
}


//Metodo de la division
void IEEEOperations::divide()
{

    //Metodo que saca mantisa, signo y exponente de A y B
    binaryTransform();

    //Test
    cout<<" Signo A: "<<signoA<<" Exponente A: "<<exponenteA<<" Mantisa A: "<<mantisaA<<endl;
    cout<<" Signo B: "<<signoB<<" Exponente B: "<<exponenteB<<" Mantisa B: "<<mantisaB<<endl;

    //Numero para tests
    *result = 3.14;
}
