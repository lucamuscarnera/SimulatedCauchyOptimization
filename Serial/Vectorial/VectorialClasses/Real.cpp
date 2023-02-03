//Real.cpp
#include "Real.h"

void Real::buildCanonicalNeighbourhood (int n,double t) {					// assemblaggio del vicinato canonico
			canonicalNeighbourhood.flush();	
			canonicalNeighbourhood.init(n);
			std::default_random_engine generator;
			std::normal_distribution<double> distribution(0.0,1.0);
			for(int i = 0; i < n;i++)
			{
				Real value = distribution(generator) * sqrt(t);
				canonicalNeighbourhood[i] = (value);
			}
	}

Neighbourhood<Real> Real::neighbourhood() const {									// ottenimento del vicinato	
			int n = canonicalNeighbourhood.size();
			std::vector<Real> ret(n); 
			for(int i = 0; i < n;i++)
			{
				double value = data + canonicalNeighbourhood[i];
				ret[i] = value;
			}
			return ret;
	}
	
Real Real::generalizedGradient(std::function<double(Real)> f) {				// calcolo del gradiente generalizzato
		auto Y  = neighbourhood();
		auto fY = Y.apply(f);
		double ret = 0.;
		double weightMean = 0.;
		#pragma omp declare reduction( sum : double : omp_out = omp_in + omp_out )
		#pragma omp parallel for num_threads( optimizationContext-> getThreads() ) reduction(sum:weightMean) reduction(sum:ret) default(none) shared(fY,Y,data)
		for(int i = 0 ; i < fY.size();i++)
		{
			double w 			= optimizationContext -> enhancingFunction(fY[i]);						// TODO COLLEGARE LA FUNZIONE DI ASSEGNAZIONE DEI PESI AL CONTESTO DI OTTIMIZZAZIONE
			weightMean 			= weightMean + w; //( i * weightMean + w ) / ( i + 1 );
			double to_add      	=  fY[i] * w * (  Y[i] - data ); 
			ret					= ret + to_add; //(ret * i + to_add)/(i + 1.);
		}
		return ret * ( 1. / weightMean );	
	}


/*** operatori ****/
	
	bool Real::operator < (const Real & other) const {
		return data < other.data;
	}
	
	Real Real::operator * (double other) const
	{
		Real ret(data * other);
		
		return ret;
	}
	
	Real & Real::operator += (Real & other) {
		data += other.data;
		return *this;
	}

	Real & Real::operator *= (double other) {
		data *= other;
		return *this;
	}
	
	Real::operator double() {										// compatibilitá con l'oggetto wrapped
		return data;
	}

/*** visualizzazione ***/

	void Real::show() {
		std::cout << data << std::endl;
	}
	
	
Neighbourhood<Real> Real::canonicalNeighbourhood;					// vicinato canonico