#include "rhf.h"


rhf::rhf(std::string FCIDUMP){
	equations = rhf_equations(FCIDUMP);
	Energy = 1;
	oldEnergy = 0;
	maxIters = 50;
}

rhf::rhf(std::string FCIDUMP,int Electronadd){
	equations = rhf_equations(FCIDUMP, Electronadd);
	Energy = 1;
	oldEnergy = 0;
	maxIters = 50;
}


void rhf::compute(){

	equations.initialguess();

	cout << "Iteration" << std::setw(10) << "Energy" << std::setw(15) << "Ediff" << endl;
	for (int Iteration = 1; Iteration < maxIters; Iteration++){
		equations.calculateDensity();
		equations.calculateFock();
		equations.calculateAOtoCO();
		equations.calculateEnergy();
		Energy = equations.getEnergy();
		cout << std::setw(5) << Iteration << std::setw(19) << Energy
				<< std::setw(15) << abs(Energy-oldEnergy) << endl;
		if (abs(Energy - oldEnergy) < pow(10, -6)){
			break;
		}
		if (Iteration == maxIters-1){
			cout << "did not converged after " << Iteration << " Iterations";
			break;
		}
		oldEnergy = Energy;
	}
}

double rhf::getEnergy(){
	return Energy;
}
