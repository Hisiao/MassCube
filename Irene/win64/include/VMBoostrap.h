/******************************************************************************
File: VMBoostrap.h

Description: Non-creatable class defining static helper methods for initializing
or, yes that's right; boo(t)strapping boost collection classes from the 
multi-dimensional std vectors used in the Ae9Ap9 database layer to store data.

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
  1.0          10/20/2009 created
  
******************************************************************************/
#ifndef VMBooSTRAP_H
#define VMBooSTRAP_H

#include "VMTypes.h"

#include "boost/numeric/ublas/vector.hpp" // from boost ublas library
#include "boost/numeric/ublas/matrix.hpp" // from boost ublas library

  // Static helper methods for initializing boost collections from 
  // multi-dimensional vectors used in the ae9ap9 database layer

  class VMBoostrap
  {
  public:
      template <class T, class A>
      static int bvecFrom2DStdVec( boost::numeric::ublas::vector<T, A>& bvec, 
                                   const std::vector<T>& stdvec,
                                   int iRowMin, int iRowMax, int iRowSize,
                                   int iColMin, int iColMax );

      template <class T, class L, class A>
      static int matrixFrom2DStdVec( boost::numeric::ublas::matrix<T, L, A>& bmatrix, 
                                     const std::vector<T>& stdvec, 
                                     int iRowMin, int iRowMax, int iRowSize,
                                     int iColMin, int iColMax );

      template <class T, class A>
      static int applyUnaryFnToBvec( const boost::numeric::ublas::vector<T, A>& bvecIn, 
                                     boost::numeric::ublas::vector<T, A>& bvecOut, 
                                     T unaryFn(T) );

      template <class T, class A>
      static int applyBinaryFnToBvec( const boost::numeric::ublas::vector<T, A>& bvec1In, 
                                      const boost::numeric::ublas::vector<T, A>& bvec2In, 
                                      boost::numeric::ublas::vector<T, A>& bvecOut, 
                                      T binaryFn(T,T) );

      template <class T, class L, class A>
      static int applyUnaryFnToMatrix( const boost::numeric::ublas::matrix<T, L, A>& bmatrixIn, 
                                       boost::numeric::ublas::matrix<T, L, A>& bmatrixOut, 
                                       T unaryFn(T) );

      template <class T, class L, class A>
      static int applyBinaryFnToMatrix( const boost::numeric::ublas::matrix<T, L, A>& bmatrix1In, 
                                        const boost::numeric::ublas::matrix<T, L, A>& bmatrix2In, 
                                        boost::numeric::ublas::matrix<T, L, A>& bmatrixOut, 
                                       T binaryFn(T,T) );

      template<class T, std::size_t IB, class IA, class TA>
      static int applyUnaryFnToBcvec( const boost::numeric::ublas::compressed_vector<T, IB, IA, TA>& bvecIn, 
                                      boost::numeric::ublas::compressed_vector<T, IB, IA, TA>& bvecOut, 
                                      T unaryFn(T) );

      template<class T, std::size_t IB, class IA, class TA>
      static int applyBinaryFnToBcvec( const boost::numeric::ublas::compressed_vector<T, IB, IA, TA>& bvec1In, 
                                       const boost::numeric::ublas::vector<T, TA>& bvec2In, 
                                       boost::numeric::ublas::compressed_vector<T, IB, IA, TA>& bvecOut, 
                                       T binaryFn(T,T) );

      static int prodOfBvecMatrixCompressed( const bdvector& bvectorIn, 
                                             const bdmatrix& bmatrixIn,
                                             const bdcvector& cvecIndicesUsed, 
                                             bdcvector& bvectorOut );


      // Callback fns used in conjunction with iterative methods above to
      // perform the action on individual elements of the collection

      static double normCDF( double dZ );           // applied to container elements "normal distrib, cum. dist @ Z
      static double normInverseCDF( double dPct );  // applied to container elements "normal distrib, inv. cum. dist @ Pct

  private:
      VMBoostrap(){};  // non-creatable class
      ~VMBoostrap(){};
  };


#endif
