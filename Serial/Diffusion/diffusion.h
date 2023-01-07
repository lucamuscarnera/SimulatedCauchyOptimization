/*
* Diffusion.h
* Luca Muscarnera ~ 2023
*/

#pragma once
#include "../Utilities/utilities.h"
#include <functional>
#include <random>

std::function<double(double)> relax_grad(std::function<double(double)> f, double t, double p = 0., int n = 1000)
{

    std::function<double(double)> ret = [=](double x)
    {
        std::default_random_engine generator;
        std::normal_distribution<double> distribution(0., t*t);

        std::vector<double> X(n);
        for (int i = 0; i < X.size(); i++)
        {
            double noise = distribution(generator);
            double number = x + noise;
            X[i] = number;
        }
		
		
		
		// definition of the weights
		std::vector<double> Weights(n);		
		std::vector<double> Y = map(f, X);
		double maxY           = max(Y);
		
		for (int i = 0; i < X.size(); i++)
        {	
			Weights[i] = exp(- p * log( maxY ))  + 
						 exp(  p * log(f(X[i]) / maxY ) );
		}
		
		
		
		// computation of the approximated derivative
        std::vector<double> A,B,C;
		Y = pairwiseprod( Y , Weights);	    // f(X) * weights

        A = pairwiseprod( Y,  X); 			// f(X) * weights * X		
		B = product( x, Y );				// x * f(X) * weights
		C = sub(A,B);						// f(X) * weights * X	 - x * f(X) * weights
		
        return mean(C) / mean(Weights);
    };
    return ret;
}
