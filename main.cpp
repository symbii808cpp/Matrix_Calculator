/*
Author: Mpilo Ntaka
Date Code Last Updated: 04 May 2025
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

void Pause() {
    cin.ignore(100, '\n');
    cout << "Press the Enter Key to continue" << endl;
    cin.get();
}

int main() {
    srand(time(nullptr));   //Generates different random values on each program execute
    State_Vector vector1("Random (3 x 1)");
    State_Vector vector2(6, "Random (6 x 1)");
    State_Vector vector3(3, "User-Defined", Matrix::DETERMINED_ENTRIES);

    cout << vector1 << endl;
    cout << vector2 << endl;
    cout << vector3 << endl;

    cout << "===========ADDITION AND SUBTRACTION===================" << endl;
    State_Vector vector4("Random Vector 4");
    vector4 = vector1 + vector3;
    cout << vector4 << endl;
    vector4 = vector1 - vector3;
    cout << vector4 << endl;

    cout << "===========ASSIGNMENT START=================" << endl;
    vector1 = vector3;
    cout << "===========ASSIGNMENT FINISH=================" << endl;

    cout << vector1 << endl;
    cout << vector2 << endl;
    cout << vector3 << endl;
    
    Pause();
    return 0;
}