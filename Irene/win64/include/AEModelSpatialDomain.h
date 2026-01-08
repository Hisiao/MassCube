/******************************************************************************

File: AEModelSpatialDomain.h

Description: The Ae9Ap9 model utilizes spatial flux distributions represented
   using one or more spatial coordinate domains. That is, some regions of space
   may maintain flux using alpha, Lm coordinates and some across K, Phi or even
   K, HMin space. The AEModelSpatialDomain class wraps the collection of these
   spatial grids to hide the various coordinate systems from client classes
   when retrieving flux data.

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
  1.0          01/05/2011 Created
  
******************************************************************************/
#ifndef AEMODELSPATIALDOMAIN_H
#define AEMODELSPATIALDOMAIN_H

#include "CMagfield.h"
#include "AETypes.h"
#include "AEEnums.h"
#include "AEModelGrid.h"
#include "AEDBModelSpatialDomain.h"
#include "AESpatialCoordCalculator.h"

namespace ae9ap9 
{
    class AEModelSpatialDomain : public AEErrorHandler
    {
    public:
        AEModelSpatialDomain();
        ~AEModelSpatialDomain();

        int initialize( AEDBModelSpatialDomain& domainData,
                        const string& strKPhiDataSource,
                        const string& strKHMinDataSource );

        // Accessors for data members and sizes

        int getN()       { return m_iN; }
        int getNActive() { return m_iNActive; }

//        dvector& getActiveFilteredIndices() {return m_vdActiveToAll; }

        dvector& getAllIndices() { return m_vdAllToActive; }

        int getMaxN23(); // get the largest possible size of the Q2, Q3 dimension data

        int computeCoordinates( CMagfield&      magfieldModel,
                                emfCoordSys  eCoordSys,
                                const dvector&  vdDateTimes,
                                const dvector&  vdCoordsAxis1,
                                const dvector&  vdCoordsAxis2, 
                                const dvector&  vdCoordsAxis3,
                                const bdmatrix& bmdPitchAngles,
                                bdmatrix&       bmdX2,
                                bdmatrix&       bmdX3 );

        // Retrieve the flux grid used to compute the spatial coordinates for
        // the point at time iTimeIndex and direction iDirIndex.

        AEModelGrid* getFluxGrid( int iTimeIndex, int iDirIndex );

        // Retrieve the ith spatial grid in this spatial domain 0..n-1

        int getNumFluxGrids() { return int(m_vpGrids.size()); }
        AEModelGrid* getFluxGrid( int iGridIndex = 0 );

    protected:

        // internal helper methods

        int createGridCoordCalc( const string& strCoordDomain, AESpatialCoordCalculator** ppCoordCalc );

        int appendPitchAngle90( const bdmatrix& bmdPitchAngles, bdmatrix& bmdPAsWith90 );

        int computeMagneticFieldData( CMagfield& magfieldModel, 
                                      emfCoordSys  eCoordSys,
                                      const dvector&  vdDateTimes,
                                      const dvector&  vdCoordsAxis1,
                                      const dvector&  vdCoordsAxis2, 
                                      const dvector&  vdCoordsAxis3,
                                      const bdmatrix& bmdPitchAngles,
                                      bdmatrix& bmdB,
                                      dvector&  vdBmin,
                                      dvector&  vdBlocal,
                                      bdmatrix& bmdI,
                                      bdmatrix& bmdBmirror,
                                      bdmatrix& bmdLm );
                                      
        int localComputeAdInvariants( CMagfield& magfieldModel, double dDate, S3Tuple& pos, dvector& vdPitchAngles,
                                      double& dBMin, dvector& vdB, dvector& vdLm, dvector& vdI );

        void quickSortDesc( dvector& vdPitchAngles, ivector& viPAIndices );

        bool isShabanskyOrbit( double dDateTime, 
                               double dBMinXGEO, 
                               double dBMinYGeo, 
                               double dBMinZGeo );

        // protected data members

        int      m_iN;
        int      m_iNActive;
        string   m_strX2X3Type;
//        dvector  m_vdActiveToAll;
        dvector  m_vdAllToActive;
        string   m_strKPhiDataSource;
        string   m_strKHMinDataSource;

        // 1 spatial grid per domain, in order of precedence

        vector<AEModelGrid*>              m_vpGrids;       

        // 1 coord calculater per domain, order corresponds to grids

        vector<AESpatialCoordCalculator*> m_vpCoordCalcs;  

        // maps grid used at each spatial location by computeCoordinates
        // use time and direction indices as surrogate for location in calls to getFluxGrid
        // resets at each call to computeCoordinates

        boost::numeric::ublas::matrix<short> m_bmsGeoCoordsToGridIndex; 

        // Need pitch angles sorted for computeAdInvariants

        dvector m_vdSortedPitchAngles;

        // saved coordinate calculations, when requested
        bdmatrix m_bmdPitchAngles;
        bdmatrix m_bmdAlpha;
        bdmatrix m_bmdLm; 
        bdmatrix m_bmdK; 
        bdmatrix m_bmdPhi;
        bdmatrix m_bmdHmin; 
        dvector  m_vdBmin;
        dvector  m_vdBlocal;
        dvector  m_vdMagLT;

     private:
        // prevent auto-creation, no reason for them to exist
        AEModelSpatialDomain( const AEModelSpatialDomain& source ); 
        AEModelSpatialDomain& operator =( const AEModelSpatialDomain& source ); 
    };

} // namespace

#endif
