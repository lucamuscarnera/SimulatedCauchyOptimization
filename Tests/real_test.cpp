/*
Real test:
Massimizziamo la funzione 
								f(x) = 3 * exp( - x^2 ) + sin(x)^2
*/

#include <iostream>
#include "../Serial/Vectorial/VectorialModule.h"
#include "../Serial/Optimizer/OptimizerModule.h"
#include "../Serial/Profiler/Profiler.h"
#include "../Serial/OutputManager/CSV.h"
#include "../Serial/Optimizer/OptimizerScheduler/callbacks.h"
#include "../getThreadsFromQuery.h"

#include <omp.h>

double f(double _x) {
	double x = _x - 2.;
	return 3 * exp( - x* x ) + sin( x ) * sin( x );
}

int main(int argc, char * argv[]) {
		Real x = 7.;
		double T = 3;
		int precision = 10000;
		int threads = getThreadsFromQuery(argc,argv);
		Optimizer<Real> 		O(x, 
								 f, 
								 T,
								 precision
								 );
		O.addCallback(callbacks::simpleCallback);
		O.addCallback(callbacks::backwardInTime<Real>(10));
		O.addCallback(callbacks::storeLossesCallback("loss.csv"));
		O.addCallback(callbacks::writeCallback("posizioni_x.csv"));
		O.addCallback(callbacks::relaxedEvaluationCallback("misurazioni.csv", -10,10,100) );
		O.setThreads(threads);
		O(1000);
		O.getStatus().write();
		std:: cout << x;
}	