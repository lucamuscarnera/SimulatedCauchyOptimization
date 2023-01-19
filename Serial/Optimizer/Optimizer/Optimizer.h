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

#include "../../../Serial/Vectorial/VectorialModule.h"
#include "../../../Serial/Optimizer/OptimizerStatus/OptimizerStatus.h"

template <Vectorial T>
class Optimizer{
	public:
/***Status********************************************************************************************************************/
OptimizerStatus<T> status;
/*****************************************************************************************************************************/
	
	Optimizer(T & x, std::function<double(T)> f, double time, int precision = 1000) : x(x) , f(f), time(time) {
		x.buildCanonicalNeighbourhood(precision);		// costruisco il vicinato canonico
		x.optimizationContext = this;					// imposto il contesto di ottimizzazione
	};

/*****************************************************************************************************************************/	
	
	double relaxedEvaluation(T & x) {					// la relazed evaluation é invariante per spazio di ottimizzazione
		auto Y  = x.neighbourhood(time);
		auto fY = Y.apply(f);
		double ret = 0.;
		double weightMean = 0.;
		
		for(int i = 0 ; i < fY.size();i++)
		{
			double w = 1 + fY[i]*fY[i];
			weightMean = ( i * weightMean + w ) / ( i + 1 );
			ret = (i * ret + fY[i] * w ) / (i + 1);
		}
		return ret * ( 1. / weightMean );
	} 

/*****************************************************************************************************************************/

	void operator() (int iter) {
		T mom = x * 0;
		for(int i = 0 ; i < iter; i ++ )
		{
			mom =  mom * 0.9;
			T increment = x.generalizedGradient(f,time) * 0.1;
			mom += increment;
			x += mom;
			x.show();
			//std::clog << relaxedEvaluation(x) << std::endl;
			//printf("\t\t%f\t\t%f\n",relaxedEvaluation(x), f(x));
		}
	}
	

/*****************************************************************************************************************************/

	T & x;	
	std::function<double(T)> f;
	double time;
};
