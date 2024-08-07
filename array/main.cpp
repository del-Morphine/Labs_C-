#include <iostream>
#include <ctime>
#include "Array.h"

int main(int argc, char const *argv[])
{
    using namespace std;

    int size;
    cin >> size;

    Array a = Array(size, 0, size);
    Array b = a;
    Array c = a;

    double start, finish;
    start = clock();
    a.HeapSort();
    finish = clock();
    cout << "HeapSort of " << size << " elements array: " << (finish - start) << endl; 

    start = clock();
    b.ShellSort();
    finish = clock();
    cout << "ShellSort of " << size << " elements array: " << (finish - start) << endl;

    start = clock();
    c.QuickSort(0, c.Size() - 1);
    finish = clock();
    cout << "QuickSort of " << size << " elements array: " << (finish - start) << endl;    

    return 0;
}
