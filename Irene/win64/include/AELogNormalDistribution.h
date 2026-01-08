/******************************************************************************
$HeadURL$

File: AELogNormalDistribution.h

Description: Class declaration for the AELogNormalDistribution class
This class implements the calculations of various flux statistics using
a LogNormal statistical distribution.

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
#ifndef AELOGNORMALDISTRIBUTION_H
#define AELOGNORMALDISTRIBUTION_H

#include "AETypes.h"
#include "AEMarginalDistribution.h"

namespace ae9ap9
{
    class AELogNormalDistribution : public AEMarginalDistribution
    {
    public:
        AELogNormalDistribution();
        ~AELogNormalDistribution();

        // inherited virtual functions

        int setParameters( const bdvector& bvdLnMedianFlux, 
                           const bdvector& bvdLn95Minus50PctFlux );

        int ZToFlux( const bdcvector& bvdZ, const bbcvector& bvbPtsToCompute, bdcvector& bvdFlux );

        int meanFlux( bdvector& bvdFlux );

        int percentileFlux( double dPercentile, bdvector& bvdFlux );

    protected:

        bdvector m_bvdLnMedianFlux;
        bdvector m_bvdLn95thPctFlux;
        bdvector m_bvdStdDev;
    };
}

#endif
