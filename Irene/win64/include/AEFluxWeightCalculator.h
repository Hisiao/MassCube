/******************************************************************************
$HeadURL$

File: AEFluxWeightCalculator.h

Description: Declaration of the AEFluxWeightCalculator class. This class
contains all of the calculation logic for computing flux weights for given
dates, positions, energy levels and grid data for a given model.

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
  1.0          10/28/2009 created
  
SVNTag: $Id$
******************************************************************************/
#ifndef AEFLUXWEIGHTCALCULATOR_H
#define AEFLUXWEIGHTCALCULATOR_H

#include "CMagfield.h"
#include "AETypes.h"
#include "AEEnums.h"
#include "AEErrorHandler.h"
#include "AEModelSpatialDomain.h"
#include "AEModelGrid.h"


namespace ae9ap9
{
    class AEFluxWeightCalculator : public AEErrorHandler
    {
    public:
        AEFluxWeightCalculator();
        ~AEFluxWeightCalculator();

        int initialize( const string& strMagfieldModelDBPath );

        int computeFluxWeights( AEModelSpatialDomain& spatialDomain,
                                eAEEnergyFluxType     eFluxType,
                                const bdmatrix&       bmdEnergies,
                                const dvector&        vdTimes, 
                                emfCoordSys           eCoordSys,
                                const dvector&        vdCoordsAxis1,
                                const dvector&        vdCoordsAxis2, 
                                const dvector&        vdCoordsAxis3,
                                const dvector&        vdPitchAngles,
                                eAEPreCompute         ePreCompute,
                                const bool&           bOmniDirectional = false );

        int computeFluxWeights( AEModelSpatialDomain& spatialDomain,
                                eAEEnergyFluxType     eFluxType,
                                const bdmatrix&       bmdEnergies,
                                const dvector&        vdTimes, 
                                emfCoordSys           eCoordSys,
                                const dvector&        vdCoordsAxis1,
                                const dvector&        vdCoordsAxis2, 
                                const dvector&        vdCoordsAxis3,
                                const bdmatrix&       bmdPitchAngles, 
                                eAEPreCompute         ePreCompute );

        // Note: this accessor assumes that all flux weights have been pre-computed
        //       and that iTime, iEnergy, iDirection are indices into the time, energy
        //       and direction/pitch angle collections passed during that pre-compute.

        int getH( int iTime, int iEnergy, int iDirection, dvector& vdH, bbcvector& vbHInUse );

        // [tbd] accessors for h that don't assume pre-compute

        int calcDirPitchAngles ( const emfCoordSys eCoordSys,
                                 const dvector& vdDates,  
                                 const dvector& vdX,      
                                 const dvector& vdY,       
                                 const dvector& vdZ,      
                                 const vdvector& vvdDirX, 
                                 const vdvector& vvdDirY, 
                                 const vdvector& vvdDirZ, 
                                 vdvector& vvdPitchAngles );

    protected:

        // internal helper methods

        int make_hE( AEModelSpatialDomain& spatialDomain, eAEEnergyFluxType eFluxType, 
                     dvector& vdEnergies, bdcmatrix& bcmdHE );

        int make_h23( AEModelSpatialDomain& spatialDomain, const dvector& vdDateTimes, 
                      int iDateTimeIndex, int iDirectionIndex, bool bOmniDirectional, 
                      const dvector& vdPitchAngles, bdmatrix& bmdX2, bdmatrix& bmdX3, 
                      bdcmatrix& bcmdH23 );

        int make_h( AEModelSpatialDomain& spatialDomain, eAEEnergyFluxType eFluxType, 
                    const dvector& vdDateTimes, int iDateTimeIndex, 
                    const bdmatrix& bmdEnergies, int iEnergyIndex, int iDirIndex, 
                    bool bOmniDirectional, const bdmatrix& bmdPitchAngles, 
                    bdmatrix& bmdX2, bdmatrix& bmdX3, bdcmatrix& bcmdH );

        int make_hAll( AEModelSpatialDomain& spatialDomain, eAEEnergyFluxType eFluxType, 
                       const dvector& vdDateTimes, const bdmatrix& bmdPitchAngles, 
                       const bdmatrix& bmdEnergies, bool bOmniDirectional, 
                       bdmatrix& bmdX2, bdmatrix& bmdX3, size_t nDirections );

        int make_EDifferentialWeights( AEModelSpatialDomain& spatialDomain, 
                                       const dvector& vdEnergies, bdcmatrix& bcmdHE );

        int make_EIntegralWeights( AEModelSpatialDomain& spatialDomain, 
                                   const dvector& vdEnergies, bdcmatrix& bcmdHE );

        int make_Q2Q3Weights( AEModelSpatialDomain& spatialDomain, int iDateTimeIndex, 
                              int iDirectionIndex, const dvector& vdQ2, 
                              const dvector& vdQ3, bdcmatrix& bcmdH23 );

        int make_Q2Q3HOmni( AEModelSpatialDomain& spatialDomain, int iDateTimeIndex, 
                            int iDirectionIndex, const dvector& vdQ2, 
                            const dvector& vdQ3, const dvector& vdPitchAngles, 
                            bdcmatrix& bcmdHOmni);

        int combine_hEh23( AEModelSpatialDomain& spatialDomain, bdcmatrix& bcmdHE, 
                           bdcmatrix& bcmdH23, bdcmatrix& bcmdH );

        // data members

        unique_ptr<CMagfield>              m_pMagfieldModel; // magnetic field model engine

        bdmatrix                         m_bmdX2;          // computed
        bdmatrix                         m_bmdX3;          // 

        bdcmatrix                        m_bcmdHE;         // nEnergies x nGridEnergies
        vbdcmatrix                       m_vbcmdH23;       // nTimes x {nDirections x nActiveGridPts}
        vector<vbdcmatrix>               m_vvbcmdH;        // nTimes x {nEnergies x {nDirections x nActiveGridPts}}

    };
}

#endif
