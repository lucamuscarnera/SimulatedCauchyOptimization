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
#include <random>
#include <iomanip>

#include "../../../Serial/Optimizer/OptimizerModule.h"


class Vector {
	public:
	
/***Costruttori***************************************************************************************************************/

	Vector() : data() {};
	Vector(int i) : data(i) {};
	Vector(std::vector<double> input) : data(input) {};
/***Storage*******************************************************************************************************************/

	std::vector<double> data;
	
/***Gestione neighbourhoods**************************************************************************************************/

	Neighbourhood<Vector> canonicalNeighbourhood;				// vicinato canonico
	void buildCanonicalNeighbourhood (int n) {					// assemblaggio del vicinato canonico
			std::default_random_engine generator;
			std::normal_distribution<double> distribution(0.0,1.0);
			for(int i = 0; i < n;i++)
			{
				std::vector<double> row(size());
				for(int j = 0 ; j < size();j++) {
					row[j] = distribution(generator);
				}
				canonicalNeighbourhood.push_back(row);
			}
			
	}
	
	Neighbourhood<Vector> neighbourhood(double t) {				//	
			int n = canonicalNeighbourhood.size();
			std::vector<Vector> ret(n);
			for(int i = 0; i < n;i++)
			{
				std::vector<double> row(size());
				for(int j = 0 ; j < size();j++) {
					row[j] = data[j] + canonicalNeighbourhood[i].data[j] * sqrt(t);
					//printf( "%.4f\t",row[j]);
				}
				ret[i] = (row);
				//std::cout << std::endl;
			}
			return ret;
	}
	
/***Gradiente Generalizzato - direzione privilegiata*************************************************************************/

	Vector generalizedGradient(std::function<double(Vector)> f, double t) {
		auto Y  = neighbourhood(t);
		auto fY = Y.apply(f);
		Vector ret(data.size());
		double weightMean = 0.;
		for(int i = 0 ; i < fY.size();i++)
		{
			double w = (1 + pow(fY[i],15));
			weightMean = ( i * weightMean + w ) / ( i + 1 );
			for(int j = 0 ; j < data.size();j++)
			{
				double to_add       =  fY[i] * w * (  Y[i].data[j] - data[j]  ); 
				ret.data[j] 		= (ret.data[j] * i + to_add)/(i + 1.);
			}
		}
		return ret * ( 1. / weightMean );
		
	}
	
/***Operatori****************************************************************************************************************/
	
	Vector operator * (double other)
	{
		Vector ret(data);
		for(int j = 0 ; j < data.size();j++)
			{
				ret.data[j] *= other;
			}
		return ret;
	}
	
	Vector & operator += (Vector & other) {
		for(int j = 0 ; j < data.size();j++)
			{
				data[j] += other.data[j];
			}
		return *this;
	}
	
	operator std::vector<double>() {			// compatibilitá con l'oggetto wrapped
		return data;
	}
	
/***Metodi specializzati*****************************************************************************************************/	
	
	int size() {
		return data.size();
	}
	
	
	void show() {
		std::cout << "->\t";
		std::cout << std::setprecision(4) ;
		for(int i = 0 ; i < size();i++)
			std::cout << data[i] << std::setw(15);
		std::cout << std::endl;
	}
	
	
/***Contesto di ottimizzazione************************************************************************************************/

	Optimizer<Vector> * optimizationContext;							// contesto di ottimizzazione
	
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