#include "rhf_equations.h"

rhf_equations::rhf_equations(){
	rhf_equations(0,0);
}

rhf_equations::rhf_equations(int NORB, int NOCC){
	this->NORB = NORB;
	this->NOCC = NOCC;
	Nuclearenergy = 0;

	initHelpers();
	initTensors();
}

rhf_equations::rhf_equations(string FCIDUMP){
	Electronadd = 0;
	this->FCIDUMP = FCIDUMP;
	initHelpers();
	initReader();
	initConstants();
	initTensors();
	initTWO();
	ONE = reader.getONE();
}

rhf_equations::rhf_equations(string FCIDUMP, int Electronadd){
	this->Electronadd = Electronadd;
	this->FCIDUMP = FCIDUMP;
	initHelpers();
	initReader();
	initConstants();
	initTensors();
	initTWO();
	ONE = reader.getONE();
}

void rhf_equations::initHelpers(){
	iam2 = Tensor(2.0);
	Densitycompiled = false;
	Energycompiled = false;
	Fockcompiled = false;
}

void rhf_equations::initTensors(){
	NOCCRange = taco::Tensor<double>({NORB,NORB},taco::Format({taco::Sparse,taco::Sparse}));
	initRange();
	Density = taco::Tensor<double>({NORB,NORB},taco::Format({taco::Dense,taco::Sparse}));
	Fock = taco::Tensor<double>({NORB,NORB},taco::Format({taco::Dense,taco::Dense}));
	Energy = Tensor();
	AOtoCO = taco::Tensor<double>({NORB,NORB},taco::Format({taco::Dense,taco::Dense}));
	ONE = taco::Tensor<double>({NORB,NORB},taco::Format({taco::Dense,taco::Sparse}));
	TWO = taco::Tensor<double>({NORB,NORB,NORB,NORB},taco::Format({taco::Dense,taco::Sparse,taco::Sparse,taco::Sparse}));
	two = taco::Tensor<double>({NORB,NORB,NORB,NORB},taco::Format({taco::Dense,taco::Sparse,taco::Sparse,taco::Sparse}));
}

void rhf_equations::initRange(){
	for (int i = 0; i < NOCC; i++){
		NOCCRange.insert({i,i}, 1.0);
	}
	NOCCRange.pack();
}

void rhf_equations::initReader(){
	reader = ReadFCIDUMP();
	reader.setElectronadd(Electronadd);
	reader.openfile(FCIDUMP);
	reader.fillarrays();
}

void rhf_equations::initTWO(){
	two = reader.gettwo();
	calcTWO();
}

void rhf_equations::initConstants(){
	NORB = reader.getNORB();
	NOCC = reader.getNELEC()/ 2;
	Nuclearenergy = reader.getNuclearenergy();
}

void rhf_equations::calcTWO(){
	taco::IndexVar i("i"),j("j"),k,l;
	// minus needs to stay in front for getting minus in the result
	//Taco can't deal with nonlinear indizes, therefore transpose has to be used.
	TWO(i,j,k,l) = - two.transpose({0,3,2,1})(i,j,k,l) + two(i,j,k,l) * iam2();
	TWO.evaluate();
}

void rhf_equations::initialguess(){
	for (int i = 0; i < NORB; i++){
		AOtoCO.insert({i,i},1.0);
	}
	AOtoCO.pack();
}

void rhf_equations::calculateDensity(){
	taco::IndexVar i,j,k,l;
	//since Taco doesn't have Slicing the corresponding Range-Matrix is used
	//since Taco can't deal with Matrix(i,i) one more Index needed
	Density(i,j) = AOtoCO(i,l)*AOtoCO(j,k)*NOCCRange(k,l);
	if (!Densitycompiled){
		Density.compile();
		Densitycompiled = true;
	}
	Density.assemble();
	Density.compute();
}

void rhf_equations::calculateFock(){
	taco::IndexVar i("i"),j,k,l;
	//Taco can't deal with nonlinear indizes, therefore transpose has to be used.
	Fock(i,j) = ONE(i,j) + Density.transpose({1,0})(k,l)*TWO(i,j,k,l);
	if (!Fockcompiled){
		Fock.compile();
		Fockcompiled = true;
	}
	Fock.assemble();
	Fock.compute();
}

void rhf_equations::calculateAOtoCO(){
	diagonalizer diag = diagonalizer();
	diag.setMatrix(Fock);
	diag.compute();
	AOtoCO = diag.getEigenvectors();
}

void rhf_equations::calculateEnergy(){
	taco::IndexVar i,j;
	Energy = Density.transpose({1,0})(i,j)*(ONE(i,j) + Fock(i,j));
	if (!Energycompiled){
		Energy.compile();
		Energycompiled = true;
	}
	Energy.assemble();
	Energy.compute();
}

void rhf_equations::setAOtoCO(taco::Tensor<double> AOtoCO){
	this->AOtoCO = AOtoCO;
}

void rhf_equations::setDensity(taco::Tensor<double> Density){
	this->Density = Density;
}


taco::Tensor<double> rhf_equations::getDensity(){
	return Density;
}

void rhf_equations::setNELECRange(Tensor NOCCRange){
	this->NOCCRange = NOCCRange;
}

void rhf_equations::setONE(Tensor ONE){
	this->ONE = ONE;
}

void rhf_equations::settwo(Tensor two){
	this->two = two;
}

void rhf_equations::setTWO(Tensor TWO){
	this->TWO = TWO;
}


Tensor rhf_equations::getTWO(){
	return TWO;
}

Tensor rhf_equations::gettwo(){
	return two;
}

Tensor rhf_equations::getONE(){
	return ONE;
}

void rhf_equations::setFock(Tensor Fock){
	this->Fock = Fock;
}

Tensor rhf_equations::getFock(){
	return Fock;
}

double rhf_equations::getEnergy(){
	double Electronenergy = getScalar(Energy);
	return Electronenergy + Nuclearenergy;
}

double rhf_equations::getNuclearenergy(){
	return Nuclearenergy;
}

double rhf_equations::getScalar(Tensor Tensor){
	return ((double*)Tensor.getStorage().getValues().getData())[0];
}

void rhf_equations::printTensor(taco::Tensor<double> Output){
	for (unsigned int i = 0; i < Output.getStorage().getValues().getSize(); i++){
		std::cout << static_cast<const double*>(Output.getStorage().getValues().getData())[i] << std::endl;
	}
}


