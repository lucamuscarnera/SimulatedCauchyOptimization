/*
*  	Utilities.h
*	Author:	Luca Muscarnera
*	Contiene le procedure necessarie per la manipolazione di std::vector contenenti elementi affini al concetto Vectorial.
*	Tutti i metodi sono definiti mediante template, in particolare la richiesta che il template fa agli oggetti é che la classe
*	di appartenenza abbia le operazioni necessarie ( somma , prodotto per scalare)
*/
#pragma once
#include <functional>
#include "../../Serial/Vectorial/Vectorial/Vectorial.h"


template<class T>
std::vector<T> scatterAndShift(T obj, std::vector<T> & distribution)
{
	/*
	*	ScatterAndShift : Vectorial obj, vector<Vectorial> distribution ---> vector<Vectorial>
	*	Dato in ingresso un oggetto Vectorial, produce una collezione di oggetti Vectorial che sono
	*	fondamentalmente la traslazione della distribuzione tale da rendere la distribuzione centrata
	*	nell'oggetto obj.
	*	ES:					scatterAndShift( 1., [-0.5, 0., 0.5] ) := [0.5, 1., 0.5]
	*/
	std::clog << "test2";
	int n = distribution.size();
	std::vector<T> result(n);
	std::clog << std::endl;
	obj.show();
	distribution[0].show();
	for(int i = 0; i < n;i++)
	{
		std::clog << ">>"<< i ;
		result[i] = obj + distribution[i];
	}
	return result;
} 

template<class T>
std::vector<double> map( std::function<double(T)> f, std::vector<T> X  )
{
		int n = X.size();
		std::vector<double> y(n); // inizializzo il vettore che conterrá i risultati
		for(int i = 0 ; i < n;i++)
				y[i] = f(X[i]);
		return y;
}
