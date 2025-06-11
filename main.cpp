/*
Author: Mpilo Ntaka
Date Code Last Updated: 07 June 2025
Programming Language: C++
Application Program Name: Matrix Calculator
Application Purpose: To model matrices and perform various operations and transformations on them

TO DO next: Create utility display functions in seperate .h and .cpp files 

In main.cpp code example problems/bugs:
    matrix3 = matrix1 + matrix2;
    matrix3 = matrix1 - matrix2;
    matrix3 = matrix1 * matrix2;

The above example objects are Matrix objects
*/

#include "Matrix.h"
#include "State_Vector.h"    

int main() {
    srand(time(nullptr));   //Generates different random values on each program execute

    int noRows, noCols;
    cout << "Enter the number of rows: ";
    cin >> noRows;
    cout << "Enter the number of columns: ";
    cin >> noCols;

    Matrix augMatrix(noRows, noCols, "Augmented", Matrix::DETERMINED_ENTRIES);
    cout << "=========== GIVEN AUGMENTED MATRIX ===============" << endl;
    cout << augMatrix << endl;

    Matrix reducedAugMatrix(augMatrix.getMatrix_Rows(), augMatrix.getMatrix_Cols(), "Reduced");
    reducedAugMatrix = augMatrix.Gaussian_Elimination();
    cout << "=========== REDUCED AUGMENTED MATRIX =============" << endl;
    cout << reducedAugMatrix << endl;
    
    cout << "Program ended successfully!!!" << endl;
    Matrix::pauseProgram();
    return 0;
}