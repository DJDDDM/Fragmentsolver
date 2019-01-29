#include <iostream>
#include <cassert>
#include "main.h"
#include "ReadFCIDUMP.h"
#include "rhf_equations.h"
#include "rhf.h"
using namespace std;

void test_Water(){
	rhf RHF = rhf("FCIDUMP");
	RHF.compute();
	double E_actual = RHF.getEnergy();
	double E_exp = -75.9839254;
	assert(abs(E_actual-E_exp) < pow(10,-6));
}

void test_Water2plus(){
	rhf RHF = rhf("FCIDUMP",-2);
	RHF.compute();
	double E_actual = RHF.getEnergy();
	double E_exp = -74.5379478;
	assert(abs(E_actual-E_exp) < pow(10,-6));
}

void test_rhf(){
	test_Water();
	test_Water2plus();
	cout << "passed rhf" << endl;
}



