#ifndef ROUTE_H
#define ROUTE_H
#include <iostream>
using namespace std;
class Route
{

private:
	int *r;
	int n;
public:
	Route(int towns = 0);
	Route(const Route&);
	Route& operator=(const Route&); 
	~Route();

	bool Next_route();
	int Route_price(int **matrix);
	
	friend ostream &operator<< (ostream &out, const Route &another);
};

#endif