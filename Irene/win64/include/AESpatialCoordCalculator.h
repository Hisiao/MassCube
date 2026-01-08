/******************************************************************************
$HeadURL$

File: AESpatialCoordCalculator.h

Description: Class declaration for the AESpatialCoordCalculator class, which
defines the interface for all spatial coordinate calculators. The currently
supported spatial domains are: alpha, Lm; K, Phi; K, HMin.

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
#ifndef AESPATIALCOORDCALCULATOR_H
#define AESPATIALCOORDCALCULATOR_H

#include <memory>
#include <string>
using namespace std;
#include "CMagfield.h"
#include "AETypes.h"
#include "AEEnums.h"
#include "AEErrorHandler.h"

namespace ae9ap9
{
    class AESpatialCoordCalculator : public AEErrorHandler
    {
    public:

        AESpatialCoordCalculator();
        virtual ~AESpatialCoordCalculator();

        // virtual model specific flux parameters calculator method

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

    protected:

      // internal helper methods

      // protected data members

      const double   dDG2RAD;

    };
}

#endif
