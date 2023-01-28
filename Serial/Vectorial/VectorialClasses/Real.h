/*
*  	Vector.h
*	Author:	Luca Muscarnera
*	Descr : Contiene l'oggetto Vector, un elemento della famiglia di classi Vectorial adatto per rappresentare vettori.
*/

#pragma once
#include <iostream>
#include <vector>




class Real {
	public:
/***Zero***********************************************************************************************************************/
	
	static Real zero(Real & x)  {
		return Real(0);
	};
	
/***Costruttori***************************************************************************************************************/

	Real() : data(0) {};
	Real(double input) : data(input) {};
	
/***Storage*******************************************************************************************************************/

	double data;

/***gestione neighbourhoods**************************************************************************************************/

	Neighbourhood<Real> canonicalNeighbourhood;					// vicinato canonico
	void buildCanonicalNeighbourhood (int n,double t) {					// assemblaggio del vicinato canonico
			std::default_random_engine generator;
			std::normal_distribution<double> distribution(0.0,1.0);
			for(int i = 0; i < n;i++)
			{
				const  Real & value = distribution(generator) * sqrt(t);
				canonicalNeighbourhood.push_back(value);
			}
	}
	
	Neighbourhood<Real> neighbourhood() {				//	
			int n = canonicalNeighbourhood.size();
			std::vector<Real> ret(n);
			for(int i = 0; i < n;i++)
			{
				double value = data + canonicalNeighbourhood[i];
				ret[i] = value;
			}
			return ret;
	}
	
/***Gradiente Generalizzato - direzione privilegiata*************************************************************************/

	Real generalizedGradient(std::function<double(Real)> f) {
		auto Y  = neighbourhood();
		auto fY = Y.apply(f);
		double ret = 0.;
		double weightMean = 0.;
		for(int i = 0 ; i < fY.size();i++)
		{
			double w 			= 1 + fY[i]*fY[i];
			weightMean 			= ( i * weightMean + w ) / ( i + 1 );
			double to_add      	=  fY[i] * w * (  Y[i] - data ); 
			ret					= (ret * i + to_add)/(i + 1.);
		}
		return ret * ( 1. / weightMean );	
	}
	
/***Operatori****************************************************************************************************************/
	bool operator < (const Real & other) const {
		return data < other.data;
	}
	
	Real operator * (double other)
	{
		Real ret(data * other);
		
		return ret;
	}
	
	Real & operator += (Real & other) {
		data += other.data;
		return *this;
	}

	Real & operator *= (double other) {
		data *= other;
		return *this;
	}
	
	operator double() {								// compatibilitá con l'oggetto wrapped
		return data;
	}
	
/***Metodi specializzati*****************************************************************************************************/	
	

	
	
	void show() {
		std::cout << data << std::endl;
	}
	
/***Contesto di ottimizzazione************************************************************************************************/

	Optimizer<Real> * optimizationContext;							// contesto di ottimizzazione

/*** Procedura di ottimizzazione ****/

static void improve(Real & x , Real & increment, Real & mom) {
			mom += increment;			
			x += mom;					
}


};

