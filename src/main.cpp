#include "main.h"
#include <iostream>
#include <iomanip>
#include "rhf.h"
using namespace std;

void testsuite(){
	test_diagonalizer();
	test_rhf_equations();
	test_rhf();
	test_ReadFCIDUMP();
	cout << "passed all" << endl;
}

int main(){
	bool Debug = true;
	std::cout << std::fixed << std::setprecision(7);
	if (Debug){
		testsuite();
	} else {
		cout << "which FCIDUMP?:" << endl;
		string FCIDUMP;
		cin >> FCIDUMP;
		cout << "How many Electrons(even number cause RHF) do you like to add?:" << endl;
		int Electronadd;
		cin >> Electronadd;
		rhf RHF = rhf(FCIDUMP,Electronadd);
		RHF.compute();
		double RHF_Energy = RHF.getEnergy();
		cout << "The Energy is " << RHF_Energy << endl;
	}
}



