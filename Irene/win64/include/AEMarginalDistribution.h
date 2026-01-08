/******************************************************************************
$HeadURL$

File: AEMarginalDistribution.h

Description: Class declaration for the AEMarginalDistribution class
This base class defines the interface for classes that calculate
flux statistics using various statistical distributions.

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
  1.0          10/20/2009 created
  
SVNTag: $Id$
******************************************************************************/
#ifndef AEMARGINALDISTRIBUTION
#define AEMARGINALDISTRIBUTION

#include "AETypes.h"
#include "AEErrorHandler.h"

namespace ae9ap9
{
    class AEMarginalDistribution : public AEErrorHandler
    {
    public:
        AEMarginalDistribution(){}
        virtual ~AEMarginalDistribution(){}

        virtual int setParameters( const bdvector& bvdLnMedianFlux, 
                                   const bdvector& bvdLn95Minus50PctFlux ) = 0;

        //virtual int ZToFlux( const bdvector& bvdZ, bdvector& bvdFlux ) = 0;
        virtual int ZToFlux( const bdcvector& bvdZ, const bbcvector& bvbPtsToCompute, bdcvector& bvdFlux ) = 0;

        virtual int meanFlux( bdvector& bvdFlux ) = 0;

        virtual int percentileFlux( double dPercentile, bdvector& bvdFlux ) = 0;

    protected:
    };
}

#endif
