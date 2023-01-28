/**
~ GreedyBenchmark.cpp
Author : Luca Muscarnera
Description : Confronta il classico algoritmo di ottimizzazione greedy per il
problema del Travelling Salesman con il nuovo algoritmo greedy ottenuto
partendo dalla Simulated Cauchy Optimization
*/


#include <iostream>
#include "Serial/Vectorial/VectorialModule.h"
#include "Serial/Optimizer/OptimizerModule.h"
#include "Serial/Visualization/Cpp/Visualization.h"
#include "Serial/Profiler/Profiler.h"
#include "Serial/OutputManager/CSV.h"
#include <omp.h>

std::function<double(Permutation)> travelling_salesman(std::vector<std::vector<double>> X)
{
	std::function<double(Permutation)> ret = [=](Permutation P) {
		auto newX = P.apply(X);
		double retval = 0;
		for(int i = 0 ; i < newX.size() - 1;i++)
		{
			double norma = 0;
			for(int j = 0 ; j < newX[i].size();j++)
			{
				norma += (newX[i + 1][j] - newX[i][j])*(newX[i + 1][j] - newX[i][j]);
			}
			norma = (norma);
			retval += norma;
		}
		return sqrt(retval);
	};
	return ret; 
}



// greedy optimization

std::vector<std::vector<double>> initialize_points(int n,int m)
{
	std::vector<std::vector<double>> X(n, std::vector<double>(m));
	std::default_random_engine generator;
	std::normal_distribution<double> distribution(0.0,1.0);

	for(int i = 0;i < n;i++) {
		for(int j = 0;j < m;j++)
			X[i][j] = distribution(generator);
	}
	return X;
}


int greedy(std::vector<std::vector<double>> X) {
	std::vector<double> losses;
	
	auto r =  travelling_salesman(X);
	Permutation x(X.size());	// inizializzo una permutazione identitá
	
	for(int it = 0 ; it < 100;it++)
	{
	double min;
	Permutation new_x(X.size());
	for(int i  = 0 ; i < X.size() - 1; i++)	{
			for(int j = i + 1 ; j < X.size(); j++)	{	
				Permutation P(X.size());
				P = P.data.swap(i,j);	// perturbazione dell'identitá
				P += x;
				
				double curr = r(P);
				if ( i > 0) {
					if(curr <= min)
					{
						new_x = P;
						min = curr;
					}
				} else {
					min = curr;
					new_x = P;
				}
			}
		}
		x = new_x;
		
		losses.push_back(min);
		
		std::clog << r(x) << std::endl;
		
	}

	return 1;
}


int main() {
	
	int n = 20;
	int m = 2;
	int T = 10;
	int precision = 300;
	
	std::clog << "Soluzione Greedy : " << std::endl;
	
	Permutation P(n);
	
	auto X = initialize_points(n,m);
	auto r = travelling_salesman(X);
	
	greedy(X);
	
	std::clog  << std::endl << "Soluzione Greedy con Simulated Cauchy Optimization : " << std::endl;
	
	Optimizer<Permutation> O(P, 
							 travelling_salesman(X), 
							 T,
							 precision
							 );
	

	
	O.addCallback(permutationCallback);	
	
	O(100);	
}