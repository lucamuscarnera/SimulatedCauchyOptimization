#include <iostream>
#include <math.h>
#include <functional>
#include "Serial/Diffusion/diffusion.h"
#include "Serial/Optimizer/optimizer.h"
#include "Serial/Utilities/utilities.h"
#include <string>

double f(double x)
{
		std::function<int(double)> counter = [=](int x) {
			double ret = 0.;
			for(int i = 1 ; i < 6;i++)
			{
				ret += x % (2 + i);
			}
			return ret;
		};
		return (x > 0  ) * 
			   (x < 100) * 
			   counter( (int) x ) ;
}


int main(int argc, char ** argv) {
	// command line instructions
	double x0 = 0.;
	double maxiter = 100;
	double r = 3.0;
	double p = 1.0;
	int n = 3000;
	for (int i = 0; i < argc; ++i) {
        std::string curr = argv[i];
		if( curr == "-showfunction" ) { 
			
			for(int j = 0 ; j < 100;j++)
				std::cout << j << "\t" << f(j) << std::endl; 
			
			return 0;
		}
		if( curr == "-x0" ) { // initial guess
			std::string next = argv[i + 1];
			x0 = std::stod(next);
			std::cout << "starting from " << x0 << std::endl;
		}
		if( curr == "-n" ) { // number of iterations
			std::string next = argv[i + 1];
			maxiter = std::stod(next);
			std::cout << "number of iterations =  " << maxiter << std::endl;
		}
		if( curr == "-r" ) { // relaxing parameter
			std::string next = argv[i + 1];
			r = std::stod(next);
			std::cout << "relaxing parameter =  " << r << std::endl;
		}
		if( curr == "-p" ) { // enhancing parameter
			std::string next = argv[i + 1];
			p = std::stod(next);
			std::cout << "enhancing parameter =  " << p << std::endl;
		}
		if( curr == "-precision" ) { // enhancing parameter
			std::string next = argv[i + 1];
			n = std::stoi(next);
			std::cout << "approximation of expected value, samples =  " << n << std::endl;
		}
	}
	std::cout << "+-------------------------------------------+" << std::endl;
	Optimizer<double> O( relax_grad(f, r, p, n) );
	O(x0, maxiter, 0.01);
	
}