/*
*	OptimizerStatus.h
*	Author : Luca Muscarnera
*	Description : Contiene lo stato della procedura di ottimizzazione
*/

#pragma once
#include "../../../Serial/Vectorial/VectorialModule.h"
#include <string>
#include <fstream>

template <Vectorial T>
class OptimizerStatus {
	public:
	std::vector<T> history;
	void push_back(T & event)
	{
		history.push_back(event);
	}
	
	void csv(std::string filename)
	{
			//TODO
	}
};