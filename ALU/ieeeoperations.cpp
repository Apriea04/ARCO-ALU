#include "ieeeoperations.h"


// Constructor
IEEEOperations::IEEEOperations(union Code op1, union Code op2)
{
    this->op1 = op1;
    this->op2 = op2;
}

// Destructor
IEEEOperations::~IEEEOperations()
{
}

/**
 * @brief Returns the result of the main operation
 * @return result (union Code)
 */
union Code IEEEOperations::getResult()
{
    return result;
}

/**
 * @brief Checks if main operation is a product
 * @return Main operation is product or not (bool)
 */
bool IEEEOperations::getMulti()
{
    return multi;
}


// Metodo que le pasas un union code y devuelve el binario ieee754
string IEEEOperations::translateIEEE(union Code op)
{
    // Transforma a bits el exponente
    std::bitset<8> exponenteBin(op.bitfield.expo);
    // Transforma a bits la parte fraccionaria
    std::bitset<23> fraccionariaBin(op.bitfield.partFrac);
    // Devuelve el numero en binario IEEE754
    return std::to_string(op.bitfield.sign) + exponenteBin.to_string() + fraccionariaBin.to_string();
}

// Metodo que le pasas un union Code y devuelve el numero en hexadecimal
string IEEEOperations::translateHex(union Code op)
{

    // Transforma a bits el exponente
    std::bitset<8> exponenteBin(op.bitfield.expo);
    // Transforma a bits la parte fraccionaria
    std::bitset<23> fraccionariaBin(op.bitfield.partFrac);
    // Devuelve el numero en binario IEEE754
    string bin = std::to_string(op.bitfield.sign) + exponenteBin.to_string() + fraccionariaBin.to_string();

    // Convertir el string binario a un objeto bitset
    bitset<64> bitSet(bin);

    // Convertir el objeto bitset a un número hexadecimal en un stringstream
    stringstream ss;
    ss << hex << uppercase << bitSet.to_ullong();
    string hexString = ss.str();
    return "0x" + hexString;
}


/**
 * @brief Returns the 2s-complement of a number
 * @param n, number to calculate the 2s-complement (unsigned int)
 * @return 2s-complement of n (unsigned int)
 */
unsigned int IEEEOperations::complementoDos(unsigned int n)
{

    unsigned int tmp = ~n + 1; // Hago el complemento a 2
    if (tmp > 0xFFFFFF)
    {
        tmp &= 0xFFFFFF; // Lo dejo con 24 bits
    }
    return tmp;
}

/**
 * @brief Returns the 1s-complement of a number
 * @param n , number to calculate the 1s-complement (unsigned int)
 * @return 1s-complement of n (unsigned int)
 */
unsigned int IEEEOperations::complementoUno(unsigned int n)
{

    bitset<24> tmp{n};
    tmp = ~tmp;
    unsigned int negated = tmp.to_ullong();
    return negated;
}

/**
 * @brief Checks if numbers a & b are opposite
 * @param a , first number (float)
 * @param b , second number (float)
 * @return a & b are opposite (true) or not (false) (bool)
 */
bool IEEEOperations::operandosOpuestos(float a, float b)
{
    return a == -b;
}

/**
 * @brief Checks if first operand is denormal
 * @return op1 is denormal (true) or not (false) (bool)
 */
bool IEEEOperations::esOp1Denormal()
{
    Code a;
    a.numero = op1.numero;
    return a.bitfield.expo == 0b00000000;
}

/**
 * @brief Checks if second operand is denormal
 * @return op2 is denormal (true) or not (false) (bool)
 */
bool IEEEOperations::esOp2Denormal()
{
    Code b;
    b.numero = op2.numero;
    return b.bitfield.expo == 0b00000000;
}

/**
 * @brief Checks if argument operand is denormal
 * @param num , number to check (Code)
 * @return num is denormal (true) or not (false) (bool)
 */
bool IEEEOperations::esOpDenormal(Code num)
{
    return num.bitfield.expo == 0b00000000;
}

/**
 * @brief Operation selector
 * @param n , string with the operation type selected
 */
void IEEEOperations::operations(string n)
{
    if (n == "suma")
    {
        add();
    }
    else if (n == "multiplicacion")
    {
        multiply();
    }
    else if (n == "division")
    {
        divide();
    }
}

/**
 * @brief MAIN ADD METHOD (Inmersion method)
 */

void IEEEOperations::add()
{
    union Code a, b;
    a.numero = op1.numero;
    b.numero = op2.numero;

    result.numero = addVals(a.numero, b.numero);
}

/**
 * @brief Apply the sum of numbers a & b through params.
 * @param a , first number (float)
 * @param b , second number (float)
 * @return sum of numbers a & b (float)
 */
float IEEEOperations::addVals(float a, float b)
{
    union Code ca, cb, res;
    ca.numero = a;
    cb.numero = b;
    res.numero = 0;

    unsigned int mA = ca.bitfield.partFrac | 0x800000;
    unsigned int mB = cb.bitfield.partFrac | 0x800000;

    /*
    Operandos denormales
    1. Asignar valor 0 a la parte entera de la mantisa en lugar de 1.
    2. Asignar al exponente el valor mínimo representable. (1 en IEEE754)
    3. Operar normalmente.
    */

    if (operandosOpuestos(a, b))
    {

        if (ca.bitfield.expo == 0xFF && ca.bitfield.partFrac == 0)
        {
            res.bitfield.expo = 0xFF;
            res.bitfield.sign = ca.bitfield.sign;
            res.bitfield.partFrac = 0x0;
            // Forzamos un inf
        }
        else
        {
            res.numero = 0;
        }
        return res.numero;
    }
    if (esOpDenormal(ca))
    {
        mA &= 0x7FFFFF;
    }
    if (esOpDenormal(cb))
    {
        mB &= 0x7FFFFF;
    }
    if (ca.bitfield.expo == 0xFF && ca.bitfield.partFrac == 0)
    {
        res.bitfield.expo = 0xFF;
        res.bitfield.sign = ca.bitfield.sign;
        res.bitfield.partFrac = 0x0; // Si op1 es inf, el resultado tb
        return res.numero;
    }
    else if (cb.bitfield.expo == 0xFF && cb.bitfield.partFrac == 0)
    {
        res.bitfield.expo = 0xFF;
        res.bitfield.sign = cb.bitfield.sign;
        res.bitfield.partFrac = 0x0; // Si op1 es inf, el resultado tb
        return res.numero;
    }

    int exponente;

    // Paso 1
    //Seteamos a 0 g, r, y el sticky
    //Seteamos a false los booleanos operandos_intercambiados y complementado P

    int g = 0;
    int r = 0;
    int st = 0;
    bool c = false; // Acarreo
    bool operandos_intercambiados = false;
    bool complementado_P = false;
    unsigned int mask;

    // Paso 2
    /*
    Si (ea<eb) entonces
    1. Intercambiar los operandos (A⇔B);
    2. Operandos_intercambiados=cierto;
    */
    if (ca.bitfield.expo < cb.bitfield.expo)
    {
        // Intercambiamos los operandos
        Code tmp = ca;
        ca = cb;
        cb = tmp;
        mask = mA;
        mA = mB;
        mB = mask;
        operandos_intercambiados = true;
    }

    // Paso 3
    /*
    ExponenteSuma = ExponenteA
    d = ExponenteA – ExponenteB
    */

    exponente = ca.bitfield.expo;
    int d = ca.bitfield.expo - cb.bitfield.expo;

    // Paso 4
    /*
    Si (SignoA ≠ SignoB)
    MantisaB = Complemento_2 (MantisaB)
    */

    if (ca.bitfield.sign != cb.bitfield.sign)
    {
        mB = complementoDos(mB);
    }

    //Paso 5
    //P = MantisaB

    unsigned int p = mB;

    // Paso 6
    /*
    Asignar
    Bit de guarda: g = P sub d-1
    Bit de redondeo: r = P sub d-2
    Bit sticky: st = OR (P sub d-3, P sub d-4, … , P sub 0)
    ¡¡Siempre que esos bits existan!!
    */

    if (d >= 1)
    {
        g = (p >> (d - 1)) & 1;
    }
    if (d >= 2)
    {
        r = (p >> (d - 2)) & 1;
    }

    if (d >= 3)
    {

        unsigned int tmp = p;

        for (int i = 0; i < d - 3; i++)
        {
            st |= (tmp & 0b1); // Hacemos un or con el último bit de tmp
            tmp = tmp >> 1;    // Desplazamos tmp uno a la derecha.
        }
    }
    // Paso 7
    /*
    Si (SignoA ≠ SignoB)
        Desplazar P a la derecha d bits introduciendo 1´s por
        la izquierda.
    Si no
        Desplazar P a la derecha d bits introduciendo 0´s por
        la izquierda.
    */

    if (cb.bitfield.sign != ca.bitfield.sign)
    {
        p = complementoUno(p);
        p = p >> d;
        p = complementoUno(p);
    }
    else
    {
        p = p >> d;
    }

    // Paso 8
    /*
    P = MantisaA + P ; C1 = acarreo de la suma entera
    */

    p = p + mA;

    // ¿Se ha producido desbordamiento? (un acarreo al final)
    if (p > 0xFFFFFF)
    {                     // Si p >= 2^24, es que ocupa 25 bits y el primero es un uno, esdecir, hubo desbordamiento y acarreo
        p = p & 0xFFFFFF; // Me cargo ese uno que se añadió al producirse desbordamiento
        c = 1;            // c=1
    }
    else
    {
        c = 0; // c = 0
    }

    // Paso 9
    /*
    Si (SignoA ≠ SignoB) y (Pn – 1 = 1) y (C1 = 0) entonces (Solamente es posible si d = 0)
    P = Complemento_2 (P)
    Complementada_P = cierto
    */
    if (ca.bitfield.sign != cb.bitfield.sign && ((p & 0x800000) == 0x800000) && (c == 0))
    {
        p = complementoDos(p);
        complementado_P = true;
    }

    // Paso 10
    /*
    Si (SignoA = SignoB) y (C1 = 1) entonces
        St = OR(g, r, st)
        r = P sub 0
        Desplazar 1 bit a la derecha (C sub 1, P)
        Ajustamos el exponente de la suma ExponenteSuma = ExponenteSuma + 1;
    Si no
        Calcula K bits que es necesario desplazar P para que sea una mantisa normalizada.
        Si (k=0) entonces st = OR(r,st), r=g;
        Si (k>1) entonces r=0, st=0;
        Desplazar (P,g) a la izquierda k bits y ajustar el exponente de la suma: ExponenteSuma = ExponenteSuma - K
    */

    if (ca.bitfield.sign == cb.bitfield.sign && c == 1)
    {
        st = g | r | st;
        r = p & 0x1;
        p = p >> 1;
        if (c)
        {
            p = p | 0x800000; // Poner el uno de c
        }
        else
        {
            p = p & mask;
        }
        // Ajusto el exponente de la suma:
        exponente += 1;
    }
    else
    {
        // Calculo cuántos bits tengo que desplazar P para que sea una mantisa normalizadop1.
        //  Para responder a eso, me basta con saber donde está el primer uno empezando por la izquierdop1.
        //  Eso se puede hacer con log2:
        bool encontrado = false;
        int k = -1;
        unsigned int tmp = 0x800000;
        while (!encontrado && k < 24)
        {
            encontrado = p & tmp;
            tmp = tmp >> 1;
            k++;
        }

        if (k == 0)
        {
            st = r | st;
            r = g;
        }
        else if (k > 1)
        {
            r = 0;
            st = 0;
        }

        // Desplazar (P,g) a la izquierda k bits:
        if (k != 0)
        {               // Si k es 0, no hya nada que desplazar
            p = p << 1; // Desplazo P a la izquierda dejando un 0 al final
            p = p | g;  // Con el 0 hago un or para poner el valor de g
            // Ya se ha desplazado p,g una unidad. Se hacen los desplazamientos restantes:
            p = p << (k - 1);
        }
        exponente -= k;
    }

    // Paso 11
    /*
    Redondear P
    Si (r = 1 y st = 1) o (r = 1 y st = 0 y P0 = 1) entonces
        P = P + 1; C2 = acarreo
        Si (C2 = 1) desplazar 1 bit a la derecha (C2, P) y ajustar el exponente de la suma:
            ExponenteSuma = ExponenteSuma + 1
        MantisaSuma = P
    */



    unsigned int p0 = p & 0x1;
    if ((r == 1 && st == 1) || (r == 1 && st == 0 && p0 == 1))
    {
        p += 1;
        if (p > 0xFFFFFF)
        {
            c = true;
            p = p & 0xFFFFFF;
        }
        else
        {
            c = false;
        }
        if (c)
        {                     // Si hay acarreo, c=1
            p = p >> 1;       // Desplazo 1 bit a la derecha p
            p = p | 0x800000; // Añado el uno del carry al principio
            exponente += 1;   // Ajustamos el exponente
        }
    }
    res.bitfield.partFrac = p & 0x7FFFFF;

    // Paso 12
    /*
     Calcular el signo del resultado
    Si (operandos_intercambiados = falso) y (Complemento_P = cierto) entonces
        SignoSuma = SignoB
    Si no
        SignoSuma = SignoA
    */

    if ((operandos_intercambiados == false) && (complementado_P == true))
    {
        res.bitfield.sign = cb.bitfield.sign;
    }
    else
    {
        res.bitfield.sign = ca.bitfield.sign;
    }

    // Añadido por consejo de David:
    // tratamiento del exponente tanto en los operandos denormales como en el resultado final.

    if (exponente <= 0)
    {
        exponente = 1;
    }
    res.bitfield.expo = static_cast<unsigned int>(exponente) & 0xFF;

    // Paso 13
    /*Devolvemos el resultado*/
    return res.numero;
}

/**
 * @brief Calculate the product without sign of 2 mantissas
 * @param MA , pointer with mantissa A (bitset<24>*)
 * @param MB , mantissa B (bitset<24>)
 * @return product of mantissa A and mantissa B (unsigned int)
 */
unsigned int IEEEOperations::multiplyWithoutSign(bitset<24> *MA, bitset<24> MB)
{


    // Paso 1
    /*
    Almacenar valores en los registros A, B y P
    A = a sub n-1, a sub n-2, ..., a0
    B = b sub n-1, b sub n-2, ..., b0
    P = 0

    */

    bitset<24> P{0};
    int n = 24;
    bool c = false; // Acarreo

    unsigned long long val = 0;

    for (int i = 0; i < n; i++)
    {
        // Paso 2.1
        //Si (a sub 0=1) entonces P=P+B; c=acarreo
        //Si no P=P+0;
        if (MA->test(0))
        {
            val = addVals(P.to_ullong(), MB.to_ullong());

            P = bitset<24>{val};
        }
        else
        {
            val = 0;
        }

        // ¿Se ha producido desbordamiento? (un acarreo al final)
        if (val >= 0xFFFFFF)
        { // Si p >= 2^24, es que ocupa 25 bits y el primero es un uno, esdecir, hubo desbordamiento y acarreo
            // Hubo un acarreo al final de la suma ya que este AND ya que no devuelve 0
            P = P.to_ullong() & 0b111111111111111111111111; // Me cargo ese uno que se añadió al producirse desbordamiento
            c = true;                                       // c=1
        }
        else
        {
            c = false;
        }

        // Paso 2.2
        // Desplazar 1 bit a la derecha (c,P,A)

        *MA = bitset<24>{MA->to_ullong() >> 1};

        if (P.test(0))
        {
            MA->set(n - 1);
        }
        else
        {
            MA->reset(n - 1);
        }

        P = P >> 1;

        if (c)
        {
            P.set(n - 1);
        }
        else
        {
            P.reset(n - 1);
        }
    }

    // Paso 3: Devolver
    /* El producto aparece como un valor de 2n bits en el conjunto de registros (P,A) */
    return P.to_ullong();
}

/**
 * @brief Checks if a number is overflowed
 * @param exponent , int
 * @return overflow (true) or not (false) (bool)
 */
bool IEEEOperations::checkOverflow(int exponent)
{
    return exponent > 254;
}

/**
 * @brief Checks if a number is underflowed
 * @param exponent , int
 * @return underflow (true) or not (false) (bool)
 */
bool IEEEOperations::checkUnderflow(int exponent)
{

    return exponent < 1;
}

/**
 * @brief MAIN PRODUCT METHOD (Inmersion method)
 */

void IEEEOperations::multiply()
{
    result.numero = 0;

    union Code a, b;
    a.numero = op1.numero;
    b.numero = op2.numero;
    bool denormal;

    result.numero = multiplyVals(a.numero, b.numero, &denormal);

    multi = denormal;
}

/**
 * @brief Calculate the product of 2 values
 * @param a , first number (float)
 * @param b , second number (float)
 * @return P, product a*b (float)
 */

float IEEEOperations::multiplyVals(float a, float b, bool *resultDenormal)
{
    //Forzamos si un valor es 0, devuelvo 0, o si hay 0 * inf, devuelvo NAN
    if(a == 0 && b == INFINITY){
        return NAN;
    }
    else if(a == INFINITY && b == 0){
        return NAN;
    }
    else if(a == 0 || b == 0){
        return 0;
    }

    *resultDenormal = false;

    union Code ca, cb, res;
    ca.numero = a;
    cb.numero = b;
    res.numero = 0;
    int n = 24;

    bitset<24> mA{ca.bitfield.partFrac};
    bitset<24> mB{cb.bitfield.partFrac};

    mA.set(n - 1);
    mB.set(n - 1);

    // Paso 2
    // Exponente del producto = e sub a + e sub b
    int exponent = (ca.bitfield.expo - 127) + (cb.bitfield.expo - 127);

    // Paso 3
    //Cálculo de la mantisa del producto, mp:

    // 3.1 Mantisa del producto
    //(P,A)=m sub a * m sub b; Se utiliza el algoritmo del producto de enteros sin signo.

    bitset<24> P{multiplyWithoutSign(&mA, mB)};

    /*
    3.2

    Si (P sub n-1 = 0) entonces desplazar (P,A) un bit a la izquierda.
    Si_no sumar 1 al exponente del producto
    */
    if (!P.test(n - 1))
    {
        P = bitset<24>{P << 1};
        if (mA.test(n - 1))
        {
            P.set(0);
        }
        else
        {
            P.reset(0);
        }

        mA = bitset<24>{mA << 1};
    }
    else
    {
        exponent++;
    }

    // 3.3 Bit redondeo: r = a sub n-1
    bool r = mA.test(n - 1);

    // 3.4 Bit sticky
    //st=OR(a sub n-2,a sub n-3,..a sub 0); (Siendo n el número de bits de la mantisa).


    bool st = false;

    for (int i = 0; i < n; i++)
    {
        if (mA.test(i))
        {
            st = true;
            break;
        }
    }

    // 3.5 Redondeo
    /*
    Si (r=1 y st=1) O (r=1 y st=0 y P sub 0=1) entonces P=P+1;
    Comprobación de desbordamientos.
    Tratamiento específico cuando hay operandos denormales.
    m sub p = P
    */
    if ((r == true && st == true) || (r == true && st == false && P.test(0)))
    {
        unsigned long long value = addVals(P.to_ullong(), 1);
        P = bitset<24>{value};
    }

    exponent += 127;

    // Paso 1
    // Signo del producto
    res.bitfield.sign = ca.bitfield.sign ^ cb.bitfield.sign;

    // Comprobación desbordamientos

    /*
    Hay desbordamiento a infinito (overflow) cuando el exponente del
    producto es mayor que el máximo representable.
    */
    if (checkOverflow(exponent))
    {
        res.bitfield.partFrac = 0;
        res.bitfield.expo = 255;
    }

    /*
    Tratamiento de desbordamiento a cero (underflow):

    Si (exponente_producto<exponente_mínimo) entonces:
        t=exponente_mínimo-exponente_producto;

        Si (t ≥ numero_bits_mantisa) entonces
            1. Hay underflow (porque se desplaza toda la mantisa).

        Si_no
            i. Desplazar aritméticamente (P,A) t bits a la derecha.
            Nota: El resultado será un valor denormal.
            2. exponente_producto=exponente_mínimo

    */
    else if (checkUnderflow(exponent))
    {

        int t = 1 - exponent;

        if (t >= n)
        {
            res.bitfield.expo = 0;
            *resultDenormal = true; // Porque no podemos representar el resultado.
        }
        else
        {
            // 1. Desplazar aritméticamente (P,A) t bits a la derecha
            for (int i = 0; i < t; i++)
            {
                mA = bitset<24>{mA >> 1};

                if (P.test(0))
                {
                    mA.set(n - 1);
                }
                else
                {
                    mA.reset(n - 1);
                }

                if (P.test(n - 1))
                {
                    P = bitset<24>{P >> 1};
                    P.set(n - 1);
                }
                else
                {
                    P = bitset<24>{P >> 1};
                    P.reset(n - 1);
                }
            }

            // 2. Exponente producto = Exponente mínimo
            *resultDenormal = true;
            res.bitfield.expo = 1;
        }
    }
    else
    {
        /*
        Cuando alguno de los operandos es denormal, hay varios casos posibles:

        1. exponente_producto<exponente_mínimo: (resultado denormal)
            Igual tratamiento que en el caso de underflow

        2. exponente_producto>exponente_mínimo:
            - t sub 1=exponente_producto-exponente_mínimo;
            - t sub 2=número de bits que es necesario desplazar (P,A) hacia la
              izquierda para que la mantisa quede normalizada.
            - t=min(t1,t2);
            - exponente_producto=exponente_producto-t;
            - Desplazar aritméticamente (P,A) t bits a la izquierda.

        3. exponente_producto=exponente_mínimo:
            - El resultado es directamente un denormal.


        Operandos cero: 0*n = 0
        0 * +-inf = indeterminado (NaN)

        */

        // Tratamiento específico con operandos denormales
        if (esOp1Denormal() || esOp2Denormal())
        {

            // Caso 1: Exponente producto < Exponente mínimo

            if (exponent < 1)
            {
                int t = addVals(1, -exponent);

                if (t >= n)
                {
                    res.bitfield.partFrac = 1;
                    res.bitfield.expo = 255;
                }
                else
                {
                    // 1. Desplazar aritméticamente (P,A) t bits a la derecha
                    for (int i = 0; i < t; i++)
                    {
                        mA = bitset<24>{mA >> 1};

                        if (P.test(0))
                        {
                            mA.set(n - 1);
                        }
                        else
                        {
                            mA.reset(n - 1);
                        }

                        if (P.test(n - 1))
                        {
                            P = bitset<24>{P >> 1};
                            P.set(n - 1);
                        }
                        else
                        {
                            P = bitset<24>{P >> 1};
                            P.reset(n - 1);
                        }
                    }

                    // 2. Exponente producto = Exponente mínimo
                    *resultDenormal = true;
                    res.bitfield.expo = 1;
                }
            }

            // Caso 2: Exponente producto > Exponente mínimo

            else if (exponent > 1)
            {
                int t1 = addVals(exponent, -1);
                int t2 = 0;

                while (mA.test(n - t2 - 1) == false)
                {
                    t2++;
                }

                int t = min(t1, t2);

                res.bitfield.expo = addVals(exponent, -t);

                // Desplazar aritméticamente (P,A) t bits a la izquierda
                for (int i = 0; i < t; i++)
                {
                    P = bitset<24>{P << 1};

                    if (mA.test(n - 1))
                    {
                        P.set(0);
                    }
                    else
                    {
                        P.reset(0);
                    }

                    if (mA.test(0))
                    {
                        mA = bitset<24>{mA << 1};
                        mA.set(0);
                    }
                    else
                    {
                        mA = bitset<24>{mA << 1};
                        P.reset(0);
                    }
                }
            }

            // Caso 3: Exponente producto = Exponente mínimo

            else
            {
                *resultDenormal = true;
            }
        }

        res.bitfield.expo = exponent;
        // Mantisa final
        res.bitfield.partFrac = P.to_ullong();
    }

    return res.numero;
}

/**
 * @brief MAIN DIVISION METHOD
 */
void IEEEOperations::divide()
{
    //Método de Goldschmidt
    result.numero = 0;

    union Code a, b;
    a.numero = op1.numero;
    b.numero = op2.numero;
    int n = 24;
    bool prodDenormal;

    //Creamos las mantisas de a y b
    bitset<24> mA{a.bitfield.partFrac};
    bitset<24> mB{b.bitfield.partFrac};


    mA.set(n - 1);
    mB.set(n - 1);

    //Si el divisor es 0, forzamos el NaN
    if (b.bitfield.expo < 1)
    {
        // Not a Number
        result.bitfield.sign = 0;
        result.bitfield.expo = 255;
        result.bitfield.partFrac = 1;
    }
    //Si el numero de a es 0...
    else if (a.numero == 0)
    {
    }
    //Si el exponente de a es > a 254, forzamos el infinito.
    else if (a.bitfield.expo > 254)
    {
        // Infinite
        result.bitfield.sign = 0;
        result.bitfield.expo = 255;
        result.bitfield.partFrac = 0;
    }

    //Si el exponente de b es > a 254, forzamos el NaN
    else if (b.bitfield.expo > 254)
    {
        // Not a Number
        result.bitfield.sign = 0;
        result.bitfield.expo = 255;
        result.bitfield.partFrac = 1;
    }
    //Si no...
    else
    {

        // Paso 1
        // Escalar A y B de tal forma que b pertenezca a [1,2)

        /*
        Para escalar un número tenemos que coger los 24 bits de su
        mantisa de tal forma que la parte entera del número sea el bit
        que no se representa en IEEE754 y el resto de bits serán la parte
        fraccionaria. Una vez separadas la parte entera de la
        fraccionaria hay que pasar el número a decimal. De esta forma
        obligamos a que el número escalado pertenezca al intervalo
        [1,2).
        */

        float numA = 0;
        float numB = 0;

        for (int i = 0; i < n; i++)
        {
            if (mA.test(n - i - 1))
            {
                numA = addVals(numA, pow(2, -i));
            }

            if (mB.test(n - i - 1))
            {
                numB = addVals(numB, pow(2, -i));
            }
        }

        // Paso 2
        // Buscar una solución aproximada b'=1/b en la tabla

        /*
            b       b' = 1/b
        [1, 1.25)    1.00
        [1.25, 2)    0.80
        Calculo del signo de la división
        SignoDivision = SignoA XOR SignoB

        Cálculo del exponente de la División:
        ExpDivision = ExA – ExB + Ex(A*1/B)

        */
        float b_prime = 0;

        if (numB >= 1 && numB < 1.25)
        {
            b_prime = 1.00;
        }

        if (numB >= 1.25 && numB < 2.00)
        {
            b_prime = 0.80;
        }

        // Paso 3
        // Asignar x0 = a*b' ; y0 = b*b'

        float x0 = multiplyVals(numA, b_prime, &prodDenormal);
        float y0 = multiplyVals(numB, b_prime, &prodDenormal);

        // Paso 4
        // Iterar para obtener una aproximación
        //r=2-y sub i; y sub i+1 = y sub i * r;
        //x sub i+1 = x sub i * r;

        //Cuando el valor del cociente x sub i se obtiene con
        //precisión suficiente, se detiene el proceso.
        //X1 - X0 < 10-4, condición de finalización.

        float r = 0, x = x0, y = y0, x_old = 0;

        float threshold = pow(10, -4);
        float difference = 100;

        while (difference >= threshold)
        {
            r = addVals(2, -y);
            y = multiplyVals(y, r, &prodDenormal);
            x_old = x;
            x = multiplyVals(x, r, &prodDenormal);

            difference = abs(addVals(x, -x_old));
        }

        union Code last_x;
        last_x.numero = x;

        // Paso 6
        // Exponente de la división
        //ExpDiv = ExA – ExB + Ex (A * 1/B)

        float exponent = addVals(static_cast<float>(a.bitfield.expo), -static_cast<float>(b.bitfield.expo));
        exponent = addVals(exponent, last_x.bitfield.expo);

        // Paso 5
        // Signo de la división
        //SignoDivision = SignoA XOR Signo B = 0 XOR 0 = 0
        result.bitfield.sign = a.bitfield.sign ^ b.bitfield.sign;

        if (exponent > 254)
        {
            result.bitfield.expo = 255;
            result.bitfield.partFrac = 0;
        }
        else
        {
            result.bitfield.expo = exponent;

            // Paso 7
            // Parte fraccionaria de la división
            //Obtenemos el resultado final
            //Pasamos el numero a decimal
            result.bitfield.partFrac = last_x.bitfield.partFrac;
        }
    }
}
