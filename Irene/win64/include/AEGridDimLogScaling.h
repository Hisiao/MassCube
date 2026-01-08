/******************************************************************************
$HeadURL$

File: AEGridDimLogScaling.h

Description: AEGridDimLogScaling is a class that defines the scaling the coordinates 
    of an AEGridDimension object using a natural log scale.

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
#ifndef AEGRIDDIMLOGSCALING
#define AEGRIDDIMLOGSCALING

#include "AEGridDimScaling.h"

namespace ae9ap9 
{
    class AEGridDimLogScaling : public AEGridDimScaling
    {
    public:
        AEGridDimLogScaling();
        ~AEGridDimLogScaling();

        // self copy
        AEGridDimLogScaling* clone();

        int physicalToGrid( double dPhysicalCoord, double& dGridCoord );
        int gridToPhysical( double dGridCoord, double& dPhysicalCoord );

    protected:
    };

} /*namespace*/

#endif
