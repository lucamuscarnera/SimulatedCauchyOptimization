/*
*  	Vector.h
*	Author:	Luca Muscarnera
*	Descr : Contiene la classe vector, che definisce il rappresentate della classe di equivalenza tra insiemi appartenenti alla collezione Vectorial.	
*			Ogni oggetto di tipo vectorial puó essere identificato con un oggetto di tipo Vector. 
*			Il tipo Vector implementa l'operazione di somma e di prodotto per scalare.
*			Contiene inoltre la classe Real, che costruisce un wrapper per la gestione dei double.
*/
#pragma once
#include <iostream>
#include <vector>
#include "../../NormalGeneration/SimpleGaussian/SimpleGaussian.h"

class Real
{
	public:
		Real() : data(0) {};
		Real(double input) : data(input) {};
		
		Real operator + (Real & other) {
			double newData = data + other.data;
			return Real(newData);
		}

		Real operator * (double other) {
			double newData = data + other;
			return Real(newData);
		}
		
		void show() {
			std::cout << data << std::endl;
		}
		static constexpr bool scalar = true;
		operator double() {
			return data;
		}
		
		/* generatore delle distribuzioni : ad ogni tipo Vectorial é associata una distribuzione 
		*  da utilizzarsi per l'approssimazione del rilassamento
		*/

		std::vector<Real> neighborhood(int n,double t = 1.) {
			return Standard<Real>(n, t);
		}
	private:
		double data;
};

class Vector
{
	public:
		Vector() : data(NULL) {};
		Vector(std::vector<double> input) : data(input) {};			// costruttore 
		
		Vector operator + (Vector & other) {
			std::vector<double> newData(data.size());
			for(int i = 0 ; i < data.size();i++)
				newData[i] = data[i] + other.data[i];
			return Vector(newData);
		};
		
		Vector operator * (double other)   {
			std::vector<double> newData(data.size());
			for(int i = 0 ; i < data.size();i++)
				newData[i] = data[i] * other;
			return Vector(newData);
		};
		void show() { 
			for(int i = 0 ;  i < data.size(); i++ )
			{
				std::cout << "\t" << data[i] << std::endl;
			}
			std::cout<<std::endl;
		}
		
		static constexpr bool scalar = false;
		
		/* generatore delle distribuzioni : ad ogni tipo Vectorial é associata una distribuzione 
		*  da utilizzarsi per l'approssimazione del rilassamento
		*/
		
		std::vector<Vector> neighborhood(int n, double t = 1.0) {
			return Standard<Vector>(n, data.size(),t);
		}
		
		operator std::vector<double>() {
			return data;
		}
	private:
		std::vector<double> data;
};

Vector operator *(double other, Vector & a) {
	return a * other;
}