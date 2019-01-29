#include <iostream>
#include "main.h"
#include "taco.h"
#include "rhf_equations.h"
#include "testhelpers.h"
#include <vector>
using namespace std;


void Tensorprint(Tensor Tensor){
	for (unsigned int i = 0; i < Tensor.getStorage().getValues().getSize(); i++){
		cout << static_cast<const double*>(Tensor.getStorage().getValues().getData())[i] << endl;
	}
}

void test_dens(){
	int NORB = 2;
	int NOCC = 1;

	Tensor C({NORB,NORB}, taco::Format({taco::Dense,taco::Dense}));
	double input = 1.0;
	for (int i = 0; i < C.getDimension(0); i++){
		for (int j = 0; j < C.getDimension(1); j++){
			C.insert({i,j},input);
			input++;
		}
	}
	C.pack();

	Tensor Expected({NORB,NORB}, taco::Format({taco::Dense, taco::Dense}));
	Expected.insert({0,0},1.0);
	Expected.insert({0,1},3.0);
	Expected.insert({1,0},3.0);
	Expected.insert({1,1},9.0);
	Expected.pack();

	Tensor Range({NORB,NORB}, taco::Format({taco::Sparse, taco::Sparse}));
	Range.insert({0,0},1.0);
	Range.pack();

	rhf_equations op = rhf_equations(NORB,NOCC);
	op.setAOtoCO(C);
	op.setNELECRange(Range);
	op.calculateDensity();
	Tensor P = op.getDensity();

	Tensorassert(P,Expected);
}

void test_fock(){
	int NORB = 2;
	int NOCC = 1;
	rhf_equations op = rhf_equations(NORB,NOCC);

	Tensor TWO({NORB,NORB,NORB,NORB}, taco::Format({taco::Dense, taco::Sparse, taco::Sparse, taco::Sparse}));
	TWO.insert({0,1,0,0},2.0);
	TWO.insert({0,0,0,1},-1.0);
	TWO.pack();
	op.setTWO(TWO);

	Tensor P({NORB,NORB}, taco::Format({taco::Dense, taco::Sparse}));
	P.insert({1,0},2.0);
	P.pack();
	op.setDensity(P);

	Tensor H({NORB,NORB}, taco::Format({taco::Dense, taco::Sparse}));
	H.insert({0,0},3.0);
	H.pack();
	op.setONE(H);

	Tensor F({NORB,NORB}, taco::Format({taco::Dense, taco::Sparse}));
	op.calculateFock();
	F = op.getFock();

	Tensor F_exp({NORB,NORB}, taco::Format({taco::Dense, taco::Sparse}));
	F_exp.insert({0,0},1.0);
	F_exp.pack();

	Tensorassert(F,F_exp);
	Tensorassert(F_exp,F);
}

void test_TWO(){
	int NORB = 2;
	int NOCC = 1;
	rhf_equations op = rhf_equations(NORB,NOCC);

	Tensor two({NORB,NORB,NORB,NORB}, taco::Format({taco::Dense, taco::Sparse, taco::Sparse, taco::Sparse}));
	two.insert({0,1,0,0},1.0);
	two.pack();
	op.settwo(two);

	Tensor TWO({NORB,NORB,NORB,NORB}, taco::Format({taco::Dense, taco::Sparse, taco::Sparse, taco::Sparse}));
	op.calcTWO();
	TWO = op.getTWO();

	Tensor TWO_exp({NORB,NORB,NORB,NORB}, taco::Format({taco::Dense, taco::Sparse, taco::Sparse, taco::Sparse}));
	TWO_exp.insert({0,1,0,0},2.0);
	TWO_exp.insert({0,0,0,1},-1.0);
	TWO_exp.pack();

	Tensorassert(TWO,TWO_exp);
	Tensorassert(TWO_exp,TWO);
}

void test_Energy(){
	int NORB = 2;
	int NOCC = 1;
	rhf_equations op = rhf_equations(NORB,NOCC);

	taco::IndexVar i,j;
	Tensor P({NORB,NORB}, taco::Format({taco::Dense, taco::Sparse}));
	P.insert({0,0},2.0);
	P.insert({1,1},2.0);
	P.pack();
	op.setDensity(P);

	Tensor H({NORB,NORB}, taco::Format({taco::Dense, taco::Sparse}));
	H.insert({0,1},1.0);
	H.insert({1,0}, 1.0);
	H.pack();
	op.setONE(H);

	Tensor F({NORB,NORB}, taco::Format({taco::Dense, taco::Sparse}));
	F.insert({0,1},2.0);
	F.insert({1,1},3.0);
	F.pack();
	op.setFock(F);

	op.calculateEnergy();
	double Energy = op.getEnergy();
	assert(Energy == 6);
}

void test_rhf_equations(){
	test_dens();
	test_fock();
	test_TWO();
	test_Energy();
	std::cout << "passed rhf_equations" << std::endl;
}
