/*
*	~ Callbacks.h
*	Author 		: Luca Muscarnera
*	Description : Gestisce le callbacks ad ogni iterazione
*/
#pragma once
#include "../../OutputManager/CSV.h"


namespace callbacks
{

class storeLossesCallback
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
			c -> status.filename = filename;;
		}
		void operator() ( Optimizer<Vectorial> * c ) {
			c -> status.push_back( c-> getVariable() );
			std::clog << "aggiunto evento alla cronologia! "<< c->status.size() << "\tloss=" << c -> getFunction()( c -> getVariable()) << std::endl;
			c->getVariable().show();
		}		
		std::string filename;
} ;



class pippoCallback
{
	public:
		pippoCallback(std::string nome) : nome(nome) {			
		}
		
		void operator() (Optimizer<Vectorial> * c) {
			std::clog << nome << std::endl;
		}
		
		std::string nome;
};

};
