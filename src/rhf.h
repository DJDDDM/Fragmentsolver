#include <string>
#include <iomanip>
#include <iostream>
#include "rhf_equations.h"

#ifndef RHF_H_
#define RHF_H_

class rhf{
public:
	rhf();
	rhf(std::string FCIDUMP);
	rhf(std::string FCIDUMP,int Electronadd);
	void compute();
	double getEnergy();
private:
	rhf_equations equations;
	double Energy;
	double oldEnergy;
	int maxIters;
};



#endif /* RHF_H_ */
