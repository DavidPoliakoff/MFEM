// Copyright (c) 2010, Lawrence Livermore National Security, LLC. Produced at
// the Lawrence Livermore National Laboratory. LLNL-CODE-443211. All Rights
// reserved. See file COPYRIGHT for details.
//
// This file is part of the MFEM library. For more information and source code
// availability see http://mfem.org.
//
// MFEM is free software; you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License (as published by the Free
// Software Foundation) version 2.1 dated February 1999.

#ifndef MFEM_MATRIX
#define MFEM_MATRIX

#include "../general/array.hpp"
#include "../general/globals.hpp"
#include "operator.hpp"

namespace mfem
{

// Abstract data types matrix, inverse matrix

class MatrixInverse;

/// Abstract data type matrix
class Matrix : public Operator
{
   friend class MatrixInverse;
public:
   /// Creates a square matrix of size s.
   explicit Matrix(int s) : Operator(s) { }

   /// Creates a matrix of the given height and width.
   explicit Matrix(int h, int w) : Operator(h, w) { }

   /// Returns reference to a_{ij}.
   virtual double &Elem(int i, int j) = 0;

   /// Returns constant reference to a_{ij}.
   virtual const double &Elem(int i, int j) const = 0;

   /// Returns a pointer to (an approximation) of the matrix inverse.
   virtual MatrixInverse *Inverse() const = 0;

   /// Finalizes the matrix initialization.
   virtual void Finalize(int) { }

   /// Prints matrix to stream out.
   virtual void Print (std::ostream & out = mfem::out, int width_ = 4) const;

   /// Destroys matrix.
   virtual ~Matrix() { }
};


/// Abstract data type for matrix inverse
class MatrixInverse : public Solver
{
public:
   MatrixInverse() { }

   /// Creates approximation of the inverse of square matrix
   MatrixInverse(const Matrix &mat)
      : Solver(mat.height, mat.width) { }
};

/// Abstract data type for sparse matrices
class AbstractSparseMatrix : public Matrix
{
public:
   /// Creates a square matrix of the given size.
   explicit AbstractSparseMatrix(int s = 0) : Matrix(s) { }

   /// Creates a matrix of the given height and width.
   explicit AbstractSparseMatrix(int h, int w) : Matrix(h, w) { }

   /// Returns the number of non-zeros in a matrix
   virtual int NumNonZeroElems() const = 0;

   /** Gets the columns indexes and values for row *row*.
       Returns:
       0 if cols and srow are copies of the values in the matrix.
       1 if cols and srow are views of the values in the matrix. */
   virtual int GetRow(const int row, Array<int> &cols, Vector &srow) const = 0;

   /** If the matrix is square, it will place 1 on the diagonal (i,i) if row i
       has "almost" zero l1-norm.
       If entry (i,i) does not belong to the sparsity pattern of A, then an
       error will occur. */
   virtual void EliminateZeroRows() = 0;

   /// Matrix-Vector Multiplication y = A*x
   virtual void Mult(const Vector &x, Vector &y) const = 0;
   /// Matrix-Vector Multiplication y = y + val*A*x
   virtual void AddMult(const Vector &x, Vector &y,
                        const double val = 1.) const = 0;
   /// MatrixTranspose-Vector Multiplication y = A'*x
   virtual void MultTranspose(const Vector &x, Vector &y) const = 0;
   /// MatrixTranspose-Vector Multiplication y = y + val*A'*x
   virtual void AddMultTranspose(const Vector &x, Vector &y,
                                 const double val = 1.) const = 0;

   /// Destroys AbstractSparseMatrix.
   virtual ~AbstractSparseMatrix() { }
};

}

#endif
