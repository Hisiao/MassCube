/******************************************************************************
File: CAdKPhiCoordCalc.h

Description: Class declaration for CAdKPhiCoordCalc, which
computes the K and Phi dimension coordinates from geospatial coordinates

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
#ifndef CADKPHICOORDCALC_H
#define CADKPHICOORDCALC_H

#include "CAdNeuralNetCoordCalc.h"

  class CAdKPhiCoordCalc : public CAdNeuralNetCoordCalc
  {
  public:
      CAdKPhiCoordCalc();
      ~CAdKPhiCoordCalc();

      // virtual method called by base class to perform model specific computation

      eGENERIC_ERROR_CODE calculateCoords( 
                            emfCoordSys  eCoordSys,
                            const tvector& vtTimes,         // [ nTimes ]
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
                            std::bdmatrix&       bmdX3 );        // out [ nTimes, nAngles ]
//  protected:

  };


#endif
