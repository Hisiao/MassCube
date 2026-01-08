/******************************************************************************
File: CAdNeuralNetCoordCalc.h

Description: Class declaration for CAdNeuralNetCoordCalc, which serves as a
base class for spatial coordinate calculators using a neural network
to calculate coordinates in a particular spatial domain.

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
  1.0          1/4/2011 created
  
******************************************************************************/
#ifndef CADNEURALNETCOORDCALC_H
#define CADNEURALNETCOORDCALC_H

#include "CMagfield.h"
#include "CTimeValue.h"
#include "VMTypes.h"

//#include <cmath>
#include <limits>

// Use the std NaN constant as a double bad value flag
const double STD_NaN = std::numeric_limits<double>::quiet_NaN();

// define vector of time values
typedef vector<CTimeValue>     tvector;


  class CAdNeuralNetCoordCalc
  {
  public:
      CAdNeuralNetCoordCalc();
      virtual ~CAdNeuralNetCoordCalc();

      eGENERIC_ERROR_CODE loadNeuralNet( const string& strPath );

      // virtual method called by base class to perform model specific computation

      virtual eGENERIC_ERROR_CODE calculateCoords(
                                    emfCoordSys eCoordSys,
                                    const tvector&  vtTimes,        // [ nTimes ]
                                    const S3TupleVec& s3vPositions, // [ nTimes ]
                                    const dvector& vdPitchAngles,   // [ nAngles ]
                                    const std::bdmatrix& bmdB,           // [ nTimes, 3 (x,y,z) ]
                                    const dvector&  vdBmin,         // [ nTimes ]
                                    const dvector&  vdBlocal,       // [ nTimes ]
                                    const std::bdmatrix& bmdI,           // [ nTimes, nAngles ]
                                    const std::bdmatrix& bmdBmirror,     // [ nTimes, nAngles ]
                                    const std::bdmatrix& bmdLm,          // [ nTimes, nAngles ]
                                    const double&   dX2Min,
                                    const double&   dX2Max,
                                    const double&   dX3Min,
                                    const double&   dX3Max,
                                    std::bdmatrix&       bmdX2,          // out [ nTimes, nAngles ]
                                    std::bdmatrix&       bmdX3 ) = 0;    // out [ nTimes, nAngles ]

  protected:

      eGENERIC_ERROR_CODE neuralNetEval( std::bdmatrix& bmdX, bvector& vbInRange, std::bdmatrix& bmdY );

      double polynomialEval( dvector& vdCoefsDescending, double dX );

      // protected data members

      int      m_iNumX;
      int      m_iNumH;
      int      m_iNumY;
      dvector  m_vdXBar;
      dvector  m_vdSx;
      dvector  m_vdYBar;
      dvector  m_vdSy;
      dvector  m_vdThetaStructW;
      std::bdmatrix m_bmdW;
      dvector  m_vdThetaStructW0;
      dvector  m_vdThetaStructV;
      std::bdmatrix m_bmdV;
      dvector  m_vdThetaStructV0;
      
      // data members

      // persisted fields

      uivector m_vuiNh;
      uivector m_vuiNhDims;

      uivector m_vuiNx;
      uivector m_vuiNxDims;

      uivector m_vuiNy;
      uivector m_vuiNyDims;

      uivector m_vuiSxDims;
      uivector m_vuiSyDims;

      dvector  m_vdTheta;
      uivector m_vuiThetaDims;

      dvector  m_vdPolyCoeffsLog10B;
      uivector m_vuiPolyCoeffsLog10BDims;

      dvector  m_vdMaxMJD;
      uivector m_vuiMaxMJDDims;

      dvector  m_vdMaxYear;
      uivector m_vuiMaxYearDims;

      dvector  m_vdMaxRoot4J;
      uivector m_vuiMaxRoot4JDims;

      uivector m_vuiXBarDims;
      uivector m_vuiYBarDims;

      uivector m_vuiThetaStructNh;
      uivector m_vuiThetaStructNhDims;

      uivector m_vuiThetaStructNx;
      uivector m_vuiThetaStructNxDims;

      uivector m_vuiThetaStructNy;
      uivector m_vuiThetaStructNyDims;

      dvector m_vdThetaStructNTheta;
      uivector m_vuiThetaStructNThetaDims;

      uivector m_vuiThetaStructVDims;
      uivector m_vuiThetaStructV0Dims;
      uivector m_vuiThetaStructWDims;
      uivector m_vuiThetaStructW0Dims;

   };
#endif
