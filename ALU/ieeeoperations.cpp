#include "ieeeoperations.h"
#include <cmath>
#include <iostream>
#include <bitset>
#include <sstream>


//Constructor
IEEEOperations::IEEEOperations(union Code op1, union Code op2)
{
    this->op1 = op1;
    this->op2 = op2;
    //this->result = result;
}

//Destructor
IEEEOperations::~IEEEOperations()
{

}


//Metodo que devuelve el resultado de una operacion
union Code IEEEOperations::getResult(){
    return salida;
}


//string IEEEOperations::getIEEEResult(){
//    return std::to_string(this->result->bitfield.sign+this->result->bitfield.expo+this->result->bitfield.partFrac);
//}


//Metodo que le pasas un float y devuelve el binario ieee754
string IEEEOperations::translateDecToIEEE(union Code op)
{
    //Transforma a bits el exponente
    std::bitset<8> exponenteBin(op.bitfield.expo);
    //Transforma a bits la parte fraccionaria
    std::bitset<23> fraccionariaBin(op.bitfield.partFrac);
    //Devuelve el numero en binario IEEE754
    return std::to_string(op.bitfield.sign) + exponenteBin.to_string() + fraccionariaBin.to_string();
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

    //a.numero = op1;
    op1 = a;

    signoA = a.bitfield.sign;
    exponenteA = a.bitfield.expo;
    mantisaA = a.bitfield.partFrac;



    union Code b;
    //b.numero = op2;
    op2 = b;

    signoB = b.bitfield.sign;
    exponenteB = b.bitfield.expo;
    mantisaB = b.bitfield.partFrac;
}

//Metodo para calcular el complemento a 2 (Tengo serias dudas de que funcione bien)
unsigned int IEEEOperations::complementoDos(unsigned int n) {
    //Según ChatGPT, podemos negar un número con (~variable), luego el complemento a 2 es
    unsigned int tmp = ~n +1; //Hago el complemento a 2
    if (tmp > 0xFFFFFF) {
        tmp %= 0xFFFFFF +1; //Lo dejo con 24 bits
    }
    return tmp;

}

unsigned int IEEEOperations::complementoUno(unsigned int n) {

    bitset<24> tmp{n};
    tmp = ~tmp;
    unsigned int negated = tmp.to_ullong();
    return negated;

}

bool IEEEOperations::operandosOpuestos() {
    return op1.numero == -op2.numero;
}


bool IEEEOperations::esOp1Denormal() {
    Code a;
    a.numero = op1.numero;
    return a.bitfield.expo == 0b00000000;
}

bool IEEEOperations::esOp2Denormal() {
    Code b;
    b.numero = op2.numero;
    return b.bitfield.expo == 0b00000000;
}

//Metodo de la suma
void IEEEOperations::add()
{
    //Metodo que saca mantisa, signo y exponente de A y B
    //binaryTransform();

    union Code a,b, result;
    a.numero = op1.numero;
    b.numero = op2.numero;

    mantisaA=a.bitfield.partFrac | 0x800000;
    mantisaB=b.bitfield.partFrac | 0x800000;

    //Casos raros:
    if (operandosOpuestos()) {
        if (a.bitfield.expo == 0xFF && a.bitfield.partFrac == 0) {
            result.bitfield.expo = 0xFF;
            result.bitfield.partFrac = 0x002000;
            //Forzamos un NaN
            salida.numero = std::numeric_limits<float>::quiet_NaN();
        } else {
            result.numero=0;
            salida.numero = 0;
            //TODO debería sobrar una de las dos líneas
        }
        return;
    } else{

        if (esOp1Denormal()) {
            mantisaA &= 0b011111111111111111111111;
        }
        if (esOp2Denormal()) {
            mantisaB &= 0b011111111111111111111111;
        }
    }

    //Paso 1
    cout<<"Paso 1: "<<endl;

    cout<<" Signo A: "<<a.bitfield.sign<<" Exponente A: "<<a.bitfield.expo<<" Fraccionaria A: "<<a.bitfield.partFrac<<endl;
    cout<<" Signo B: "<<b.bitfield.sign<<" Exponente B: "<<b.bitfield.expo<<" Fraccionaria B: "<<b.bitfield.partFrac<<endl;

    int g = 0;
    int r = 0;
    int st = 0;
    int n = 24;
    bool c= false; //Acarreo
    bool operandos_intercambiados = false;
    bool complementado_P = false;
    unsigned int mask;


    //Paso 2
    cout<<"Paso 2: Si a < b intercambiamos"<<endl;
    if(a.bitfield.expo < b.bitfield.expo){
        //Intercambiamos los operandos
        Code tmp = a;
        a = b;
        b = tmp;
        mask = mantisaA;
        mantisaA = mantisaB;
        mantisaB = mask;
        operandos_intercambiados = true;
    }
    cout<<" Signo A: "<<a.bitfield.sign<<" Exponente A: "<<a.bitfield.expo<<" Fraccionaria A: "<<a.bitfield.partFrac<<endl;
    cout<<" Signo B: "<<b.bitfield.sign<<" Exponente B: "<<b.bitfield.expo<<" Fraccionaria B: "<<b.bitfield.partFrac<<endl;

    //Paso 3
    cout<<"Paso 3: "<<endl;

    result.bitfield.expo = a.bitfield.expo;
    unsigned int d = a.bitfield.expo - b.bitfield.expo; //TODO funciona sin decirle que son 8bits? D: Creo que si, con el cout salen cosas decentes


    cout<<"Exponente suma: "<<result.bitfield.expo<<" d: "<<d<<endl;

    //Paso 4
    cout<<"Paso 4: "<<endl;

    if(a.bitfield.sign != b.bitfield.sign){
        mantisaB = complementoDos(mantisaB);
        cout<<"Los signos no coinciden. Mantisa B: "<<mantisaB<<endl;
    }
    else{
        //La mantisa que da aquí es un poco rara
        cout<<"Los signos coinciden. Mantisa B: "<<mantisaB<<endl;
    }


    //Paso 5
    cout<<"Paso 5: "<<endl;

    unsigned int p = mantisaB; //TODO funciona sin decirle que son 24 bits?
    cout<<"Valor de p: "<<p<<endl;

    //Paso 6
    cout<<"Paso 6: "<<endl;
    if (d>=1) {
        g = (p >> (d-1)) & 1;
        cout<<"Valor de g cuando d >= 1: "<<g<<endl;
    }
    if (d>=2) {
        r = (p >> (d-2)) & 1;
        cout<<"Valor de r cuando d >= 2: "<<r<<endl;
    }

    if (d>=3) {

        mask = (1u << (d+1));  // Creamos una máscara de bits que tenga 1 en las posiciones 0 a d, inclusive
        unsigned int subset = p & mask;
        st = (subset!=0) ? 1:0;
        cout<<"Valor de mask cuando d >= 3: "<<mask<<endl<<"Valor de st: "<<st<<endl;
    }
    //Paso 7
    cout<<"Paso 7: "<<endl;
    if (b.bitfield.sign != a.bitfield.sign) {
        p = complementoUno(p);
        p = p >> d;
        p = complementoUno(p);
        cout<<"Valor de mask cuando signos de a y b no coinciden: "<<mask<<endl<<"Valor de p: "<<p<<endl;
    } else {
        p = p >> d;
        cout<<"Si los valores coiniden: valor de p "<<p<<endl;
    }

    //Paso 8
    cout<<"Paso 8: "<<endl;
    p = p + mantisaA;

    cout<<"Valor de p: "<<p<<endl;

    //¿Se ha producido desbordamiento? (un acarreo al final)
    if (p>=16777216) { //Si p >= 2^24, es que ocupa 25 bits y el primero es un uno, esdecir, hubo desbordamiento y acarreo
        //Hubo un acarreo al final de la suma ya que este AND ya que no devuelve 0
        p = p & 0b111111111111111111111111; //Me cargo ese uno que se añadió al producirse desbordamiento
        c = true; //c=1
        mask = 0b100000000000000000000000;

        cout<<"Hubo acarreo al final de la suma. Valor de p: "<<p<<" Valor de c: "<<c<<"Valor de mask: "<<mask<<endl;
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
    cout<<" Signo A: "<<a.bitfield.sign<<" Exponente A: "<<a.bitfield.expo<<" Fraccionaria A: "<<a.bitfield.partFrac<<endl;
    cout<<" Signo B: "<<b.bitfield.sign<<" Exponente B: "<<b.bitfield.expo<<" Fraccionaria B: "<<b.bitfield.partFrac<<endl;
    cout<<" Signo Result: "<<result.bitfield.sign<<" Exponente Result: "<<result.bitfield.expo<<" Fraccionaria Result: "<<result.bitfield.partFrac<<endl;

    salida = result;
}


unsigned int IEEEOperations::multiplyWithoutSign()
{
    bool c = false;      // Acarreo
    unsigned int p= 0;
    int n = 24;

    for (int i = 0; i < n; i++)
    {
        // Parte 1
        if(mantisaA%2==1)
        {
            p = p + mantisaB;
            //¿Se ha producido desbordamiento? (un acarreo al final)
            if (p>=16777216) { //Si p >= 2^24, es que ocupa 25 bits y el primero es un uno, es decir, hubo desbordamiento y acarreo
                //Hubo un acarreo al final de la suma ya que este AND ya que no devuelve 0
                p = p & 0xFFFFFF; //Me cargo ese uno que se añadió al producirse desbordamiento
                c = true; //c=1

            } else {
                c = false; //c = 0
            }
        } //Else: p+=0

        // Parte 2
        mantisaA = mantisaA >> 1;

        if(p%2==1)
        {
            mantisaA = mantisaA | 0x800000; //Ponemos en a el uno a la izquierda de P0
        }

        p = p >> 1;

        if(c==true)
        {
            p = p | 0x800000; //Ponemos a la izquierda de p el 1 de c
        }

        c = false; //Al desplazar, en c entra un 0
    }

}

bool IEEEOperations::checkOverflow()
{
    return this->result->bitfield.expo>254;
}

//Metodo de la multiplicacion
void IEEEOperations::multiply()
{
    //Metodo que saca mantisa, signo y exponente de A y B
    //binaryTransform();

    union Code a,b;
    a.numero = op1.numero;
    b.numero = op2.numero;

    this->mantisaA=a.bitfield.partFrac | 0x800000;
    this->mantisaB=b.bitfield.partFrac | 0x800000;

    //Casos raros:
    if (operandosOpuestos()) {
        result->numero=0;
        salida.numero = 0;
        //TODO debería sobrar una de las dos líneas
        return;
    } else{

        if (esOp1Denormal()) {
            mantisaA &= 0b011111111111111111111111;
        }
        if (esOp2Denormal()) {
            mantisaB &= 0b011111111111111111111111;
        }
    }

    //Paso 1
    unsigned int p;
    int n = 24;
    cout<<"Paso 1: "<<endl;

    cout<<" Signo A: "<<a.bitfield.sign<<" Exponente A: "<<a.bitfield.expo<<" Fraccionaria A: "<<a.bitfield.partFrac<<endl;
    cout<<" Signo B: "<<b.bitfield.sign<<" Exponente B: "<<b.bitfield.expo<<" Fraccionaria B: "<<b.bitfield.partFrac<<endl;

    result->bitfield.sign = a.bitfield.sign ^ b.bitfield.sign;

    result->bitfield.expo = a.bitfield.expo + b.bitfield.expo;

    // Paso 1: Multiplicación binaria sin signo de las mantisas
    p = multiplyWithoutSign(); //En mantisaA está A y en p, P.


    // Paso 2
    if((p >> (n-1)) %2 == 0)
    {
        bool bit = (mantisaA << (n-1)) & 1;
        mantisaA = mantisaA << 1;

        p = p << 1;

        if(bit) //si el bit es 1, hay que metérselo a P, sino no hace falta ya que por defecto se puso un 0 al desplazar.
        {
            p = p + 1;
        }

    }
    else
    {
        result->bitfield.expo++;
    }

    // Paso 3: bit de redondeo
    bool r = (mantisaA >> (n-1)) & 1;

    // Paso 4: bit sticky
    unsigned int mask = (1u << (n-2));  // Creamos una máscara de bits que tenga 1 en las posiciones 0 a n-2, inclusive
    unsigned int subset = mantisaA & mask;
    bool st = subset!=0;
    cout<<"Valor de mask: "<<mask<<endl<<"Valor de st: "<<st<<endl;

    // Paso 5: redondeo
    if((r==1 && st==1) || (r==1 && st==0 && p%2==1))
    {
        p = p + 1;
    }



    // Comprobación de desbordamientos
    bool overflow = checkOverflow();

    if (overflow) {
        //Convertimos el número en infinito
        this->result->bitfield.expo=255;
        this->result->bitfield.partFrac = 0;
    }
    //Underflow y punto 1 operandos denormales
    if (this->result->bitfield.expo < 1) {
        int t = 1 - result->bitfield.expo;
        if (t >= 24){ //la mantisa tiene 24 bits
            //Underflow --> convertimos el número en 0
            this->result->numero = 0;
        } else {
            // TODO: Teóricamente hay que desplazar P,A t bits a la derecha, pero si luego solo vamos a utilizar P, ¿para qué tener en cuenta a A?
            p = p>>t;
            this->result->bitfield.expo = 0; //Resultado denormal
        }
    }
    // Tratamiento de operandos denormales
    if (esOp1Denormal() || esOp2Denormal()) {
        if (this->result->bitfield.expo > 1) { //TODO el exponente mínimo es 1?
            unsigned int t1 = this->result->bitfield.expo - 1;

            int k = 23;
            unsigned long p2 = p;
            p2 = p2<<24;
            p2 = p2 + mantisaA;
            k -= std::log2(p2);
            unsigned int t2 = k;

            unsigned int t=t1;
            if (t2<t1) {
                t = t2;
            }

            this->result->bitfield.expo = this->result->bitfield.expo -t;
            p2 = p2 >> (24-t); //Esto es lo mismo que desplazar aritméticamente (P,A) t bits a la izda. TODO: Seguro?
            p = p2;
        }
    }
    result->bitfield.partFrac = (p & 0x7FFFFF);

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
