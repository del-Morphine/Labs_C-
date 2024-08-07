#include <iostream>
#include <cstdlib>
#include <ctime>

typedef unsigned char BASE;
typedef unsigned short DBASE;
#define BASE_SIZE sizeof(BASE)*8;

using namespace std;

class BigNumber
{
    private:
        BASE *coef;
        int len;
        int maxlen;

        void lengthNormalization()
        {
            for(int i = len - 1; i > 0 && coef[i] == 0; i--)
                len--;
        }

    public:
        BigNumber(int maxlen = 1, bool random_option = false);
        BigNumber(const BigNumber &other);
        ~BigNumber();

        BigNumber& operator=(const BigNumber &other);
        bool operator==(const BigNumber &other) const;
        bool operator!=(const BigNumber &other) const;
        bool operator<(const BigNumber &other) const;
        bool operator>(const BigNumber &other) const;
        bool operator<=(const BigNumber &other) const;
        bool operator>=(const BigNumber &other) const;
        
        BigNumber operator+(const BigNumber &other) const;
        BigNumber operator+(const BASE &summand) const;
        BigNumber operator-(const BigNumber &other) const;
        BigNumber operator*(const BigNumber &other) const;
        BigNumber operator*(const BASE &multiplier) const;
        BigNumber operator/(const BigNumber &other) const;
        BigNumber operator/(const BASE &divisor) const;
        BigNumber operator%(const BASE &divisor) const;



        void scan_decimal();
        void print_decimal();
        friend ostream& operator<<(ostream &out, const BigNumber &number_to_print);
        friend istream& operator>>(istream &in, BigNumber &number_to_scan);
};

BigNumber::BigNumber(int maxlen, bool random_option)
{
    this->maxlen = maxlen;
    len = maxlen;
    if(maxlen <= 0)
        return;

    if(random_option == false)
    {
        len = 1;
        coef = new BASE[maxlen];
        for(int i = 0; i < maxlen; i++)
            coef[i] = 0;
        return;
    }

    coef = new BASE[maxlen];
    if(sizeof(BASE) > 2)
    {
        for(int i = 0; i < maxlen - 1; i++)
        {
            coef[i] = rand();
            coef[i] |= rand() << 16;
        }
    }
    for(int i = 0; i < maxlen - 1; i++)
        coef[i] = rand();

    coef[len - 1] = (rand() + 1) % (RAND_MAX - 1);
}

BigNumber::BigNumber(const BigNumber &other)
{
    maxlen = other.maxlen;
    len = other.len;
    coef = new BASE[len];
    for(int i = 0; i < len; i++)
        coef[i] = other.coef[i];
}

BigNumber::~BigNumber()
{
    if(coef)
    {
        delete coef;
        coef = NULL;
    }
}

BigNumber& BigNumber::operator=(const BigNumber &other)
{
    if(this == &other)
        return *this;
    if(coef != NULL)
        delete[] coef;
    
    maxlen = other.maxlen;
    len = other.len;
    coef = new BASE[maxlen]; //если у числа справа от '=' будет len < maxlen, то выделится лишнее

    for(int i = 0; i < len; i++)
        coef[i] = other.coef[i];

    return *this;
}

bool BigNumber::operator==(const BigNumber &other) const
{
    if(len < other.len)
        return false;
    if(len > other.len)
        return false;
    for(int i = 0; i < len; i++)
        if(coef[i] != other.coef[i])
            return false;
    return true;
}

bool BigNumber::operator!=(const BigNumber &other) const
{
    if(len < other.len)
        return true;
    if(len > other.len)
        return true;
    for(int i = 0; i < len; i++)
        if(coef[i] != other.coef[i])
            return true;
    return false;
}

bool BigNumber::operator<(const BigNumber &other) const
{
    if(len < other.len)
        return true;
    if(len > other.len)
        return false;
    for(int i = len - 1; i >= 0; i--)
    {        
        if(coef[i] > other.coef[i])
            return false;
        if(coef[i] < other.coef[i])
            return true;
    }
    return false;
}

bool BigNumber::operator>(const BigNumber &other) const 
{
    if(len > other.len)
        return true;
    if(len < other.len)
        return false;
    for(int i = len - 1; i >= 0; i--)
    {    
        if(coef[i] < other.coef[i])
            return false;
        if(coef[i] > other.coef[i])
            return true;
    }
    return false;
}

bool BigNumber::operator<=(const BigNumber &other) const
{
    if(len < other.len)
        return true;
    if(len > other.len)
        return false;
    for(int i = len - 1; i >= 0; i--)
        if(coef[i] > other.coef[i])
            return false;
    return true; 
}

bool BigNumber::operator>=(const BigNumber &other) const
{
    if(len > other.len)
        return true;
    if(len < other.len)
        return false;
    for(int i = len - 1; i >= 0; i--)
        if(coef[i] < other.coef[i])
            return false;
    return true; 
}

BigNumber BigNumber::operator+(const BigNumber &other) const
{
    const BigNumber *bigger;
    const BigNumber *smaller;
    if(len > other.len)
    {
        bigger = this;
        smaller = &other;
    }
    else
    {
        bigger = &other;
        smaller = this;
    }

    BigNumber res(bigger->len + 1, false);
    res.len = bigger->len + 1;  //конструктор с random_option=false сокращает len до 1; поэтому переприсваиваем

    
    for(int i = 0; i < smaller->len; i++)
        res.coef[i] = smaller->coef[i];

    BASE carry = 0;
    DBASE sum;
    for(int i = 0; i <= bigger->len; i++)
    {
        sum = (DBASE)res.coef[i] + (DBASE)bigger->coef[i];
        res.coef[i] = (BASE)sum;
        carry = sum >> BASE_SIZE;
        res.coef[i + 1] += carry;
    }

    res.lengthNormalization();
    return res;
}

BigNumber BigNumber::operator+(const BASE &summand) const
{
    BigNumber res(len + 1, false);
    res.len = len + 1;

    for(int i = 0; i < len; i++)
        res.coef[i] = coef[i];

    DBASE sum = (DBASE)res.coef[0] + (DBASE)summand;
    BASE carry = sum >> BASE_SIZE;
    res.coef[0] = (BASE)sum;
    for(int i = 0; carry; i++)
    {
        sum = res.coef[i + 1] + carry;
        carry = sum >> BASE_SIZE;
        res.coef[i + 1] = sum;
    }

    res.lengthNormalization();
    return res;
}

BigNumber BigNumber::operator-(const BigNumber &other) const
{
    if(*this < other)
        exit(EXIT_FAILURE);

    BigNumber res(len, false); 
    res.len = len;    
    DBASE sub;
    BASE borrow = 0;
    int base_size = BASE_SIZE;
    
    for(int i = 0; i < other.len; i++) 
    {
        sub = ((DBASE)1 << base_size) | coef[i];
        sub -= other.coef[i] + borrow;
        res.coef[i] = (BASE)sub;
        borrow = !(sub >> base_size);
    }

    for(int i = other.len; i < len; i++) 
    {
        sub = ((DBASE)1 << base_size) | coef[i];
        sub -= borrow;
        res.coef[i] = (BASE)sub;
        borrow = !(sub >> base_size);
    }

    res.lengthNormalization();
    return res;
}

BigNumber BigNumber::operator*(const BigNumber &other) const
{
    BigNumber res(len + other.len, false);
    res.len = len + other.len;
    
    BASE carry;
    DBASE mul;
    for(int i = 0; i < other.len; i++)
    {
        if(other.len != 0)
        {
            carry = 0;
            for(int j = 0; j < len; j++)
            {
                mul = (DBASE)other.coef[i] * (DBASE)coef[j] + (DBASE)res.coef[i + j] + (DBASE)carry;
                res.coef[i + j] = (BASE)mul;
                carry = mul >> BASE_SIZE;
            }
            res.coef[len + i] = carry;
        }
    }

    res.lengthNormalization();
    return res;
}

BigNumber BigNumber::operator*(const BASE &multiplier) const
{
    int base_size = BASE_SIZE;
    BigNumber res(len + 1, false); 
    res.len = len + 1;

    DBASE mul;
    BASE carry = 0;
    for(int i = 0; i < len; i++) 
    {
        mul = (DBASE)coef[i] * (DBASE)multiplier + (DBASE)carry;
        res.coef[i] = (BASE)mul;	
        carry = (BASE)(mul >> base_size);
    }
    res.coef[len] = carry;

    res.lengthNormalization();    
    return res;
}

BigNumber BigNumber::operator/(const BigNumber &other) const
{
    if(other.len == 1 && other.coef[0] == 0)
        exit(EXIT_FAILURE);

    if(*this < other)
    {
        BigNumber res(1);
        return res;
    }
    if(other.len == 1)
    {               //деление на однозначное число
        return *this / other.coef[0];
    }
    if(*this == other){               //если делимое и делитель равны
        BigNumber res(1);
        res.coef[0] = 1;
        return res;
    }

    int base_size = BASE_SIZE;
    int m = len - other.len;
    //D1
    DBASE b = 1 << base_size;
    DBASE d = b / ((DBASE)other.coef[other.len - 1] + (DBASE)1);

    BigNumber u = *this * d;
    BigNumber v = other * d;
    BigNumber res(m + 1, false);
    res.len = m + 1;

    if(u.len == len)
    {
        u.len++;
        u.coef[u.len - 1] = 0;
    }

    //D2
    DBASE q;
    DBASE r;
    BigNumber tmp (v.len + 1);
    BigNumber t;
    tmp.len = v.len + 1;
    int j = m; 
    for(j; j >= 0; j--)
    {
        q = (DBASE)((DBASE)((DBASE)((DBASE)(u.coef[j + v.len]) * (DBASE)(b)) + (DBASE)(u.coef[j + v.len - 1])) / (DBASE)(v.coef[v.len - 1]));
        r = (DBASE)((DBASE)((DBASE)((DBASE)(u.coef[j + v.len]) * (DBASE)(b)) + (DBASE)(u.coef[j + v.len - 1])) % (DBASE)(v.coef[v.len - 1]));
    
        if(q == b || (DBASE)((DBASE)(q) * (DBASE)(v.coef[v.len - 2])) > (DBASE)((DBASE)((DBASE)(b) * (DBASE)(r)) + (DBASE)(u.coef[j + v.len - 2])))
        {
            q--;
            r = (DBASE)(r) + (DBASE)(v.coef[v.len - 1]);

            if((DBASE)r < b)
                if(q == b || (DBASE)((DBASE)(q) * (DBASE)(v.coef[v.len - 2])) > (DBASE)((DBASE)((DBASE)(b) * (DBASE)(r)) + (DBASE)(u.coef[j + v.len - 2])))
                    q--;
        }

        for(int i = 0; i < v.len + 1; i++)
            tmp.coef[i] = u.coef[j + 1];
        t = v * (BASE)q;
        if(tmp < t)
        {
            q--;
            t = t - v;
        }
        tmp = tmp - t;
        res.coef[j] = (BASE)q;

        for(int i = 0; i < v.len + 1; i++)
            u.coef[i + 1] = tmp.coef[i];
    }

    res.lengthNormalization();
    return res;
}

BigNumber BigNumber::operator/(const BASE &divisor) const
{
		if (divisor == 0) 
			exit(3);

        BigNumber res(len, false); 
		res.len = len;

		DBASE division;
		BASE remainder = 0;
        int base_size = BASE_SIZE;
		for(int i = 0; i < len; i++) 
		{
			division = ((DBASE)remainder << base_size) + (DBASE)coef[len - i - 1];
			res.coef[len - i - 1] = (BASE)(division / divisor);
			remainder = (BASE)(division % divisor);
		}

        res.lengthNormalization();
		return res;  
}

BigNumber BigNumber::operator%(const BASE &divisor) const
{
    if(divisor == 0)
        exit(EXIT_FAILURE);
        
    BigNumber res(1, false);

    DBASE division;
    BASE remainder = 0;
    int base_size = BASE_SIZE;
    for(int i = 0; i < len; i++)
    {
        division = ((DBASE)remainder << base_size) + (DBASE)coef[len - 1 - i];
        remainder = (BASE)(division % (DBASE)divisor);
    }
    res.coef[0] = remainder;
    
    return res;
}

void BigNumber::scan_decimal() 
{
    
    char number_string[101];
    cin.getline(number_string, 100);
    int digits = 0;
    for(digits; number_string[digits]; digits++)
        if(number_string[digits] < '0' || '9' < number_string[digits])
            exit(EXIT_FAILURE);

    BASE digit = 0;
    int base_size = BASE_SIZE;
    BigNumber res(1, false);
    res.len = res.maxlen;
    for (int i = 0; i < digits; i++) 
    {
        digit = (BASE)(number_string[i] - '0');
        res = res * 10 + digit;
    }
    res.lengthNormalization();
    *this = res;
}

void BigNumber::print_decimal()
{
    BigNumber digit(1, false);
    char s[500];
    BigNumber to_print = *this;
    if (to_print.coef[0] == 0 && to_print.len == 1) 
    {
        cout << '0';
        return; 
    }
    
    int i = 0;
    for(i; !(to_print.coef[0] == 0 && to_print.len == 1); i++) 
    {
        digit = to_print % 10;
        s[i] = digit.coef[0] + '0';
        to_print = to_print / 10;
    }

    for (i -= 1; i >= 0; i--) 
        cout << s[i];
}

ostream& operator<<(ostream &out, const BigNumber &number_to_print)
{
    if(&number_to_print == NULL)
        return out;
    int base_size = BASE_SIZE;
    char *number_string = new char[number_to_print.len * base_size / 4 + 1];
    int mask = 0b00001111;
    BASE hex_digit;
    int digit = 0;

    for(int i = number_to_print.len - 1; i >= 0; i--)
        for(int shift = (sizeof(BASE)*2-1)*4; shift >= 0; shift -= 4)
        {
            hex_digit = (number_to_print.coef[i] >> shift) & mask;
            if(0 <= hex_digit && hex_digit <= 9)
            { 
                hex_digit += '0';   
                number_string[digit] = hex_digit;
            }
            else if(hex_digit >= 10)
            {
                hex_digit += -10 + 'A';    
                number_string[digit] = hex_digit;
            }    
            digit++;
        }
    number_string[digit] = '\0';    
    out << number_string;
    delete[] number_string;
    return out;
}

istream& operator>>(istream &in, BigNumber &number_to_scan)
{
    char number_string[101];
    in.getline(number_string, 100);
    
    int digits = 0;
    bool is_digit;
    for(digits; number_string[digits]; digits++)
    {        
        is_digit = false;
        if('0' <= number_string[digits] && number_string[digits] <= '9')
            is_digit = true;
        else if('a' <= number_string[digits] && number_string[digits] <= 'f')
            is_digit = true;
        else if('A' <= number_string[digits] && number_string[digits] <= 'F')
            is_digit = true;
        if(is_digit == false)
            exit(EXIT_FAILURE);
    }

    if(number_to_scan.coef != NULL)
        delete[] number_to_scan.coef;

    int base_size = BASE_SIZE;
    number_to_scan.len = (digits - 1) / (base_size / 4) + 1; //кол-во цифр делим на кол-во цифр, которое может хранить типа данных
    //number_to_scan.len += ((digits) % (base_size / 4)) > 0; //если деление выше дало ноль, то тип хранит цифры даже с запасом, деление будет с остатком
    number_to_scan.maxlen = number_to_scan.len;
    number_to_scan.coef = new BASE[number_to_scan.len];

    for(int i = 0; i < number_to_scan.len; i++)
        number_to_scan.coef[i] = 0;

    BASE digit;
    int shift = 0;
    
    for(int i = digits - 1, j = 0; i >= 0; i--)
    {
        if('0' <= number_string[i] && number_string[i] <= '9')
            digit = number_string[i] - '0';
        else if('a' <= number_string[i] && number_string[i] <= 'f')
            digit = number_string[i] + 10 - 'a';
        else if('A' <= number_string[i] && number_string[i] <= 'F')
            digit = number_string[i] + 10 - 'A';

        number_to_scan.coef[j] |= digit << shift;
        shift += 4;
        if(shift > (sizeof(BASE)*2-1)*4)
        {
            shift = 0;
            j++;
        }
    }    
    for(int i = number_to_scan.len - 1; i > 0 && number_to_scan.coef[i] == 0; i--)
        number_to_scan.len--;
    return in;
}

void test(){
    srand((unsigned)time(NULL));
    int M = 1000;
    int T = 1000;

    BigNumber A;
    BigNumber B;
    BigNumber C;
    BigNumber D;
    do {

        int n = rand()%M + 1;
        int m = rand()%M + 1;
        BigNumber E (n, 1);
        BigNumber G (m, 1);
        A = E;
        B = G;
        C = A / B;
        //D = A % B;
        cout<<"m: "<< m <<" ";
        cout<<"n: "<< n <<" ";
        cout<<"T: "<< T <<endl;
    }
    while (A == C * B + D && A - D == C*B && D<B && --T);
    cout << T << endl;

}


int main(int argc, char const *argv[])
{
    srand(time(0));
    BigNumber a(30, true);
    BigNumber b(14, true);
    cout << a << endl << b << endl;
    BigNumber c = a / b;
    cout << c << endl;
    return 0;
}
