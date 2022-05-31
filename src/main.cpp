#include <iostream>
#include <string>
#include <chrono>
#include "Calculation.h"
#include <Eigen/Dense>
#include <Eigen/Cholesky>
#include <Eigen/LU>
#include <Eigen/QR>
#include <Eigen/SVD>
#include <Eigen/IterativeLinearSolvers>

using namespace std;
using namespace Eigen;

const int gridNumOfX = 5;
const int gridNumOfY = 2;
const int gridNumOfZ = 3;

double frontEfficiency = 4.9665e-06;
double colEfficiency =  6.225e-07;
double rowEfficiency = 6.55e-07;
double diaEfficiency = 6.55E-07;

void cal(MatrixXd& E, int x, int z, int col) {
    MatrixXd Ei = MatrixXd::Zero(3, 5);
    for (int i = x-1;  i <= x+1; i++) {
        for (int j = z-1;  j <= z+1; j++) {
            if (i < 0 || j < 0 || i >= gridNumOfX || j >= gridNumOfZ) continue;
            if (i == x && j == z) {
                Ei(j, i) = frontEfficiency;
            } else if (i == x && j != z) {
                Ei(j, i) = colEfficiency;
            } else if (i != x && j == z) {
                Ei(j, i) = rowEfficiency;
            } else if (i != x && j != z) {
                Ei(j, i) = diaEfficiency;
            }
        }
    }
    Ei.resize(1,15);
    E.row(col) = Ei;
//    cout << col << "-----------------" << endl;
//    cout << E << endl;
}

int main() {
    MatrixXd C = MatrixXd::Zero(gridNumOfZ, gridNumOfX);
    MatrixXd A = MatrixXd::Zero(15, 1);
    MatrixXd E = MatrixXd::Zero(15, 15);
    C  <<   0,	7.45E-07,	7.00E-07,	6.05E-07,	0.00E+00,
            0,	6.55E-07,	4.89E-06,	6.10E-07,	0,
            0,	6.10E-07,	7.15E-07,	6.55E-07,	0.00E+00;
//    C  <<   0,	6.5E-07,	4.885E-06,	6.1E-07,	0.00E+00,
//            0,	6.1E-07,	7.1E-07,	6.55E-07,	0,
//            0,	0,	0,	0,	0.00E+00;
//    C  <<   0,	7.00E-08,	7.00E-08,	7.00E-08,	0,
//            0,	7.00E-08,	1.1E-07,	7.00E-08,	0,
//            0,	7.00E-08,	7.00E-08,	7.00E-08,	0.00E+00;
    C = C * 1e8;
    C.resize(15,1);
    cout << C << endl;
    int col = 0;
    for (int i = 0; i < gridNumOfX; i++) {
        for (int j = 0; j < gridNumOfZ; j++) {
            cal(E, i, j, col);
            col++;
        }
    }
    SparseMatrix<double> E1(15, 15);
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (E(i,j) != 0) E1.insert(i,j) = E(i,j);
        }
    }
    cout << E1 << endl;



    BiCGSTAB<SparseMatrix<double>> solver;
    solver.compute(E1);
    A = solver.solve(C);
    std::cout << "#iterations:     " << solver.iterations() << std::endl;
    std::cout << "estimated error: " << solver.error()      << std::endl;
    std::cout << "The activity matrix is:\n" << A << std::endl;
    std::cout << "The total activity is:\n" << A.sum() << std::endl;

//    ConjugateGradient<SparseMatrix<double>, Lower|Upper> solver;
//    solver.compute(E1);
//    A = solver.solve(C);
//    std::cout << "#iterations:     " << solver.iterations() << std::endl;
//    std::cout << "estimated error: " << solver.error()      << std::endl;
//    std::cout << "The activity matrix is:\n" << A << std::endl;
//    std::cout << "The total activity is:\n" << A.sum() << std::endl;

}



void calculate() {
    Calculation CalculationB;
    CalculationB.TypeOfCal=0;
    CalculationB.IsOnCal= true;

    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

    CalculationB.StartDoing();
    CalculationB.m_FileName = "";
    std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
    std::cout << "Run Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << "ms.\n";
}
