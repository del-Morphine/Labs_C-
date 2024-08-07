#ifndef BOOLEAN_H
#define BOOLEAN_H

#include <iostream>
using namespace std;

class Boolean_vector
{
    private:
        uint8_t *vector;
        int bits;
        int bytes;
    
    public:
        Boolean_vector(int bits = 1);
        Boolean_vector(const char* strVector);
        Boolean_vector(const Boolean_vector &other);
        ~Boolean_vector();

        void Set1(int pos);
        void Set0(int pos);
        int operator[](int pos);

        Boolean_vector& operator=(const Boolean_vector &other);
        Boolean_vector operator| (const Boolean_vector &other);
        Boolean_vector operator& (const Boolean_vector &other);
        Boolean_vector operator~();

        int Weight();

        friend ostream& operator<<(ostream &out, const Boolean_vector &other);
        friend istream& operator>>(istream &in, Boolean_vector &other);
};

class Boolean_matrix
{
    private:
        Boolean_vector *matrix;
        int rows, cols;
    public:
        Boolean_matrix(int rows = 1, int cols = 1);
        Boolean_matrix(const Boolean_matrix &other);    
        Boolean_vector& operator[](int row);
        Boolean_matrix& operator=(const Boolean_matrix& other);

        friend ostream& operator<<(ostream &out, Boolean_matrix &other);
};

#endif