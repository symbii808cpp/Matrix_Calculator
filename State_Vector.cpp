#include "State_Vector.h"

//Function is COMPLETE
//Constructor to create a random (3 x 1) state vector
State_Vector::State_Vector(string matrix_name) : Matrix(matrix_name) {
    this->rows = VECTOR_NUMBER_OF_ROWS;
    this->cols = VECTOR_NUMBER_OF_COLS;
    this->matrix_name = matrix_name;

    //Instantiated this->stateVector part
    this->stateVector = new float[this->rows];
    for (int i = 0; i < this->rows; i++) {
        this->stateVector[i] = static_cast<float>(random_entryGenerator(9, 0));
    }
}

//Function is COMPLETE
//Constructor to create a random (rows x 1) state vector
State_Vector::State_Vector(int rows, string matrix_name) : 
    Matrix(rows, VECTOR_NUMBER_OF_COLS, matrix_name) {
    this->rows = rows;
    this->cols = VECTOR_NUMBER_OF_COLS;
    this->matrix_name = matrix_name;

    //Instantiated this->stateVector part
    this->stateVector = new float[this->rows];
    for (int i = 0; i < this->rows; i++) {
        this->stateVector[i] = static_cast<float>(random_entryGenerator(9, 0)); 
    }
}

//Function is COMPLETE
//Constructor to create a user entry-determined state vector
//Every State_Vector must have its OWN instantiated **matrix, i.e., this->matrix
State_Vector::State_Vector(int rows, string matrix_name, bool) :
    Matrix(rows, VECTOR_NUMBER_OF_COLS, matrix_name) {
    this->rows = rows;
    this->cols = VECTOR_NUMBER_OF_COLS;
    this->matrix_name = matrix_name;
    this->stateVector = new float[this->rows];
    data_entryProcedure(this->rows);
}

//Function is COMPLETE
//Destructor to deallocate memory of stateVector
State_Vector::~State_Vector() {
    if (this->matrix != nullptr) {
        for (int i = 0; i < this->rows; i++)
            delete [] this->matrix[i];

        delete [] this->matrix;
        this->matrix = nullptr;
        cout << "Deleted the " << this->matrix_name << " Matrix part." << endl;
    }

    delete [] this->stateVector;
    this->stateVector = nullptr;
    cout << "Deleted the " << this->matrix_name << " Vector." << endl;
}

//Function is COMPLETE
//Function to rescale the size of matrixToRescale to the size of matrixToCopy
//Function to only execute if the dimensions of matrixToCopy are larger than...
//... that of the *this object
void State_Vector::rescaleMatrix(const Matrix& matrixToCopy) {
    try {
        if (this->rows > matrixToCopy.getMatrix_Rows()) {
            throw RescaleMatrix_exception();

        } else if (this->rows == matrixToCopy.getMatrix_Rows()) {
            throw DoNothing_exception();

        } else if (this->rows < matrixToCopy.getMatrix_Rows()) {
            throw ContinueExecution_exception();
        }

    } catch (const RescaleMatrix_exception& ex) {
        cerr << "The " + this->matrix_name + " Vector is too large to be rescaled smaller "
            + "to the size of the " + matrixToCopy.getMatrix_Name() + " Matrix." << endl;
        return;
    
    } catch (const ContinueExecution_exception& ex) {
        //Function to continue execution when ContinueExecution_exception is caught
        
    } catch (const DoNothing_exception& ex) { return; }

    State_Vector this_tempStorage(this->rows, "Temporary");
    if (this->stateVector != nullptr) {
        for (int i = 0; i < this_tempStorage.rows; i++)
            this_tempStorage.stateVector[i] = this->stateVector[i];
        //this_tempStorage = *this;
        //this_tempStorage.operator=(*this);

        //Delete the memeory of the *this object
        delete [] this->stateVector;
        this->stateVector = nullptr;        
    }

    //Rescale the memory of the *this object
    this->stateVector = new float[matrixToCopy.getMatrix_Rows()];

    //Zero-out the *this object, and refill it with its old data
    zero_outMatrix(*this);
    for (int i = 0; i < this->getMatrix_Rows(); i++)
        this->stateVector[i] = this_tempStorage.stateVector[i];

    this->Matrix::rescaleMatrix(matrixToCopy);
}

//Function is COMPLETE
//Function to zero-out the *this matrix object
void State_Vector::zero_outMatrix() { zero_outMatrix(*this); }

//Getter-Function is COMPLETE
//Getter-Function to return the stateVector pointer
float* State_Vector::getState_Vector() const { return this->stateVector; }

//Function is COMPLETE
//Overloaded Inerstion Operator Function
//Operator Function to overload the '<<' operator to work with the State_Vector class
//This will allow to display the state vector by using the '<<' operator
ostream& operator<<(ostream& insertion_op, const State_Vector& rhs_vector) {
    insertion_op << "======= " << rhs_vector.matrix_name << " Vector ======="; 
    insertion_op << endl << endl;

    for (int i = 0; i < rhs_vector.rows; i++) {
        insertion_op << setw(6) << "[ ";
        insertion_op << rhs_vector.stateVector[i];
        insertion_op << " ]" << endl;
    }
    insertion_op << endl << endl;
    return insertion_op;
}

//Function is COMPLETE
//Overloaded Copy Assignment Operator 
//Operator Function to equate RHS vector to the LHS vector entry wise
//Object-slicing is expected in the parsed parameters of this function...
//... and on the return of the *this object and will call...
//... the appropriate Getter-Functions of the parsed-in object
Matrix& State_Vector::operator=(const Matrix& rhs_matrix) {
    if (this == &rhs_matrix) { return *this; }

    try {
        if (!rhs_matrix.IS_VECTOR())
            throw ObjectSliced_exception();

        //Error message to be displayed if the matrices are not of the same size dimension
        if (rhs_matrix.IS_VECTOR() && rhs_matrix.getMatrix_Rows() != this->rows)
            throw EitherDimensions_exception();
    
    } catch (const ObjectSliced_exception& ex) {
        return this->Matrix::operator=(rhs_matrix);
    
    } catch (const EitherDimensions_exception& ex) {
        rhs_matrix.getDimensionSize_ErrorMessage(*this, rhs_matrix);
        return *this;
    }

    for (int i = 0; i < rhs_matrix.getMatrix_Rows(); i++)
        this->stateVector[i] = rhs_matrix.getState_Vector()[i];

    return *this;
}    

//Function is COMPLETE
//Overloaded Copy Assignment Operator
//Operator Function to equate every entry of the *this object...
//... to the rhs_const_number value
//Object-slicing is expected on the return the *this object
Matrix& State_Vector::operator=(float&& rhs_const_number) {
    for (int i = 0; i < this->rows; i++)
        this->stateVector[i] = rhs_const_number;
    
    return *this;
}

//Function is COMPLETE
//Overloaded Addition Operator
//Operator Function to create a new State_Vector object,...
//... add the rhs_matrix to the *this object entry-wise...
//... and equate those entry-wise results to the new...
//... State_Vector object and return it
//Object-slicing is expected in the parsed parameters of this function...
//... and on the return of the newly created object and will call...
//... the appropriate Getter-Functions of the parsed-in object
Matrix State_Vector::operator+(const Matrix& rhs_matrix) {
    try {
        if (!rhs_matrix.IS_VECTOR())
            throw ObjectSliced_exception();

        //Error message to be displayed if the matrices are not of the same size dimension
        if (rhs_matrix.IS_VECTOR() && rhs_matrix.getMatrix_Rows() != this->rows) 
            throw EitherDimensions_exception();
    
    } catch (const ObjectSliced_exception& ex) {
        return this->Matrix::operator+(rhs_matrix);

    } catch (const EitherDimensions_exception& ex) {
        rhs_matrix.getDimensionSize_ErrorMessage(*this, rhs_matrix);
        return *this;
    }

    State_Vector resultant(this->rows, "Resultant");
    for (int i = 0; i < resultant.getMatrix_Rows(); i++)
        resultant.stateVector[i] = this->stateVector[i] + rhs_matrix.getState_Vector()[i];

    return resultant;
}     

//Function is COMPLETE
//Overloaded Addition Operator
//Operator Function to add the rhs_const_number to 
//... every entry of the *this object (entry-wise), and to then...
//... create a new State_Vector object, equate the individual entry reuslts...
//... of the above to the new object and return it
//Object-slicing is expected on the return the new object
Matrix State_Vector::operator+(float&& rhs_const_number) {
    State_Vector resultant(this->rows, "Resultant");
    for (int i = 0; i < resultant.getMatrix_Rows(); i++)
        resultant.stateVector[i] = this->stateVector[i] + rhs_const_number;

    return resultant;
}

//Function is COMPLETE
//Overloaded Subtraction Operator
//Operator Function to create a new State_Vector object,...
//... subtract the rhs_matrix from the *this object entry-wise...
//... and equate those entry-wise results to the new...
//... State_Vector object and return it
//Object-slicing is expected in the parsed parameters of this function...
//... and on the return of the newly created object and will call...
//... the appropriate Getter-Functions of the parsed-in object
Matrix State_Vector::operator-(const Matrix& rhs_matrix) {
    try {
        if (!rhs_matrix.IS_VECTOR())
            throw ObjectSliced_exception();

        //Error message to be displayed if the matrices are not of the same size dimension
        if (rhs_matrix.IS_VECTOR() && rhs_matrix.getMatrix_Rows() != this->rows) 
            throw EitherDimensions_exception();
    
    } catch (const ObjectSliced_exception& ex) {
        return this->Matrix::operator+(rhs_matrix);

    } catch (const EitherDimensions_exception& ex) {
        rhs_matrix.getDimensionSize_ErrorMessage(*this, rhs_matrix);
        return *this;
    }

    State_Vector resultant(this->rows, "Resultant");
    for (int i = 0; i < resultant.getMatrix_Rows(); i++)
        resultant.stateVector[i] = this->stateVector[i] - rhs_matrix.getState_Vector()[i];
    
    return resultant;
} 

//Function is COMPLETE
//Overloaded Subtraction Operator
//Operator Function to subtract the rhs_const_number from 
//... every entry of the *this object (entry-wise), and to then...
//... create a new State_Vector object, equate the individual entry reuslts...
//... of the above to the new object and return it
//Object-slicing is expected on the return the new object
Matrix State_Vector::operator-(float&& rhs_const_number) {
    State_Vector resultant(this->rows, "Resultant");
    for (int i = 0; i < resultant.getMatrix_Rows(); i++)
        resultant.stateVector[i] = this->stateVector[i] - rhs_const_number;

    return resultant;
}

//Function is COMPLETE
//Overloaded Multiplication Operator
//Operator Function to use and return either the Outer_Product function...
//... or the Inner Product function to multiply two state vector objects...
//... together based on user input.
//Object-Slicing is expected to take place 
//Operator Function to return either of the types depending on user-input
variant<Matrix, float> State_Vector::operator*(const Matrix& rhs_matrix) {
    if (!rhs_matrix.IS_VECTOR())
        return this->Matrix::operator*(rhs_matrix);

    //The rest of the error-checks are in the functions of the switch-case statement below

    char answer = '\0';
    cout << "Would you like an Inner Product or an Outer Product from multiplying the "
        << this->matrix_name + " and " + rhs_matrix.getMatrix_Name() + " vectors?" << endl
        << "Select the 'I' or 'O' character  for Inner Product or Outer Product (respectively): ";
    
    bool done = false;
    do {
        cin >> answer;
        switch (toupper(answer)) {
            case 'I': return Inner_Product(*this, rhs_matrix);
            case 'O': return Outer_Product(*this, rhs_matrix);
            default:
                cerr << "Please enter the appropriate input choices stated above ^" << endl;
        }
    } while (!done);
    return *this;
}     

//Function is COMPLETE
//Overloaded Multiplication Function
//Operator Function to create a new State_Vector object,...
//... the multiply each entry of the *this object by rhs_const_number...
//... and equate the results of each entry to the new object and return it...
Matrix State_Vector::operator*(float&& rhs_const_number) {
    State_Vector resultant(this->rows, "Resultant");
    for (int i = 0; i < resultant.getMatrix_Rows(); i++)
        resultant.stateVector[i] = this->stateVector[i] * rhs_const_number;

    return resultant;
}

//Function is COMPLETE
//Overloaded Addition-Assignment Operator
//Operator Function to add rhs_matrix to the *this operator...
//... entry wise and return the *this object
//Object-Slicing is expected to take place
Matrix& State_Vector::operator+=(const Matrix& rhs_matrix) {
    try {
        if (!rhs_matrix.IS_VECTOR())
            throw ObjectSliced_exception();

        //Error message to be displayed if the matrices are not of the same size dimension
        if (rhs_matrix.IS_VECTOR() && rhs_matrix.getMatrix_Rows() != this->rows)
            throw EitherDimensions_exception();  
    
    } catch (const ObjectSliced_exception& ex) {
        return this->Matrix::operator+=(rhs_matrix);

    } catch (const EitherDimensions_exception& ex) {
        rhs_matrix.getDimensionSize_ErrorMessage(*this, rhs_matrix);
        std::exit(EXIT_PROGRAM);
    }

    for (int i = 0; i < this->getMatrix_Rows(); i++)
        this->stateVector[i] += rhs_matrix.getState_Vector()[i];

    return *this;
}   

//Function is COMPLETE
//Overloaded Addition-Assignment Operator
//Operator Function to add the rhs_const_number to each entry...
//... of the *this object and return it
Matrix& State_Vector::operator+=(float&& rhs_const_number) {
    for (int i = 0; i < this->rows; i++)
        this->stateVector[i] += rhs_const_number;

    return *this;
}

//Function is COMPLETE
//Overloaded Subtraction-Assignemnet Operator
//Operator Function to subtract rhs_matrix from the *this operator...
//... entry wise and return the *this object
//Object-Slicing is expected to take place
Matrix& State_Vector::operator-=(const Matrix& rhs_matrix) {
    try {
        if (!rhs_matrix.IS_VECTOR())
            throw ObjectSliced_exception();

        //Error message to be displayed if the matrices are not of the same size dimension
        if (rhs_matrix.IS_VECTOR() && rhs_matrix.getMatrix_Rows() != this->rows)
            throw EitherDimensions_exception();
    
    } catch (const ObjectSliced_exception& ex) {
        return this->Matrix::operator-=(rhs_matrix);
    
    } catch (const EitherDimensions_exception& ex) {
        rhs_matrix.getDimensionSize_ErrorMessage(*this, rhs_matrix);
        std::exit(EXIT_PROGRAM);
    }

    for (int i = 0; i < this->getMatrix_Rows(); i++)
        this->stateVector[i] -= rhs_matrix.getState_Vector()[i];
    
    return *this;
}   

//Function is COMPLETE
//Overloaded Subtraction-Assignment Operator
//Operator Function to subtract the rhs_const_number from...
//... each entry of the *this object and return it
Matrix& State_Vector::operator-=(float&& rhs_const_number) {
    for (int i = 0; i < this->rows; i++)
        this->stateVector[i] -= rhs_const_number;
    
    return *this;
}

//Function NEEDS TESTING
//Overloaded Multiplication-Assignement Operator
//Operator Function to compute and use the Outer_Product() function...
//... between the *this and rhs_matrix objects and return the *this object
//Object-Slicing is expected to take place 
Matrix& State_Vector::operator*=(const Matrix& rhs_matrix) {
    if (!rhs_matrix.IS_VECTOR())
        return this->Matrix::operator*=(rhs_matrix);

    //========= SUCH A FUNCTION IS NONSENSICAL, AS MUTLIPLYING TWO VECTORS TOGETHER...
    //          ...MUST GIVE EITHER A SCALAR FLOAT NUMBER, OR A MATRIX. IN THIS CASE...
    //          ...THE LHS OBJECT IS THE *this OBJECT OF THE State_Vector CLASS...
    //          ...WHICH WE INTEND TO RETURN TO ==========//
    
    return *this = Outer_Product(*this, rhs_matrix);
} 

//Function is COMPLETE
//Overloaded Multiplication-Assignment Operator
//Operator Function to multiply the rhs_const_number to...
//... each entry of the *this object and return it
Matrix& State_Vector::operator*=(float&& rhs_const_number) {
    for (int i = 0; i < this->rows; i++)
        this->stateVector[i] *= rhs_const_number;
    
    return *this;
}

//Function is COMPLETE
//Overloaded Equality Check Operator
//Operator Function to check if each corresponding entry of the...
//... *this and rhs_matrix objects are equal to each other
//Function to return true if all corresponding entries are equal...
//... to each other, and to return false if otherwise
bool State_Vector::operator==(const Matrix& rhs_matrix) const {
    if (!rhs_matrix.IS_VECTOR())
        return this->Matrix::operator==(rhs_matrix);

    if (rhs_matrix.IS_VECTOR() && rhs_matrix.getMatrix_Rows() != this->rows) { return false; }

    for (int i = 0; i < this->getMatrix_Rows(); i++)
        if (this->stateVector[i] != rhs_matrix.getState_Vector()[i]) { return false; }
    
    return true;
}

//Function is COMPLETE
//Overloaded Not-Equal-To Equality Operator
//Operator Function to check if any corresponding entries of...
//... the *this and rhs_matrix objects are not equal
//Function to return true if corresponding entries are not equal...
//... to each other, and to return false if otherwise
bool State_Vector::operator!=(const Matrix& rhs_matrix) const {
    if (!rhs_matrix.IS_VECTOR())
        return this->Matrix::operator==(rhs_matrix);

    if (rhs_matrix.IS_VECTOR() && rhs_matrix.getMatrix_Rows() != this->rows) { return true; }

    for (int i = 0; i < this->getMatrix_Rows(); i++)
        if (this->stateVector[i] != rhs_matrix.getState_Vector()[i]) { return true; }

    return false;
}

//Function is COMPLETE
//Overloaded Greater-Than-Equal-To Operator
//Operator Function to check if the sum of the entries of the *this...
//... object is greater-than or equal to that of the sum of the...
//... entries of the rhs_matrix object
//Function to return true if the above is true, and to return false if otheriwse
bool State_Vector::operator>=(const Matrix& rhs_matrix) const {
    if (!rhs_matrix.IS_VECTOR())
        return this->Matrix::operator>=(rhs_matrix);

    int loop_counter = 0;
    float thisSum_Entries = 0.0f;
    float rhsSum_Entries = 0.0f;

    if (this->getMatrix_Rows() >= rhs_matrix.getMatrix_Rows()) {
        for (; loop_counter < rhs_matrix.getMatrix_Rows(); loop_counter++) {
            thisSum_Entries += this->stateVector[loop_counter];
            rhsSum_Entries += rhs_matrix.getState_Vector()[loop_counter];
        }

        for (; loop_counter < this->getMatrix_Rows(); loop_counter++) {
            if (this->getMatrix_Rows() == rhs_matrix.getMatrix_Rows()) { break; }
            thisSum_Entries += this->stateVector[loop_counter];
        }

    } else if (this->getMatrix_Rows() <= rhs_matrix.getMatrix_Rows()) {
        for (; loop_counter < this->getMatrix_Rows(); loop_counter++) {
            thisSum_Entries += this->stateVector[loop_counter];
            rhsSum_Entries += rhs_matrix.getState_Vector()[loop_counter];
        }

        for (; loop_counter < rhs_matrix.getMatrix_Rows(); loop_counter++) {
            if (this->getMatrix_Rows() == rhs_matrix.getMatrix_Rows()) { break; }
            rhsSum_Entries += rhs_matrix.getState_Vector()[loop_counter];
        }
    }
    
    if (thisSum_Entries >= rhsSum_Entries) { return true; }
    return false;
}

//Function is COMPLETE
//Overloaded Less-Than-Equal-To Operator
//Operator Function to check if the sum of the entries of the *this...
//... object is less-than or equal to that of the sum of the...
//... entries of the rhs_matrix object
//Function to return true if the above is true, and to return false if otheriwse
bool State_Vector::operator<=(const Matrix& rhs_matrix) const {
    if (!rhs_matrix.IS_VECTOR())
        return this->Matrix::operator<=(rhs_matrix);

    int loop_counter = 0;
    float thisSum_Entries = 0.0f;
    float rhsSum_Entries = 0.0f;

    if (this->getMatrix_Rows() >= rhs_matrix.getMatrix_Rows()) {
        for (; loop_counter < rhs_matrix.getMatrix_Rows(); loop_counter++) {
            thisSum_Entries += this->stateVector[loop_counter];
            rhsSum_Entries += rhs_matrix.getState_Vector()[loop_counter];
        }

        for (; loop_counter < this->getMatrix_Rows(); loop_counter++) {
            if (this->getMatrix_Rows() == rhs_matrix.getMatrix_Rows()) { break; }
            thisSum_Entries += this->stateVector[loop_counter];
        }

    } else if (this->getMatrix_Rows() <= rhs_matrix.getMatrix_Rows()) {
        for (; loop_counter < this->getMatrix_Rows(); loop_counter++) {
            thisSum_Entries += this->stateVector[loop_counter];
            rhsSum_Entries += rhs_matrix.getState_Vector()[loop_counter];
        }

        for (; loop_counter < rhs_matrix.getMatrix_Rows(); loop_counter++) {
            if (this->getMatrix_Rows() == rhs_matrix.getMatrix_Rows()) { break; }
            rhsSum_Entries += rhs_matrix.getState_Vector()[loop_counter];
        }
    }
    
    if (thisSum_Entries <= rhsSum_Entries) { return true; }
    return false;    
}

//Function is COMPLETE
//Overloaded Less-Than Operator
//Operator Function to check if the sum of the entries of the *this...
//... object is less-than that of the sum of the entries of the rhs_matrix object
//Function to return true if the above is true, and to return false if otheriwse
bool State_Vector::operator<(const Matrix& rhs_matrix) const {
    if (!rhs_matrix.IS_VECTOR())
        return this->Matrix::operator<=(rhs_matrix);

    int loop_counter = 0;
    float thisSum_Entries = 0.0f;
    float rhsSum_Entries = 0.0f;

    if (this->getMatrix_Rows() >= rhs_matrix.getMatrix_Rows()) {
        for (; loop_counter < rhs_matrix.getMatrix_Rows(); loop_counter++) {
            thisSum_Entries += this->stateVector[loop_counter];
            rhsSum_Entries += rhs_matrix.getState_Vector()[loop_counter];
        }

        for (; loop_counter < this->getMatrix_Rows(); loop_counter++) {
            if (this->getMatrix_Rows() == rhs_matrix.getMatrix_Rows()) { break; }
            thisSum_Entries += this->stateVector[loop_counter];
        }

    } else if (this->getMatrix_Rows() <= rhs_matrix.getMatrix_Rows()) {
        for (; loop_counter < this->getMatrix_Rows(); loop_counter++) {
            thisSum_Entries += this->stateVector[loop_counter];
            rhsSum_Entries += rhs_matrix.getState_Vector()[loop_counter];
        }

        for (; loop_counter < rhs_matrix.getMatrix_Rows(); loop_counter++) {
            if (this->getMatrix_Rows() == rhs_matrix.getMatrix_Rows()) { break; }
            rhsSum_Entries += rhs_matrix.getState_Vector()[loop_counter];
        }
    }
    
    if (thisSum_Entries < rhsSum_Entries) { return true; }
    return false;
}

//Function is COMPLETE
//Overloaded Greater-Than Operator
//Operator Function to check if the sum of the entries of the *this...
//... object is greater-than that of the sum of the entries of the rhs_matrix object
//Function to return true if the above is true, and to return false if otheriwse
bool State_Vector::operator>(const Matrix& rhs_matrix) const {
    if (!rhs_matrix.IS_VECTOR())
        return this->Matrix::operator<=(rhs_matrix);

    int loop_counter = 0;
    float thisSum_Entries = 0.0f;
    float rhsSum_Entries = 0.0f;

    if (this->getMatrix_Rows() >= rhs_matrix.getMatrix_Rows()) {
        for (; loop_counter < rhs_matrix.getMatrix_Rows(); loop_counter++) {
            thisSum_Entries += this->stateVector[loop_counter];
            rhsSum_Entries += rhs_matrix.getState_Vector()[loop_counter];
        }

        for (; loop_counter < this->getMatrix_Rows(); loop_counter++) {
            if (this->getMatrix_Rows() == rhs_matrix.getMatrix_Rows()) { break; }
            thisSum_Entries += this->stateVector[loop_counter];
        }

    } else if (this->getMatrix_Rows() <= rhs_matrix.getMatrix_Rows()) {
        for (; loop_counter < this->getMatrix_Rows(); loop_counter++) {
            thisSum_Entries += this->stateVector[loop_counter];
            rhsSum_Entries += rhs_matrix.getState_Vector()[loop_counter];
        }

        for (; loop_counter < rhs_matrix.getMatrix_Rows(); loop_counter++) {
            if (this->getMatrix_Rows() == rhs_matrix.getMatrix_Rows()) { break; }
            rhsSum_Entries += rhs_matrix.getState_Vector()[loop_counter];
        }
    }
    
    if (thisSum_Entries > rhsSum_Entries) { return true; }
    return false;
}

//Function is COMPLETE
//Function to allow user to input matrix entries
void State_Vector::data_entryProcedure(const int& rows) {
    cout << "Please input the matrix data entries below for the "
        << this->matrix_name << " matrix:" << endl;
    float matrix_entry;

    for (int i = 0; i < rows; i++) {
        cout << this->matrix_name << " matrix entry [" << i << "]: ";
        cin >> matrix_entry; 
        this->stateVector[i] = matrix_entry; 
    }
    cout << endl << endl;
}

//Function is COMPLETE
//Function to compute and return the Euclidean Inner Product of the lhs and rhs vectors 
float State_Vector::Inner_Product(const Matrix& lhs_vector, const Matrix& rhs_vector) {
    try {
        if (lhs_vector.IS_VECTOR() && rhs_vector.IS_VECTOR() &&
            lhs_vector.getMatrix_Rows() != rhs_vector.getMatrix_Rows()) 
            throw EitherDimensions_exception();
        
    } catch (const EitherDimensions_exception& ex) {
        rhs_vector.getDimensionSize_ErrorMessage(lhs_vector, rhs_vector);
        std::exit(EXIT_PROGRAM);
    }

    float inner_product = 0;
    for (int i = 0; i < lhs_vector.getMatrix_Rows(); i++)
        inner_product += lhs_vector.getState_Vector()[i] * rhs_vector.getState_Vector()[i]; 

    return inner_product;
}

//Function is COMPLETE
//Function to compute and return the outer product (matrix) from the lhs and rhs vectors
Matrix State_Vector::Outer_Product(const Matrix& lhs_vector, const Matrix& rhs_vector) {
    try {
        if (lhs_vector.IS_VECTOR() && rhs_vector.IS_VECTOR()
            && lhs_vector.getMatrix_Cols() != VECTOR_NUMBER_OF_COLS
            && rhs_vector.getMatrix_Cols() != VECTOR_NUMBER_OF_COLS) 
            throw EitherDimensions_exception();

    } catch (const EitherDimensions_exception& ex) {
        rhs_vector.getMultiplication_ErrorMessage(lhs_vector, rhs_vector);
        std::exit(EXIT_PROGRAM);
    }

    Matrix outer_product(lhs_vector.getMatrix_Rows(), rhs_vector.getMatrix_Rows(), 
        "Outer Product from " + lhs_vector.getMatrix_Name() + " and " + rhs_vector.getMatrix_Name());
    
    //Create a temporary State_Vector and instantiate its **matrix part
    //Later equate it to outer_product.matrix
    State_Vector temp_outerProd(lhs_vector.getMatrix_Rows(), "Temporary");
    temp_outerProd.rows = lhs_vector.getMatrix_Rows();
    temp_outerProd.cols = rhs_vector.getMatrix_Rows();  //Correct, DO NOT change!

    temp_outerProd.matrix = new float*[temp_outerProd.getMatrix_Rows()];
    for (int i = 0; i < temp_outerProd.getMatrix_Rows(); i++)
        temp_outerProd.getMatrix_ptrMatrix()[i] = new float[temp_outerProd.getMatrix_Cols()];
    
    for (int i = 0; i < temp_outerProd.getMatrix_Rows(); i++) {
        for (int j = 0; j < temp_outerProd.getMatrix_Cols(); j++)
            temp_outerProd.matrix[i][j] = lhs_vector.getState_Vector()[i] * rhs_vector.getState_Vector()[j];
    }
    outer_product = temp_outerProd;
    return outer_product;
}

//Function to compute and return a transformed state vector from the matrix transformation/operation...
//... Ax = x', where A == lhs_matrix, x == rhs_vector, and x' == tranformed_vector is to be returned
State_Vector State_Vector::Vector_Transform(const Matrix& lhs_matrix, State_Vector& rhs_vector) {
    State_Vector transformedVector(lhs_matrix.getMatrix_Cols(), "Transformed");
    zero_outMatrix(transformedVector);
    //Initialise and zero-out the transformed vector

    //Set rhs_vector.stateVector to scale
    if (rhs_vector.stateVector == nullptr) {
        rhs_vector.stateVector = new float[lhs_matrix.getMatrix_Cols()];
        for (int i = 0; i < lhs_matrix.getMatrix_Cols(); i++)
            rhs_vector.stateVector[i] = static_cast<float>(random_entryGenerator(9, 0));
    }

    try {
        if (rhs_vector.stateVector != nullptr && rhs_vector.rows != lhs_matrix.getMatrix_Cols())
            throw EitherDimensions_exception();
        
        //Re-Initialise the transformed vector from the operation transformation x' = Ax
        for (int i = 0; i < lhs_matrix.getMatrix_Rows(); i++) {
            for (int j = 0; j < lhs_matrix.getMatrix_Cols(); j++)
                transformedVector.stateVector[i] += lhs_matrix.getMatrix_ptrMatrix()[i][j] * rhs_vector.stateVector[j];
        }   
    
    } catch (const EitherDimensions_exception& ex) {
        lhs_matrix.getMultiplication_ErrorMessage(lhs_matrix, rhs_vector);
        std::exit(EXIT_PROGRAM);
    }
    return transformedVector;
}

//Function is COMPLETE
//Function to zero-out the entire vector
void State_Vector::zero_outMatrix(Matrix& vector) {
    if (vector.getState_Vector() == nullptr) { return; }
    for (int i = 0; i < vector.getMatrix_Rows(); i++)
        vector.getState_Vector()[i] = 0; 

    this->Matrix::zero_outMatrix();
}