/***********************************************************************

Module: FloatMath.h

Description: Declarations for variety of high-precision floating point math operations. 

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
     1.0       08/16/2011   Created

------------------------------------------------------------------------

***********************************************************************/

#ifndef FLOATMATH_H
#define FLOATMATH_H

#include "SpWxTypes.h" // needed for S3Tuple definition

class CFloatMath
{
 public:
  CFloatMath();
  ~CFloatMath();

public:
  // dblCompare: computes relative or absolute difference between
  // doubles with specified tolerances, returning the computed
  // difference and comparison state.

  static bool dblCompare(const double& dA, const double& dB,
                  const double& dRelTol, const double& dAbsTol,
                  double& dDiff);

  // version with default tolerances, dRelTol=1e-9, dAbsTol=1e-15

  static bool dblCompare(const double& dA, const double& dB, double& dDiff) 
  {
    return dblCompare(dA, dB, 1.e-9, 1.e-15, dDiff);
  }

  /////////// BELOW HERE NOT IMPLEMENTED YET //////////////
  // dblCompareUlp: like dblCompare but tolerances are based on ULP
  // (ULP = units in the last place); see eg
  // http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm
  bool dblCompareUlp(const double& dA, const double& dB,
                     const double& dMaxUlps, double& dDiff);
  
  // preciseSum: compute sum robust against floating point error
  // accumulation; see
  // http://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html
  // Theorem 8 (Kahan Summation Formula)
  double preciseSum(const dvector& dvIn);
  double runningSum(const double& dIn);
  void clearRunningSum();

  // preciseQuadRoots: compute quadratic roots robust against floating
  // point error accumulation; see eg:
  // http://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/overview/reals.html
  // or Numerical Recipes
  void preciseQuadRoots(const double& a, const double& b, const double& c, double& r1, double& r2);

  // preciseVariance: compute variance and stddev precisely perhaps
  // should be own class?
  // see http://www.johndcook.com/standard_deviation.html and
  // http://www.johndcook.com/blog/2008/09/26/comparing-three-methods-of-computing-standard-deviation/

private:
   double m_RunningSum;

};

#endif
