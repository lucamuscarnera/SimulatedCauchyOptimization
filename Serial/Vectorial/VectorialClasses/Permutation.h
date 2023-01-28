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

#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>

class CauchyRepresentation
{
	/*  Classe costruita per contenere la rappresentazione bivettoriale
	*   di una permutazione. É stata trattata separatamente dal wrapper
	*   Permutation in modo da avere dei metodi che sono formalmente parte
	*	della rappresentazione matriciale 
	*/
	public:

/***Costruttori**************************************************************************************************/
		
		CauchyRepresentation(int n) : upper(n) , lower(n)
		{
			for(int i = 0 ; i < n ; i++)
			{
				upper[i] = i;
				lower[i] = i;
			}
		}
		
		CauchyRepresentation(std::vector<int> & upper, 					// permutazione partendo da vettori
							 std::vector<int> & lower
							) : upper(upper) , lower(lower) {};
	
/***Getter*******************************************************************************************************/

		std::vector<int> & getLower()  {
			return lower;
		}
		std::vector<int> & getUpper()  {
			return upper;
		}
		
/***Getter di uso interno****************************************************************************************/

		// getter in verrsione const per permettere l'ottimizzazione
		// da parte del compilatore
			
		const std::vector<int> & cgetUpper() const {
			return upper;
		}	
		const std::vector<int> & cgetLower() const {
			return lower;
		}

/***Swap*********************************************************************************************************/
		
		CauchyRepresentation swap(int a,int b)
		{
			std::vector<int> newLower = getLower();
			std::vector<int> newUpper = getUpper();
			std::swap( newLower[a], newLower[b] );
			return CauchyRepresentation(newUpper,newLower);
		}

		CauchyRepresentation & inplaceSwap(int a,int b)
		{
			std::swap( lower[a], lower[b] );
			return *this;
		}
		
/****************************************************************************************************************/
/****************************************************************************************************************/
/****************************************************************************************************************/
	
	private:
		std::vector<int> upper;
		std::vector<int> lower;
};

class Permutation {
	public:

/***Zero***********************************************************************************************************************/
	
	static Permutation zero(Permutation & x)  {
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

	Permutation(int n) : data(n) , data_size(n) {};										// (1)
	
	Permutation(std::vector<int> & upper,												// definizione a partire dalla rappresentazione matriciale
				std::vector<int> & lower
				) : data(upper,lower) , data_size(upper.size()) {};
				
	Permutation(CauchyRepresentation  c) : data(c), data_size(c.getUpper().size()) {};	// definizione a partire dalla rappresentazione matriciale
		
/***Storage*******************************************************************************************************************/
	
	int data_size;						//
	CauchyRepresentation data;			//	

/***gestione neighbourhoods**************************************************************************************************/

	static Neighbourhood<Permutation> canonicalNeighbourhood;				// vicinato canonico
	
	void buildCanonicalNeighbourhood (int n,int T = 5) {					// assemblaggio del vicinato canonico
																			// TODO : da rendere omogeneo
																			
			canonicalNeighbourhood.flush();									// svuoto il vicinato canonico nel caso fosse stato precedentemente inizializzato
			
			std::random_device rd;  										//	Will be used to obtain a seed for the random number engine
			std::mt19937 generator(rd()); 									//	Standard mersenne_twister_engine seeded with rd()
			std::uniform_int_distribution<> distribution( 0 , size()-1);
			std::uniform_int_distribution<> distribution_steps( 0 , T);
			
			
			for(int i = 0 ; i < n;i++)
			{
				CauchyRepresentation path(size());							// costruisco i path a partire dall'identitá
				for(int j = 0 ; j < T ;j++)
				{
					int a = distribution(generator);						/*  genero un path definito come composizione 	*/
					int b = distribution(generator);						/*  di swap 									*/
					path = path.swap(a,b);
				}
				Permutation P(path);					
				canonicalNeighbourhood.push_back(P);						// aggiungo il path trovato al caninocalNeighbourhood
			}				
			
	}
	
	Neighbourhood<Permutation>  neighbourhood() {				
			int n = canonicalNeighbourhood.size();							//	carico la grandezza del canonicalNeighbourhood
			std::vector<Permutation> ret;									//  inizializzo un vettore di permutazioni
			
			for(int i = 0; i < n;i++)										// per ogni elemento nel canonical neighbour...
			{																
				Permutation value = *this;									// Costruisco un oggetto copia della permutazione
				value += canonicalNeighbourhood[i];							// gli applico una perturbazione descritta dalla permutazione nel canonicalNeighbour
				ret.push_back(value);										// e la aggiungo al vettore di permutazioni
			}
			return ret;														// ritorno il vettore, che verrá castato come Neighbourhood
	}

/***Gradiente Generalizzato - direzione privilegiata*************************************************************************/

	Permutation generalizedGradient(std::function<double(Permutation)> f)
	{
		//  calcolo i vicini a 1-swap e costruisco una neighbourhood
		Neighbourhood<Permutation> direzioni;
		Neighbourhood<Permutation> vicini;
		std::vector<double> vals;
		
		// inizializzo la permutazione da ritornare con un identitá
		Permutation ret(size());
		double min  = 10000;						// TODO  : fixare sta roba
		
		// inizializzo due variabili per contenere le coordinate dello swap migliore
		int best_a = 0;
		int best_b = 0;
		
		// itero lungo tutti i possibili swap ( compresi quelli stazionari)
		for(int i  = 0 ; i < size() - 1; i++)
		{
			for(int j = i + 1 ; j < size(); j++)
			{
				// genero una copia della permutazione con lo swap associato alle iterazioni del loop
				Permutation P = *this;	
				P.data.inplaceSwap(i,j);
				auto direzione = P;
				
				// valuto la permutazione ottenuta
				double curr = optimizationContext->relaxedEvaluation(P);
				
				// se il valore valutato risulta minore del minimo , aggiorno le coordinate della direzione favorita e di conseguenza il minimo
				if(curr < min)
				{
					best_a = i;
					best_b = j;
					min = curr;
				}
			}
		}
		
		// ritorno un identitá con lo swap che migliorava la permutazione corrente, in modo da poterla sommare nel loop
		ret.data.inplaceSwap(best_a,best_b);
		return ret;
	}
	
/***Operatori****************************************************************************************************************/
	
	bool operator < (const Permutation & other) const
	{
		for(int i = 0 ; i < other.size();i++)
		{
			if( data.cgetLower()[i] < other.data.cgetLower()[i])
				return true;
			else
				if( data.cgetLower()[i] > other.data.cgetLower()[i])
					return false;
		}
		return false;
	}
	
	Permutation operator * (double other)
	{		
		return *this;						// il semigruppo delle permutazioni
											// non ha un operazione di moltiplicazione
	}
	
	Permutation operator *= (double other)
	{		
		return *this;						// il semigruppo delle permutazioni
											// non ha un operazione di moltiplicazione
	}
	
	bool operator == (Permutation & other)
	{
		for(int i = 0 ; i < other.size();i++)
		{
			if ( other.data.getLower()[ other.data.getUpper()[i] ] != data.getLower()[data.getUpper()[i]] )
				return false;
		}
		return true;
	}
	
	Permutation operator + ( Permutation & other)
	{
		Permutation ret = *this;
		ret += other;
		ret.canonicalNeighbourhood = other.canonicalNeighbourhood;
		ret.optimizationContext = other.optimizationContext;
		return ret;
	}
	
	Permutation & operator += (Permutation & other) {
		
		// definisco l'operazione di somma come composizione delle permutazioni
		// si parla infatti di SEMIgruppo delle permutazioni per via della non commutativitá
		// della composizione di funzioni, che peró risulta essere associativa.
		data.getLower() = other.apply(data.getLower());
		return *this;
	}
	
/***Metodi specializzati*****************************************************************************************************/	
	
		int size() const {
			return data_size;
		}
		template<class T>
		
		std::vector<T> apply(std::vector<T>  input) const {
			std::vector<T> ret(input.size());
			for(int i = 0 ; i < input.size();i++)
			{
				ret[ data.cgetUpper()[i] ] = input[ data.cgetLower()[i] ]; 
			}
			return ret;
		}
	
		void show() {
			std::cout << std::endl;
			int n = size();
			for(int i = 0 ; i < n;i++)
			{
				std::cout << data.getUpper()[i] << ( (i < (n-1))? std::setw(5) : std::setw(0));
			}
				std::cout << std::endl;
			for(int i = 0 ; i < n;i++)
			{
				std::cout << data.getLower()[i] << ( (i < (n-1))? std::setw(5) : std::setw(0));
			}
				std::cout << std::endl;
		}
	
/***Contesto di ottimizzazione************************************************************************************************/

	Optimizer<Permutation> * optimizationContext;												 // contesto di ottimizzazione
	
/***Rappresentazione vettoriale **********************************************************************************************/
	
	const std::vector<int> & flatten() {
		return data.cgetLower();
	}
	
/***Procedura di ottimizzazione***/
static void improve(Permutation & x , Permutation & increment, Permutation & mom) {
			x += increment;						
}

};
//std::map<Permutation , double > Permutation::memory;
Neighbourhood<Permutation> Permutation::canonicalNeighbourhood;


#include "../../../Serial/Optimizer/OptimizerScheduler/callbacks.h"
