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
    //Según ChatGPT, podemos negar un número con (~variable), luego el complemento a 2 es
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
bool IEEEOperations::operandosOpuestos()
{
    return op1.numero == -op2.numero;
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

//Metodo de la suma

void IEEEOperations::add()
{
    union Code a,b;
    a.numero = op1.numero;
    b.numero = op2.numero;

    mantisaA = a.bitfield.partFrac | 0x800000;
    mantisaB = b.bitfield.partFrac | 0x800000;

    //Casos raros:
    //TODO reformatear esto
    if (operandosOpuestos()) {
        if (a.bitfield.expo == 0xFF && a.bitfield.partFrac == 0) {
            result.bitfield.expo = 0xFF;
            result.bitfield.sign = a.bitfield.sign;
            result.bitfield.partFrac = 0x0;
            //Forzamos un inf
        } else {
            result.numero=0;
        }
        return;
    } if (esOp1Denormal()) {
        mantisaA &= 0x7FFFFF;
    }
    if (esOp2Denormal()) {
        mantisaB &= 0x7FFFFF;
    } if (a.bitfield.expo == 0xFF && a.bitfield.partFrac == 0) {
        result.bitfield.expo = 0xFF;
        result.bitfield.sign = a.bitfield.sign;
        result.bitfield.partFrac = 0x0; //Si op1 es inf, el resultado tb
        return;
    }

    // Paso 1
    cout << "Paso 1: " << endl;

    cout << " Signo A: " << a.bitfield.sign << " Exponente A: " << a.bitfield.expo << " Fraccionaria A: " << a.bitfield.partFrac << endl;
    cout << " Signo B: " << b.bitfield.sign << " Exponente B: " << b.bitfield.expo << " Fraccionaria B: " << b.bitfield.partFrac << endl;

    int g = 0;
    int r = 0;
    int st = 0;
    bool c; //Acarreo
    bool operandos_intercambiados = false;
    bool complementado_P = false;
    unsigned int mask;

    // Paso 2
    cout << "Paso 2: Si a < b intercambiamos" << endl;
    if (a.bitfield.expo < b.bitfield.expo)
    {
        // Intercambiamos los operandos
        Code tmp = a;
        a = b;
        b = tmp;
        mask = mantisaA;
        mantisaA = mantisaB;
        mantisaB = mask;
        operandos_intercambiados = true;
    }

    // Paso 3
    cout << "Paso 3: " << endl;

    result.bitfield.expo = a.bitfield.expo;
    int d = a.bitfield.expo - b.bitfield.expo;

    cout << "Exponente suma: " << result.bitfield.expo << " d: " << d << endl;

    // Paso 4
    cout << "Paso 4: " << endl;

    if (a.bitfield.sign != b.bitfield.sign)
    {
        mantisaB = complementoDos(mantisaB);
        cout << "Los signos no coinciden. Mantisa B: " << mantisaB << endl;
    }

    unsigned int p = mantisaB;
    cout<<"Valor de p: "<<p<<endl;

    //Paso 6
    cout<<"Paso 6: "<<endl;
    if (d>=1) {
        g = (p >> (d-1)) & 1;
        cout<<"Valor de g cuando d >= 1: "<<g<<endl;
    }
    if (d >= 2)
    {
        r = (p >> (d - 2)) & 1;
        cout << "Valor de r cuando d >= 2: " << r << endl;
    }

    if (d >= 3)
    {

        unsigned int tmp = p;

        for (int i = 0; i<d-3; i++) {
            st |= (tmp & 0b1); //Hacemos un or con el último bit de tmp
            tmp = tmp >> 1; //Desplazamos tmp uno a la derecha.
        }

        cout<<"Valor de mask cuando d >= 3: "<<mask<<endl<<"Valor de st: "<<st<<endl;
    }
    // Paso 7
    cout << "Paso 7: " << endl;
    if (b.bitfield.sign != a.bitfield.sign)
    {
        p = complementoUno(p);
        p = p >> d;
        p = complementoUno(p);
        cout << "Valor de mask cuando signos de a y b no coinciden: " << mask << endl
             << "Valor de p: " << p << endl;
    }
    else
    {
        p = p >> d;
        cout << "Si los valores coiniden: valor de p " << p << endl;
    }

    // Paso 8
    cout << "Paso 8: " << endl;
    p = p + mantisaA;

    cout << "Valor de p: " << p << endl;

    //¿Se ha producido desbordamiento? (un acarreo al final)
    if (p>0xFFFFFF) { //Si p >= 2^24, es que ocupa 25 bits y el primero es un uno, esdecir, hubo desbordamiento y acarreo
        p = p & 0xFFFFFF; //Me cargo ese uno que se añadió al producirse desbordamiento
        c = 1; //c=1

        cout<<"Hubo acarreo al final de la suma. Valor de p: "<<p<<" Valor de c: "<<c<<"Valor de mask: "<<mask<<endl;
    } else {
        c = 0; //c = 0
    }

    //Paso 9
    cout<<"Paso 9: "<<endl;
    if (a.bitfield.sign != b.bitfield.sign && ((p & 0x800000) == 0x800000) && (c==0)) {
        p = complementoDos(p);
        complementado_P = true;
    }

    //Paso 10
    cout<<"Paso 10: "<<endl;
    if (a.bitfield.sign == b.bitfield.sign && c==1) {
        st = g|r|st;
        r = p & 0x1;
        p = p >> 1;
        if (c==1) {
            p = p | 0x800000; //Poner el uno de c
        }
        else
        {
            p = p & mask;
        }
        // Ajusto el exponente de la suma:
        result.bitfield.expo += 1;
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
        result.bitfield.expo -= k;
    }

    //Paso 11
    cout<<"Paso 11: "<<endl;
    unsigned int p0 = p & 0x1;
    if ((r==1 && st==1) || (r==1 && st==0 && p0 == 1)) {
        p+=1;
        if (p > 0xFFFFFF){
            c = 1;
            p = p & 0xFFFFFF;
        } else {
            c=0;
        }
        if (c==1) { //Si hay acarreo, c=1
            p = p >> 1; //Desplazo 1 bit a la derecha p
            p = p | 0x800000; //Añado el uno del carry al principio
            result.bitfield.expo += 1; //Ajustamos el exponente
        }
    }
    result.bitfield.partFrac = p & 0x7FFFFF;
    //result.mantisa=p; //Falla

    // Paso 12
    cout << "Paso 12: " << endl;
    if ((operandos_intercambiados == false) && (complementado_P == true))
    {
        result.bitfield.sign = b.bitfield.sign;
    }
    else
    {
        result.bitfield.sign = a.bitfield.sign;
    }

    //Paso 13
    cout<<"Paso 13: "<<endl;

    //Test
    cout<<" Signo A: "<<a.bitfield.sign<<" Exponente A: "<<a.bitfield.expo<<" Fraccionaria A: "<<a.bitfield.partFrac<<endl;
    cout<<" Signo B: "<<b.bitfield.sign<<" Exponente B: "<<b.bitfield.expo<<" Fraccionaria B: "<<b.bitfield.partFrac<<endl;
    cout<<" Signo Result: "<<result.bitfield.sign<<" Exponente Result: "<<result.bitfield.expo<<" Fraccionaria Result: "<<result.bitfield.partFrac<<endl;

}

unsigned int IEEEOperations::multiplyWithoutSign(bitset<24> *MA, bitset<24> MB)
{

    // Paso 1
    //  Almacenar valores en los registros A, B y P
    bitset<24> P{0};
    int n = 24;
    bool c = false; // Acarreo

    cout << MA->to_string() << endl;
    cout << MA->test(0) << endl;

    for (int i = 0; i < n; i++)
    {
        // Paso 1.1
        if (MA->test(0))
        {
            P = bitset<24>{P.to_ullong() + MB.to_ullong()};
        }

        // ¿Se ha producido desbordamiento? (un acarreo al final)
        if (P.to_ullong() >= 0xFFFFFF)
        { // Si p >= 2^24, es que ocupa 25 bits y el primero es un uno, esdecir, hubo desbordamiento y acarreo
            // Hubo un acarreo al final de la suma ya que este AND ya que no devuelve 0
            P = P.to_ullong() & 0b111111111111111111111111; // Me cargo ese uno que se añadió al producirse desbordamiento
            c = true;                                       // c=1
            cout << "Hubo acarreo al final de la suma. Valor de p: " << P.to_string() << " Valor de c: " << c << endl;
        }
        else
        {
            c = false;
        }

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
        cout << "P: " << P.to_string() << " A: " << MA->to_string() << endl;
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
    int n = 24;

    bitset<24> mA{a.bitfield.partFrac};
    bitset<24> mB{b.bitfield.partFrac};

    mA.set(n - 1);
    mB.set(n - 1);

    // Paso 1
    // Signo del producto
    result.bitfield.sign = a.bitfield.sign ^ b.bitfield.sign;

    // Paso 2
    // Exponente del producto
    int exponent = (a.bitfield.expo - 127) + (b.bitfield.expo - 127);

    // Paso 3
    // 3.1 Mantisa del producto
    cout << "A: " << mA.to_string() << endl;
    bitset<24> P{multiplyWithoutSign(&mA, mB)};
    cout << "P: " << P.to_string() << " A: " << mA.to_string() << endl;

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
        cout << "A0 = 0" << endl;
        cout << "P: " << P.to_string() << " A: " << mA.to_string() << endl;
    }
    else
    {
        cout << "A0 = 1" << endl;
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
        P = bitset<24>{P.to_ullong() + 1};
        cout << "P: " << P.to_string() << " A: " << mA.to_string() << endl;
    }

    exponent += 127;

    // Comprobación desbordamientos
    if (checkOverflow(exponent))
    {
        result.bitfield.partFrac = 0;
        result.bitfield.expo = 255;
    }

    else if (checkUnderflow(exponent))
    {

        int t = 1 - exponent;

        if (t >= n)
        {
            result.bitfield.partFrac = 1;
            result.bitfield.expo = 255;
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

            result.bitfield.expo = 1;
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
                int t = 1 - exponent;

                if (t >= n)
                {
                    result.bitfield.partFrac = 1;
                    result.bitfield.expo = 255;
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

                    result.bitfield.expo = 1;
                }
            }

            // Caso 2: Exponente producto > Exponente mínimo

            else if (exponent > 1)
            {
                int t1 = exponent - 1;
                int t2 = 0;

                while (mA.test(n - t2 - 1) == false)
                {
                    t2++;
                }

                int t = min(t1, t2);

                result.bitfield.expo = exponent - t;

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
                // TODO Controlar casos denormales
            }
        }

        result.bitfield.expo = exponent;
        // Mantisa final
        cout << "P: " << P.to_string() << endl;
        cout << "E: " << exponent << endl;
        cout << "E: " << result.bitfield.expo << endl;
        cout << "P: " << P.to_ullong() << endl;
        result.bitfield.partFrac = P.to_ullong();
    }
}

void IEEEOperations::multiplyA()
{
    result.numero = 0;

    union Code a, b;
    a.numero = op1.numero;
    b.numero = op2.numero;

    this->mantisaA = a.bitfield.partFrac | 0x800000;
    this->mantisaB = b.bitfield.partFrac | 0x800000;

    // Casos raros:
    if (operandosOpuestos())
    {
        result.numero = 0;
        return;
    }
    else
    {

        if (esOp1Denormal())
        {
            mantisaA &= 0b011111111111111111111111;
        }
        if (esOp2Denormal())
        {
            mantisaB &= 0b011111111111111111111111;
        }
    }

    // Paso 1
    unsigned int p;
    int n = 24;
    cout << "Paso 1: " << endl;

    cout << " Signo A: " << a.bitfield.sign << " Exponente A: " << a.bitfield.expo << " Fraccionaria A: " << a.bitfield.partFrac << endl;
    cout << " Signo B: " << b.bitfield.sign << " Exponente B: " << b.bitfield.expo << " Fraccionaria B: " << b.bitfield.partFrac << endl;

    result.bitfield.sign = a.bitfield.sign ^ b.bitfield.sign;

    result.bitfield.expo = a.bitfield.expo + b.bitfield.expo;

    // Paso 1: Multiplicación binaria sin signo de las mantisas
    // p = multiplyWithoutSign(); //En mantisaA está A y en p, P.

    // Paso 2
    if ((p >> (n - 1)) % 2 == 0)
    {
        bool bit = (mantisaA << (n - 1)) & 1;
        mantisaA = mantisaA << 1;

        p = p << 1;

        if (bit) // si el bit es 1, hay que metérselo a P, sino no hace falta ya que por defecto se puso un 0 al desplazar.
        {
            p = p + 1;
        }
    }
    else
    {
        result.bitfield.expo++;
    }

    // Paso 3: bit de redondeo
    bool r = (mantisaA >> (n - 1)) & 1;

    // Paso 4: bit sticky
    unsigned int mask = (1u << (n - 2)); // Creamos una máscara de bits que tenga 1 en las posiciones 0 a n-2, inclusive
    unsigned int subset = mantisaA & mask;
    bool st = subset != 0;
    cout << "Valor de mask: " << mask << endl
         << "Valor de st: " << st << endl;

    // Paso 5: redondeo
    if ((r == 1 && st == 1) || (r == 1 && st == 0 && p % 2 == 1))
    {
        p = p + 1;
    }

    // Comprobación de desbordamientos
    bool overflow = checkOverflow(1);

    if (overflow)
    {
        // Convertimos el número en infinito
        this->result.bitfield.expo = 255;
        this->result.bitfield.partFrac = 0;
    }
    // Underflow y punto 1 operandos denormales
    if (this->result.bitfield.expo < 1)
    {
        int t = 1 - result.bitfield.expo;
        if (t >= 24)
        { // la mantisa tiene 24 bits
            // Underflow --> convertimos el número en 0
            this->result.numero = 0;
        }
        else
        {
            // TODO: Teóricamente hay que desplazar P,A t bits a la derecha, pero si luego solo vamos a utilizar P, ¿para qué tener en cuenta a A?
            p = p >> t;
            this->result.bitfield.expo = 0; // Resultado denormal
        }
    }
    // Tratamiento de operandos denormales
    if (esOp1Denormal() || esOp2Denormal())
    {
        if (this->result.bitfield.expo > 1)
        { // TODO el exponente mínimo es 1?
            unsigned int t1 = this->result.bitfield.expo - 1;

            int k = 23;
            unsigned long p2 = p;
            p2 = p2 << 24;
            p2 = p2 + mantisaA;
            k -= std::log2(p2);
            unsigned int t2 = k;

            unsigned int t = t1;
            if (t2 < t1)
            {
                t = t2;
            }

            this->result.bitfield.expo = this->result.bitfield.expo - t;
            p2 = p2 >> (24 - t); // Esto es lo mismo que desplazar aritméticamente (P,A) t bits a la izda. TODO: Seguro?
            p = p2;
        }
    }
    result.bitfield.partFrac = (p & 0x7FFFFF);
}

// Metodo de la division
void IEEEOperations::divide()
{

    // Test
    cout << " Signo A: " << signoA << " Exponente A: " << exponenteA << " Mantisa A: " << mantisaA << endl;
    cout << " Signo B: " << signoB << " Exponente B: " << exponenteB << " Mantisa B: " << mantisaB << endl;

    // Numero para tests
    result.numero = 33;
}
