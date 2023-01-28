/*
*  	Vector.h
*	Author:	Luca Muscarnera
*	Descr : Contiene la classe vector, che definisce il rappresentate della classe di equivalenza tra insiemi appartenenti alla collezione Vectorial.	
*			Ogni oggetto di tipo vectorial puó essere identificato con un oggetto di tipo Vector. 
*			Il tipo Vector implementa l'operazione di somma e di prodotto per scalare.
*			Contiene inoltre la classe Real, che costruisce un wrapper per la gestione dei double.
*/

#pragma once
#include <iostream>
#include <vector>




class Combination {
	public:
	
/***Costruttori***************************************************************************************************************/

	Combination() : data() {};
	Combination(int id, int size) : data() {
		std::vector<int> ret(size);
		for(int i = 0 ; i < size;i++)
		{
			
		}
	};
	Combination(std::vector<int> input) : data(input) {};	// non si puo usare vector bool!
	
/***Storage*******************************************************************************************************************/

	std::vector<int> data;

/***gestione neighbourhoods**************************************************************************************************/

	Neighbourhood<Combination> canonicalNeighbourhood;					// vicinato canonico
	void buildCanonicalNeighbourhood (int n) {							// assemblaggio del vicinato canonico
			std::default_random_engine generator;
			std::normal_distribution<double> distribution(0.0,1.0);
			
			for(int i = 0; i < n;i++)
			{
				int value = distribution(generator);
								
				canonicalNeighbourhood.push_back(value);
			}
	}
	
	Neighbourhood<Real> neighbourhood(double t) {				//	
			int n = canonicalNeighbourhood.size();
			std::vector<Real> ret(n);
			for(int i = 0; i < n;i++)
			{
				double value = data + canonicalNeighbourhood[i] * sqrt(t);
				ret[i] = value;
			}
			return ret;
	}
	
/***Gradiente Generalizzato - direzione privilegiata*************************************************************************/

	Real generalizedGradient(std::function<double(Real)> f, double t) {
		auto Y  = neighbourhood(t);
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
	
	Real operator * (double other)
	{
		Real ret(data * other);
		
		return ret;
	}
	
	Real & operator += (Real & other) {
		data += other.data;
		return *this;
	}
	
	operator double() {								// compatibilitá con l'oggetto wrapped
		return data;
	}
	
/***Metodi specializzati*****************************************************************************************************/	
	
	
	int size() {
		return data.size();
	}
	
	void show() {
		std::cout << data << std::endl;
	}
	
/***Contesto di ottimizzazione************************************************************************************************/

	Optimizer<Real> * optimizationContext;							// contesto di ottimizzazione
	
};


/* OLD CODE ********************************************************
class Real
{
	public:
		Real() : data(0) {};
		Real(double input) : data(input) {};
		
		Real operator + (Real & other) {
			double newData = data + other.data;
			return Real(newData);
		}

		Real operator * (double other) {
			double newData = data + other;
			return Real(newData);
		}
		
		void show() {
			std::cout << data << std::endl;
		}
		static constexpr bool scalar = true;
		operator double() {
			return data;
		}
		
		// generatore delle distribuzioni : ad ogni tipo Vectorial é associata una distribuzione 
		//  da utilizzarsi per l'approssimazione del rilassamento
		//

		std::vector<Real> neighborhood(int n,double t = 1.) {
			return Standard<Real>(n, t);
		}
	private:
		double data;
};

class Vector
{
	public:
		Vector() : data(NULL) {};
		Vector(std::vector<double> input) : data(input) {};			// costruttore 
		
		Vector operator + (Vector & other) {
			std::vector<double> newData(data.size());
			for(int i = 0 ; i < data.size();i++)
				newData[i] = data[i] + other.data[i];
			return Vector(newData);
		};
		
		Vector operator * (double other)   {
			std::vector<double> newData(data.size());
			for(int i = 0 ; i < data.size();i++)
				newData[i] = data[i] * other;
			return Vector(newData);
		};
		void show() { 
			for(int i = 0 ;  i < data.size(); i++ )
			{
				std::cout << "\t" << data[i] << std::endl;
			}
			std::cout<<std::endl;
		}
		
		static constexpr bool scalar = false;
		
		// generatore delle distribuzioni : ad ogni tipo Vectorial é associata una distribuzione 
		//  da utilizzarsi per l'approssimazione del rilassamento
		//
		
		std::vector<Vector> neighborhood(int n, double t = 1.0) {
			return Standard<Vector>(n, data.size(),t);
		}
		
		operator std::vector<double>() {
			return data;
		}
	private:
		std::vector<double> data;
};

Vector operator *(double other, Vector & a) {
	return a * other;
}
*/