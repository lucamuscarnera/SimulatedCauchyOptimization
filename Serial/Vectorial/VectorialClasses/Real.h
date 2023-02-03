/*
*  	Vector.h
*	Author:	Luca Muscarnera
*	Descr : Contiene l'oggetto Vector, un elemento della famiglia di classi Vectorial adatto per rappresentare vettori.
*/

#ifndef REAL_H
#define REAL_H

#include <iostream>
#include <vector>

#include "../../Optimizer/Optimizer/Optimizer.h"


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

	static Neighbourhood<Real> canonicalNeighbourhood;					// vicinato canonico
	void buildCanonicalNeighbourhood (int n,double t);
	Neighbourhood<Real> neighbourhood() const;
	
/***Gradiente Generalizzato - direzione privilegiata*************************************************************************/
	private:
	// il gradiente generalizzato ha senso solo se esiste un contesto di ottimizzazione
	// per garantire ció poniiamo il membro con disponibilitá di accesso privata.
	// in questo modo potrá accedere esternamente solo la classe dichiarata FRIEND ovvero il template do optimizer
	// associato al tipo della classe
	
	Real generalizedGradient(std::function<double(Real)> f);
	
/***Operatori****************************************************************************************************************/
	public:
	
	bool operator < (const Real & other) const;
	Real operator * (double other) const;
	Real & operator += (Real & other);
	Real & operator *= (double other);
	operator double();
	
/***Metodi specializzati*****************************************************************************************************/	
	
	void show();
	
/***Contesto di ottimizzazione************************************************************************************************/

	Optimizer<Real> * optimizationContext;							// contesto di ottimizzazione

/*** Procedura di ottimizzazione ****/
protected:		// accessibile solo dall'ottimizzatore!
static void improve(Real & x , Real & increment, Real & mom) {
			increment *= 0.001;
			mom +=  increment;			
			x += mom;					
}

public:
/*** flatten ***/
	const std::vector<double> flatten() {
		std::vector<double> ret{data};
		return ret;
	};

public:
	static double defaultEnhancingFunction(double fy) {	// funzione iniettiva che descrive la velocitá di propagazione dell'informazione
		return 1. + fy * fy;
	}

friend Optimizer<Real>;

};

#endif