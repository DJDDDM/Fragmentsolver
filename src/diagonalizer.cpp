#include "diagonalizer.h"

void diagonalizer::setMatrix(Tensor Matrix){
	this->Matrix = Matrix;
}

void diagonalizer::compute(){
	Eigenmatrix = transform(Matrix);

	Eigen::EigenSolver<Eigen::MatrixXd> Eigenobject(Eigenmatrix);
	Eigen::MatrixXcd Eigenvectorscomplex = Eigenobject.eigenvectors();
	Eigen::VectorXcd Eigenvaluescomplex = Eigenobject.eigenvalues();

	Eigen::MatrixXd Eigenvectorsreal = Eigenvectorscomplex.real();
	Eigen::VectorXd Eigenvaluesreal = Eigenvaluescomplex.real();

	Eigen::VectorXd Sortperm = sortperm(Eigenvaluesreal);
	Eigen::VectorXd sortedEigenvalues = sort(Eigenvaluesreal, Sortperm);
	Eigen::MatrixXd sortedEigenvectors = sort(Eigenvectorsreal, Sortperm);

	Eigenvectors = transform(sortedEigenvectors);
	Eigenvalues = transform(sortedEigenvalues.asDiagonal());
}

Tensor diagonalizer::getEigenvalues(){
	return Eigenvalues;
}

Tensor diagonalizer::getEigenvectors(){
	return Eigenvectors;
}

Tensor diagonalizer::getMatrix(){
	return Matrix;
}

Tensor diagonalizer::transform(Eigen::MatrixXd Eigenmatrix){
	Tensor Tacomatrix({Eigenmatrix.cols(),Eigenmatrix.rows()},taco::Format({taco::Dense,taco::Dense}));
	int counter = 0;
	for (int i=0; i < Eigenmatrix.cols(); i++){
		for (int j=0; j < Eigenmatrix.rows(); j++){
			Tacomatrix.insert({i,j}, Eigenmatrix(i,j));
			counter ++;
		}
	}
	Tacomatrix.pack();
	return Tacomatrix;

}

Eigen::MatrixXd diagonalizer::transform(Tensor Tacomatrix){
	Eigen::MatrixXd Eigenmatrix(Tacomatrix.getDimension(0),Tacomatrix.getDimension(1));
	int counter = 0;
	for (int i=0; i < Tacomatrix.getDimension(0); i++){
		for (int j=0; j < Tacomatrix.getDimension(1); j++){
			Eigenmatrix(i,j) = ((double*)Tacomatrix.getStorage().getValues().getData())[counter];
			counter ++;
		}
	}
	return Eigenmatrix;
}

Eigen::VectorXd diagonalizer::sortperm(Eigen::VectorXd Vector){
	Eigen::VectorXd Sortperm(Vector.rows());
	double store;
	for (int i = 0; i < Vector.rows(); i++){
		Sortperm(i) = i;
	}
	for (int j = 0; j < Vector.rows(); j++){
	for (int i = j-1; i >= 0; i--){
		if (Vector(i) > Vector(i+1)){
				store = Vector(i);
				Vector(i) = Vector(i+1);
				Vector(i+1) = store;

				store = Sortperm(i);
				Sortperm(i) = Sortperm(i+1);
				Sortperm(i+1) = store;
			}
		}
	}
	return Sortperm;
}

Eigen::VectorXd diagonalizer::sort(Eigen::VectorXd Vector, Eigen::VectorXd Sortperm){
	Eigen::VectorXd sortedVector(Vector.rows());
	for (int i = 0; i < Vector.rows() ; i++){
		sortedVector(i) = Vector(Sortperm(i));
	}
	return sortedVector;
}

Eigen::MatrixXd diagonalizer::sort(Eigen::MatrixXd Matrix, Eigen::VectorXd Sortperm){
	Eigen::MatrixXd sortedMatrix(Matrix.cols(),Matrix.rows());
	for (int i = 0; i < Matrix.cols() ; i++){
		sortedMatrix.col(i) = Matrix.col(Sortperm(i));
	}
	return sortedMatrix;
}
