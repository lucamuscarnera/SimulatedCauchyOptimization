// CauchyRepresentation.cpp
#include "CauchyRepresentation.h"

std::vector<int> & CauchyRepresentation::getLower()  {
			return lower;
		}
std::vector<int> & CauchyRepresentation::getUpper()  {
			return upper;
		}
		

// getter in verrsione const per permettere l'ottimizzazione
// da parte del compilatore
			
const std::vector<int> & CauchyRepresentation::cgetUpper() const {
			return upper;
		}	
const std::vector<int> & CauchyRepresentation::cgetLower() const {
			return lower;
		}

int CauchyRepresentation::size() const {
	return cgetLower().size();
}
// Swap

CauchyRepresentation CauchyRepresentation::swap(int a,int b) const
		{
			std::vector<int> newLower = cgetLower();
			std::vector<int> newUpper = cgetUpper();
			std::swap( newLower[a], newLower[b] );
			return CauchyRepresentation(newUpper,newLower);
		}

CauchyRepresentation & CauchyRepresentation::inplaceSwap(int a,int b)
		{
			std::swap( lower[a], lower[b] );
			return *this;
		}