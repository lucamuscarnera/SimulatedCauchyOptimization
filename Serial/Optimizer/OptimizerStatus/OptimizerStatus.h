/*
*	OptimizerStatus.h
*	Author : Luca Muscarnera
*	Description : Contiene lo stato della procedura di ottimizzazione
*   É stato definito un container per la cronologia della procedura di ottimizzazione in modo da poter essere analizzata
* 	succesivamente all'interno dell applicazione.
*	Ad esempio, nel caso volessimo analizzare il grado di non-convessitá del problema il vettore LOSS offre uno spaccato dell'andamento
*	della variabile ottimizzanda permettendoci di avere un insight sulla natura della funzione ottimizzata.
*/

#pragma once

#ifndef OPTIMIZERSTATUS_H
#define OPTIMIZERSTATUS_H

#include <string>
#include <fstream>
#include <functional>


template <class T>
class OptimizerStatus {
	public:
	
	std::string filename;
	
	private:
	
	std::vector<T> history;
	std::vector<double> loss;
	
	public:
	
	const std::vector<T> & getHistory() const {
		return history;
	}
	
	const std::vector<double> & getLosses() const  {
		return loss;
	}
	
	int size() {
		return history.size();
	}
	
	void push_back(T & event, double pippo)
	{
		history.push_back(event);
		loss.push_back(pippo);
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

#endif