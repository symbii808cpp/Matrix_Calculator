#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstring>
#include <string>
#include <cstdlib>
#include <variant>
#include <vector>
#include "Utility.h"
using namespace std;

class Matrix {
    friend ostream& operator<<(ostream& insertion_op, const Matrix& rhs_matrix); //TEST PASSED!!!
    friend Matrix operator*(float&& rhs_const_number, const Matrix& rhs_matrix); //TEST PASSED!!!

private:
    int rows, cols;
    string matrix_name;
    float **matrix = nullptr;
    vector<bool> zeroRow_recordLog;

    bool isSquareMatrix() const;

    //==============Gaussian Elimination Utility Functions===================
    void sub_from_and_multiplyRow(Matrix& reduced, const int& targetRow, const int& constRow);
    void multiplyRow(Matrix& reduced, const int& targetRow, const int& start_targetCol);
    void swapRows(Matrix& reduced, const int& firstRow, const int& secondRow);
    bool isZeroRow(const Matrix& reduced, const int& rowIndex) const;
    bool isCanMatrixProgress(const Matrix& reduced, const int& row_absoluteTracker) const;
    //Above functions to be used within a for-loop
    void move_zeroRows(Matrix& reduced);
    void make_leadingVariablesPositive(Matrix& reduced);
    void InfinitlyManySolutions_Message(const Matrix& objMatrix) const;
    void NoSolutionSet_Message(const Matrix& objMatrix) const;
    bool isInfinitlyManySolutions(const Matrix& reduced) const;
    bool isNoSolutionSet(const Matrix& reduced) const;

protected:

    //Class utility functions
    int random_entryGenerator(int upper, int lower);    //TEST PASSED!!!
    void data_entryProcedure(const int& rows, const int& cols); //TEST PASSED!!!
    void dimensionSize_ErrorMessage(const Matrix& lhs_matrix, const Matrix& rhs_matrix) const;  //TEST PASSED!!!
    void multiplication_ErrorMessage(const Matrix& lhs_matrix, const Matrix& rhs_matrix) const; //TEST PASSED!!!

    virtual void zero_outMatrix(Matrix& objMatrix);

public:
    inline static constexpr bool DETERMINED_ENTRIES = true;
    inline static constexpr int EXIT_PROGRAM = -1;

    //Creates a (rows x cols) matrix with determined entries
    Matrix(int rows, int cols, string matrix_name, bool); 
    Matrix(string matrix_name);                         //Creates a random 3x3 square matrix
    Matrix(int rows, int cols, string matrix_name);     //Creates a random (rows x cols) matrix 
    virtual ~Matrix();
    //TESTS PASSED!!! for the above Constructors and Destructor

    void display_matrix() const;  //TEST PASSED!!!
    static void pauseProgram();

    float Trace() const;
    Matrix Transpose() const;
    Matrix Gaussian_Elimination();
    Matrix Gauss_Jordan_Elimination();

    void getDimensionSize_ErrorMessage(const Matrix& lhs_matrix, const Matrix& rhs_matrix) const;
    void getMultiplication_ErrorMessage(const Matrix& lhs_matrix, const Matrix& rhs_matrix) const;
    int getMatrix_Rows() const;
    int getMatrix_Cols() const;
    string getMatrix_Name() const;
    float** getMatrix_ptrMatrix() const;

    virtual float* getState_Vector() const;     
    //NEEDS TESTING (Test this by testing State_Vector::operator=() )

    inline virtual bool IS_VECTOR() const { return false; }
    virtual void rescaleMatrix(const Matrix& matrixToCopy);
    virtual void zero_outMatrix();

    virtual Matrix& operator=(const Matrix& rhs_matrix);    //Overloaded Copy Assignment Operators
    virtual Matrix& operator=(float&& rhs_const_number);
    //TESTS PASSED!!! for the two above operator functions

    virtual Matrix operator+(const Matrix& rhs_matrix);     //Overloaded Addition Operators
    virtual Matrix operator+(float&& rhs_const_number);
    virtual Matrix operator-(const Matrix& rhs_matrix);     //Overloaded Subtraction Operators
    virtual Matrix operator-(float&& rhs_const_number);
    Matrix operator*(const Matrix& rhs_matrix);             //Overloaded Multiplication Operators
    virtual Matrix operator*(float&& rhs_const_number);
    //TESTS PASSED!!! for the six above operator functions
    
    virtual Matrix& operator+=(const Matrix& rhs_matrix);   //Overloaded Self Addition Operators
    virtual Matrix& operator+=(float&& rhs_const_number);
    virtual Matrix& operator-=(const Matrix& rhs_matrix);   //Overloaded Self Subtraction Operators
    virtual Matrix& operator-=(float&& rhs_const_number);
    virtual Matrix& operator*=(const Matrix& rhs_matrix);   //Overloaded Self Multiplication Operators
    virtual Matrix& operator*=(float&& rhs_const_number);
    //TESTS PASSED!!! for the six above operator functions
 
    //Overloaded Comparison Operators
    virtual bool operator==(const Matrix& rhs_matrix) const;
    virtual bool operator!=(const Matrix& rhs_matrix) const;
    virtual bool operator>=(const Matrix& rhs_matrix) const;
    virtual bool operator<=(const Matrix& rhs_matrix) const;
    virtual bool operator<(const Matrix& rhs_matrix) const;
    virtual bool operator>(const Matrix& rhs_matrix) const;
    //TESTS PASSED!!! for the six above operator functions

};

#endif