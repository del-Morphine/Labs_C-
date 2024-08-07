#include <cstdlib>
#include <iostream>
#include <ctime>
#include "Array.h"

Array::Array(const int* array, int size)
{
    this->size = size;
    arr = new int[size];
    for(int i = 0; i < size; ++i)
        arr[i] = array[i];
}

Array::Array(int size, int order, int range)
{
    this->size = size;
    arr = new int[size];
    if(order == 1)
    {
        for(int i = 0; i < size; ++i)
            arr[i] = i;
        return;
    }
    else if(order == 2)
    {
        for(int i = size - 1; i >= 0; --i)
            arr[i] = i;
        return;
    }    
    for(int i = 0; i < size; ++i)
        arr[i] = rand() % range;
}

Array::~Array()
{
    if(arr)
        delete[] arr;
}

Array::Array(const Array& other)
{
    size = other.size;
    arr = new int[size];
    for(int i = 0; i < size; ++i)
        arr[i] = other.arr[i];
}

Array& Array::operator=(const Array& other)
{
    if(this == &other)
        return *this;
    if(arr)
        delete[] arr;
    size = other.size;
    arr = new int[size];
    for(int i = 0; i < size; ++i)
        arr[i] = other.arr[i];
    return *this;
}

int& Array::operator[](int index)
{
    if(index < 0 || index > size - 1)
        exit(EXIT_FAILURE);
    return arr[index];
}

bool Array::Test()
{
    for(int i = 0; i < size - 1; ++i)
        if(arr[i] > arr[i + 1])
            return false;
    return true;
}

bool Array::operator==(const Array& other)
{
    if(size != other.size)
        return false;
    if(this == &other)
        return true;
    if(size != other.size)
        return false;
    for(int i = 0; i < size; ++i)
        if(arr[i] != other.arr[i])
            return false;
    return true;
}

void Array::ShellSort()
{   
    int tmp, j;
    for(int step = size/2; step > 0; step /= 2)
        for(int i = step; i < size; ++i)
        {
            tmp = arr[i];
            for(j = i - step; j >= 0 && arr[j] > tmp; j -= step)
                arr[j + step] = arr[j];
            arr[j + step] = tmp;
        }
}

void Array::RadixSort(int left, int right, int k)
{
    if(left >= right || k < 0)
        return;

    int i = left;
    int j = right;
    int tmp;
    while(i <= j)
    {
        while(i <= j && !(arr[i] & (1<<k)))
            ++i;
        while(i <= j && (arr[j] & (1<<k)))
            --j;
        if(i < j)
        {
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            ++i;
            --j;
        }
    }

    this->RadixSort(left, j, k - 1);
    this->RadixSort(i, right, k - 1);
}

void Array::QuickSort(int left, int right)
{
    if(left >= right)
        return;
    int i = left;
    int j = right;
    int mid = arr[(i + j) / 2];
    int tmp;
    while(i <= j)
    {
        while(arr[i] < mid)
            ++i;
        while(arr[j] > mid)
            --j;
        if(i <= j)
        {
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            ++i, --j;
        }
    }
    this->QuickSort(left, j);
    this->QuickSort(i, right);
}

void Array::Heapify(int i, int n)
{
    int left;
    int right;
    int curr = arr[i];
    int index = i;

    for(;;)
    {
        left = 2*index + 1;
        right = left + 1;
        if(left < n && arr[left] > curr)
            index = left;
        if(right < n && arr[right] > arr[index])
            index = right;
        if(index == i)
            break;
        arr[i] = arr[index];
        arr[index] = curr;
        i = index;
    }
    

}

void Array::HeapSort()
{
    for(int i = (size / 2) - 1; i >= 0; --i)
        this->Heapify(i, size);
    int tmp;
    for(int i = size - 1; i > 0; --i)
    {
        tmp = arr[0];
        arr[0] = arr[i];
        arr[i] = tmp;
        this->Heapify(0, i);
    }
}

istream& operator>>(istream &in, Array &other)
{
    int size;
    cout << "Size: ";
    cin >> size;
    
    int *arr = new int[size];
    for(int i = 0; i < size; ++i)
        in >> arr[i];
    if(other.arr)
        delete[] arr;
    other.arr = arr;
    return in;
}

ostream& operator<<(ostream &out, Array &other)
{
    for(int i = 0; i < other.size; ++i)
        out << other.arr[i] << ' ';
    return out;
}