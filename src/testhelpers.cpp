#include "testhelpers.h"
#include "taco.h"


void Tensorassert(Tensor Actual, Tensor Expected){
	for (unsigned int i = 0; i < Actual.getStorage().getValues().getSize(); i++){
		double actual = static_cast<const double*>(Actual.getStorage().getValues().getData())[i];
		double expected = static_cast<const double*>(Expected.getStorage().getValues().getData())[i];
		assert(actual == expected);
	}
}



