/*
* Diffusion.h
* Author : Luca Muscarnera
*/
#include "../Utilities/Utilities.h"

template<Vectorial T> 
class relax 
{
public:
std::vector<T> sampling ;

// functor
std::function<double(T &)> 
operator () (
			std::function<double(T)> f,
			T zero,							// origine dello spazio
			double time,
			double propagation = 0.,
			std::function<double(double)> 
				weightFunction = [](  double y ) {return 1.; } 
		 )
{
		std::clog << "test";
		sampling = zero.neighborhood(1000, time); 		// campionamento
		
		std::function<double(T &)> ret = [=](T & x) {
			std::clog << "test";
			
			auto Y = scatterAndShift(x , sampling);				// estraggo i vicini

		std::clog << "test3";
			auto fY = map(f,Y);									// calcolo il valore della funzione nei vicini
			auto  w = map<double>(weightFunction , fY);			// associo ad ogni valore un peso

			double num = 0.;	
			double den = 0;
			
			for(int i = 0; i < fY.size();i++)					// calcolo la media campionaria
			{
				num = (i * num + fY[i] * w[i]  )/(i + 1.);	
				den = (i * den + w[i])/(i + 1.);
			}
			
			return num/den;											// ritorno la media campionaria
		};
		return ret;
}
};