/*
*  	SimpleGaussian.h
*	Author:	Luca Muscarnera
*	Gestisce la generazione di una distribuzione normale standard mono/multivariata.
*	
*/
#pragma once
#include <random>
#include "../../Vectorial/Vectorial/Vectorial.h"
#include "../../Vectorial/Vector/Vector.h"

template<Vectorial T>
std::vector<T> Standard(int n, double t, int m = 1)
{
	/*
	* Genera un campionamento di n individui da una distribuzione normale
	*/
	std::default_random_engine generator;
	std::normal_distribution<double> distribution(0.0,t);
	std::vector<T> result(n);	// alloca la memoria necessaria
	if constexpr(T::scalar)
	{
		for(int i = 0 ; i < n;i++) {
			double value = distribution(generator);	// creo il valore random
			result[i] = T(value);
		}
	} else {
		for(int i = 0 ; i < n;i++) {
			std::vector<double> row(m);
			for(int j = 0 ; j < m;j++) {
				double value = distribution(generator);	// creo il valore random
				row[j] = value;
			}
			result[i] = T(row);
		}
	}
	return result;
}
