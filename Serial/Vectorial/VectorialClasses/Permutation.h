/*
*  	Permutation.h
*	Author:	Luca Muscarnera
*	Descr : Contiene le classe Permutation, inquadrata nella famiglia Vectorial.
*	Gli ingredienti principali della classe sono:
*		1.	apply(...) : Applicazione di una permutazione su un vettore
*
*		2.1	somma di permutazioni 
*		2.2	zero 								dotano la classe di una struttura di semigruppo
*/

#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>

#include "../../Optimizer/Optimizer/Optimizer.h"
#include "PermutationHelpers/CauchyRepresentation.h"
#include "PermutationHelpers/minobj.h"

class Permutation {
	public:
 
/***Zero***********************************************************************************************************************/
	
	static Permutation zero(const Permutation & x) {
		/* Dato un elemento di riferimento, produce l'elemento neutro dello spazio in cui esso é contenuto */
		int n = x.size();																// TODO BASARLO SU (1)
		std::vector<int> vec(n);
		for(int i = 0 ; i < n ; i ++)
		{
			vec[i] = i;
		}
		return Permutation(vec,vec);
	};
	
/***Costruttori***************************************************************************************************************/

	Permutation(int n) : data(n)  {};										// (1)
	Permutation(std::vector<int> & upper,												// definizione a partire dalla rappresentazione matriciale
				std::vector<int> & lower
				) : data(upper,lower)  {};
				
	Permutation(CauchyRepresentation & c) : data(c) {};	// definizione a partire dalla rappresentazione matriciale
		
/***Storage*******************************************************************************************************************/
	private:
						
	CauchyRepresentation data;		

	public:
	CauchyRepresentation & getData() {
		return data;
	}		

/***gestione neighbourhoods**************************************************************************************************/
	
	private:
	static Neighbourhood<Permutation> canonicalNeighbourhood;				// vicinato canonico
	void buildCanonicalNeighbourhood (int n,int T);							// costruzione vicinato canonico
	
	public:
	Neighbourhood<Permutation>  neighbourhood() const;						// ottenimento vicinato
/***Gradiente Generalizzato - direzione privilegiata*************************************************************************/
	
	private:
	// il gradiente generalizzato ha senso solo se esiste un contesto di ottimizzazione
	// per garantire ció poniiamo il membro con disponibilitá di accesso privata.
	// in questo modo potrá accedere esternamente solo la classe dichiarata FRIEND ovvero il template do optimizer
	// associato al tipo della classe
	Permutation generalizedGradient(std::function<double(Permutation)> f);
	
/***Operatori****************************************************************************************************************/
	public:
	
	bool operator < (const Permutation & other) const;
	Permutation operator * (double other);
	Permutation operator *= (double other);
	bool operator == (Permutation & other);
	Permutation operator + ( Permutation & other);
	Permutation & operator += (Permutation & other);
	
/***Metodi specializzati*****************************************************************************************************/	
	
		int size() const;
		
		template<class T>
		std::vector<T> apply(const std::vector<T>  & input) const {		 			// FONDAMENTALE : l'ottimizzazione fatta cambiando da vector a const vector & é MOLTO forte
			std::vector<T> ret(input.size());										// 64 secondi vs 86 secondi!
			for(int i = 0 ; i < input.size();i++)
			{
				ret[ data.cgetUpper()[i] ] = input[ data.cgetLower()[i] ]; 
			}
			return ret;
		}

		template<class T>
		std::vector<T>  & inplaceApply(std::vector<T>  & input) {		 			// applica in loco la permutazione
			std::vector<T> ret(input.size());										 
			for(int i = 0 ; i < input.size();i++)
			{
				ret[ data.cgetUpper()[i] ] = input[ data.cgetLower()[i] ]; 
			}
			input = ret;
			return input;
		}		
		
	
		void show() const;
		
/***Contesto di ottimizzazione************************************************************************************************/

	Optimizer<Permutation> * optimizationContext;												 // contesto di ottimizzazione
	

/***Procedura di ottimizzazione***/
	static void improve(Permutation & x , Permutation & increment, Permutation & mom) {
				x += increment;						
	}

/*** flatten ***/
	const std::vector<int> & flatten();

/*** enhancing function ***/
	static double defaultEnhancingFunction(double fy) {	// funzione iniettiva che descrive la velocitá di propagazione dell'informazione
		return 1. + 1./(0.1 + fy * fy);
	}
	
	friend Optimizer<Permutation>;
};
//std::map<Permutation , double > Permutation::memory;



#endif