# Linear-Algebra-Engine

A custom-built **C++ Linear Algebra Engine** designed to perform core matrix and vector operations using **Object-Oriented Programming, operator overloading, and polymorphism**.  
This project serves both as an **educational tool for exploring linear algebra computationally** and as a **demonstration of clean software engineering practices in C++**.
This program is designed to perform various matrix operations and transformations, e.g., addition, multiplication, elimination, inversion, etc.

---

## ✅ Features Implemented

| Feature | Status |
|---------|--------|
| Matrix class with dynamic memory management | ✅ |
| State (column) vector class inheriting from Matrix | ✅ |
| Operator overloading (`+`, `-`, `*`, `=`, `<<`, etc.) | ✅ |
| Matrix addition & subtraction | ✅ |
| Matrix multiplication (Matrix × Matrix, Matrix × Vector, etc.) | ✅ |
| Scalar multiplication | ✅ |
| Inner & Outer product for vectors | ✅ |
| Gaussian Elimination / Row Reduction | ✅ |
| Error handling for dimension mismatches | ✅ |

---

## 🧠 Project Structure
Linear-Algebra-Engine/
│── Matrix.h / Matrix.cpp → Core matrix implementation
│── State_Vector.h / State_Vector.cpp → Derived vector class
│── Utility.cpp → (Reserved for helper functions and classes)
│── main.cpp → Test harness for Gaussian Elimination


---

## 🔧 Compilation & Execution

Using **g++** in the Windows Command Prompt:

```bash
  g++ -Wall -std=c++17 main.cpp
  g++ -Wall -std=c++17 main.cpp -o main.exe
  main
