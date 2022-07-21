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
    cout<<"matrix 2x3 1->6"<<endl;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cout<<matrix_23(i,j)<<"\t";
        }
        cout<<endl;
    }
    
}
