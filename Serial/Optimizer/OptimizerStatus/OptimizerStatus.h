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
	
	std::string filename;
	
	std::vector<T> history;
	std::vector<double> loss;
	
	int size() {
		return history.size();
	}
	
	void push_back(T & event)
	{
		history.push_back(event);
	}
	
	void write() {
			std::ofstream file(filename , std::ofstream::out );
			file << "";
			file.close();
			file.open(filename, std::ofstream::app );
			std::clog << "cronologia lunga : " <<  history.size() << std::endl;
			for( auto h : history ) {
				auto w = h.flatten();
				for(int i = 0 ; i < w.size();i++)
				{
					file << w[i];
					if(i < (w.size() -1) )
						file << ",";
					else
						file << "\n";
				}
			}
			file.close();
	}
	
};