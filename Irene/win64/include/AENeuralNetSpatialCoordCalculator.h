/******************************************************************************
$HeadURL$

File: AENeuralNetSpatialCoordCalculator.h

Description: Class declaration for AENeuralNetSpatialCoordCalculator, which
serves as a base class for spatial coordinate calculators using a neural network
to calculate coordinates in a particular spatial domain.

 Classification:

   Unclassified

 Project Name:

   AE9/AP9/SPM Radiation Environment Models

   Developed under US Government contract # FA9453-12-C-0231

 Rights and Restrictions:

   Copyright 2014 Atmospheric and Environmental Research, Inc. (AER)

   DISTRIBUTION A. Approved for public release; distribution is unlimited.

   The AE9/AP9/SPM software license is contained in the 'documents/Licenses' 
   folder of this distribution file collection.

 Author:

   This software was developed by AER staff

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
  
SVNTag: $Id$
******************************************************************************/
#ifndef AENEURALNETSPATIALCOORDCALCULATOR_H
#define AENEURALNETSPATIALCOORDCALCULATOR_H

#include "AESpatialCoordCalculator.h"
#include "AEDBNeuralNet.h"

namespace ae9ap9
{
    class AENeuralNetSpatialCoordCalculator : public AESpatialCoordCalculator
    {
    public:
        AENeuralNetSpatialCoordCalculator();
        virtual ~AENeuralNetSpatialCoordCalculator();

        void setNeuralNetDataSource( const string& strNNDataSource ) 
                {m_strNeuralNetDataSource = strNNDataSource; }

        int loadNeuralNet();

    protected:

        // virtual method called by base class to perform model specific computation

        virtual int calculateModelX2X3(emfCoordSys     eCoordSys,
                                       const dvector&  vdDateTimes,    // [ nTimes ]
                                       const dvector&  vdCoordsAxis1,  // [ nTimes ]
                                       const dvector&  vdCoordsAxis2,  // [ nTimes ] 
                                       const dvector&  vdCoordsAxis3,  // [ nTimes ]
                                       const bdmatrix& bmdPitchAngles, // [ nTimes or 1, nAngles  ]
                                       const bdmatrix& bmdB,           // [ nTimes, 3 (x,y,z) ]
                                       const dvector&  vdBmin,         // [ nTimes ]
                                       const dvector&  vdBlocal,       // [ nTimes ]
                                       const bdmatrix& bmdI,           // [ nTimes, nAngles ]
                                       const bdmatrix& bmdBmirror,     // [ nTimes, nAngles ]
                                       const bdmatrix& bmdLm,          // [ nTimes, nAngles ]
                                       bdmatrix&       bmdX2,          // out [ nTimes, nAngles ]
                                       bdmatrix&       bmdX3 ) = 0;    // out [ nTimes, nAngles ]

        int neuralNetEval( bdmatrix& bmdX, bvector& vbInRange, bdmatrix& bmdY );

        double polynomialEval( dvector& vdCoefsDescending, double dX );

        // protected data members

        string  m_strNeuralNetDataSource;

        AEDBNeuralNet* m_pDBNeuralNet;

        int      m_iNumX;
        int      m_iNumH;
        int      m_iNumY;
        dvector  m_vdXBar;
        dvector  m_vdSX;
        dvector  m_vdYBar;
        dvector  m_vdSY;
        dvector  m_vdThetaStructW;
        bdmatrix m_bmdW;
        dvector  m_vdThetaStructW0;
        dvector  m_vdThetaStructV;
        bdmatrix m_bmdV;
        dvector  m_vdThetaStructV0;
    };
}

#endif
