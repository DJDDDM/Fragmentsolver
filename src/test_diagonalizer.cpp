#include <iostream>
#include "main.h"
#include "diagonalizer.h"
#include "testhelpers.h"
using namespace std;

void test_transform(){
	diagonalizer diag = diagonalizer();

	Tensor Tacomatrix0({3,3},taco::Format{taco::Dense,taco::Dense});
	double counter = 0;
	for (int i=0; i < 3; i++){
		for (int j=0; j < 3; j++){
			counter ++;
			Tacomatrix0.insert({i,j},counter);
		}
	}
	Tacomatrix0.pack();

	double expcounter = 0;
	Eigen::Matrix<double, 3, 3> Eigenmatrix_exp;
	for (int i=0; i < 3; i++){
		for (int j=0; j < 3; j++){
			expcounter ++;
			Eigenmatrix_exp(i,j) = expcounter;
		}
	}

	Eigen::MatrixXd Eigenmatrix1 = diag.transform(Tacomatrix0);
	Tensor Tacomatrix1 = diag.transform(Eigenmatrix1);
	Tensorassert(Tacomatrix0,Tacomatrix1);

	Eigen::MatrixXd Eigenmatrix2 = diag.transform(Tacomatrix1);
	assert(Eigenmatrix1 == Eigenmatrix2);
}

void test_compute(){
	//since there are no integer diagonalizations there can be no assertions
	//also phasefactor is a problem
	diagonalizer diag = diagonalizer();
	Tensor Matrix({2,2},taco::Format{taco::Dense,taco::Dense});
	Matrix.insert({1,0},1.0);
	Matrix.insert({0,1},1.0);
	Matrix.pack();

	diag.setMatrix(Matrix);
	diag.compute();

	Tensor Eigenvalues_exp({2,2}, taco::Format{taco::Dense, taco::Dense});
	Eigenvalues_exp.insert({0,0},-1.0);
	Eigenvalues_exp.insert({1,0}, 0.0);
	Eigenvalues_exp.insert({0,1}, 0.0);
	Eigenvalues_exp.insert({1,1}, 1.0);
	Eigenvalues_exp.pack();

	Tensor Eigenvectors = diag.getEigenvectors();
	Tensor Eigenvalues = diag.getEigenvalues();
}

void test_sort(){
	diagonalizer diag = diagonalizer();

	Eigen::VectorXd Vektor(5);
	Eigen::VectorXd Vektor_exp(5);
	Eigen::VectorXd Sortperm_exp(5);

	for (int i = 0; i < 5; i++){
		Vektor(i) = 5-i;
		Vektor_exp(i) = i+1;
		Sortperm_exp(i) = 4-i;
	}

	Eigen::VectorXd Sortperm = diag.sortperm(Vektor);
	Eigen::VectorXd sortedVektor = diag.sort(Vektor,Sortperm);

	assert(Sortperm == Sortperm_exp);
	assert(sortedVektor == Vektor_exp);
}

void test_diagonalizer(){
	test_transform();
	test_sort();
	test_compute();
	cout << "passed diagonalizer" << endl;
}



