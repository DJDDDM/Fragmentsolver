#include <iostream>
#include <fstream>
#include "ReadFCIDUMP.h"

using namespace std;

void ReadFCIDUMP::openfile(){
	if (name == ""){
		name = defaultname;
		cerr << "empty name error while opening the file" << endl;
		cerr << "name was set to" << defaultname << endl;
	}
	FCIDUMP.open(name);
	if (!FCIDUMP.is_open()){
		cerr << "file '" << name << "' does not exist" << endl;
		cerr << "exiting programm" << endl;
		exit(0);
	}
}

void ReadFCIDUMP::openfile(string name){
	this->name = name;
	openfile();
}

void ReadFCIDUMP::closefile(){
	FCIDUMP.close();
}

void ReadFCIDUMP::setname(string newname){
	name = newname;
}

string ReadFCIDUMP::getname(){
	return name;
}

string ReadFCIDUMP::getcurrentline(){
	return currentline;
}

int ReadFCIDUMP::getNORB(){
	return NORB;
}

int ReadFCIDUMP::getNELEC(){
	return NELEC;
}

Tensor ReadFCIDUMP::getONE(){
	return ONE;
}

Tensor ReadFCIDUMP::gettwo(){
	return two;
}

double ReadFCIDUMP::getNuclearenergy(){
	return Nuclearenergy;
}

void ReadFCIDUMP::readnextline(){
	getline(FCIDUMP,currentline);
}

void ReadFCIDUMP::skipnamelist(){
	while (!(currentline.find("/") != string::npos)){
		readnextline();
	}
}

void ReadFCIDUMP::readnamelist(){
	readnextline();
	NORB = stoi(stringbetween("NORB=", ","));
	NELEC = stoi(stringbetween("NELEC=", ",")) + Electronadd;
	cout << "Number of Orbitals: " << NORB << endl;
	cout << "Number of Electrons: " << NELEC << endl;
	skipnamelist();
}

string ReadFCIDUMP::stringbetween(string key1, string key2){
		int first = currentline.find(key1) + key1.size();
		int second = currentline.substr(first).find(",");
		string between = currentline.substr(first,second);
	return between;
}

void ReadFCIDUMP::readintegral(){
	integral = stod(currentline.substr(0,24));
}

void ReadFCIDUMP::readindizes(){
	first = stoi(currentline.substr(24,4));
	second = stoi(currentline.substr(28,4));
	third = stoi(currentline.substr(32,4));
	fourth = stoi(currentline.substr(36,4));
}

int ReadFCIDUMP::getindize(int number){
	if (number == 1){
		return first;
	} else if (number == 2) {
		return second;
	} else if (number == 3) {
		return third;
	} else if (number == 4) {
		return fourth;
	} else {
		cerr << "invalid call for indize" << endl;
	}
	return 0;
}

double ReadFCIDUMP::getintegral(){
	return integral;
}


void ReadFCIDUMP::initarrays(){
	two = Tensor({NORB,NORB,NORB,NORB},taco::Format({taco::Dense, taco::Sparse, taco::Sparse, taco::Sparse}));
	ONE = Tensor({NORB,NORB},taco::Format({taco::Dense, taco::Sparse}));
}

void ReadFCIDUMP::progressnextline(){
	readnextline();
	readintegral();
	readindizes();
}

void ReadFCIDUMP::fillarrays(){
	readnamelist();
	initarrays();

	progressnextline();
	while (fourth != 0){
		two.insert({first-1,second-1,third-1,fourth-1},integral);
		two.insert({second-1,first-1,third-1,fourth-1},integral);
		two.insert({second-1,first-1,fourth-1,third-1},integral);
		two.insert({first-1,second-1,fourth-1,third-1},integral);

		two.insert({third-1,fourth-1,first-1,second-1},integral);
		two.insert({fourth-1,third-1,first-1,second-1},integral);
		two.insert({fourth-1,third-1,second-1,first-1},integral);
		two.insert({third-1,fourth-1,second-1,first-1},integral);

		progressnextline();
	}
	two.pack();
	while(second != 0){
		ONE.insert({first-1,second-1},integral);
		ONE.insert({second-1,first-1},integral);
		progressnextline();
	}
	ONE.pack();
	while(first != 0){
		progressnextline();
	}
	if (first == 0 and second == 0) Nuclearenergy = integral;

	closefile();
}

void ReadFCIDUMP::setElectronadd(int Electronadd){
	this->Electronadd = Electronadd;
}
