/******************************************************************************
$HeadURL$

File: AEGridDimScaling.h

Description: AEGridDimScaling is a base class for classes that define specific
    methods of scaling the coordinates of an AEGridDimension object. This class
    also serves as default implementation for unscaled dimensions 
    (ie: physical = grid coordinate)

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
#ifndef AEGRIDDIMSCALING
#define AEGRIDDIMSCALING

namespace ae9ap9 
{
    class AEGridDimScaling
    {
    public:

        // constructors
        AEGridDimScaling();

        // destructor
        virtual ~AEGridDimScaling();

        // self copy
        virtual AEGridDimScaling* clone();

        virtual int physicalToGrid( double dPhysicalCoord, double& dGridCoord );
        virtual int gridToPhysical( double dGridCoord, double& dPhysicalCoord );

    protected:
    };

} /*namespace*/

#endif
