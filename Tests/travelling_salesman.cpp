/**
~ GreedyBenchmark.cpp
Author : Luca Muscarnera
Description : Confronta il classico algoritmo di ottimizzazione greedy per il
problema del Travelling Salesman con il nuovo algoritmo greedy ottenuto
partendo dalla Simulated Cauchy Optimization
*/


#include <iostream>
#include "../Serial/Vectorial/VectorialModule.h"
#include "../Serial/Optimizer/OptimizerModule.h"
#include "../Serial/Profiler/Profiler.h"
#include "../Serial/OutputManager/CSV.h"
#include "../Serial/Optimizer/OptimizerScheduler/callbacks.h"
#include "../getThreadsFromQuery.h"

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
		double norma = 0;
		for(int j = 0 ; j < newX[0].size();j++)
			norma += (newX[ X.size() - 1][j] - newX[0][j])*(newX[X.size() - 1][j] - newX[0][j]);
		retval+= norma;
		
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
		//for(int j = 0;j < m;j++)
			X[i][0] = cos( (static_cast<double>(i) / n) * 2 * 3.1459 ); // distribution(generator);
			X[i][1] = sin( (static_cast<double>(i) / n) * 2 * 3.1459 ); // distribution(generator);
	}
	
	Permutation r(n);
	std::default_random_engine gen;
	std::uniform_int_distribution<int> distr(0,n - 1);
  
	// mescolando a caso...
	for(int i = 0 ;i  < 1000;i++)
	{
		r.getData().inplaceSwap( distr(gen), distr(gen) );
	}
	
	return r.apply(X);
}


int greedy(std::vector<std::vector<double>> X) {
	std::vector<double> losses;
	
	auto r =  travelling_salesman(X);
	Permutation x(X.size());	// inizializzo una permutazione identitá
	std::vector<std::vector<int>>  perms;
	CSV perms_greedy("permutazioni_greedy.csv");
	CSV losses_greedy("loss_permutazioni_greedy.csv");
	
	for(int it = 0 ; it < 100;it++)
	{
	double min;
	Permutation new_x(X.size());
	for(int i  = 0 ; i < X.size() - 1; i++)	{
			for(int j = i + 1 ; j < X.size(); j++)	{	
				Permutation P(X.size());
				P.getData().inplaceSwap(i,j);	// perturbazione dell'identitá
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
		perms.push_back(x.flatten());
		losses.push_back(min);
		
		std::clog << r(x) << std::endl;
	}
	for(auto p : perms)
		perms_greedy.write(p);
	losses_greedy.write(losses);

	return 1;
}



int main(int argc,char * argv[]) {

	int n = 15;	// good value : 10 or 15
	int m = 2;
	int T = 30;
	int precision = 1000;	// good value 500 or 1000
	int threads = getThreadsFromQuery(argc,argv);

	std::clog << "Soluzione Greedy : " << std::endl;
	

	
	auto X = initialize_points(n,m);
	auto r = travelling_salesman(X);
	
	CSV X_csv("X.csv");
	for( auto & x : X)
		X_csv.write(x);
	
	greedy(X);
	
	

	Permutation P(n);
	Profiler Prof("Greedy stochastic optimization");	
	Prof.init("SOCGreedy profiling.log");
	std::clog  << std::endl << "Soluzione Greedy con Simulated Cauchy Optimization : " << std::endl;
	
	int pluto;	
	Optimizer<Permutation> O(P, 
							 travelling_salesman(X), 
							 T,
							 precision
							 );
							 
				O.addCallback(callbacks::permutationCallback);	
				O.addCallback(callbacks::writeCallback("permutazioni.csv"));
				O.addCallback(callbacks::storeLossesCallback("loss_permutazioni.csv"));
				O.setThreads(threads);
	//O.addCallback(callbacks::bestResultCallback<Permutation>("migliore.csv"));
	O(500);	
	O.getStatus().write();
	

	
}