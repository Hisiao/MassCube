/******************************************************************************
File: VMTypes.h

Description: Datatype declarations used for Vectors and Matrices

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

  Version      Date       Notes
  1.0          09/02/2009 created
  
******************************************************************************/
#ifndef VMTYPES_H
#define VMTYPES_H

#include <vector>
using namespace std;

// One dimensional vector of doubles, ect..., using std::vector template

typedef vector<char>           cvector;

typedef vector<unsigned char>  ucvector;

typedef vector<short>          sivector; // avoid conflict svector in SpWx

typedef vector<unsigned short> usvector;

typedef vector<int>            ivector;

typedef vector<unsigned int>   uivector;

typedef vector<bool>           bvector;

// Two dimensional vector of unsigned ints

typedef vector<uivector>       vuivector;

typedef vector<float>          fvector;

typedef vector<double>         dvector;

// Two dimensional vector of doubles

typedef vector<dvector>        vdvector;

// Three dimensional vector of doubles

typedef vector<vdvector>       vvdvector;

#include "boost/multi_array.hpp"                 // from boost library
#include "boost/numeric/ublas/vector.hpp"        // from boost ublas library
#include "boost/numeric/ublas/matrix.hpp"        // from boost ublas library
#include "boost/numeric/ublas/matrix_sparse.hpp" // from boost ublas library
#include "boost/numeric/ublas/vector_sparse.hpp" // from boost ublas library
//using namespace boost;

namespace std 
{
    // Three dimensional array of doubles 
    // (for returning dimensional flux results)

    typedef boost::multi_array<double, 3> array_3d;

    // Avoid confusion and possible namespace collisions with std vectors
    // by effectively renaming boost vectors. Do the same with the matrix
    // purely for convenience

    typedef boost::numeric::ublas::vector<double>       bdvector;
    typedef boost::numeric::ublas::vector<unsigned int> buivector;

    typedef boost::numeric::ublas::matrix<double>       bdmatrix;
    typedef boost::numeric::ublas::matrix<unsigned int> buimatrix;

    typedef vector<bdvector> vbdvector;
    typedef vector<bdmatrix> vbdmatrix;

    typedef boost::numeric::ublas::compressed_matrix<double> bdcmatrix;

    typedef bdcmatrix::iterator1 bdcmIter1;
    typedef bdcmatrix::iterator2 bdcmIter2;

    typedef vector<bdcmatrix> vbdcmatrix;

    typedef boost::numeric::ublas::compressed_vector<bool> bbcvector;
    typedef bbcvector::iterator bbcvIter;
    typedef bbcvector::const_iterator bbcvcIter;

    typedef boost::numeric::ublas::compressed_vector<double> bdcvector;
    typedef bdcvector::iterator bdcvIter;
    typedef bdcvector::const_iterator bdcvcIter;

} /*namespace*/

#endif
