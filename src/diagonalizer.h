#include "taco.h"
#include <eigen3/Eigen/Eigenvalues>

#ifndef SRC_DIAGONALIZER_H_
#define SRC_DIAGONALIZER_H_

typedef taco::Tensor<double> Tensor;
class diagonalizer{
public:
	void setMatrix(Tensor Matrix);
	Tensor getMatrix();
	void compute();
	Tensor getEigenvectors();
	Tensor getEigenvalues();
	Tensor transform(Eigen::MatrixXd Eigenmatrix);
	Eigen::MatrixXd transform(Tensor Tacomatrix);
	Eigen::VectorXd sortperm(Eigen::VectorXd Vector);
	Eigen::VectorXd sort(Eigen::VectorXd Vector, Eigen::VectorXd Sortperm);
	Eigen::MatrixXd sort(Eigen::MatrixXd Matrix, Eigen::VectorXd Sortperm);
private:
	Tensor Matrix;
	Tensor Eigenvectors;
	Tensor Eigenvalues;
	Eigen::MatrixXd Eigenmatrix;
};




#endif /* SRC_DIAGONALIZER_H_ */
