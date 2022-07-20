#include <iostream>
using namespace std;

#include <ctime>

#include <Eigen/Core>

#include <Eigen/Dense>
using namespace Eigen;

#define MATRIX_SIZE 50

int main(int argc, char **argv)
{
    // definition
    // 2x3 martix
    Matrix<float, 2, 3> matrix_23;
    // 3x1 martix
    Vector3d v_3d;
    Matrix<float, 3, 1> vd_3d;
    // 3x3 0s
    Matrix3d matrix_33 = Matrix3d::Zero();
    // not sure size
    Matrix<double, Denamic, Dynamic> matrix_dynamic;
    MatrixXd matrix_x;

    // operation
    matrix_23 << 1,2,3,4,5,6;
    cout << "matrix 2x3 1->6: \n" << matrix_23 << endl;
}