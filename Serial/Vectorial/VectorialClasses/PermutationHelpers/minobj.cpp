//minobj.cpp
#include "minobj.h"

minobj & minobj::transform(minobj & other)  {
			if(other.val < val) {
				return other;
			}
			return *this;
		};