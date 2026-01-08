/******************************************************************************
$HeadURL$

File: AEScenario.h

Description: This header contains the declaration for the AEScenario class,
which serves as the base class for the AEStaticScenario and AEDynamicScenario
classes. These classes implement monte carlo simulations of flux calculations
using statistical distributions specific to the model loaded.

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
#ifndef AESCENARIO_H
#define AESCENARIO_H

#include <memory>
using namespace std;

#include "AEErrors.h"
#include "AEErrorHandler.h"
#include "AEMarginalDistribution.h"

namespace ae9ap9
{
    class AEScenario : public AEErrorHandler
    {
    public:
        AEScenario();
        virtual ~AEScenario();

        int initialize( const string& strMarginalType,
                        const bdvector& bvdLnMedianFlux, 
                        const bdvector& bvdLn95Minus50PctFlux );

        virtual int advance( double dTargetTime ) = 0;

        virtual int computeFlux( bbcvector& bvbPtsToCompute ) = 0;

        int getFlux( bdvector& bvdFlux) { bvdFlux = m_bvdFlux; return (AE_ERR_NOERROR); }

    protected:

        unique_ptr<AEMarginalDistribution> m_pDistribution;

        double   m_dTime;
        bdvector m_bvdFlux;
    };
}

#endif
