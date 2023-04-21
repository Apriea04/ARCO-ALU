#include "ieeeoperations.h"
#include <cmath>
#include <iostream>
#include <bitset>
#include <sstream>


IEEEOperations::IEEEOperations(float op1, float op2)
{
    this->op1 = op1;
    this->op2 = op2;
    this->result = result;
}

IEEEOperations::~IEEEOperations()
{

}


float IEEEOperations::getResult(){
    //return this->result->numero;
    return salida;
}
string IEEEOperations::getIEEEResult(){
    return std::to_string(this->result->bitfield.sign+this->result->bitfield.expo+this->result->bitfield.partFrac);
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

bool IEEEOperations::operandosIguales() {
    return op1 == op2;
}


bool IEEEOperations::esOp1Denormal() {
    Code a;
    a.numero = op1;
    return a.bitfield.expo == 0b00000000;
}

bool IEEEOperations::esOp2Denormal() {
    Code b;
    b.numero = op2;
    return b.bitfield.expo == 0b00000000;
}

//Metodo de la suma
void IEEEOperations::add()
{
    //Metodo que saca mantisa, signo y exponente de A y B
    //binaryTransform();

    union Code a,b, result;
    a.numero = op1;
    b.numero = op2;

    //Añadimos un uno a la izquierda de la parte fraccionaria de la mantisa:
    unsigned int tempVal = 0;
    tempVal |= a.bitfield.partFrac; //Colocamos la parte fraccionaria a la derecha
    tempVal |= (1<<23); //Ponemos un uno a la izquierda
    a.mantisa = tempVal; //Guardamos esa mantisa en a
    tempVal |= b.bitfield.partFrac; //Colocamos la parte fraccionaria de b a la derecha
    b.mantisa = tempVal; //Guardamos esa mantisa en b.

    //Casos raros:
    if (operandosIguales()) {
        result.numero=0;
    } else{

        if (esOp1Denormal()) {
            tempVal &= ~(1<<23); //Ponemos un cero a la izquierda
            a.mantisa=tempVal;
        }
        if (esOp2Denormal()) {
            tempVal &= ~(1<<23); //Ponemos un cero a la izquierda
            b.mantisa=tempVal;
        }
    }

    //Paso 1
    cout<<"Paso 1: "<<endl;
    int g = 0;
    int r = 0;
    int st = 0;
    int n = 24;
    bool c= false; //Acarreo
    bool operandos_intercambiados = false;
    bool complementado_P = false;
    unsigned int unos = 0b111111111111111111111111;
    unsigned int mask;


    //Paso 2
    cout<<"Paso 2: "<<endl;
    if(a.bitfield.expo < b.bitfield.expo){
        //Intercambiamos los operandos
        Code tmp = a;
        a = b;
        b = tmp;
        operandos_intercambiados = true;

    }

    //Paso 3
    cout<<"Paso 3: "<<endl;
    result.bitfield.expo = a.bitfield.expo;
    unsigned int d = a.bitfield.expo - b.bitfield.expo; //TODO funciona sin decirle que son 8bits?

    //Paso 4
    cout<<"Paso 4: "<<endl;

    if(a.bitfield.sign != b.bitfield.sign){
        b.mantisa = complementoDos(b.mantisa);
    }


    //Paso 5
    cout<<"Paso 5: "<<endl;

    unsigned int p = b.mantisa; //TODO funciona sin decirle que son 24 bits?

    //Paso 6
    cout<<"Paso 6: "<<endl;
    if (d>=1) {
        g = (p >> (d-1)) & 1;
    }
    if (d>=2) {
        r = (p >> (d-2)) & 1;
    }

    if (d>=3) {

        mask = (1u << (d+1));  // Creamos una máscara de bits que tenga 1 en las posiciones 0 a d, inclusive
        unsigned int subset = p & mask;
        st = (subset!=0) ? 1:0;
    }
    //Paso 7
    cout<<"Paso 7: "<<endl;
    if (b.bitfield.sign != a.bitfield.sign) {
        mask = (1u << (sizeof(unsigned int)*8 - d)) - 1;  // Creamos una máscara de bits que tenga 1 en las posiciones más altas y 0 en las posiciones más bajas
        p = p | (mask << d); // Desplazamos el valor de p d bits a la derecha e insertamos 1s en las posiciones más altas
    } else {
        p = p >> d;
    }

    //Paso 8
    cout<<"Paso 8: "<<endl;
    p = p + a.mantisa;

    //¿Se ha producido desbordamiento? (un acarreo al final)
    if (p>=16777216) { //Si p >= 2^24, es que ocupa 25 bits y el primero es un uno, esdecir, hubo desbordamiento y acarreo
        //Hubo un acarreo al final de la suma ya que este AND ya que no devuelve 0
        p = p & 0b111111111111111111111111; //Me cargo ese uno que se añadió al producirse desbordamiento
        c = true; //c=1
        mask = 0b100000000000000000000000;
    } else {
        c = false; //c = 0
        mask = 0b011111111111111111111111;
    }

    //Paso 9
    cout<<"Paso 9: "<<endl;
    if (a.bitfield.sign != b.bitfield.sign && ((p>>23 & 1u) == 1) && (c==false)) {
        p = complementoDos(p);
        complementado_P = true;
    }

    //Paso 10
    cout<<"Paso 10: "<<endl;
    if (a.bitfield.sign == b.bitfield.sign && c) {
        st = g|r|st;
        r = p & 1u;
        p = p >> 1;
        if (c) {
            p = p|mask;
        } else {
            p = p&mask;
        }
        //Ajusto el exponente de la suma:
        result.bitfield.expo += 1;
    } else {
        //Calculo cuantos bits tengo que desplazar P para que sea una mantisa normalizada.
        // Para responder a eso, me basta con saber donde está el primer uno empezando por la izquierda.
        // Eso se puede hacer con log2:
        int k = 23;
        k -= std::log2(p);

        if (k == 0){
            st = r|st;
            r = g;
        } else if (k>1) {
            r=0;
            st=0;
        }

        //Desplazar (P,g) a la izquierda k bits:
        if (k != 0) { //Si k es 0, no hya nada que desplazar, no?? TODO
            p = p <<1; //Desplazo P a la izquierda dejando un 0 al final
            p = p | g; //Con el 0 hago un or para poner el valor de g
            //Ya se ha desplazado p,g una unidad. Se hacen los desplazamientos restantes:
            p = p << (k-1);
        }
        result.bitfield.expo -=k;
    }

    //Paso 11
    cout<<"Paso 11: "<<endl;
    unsigned int p0 = p & 1;
    if ((r==1 && st==1) || (r==1 && st==0 && p0 == 1)) {
        p+=1;
        if (c) { //Si hay acarreo, c=1
            p = p >> 1; //Desplazo 1 bit a la derecha p
            p = p | 0b100000000000000000000000; //Añado el uno del carry al principio
            result.bitfield.expo += 1; //Ajustamos el exponente
        }
    }
    //result.mantisa=p; //Falla

    //Paso 12
    cout<<"Paso 12: "<<endl;
    if ((operandos_intercambiados == false) && (complementado_P==true)) {
        result.bitfield.sign = b.bitfield.sign;
    } else {
        result.bitfield.sign = a.bitfield.sign;
    }

    //Paso 13
    cout<<"Paso 13: "<<endl;
    //Normalizo la mantisa
    int normalizador = 23 - log2(p) + 1;
    p= p<<normalizador;
    result.bitfield.expo -= normalizador;
    result.bitfield.partFrac = (p & 0x7FFFFF);

    this->result = &result;

    //Test
    cout<<" Signo A: "<<a.bitfield.sign<<" Exponente A: "<<a.bitfield.expo<<" Mantisa A: "<<a.bitfield.partFrac<<endl;
    cout<<" Signo B: "<<b.bitfield.sign<<" Exponente B: "<<b.bitfield.expo<<" Mantisa B: "<<b.bitfield.partFrac<<endl;
    cout<<" Signo Result: "<<result.bitfield.sign<<" Exponente Result: "<<result.bitfield.expo<<" Mantisa Result: "<<result.bitfield.partFrac<<endl;

    salida = result.numero;
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
}
