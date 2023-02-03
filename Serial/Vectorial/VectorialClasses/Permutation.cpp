// Permutation.cpp
#include "Permutation.h"

// costruzione vicinato canonico

void Permutation::buildCanonicalNeighbourhood (int n,int T) {				// assemblaggio del vicinato canonico
																			// TODO : da rendere omogeneo
																			
			canonicalNeighbourhood.flush();									// svuoto il vicinato canonico nel caso fosse stato precedentemente inizializzato
			
			std::random_device rd;  										//	Will be used to obtain a seed for the random number engine
			std::mt19937 generator(rd()); 									//	Standard mersenne_twister_engine seeded with rd()
			std::uniform_int_distribution<> distribution( 0 , size()-1);
			std::uniform_int_distribution<> distribution_steps( 0 , T);
			
			
			
			for(int i = 0 ; i < n;i++)
			{
				CauchyRepresentation path(size());							// costruisco i path a partire dall'identitá
				for(int j = 0 ; j < T ;j++)
				{
					int a = distribution(generator);						/*  genero un path definito come composizione 	*/
					int b = distribution(generator);						/*  di swap 									*/
					path = path.swap(a,b);
				}
				Permutation P(path);					
				canonicalNeighbourhood.push_back(P);						// aggiungo il path trovato al caninocalNeighbourhood
			}				
			
	}


// ottenimento vicinato
Neighbourhood<Permutation>  Permutation::neighbourhood() const {				
			int n = canonicalNeighbourhood.size();							//	carico la grandezza del canonicalNeighbourhood
			std::vector<Permutation> ret(n, 0);								//  inizializzo un vettore di permutazioni
			#pragma omp parallel for num_threads( optimizationContext-> getThreads() ) shared(ret,n)
			for(int i = 0; i < n;i++)										// per ogni elemento nel canonical neighbour...
			{																
				//#pragma atomic 
				{
				Permutation value = *this;									// Costruisco un oggetto copia della permutazione
				value += canonicalNeighbourhood[i];							// gli applico una perturbazione descritta dalla permutazione nel canonicalNeighbour
				ret[i] = value;												// e la aggiungo al vettore di permutazioni
				}
			}
			return ret;														// ritorno il vettore, che verrá castato come Neighbourhood
	}
	
// gradiente generalizzato
Permutation Permutation::generalizedGradient(std::function<double(Permutation)> f)
	{
		//  calcolo i vicini a 1-swap e costruisco una neighbourhood
		std::vector<double> vals;
		
		// inizializzo la permutazione da ritornare con un identitá
		Permutation ret(size());
		//double min  = 10000;						// TODO  : fixare sta roba
		
		// inizializzo due variabili per contenere le coordinate dello swap migliore
		//int best_a = 0;
		//int best_b = 0;
		
		
		
		std::vector<int> firsts( (size() -1) * (size()) );
		std::vector<int> seconds( (size() -1) * (size()) );
		
		int count = 0;
		for(int i = 0 ; i < size() - 1;i++)
			for(int j = i + 1 ; j < size();j++) {
				firsts[count] = i;
				seconds[count] = j;
				count++;
			}
			
		int couple_size = (size() - 1) * (size());
		
		minobj min(1000,	// TODO : FIXARE
				0,1);
		
		#pragma omp declare reduction( minstep : minobj : omp_out = omp_in.transform(omp_out) )
		Permutation Q = *this;	
		#pragma omp parallel num_threads( optimizationContext-> getThreads() ) shared(min,firsts,seconds,couple_size,Q) default(none) 
		{
			// COSTRUISCO UNA COPIA DURANTE LA CREAZIONE DEL THREAD
			Permutation P = Q;												
			#pragma omp for reduction(minstep : min) schedule(static)
			for(int x  = 0 ; x < couple_size; x++)
			{
				int i = firsts[x];
				int j = seconds[x];
				{
					
					
					// genero una copia della permutazione con lo swap associato alle iterazioni del loop
					P.data.inplaceSwap(i,j);

					// valuto la permutazione ottenuta
					auto curr = minobj( optimizationContext->relaxedEvaluation(P) , i , j);
					
					// se il valore valutato risulta minore del minimo , aggiorno le coordinate della direzione favorita e di conseguenza il minimo
				
					min = min.transform(curr);

					// sfrutto il fatto, che nel sottogruppo delle permutazioni di distanza uno dall'identitá
					// ogni permutazione ha come elemento inverso se stessa
					P.data.inplaceSwap(i,j);
				}
			}
		}
		int best_a = min.getA();
		int best_b = min.getB();
		ret.data.inplaceSwap(best_a,best_b);
		return ret;
	}
	

/* Operatori */
	
	bool Permutation::operator < (const Permutation & other) const
	{
		for(int i = 0 ; i < other.size();i++)
		{
			if( data.cgetLower()[i] < other.data.cgetLower()[i])
				return true;
			else
				if( data.cgetLower()[i] > other.data.cgetLower()[i])
					return false;
		}
		return false;
	}

	Permutation Permutation::operator * (double other)
	{		
		return *this;						// il semigruppo delle permutazioni
											// non ha un operazione di moltiplicazione
	}


	Permutation Permutation::operator *= (double other)
	{		
		return *this;						// il semigruppo delle permutazioni
											// non ha un operazione di moltiplicazione
	}
	
	
	bool Permutation::operator == (Permutation & other)
	{
		for(int i = 0 ; i < other.size();i++)
		{
			if ( other.data.getLower()[ other.data.getUpper()[i] ] != data.getLower()[data.getUpper()[i]] )
				return false;
		}
		return true;
	}
	
	Permutation Permutation::operator + ( Permutation & other)
	{
		Permutation ret = *this;
		ret += other;
		//ret.canonicalNeighbourhood = other.canonicalNeighbourhood;
		ret.optimizationContext = other.optimizationContext;
		return ret;
	}
	
	Permutation & Permutation::operator += (Permutation & other) {
		
		// definisco l'operazione di somma come composizione delle permutazioni
		// si parla infatti di SEMIgruppo delle permutazioni per via della non commutativitá
		// della composizione di funzioni, che peró risulta essere associativa.
		
		//data.getLower() = other.apply(data.getLower());
		other.inplaceApply(data.getLower());	
		return *this;
	}
	
/// metodi specializzati

		int Permutation::size() const {
			return data.size();
		}
		
// visualizzazione

	
		void Permutation::show() const {
			std::cout << std::endl;
			int n = size();
			for(int i = 0 ; i < n;i++)
			{
				std::cout << data.cgetUpper()[i] << ( (i < (n-1))? std::setw(5) : std::setw(0));
			}
				std::cout << std::endl;
			for(int i = 0 ; i < n;i++)
			{
				std::cout << data.cgetLower()[i] << ( (i < (n-1))? std::setw(5) : std::setw(0));
			}
				std::cout << std::endl;
		}

// flattening

	const std::vector<int> & Permutation::flatten() {
			return data.cgetLower();
	}
		
Neighbourhood<Permutation> Permutation::canonicalNeighbourhood;

