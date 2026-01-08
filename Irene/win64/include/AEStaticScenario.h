/******************************************************************************
$HeadURL$

File: AEStaticScenario.h

Description: This header contains the declaration for the AEStaticScenario class
This classes implements statistical variations of flux paramaters for a single
snapshot in time.

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
  1.0          10/23/2009 created
  
SVNTag: $Id$
******************************************************************************/
#ifndef AESTATICSCENARIO_H
#define AESTATICSCENARIO_H

#include "AEScenario.h"

namespace ae9ap9
{
    class AEStaticScenario : public AEScenario
    {
    public:
        AEStaticScenario();
        ~AEStaticScenario();

        int initialize( const string& strMarginalType,
                        const bdvector& bvdLnMedianFlux, 
                        const bdvector& bvdLn95Minus50PctFlux,
                        double dPercentile = 0 );

        int advance( double dTargetTime );

        int computeFlux( bbcvector& bvbPtsToCompute );

    protected:

        bool   m_bMeanFlux;
        double m_dPercentileFlux;
    };
}

#endif
