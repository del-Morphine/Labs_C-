#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Route.h"

int **create_matrix(int n);
void print_matrix(int **matrix, int n);

int main(int argc, char const *argv[])
{
    using namespace std;

    int towns;
    cout << "Count of towns: ";
    cin >> towns;

    if(towns <= 0)
    {
        cout << "There is no any towns\n";
        return 1;
    }


    cout << endl;
    Route way(towns);
    Route cmp_way(towns);
    // Генерируем случайную матрицу стоимости 
    int **matrix = create_matrix(towns);
    print_matrix(matrix, towns);
   
    // Генерируем следующие последовательности и вычисляем цены
    int price;
    int cmp_price = way.Route_price(matrix);
    do
    {   
        price = way.Route_price(matrix);
        cout << "Price of the "<< way << " route is " << price << endl;
        if(cmp_price > price)
        {
            cmp_price = price;
            cmp_way = way;
        }
    }
    while(way.Next_route());

    cout << "\nMinimal " << cmp_way << " " << cmp_price << endl;

    for(int i = 0; i < towns; i++)
        delete [] matrix[i];
    delete [] matrix;

    return 0;
}

int **create_matrix(int n)
{
    srand(time(NULL));
    int **square_matrix = new int* [n];
    for(int i = 0; i < n; i++)
        square_matrix[i] = new int [n];
    
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            if(i == j)
                square_matrix[i][j] = 0;
            else
                square_matrix[i][j] = rand() % 10 + 1;
                
        }
    }

    return square_matrix; 
}

void print_matrix(int **matrix, int n)
{
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            printf("%2d ", matrix[i][j]);
        }
        cout << endl;
    }    
    cout << endl;    
}