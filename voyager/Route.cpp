#include "Route.h"
using namespace std;

Route::Route(int towns)
{
	n = towns;
	r = new int [n];
	// Тождественная перестановка
	for(int i = 0; i < n; i++)
		r[i] = i;
}

Route::Route(const Route& other_route)
{
	n = other_route.n;
	r = new int[n];
	for(int i = 0; i < n; i++)
		r[i] = other_route.r[i];
}

Route::~Route()
{
	if(r != nullptr)
	{
		delete[] r;
		r = nullptr;
	}
}

bool Route::Next_route()
{	
	int i = n - 2;
	if(i <= 0)
		return false;
	// Ищем пару (P[i] < P[i+1])
	for(i; i >= 0; i--)
	{
		if(i == 0)
			return false;
		if(r[i] < r[i+1])
			break;
	}

	// Ищем максимальный j: P[i] < P[j]
	int j = n - 1;
	for(j; j > i; j--)
	{
		if(r[i] < r[j])
			break;
	}

	// Меняем местами
	r[i] = r[i] ^ r[j];
	r[j] = r[j] ^ r[i];
	r[i] = r[i] ^ r[j];

	// Реверс
	int left = i + 1;
	int right = n - 1;
	

	while(left < right)
	{
		r[left] = r[left] ^ r[right];
		r[right] = r[right] ^ r[left];
		r[left] = r[left] ^ r[right];

		left ++;
		right --;
	}
	return true;
}

int Route::Route_price(int **matrix)
{
	int price = 0;
	int i = 0;
	for(i; i < n - 1; i++)
		price += matrix[r[i]][r[i+1]];
	price += matrix[r[i]][0];
	return price;
}


ostream &operator<< (ostream &out, const Route &another)
{
	for(int i = 0; i < another.n; i++)
		out << another.r[i] + 1 << "->";
	out << '1' ;	
	return out;
}

Route &Route::operator= (const Route &other)
{
	if(this == &other)
		return *this;
	
	n = other.n;
	if(r != nullptr)
		delete[] r;

	r = new int[other.n];
	for(int i = 0; i < n; i++)
		r[i] = other.r[i];	 

	return *this;
}