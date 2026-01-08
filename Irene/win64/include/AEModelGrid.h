/******************************************************************************
$HeadURL$

File: AEModelGrid.h

Description: The AEModelGrid class encapsulates the functionality of the
   AE-9/AP-9 grid object for the TEM and TPM models. The initial settings
   are read from the model repository and the implemented through the
   generic AEGrid class hierarchy.

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
  1.0          09/03/2009 Created
  
SVNTag: $Id$
******************************************************************************/
#ifndef AEMODELGRID_H
#define AEMODELGRID_H

#include "AETypes.h"
#include "AEGrid.h"
#include "AEDBModelGrid.h"
#include "AECartesianFilter.h"
#include "AELinearBasisFunction.h"

namespace ae9ap9 
{
    class AEModelGrid : public AEGrid
    {
    public:
        AEModelGrid();
        ~AEModelGrid();

        int initialize( AEDBModelGrid& gridData );

        int filterUsingLossCone();

        int recomputeEvalFunctions();

        // Accessors for data members and sizes

        int getN()       { return m_iN; }
        int getN23()     { return m_iN23; }
        int getN23Full() { return m_iN23Full; }
        int getNActive() { return m_iNActive; }
        string& getX2X3Type() { return m_strX2X3Type; }

        // loss cone filter
        const AECartesianFilter& getLossConeFilter() { return *m_pLossConeFilter; }
        void setLossConeFilter( const AECartesianFilter& filter );

        // filtered cartesian grid
        const dvector& getLossConeFilteredIndices() { return m_vdLossConeFilteredIndices; }
        void setLossConeFilteredIndices( const dvector& vdIndices ) 
             { m_vdLossConeFilteredIndices = vdIndices; }
        const dvector& getLossConeReverseIndices() { return m_vdLossConeReverseIndices; }
        void setLossConeReverseIndices( const dvector& vdIndices ) 
             { m_vdLossConeReverseIndices = vdIndices; }

        // evaluation (linear basis) functions

        AELinearBasisFunction& getEvalFunctionsE() { return m_lbfE; }
        void setEvalFunctionsE( const AELinearBasisFunction& lbfE ) { m_lbfE = lbfE; }

        AELinearBasisFunction& getEvalFunctionsQ2() { return m_lbfQ2; }
        void setEvalFunctionsQ2( const AELinearBasisFunction& lbfQ2 ) { m_lbfQ2 = lbfQ2; }

        AELinearBasisFunction& getEvalFunctionsQ3() { return m_lbfQ3; }
        void setEvalFunctionsQ3( const AELinearBasisFunction& lbfQ3 ) { m_lbfQ3 = lbfQ3; }

        // activity filter
        const dvector& getActiveFilteredIndices() { return m_vdActiveFilteredIndices; }
        void setActiveFilteredIndices( const dvector& vdIndices ) 
             { m_vdActiveFilteredIndices = vdIndices; }
        const dvector& getActiveReverseIndices() { return m_vdActiveReverseIndices; }
        void setActiveReverseIndices( const dvector& vdIndices ) 
             { m_vdActiveReverseIndices = vdIndices; }


    protected:

        // internal helper methods

        int initializeDim( AEDBModelGridDimension& dimData, const string& strDimName );
        int initializeLbf( AEDBLinearBasisFunction& dbLbf, AELinearBasisFunction& lbf );

        // Data elements

        int      m_iN;          // E.N * N23 reduced
        int      m_iN23;        // N23 reduced
        int      m_iN23Full;    // N23 full
        int      m_iNActive;    // filtered 
        string   m_strX2X3Type; // spatial grid coordinate domain


        // loss cone filter
        AECartesianFilter* m_pLossConeFilter;

        // filtered cartesian grid
        dvector m_vdLossConeFilteredIndices;
        dvector m_vdLossConeReverseIndices;

        // evaluation (linear basis) functions

        AELinearBasisFunction m_lbfE;
        AELinearBasisFunction m_lbfQ2;
        AELinearBasisFunction m_lbfQ3;

        // [tbd] activity filter
        /*AECartesianFilter* m_pActivityFilter;*/ // [tbd]
        dvector m_vdActiveFilteredIndices;
        dvector m_vdActiveReverseIndices;

    private:
        // prevent auto-creation, no reason for them to exist
        AEModelGrid( const AEModelGrid& source ); 
        AEModelGrid& operator =( const AEModelGrid& source ); 
    };

} // namespace

#endif
