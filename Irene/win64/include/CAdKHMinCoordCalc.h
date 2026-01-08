/******************************************************************************
File: CAdKHMinCoordCalc.h

Description: Class declaration for CAdKHMinCoordCalc, which
computes the K and HMin dimension coordinates from geospatial coordinates.

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
#ifndef CADKHMINCOORDCALC_H
#define CADKHMINCOORDCALC_H

#include "CAdNeuralNetCoordCalc.h"

/**
 * @file: CAdKHMinCoordCalc.h
 * Performs the bulk of the calculation of K, HMin values common to both omnidirectional and directional calculations
 */
  class CAdKHMinCoordCalc : public CAdNeuralNetCoordCalc
  {
  public:
      CAdKHMinCoordCalc();
      ~CAdKHMinCoordCalc();

      // virtual method called by base class to perform model specific computation
      /**
      *
      * Performs the bulk of the calculation of K, HMin values; which
      * is common to both omnidirectional and directional calculations
      *
      *    @param eCoordSys
      *           Enumerated id of reference coordinate system (see AEEnums.h)
      *    @param vtTimes
      *           vector of TimeValues for each input position
      *    @param s3vPositions
      *           X, y, z coordinates of each input position to process
      *    @param vdPitchAngles
      *           matrix of pitch angles to compute: [1 | nTimes] x nAngles
      *    @param bmdB
      *           matrix of B vector x,y,z components [ntimes, 3]
      *    @param vdBmin
      *           vector of Bmin values [ntimes] for each positions field line
      *    @param vdBlocal
      *           vector of B magnitude values [ntimes] for each positions field line
      *    @param bmdI
      *           matrix of McIlWain's invariant I [ntimes, nAngles]
      *    @param bmdBmirror
      *           matrix of B mirror values for each position, angle [ntimes, nAngles]
      *    @param bmdLm
      *           matrix of input McIlWain's L (Lm) values [ntimes, nAngles]
      *    @param dX2Min
      *           minimum accepted value for X2 values
      *    @param dX2Max
      *           maximum accepted value for X2 values
      *    @param dX3Min
      *           minimum accepted value for X3 values
      *    @param dX3Max
      *           maximum accepted value for X3 values
      *    @param[out] bmdX2
      *           matrix to hold K values to be computed
      *    @param[out] bmdX3
      *           matrix to hold Hmin values to be computed
      *    @return eGENERIC_ERROR_CODE
      *           enumerated error code (see GenericModelEnum.h)
      */
      eGENERIC_ERROR_CODE calculateCoords( 
                            emfCoordSys  eCoordSys,
                            const tvector& vtTimes,         // [ nTimes ]
                            const S3TupleVec& s3vPositions, // [ nTimes ]
                            const dvector& vdPitchAngles,   // [ nAngles ]
                            const std::bdmatrix& bmdB,      // [ nTimes, 3 (x,y,z) ]
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
  };

#endif
