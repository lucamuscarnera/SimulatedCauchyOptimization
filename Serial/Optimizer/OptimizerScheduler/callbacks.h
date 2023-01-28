/*
*	~ Callbacks.h
*	Author 		: Luca Muscarnera
*	Description : Gestisce le callbacks ad ogni iterazione
*/
#pragma once
#include "../../OutputManager/CSV.h"

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

