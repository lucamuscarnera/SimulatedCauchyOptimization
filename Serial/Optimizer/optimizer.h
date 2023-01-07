/*
* Optimizer.h
* Luca Muscarnera ~ 2023
*/
#pragma once
#include <functional>

class OptimizerStatus
{
	int status;
};

template <class T>
class Optimizer
{
public:
	Optimizer( std::function<double(T)> fgrad) {
		this->fgrad = fgrad;
	}
	T operator ()(T x, int maxiter = 1000, double alpha = 0.1) {
		for (int i = 0; i < maxiter; i++)
		{
			x = x + alpha * fgrad(x);
			/* debug */
			if( (i == 0) ||  (  (int) (((double) i ) / (maxiter) * 10) < (int) (((double) i + 1 ) / (maxiter) * 10) ) )
				std::cout << "iteration #" << i << "\t" << x << std::endl;
		}
		return x;
	}
private:
	std::function<double(T)> fgrad;
	OptimizerStatus status;
};