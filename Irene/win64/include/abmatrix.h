/***********************************************************************

Module: AbMatrix.h

Description: Declarations for a templated functions for a vector of vectors. 
   Can be one of 'int', 'double', 'bool' or 'string' data types.  No mixing
   of data types within single matrix is allowed.

 Rights and Restrictions:

   Copyright 2014 Atmospheric and Environmental Research, Inc. (AER)

   DISTRIBUTION A. Approved for public release; distribution is unlimited. 

 Author:

   This software was developed and/or modified by AER staff

 Contact:

   Atmospheric and Environmental Research, Inc.
   131 Hartwell Avenue
   Lexington, MA 02421-3126 USA
   Phone: 781.761.2288
   email: spwx@aer.com

 References:

   None

 Revision history:

   VERSION     DATE         NOTES
// Dmatrix header  definition from Amptrace base_0.h
// Original author: Michael Starks
//  converted to abstract container by CJ Roth
     1.0       08/29/2011   Created

------------------------------------------------------------------------

***********************************************************************/

#include <iostream>
#include <exception>
#include <vector>
#include <string>

#include <stdio.h>

#include "SpWxTypes.h"

// Dmatrix header  definition from Amptrace base_0.h
// Original author: Michael Starks
//  converted to abstract container by CJ Roth


#ifndef ABMATRIX
#define ABMATRIX

using namespace std;

template <class Type>
class AbMatrix
{

 public:

  AbMatrix() { };
  AbMatrix(unsigned int, unsigned int);

  // matrix sizing
  void clear();
  void reserve(size_t);
  size_t capacity() const;
  size_t size() const;
  void resize(unsigned int, unsigned int);

  // content insertion and deletion
  void push_back(const vector<Type>&);
  void insert(unsigned int, vector<Type>&);
  void erase(unsigned int, unsigned int);

  // operators
  vector<Type>& operator[](unsigned int);
  //   '*' operator is undefined for 'bool' and 'string' types
  vector<Type> operator*(const vector<Type>&);

 private:
  vector< vector<Type> > matData;

//#  unable to make this work as template
//  friend ostream& operator<< <>(ostream&, const AbMatrix<Type>&);  // stream extraction
};

typedef AbMatrix<double> dmatrix;
typedef AbMatrix<string> smatrix;
typedef AbMatrix<bool> bmatrix;
typedef AbMatrix<int> imatrix;
#endif  // ABMATRIX

