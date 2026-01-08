/******************************************************************************
$HeadURL$

File: AEPerturbedScenario.h

Description: This header contains the declaration for the AEPerturbedScenario class
This classes implements statistical variations of flux paramaters over a set of 
time invariant values. It also serves as the base class for AEDynamicScenario, whose
values vary with both scenario and time.

 Classification:

   Unclassified

 Project Name:

   AE9/AP9/SPM Radiation Environment Models

   Developed under US Government contract # FA9453-12-C-0231

 Rights and Restrictions:

   Copyright 2015 Atmospheric and Environmental Research, Inc. (AER)

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
#ifndef AEPERTURBEDSCENARIO_H
#define AEPERTURBEDSCENARIO_H

#include "AEScenario.h"
#include "AERandomNumberGenerator.h"

namespace ae9ap9
{
    class AEPerturbedScenario : public AEScenario
    {
    public:
        AEPerturbedScenario();
        ~AEPerturbedScenario();

        int initialize( const string&    strMarginalType,
                        const bdvector&  bvdLnMedianFlux, 
                        const bdvector&  bvdLn95Minus50PctFlux,
                        bcmdmatrix&      bmdSThetaTranspose,
                        int              iScenarioNumber,
                        bool             bPerturbUsingUniformDist,
                        bool             bPerturbFluxMap = true );

        virtual int advance( double dTargetTime );

        virtual int computeFlux( bbcvector& bvbPtsToCompute );

    protected:

        // protected data members

        int       m_iScenarioNumber;

        // Random number generators for monte carlo perturbations
        unique_ptr<AERandomNumberGenerator> m_randEpsilon;
    };
}

#endif
