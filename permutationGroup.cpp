// PermutationGroup

#include <iostream>
#include <vector>
#include <iomanip>
#include <math.h>
#include <random>
#include <functional>

template<class T>
class Neighbourhood {
	public:
		//Neighbourhood(Neighbourhood & other) : data(other.data) {};
		Neighbourhood() {};
		Neighbourhood(std::vector<T> input) : data(input) {};
		std::vector<T> data;
		void push_back(T input){
			data.push_back(input);
		}
		
		
		int size() {
			return data.size();
		}
		T & operator[] (int i)
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


class Couple
{
		public:
			Couple( int first, int second ) : first(first) , second(second) {};
			int getFirst() {return first;};
			int getSecond() {return second;};
		private:
			int first;
			int second;
};

class Permutation
{
	public:
		Neighbourhood<Permutation> neighbourhood;
		
		Permutation(int n) : n(n), lower(n) , upper(n)	// costruttore base
		{
			// restituisce la permutazione banale
			for(int i = 0 ; i < n ; i++)
			{
				upper[i] = i;
				lower[i] = i;
			}
		}
		
		
		Permutation ( std::vector<int> upper , std::vector<int> lower ) : n(upper.size()), upper(upper) , lower(lower) {}; // costruttore da vettori
		
		/**************************************************************************************************/
		/**Applicazione di una permutazione****************************************************************/
		/**************************************************************************************************/
		
		
		template<class T>
		std::vector<T> apply(std::vector<T> & source)
		{
			int size = source.size();
			std::vector<T> ret(size);
			for(int i = 0 ; i < size;i++)
			{
				ret[ upper[i] ] = source[ lower[i] ]; 
			}
			return ret;
		}


		/**************************************************************************************************/
		/**Somma di permutazioni (non commutativa)*********************************************************/
		/**************************************************************************************************/
		
		
		Permutation operator + (Permutation & other)
		{
			// ordino l'upper della seconda permutazione in modo che sia della forma { 0 , 1 , ... , n-1 }
				// clono upper e lower dell'addendo
				std::vector<int> tmpUpper = other.upper;
				std::vector<int> tmpLower = other.lower;
				// sorting
				// uso un counting sort 
				std::vector<int> newLower(tmpLower.size());
				for(int i = 0 ; i < tmpLower.size();i++)
				{
					newLower[ tmpUpper[i] ] = tmpLower[ tmpUpper[i] ]; 
				}
				
				
			// Permuto la seconda permutazione usando come permutazione il lower della prima
				
				newLower = apply(newLower);
				auto P = Permutation(
					upper,
					newLower
				);
				P.neighbourhood = neighbourhood;
				return P;
		}
		
		/**************************************************************************************************/
		/**Perturbazione***********************************************************************************/
		/**************************************************************************************************/
			
		static Permutation perturbation(			// METODO STATICO
				int n ,
				std::vector<Couple> couples 		// prende in ingresso una collezione di coppie di indici da swappare
		) {
			Permutation ret(n);							// costruisce un identitá
			
			for(int i = 0 ; i < couples.size() ; i++)
			{
				Couple currCouple = couples[i];
				int first  = currCouple.getFirst();
				int second = currCouple.getSecond();
				int tmpFirst = ret.lower[first];
				ret.lower[first] = ret.lower[second];
				ret.lower[second] = tmpFirst;
			}
			return ret;
		}
		
		/**************************************************************************************************/
		/**Costruzione del vicinato************************************************************************/
		/**************************************************************************************************/
		
		void buildNeighbourhood(int samples , double t)
		{
			std::random_device rd; // obtain a random number from hardware
			std::mt19937 generator(rd()); // seed the generator
			std::normal_distribution<double> distribution(0 , t); 		  // define the range
			
			std::uniform_int_distribution<> random_sample(0 , n - 1); // selettore di elemento
			
			// vogliamo costruire un vicinato che ci permetta di analizzare il flusso di informazione
			Neighbourhood<Permutation> newNeighbourhood;
			for(int i = 0 ; i < samples ; i ++)
			{
				int how_many = static_cast<int>(abs(distribution(generator)));
				//std::clog << "generating " << how_many << std::endl;
				std::vector<Couple> coppie;
				for(int j = 0 ; j < how_many;j++)
				{
					auto first = random_sample(generator);
					auto second = random_sample(generator);
					coppie.push_back(
						Couple(first,second)
					);
				}
				auto P = perturbation( n , coppie  );
				//P.show();
				newNeighbourhood.push_back( P );
			}
			neighbourhood = newNeighbourhood;
		}
		
		/**************************************************************************************************/
		/**Gradiente Generalizzato*************************************************************************/
		/**************************************************************************************************/
		
		Permutation generalizedGradient(std::function<double(Permutation)> f)
		{
			// genero tutte i vicini (1 swap di distanza)
			
			// per prima cosa genero le coppie che definiscono gli swap di elementi
			std::vector<Permutation> vicini;
			std::vector<Permutation> direzioni;
			std::vector<std::vector<Couple>> listCoppie;
			for(int i = 0 ; i < (n - 1);i++)
			{
				for(int j = i + 1; j < n ; j++)
				{
					listCoppie.push_back( std::vector<Couple>{Couple(i,j)});
				}
			}
			
			//std::cout <<  "Ho un vicinato di " << neighbourhood.size() << std::endl;
			
			// a questo putno partendo dalle coppie genero le permutazioni derivanti dall'identitá
			for(int i = 0 ; i < listCoppie.size();i++)
			{
				// ogni perturbazione dell'identitá identifica una direzione
				Permutation P = perturbation(n, listCoppie[i]) ;
				P.neighbourhood = neighbourhood;
				direzioni.push_back(P);
				
				// una direzione + me stesso = un mio vicino
				P = P + *this;
				
				for(int k = 0 ; k < P.neighbourhood.size();k++) {
					P.neighbourhood[k] = P.neighbourhood[k] + P;
				}
				vicini.push_back(
						P
				);
			}
			
			std::vector<double>  medie;
			//std::clog<< "ho " << vicini.size() << " vicini " << std::endl; 
			for(auto v: vicini)
			{
				//std::cout << "questo vicino ha un vicinato di " << v.neighbourhood.size() << " vicini "<<std::endl;
				double ret = 0;
				auto Y = v.neighbourhood.apply(f);
				double wmean = 0.;
				for(int k = 0 ; k < Y.size();k++)
				{
					double w = 1. / (1. + Y[k]);
					ret   = ( k * ret + Y[k] * w ) / ( k + 1);
					wmean = ( k * wmean + w ) /(k +1 );
 					//std::cout << "\t\t" << ret << std::endl;
				}
				medie.push_back(ret/wmean);
				//std::clog << ret << std::endl;
			}
			//getchar();
			
			// trovo la media minima
			
			auto minimo = medie[0];
			int  argmin = 0;
			for(int i = 1; i < medie.size();i++)
			{
				if (medie[i] < minimo)
				{
					minimo = medie[i];
					argmin = i;
				}
			}
			//std::clog << minimo << std::endl;
			return direzioni[argmin];
		}
		
		
		void show() {
			std::cout << std::endl;
			for(int i = 0 ; i < n;i++)
			{
				std::cout << upper[i] << ( (i < (n-1))? std::setw(5) : std::setw(0));
			}
				std::cout << std::endl;
			for(int i = 0 ; i < n;i++)
			{
				std::cout << lower[i] << ( (i < (n-1))? std::setw(5) : std::setw(0));
			}
				std::cout << std::endl;
		}
	private:
		std::vector<int> upper;
		std::vector<int> lower;
		int n;
		
};

class basicVector {
	public:
	basicVector() : data() {};
	basicVector(std::vector<double> x) : data(x) {};
	double norm() {
		double ret = 0;
		for(int i = 0 ; i < data.size();i++)
		{
			ret += (data[i] * data[i]);
		}
		return sqrt(ret);
	}
	double operator [] (int x) { return data[x]; }
	basicVector operator +  (basicVector & y) { 
		std::vector<double> ret = data;
		for(int i = 0 ; i < ret.size();i++)
		{
			ret[i] += y[i];
		}
		return basicVector(ret);
	}

	basicVector operator -  (basicVector & y) { 
		std::vector<double> ret = data;
		for(int i = 0 ; i < ret.size();i++)
		{
			ret[i] -= y[i];
		}
		return basicVector(ret);
	}
	
	basicVector operator *  (basicVector & y) { 
		std::vector<double> ret = data;
		for(int i = 0 ; i < ret.size();i++)
		{
			ret[i] *= y[i];
		}
		return basicVector(ret);
	}
	
	basicVector operator /  (basicVector & y) { 
		std::vector<double> ret = data;
		for(int i = 0 ; i < ret.size();i++)
		{
			ret[i] /= y[i];
		}
		return basicVector(ret);
	}
	
	void show() {
			for(int i = 0 ; i < data.size();i++)
			{
				std::cout << data[i] << ( (i < (data.size()-1))? std::setw(15) : std::setw(0));
			}
				std::cout << std::endl;
	}
	
		std::vector<double> data;
	
};

double travellingSalesman(std::vector<basicVector> X, Permutation p) {
	std::vector<basicVector> path = X;
	path = p.apply(path);
	double ret = 0.;
	for(int i = 1; i < path.size();i++)
	{
		ret += ( path[i] - path[i - 1] ).norm();
	}
	return sqrt(ret);
}


int main() {
	
	std::default_random_engine generator;
	std::normal_distribution<double> distribution(0.0,1.0);
	
	std::vector<basicVector> X(50);
	for(int i = 0 ; i < X.size();i++)
	{
		X[i] = basicVector( std::vector<double> { distribution(generator) , distribution(generator) }   );
	}
	
	Permutation identity(X.size()); 
	
	
	std::cout << "costo del percorso  : " << travellingSalesman(X, identity) << std::endl;
	
	identity.buildNeighbourhood(10,5.);
	auto travellingSalesman_handler = [=](Permutation p) { return travellingSalesman(X,p);};
	
	
	auto P = identity;
	std::cout << "Trivial optimization" << std::endl;
	P.buildNeighbourhood(10,0.);
	for(int i = 0; i < 10 * 15 ;i++)
		{
			P = P.generalizedGradient(travellingSalesman_handler) + P;
			//P.show();
			std::cout <<  i << "\tError = " << travellingSalesman_handler(P) << std::endl;
		}
	
	P = identity;
	std::cout << "Simulated cauchy optimization" << std::endl;
	for(double t = 15. ; t > 0.1 ; t -= 1.)
	{
		P.buildNeighbourhood(10,t);
		for(int i = 0; i < 10;i++)
		{
			P = P.generalizedGradient(travellingSalesman_handler) + P;
			//P.show();
			std::cout << "T = " << t << "\tError = " << travellingSalesman_handler(P) << std::endl;
		}
		
	}
}

/*
int main()
{
std::vector data{1.,3.,5.,7.,9.};
Permutation reversing(
				std::vector<int>{0,1,2,3,4} , 
				std::vector<int>{4,3,2,1,0} );

Permutation identity = reversing + reversing;
Permutation::perturbation(
	10,
	std::vector<Couple> {
		Couple(0,3),
		Couple(2,1)
	}
).show();


basicVector z(std::vector<double> {1.,2.,3.});
(z * z).show();
}*/