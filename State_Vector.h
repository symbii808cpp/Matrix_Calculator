#ifndef _STATE_VECTOR_H_
#define _STATE_VECTOR_H_

#include "Matrix.h"

class State_Vector: public Matrix {
    friend ostream& operator<<(ostream& insertion_op, const State_Vector& rhs_vector);

private:
    static const int VECTOR_NUMBER_OF_ROWS = 3;
    static const int VECTOR_NUMBER_OF_COLS = 1;
    float *stateVector = nullptr;

    void data_entryProcedure(const int& rows);
    float Inner_Product(const Matrix& lhs_vector, const Matrix& rhs_vector);
    Matrix Outer_Product(const Matrix& lhs_vector, const Matrix& rhs_vector);
    State_Vector Vector_Transform(const Matrix& lhs_matrix, State_Vector& rhs_vector);

    virtual void zero_outMatrix(Matrix& objMatrix) override;

public:
    //Creates a (rows x cols) matrix with determined entries
    State_Vector(int rows, string matrix_name, bool); 
    State_Vector(string matrix_name);               //Creates a random 3x1 vector
    State_Vector(int rows, string matrix_name);     //Creates a random (rows x 1) vector 
    virtual ~State_Vector();

    inline bool IS_VECTOR() const { return true; }
    void rescaleMatrix(const Matrix& matrixToCopy);
    void zero_outMatrix();

    float* getState_Vector() const;

    Matrix& operator=(const Matrix& rhs_matrix);    //Overloaded Copy Assignment Operators
    Matrix& operator=(float&& rhs_const_number);

    Matrix operator+(const Matrix& rhs_matrix);     //Overloaded Addition Operators
    Matrix operator+(float&& rhs_const_number);
    Matrix operator-(const Matrix& rhs_matrix);     //Overloaded Subtraction Operators
    Matrix operator-(float&& rhs_const_number);
    variant<Matrix, float> operator*(const Matrix& rhs_matrix);      //Overloaded Multiplication Operators      
    Matrix operator*(float&& rhs_const_number);     
    
    Matrix& operator+=(const Matrix& rhs_matrix);   //Overloaded Self Addition Operators
    Matrix& operator+=(float&& rhs_const_number);
    Matrix& operator-=(const Matrix& rhs_matrix);   //Overloaded Self Subtraction Operators
    Matrix& operator-=(float&& rhs_const_number);
    Matrix& operator*=(const Matrix& rhs_matrix);   //Overloaded Self Multiplication Operators
    Matrix& operator*=(float&& rhs_const_number);
 
    //Overloaded Comparison Operators
    bool operator==(const Matrix& rhs_matrix) const;
    bool operator!=(const Matrix& rhs_matrix) const;
    bool operator>=(const Matrix& rhs_matrix) const;
    bool operator<=(const Matrix& rhs_matrix) const;
    bool operator<(const Matrix& rhs_matrix) const;
    bool operator>(const Matrix& rhs_matrix) const;
};

#endif