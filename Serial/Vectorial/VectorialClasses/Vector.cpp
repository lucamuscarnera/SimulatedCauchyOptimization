// Vector.cpp
#include "Vector.h"
#include <omp.h>

/*assemblaggio del vicinato canonico */
void Vector::buildCanonicalNeighbourhood (int n, double t) {				
	
			// svuoto il vicinato canonico nel caso fosse stato precedentemente inzializzato
			canonicalNeighbourhood.flush();	
			canonicalNeighbourhood.init(n);
			// inizializzo la distribuzione normale standard per il campionamento
			std::default_random_engine generator;
			std::normal_distribution<double> distribution(0.0,1.0);
			
			#pragma omp parallel for num_threads(optimizationContext->getThreads() ) 
			for(int i = 0; i < n;i++)
			{
				// inizializzo il vettore da inserire nel canonicalNeighbourhood
				std::vector<double> row(size());
				
				// per ogni componente del vettore...
				for(int j = 0 ; j < size();j++) {
					// ...campiono un elemento...
					row[j] = distribution(generator) * sqrt(t);		// remark : N(0,1) * sqrt(T) = N(0,T)
				}
				
				Vector z = row;
				// ... e lo aggiungo al vicinato
				canonicalNeighbourhood[i] = (z);
			}
			
	}
	
/* ottenimento del vicinato */
	Neighbourhood<Vector> Vector::neighbourhood() const {				
			// carico il numero di elementi nel canonicalNeighbourhood
			int n = canonicalNeighbourhood.size();
			
			// carico un vettore di Vectors 
			std::vector<Vector> ret(n);
			
			// per ogni elemento nel canonicalNeighbourhood
			#pragma omp parallel for num_threads(optimizationContext->getThreads() ) 
			for(int i = 0; i < n;i++)
			{
				// ...inizializzo un vettore...
				std::vector<double> row(size());
				for(int j = 0 ; j < size();j++) {
					// inserisco alla j-esima posizione la traslazione del valore corrente sulla compoonente j-esima definita nell'i-esimo vettore del vicinato
					row[j] = data[j] + canonicalNeighbourhood[i].data[j];
				}
				// inserisco nel vicinato
				ret[i] = (row);
			}
			return ret;
	}
	
	
/* gradiente generalizzato */


	Vector Vector::generalizedGradient(std::function<double(Vector)> f) {
		auto Y  = neighbourhood();
		auto fY = Y.apply(f);
		
		Vector ret(data.size());
		double weightMean = 0.;
		
		int N = fY.size();
		
		
		
		for(int j = 0 ; j < data.size();j++) {
			double x = 0;
			#pragma omp parallel num_threads(optimizationContext->getThreads() ) default(none) shared(weightMean,optimizationContext,fY,Y,data,N,j,x)
			{
				#pragma omp declare reduction( sum : double : omp_out = omp_in + omp_out )
				#pragma omp for reduction(sum: x ) reduction(+:weightMean)
				for(int i = 0 ; i < N ;i++)
				{
					double w = optimizationContext -> enhancingFunction(fY[i]);
					weightMean = weightMean + w/N;
					x = x +  fY[i] * w * (  Y[i].data[j] - data[j]  )/N;
				}
			}
			ret.getData()[j] = x;
		}
		//std::clog << clock() << std::endl;
		return ret * ( 1. / weightMean );
		
	}
	
/* output */

	void Vector::show() {
		std::cout << "->\t";
		std::cout << std::setprecision(4) ;
		for(int i = 0 ; i < size();i++)
			std::cout << data[i] << std::setw(15);
		std::cout << std::endl;
	}
	

// operatori

bool Vector::operator < (const Vector & other) const {
		for(int i = 0 ; i < other.size();i++)
		{
			if(data[i] < other.data[i])
			{
				return true;
			}
		}
		return false;
	}

Vector Vector::operator * (double other)
	{
		Vector ret(data);
		for(int j = 0 ; j < data.size();j++)
			{
				ret.data[j] *= other;
			}
		return ret;
	}	
	
Vector Vector::operator + (Vector & other)
	{
		Vector ret(data);
		for(int j = 0 ; j < data.size();j++)
			{
				ret.data[j] += other[j];
			}
		return ret;
	}	
	
Vector & Vector::operator += (Vector & other) {
		for(int j = 0 ; j < data.size();j++)
			{
				data[j] += other.data[j];
			}
		return *this;
	}

Vector & Vector::operator *= (double  other) {
		for(int j = 0 ; j < data.size();j++)
			{
				data[j] *= other;
			}
		return *this;
	}
	
Vector::operator std::vector<double>() {			// compatibilitá con l'oggetto wrapped
		return data;
	}
	
// metodi specializzati

int Vector::size() const {
		return data.size();
	}


// gestione contesto di ottimizzazione

	Optimizer<Vector> * Vector::getOptimizationContext() const {
		return optimizationContext;
	};
	void Vector::setOptimizationContext(Optimizer<Vector> * other) {
		optimizationContext = other;
	};
	
// gestione strategia di improvement


Neighbourhood<Vector> Vector::canonicalNeighbourhood;