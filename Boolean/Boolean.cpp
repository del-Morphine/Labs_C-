#include "Boolean.h"

Boolean_vector::Boolean_vector(int bits)
{
    bytes = (bits / 8);
    this->bits = bits;

    vector = new uint8_t[(bits / 8) + (bits%8 > 0)];
    for(int byte = 0; byte < bytes; byte++)
        vector[byte] = 0;
    vector[bytes] = 0;
}

Boolean_vector::Boolean_vector(const char *strVector)
{
    bits = 0;
    for(bits; strVector[bits]; bits++)
        if(strVector[bits] != '0' && strVector[bits] != '1')
            return;
    
    bytes = bits / 8;
    vector = new uint8_t[(bits / 8) + (bits%8 > 0)];

    for(int byte = 0; byte <= bytes; byte++)
        vector[byte] = 0;

    int index = 0;
    for(int byte = 0; byte < bytes; ++byte) {
        for(int bit = 0; bit < 8; ++bit) {
            index = bits - 1 - (byte * 8 + bit);
            vector[byte] |= ((strVector[index] - '0') << bit);
        }
    }
    for(int bit = 0; bit < bits % 8; ++bit) {
        index = bits - 1 - (bytes * 8 + bit);
        vector[bytes] |= ((strVector[index] - '0') << bit);
    }    
}

Boolean_vector::Boolean_vector(const Boolean_vector &other)
{
    bits = other.bits;
    bytes = other.bytes;
    vector = new uint8_t[bytes];

    for(int byte = 0; byte < bytes; byte++)
        vector[byte] = other.vector[byte];
    vector[bytes] = other.vector[bytes];
}

Boolean_vector::~Boolean_vector()
{
    if(vector)
        delete[] vector;
}

void Boolean_vector::Set1(int pos)
{
    if(pos < 0 || pos > bits - 1)
        return;
    vector[pos / 8] |= 1 << (pos % 8);
}

void Boolean_vector::Set0(int pos)
{
    if(pos < 0 || pos > bits - 1)
        return;
    vector[pos / 8] &= ~(1 << (pos % 8));    
}

int Boolean_vector::operator[](int pos)
{
    if(pos < 0 || pos > bits - 1)
        return -1;
    return vector[pos / 8] >> (pos % 8) & 1;   
}

Boolean_vector& Boolean_vector::operator=(const Boolean_vector &other)
{
    if(this == &other)
        return *this;
    if(vector)
        delete[] vector;
    
    bits = other.bits;
    bytes = other.bytes;
    vector = new uint8_t[bytes];

    for(int byte = 0; byte < bytes; byte++)
        vector[byte] = other.vector[byte];
    vector[bytes] = other.vector[bytes];
    return *this;
}

Boolean_vector Boolean_vector::operator| (const Boolean_vector &other)
{
    const Boolean_vector *greater;
    const Boolean_vector *less;
    if(bits > other.bits)
    {
        greater = this;
        less = &other;
    }
    else
    {
        greater = &other;
        less = this;
    }
    Boolean_vector res = *greater;

    for(int byte = 0; byte < less->bytes; byte++)
        res.vector[byte] = vector[byte] | other.vector[byte];
    for(int bit = 0; bit < less->bits % 8; bit++)
        res.vector[less->bytes] |= (vector[less->bytes] | other.vector[less->bytes]) & (1 << bit);
    return res;
}

Boolean_vector Boolean_vector::operator& (const Boolean_vector &other)
{
    int short_vector_bits;
    const Boolean_vector *greater;
    const Boolean_vector *less;
    if(bits > other.bits)
    {
        less = &other;
        greater = this;
    }
    else
    {
        less = this;
        greater = &other;
    }
    Boolean_vector res = *greater;

    int byte = 0;
    for(byte; byte < less->bytes + (less->bits%8 > 0); byte++)
        res.vector[byte] = vector[byte] & other.vector[byte];
    for(byte; byte < greater->bytes + (greater->bits%8 > 0); byte++)
        res.vector[byte] = 0;
    return res;    
}

Boolean_vector Boolean_vector::operator~()
{
    uint8_t mask = 0b11111111;
    Boolean_vector res = *this;
    for(int byte = 0; byte < bytes; byte++)
        res.vector[byte] ^= mask;
    for(int bit = 0; bit < bits % 8; bit++)
        res.vector[bytes] ^= 1 << bit;
    return res;
}

int Boolean_vector::Weight()
{
    int weight = 0;
    for(int byte = 0; byte < bytes; byte++)
        for(int bit = 0; bit < 8; bit++)
            weight += (vector[byte] >> bit) & 1;
    for(int bit = 0; bit < bits % 8; bit++)
        weight += (vector[bytes] >> bit) & 1;
    return weight;
}

ostream& operator<<(ostream &out, const Boolean_vector &other)
{
    char *strVector = new char[other.bits + 1];
    for(int bit = other.bits - 1; bit >= 0; --bit) {
        strVector[other.bits - bit - 1] = other.vector[bit / 8] >> (bit % 8) & 1;
        strVector[other.bits - bit - 1] += '0';
    }
    strVector[other.bits] = '\0';    

    out << strVector;
    delete[] strVector;
    return out;
}

istream& operator>>(istream &in, Boolean_vector &other)
{
    char strVector[257];
    in.getline(strVector, 256);

    int bits = 0;
    for(bits; strVector[bits]; bits++)
        if(strVector[bits] != '0' && strVector[bits] != '1')
            return in;
            
    int bytes = bits / 8;
    uint8_t *vector = new uint8_t[bytes + (bits%8 > 0)];
    int index = 0;
    for(int byte = 0; byte < bytes; ++byte) {
        for(int bit = 0; bit < 8; ++bit) {
            index = bits - 1 - (byte * 8 + bit);
            vector[byte] |= ((strVector[index] - '0') << bit);
        }
    }
    for(int bit = 0; bit < bits % 8; ++bit) {
        index = bits - 1 - (bytes * 8 + bit);
        vector[bytes] |= ((strVector[index] - '0') << bit);
    }

    other.bits = bits;
    other.bytes = bytes;
    if(other.vector)
        delete[] other.vector;
    other.vector = vector;
    return in;      
}

Boolean_matrix::Boolean_matrix(int rows, int cols)
{
    this->rows = rows;
    this->cols = cols;
    matrix = new Boolean_vector[rows];
    for(int row = 0; row < rows; row++)
        matrix[row] = Boolean_vector(cols);
}

Boolean_matrix::Boolean_matrix(const Boolean_matrix &other)
{
    rows = other.rows;
    cols = other.cols;
    matrix = new Boolean_vector[rows];
    for(int row = 0; row < rows; row++)
        matrix[row] = other.matrix[row];
}

Boolean_vector& Boolean_matrix::operator[](int row)
{
    return matrix[row];
}

Boolean_matrix& Boolean_matrix::operator=(const Boolean_matrix &other)
{
    if(this == &other)
        return *this;
    if(matrix)
        delete[] matrix;
    rows = other.rows;
    cols = other.cols;
    matrix = new Boolean_vector[rows];
    for(int row = 0; row < rows; row++)
        matrix[row] = other.matrix[row];    
    return *this;
}

ostream& operator<<(ostream &out, Boolean_matrix &other)
{
    for(int row = 0; row < other.rows; row++)
        out << other.matrix[row] << endl;
    return out;
}