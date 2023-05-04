#include "ieeeoperations.h"

// Constructor
IEEEOperations::IEEEOperations(union Code op1, union Code op2)
{
    this->op1 = op1;
    this->op2 = op2;
    // this->result = result;
}

// Destructor
IEEEOperations::~IEEEOperations()
{
}

// Metodo que devuelve el resultado de una operacion
union Code IEEEOperations::getResult()
{
    return result;
}

//Metodo que devuelve el booleano de la multiplicacion
bool IEEEOperations::getMulti()
{
    return multi;
}

// Metodoo para debug
// Metodo que le pasas un int y devuelve el binario ieee754
string IEEEOperations::intBinario(unsigned int op)
{
    union Code x;
    x.numero = op;
    // Transforma a bits el exponente
    std::bitset<8> exponenteBin(x.bitfield.expo);
    // Transforma a bits la parte fraccionaria
    std::bitset<23> fraccionariaBin(x.bitfield.partFrac);
    // Devuelve el numero en binario IEEE754
    return std::to_string(x.bitfield.sign) + exponenteBin.to_string() + fraccionariaBin.to_string();
}

// Metodoo para debug
// Metodo que le pasas un union Code y devuelve el numero en hexadecimal
string IEEEOperations::intHex(unsigned int op)
{

    union Code x;
    x.numero = op;

    // Transforma a bits el exponente
    std::bitset<8> exponenteBin(x.bitfield.expo);
    // Transforma a bits la parte fraccionaria
    std::bitset<23> fraccionariaBin(x.bitfield.partFrac);
    // Devuelve el numero en binario IEEE754
    string bin = std::to_string(x.bitfield.sign) + exponenteBin.to_string() + fraccionariaBin.to_string();

    // Convertir el string binario a un objeto bitset
    bitset<64> bitSet(bin);

    // Convertir el objeto bitset a un número hexadecimal en un stringstream
    stringstream ss;
    ss << hex << uppercase << bitSet.to_ullong();
    string hexString = ss.str();
    return hexString;
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
    return hexString;
}

unsigned int IEEEOperations::complementoDos(unsigned int n) {

    unsigned int tmp = ~n +1; //Hago el complemento a 2
    if (tmp > 0xFFFFFF) {
        tmp &= 0xFFFFFF; //Lo dejo con 24 bits
    }
    return tmp;
}

// Metodo para calcular el complemento a 1
unsigned int IEEEOperations::complementoUno(unsigned int n)
{

    bitset<24> tmp{n};
    tmp = ~tmp;
    unsigned int negated = tmp.to_ullong();
    return negated;
}

// Metodo que comprueba si dos operandos son opuestos
bool IEEEOperations::operandosOpuestos(float a, float b)
{
    return a == -b;
}

// Metodo que comprueba si op1 es denormal
bool IEEEOperations::esOp1Denormal()
{
    Code a;
    a.numero = op1.numero;
    return a.bitfield.expo == 0b00000000;
}

// Metodo que comprueba si op2 es denormal
bool IEEEOperations::esOp2Denormal()
{
    Code b;
    b.numero = op2.numero;
    return b.bitfield.expo == 0b00000000;
}

bool IEEEOperations::esOpDenormal(Code num)
{
    return num.bitfield.expo == 0b00000000;
}

//Metodo de la suma

void IEEEOperations::operations(string n){
    if(n == "suma"){
        add();
    }
    else if(n == "multiplicacion"){
        multiply();
    }
    else if(n == "division"){
        divide();
    }

}

void IEEEOperations::add()
{
    union Code a,b;
    a.numero = op1.numero;
    b.numero = op2.numero;

    result.numero = addVals(a.numero, b.numero);

}

float IEEEOperations::addVals(float a, float b)
{
    union Code ca,cb, res;
    ca.numero = a;
    cb.numero = b;
    res.numero = 0;

    unsigned int mA = ca.bitfield.partFrac | 0x800000;
    unsigned int mB = cb.bitfield.partFrac | 0x800000;

    //Casos raros:
    //TODO reformatear esto
    if (operandosOpuestos(a, b)) {
        if (ca.bitfield.expo == 0xFF && ca.bitfield.partFrac == 0) {
            res.bitfield.expo = 0xFF;
            res.bitfield.sign = ca.bitfield.sign;
            res.bitfield.partFrac = 0x0;
            //Forzamos un inf
        } else {
            res.numero=0;
        }
        return res.numero;
    } if (esOpDenormal(ca)) {
        mA &= 0x7FFFFF;
    }
    if (esOpDenormal(cb)) {
        mB &= 0x7FFFFF;
    } if (ca.bitfield.expo == 0xFF && ca.bitfield.partFrac == 0) {
        res.bitfield.expo = 0xFF;
        res.bitfield.sign = ca.bitfield.sign;
        res.bitfield.partFrac = 0x0; //Si op1 es inf, el resultado tb
        return res.numero;
    } else if (cb.bitfield.expo == 0xFF && cb.bitfield.partFrac == 0) {
        res.bitfield.expo = 0xFF;
        res.bitfield.sign = cb.bitfield.sign;
        res.bitfield.partFrac = 0x0; //Si op1 es inf, el resultado tb
        return res.numero;
    }

    int exponente;

    // Paso 1
    //cout << "Paso 1: " << endl;

    //cout << " Signo A: " << ca.bitfield.sign << " Exponente A: " << ca.bitfield.expo << " Fraccionaria A: " << ca.bitfield.partFrac << endl;
    //cout << " Signo B: " << cb.bitfield.sign << " Exponente B: " << cb.bitfield.expo << " Fraccionaria B: " << cb.bitfield.partFrac << endl;

    int g = 0;
    int r = 0;
    int st = 0;
    bool c = false; //Acarreo
    bool operandos_intercambiados = false;
    bool complementado_P = false;
    unsigned int mask;

    // Paso 2
    //cout << "Paso 2: Si a < b intercambiamos" << endl;
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
    //cout << "Paso 3: " << endl;

    exponente = ca.bitfield.expo;
    int d = ca.bitfield.expo - cb.bitfield.expo;

    //cout << "Exponente suma: " << exponente << " d: " << d << endl;

    // Paso 4
    //cout << "Paso 4: " << endl;

    if (ca.bitfield.sign != cb.bitfield.sign)
    {
        mB = complementoDos(mB);
        //cout << "Los signos no coinciden. Mantisa B: " << mantisaB << endl;
    }

    unsigned int p = mB;
    //cout<<"Valor de p: "<<p<<endl;

    //Paso 6
    //cout<<"Paso 6: "<<endl;
    if (d>=1) {
        g = (p >> (d-1)) & 1;
        //cout<<"Valor de g cuando d >= 1: "<<g<<endl;
    }
    if (d >= 2)
    {
        r = (p >> (d - 2)) & 1;
        //cout << "Valor de r cuando d >= 2: " << r << endl;
    }

    if (d >= 3)
    {

        unsigned int tmp = p;

        for (int i = 0; i<d-3; i++) {
            st |= (tmp & 0b1); //Hacemos un or con el último bit de tmp
            tmp = tmp >> 1; //Desplazamos tmp uno a la derecha.
        }

        //cout<<"Valor de mask cuando d >= 3: "<<mask<<endl<<"Valor de st: "<<st<<endl;
    }
    // Paso 7
    //cout << "Paso 7: " << endl;
    if (cb.bitfield.sign != ca.bitfield.sign)
    {
        p = complementoUno(p);
        p = p >> d;
        p = complementoUno(p);
        //cout << "Valor de mask cuando signos de a y b no coinciden: " << mask << endl
        //     << "Valor de p: " << p << endl;
    }
    else
    {
        p = p >> d;
        //cout << "Si los valores coiniden: valor de p " << p << endl;
    }

    // Paso 8
    //cout << "Paso 8: " << endl;
    p = p + mA;

    //cout << "Valor de p: " << p << endl;

    //¿Se ha producido desbordamiento? (un acarreo al final)
    if (p>0xFFFFFF) { //Si p >= 2^24, es que ocupa 25 bits y el primero es un uno, esdecir, hubo desbordamiento y acarreo
        p = p & 0xFFFFFF; //Me cargo ese uno que se añadió al producirse desbordamiento
        c = 1; //c=1

        //cout<<"Hubo acarreo al final de la suma. Valor de p: "<<p<<" Valor de c: "<<c<<"Valor de mask: "<<mask<<endl;
    } else {
        c = 0; //c = 0
    }

    //Paso 9
    //cout<<"Paso 9: "<<endl;
    if (ca.bitfield.sign != cb.bitfield.sign && ((p & 0x800000) == 0x800000) && (c==0)) {
        p = complementoDos(p);
        complementado_P = true;
    }

    //Paso 10
    //cout<<"Paso 10: "<<endl;
    if (ca.bitfield.sign == cb.bitfield.sign && c==1) {
        st = g|r|st;
        r = p & 0x1;
        p = p >> 1;
        if (c) {
            p = p | 0x800000; //Poner el uno de c
        }
        else
        {
            p = p & mask;
        }
        // Ajusto el exponente de la suma:
        exponente += 1;
    } else {
        //Calculo cuántos bits tengo que desplazar P para que sea una mantisa normalizadop1.
        // Para responder a eso, me basta con saber donde está el primer uno empezando por la izquierdop1.
        // Eso se puede hacer con log2:
        bool encontrado = false;
        int k = -1;
        unsigned int tmp = 0x800000;
        while (!encontrado && k <24) {
            encontrado = p & tmp;
            tmp = tmp >> 1;
            k ++;
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

        //Desplazar (P,g) a la izquierda k bits:
        if (k != 0) { //Si k es 0, no hya nada que desplazar
            p = p << 1; //Desplazo P a la izquierda dejando un 0 al final
            p = p | g; //Con el 0 hago un or para poner el valor de g
            //Ya se ha desplazado p,g una unidad. Se hacen los desplazamientos restantes:
            p = p << (k-1);
        }
        exponente -= k;
    }

    //Paso 11
    //cout<<"Paso 11: "<<endl;
    unsigned int p0 = p & 0x1;
    if ((r==1 && st==1) || (r==1 && st==0 && p0 == 1)) {
        p+=1;
        if (p > 0xFFFFFF){
            c = true;
            p = p & 0xFFFFFF;
        } else {
            c = false;
        }
        if (c) { //Si hay acarreo, c=1
            p = p >> 1; //Desplazo 1 bit a la derecha p
            p = p | 0x800000; //Añado el uno del carry al principio
            exponente += 1; //Ajustamos el exponente
        }
    }
    res.bitfield.partFrac = p & 0x7FFFFF;

    // Paso 12
    //cout << "Paso 12: " << endl;
    if ((operandos_intercambiados == false) && (complementado_P == true))
    {
        res.bitfield.sign = cb.bitfield.sign;
    }
    else
    {
        res.bitfield.sign = ca.bitfield.sign;
    }

    //Añadido por consejo de David:
    //tratamiento del exponente tanto en los operandos denormales como en el resultado final.

    if (exponente<=0) {
        exponente = 1;
    }
    res.bitfield.expo = static_cast<unsigned int>(exponente) & 0xFF;

    //Paso 13
    //cout<<"Paso 13: "<<endl;

    //Test
    //cout<<" Signo A: "<<ca.bitfield.sign<<" Exponente A: "<<ca.bitfield.expo<<" Fraccionaria A: "<<ca.bitfield.partFrac<<endl;
    //cout<<" Signo B: "<<cb.bitfield.sign<<" Exponente B: "<<cb.bitfield.expo<<" Fraccionaria B: "<<cb.bitfield.partFrac<<endl;
    //cout<<" Signo Result: "<<res.bitfield.sign<<" Exponente Result: "<<res.bitfield.expo<<" Fraccionaria Result: "<<res.bitfield.partFrac<<endl;
    return res.numero;
}

unsigned int IEEEOperations::multiplyWithoutSign(bitset<24> *MA, bitset<24> MB)
{

    // Paso 1
    //  Almacenar valores en los registros A, B y P
    bitset<24> P{0};
    int n = 24;
    bool c = false; // Acarreo

    //cout << MA->to_string() << endl;
    //cout << MB.to_string() << endl;

    unsigned long long val = 0;

    for (int i = 0; i < n; i++)
    {
        // Paso 1.1
        if (MA->test(0))
        {

            //val = P.to_ullong() + MB.to_ullong();
            val = addVals(P.to_ullong(), MB.to_ullong());
            //cout << "V: " << val << " P: " << P.to_ullong() << " M: " << MB.to_ullong() << endl;

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
            //cout << "Hubo acarreo al final de la suma. Valor de p: " << P.to_string() << " Valor de c: " << c << endl;
        }
        else
        {
            c = false;
        }

        //cout << "p: "<< P.to_string() << " a: " << MA->to_string() << endl;

        // Paso 1.2
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
        //cout << "P: " << P.to_string() << " A: " << MA->to_string() << endl;
    }

    // Paso 3: Devolver
    return P.to_ullong();
}

bool IEEEOperations::checkOverflow(int exponent)
{
    return exponent > 254;
}

bool IEEEOperations::checkUnderflow(int exponent)
{

    return exponent < 1;
}

// Metodo de la multiplicacion

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
 * @brief Inmersion method to multiply 2 values
 * @param a (float)
 * @param b (float)
 * @return P, product a*b (float)
 */

float IEEEOperations::multiplyVals(float a, float b, bool *resultDenormal)
{

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
    // Exponente del producto
    int exponent = (ca.bitfield.expo - 127) + (cb.bitfield.expo - 127);

    // Paso 3
    // 3.1 Mantisa del producto
    bitset<24> P{multiplyWithoutSign(&mA, mB)};

    // 3.2 Desplazar (P,A) 1 bit a la izquierda o sumar 1 al exponente
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

    // 3.3 Bit redondeo
    bool r = mA.test(n - 1);

    // 3.4 Bit sticky
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
    if ((r == true && st == true) || (r == true && st == false && P.test(0)))
    {
        unsigned long long value = addVals(P.to_ullong(), 1);
        P = bitset<24>{value};
        //cout << "P: " << P.to_string() << " A: " << mA.to_string() << endl;
    }

    exponent += 127;

    // Paso 1
    // Signo del producto
    res.bitfield.sign = ca.bitfield.sign ^ cb.bitfield.sign;

    // Comprobación desbordamientos
    if (checkOverflow(exponent))
    {
        res.bitfield.partFrac = 0;
        res.bitfield.expo = 255;
    }

    else if (checkUnderflow(exponent))
    {

        int t = 1 - exponent;

        if (t >= n)
        {
            res.bitfield.expo = 0;
            *resultDenormal = true; //Porque no podemos representar el resultado.
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

                res.bitfield.expo = addVals(exponent, - t);

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

// Metodo de la division
void IEEEOperations::divide()
{

    // Test
    cout << " Signo A: " << signoA << " Exponente A: " << exponenteA << " Mantisa A: " << mantisaA << endl;
    cout << " Signo B: " << signoB << " Exponente B: " << exponenteB << " Mantisa B: " << mantisaB << endl;

    result.numero = 0;

    union Code a, b;
    a.numero = op1.numero;
    b.numero = op2.numero;
    int n = 24;
    bool prodDenormal;

    bitset<24> mA{a.bitfield.partFrac};
    bitset<24> mB{b.bitfield.partFrac};

    mA.set(n - 1);
    mB.set(n - 1);

    if(b.numero == 0)
    {
        // Not a Number
        result.bitfield.sign = 0;
        result.bitfield.expo = 255;
        result.bitfield.partFrac = 1;
    }
    else if(a.numero == 0)
    {

    }
    else if(a.bitfield.expo > 254)
    {
        // Infinite
        result.bitfield.sign = 0;
        result.bitfield.expo = 255;
        result.bitfield.partFrac = 0;
    }
    else if (b.bitfield.expo> 254)
    {
        // Not a Number
        result.bitfield.sign = 0;
        result.bitfield.expo = 255;
        result.bitfield.partFrac = 1;
    }
    else
    {



        // Paso 1
        // Escalar A y B
        float numA = 0;
        float numB = 0;

        for (int i = 0; i < n; i++)
        {
            if(mA.test(n-i-1))
            {
                //numA += pow(2, -i);
                numA = addVals(numA, pow(2, -i));
            }

            if(mB.test(n-i-1))
            {
                //numB +=pow(2, -i);
                numB = addVals(numB, pow(2, -i));
            }
        }

        cout << "A: " << mA.to_string() << " Escalado: " << numA << endl;
        cout << "B: " << mB.to_string() << " Escalado: " << numB << endl;

        // Paso 2
        // Buscar una solución aproximada

        float b_prime = 0;

        if(numB >= 1 && numB < 1.25)
        {
            b_prime = 1.00;
        }

        if(numB >= 1.25 && numB < 2.00)
        {
            b_prime = 0.80;
        }

        // Paso 3
        // Asignar x0 e y0

        float x0 = multiplyVals(numA, b_prime, &prodDenormal);
        float y0 = multiplyVals(numB, b_prime, &prodDenormal);

        cout << "A: " << numA << " B: " << numB << " B': " << b_prime << endl;
        cout << "Valor x0: " << x0 << endl;
        cout << "Valor y0: " << y0 << endl;


        // Paso 4
        // Iterar para obtener una aproximación
        float r = 0, x = x0, y = y0, x_old = 0;

        float threshold = pow(10, -4);
        cout << "T: " << threshold << endl;
        float difference = 100;

        while(difference>=threshold)
        {
            r = 2 - y;
            y = multiplyVals(y, r, &prodDenormal);
            x_old = x;
            x = multiplyVals(x, r, &prodDenormal);

            cout << "R:  " << r << endl;
            cout << "Y1: " << y << endl;
            cout << "X1: " << x << endl;
            cout << "X0: " << x_old << endl;
            difference = abs(addVals(x ,-x_old));
            cout << "DIF: " << difference << endl;
        }

        union Code last_x;
        last_x.numero = x;

        //cout << "X: " << x << endl;
        //cout << "X: " << last_x.bitfield.partFrac << endl;

        // Paso 6
        // Exponente de la división
        // TODO Usar método addVals (no funciona bien aquí, pero solo ocurre aquí)
        //int exponent = a.bitfield.expo - b.bitfield.expo + last_x.bitfield.expo;

        float exponent = addVals(static_cast<float>(a.bitfield.expo), -static_cast<float>(b.bitfield.expo));
        exponent = addVals(exponent, last_x.bitfield.expo);

        // Paso 5
        // Signo de la división
        result.bitfield.sign = a.bitfield.sign ^ b.bitfield.sign;

        if(exponent > 254)
        {
            result.bitfield.expo = 255;
            result.bitfield.partFrac = 0;
        }
        else
        {
            result.bitfield.expo = exponent;

            // Paso 7
            // Parte fraccionaria de la división
            result.bitfield.partFrac = last_x.bitfield.partFrac;
        }

    }


}
