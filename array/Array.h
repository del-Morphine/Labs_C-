#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>

using namespace std;
class Array
{
    private:
        int *arr;
        int size;
    public:
        Array(const int* array, int size);
        Array(int size = 1, int order = 0, int range = 10);
        Array(const Array& other);
        ~Array();

        Array& operator=(const Array& other);
        int& operator[](int index);
        int Size() {return size;};

        bool Test();
        bool operator==(const Array& other);

        void ShellSort();
        void RadixSort(int left, int right, int k);
        void QuickSort(int left, int right);

        void Heapify(int i, int n);
        void HeapSort();
        friend istream& operator>>(istream &in, Array &other);
        friend ostream& operator<<(ostream &out, Array &other); 

};

#endif