/*
*  	Vector.h
*	Author:	Luca Muscarnera
*	Descr : Contiene la classe vector, che definisce il rappresentate della classe di equivalenza tra insiemi appartenenti alla collezione Vectorial.	
*			Ogni oggetto di tipo vectorial puó essere identificato con un oggetto di tipo Vector. 
*			Il tipo Vector implementa l'operazione di somma e di prodotto per scalare.
*			Contiene inoltre la classe Real, che costruisce un wrapper per la gestione dei double.
*/

#include <iostream>
#include <vector>
#include <random>
#include <iomanip>

#include "../../Optimizer/Optimizer/Optimizer.h"

#ifndef VECTOR_H
#define VECTOR_H

class Vector {
	public:
	
/***Zero**********************************************************************************************************************/
	
	static Vector zero(Vector & x)  {										// Zero dello spazio
		return Vector(x.size());
	};
	
/***Costruttori***************************************************************************************************************/
	
	Vector() : data() {};													// costruttore base
	Vector(int i) : data(i) {};												// costruttore di zeri a i dimensioni
	Vector(std::vector<double> input) : data(input) {};						// costruttore a partire dall'informazione grezza
	
/***Storage*******************************************************************************************************************/
	private:
	
	std::vector<double> data;												// container dell'informazione grezza
	
	public:
	
	std::vector<double> & getData() {
		return data;
	}
	
/***Gestione neighbourhoods**************************************************************************************************/
	private:
	
	static Neighbourhood<Vector> canonicalNeighbourhood;					// vicinato canonico
	void buildCanonicalNeighbourhood (int n, double t);						// costruzione vicinato canonico
	
	public:
	Neighbourhood<Vector> neighbourhood() const;									// ottenimento vicinato
	
/***Gradiente Generalizzato - direzione privilegiata*************************************************************************/
	private:
	// il gradiente generalizzato ha senso solo se esiste un contesto di ottimizzazione
	// per garantire ció poniiamo il membro con disponibilitá di accesso privata.
	// in questo modo potrá accedere esternamente solo la classe dichiarata FRIEND ovvero il template do optimizer
	// associato al tipo della classe
	Vector generalizedGradient(std::function<double(Vector)> f);			// gradiente generalizzato
	
/***Operatori****************************************************************************************************************/
	public:
	bool operator < (const Vector & other) const;
	Vector operator * (double other);
	Vector  operator + (Vector &  other);
	Vector & operator += (Vector & other);
	Vector & operator *= (double  other);
	operator std::vector<double>();
	double & operator [] (int i) {
		return data[i];
	} 
	
/***Metodi specializzati*****************************************************************************************************/	
	public:
	int size() const;
	void show();
	
	
/***Contesto di ottimizzazione************************************************************************************************/
	private:
	Optimizer<Vector> * optimizationContext;							// contesto di ottimizzazione
	Optimizer<Vector> * getOptimizationContext() const;
	void setOptimizationContext(Optimizer<Vector> * other);
	
	
/***Procedura di ottimizzazione***/
private:
static void improve(Vector & x , Vector & increment, Vector & mom) {
			mom += increment;			
			x += mom;					
}

/*** flatten ***/
public:
std::vector<double> flatten() {
	return data;
}

/*** Enhancing function */
static double defaultEnhancingFunction(double fy) {	// funzione iniettiva che descrive la velocitá di propagazione dell'informazione
		return 1. + fy * fy;
	}

friend Optimizer<Vector>;
};

#endif