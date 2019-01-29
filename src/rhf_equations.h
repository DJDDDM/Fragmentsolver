#include "taco.h"
#include "diagonalizer.h"
#include "ReadFCIDUMP.h"
#include <eigen3/Eigen/Eigenvalues>

#ifndef RHF_EQUATIONS_H_
#define RHF_EQUATIONS_H_

typedef taco::Tensor<double> Tensor;

class rhf_equations{
public:
	rhf_equations();
	rhf_equations(int NORB, int NELEC);
	rhf_equations(std::string FCIDUMP);
	rhf_equations(std::string FCIDUMP,int Nelecadd);

	void calcTWO();
	void initialguess();

	void calculateAOtoCO();
	void calculateDensity();
	void calculateFock();
	void calculateEnergy();

	void setAOtoCO(taco::Tensor<double> AOtoCO);
	void setDensity(taco::Tensor<double> Density);
	void setONE(Tensor ONE);
	void settwo(Tensor two);
	void setTWO(Tensor TWO);
	void setFock(Tensor Fock);
	void setNELECRange(Tensor NELECRange);

	Tensor getTWO();
	Tensor gettwo();
	Tensor getONE();
	Tensor getFock();
	double getEnergy();
	double getNuclearenergy();
	Tensor getDensity();

	double getScalar(Tensor Tensor);
	void printTensor(Tensor Output);

private:
	void initReader();
	void initHelpers();
	void initConstants();
	void initTensors();
	void initTWO();
	void initRange();

	int NORB;
	int NOCC;
	double Nuclearenergy;
	Tensor Density;
	Tensor AOtoCO;
	Tensor NOCCRange;
	Tensor ONE; //
	Tensor TWO; //spin adapted twoelectron matrix
	Tensor two; //not spin adapted
	Tensor Fock; //Fockmatrix
	Tensor Energy;
	Tensor iam2;
	string FCIDUMP;
	ReadFCIDUMP reader;

	bool Densitycompiled;
	bool Energycompiled;
	bool Fockcompiled;
	int Electronadd;

};




#endif /* RHF_EQUATIONS_H_ */
