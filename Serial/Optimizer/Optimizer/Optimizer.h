/*
*	Optimizer.h
*	Author : Luca Muscarnera
*	Description : Gestione dell'ascesa del gradiente. La classe implementa un polimorfismo in compile time grazie al concept vectorial ; in particolare
*	la funzione core della classe accede al metodo GeneralizedGradient dell'oggetto ottimizzato, in modo da calcolare la direzione di miglioramento.
*	Inoltre optimizer definisce un *Contesto di ottimizzazione* che puo essere utile per definire la funzione di gradiente generalizzato.
*	Infine nella classe é presente il metodo per la valutazione rilassata, che a differenza del gradiente non é dipendente dalla
*	natura dello spazio di ottimizzazione, ma dipende solo dalla funzione e dalla neighbourhood considerata ma che é un concetto
*	invariante da spazio a spazio (es. possiamo definire un vicinato nello spazio delle permutazioni e un vicinato nello spazio
*	dei vettori, ma la procedura per valutare la funzione rilassata é sempre la media campionaria sulla valutazione del vicinato , indipendentemento
*	dallo spazio ; viceversa il gradiente dipende strettamente dalla struttura dello spazio , infatti in spazi discreti come le combinazioni
*	non é possibile definire una nozione di gradiente che sia coerente con quella sulle altre classi, ma dobbiamo accontentarci
*	di definire una nozione di "direzione privilegiata" che accomuna tutti gli spazi, ma la cui implementazione dipende strettamente
*	dallo spazio in cui ci troviamo )
*/
#pragma once
#include "../../../Serial/Optimizer/OptimizerStatus/OptimizerStatus.h"
#include "../../../Serial/Vectorial/Vectorial/Vectorial.h"

#include <random>
#include <iomanip>
#include <map>
#include <omp.h>


template <class T>
class Optimizer   {
	public:
	
/***Context********************************************************************************************************************/

/***Status********************************************************************************************************************/
	
	private:    OptimizerStatus<T> status;													// contiene le informazioni sullo stato della procedura 
																							// l'ingrediente principale é il metodo "screenshot" che prende in ingresso
																							// il contesto di ottimizzazione e lo salva in un vettore
	public:		OptimizerStatus<T> & getStatus() {
		return status;
	}
	
/*****************************************************************************************************************************/
	public:	
	Optimizer(T & x, 
			  std::function<double(T)> f, 
			  double time, 
			  int precision = 1000,
			  std::function<double(double)> enhancingFunction = T::defaultEnhancingFunction
			  )
	: x(x) , 
	  f(f) , 
	  observedF(f),
	  time(time), 
	  precision(precision),
	  threads(16)
	  
	{
		
		// controllo che il template sia affine al concetto di vectorial
		constexpr bool isVectorial = Vectorial<T>;
		static_assert(isVectorial , "Lo spazio che ospita la struttura dati che stai cercando di ottimizzare non ha tutti gli elementi necessari per la procedura di ottimizzazione.");
		
		x.buildCanonicalNeighbourhood(precision,time);  						// costruisco il vicinato canonico
		x.optimizationContext = this;											// imposto il contesto di ottimizzazione
		this->enhancingFunction = enhancingFunction;
	};

/***Getters & Setters*********************************************************************************************************/	
	// threads
	
	int getThreads() {return threads;};
	void setThreads(int n) {
		std::cout << "Ottimizzatore lanciato con " << n << " threads!" << std::endl;
		threads = n;
		};
	
	// time
	void setTime(double time) {
		this->time = time;
		x.buildCanonicalNeighbourhood(precision,time); 
	}
	
	double getTime()
	{
		return time;
	}
	
	// function
	
	std::function<double(T)> getFunction() {
		return observedF;
	}
	
	void setFunction(std::function<double(T)> f) {
		this->f = f;
		this->observedF = this->f;
	}
	
	// enhancingFunction	/* descrive l'accelerazione di propagazione di informazione */ 

	std::function<double(T)> getEnhancingFunction() {
		return enhancingFunction;
	}
	
	void setEnhancingFunction(std::function<double(T)> f) {
		enhancingFunction = f;		
	}	
	
	// precision
	
	int getPrecision() {
		return precision;
	}
	
	void setPrecision(int precision)
	{
		this->precision = precision;
	}
	
	// variable
	
	T & getVariable() {
		return x;
	}
	
	
	// callbacks
	public:
	
	template <class U>
	void addCallback(U f) {
		constexpr bool has_initialization = requires(U t, Optimizer<T> * c ) {		// controllo in compile time che esista un metodo init
			{ t.init(c) } -> void; 					
		};
		std::clog << "aggiunta callback..." << std::endl;
		if constexpr( has_initialization ) {										// se l'oggetto f lo ha la chiamo, altrimento semplicemente ignoro la cosa
			std::cout << "inzializzo la callback..." << std::endl;					// faccio cosí in modo da garantire flessibilita
			f.init(this);															// sul tipo di callback da costruire, in modo da non avere la necessiitá
		} else {
			std::cout << "niente da inizializzare!" << std::endl;
		}																			// esplicita che un oggetto abbia un metodo init vuoto	
																					// inoltre essendo a compile time , non "pago" la cosa! 
																					
		callbacksList.push_back(f);													// dopo questa procedura , inseriso l'oggetto	
																					// nella collezione di callbacks, castandolo come funtore
	}
	

	
/*****************************************************************************************************************************/
	public:
	double relaxedEvaluation(const T & x) {										// la relaxed evaluation é invariante per spazio di ottimizzazione
	
		auto fY = x.neighbourhood().apply(getFunction());						// calcolo il valore della funzione per ogni punto del vicinato	

		double ret = 0.;														// inizializzo il valore di ritorno a 0 
		double weightMean = 0.;													// inizializzo il valore della media dei pesi a 0
		
		/*
		for(int i = 0 ; i < fY.size();i++)										// per ogni valore nell'insieme delle valutazioni del vicinato
		{
			double w = 1. / (0.1 + fY[i]*fY[i]);								//	calcolo il peso per il punto del vicinato corrente

			weightMean = ( i * weightMean + w ) / ( i + 1. );					// 	aggiorno la media dei pesi
			ret = (i * ret + fY[i] * w ) / (i + 1.);							// 	aggiorno la somma pesata
		}*/
		

		
		#pragma omp parallel for reduction(+:weightMean) reduction(+:ret) num_threads(16)	
		for(int i = 0 ; i < fY.size();i++)										// per ogni valore nell'insieme delle valutazioni del vicinato
		{
			double w = enhancingFunction(fY[i]);								//	calcolo il peso per il punto del vicinato corrente

			weightMean += w;					// 	aggiorno la media dei pesi
			ret 	   += fY[i] * w; 			// 	aggiorno la somma pesata
		}
		
		

		return ret * ( 1. / weightMean );										// ritorno la media pesata 
	} 

/*****************************************************************************************************************************/

	void operator() (int iter) {
		T mom = T::zero(x);														// carico l'elemento neutro dallo spazio a cui appartiene l'ottimizzando
		
		for(int i = 0 ; i < iter; i ++ )			
		{
			mom *= 0.9;																		// gestisco il momentum
			T increment = getVariable().generalizedGradient(getFunction());			    	// calcolo la direzione privilegiata


			T::improve(x, increment, mom);
			
			
			for( auto & c : callbacksList )										// chiamo ogni callback dando in ingresso il contesto corrente
				(c)(this);
		}
	}
	

/*****************************************************************************************************************************/
	private:
	
	
	int threads;
	T & x;	
	public:
	std::function<double(T)> f;											// funzione data in input al costruttore
	std::function<double(T)> observedF;									// funzione osservata dalla procedura di ottimizzazione
	std::function<double(double)> enhancingFunction;							// funzione di enhancing della propagazione dell'informazione
	double time;
	int precision;
	
	std::vector<std::function< void(Optimizer<T> * )>  > callbacksList;
/*****************************************************************************************************************************/
	
	

};
