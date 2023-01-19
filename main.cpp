/*
* main.cpp
* Author : Luca Muscarnera
* Description : check se é tutto ok
*/

#include <iostream>
#include "Serial/Vectorial/VectorialModule.h"
#include "Serial/Optimizer/OptimizerModule.h"

double f(std::vector<double> x) {
	double nucleo_1 = 0;
	double nucleo_2 = 0;
	for(int i = 0 ; i < x.size();i++)
	{
		nucleo_1 += x[i] * x[i];
		nucleo_2 += ( x[i] - 3.) * ( x[i] - 3.) ;
	}
	return exp( - nucleo_1) + 3 * exp( - 10 * nucleo_2) + 2 * sin(nucleo_1) * sin(nucleo_1);
}

double g(double x)
{
	return sin(x) * sin(x) + 3 * exp( - x*x );
}

int main() {
	//Vector x(std::vector<double>{-5.,-2.});
	//Optimizer<Vector> O(x, f, 10., 10000);
	//O(10000);
	
	Real x(-5.);
	Optimizer<Real> O(x,g,2.,1000);
	O(1000);
	
	//x.show();
	//Real y(5.4);
	//y.show();
	//std::cout << "Test!"  << f(x) << std::endl;	
}