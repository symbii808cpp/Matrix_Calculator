#include "Matrix.h"

//Function is COMPLETE
//Constructor to create a randomised 3x3 matrix
Matrix::Matrix(string matrix_name) : rows {3}, cols {3} { 
    this->matrix_name = matrix_name;
    setprecision(2);

    matrix = new float*[this->rows];
    for (int i = 0; i < this->rows; i++) {

        matrix[i] = new float[this->cols];
        for (int j = 0; j < this->cols; j++)
            matrix[i][j] = static_cast<float>(random_entryGenerator(9, 0));
    }    
}

//Function is COMPLETE
//Constructor to create a randomised (rows x cols) sized matrix 
Matrix::Matrix(int rows, int cols, string matrix_name) {
    this->matrix_name = matrix_name;
    this->rows = rows;
    this->cols = cols;
    setprecision(2);

    matrix = new float*[this->rows];
    for (int i = 0; i < this->rows; i++) {

        matrix[i] = new float[this->cols];
        for (int j = 0; j < this->cols; j++)
            matrix[i][j] = static_cast<float>(random_entryGenerator(9, 0));
    }
}

//Function is COMPLETE
//Constructor to create a user-determined (rows x cols) sized matrix
Matrix::Matrix(int rows, int cols, string matrix_name, bool determined_entries) {
    this->matrix_name = matrix_name;
    this->rows = rows;
    this->cols = cols;
    setprecision(2);

    matrix = new float*[this->rows];
    for (int i = 0; i < this->rows; i++)
        matrix[i] = new float[this->cols];
    
    data_entryProcedure(this->rows, this->cols);
}

//Destructor is COMPLETE
//Destructor to deallocate dynamic/temporary memory addresses
Matrix::~Matrix() {
    for (int i = 0; i < this->rows; i++)
        delete[] this->matrix[i];

    delete[] this->matrix;
    this->matrix = nullptr;
    cout << "Deleted the " << this->matrix_name << " Matrix." << endl;
}

//Function is COMPLETE
//Function to display matrix
void Matrix::display_matrix() const {
    cout << setprecision(3);
    cout << "======= " << this->matrix_name << " Matrix =======" << endl << endl;
    for (int i = 0; i < this->rows; i++) {
        cout << setw(6) << "[ ";

        for (int j = 0; j < this->cols; j++) {
            if (j == this->cols - 1) { 
                cout << this->matrix[i][j];
                break;
            }
            cout << this->matrix[i][j] << setw(7);
        }
        cout << " ]" << endl;
    }
    cout << endl << endl;
}

void Matrix::pauseProgram() {
    cin.ignore(100, '\n');
    cout << "Press the Enter Key to continue: " << endl;
    cin.get();
}

//Getter-Functions are COMPLETE
//Getter-Functions to display dimension-size/multiplication error message
void Matrix::getDimensionSize_ErrorMessage(const Matrix& lhs_matrix, const Matrix& rhs_matrix) const {
    dimensionSize_ErrorMessage(lhs_matrix, rhs_matrix);
}

void Matrix::getMultiplication_ErrorMessage(const Matrix& lhs_matrix, const Matrix& rhs_matrix) const {
    multiplication_ErrorMessage(lhs_matrix, rhs_matrix);
}

//Function is COMPLETE
//Function to zero-out the *this matrix object
void Matrix::zero_outMatrix() { zero_outMatrix(*this); }

//Function is COMPLETE
//Function to rescale the size of matrixToRescale to the size of matrixToCopy
//Function to only execute if the dimensions of matrixToCopy are larger than...
//... that of the *this object 
void Matrix::rescaleMatrix(const Matrix& matrixToCopy) {
    try {
        if (this->rows > matrixToCopy.rows || this->cols > matrixToCopy.cols) {
            throw RescaleMatrix_exception();

        } else if (this->rows == matrixToCopy.rows && this->cols == matrixToCopy.cols) {
            throw DoNothing_exception();

        } else if (this->rows < matrixToCopy.rows && this->cols < matrixToCopy.cols) {
            throw ContinueExecution_exception();
        }

    } catch (const RescaleMatrix_exception& ex) {
        cerr << "The " + this->matrix_name + " Matrix is either oddly dimensioned or too large "
            + "to be scaled/rescaled smaller to the size of the " + matrixToCopy.matrix_name
            + " Matrix." << endl;
        return;

    } catch (const ContinueExecution_exception& ex) {
        //Function to continue execution when ContinueExecution_exception is caught

    } catch (const DoNothing_exception& ex) { return; }
    

    Matrix this_tempStorage(this->rows, this->cols, "Temporary");
    if (this->matrix != nullptr) {
        this_tempStorage = *this;

        //Delete the memeory of the *this object
        for (int i = 0; i < this->rows; i++)
            delete [] this->matrix[i];

        delete [] this->matrix;
        this->matrix = nullptr;        
    }

    //Rescale the memeory of the *this object
    this->matrix = new float*[matrixToCopy.rows];
    for (int i = 0; i < matrixToCopy.rows; i++)
        this->matrix[i] = new float[matrixToCopy.cols];

    //Zero-out the *this object, and refill it with its old data
    zero_outMatrix(*this);
    for (int i = 0; i < matrixToCopy.rows; i++) {
        for (int j = 0; j < matrixToCopy.cols; j++)
            this->matrix[i][j] = matrixToCopy.matrix[i][j];
    }
}

//Function is COMPLETE
//Function to calculate and return the trace of a square matrix
float Matrix::Trace() const {
    float trace_value = 0;
    try {
        if (!isSquareMatrix())
            throw SquareMatrix_exception();
    
        for (int i = 0; i < this->rows; i++)
            trace_value += this->matrix[i][i];

    } catch (const SquareMatrix_exception& ex) {
        cerr << "Cannot compute trace of " << this->matrix_name
            << " as it is NOT a Square Matrix!" << endl;
        return 0.0f;
    }
    return trace_value;
}

//Function is COMPLETE
//Function to compute and return the Transpose Matrix of the 'this' matrix
Matrix Matrix::Transpose() const {
    //Transpose Matrix above has rows = this->cols, and cols = this->rows
    Matrix transpose(this->cols, this->rows, "Transpose of " + this->matrix_name);

    // i => transpose.rows => this->cols
    // j => transpose.cols => this->rows
    for (int i = 0; i < transpose.rows; i++) {
        for (int j = 0; j < transpose.cols; j++)
            transpose.matrix[i][j] = this->matrix[j][i];
    }
    return transpose;
}

//Function is COMPLETE
//Function to perform Matrix Row Operations to ...
//... Row-Reduce a matrix via Gaussian Elimination
Matrix Matrix::Gaussian_Elimination() {
    //1. Create a new Matrix Object to return and equate it to ' *this '
    Matrix reduced(this->getMatrix_Rows(), this->getMatrix_Cols(), "Reduced " + this->getMatrix_Name());
    reduced = *this;

    bool isReduced = false;
    bool isAugmented = false;
    char coe_augInput = '\0';
    int ROW_ABSOLUTE_TRACKER = 0;
    int COL_ABSOLUTE_TRACKER = 0;
    make_leadingVariablesPositive(reduced);
    move_zeroRows(reduced);

    do {
        cout << "Is the " + this->getMatrix_Name() + " Matrix a Coefficient[c] or Augmented Matrix[a]: ";
        cin >> coe_augInput;
        if (tolower(coe_augInput) != 'a' || tolower(coe_augInput) != 'c') {
            cout << "Incorrect input!!! Enter either of the letters above"
                << " indicated within the square brackets []" << endl;
            continue;
        
        } else if (tolower(coe_augInput) == 'a') { 
            isAugmented == true;
            break;

        } else if (tolower(coe_augInput) == 'c') { 
            isAugmented == false;
            break;
        }
    } while (true);

    //2. Use a outer/big for-loop to track each column of the matrix
    //   The for-loop will progress to a smaller sub-section of the matrix,
    //   when there is a leading 1 in that column and the rest of the entries below it are zero!!!
    //   *****All the code below will be contained in this outer/big for-loop*****
    for (int j = COL_ABSOLUTE_TRACKER; j < reduced.getMatrix_Cols(); j++) {
        if (isReduced) { break; }

        //3. Look for rows that are divisible by their leading variable
        //   We should have leading 1's or 0's in column number COL_ABSOLUTE_TRACKER
        for (int i = ROW_ABSOLUTE_TRACKER; i < reduced.getMatrix_Rows(); i++) {
            if (reduced.getMatrix_ptrMatrix()[i][j] != 0) {
                multiplyRow(reduced, i, j);
            }
        }
        make_leadingVariablesPositive(reduced);

        //4. In the column number COL_ABSOLUTE_TRACKER, if the element is equal to 0,
        //   sort those rows to the bottom of the matrix using the swapRows() function
        //   Use a vector record log to assist with keeping track of which rows to swap
        vector<bool> zero_leadingElement;
        for (int i = 0; i < reduced.getMatrix_Rows(); i++) {
            if (reduced.getMatrix_ptrMatrix()[i][j] == 0)
                zero_leadingElement.push_back(true);
            else zero_leadingElement.push_back(false);
        }

        //Set the bottom_rowTracker to the first row with a leading 0
        size_t bottom_rowTracker = static_cast<size_t>(reduced.getMatrix_Rows());
        for (int k = 0; k < reduced.getMatrix_Rows(); k++) {
            if (reduced.getMatrix_ptrMatrix()[bottom_rowTracker - 1][j] == 0)
                bottom_rowTracker -= 1;
            else break;
        }

        //Swap rows with the leading 1's to the top and leading 0's to the bottom
        for (size_t k = ROW_ABSOLUTE_TRACKER; k < zero_leadingElement.size(); k++) {
            if (k == bottom_rowTracker - 1) { break; }
            if (zero_leadingElement[k] && !zero_leadingElement[bottom_rowTracker - 1]) {
                bottom_rowTracker--;
                zero_leadingElement[k] = false;
                zero_leadingElement[bottom_rowTracker] = true;
                swapRows(reduced, k, bottom_rowTracker);

            } else if (zero_leadingElement[k] && zero_leadingElement[bottom_rowTracker - 1]) {
                bottom_rowTracker--;
                if (k != 0) { k--; }
            } 
        }
        make_leadingVariablesPositive(reduced);

        //4. Now to use the sub_from_and_multiplyRow() function on the rows with leading 1's
        //   Keep the top row constant, and subtract a constant multiple of the top row from the rest
        for (size_t i = ROW_ABSOLUTE_TRACKER + 1; i < bottom_rowTracker; i++)
            sub_from_and_multiplyRow(reduced, i, ROW_ABSOLUTE_TRACKER);

        make_leadingVariablesPositive(reduced);

        //5. Update the ROW_ABSOLUTE_TRACKER and COL_ABSOLUTE_TRACKER value (if needed,
        //   if not then COL_ABSOLUTE_TRACKER is updated in the outer for-loop),
        //   as well as the zero_leadingElement vector
        if (isCanMatrixProgress(reduced, ROW_ABSOLUTE_TRACKER)) {
            while (zero_leadingElement.size() != 0)
                zero_leadingElement.pop_back();

            //Now search the next column for a leading value (not zero)
            //If a leading value is found (not zero), update the ABSOLUTE_TRACKERS...
            //... and use the move_zeroRows() and make_leadingVariablesPositive() functions
            //If a leading value is NOT found (all zeros in the column), restart the for-loop...
            //... and update the COL_ABSOLUTE_TRACKER value only, and check if the matrix is deemed reduced
            for (int i = ROW_ABSOLUTE_TRACKER + 1; i < reduced.getMatrix_Rows(); i++) {
                if (reduced.getMatrix_ptrMatrix()[i][COL_ABSOLUTE_TRACKER + 1] != 0) {
                    COL_ABSOLUTE_TRACKER++;
                    ROW_ABSOLUTE_TRACKER++;
                    move_zeroRows(reduced);
                    make_leadingVariablesPositive(reduced);

                    if (ROW_ABSOLUTE_TRACKER == reduced.getMatrix_Rows() - 1 || 
                        COL_ABSOLUTE_TRACKER == reduced.getMatrix_Cols() - 1) {
                        multiplyRow(reduced, ROW_ABSOLUTE_TRACKER, COL_ABSOLUTE_TRACKER);
                        isReduced = true;

                        if (isAugmented && isInfinitlyManySolutions(reduced))
                            InfinitlyManySolutions_Message(*this);
                        else if (isAugmented && isNoSolutionSet(reduced))
                            NoSolutionSet_Message(*this);
                        
                        break;
                    }
                    break;
                
                } else if (reduced.getMatrix_ptrMatrix()[i][COL_ABSOLUTE_TRACKER + 1] == 0) {
                    if (i == reduced.getMatrix_Rows() - 1) {
                        i = ROW_ABSOLUTE_TRACKER + 1;   //Restart the for-loop
                        COL_ABSOLUTE_TRACKER++;         //Move onto the next column

                    } else if (COL_ABSOLUTE_TRACKER == reduced.getMatrix_Cols() - 1) {
                        isReduced = true;

                        if (isAugmented && isInfinitlyManySolutions(reduced))
                            InfinitlyManySolutions_Message(*this);
                        else if (isAugmented && isNoSolutionSet(reduced))
                            NoSolutionSet_Message(*this);

                        break;
                    }
                }
            }
        }
    }
    return reduced;
}

//Matrix Matrix::Gauss_Jordan_Elimination() {}

//Getter-Function is COMPLETE
//Getter-Function to return a nullptr
//The purpose of this function is to achieve polymorphism and object-slicing in...
//... the function State_Vector::operator=(const Matrix& rhs_matrix) function
//NEEDS TESTING (Test this by testing State_Vector::operator=() )
float* Matrix::getState_Vector() const { return nullptr; }

//Getter-Functions are COMPLETE
//Getter-Functions to return the number of rows, columns, name, and matrix pointer of the matrix
int Matrix::getMatrix_Rows() const { return this->rows; }
int Matrix::getMatrix_Cols() const { return this->cols; }
string Matrix::getMatrix_Name() const { return this->matrix_name; }
float** Matrix::getMatrix_ptrMatrix() const { return this->matrix; }

//Function is COMPLETE
//Overloaded Inerstion Operator Function
//Function to overload the '<<' operator to work with the Matrix class
//This will allow to display the matrix by using the '<<' operator
ostream& operator<<(ostream& insertion_op, const Matrix& rhs_matrix) {
    insertion_op << "======= " << rhs_matrix.matrix_name << " Matrix ======="; 
    insertion_op << endl << endl;
    insertion_op << setprecision(3);

    for (int i = 0; i < rhs_matrix.rows; i++) {
        insertion_op << setw(6) << "[ ";

        for (int j = 0; j < rhs_matrix.cols; j++) {
            if (j == rhs_matrix.cols - 1) { 
                insertion_op << rhs_matrix.matrix[i][j];
                break;
            }
            insertion_op << rhs_matrix.matrix[i][j] << setw(7);
        }
        insertion_op << " ]" << endl;
    }

    insertion_op << endl << endl;
    return insertion_op;
}

//Function is COMPLETE
//Overloaded Friend Multiplication Operator
//Operator to multiply each entry of the LHS matrix by the RHS number 
Matrix operator*(float&& rhs_const_number, const Matrix& rhs_matrix) {
    Matrix resultant_matrix(rhs_matrix.rows, rhs_matrix.cols, "Resultant");

    for (int i = 0; i < resultant_matrix.rows; i++) {
        for (int j = 0; j < resultant_matrix.cols; j++)
            resultant_matrix.matrix[i][j] = rhs_matrix.matrix[i][j] * rhs_const_number;
    }
    return resultant_matrix;
}

//Function is COMPLETE
//Overloaded Copy Assignment Operator 
//Operator to equate RHS matrix to the LHS matrix entry wise
//An error message will be displayed if the matrices are not of...
//... the same size dimension
Matrix& Matrix::operator=(const Matrix& rhs_matrix) {
    try {
        if (this->rows != rhs_matrix.rows || this->cols != rhs_matrix.cols) {
            throw EitherDimensions_exception();
        }   //Dimension-Size Matrix Condition

        //If the RHS and LHS matrix share the same memory address
        if (this == &rhs_matrix) { return *this; }
    
    } catch (const EitherDimensions_exception& ex) {
        dimensionSize_ErrorMessage(*this, rhs_matrix);
        return *this;
    }

    //Perform a deep copy
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++)
            this->matrix[i][j] = rhs_matrix.matrix[i][j];
    }
    return *this;
}

//Function is COMPLETE
//Overloaded 'Copy Assignment' Operator
//Operator to make all entries of the RHS matrix equal to the LHS number
Matrix& Matrix::operator=(float&& rhs_const_number) {
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++)
            this->matrix[i][j] = rhs_const_number;
    }
    return *this;
}

//Function is COMPLETE
//Overloaded Addition Operator
//Operator to add two matrices (RHS and LHS) by addition of corresponding...
//... entries and to equate those results to, and return a new Matrix object
Matrix Matrix::operator+(const Matrix& rhs_matrix) {
    try {
        if (this->rows != rhs_matrix.rows || this->cols != rhs_matrix.cols) {
            throw EitherDimensions_exception();
        }   //Addition-Subtraction Matrix Condition
    
    } catch (const EitherDimensions_exception& ex) {
        dimensionSize_ErrorMessage(*this, rhs_matrix);
        return *this;
    }

    Matrix resultant_matrix(this->rows, this->cols, "Resultant");
    for (int i = 0; i < resultant_matrix.rows; i++) {
        for (int j = 0; j < resultant_matrix.cols; j++)
            resultant_matrix.matrix[i][j] = this->matrix[i][j] + rhs_matrix.matrix[i][j];
    }
    return resultant_matrix;
}

//Function is COMPLETE
//Overloaded Addition Operator
//Operator to add the LHS number to each entry of the RHS matrix...
//... and equate that to and return a new matrix object  
Matrix Matrix::operator+(float&& rhs_const_number) {
    Matrix resultant_matrix(this->rows, this->cols, "Resultant");

    for (int i = 0; i < resultant_matrix.rows; i++) {
        for (int j = 0; j < resultant_matrix.cols; j++)
            resultant_matrix.matrix[i][j] = this->matrix[i][j] + rhs_const_number;
    }
    return resultant_matrix;
}

//Function is COMPLETE
//Overloaded Subtraction Operator
//Operator to subtract the entries of the LHS matrix from the corresponding...
//... entries of the RHS matrix, and equate those results to a new matrix...
//... and return it
Matrix Matrix::operator-(const Matrix& rhs_matrix) {
    try {
        if (this->rows != rhs_matrix.rows || this->cols != rhs_matrix.cols) {
            throw EitherDimensions_exception();
        }   //Addition-Subtraction Matrix Condition
    
    } catch (const EitherDimensions_exception& ex) {
        dimensionSize_ErrorMessage(*this, rhs_matrix);
        return *this;
    }

    Matrix resultant_matrix(this->rows, this->cols, "Resultant");
    for (int i = 0; i < resultant_matrix.rows; i++) {
        for (int j = 0; j < resultant_matrix.cols; j++)
            resultant_matrix.matrix[i][j] = this->matrix[i][j] - rhs_matrix.matrix[i][j];
    }
    return resultant_matrix;
}

//Function is COMPLETE
//Overloaded Subtraction Operator
//Operator to subtract the LHS number from all the entries of the RHS matrix...
//... and to equate the results to a new matrix and return it  
Matrix Matrix::operator-(float&& rhs_const_number) {
    Matrix resultant_matrix(this->rows, this->cols, "Resultant");

    for (int i = 0; i < resultant_matrix.rows; i++) {
        for (int j = 0; j < resultant_matrix.cols; j++)
            resultant_matrix.matrix[i][j] = this->matrix[i][j] - rhs_const_number;
    }
    return resultant_matrix;
}

//Function is COMPLETE
//Overloaded Multiplication Operator
//Operator to multiply the RHS and LHS matrices together...
//... and to equate the result to a resultant matrix and return it
Matrix Matrix::operator*(const Matrix& rhs_matrix) {
    try {
        if (this->cols != rhs_matrix.rows) 
            throw MutiplicationDimension_exception();
        //Matrix Multiplication Condition
    
    } catch (const MutiplicationDimension_exception& ex) {
        multiplication_ErrorMessage(*this, rhs_matrix);
        return *this;
    }
    
    Matrix resultant_matrix(this->rows, rhs_matrix.cols, "Resultant");

    //i => LHS matrix row number, j => RHS matrix column number
    //k => LHS matrix column *AND* RHS matrix row number
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < rhs_matrix.cols; j++) {

            float entry_summation = 0;  //Resultant Matrix new entry result
            for (int k = 0; k < rhs_matrix.rows; k++)
                entry_summation += this->matrix[i][k] * rhs_matrix.matrix[k][j];

            resultant_matrix.matrix[i][j] = entry_summation;
        }
    }
    return resultant_matrix;
}

//Function is COMPLETE
//Overloaded Multiplication Operator
//Operator to multiply each entry of the LHS matrix by the RHS number 
Matrix Matrix::operator*(float&& rhs_const_number) {
    Matrix resultant_matrix(this->rows, this->cols, "Resultant");

    for (int i = 0; i < resultant_matrix.rows; i++) {
        for (int j = 0; j < resultant_matrix.cols; j++)
            resultant_matrix.matrix[i][j] = this->matrix[i][j] * rhs_const_number;
    }
    return resultant_matrix;
}

//Function is COMPLETE
//Overloaded Addition-Assignment Operator
//Operator to add the RHS and LHS-this matrices together and to equate...
//... and return the resulting matrix to the 'this' matrix object 
Matrix& Matrix::operator+=(const Matrix& rhs_matrix) {
    try {
        if (this->rows != rhs_matrix.rows || this->cols != rhs_matrix.cols) {
            throw EitherDimensions_exception();
        }   //Matrix Addition-Subtraction Condition
    
    } catch (const EitherDimensions_exception& ex) {
        dimensionSize_ErrorMessage(*this, rhs_matrix);
        return *this;
    }

    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++)
            this->matrix[i][j] += rhs_matrix.matrix[i][j];
    }
    return *this;
}

//Function is COMPLETE
//Overloaded Addition-Assignment Operator
//Operator to add the RHS number to each entry of the LHS matrix...
//... and to equate and return the resulting matrix to 'this'
Matrix& Matrix::operator+=(float&& rhs_const_number) {
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++)
            this->matrix[i][j] += rhs_const_number;
    }
    return *this;
}

//Function is COMPLETE
//Overloaded Subtraction-Assignment Operator
//Operator to subtract the RHS matrix from the LHS-this matrix and to...
//... equate and return the resulting matrix to the 'this' matrix object  
Matrix& Matrix::operator-=(const Matrix& rhs_matrix) {
    try {
        if (this->rows != rhs_matrix.rows || this->cols != rhs_matrix.cols) {
            throw EitherDimensions_exception();
        }   //Matrix Addition-Subtraction Condition
    
    } catch (const EitherDimensions_exception& ex) {
        dimensionSize_ErrorMessage(*this, rhs_matrix);
        return *this;
    }

    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++)
            this->matrix[i][j] -= rhs_matrix.matrix[i][j];
    }
    return *this;
}

//Function is COMPLETE
//Overloaded Subtraction-Assignement Operator
//Operator to subtract the RHS number from the LHS-this matrix and to ...
//... equate and return the resulting matrix to the 'this' matrix object
Matrix& Matrix::operator-=(float&& rhs_const_number) {
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++)
            this->matrix[i][j] -= rhs_const_number;
    }
    return *this;
}

//Overloaded Multiplication-Assignment Operator
//Operator to multiply the RHS matrix and the LHS-this matrix together and...
//... to equate and return the resulting matrix to the 'this' matrix object
//In order to do this, the 'this' matrix will have be cleared out...
//... and resized in dimension-size
Matrix& Matrix::operator*=(const Matrix& rhs_matrix) {
    try {
        if (this->cols != rhs_matrix.rows) 
            throw MutiplicationDimension_exception();
        //Matrix Multiplication Condition
    
    } catch (const MutiplicationDimension_exception& ex) {
        multiplication_ErrorMessage(*this, rhs_matrix);
        return *this;
    }

    //Create temporary matrix that'll store the 'this' matrix entry data
    Matrix temporary(this->rows, this->cols, "Temporary");
    temporary = *this;

    //Deallocate and nullify the 'this' matrix memory addresses
    for (int i = 0; i < this->rows; i++)
        delete[] this->matrix[i];

    delete[] this->matrix;
    this->matrix = nullptr;

    //Reallocate memory to re-scale the 'this' matrix
    this->matrix = new float*[temporary.rows];
    for (int i = 0; i < temporary.rows; i++)
        this->matrix[i] = new float[rhs_matrix.cols];

    //This line below is the problem
    //*this = (temporary * rhs_matrix);   //Multiply and equate
    //Find another way to multiply and equate

    //i => LHS matrix row number, j => RHS matrix column number
    //k => LHS matrix column *AND* RHS matrix row number
    for (int i = 0; i < temporary.rows; i++) {
        for (int j = 0; j < rhs_matrix.cols; j++) {

            float entry_summation = 0;  //Resultant Matrix new entry result
            for (int k = 0; k < rhs_matrix.rows; k++)
                entry_summation += temporary.matrix[i][k] * rhs_matrix.matrix[k][j];

            this->matrix[i][j] = entry_summation;
        }
    }
    return *this;
}
    
//Function is COMPLETE
//Overloaded Multiplication-Assignment Operator
//Operator to multiply a scalar-number to the 'this' matrix and equate...
//... and return the results to the 'this' matrix object 
Matrix& Matrix::operator*=(float&& rhs_const_number) {
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++)
            this->matrix[i][j] *= rhs_const_number;
    }
    return *this;
}
    
//Function is COMPLETE
//Overloaded Equality Check Operator
//Operator to check if the LHS-this matrix is equal to the RHS matrix...
//... by checking if each corresponding entry of the two matrices...
//... are the same. return true if yes, return false if not
//All of the above REGARDLESS of the matrix name
bool Matrix::operator==(const Matrix& rhs_matrix) const {
    if (this == &rhs_matrix) { return true; }   //Same address, i.e same object

    //If the matrices are of the same size-dimension...
    if (this->rows == rhs_matrix.rows && this->cols == rhs_matrix.cols) {
        int counter = this->rows * this->cols;

        //... then start checking if each corresponding entry are the same
        for (int i = 0; i < rhs_matrix.rows; i++) {
            for (int j = 0; j < rhs_matrix.cols; j++) {
                if (this->matrix[i][j] == rhs_matrix.matrix[i][j]) {
                    counter--;
                    if (counter == 0) { return true; }
                } else return false;
            }
        }
    }
    return false;
}

//Function is COMPLETE
//Overloaded Not-Equal-To Equality Operator
//Operator to check if the LHS-this matrix is not equal to the RHS matrix...
//... by checking if each corresponding entry of the two matrices are...
//... equal to each other. If one of the corresponding entries are not...
//... equal to each other, then return true, else if all corresponding...
//... entries are equal then return false.
//All of the above REGARDLESS of the matrix name
bool Matrix::operator!=(const Matrix& rhs_matrix) const {

    //If their addresses are not the same...
    if (this != &rhs_matrix) { return true; }

    //If they are not of the same size-dimension
    if (this->rows != rhs_matrix.rows 
        || this->cols != rhs_matrix.cols) { 
        return true; 
    }

    //If the size-dimensions of the two matrices are the same...
    int counter = this->rows * this->cols;
    for (int i = 0; i < rhs_matrix.rows; i++) {
        for (int j = 0; j < rhs_matrix.cols; j++) {

            if (this->matrix[i][j] != rhs_matrix.matrix[i][j]) { 
                return true; 
            } else {
                counter--;
                if (counter == 0) { return false; }
            }
        }
    }
    return false;
}
    
//Function is COMPLETE
//Overloaded Greater-Than-Equal-To Operator
//Operator to check if the sum of the entries of the LHS-this matrix...
//... is greater than the sum of the entries of the RHS matrix
//Return true if yes, return false if no.
//However, if the two matrices are equal, then return true.
//All of the above REGARDLESS of the matrix name
bool Matrix::operator>=(const Matrix& rhs_matrix) const {
    if (*this == rhs_matrix) { return true; }
    
    float LHS_sum = 0;
    float RHS_sum = 0;
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            LHS_sum += this->matrix[i][j];
            RHS_sum += rhs_matrix.matrix[i][j]; 
        }
    }

    if (LHS_sum >= RHS_sum) { return true; }
    return false;
}

//Function is COMPLETE   
//Overloaded Less-Than-Equal-To Operator
//Operator to check if the sum of the entries of the LHS-this matrix...
//... is less than that of the sum of the entries of the RHS matrix.
//Return true if yes, return false if no.
//However, if the two matrices are equal, then return true.
//All of the above REGARDLESS of the matrix name
bool Matrix::operator<=(const Matrix& rhs_matrix) const {
    if (*this == rhs_matrix) { return true; }

    float LHS_sum = 0;
    float RHS_sum = 0;
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            LHS_sum += this->matrix[i][j];
            RHS_sum += rhs_matrix.matrix[i][j]; 
        }
    }

    if (LHS_sum <= RHS_sum) { return true; }
    return false;
}

//Function is COMPLETE
//Overloaded Greater-Than Operator
//Operator to check if the sum of the entries of the LHS-this matrix...
//... is greater than the sum of the entries of the RHS matrix
//Return true if yes, return false if no.
//All of the above REGARDLESS of the matrix name
bool Matrix::operator>(const Matrix& rhs_matrix) const {
    float LHS_sum = 0;
    float RHS_sum = 0;
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            LHS_sum += this->matrix[i][j];
            RHS_sum += rhs_matrix.matrix[i][j]; 
        }
    }

    if (LHS_sum > RHS_sum) { return true; }
    return false;
}

//Function is COMPLETE
//Overloaded Less-Than Operator
//Operator to check if the sum of the entries of the LHS-this matrix...
//... is less than that of the sum of the entries of the RHS matrix.
//Return true if yes, return false if no.
//All of the above REGARDLESS of the matrix name
bool Matrix::operator<(const Matrix& rhs_matrix) const {
    float LHS_sum = 0;
    float RHS_sum = 0;
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            LHS_sum += this->matrix[i][j];
            RHS_sum += rhs_matrix.matrix[i][j]; 
        }
    }

    if (LHS_sum < RHS_sum) { return true; }
    return false;
}

//Function is COMPLETE
//Function to generate and return random integers between lower and upper (inclusive)
int Matrix::random_entryGenerator(int upper, int lower) {
    int range = upper - lower + 1;
    return rand() % range + lower;
}

//Function is COMPLETE
//Function to allow user to input matrix entries 
void Matrix::data_entryProcedure(const int& rows, const int& cols) {
    cout << "Please input the matrix data entries below for the "
        << this->matrix_name << " matrix:" << endl;
    float matrix_entry;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << this->matrix_name << 
                " matrix entry [" << i << "][" << j << "]: ";
            
            cin >> matrix_entry; 
            this->matrix[i][j] = matrix_entry; 
        }
    }
    cout << endl << endl;
}

//Function is COMPLETE
//Function to display error message when the addition or subtraction...
//... operation is performed on two matrices and they are not of the...
//... same size dimension
void Matrix::dimensionSize_ErrorMessage(const Matrix& lhs_matrix, const Matrix& rhs_matrix) const {
    cerr << "Cannot equate/add/subtract " << rhs_matrix.matrix_name << " Matrix to "
        << lhs_matrix.matrix_name << " Matrix as they are not of the same size "
        << "dimension." << endl << endl;
}

//Function is COMPLETE
//Function to display error message when two matrices cannot be multiplied...
//... together due to the RHS matrix columns not being equal to the LHS matrix rows
void Matrix::multiplication_ErrorMessage(const Matrix& lhs_matrix, const Matrix& rhs_matrix) const {
    cerr << "Cannot multiply the " << lhs_matrix.matrix_name << " Matrix to the "
        << rhs_matrix.matrix_name << " Matrix, due to the number of columns " 
        << "of the " << lhs_matrix.matrix_name << " Matrix not being equal to the "
        << "number of rows of the " << rhs_matrix.matrix_name << " Matrix."
        << endl << endl;
}

//Function is COMPLETE
//Function to check if the 'this' matrix is a Square Matrix
bool Matrix::isSquareMatrix() const {
    if (this->rows == this->cols) { return true; }
    return false;
}

//Function is COMPLETE
//Function to zero-out the entire matrix
void Matrix::zero_outMatrix(Matrix& objMatrix) {
    if (objMatrix.getMatrix_ptrMatrix() == nullptr) { return; }

    for (int i = 0; i < objMatrix.getMatrix_Rows(); i++) {
        for (int j = 0; j < objMatrix.getMatrix_Cols(); j++)
            objMatrix.getMatrix_ptrMatrix()[i][j] = 0;
    }
}

//Function is COMPLETE
//Function to return true if the row at rowIndex is a zero-row
bool Matrix::isZeroRow(const Matrix& reduced, const int& rowIndex) const {
    for (int j = 0; j < reduced.getMatrix_Cols(); j++) {
        if (reduced.getMatrix_ptrMatrix()[rowIndex][j] == 0) {
            if (j == reduced.getMatrix_Cols() - 1) { return true; }
        
        } else { return false; }
    }
    return false;
}

//Function is COMPLETE
//Function to swap two rows of a matrix
void Matrix::swapRows(Matrix& reduced, const int& firstRow, const int& secondRow) {
    float temporaryElement = 0;
    for (int j = 0; j < reduced.getMatrix_Cols(); j++) {
        temporaryElement = reduced.getMatrix_ptrMatrix()[firstRow][j];
        reduced.getMatrix_ptrMatrix()[firstRow][j] = reduced.getMatrix_ptrMatrix()[secondRow][j];
        reduced.getMatrix_ptrMatrix()[secondRow][j] = temporaryElement;
    }
}

//Function is COMPLETE
//Function to find a number to multiply an entire row with such that the...
//... leading variable becomes a Leading One  
void Matrix::multiplyRow(Matrix& reduced, const int& targetRow, const int& start_targetCol) {
    float multiplicaNumber = 0;
    try {
        if (reduced.getMatrix_ptrMatrix()[targetRow][start_targetCol] == 0)
            throw DivideByZero_exception();
        
        multiplicaNumber = 1 / reduced.getMatrix_ptrMatrix()[targetRow][start_targetCol];
    
    } catch (const DivideByZero_exception& ex) {
        cerr << "Couldn't divide " + reduced.getMatrix_Name() 
            << " Matrix row by zero." << endl;
        Matrix::pauseProgram();
        std::exit(EXIT_PROGRAM);
    }

    for (int j = start_targetCol; j < reduced.getMatrix_Cols(); j++)
        reduced.getMatrix_ptrMatrix()[targetRow][j] *= multiplicaNumber;
}

//Function is COMPLETE
//Function to make all leading variables in the reduced matrix positive
//If a leading variable of a row is negative, then multiply that entire row by -1
//FUNCTION TO BE USED AFTER EVERY ROW-OPERATION!!!
void Matrix::make_leadingVariablesPositive(Matrix& reduced) {
    for (int i = 0; i < reduced.getMatrix_Rows(); i++) {
        if (isZeroRow(reduced, i)) { continue; }

        if (reduced.getMatrix_ptrMatrix()[i][0] < 0) {
            for (int j = 0; j < reduced.getMatrix_Cols(); j++)
                reduced.getMatrix_ptrMatrix()[i][j] *= -1;
        
        } else if (reduced.getMatrix_ptrMatrix()[i][0] == 0) {
            for (int j = 1; j < reduced.getMatrix_Cols(); j++) {

                if (reduced.getMatrix_ptrMatrix()[i][j] == 0) { 
                    //Continue searching the columns of the i-th row
                    continue; 

                } else if (reduced.getMatrix_ptrMatrix()[i][j] > 0) { 
                    //Break out of this inner-loop and search the next row
                    break; 

                } else if (reduced.getMatrix_ptrMatrix()[i][j] < 0) {
                    for (int k = j; k < reduced.getMatrix_Cols(); k++)
                        reduced.getMatrix_ptrMatrix()[i][k] *= -1;

                    //Break out of this inner-loop and search the next row
                    break;
                } 
            }
        }
    }
}

//Function is COMPLETE
//Function to SUBTRACT^ a multiple of the constRow from the targetRow...
//... and use those results to update the the targetRow
//Function to make sure to use rows that have Leading Variables in the same column!!!
//^The use of the make_leadingVariablesPositive() function allows...
//... us to only require subtraction of a multiple of a constant row (Correct!)
void Matrix::sub_from_and_multiplyRow(Matrix& reduced, const int& targetRow, const int& constRow) {
    make_leadingVariablesPositive(reduced);
    int COLUMN_TRACKER = 0;
    try {
        for (int j = 0; j < reduced.getMatrix_Cols(); j++) {
            if (reduced.getMatrix_ptrMatrix()[targetRow][j] == 0 &&
                reduced.getMatrix_ptrMatrix()[constRow][j] == 0) {
                continue;

            } else if (reduced.getMatrix_ptrMatrix()[targetRow][j] > 0
                && reduced.getMatrix_ptrMatrix()[constRow][j] > 0) {
                COLUMN_TRACKER = j;
                break;

            } else if ((reduced.getMatrix_ptrMatrix()[targetRow][j] == 0 && reduced.getMatrix_ptrMatrix()[constRow][j] > 0)
                || (reduced.getMatrix_ptrMatrix()[targetRow][j] > 0 && reduced.getMatrix_ptrMatrix()[constRow][j] == 0)) 
            {
                throw InvalidRowOperation_exception();
            }
        }

    } catch (const InvalidRowOperation_exception& ex) {
        cerr << "Operation Matrix::sub_from_and_multiplyRow() is invalid as target-row and "
            << "constant-row are incompatible (their leading variables are not in "
            << "the same column for the operation to be performed)." << endl;
        Matrix::pauseProgram();
        std::exit(EXIT_PROGRAM);    //For testing reasons (TO-BE changed)
    }
    
    //Find a multiplication factor such that...
    //... targetRow[leading variable] - (factor) * constRow[leading variable] = 0
    //...where 0 is the new value of reduced.getMatrix_ptrMatrix()[targetRow][0]
    float multiplicaFactor = reduced.getMatrix_ptrMatrix()[targetRow][COLUMN_TRACKER]
                            / reduced.getMatrix_ptrMatrix()[constRow][COLUMN_TRACKER];
    for (int j = COLUMN_TRACKER; j < reduced.getMatrix_Cols(); j++) {
        reduced.getMatrix_ptrMatrix()[targetRow][j] -= multiplicaFactor * reduced.getMatrix_ptrMatrix()[constRow][j];
    }
}

//Function is COMPLETE
//Function to move all the zero-rows of the matrix to the bottom of the matrix
//Function to use a vector of booleans to keep track of the locations of the zero rows
//Indicies of the vector represent the row numbers of the matrix
void Matrix::move_zeroRows(Matrix& reduced) {
    try {
        if (reduced.zeroRow_recordLog.size() != 0 && 
            reduced.zeroRow_recordLog.size() != static_cast<size_t>(reduced.getMatrix_Rows())) 
        {
            throw NullVector_exception();
        }

        //Flush/Delete the record log so that it may be updated
        while (reduced.zeroRow_recordLog.size() != 0)
            reduced.zeroRow_recordLog.pop_back();
            
    } catch (const NullVector_exception& ex) {
        cerr << "Inside move_zeroRows() function. "
            << "Could not execute further because zeroRow_recordLog.size() != 0 AND "
            << "zeroRow_recordLog.size() != reduced.getMatrix_Rows()" << endl;
        Matrix::pauseProgram();
        std::exit(EXIT_PROGRAM);
    }

    //When moving zero-rows, the zero-row record log will be updated
    for (int i = 0; i < reduced.getMatrix_Rows(); i++) {
        if (!isZeroRow(reduced, i))
            reduced.zeroRow_recordLog.push_back(false);
        else reduced.zeroRow_recordLog.push_back(true);
    }  

    //Use the vector record log to re-sort the matrix, sort the vector record log too
    size_t zeroRow_Tracker = reduced.zeroRow_recordLog.size();
    for (size_t k = 0; k < reduced.zeroRow_recordLog.size(); k++) {
        if (reduced.zeroRow_recordLog[zeroRow_Tracker - 1])
            zeroRow_Tracker -= 1;
        else break;
    }

    for (size_t i = 0; i < reduced.zeroRow_recordLog.size(); i++) {
        if (reduced.zeroRow_recordLog[i]) {
            zeroRow_Tracker -= 1;
            reduced.zeroRow_recordLog[i] = false;
            reduced.zeroRow_recordLog[zeroRow_Tracker] = true;
            swapRows(reduced, i, zeroRow_Tracker);
        }
    }
}

//Function is COMPLETE
//Function to return true if a row has a leading 1 and all entries below that leading 1 are all zeros
bool Matrix::isCanMatrixProgress(const Matrix& reduced, const int& row_absoluteTracker) const {
    int zeroColumn_Tracker = 0;
    for (int j = 0; j < reduced.getMatrix_Cols(); j++) {
        if (reduced.getMatrix_ptrMatrix()[row_absoluteTracker][j] == 0) {
            continue;

        } else if (reduced.getMatrix_ptrMatrix()[row_absoluteTracker][j] == 1) {
            zeroColumn_Tracker = j;
            break;  //Leading 1 has been found

        } else { return false; }
    }

    //If a leading 1 has been found, search if elements under that leading 1 are zeros
    if (row_absoluteTracker == reduced.getMatrix_Rows() - 1) { return true; }

    for (int i = row_absoluteTracker + 1; i < reduced.getMatrix_Rows(); i++) {
        if (reduced.getMatrix_ptrMatrix()[i][zeroColumn_Tracker] == 0) {
            if (i == reduced.getMatrix_Rows() - 1)
                return true;
            else continue;

        } else { return false; }
    }
    return false;
}

//Function is COMPLETE
//Function to determine and return true if there are more variables than independent equations...
//... i.e, when the AUGMENTED matrix has more columns than rows  
bool Matrix::isInfinitlyManySolutions(const Matrix& reduced) const {
    return (reduced.getMatrix_Cols() - 1 > reduced.getMatrix_Rows());
}

//Function is COMPLETE
//Function to determine and return true if the reduced AUGMENTED matrix has no solution
bool Matrix::isNoSolutionSet(const Matrix& reduced) const {
    for (int i = 0; i < reduced.getMatrix_Rows(); i++) {
        if (isZeroRow(reduced, i)) { break; }

        for (int j = 0; j < reduced.getMatrix_Cols() - 1; j++) {
            if (reduced.getMatrix_ptrMatrix()[i][j] == 0 && j != reduced.getMatrix_Cols() - 2) {
                continue;

            } else if (reduced.getMatrix_ptrMatrix()[i][j] == 0 && j == reduced.getMatrix_Cols() - 2) {
                if (reduced.getMatrix_ptrMatrix()[i][j] != reduced.getMatrix_ptrMatrix()[i][j + 1])
                    return true; 

            } else { break; }
        }
    }
    return false;
}

void Matrix::InfinitlyManySolutions_Message(const Matrix& objMatrix) const {
    cout << "The" + objMatrix.getMatrix_Name() + " Matrix above has Infinitly Many Solutions as there"
        << " are more variables (columns) than independent equations (rows)." << endl << endl;
}

void Matrix::NoSolutionSet_Message(const Matrix& objMatrix) const {
    cout << "The " + objMatrix.getMatrix_Name() + " Matrix above has No Solution Set." << endl << endl;
}