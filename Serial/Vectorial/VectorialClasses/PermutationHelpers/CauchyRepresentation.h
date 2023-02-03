
#include <vector>
#include <algorithm>

#ifndef CAUCHYREPRESENTATION_H
#define CAUCHYREPRESENTATION_H

class CauchyRepresentation
{
	/*  Classe costruita per contenere la rappresentazione bivettoriale
	*   di una permutazione. É stata trattata separatamente dal wrapper
	*   Permutation in modo da avere dei metodi che sono formalmente parte
	*	della rappresentazione matriciale 
	*/
	public:

/***Costruttori**************************************************************************************************/
		
		CauchyRepresentation(int n) : upper(n) , lower(n)
		{
			for(int i = 0 ; i < n ; i++)
			{
				upper[i] = i;
				lower[i] = i;
			}
		}
		
		CauchyRepresentation(std::vector<int> & upper, 					// permutazione partendo da vettori
							 std::vector<int> & lower
							) : upper(upper) , lower(lower) {};
	
/***Getter*******************************************************************************************************/

		std::vector<int> & getLower();
		std::vector<int> & getUpper();
		
/***Getter di uso interno****************************************************************************************/

		// getter in verrsione const per permettere l'ottimizzazione
		// da parte del compilatore
			
		const std::vector<int> & cgetUpper() const;
		const std::vector<int> & cgetLower() const;
		int size() const;
		
/***Swap*********************************************************************************************************/
		
		CauchyRepresentation swap(int a,int b) const;
		CauchyRepresentation & inplaceSwap(int a,int b);
		
/****************************************************************************************************************/
/****************************************************************************************************************/
/****************************************************************************************************************/
	
	private:
		std::vector<int> upper;
		std::vector<int> lower;
};
#endif