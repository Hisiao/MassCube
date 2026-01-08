/******************************************************************************
$HeadURL$

File: AEDynamicScenario.h

Description: This header contains the declaration for the AEDynamicScenario class
This classes implements statistical variations of flux paramaters over a set of 
time varying values

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
#ifndef AEDYNAMICSCENARIO_H
#define AEDYNAMICSCENARIO_H

#include "AEPerturbedScenario.h"
#include "AERandomNumberGenerator.h"

namespace ae9ap9
{
    class AEDynamicScenario : public AEPerturbedScenario
    {
    public:
        AEDynamicScenario();
        ~AEDynamicScenario();

        int initialize( const string&    strMarginalType,
                        const bdvector&  bvdLnMedianFlux, 
                        const bdvector&  bvdLn95Minus50PctFlux,
                        const bdmatrix&  bmdQ,
                        const vbdmatrix& vbmdG,
                        const bdmatrix&  bmdC,
                        bcmdmatrix&      bmdSThetaTranspose,
                        bcmdmatrix&      bmdWTranspose,
                        dvector&         vdTimeLags,
                        double           dConditioningTime,
                        int              iScenarioNumber,
                        double           dModelEpoch,
                        double           dDeltaTime,
                        bool             bPerturbUsingUniformDist,
                        bool             bPerturbFluxMap = true );

        int advance( double dTargetTime );

        int computeFlux( bbcvector& bvbPtsToCompute );

    protected:

        void q_limit( bdvector& q );

        // protected data members

        bdmatrix  m_bmdQ;
        vbdmatrix m_vbmdG;
        bdmatrix  m_bmdC;
        double    m_dDeltaTime;
        dvector   m_vdTimeLags;
        double    m_dConditioningTime;

        // Next timestep time and randomly perturbed flux
        double    m_dNextTime;
        bdvector  m_bvdNext_q;

        // Also track last timestep
        double    m_dLastTime;
        bdvector  m_bvdLast_q;

        // Maintain a current q (b/t last and next)
        bdvector  m_bvd_q;

        // To support multiple time scales and pre-conditioning,
        // a set of history data must now be maintained.

        int       m_Hist_iQIndex;    // index of which set of q (principal components) used for next_q
        int       m_Hist_iNHist;     // number of timesteps N to go back
        vbdvector m_Hist_vbvdNext_q; // historical set of q (principal components) for each time scale

        vector<bool> m_vbFluxIsSet;

        // Random number generators for monte carlo perturbations
        unique_ptr<AERandomNumberGenerator> m_randDelta;

        // 2nd Random number generators for time variant monte carlo processing
        // 1st is in AEPerturbedScenario base class (m_randDelta)
        unique_ptr<AERandomNumberGenerator> m_randEta;   // even #s

    };
}

#endif
