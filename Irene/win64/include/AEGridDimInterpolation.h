/******************************************************************************
$HeadURL$

File: AEGridDimInterpolation.h

Description: AEGridDimInterpolation is a base class for classes that define specific
    methods of interpolating between the coordinates of an AEGridDimension object. 

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

  Version      Date      Notes
  1.0          
  
SVNTag: $Id$
******************************************************************************/
#ifndef AEGRIDDIMINTERPOLATION
#define AEGRIDDIMINTERPOLATION

#include "AETypes.h"

namespace ae9ap9 
{
    class AEGridDimInterpolation
    {
    public:
        AEGridDimInterpolation();
        virtual ~AEGridDimInterpolation();

        virtual AEGridDimInterpolation* clone();

        int getNeighbors( dvector& vdKnownCoords, double dCoordinate, int& iBefore, int& iAfter );
        int getNeighbors( dvector& vdKnownCoords, const dvector& vdCoordinates, ivector& viBefore,
                          ivector& viAfter );
    protected:
    };

} /*namespace*/

#endif
