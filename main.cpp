/*
* main.cpp
* Author : Luca Muscarnera
* Description : check se é tutto ok
*/

#include <iostream>
#include "Serial/Vectorial/VectorialModule.h"
#include "Serial/Optimizer/OptimizerModule.h"
#include "Serial/Visualization/Cpp/Visualization.h"


#include <string>
#include <fstream>

double f(std::vector<double> x) {
	double nucleo_1 = 0;
	double nucleo_2 = 0;
	for(int i = 0 ; i < x.size();i++)
	{
		nucleo_1 += x[i] * x[i];
		nucleo_2 += ( x[i] - i - 1. ) * ( x[i] - i - 1.) ;
	}
	return exp( - nucleo_1) + 5 * exp( - 3 * nucleo_2) + 2 * sin(10 * nucleo_1) * sin(10 * nucleo_1);
}


int main() {
	std::clog << "Valore iniziale x : " <<std::endl;
	Vector x(std::vector<double> {-4.,-4.} );
	x.show();
	std::clog << "Ottimizziamo la funzione f che ha minimo globale in (1,2)" << std::endl;
	double T = 4.0;
	
	Optimizer<Vector> O( x,
							 f,
							 T,
							 1000);
	O.addCallback(callbacks::writeCallback("vectorOpt.csv"));
	O.addCallback(callbacks::bestResultCallback<Vector>("miglior vettore.csv"));
	O(1000);
	O.status.write();
	x.show();
}