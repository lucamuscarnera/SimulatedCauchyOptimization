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
	
/***Zero**********************************************************************************************************************/
	
	static Vector zero(Vector & x)  {
		return Vector(x.size());
	};
	
/***Costruttori***************************************************************************************************************/
	
	Vector() : data() {};
	Vector(int i) : data(i) {};
	Vector(std::vector<double> input) : data(input) {};
	
/***Storage*******************************************************************************************************************/

	std::vector<double> data;
	
/***Gestione neighbourhoods**************************************************************************************************/

	static Neighbourhood<Vector> canonicalNeighbourhood;					// vicinato canonico
	
	void buildCanonicalNeighbourhood (int n, double t) {					// assemblaggio del vicinato canonico
	
			// svuoto il vicinato canonico nel caso fosse stato precedentemente inzializzato
			canonicalNeighbourhood.flush();	
			
			// inizializzo la distribuzione normale standard per il campionamento
			std::default_random_engine generator;
			std::normal_distribution<double> distribution(0.0,1.0);
			
			for(int i = 0; i < n;i++)
			{
				// inizializzo il vettore da inserire nel canonicalNeighbourhood
				std::vector<double> row(size());
				
				// per ogni componente del vettore...
				for(int j = 0 ; j < size();j++) {
					// ...campiono un elemento...
					row[j] = distribution(generator) * sqrt(t);		// remark : N(0,1) * sqrt(T) = N(0,T)
				}
			
				// ... e lo aggiungo al vicinato
				canonicalNeighbourhood.push_back((row));
			}
			
	}
	
	Neighbourhood<Vector> neighbourhood() {				
			// carico il numero di elementi nel canonicalNeighbourhood
			int n = canonicalNeighbourhood.size();
			
			// carico un vettore di Vectors 
			std::vector<Vector> ret(n);
			
			// per ogni elemento nel canonicalNeighbourhood
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
	
/***Gradiente Generalizzato - direzione privilegiata*************************************************************************/

	Vector generalizedGradient(std::function<double(Vector)> f) {
		auto Y  = neighbourhood();
		auto fY = Y.apply(f);
		Vector ret(data.size());
		double weightMean = 0.;
		for(int i = 0 ; i < fY.size();i++)
		{
			double w = ( 1. + exp(10 * fY[i]) );
			//double w = (1./pow(100000. ,3) + pow( fY[i]/100000. ,3)); // funzione di assegnazione dei pesi
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
	
	bool operator < (const Vector & other) const {
		for(int i = 0 ; i < other.size();i++)
		{
			if(data[i] < other.data[i])
			{
				return true;
			}
		}
		return false;
	}
	
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
	
	Vector & operator *= (double  other) {
		for(int j = 0 ; j < data.size();j++)
			{
				data[j] *= other;
			}
		return *this;
	}
	
	
	operator std::vector<double>() {			// compatibilitá con l'oggetto wrapped
		return data;
	}
	
/***Metodi specializzati*****************************************************************************************************/	
	
	int size() const {
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
	
/***Procedura di ottimizzazione***/

static void improve(Vector & x , Vector & increment, Vector & mom) {
			mom += increment;			
			x += mom;					
}

};

Neighbourhood<Vector> Vector::canonicalNeighbourhood;
