/******************************************************************************
$HeadURL$

File: AECartesianFilter.h

Description: AECartesianFilter is a base class for classes that define specific
    methods of filtering the coordinates of an AEGrid object. (see
    Ae8LossConeFilter.h and OpqMaxKLossConeFilter.h)

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
#ifndef AECARTESIANFILTER
#define AECARTESIANFILTER

#include "AETypes.h"

namespace ae9ap9 
{
    class AECartesianFilter
    {
    public:
        AECartesianFilter();
        virtual ~AECartesianFilter();

        virtual AECartesianFilter* clone() const = 0;

        // Template for derived class' filtering method
        virtual int filter( const dvector& vdDim1Coords, 
                            const dvector& vdDim2Coords,
                            dvector& vdFilteredGridIndices) = 0;

    protected:
    };

} /*namespace*/

#endif
