/*
*	~ Callbacks.h
*	Author 		: Luca Muscarnera
*	Description : Gestisce le callbacks ad ogni iterazione
*/

#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "../../OutputManager/CSV.h"


namespace callbacks
{
template<class T>
class backwardInTime {
public:
	backwardInTime(int period)  : period(period)  {};
	void operator() (Optimizer<T> * c) {
		count++;
		if(c ->getTime() > 0)
			if(count % period == 0)
				c -> setTime( c ->getTime() * 0.97 );
		std::clog << "current time is " << c->getTime() << std::endl;
	}
int period;
int count;
};

struct {
void operator ()(Optimizer<Vectorial> * c ) 
{
	count++;
	std::clog << count << " ";
	c-> getVariable().show();
}
int count = 0;
} simpleCallback;


class relaxedEvaluationCallback {
	/* produce un csv in cui ogni riga é un array di valutazioni */
	public:
	relaxedEvaluationCallback(std::string filename, double min, double max, int N) :
	filename (filename),
	min (min) ,
	max (max) ,
	N ( N) ,
	lasteval(N),
	initialized(false)
	{
		// inizializzo il file 
		std::ofstream file( filename , std::ofstream::out );
		file << min << "," << max << "\n";
		file.close();
		// la prima riga contiene il minimo e il massimo;
	}
	
	
	void operator() (Optimizer<Real> * c) {				//  richiede che si stia ottimizzando un Real
				bool newscreenshot = false;
				
				if(initialized == false)
				{
					newscreenshot = true;
					initialized = true;
				} else {
					if ( lasttime != c->getTime() ) {	// é stato cambiato il tempo
						newscreenshot = true;
					} 
				}
				
				
				std::vector<double> evaluations(N);
				
				if(newscreenshot == true)
				{
					std::clog << "new screenshot!" << std::endl;
					for(int i = 0 ; i < N ; i ++ ) {
						double point = min + (max - min) * static_cast<double>(i) / N;
						evaluations[i] = c -> relaxedEvaluation( point );
						lasteval[i] = evaluations[i];
					}
						lasttime = c->getTime();
				} else {
						evaluations = lasteval;
				}
				
				
				std::ofstream file( filename , std::ofstream::app );
				
				
				for(int i = 0 ; i < N ; i ++ ) {
					file << evaluations[i];
					if( i != (N-1) )
						file << ",";
					else
						file << "\n";
			
		}
		file.close();
	}
	
	
	double lasttime;
	std::string filename;
	double min;
	double max;
	int N;
	bool initialized;
	std::vector<double> lasteval;
};


class storeLossesCallback		// ogni volta che viene chiamata aggiorna un csv con il contenuto della loss
{
	public:
	storeLossesCallback(std::string filename)  : filename(filename) {
		std::ofstream file( filename , std::ofstream::out );
		file << "";
		file.close();
	}
	
	void operator() ( Optimizer<Vectorial> * c ) 
	{
		std::clog << "storaggio della loss..." << std::endl;
		std::ofstream file( filename , std::ofstream::app );
		file << c -> getFunction() ( c -> getVariable() )  << "\n";
		file.close();
	}
	std::string filename;
};

template<Vectorial T>
class bestResultCallback
	{
		public:
			bestResultCallback(std::string filename) : count(0) , candidato(0), filename(filename) {
				std::cout << "wrong" << std::endl;
				
			}
			
			
			void operator() ( Optimizer<T> * c ) {
				// do nothing
				if(count > 0) {
					double eval  = c -> getFunction()( c ->getVariable() );
					std::clog << "best = " << best << std::endl;
					if (  eval > best )
					{
						best = eval;	
						candidato = c -> getVariable();
						
						candidato.show();
						
						auto C = candidato.flatten();
						CSV file(filename);
						file.write(C);
						
					}
				} else {
					best = c -> getFunction()( c ->getVariable() );
				}
				std::clog <<  " [ " << count << " ] migliore fino ad ora : " << best << std::endl;
				
				count++;																	// conta le iterazioni
			}
		int count;
		double best;
		T candidato;
		std::string filename;
	} ;
	


struct tmpCallback
{	
	void operator() (Optimizer<Permutation> * c) {
		history.push_back( c -> getVariable() );
		std::clog << "[ T = " 
				  <<  c->getTime() 
				  << "] Il valore corrente é " 
				  << c -> getFunction()(c->getVariable()) 
				  << " il valore percepito della funzione é "
				  << c -> relaxedEvaluation( c -> getVariable())
				  << std::endl;
		if(history.size() > 2)
		{
			if(history[history.size() - 3] == history[history.size() - 1] )
			{
				if( c->getTime() > 1)	
					c -> setTime( c -> getTime() - 1 );												// raffino il vicinato
				else
					c -> setTime(5);																// provo un altra distribuzione di path
			}
		}
	}
	static std::vector<Permutation> history;
}  permutationCallback;
std::vector<Permutation> tmpCallback::history;


class writeCallback
{
	public:
		writeCallback(std::string filename) : filename(filename) {};
		void init( Optimizer<Vectorial> * c ) {
			c -> getStatus().filename = filename;;
		}
		void operator() ( Optimizer<Vectorial> * c ) {
			c -> getStatus().push_back( c-> getVariable() , c->getFunction() (c->getVariable()) );
			std::clog << "aggiunto evento alla cronologia! "<< c->getStatus().size() << "\tloss=" << c -> getFunction()( c -> getVariable()) << std::endl;
			c->getVariable().show();
		}		
		std::string filename;
} ;




};


#endif