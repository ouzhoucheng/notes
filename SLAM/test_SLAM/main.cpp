#include <iostream>
using namespace std;

#include <ctime>

#include <eigen3/Eigen/Core>

#include <eigen3/Eigen/Dense>
using namespace Eigen;

#define MATRIX_SIZE 50

int main(int argc, char **argv)
{
    // definition
    // 2x3 martix
    Matrix<float, 2, 3> matrix_23;
    matrix_23 << 1,2,8,4,2,6;   //  input data
    cout << "matrix 2x3 1->6: \n" << matrix_23 << endl;
    // 3x1 martix
    Vector3d v_3d;
    Matrix<float, 3, 1> vd_3d;
    vd_3d << 1,2,3;
    cout << "matrix 3x1 1->3: \n" << vd_3d << endl;
    // 3x3 0s
    Matrix3d matrix_33 = Matrix3d::Zero();
    cout << "matrix 3x3 0: \n" << matrix_33 << endl;
    // not sure size
    Matrix<double, Dynamic, Dynamic> matrix_dynamic;
    MatrixXd matrix_x;
    // matrix_dynamic << 1,2,3,4,5,6,7,8,9;
    // cout << "matrix xxx 12345: \n" << matrix_dynamic << endl;

    // operation
    // visit the elements
    cout<<"matrix 2x3 1->6"<<endl;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cout<<matrix_23(i,j)<<"\t";
        }
        cout<<endl;
    }
    // times
    v_3d << 3,2,1;
    vd_3d << 4,5,6;
    Matrix<double, 2, 1>result = matrix_23.cast<double>() * v_3d;
    cout << "[1,2,3;4,5,6]*[3;2,1]=" << result << endl;
    
    // calculation
    matrix_33 = Matrix3d::Random();
    cout << "random matrix: \n" << matrix_33 << endl;
    cout << "transpose: \n" << matrix_33.transpose() << endl;
    cout << "sum: \n" << matrix_33.sum() << endl;
    cout << "trace: \n" << matrix_33.trace() << endl;
    cout << "times 10: \n" << 10 * matrix_33 << endl;
    cout << "inverse: \n" << matrix_33.inverse() << endl;
    cout << "det: \n" << matrix_33.determinant() << endl;
    // eigen
    SelfAdjointEigenSolver<Matrix3d> eigen_solver(matrix_33.transpose() * matrix_33);
    cout << "Eigen values: \n" << eigen_solver.eigenvalues() << endl;
    cout << "Eigen vectors: \n" << eigen_solver.eigenvectors() << endl;
    // solve formula
    Matrix<double, MATRIX_SIZE, MATRIX_SIZE> matrix_NN = MatrixXd::Random(MATRIX_SIZE, MATRIX_SIZE);
    matrix_NN = matrix_NN * matrix_NN.transpose();
    Matrix<double, MATRIX_SIZE, 1>v_Nd = MatrixXd::Random(MATRIX_SIZE, 1);
    // direct inverse
    Matrix<double, MATRIX_SIZE, 1> x = matrix_NN.inverse() * v_Nd;
    cout << "x = " << x.transpose() << endl;
    // QR decomposition
    x = matrix_NN.colPivHouseholderQr().solve(v_Nd);
    cout << "x = " << x.transpose() << endl;
    // cholesky decomposition
    x = matrix_NN.ldlt().solve(v_Nd);
    cout << "x = " << x.transpose() << endl;
}
