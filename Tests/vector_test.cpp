/*
Real test:
Massimizziamo la funzione 
								f(x) = 3 * exp( - x^2 ) + sin(x)^2
*/
#include "../getThreadsFromQuery.h"

#include <iostream>
#include "../Serial/Vectorial/VectorialModule.h"
#include "../Serial/Optimizer/OptimizerModule.h"
#include "../Serial/Profiler/Profiler.h"
#include "../Serial/OutputManager/CSV.h"
#include "../Serial/Optimizer/OptimizerScheduler/callbacks.h"
#include <omp.h>

double f(std::vector<double> x) {
	double normasq = 0;

	for(int i = 0 ; i < x.size();i++)
		normasq += (x[i] - 1.)*(x[i] - 1.);
	return 3 * exp( - normasq ) + sin( normasq )*sin( normasq );
}

int main(int argc, char * argv[]) {
		Vector x(std::vector<double> (5,2) );
		double T = 3;
		int precision = 100000;
		int threads = getThreadsFromQuery(argc,argv);

		Optimizer<Vector> 		O(x, 
								 f, 
								 T,
								 precision
								 );
		O.addCallback(callbacks::simpleCallback);
		//O.addCallback(callbacks::backwardInTime<Real>(10));
		//O.addCallback(callbacks::storeLossesCallback("loss.csv"));
		//O.addCallback(callbacks::writeCallback("posizioni_x.csv"));
		//O.addCallback(callbacks::relaxedEvaluationCallback("misurazioni.csv", -10,10,100) );
		O.setThreads(threads);
		O(1000);
		O.getStatus().write();
		x.show();
}	