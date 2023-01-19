/*
*	Neighbourhood.h
*	Author : Luca Muscarnera
*	Description : definisce una classe wrapper per la gestione dei campionamenti a partire da un oggetto Vectorial
*	Il core di questo wrapper é la funzione apply, che data una neighbourhood restituisce la valutazione di una funzione
*	per ogni punto della neighbourhood
*/
#pragma once
#include <vector>
#include <functional>

template<class T>
class Neighbourhood {
	public:
		Neighbourhood() {};
		Neighbourhood(std::vector<T> input) : data(input) {};
		std::vector<T> data;
		void push_back(T input){
			data.push_back(input);
		}
		int size() {
			return data.size();
		}
		T operator[] (int i)
		{
			return data[i];
		}
		
		std::vector<double> apply(std::function<double(T)> f) {
			std::vector<double> y(size());
			for(int i = 0 ; i < size();i++) {
				y[i] = f(data[i]);
			}
			return y;
		}
};