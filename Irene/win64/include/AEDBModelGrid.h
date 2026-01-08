/******************************************************************************
$HeadURL$

File: AEDBModelGrid.h

Description: Class declaration AEDBModelGrid class in the database layer of the
    Ae9Ap9Model. Clases in the database layer wrap the persistent representation
    and provide i/o for analogous classes in the model layer. Model grid data are
    persisted in an HDF5 format database. This software is thus dependent on
    the HDF5 interface library.

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
  1.0          10/02/2009 created 
  
SVNTag: $Id$
******************************************************************************/
#ifndef AEDBMODELGRID_H
#define AEDBMODELGRID_H

#include <vector>
#include <string>
using namespace std;

#include "AETypes.h"
#include "AEErrorHandler.h"
#include "AEDBModelGridDimension.h"
#include "AEDBLossCone.h"
#include "AEDBMaker.h"

namespace ae9ap9
{
    class AEDBModelGrid : public AEErrorHandler
    {
    public:
        AEDBModelGrid();
        ~AEDBModelGrid();

        int read( uint64_t iHandle, const string& strGridName );
        /*int write( uint64_t iHandle, const string& strGridName ); */

        // public accessors

        void getN( dvector& vdN, uivector& vuiNDims ) 
                 {vdN = m_vdN; vuiNDims = m_vuiNDims;}
        void setN( const dvector& vdN, const uivector& vuiNDims ) 
                 {m_vdN = vdN; m_vuiNDims = vuiNDims;}

        void getN23( dvector& vdN23, uivector& vuiN23Dims ) 
                   {vdN23 = m_vdN23; vuiN23Dims = m_vuiN23Dims;}
        void setN23( const dvector& vdN23, const uivector& vuiN23Dims ) 
                   {m_vdN23 = vdN23; m_vuiN23Dims = vuiN23Dims;}

        void getN23Full( dvector& vdN23Full, uivector& vuiN23FullDims ) 
                       {vdN23Full = m_vdN23Full; vuiN23FullDims = m_vuiN23FullDims;}
        void setN23Full( const dvector& vdN23Full, const uivector& vuiN23FullDims ) 
                       {m_vdN23Full = vdN23Full; m_vuiN23FullDims = vuiN23FullDims;}

        void getNActive( dvector& vdNActive, uivector& vuiNActiveDims ) 
                       {vdNActive = m_vdNActive; vuiNActiveDims = m_vuiNActiveDims;}
        void setNActive( const dvector& vdNActive, const uivector& vuiNActiveDims ) 
                       {m_vdNActive = vdNActive; m_vuiNActiveDims = vuiNActiveDims;}

        void getQ23Init( ucvector& vucQ23Init, uivector& vuiQ23InitDims ) 
                       {vucQ23Init = m_vucQ23Init; vuiQ23InitDims = m_vuiQ23InitDims;}
        void setQ23Init( const ucvector& vucQ23Init, const uivector& vuiQ23InitDims ) 
                       {m_vucQ23Init = vucQ23Init; m_vuiQ23InitDims = vuiQ23InitDims;}

        void getQ23Type( string& strQ23Type ) { strQ23Type = m_strQ23Type; }
        void setQ23Type( const string& strQ23Type ) { m_strQ23Type = strQ23Type; }

        void getX2X3Type( string& strX2X3Type ) { strX2X3Type = m_strX2X3Type; }
        void setX2X3Type( const string& strX2X3Type ) { m_strX2X3Type = strX2X3Type; }

        void getActiveToAll( dvector& vdActiveToAll, uivector& vuiActiveToAllDims ) 
                       {vdActiveToAll = m_vdActiveToAll; vuiActiveToAllDims = m_vuiActiveToAllDims;}
        void setActiveToAll( const dvector& vdActiveToAll, const uivector& vuiActiveToAllDims ) 
                       {m_vdActiveToAll = vdActiveToAll; m_vuiActiveToAllDims = vuiActiveToAllDims;}

        void getAllToActive( dvector& vdAllToActive, uivector& vuiAllToActiveDims ) 
                       {vdAllToActive = m_vdAllToActive; vuiAllToActiveDims = m_vuiAllToActiveDims;}
        void setAllToActive( const dvector& vdAllToActive, const uivector& vuiAllToActiveDims ) 
                       {m_vdAllToActive = vdAllToActive; m_vuiAllToActiveDims = vuiAllToActiveDims;}

        void getFullToReduced( dvector& vdFullToReduced, uivector& vuiFullToReducedDims ) 
                       {vdFullToReduced = m_vdFullToReduced; vuiFullToReducedDims = m_vuiFullToReducedDims;}
        void setFullToReduced( const dvector& vdFullToReduced, const uivector& vuiFullToReducedDims ) 
                       {m_vdFullToReduced = vdFullToReduced; m_vuiFullToReducedDims = vuiFullToReducedDims;}

        void getReducedToFull( dvector& vdReducedToFull, uivector& vuiReducedToFullDims ) 
                       {vdReducedToFull = m_vdReducedToFull; vuiReducedToFullDims = m_vuiReducedToFullDims;}
        void setReducedToFull( const dvector& vdReducedToFull, const uivector& vuiReducedToFullDims ) 
                       {m_vdReducedToFull = vdReducedToFull; m_vuiReducedToFullDims = vuiReducedToFullDims;}

        void getLossConeType( string& strLossConeType ) { strLossConeType = m_strLossConeType; }
        void setLossConeType( const string& strLossConeType ) { m_strLossConeType = strLossConeType; }

        void getSize23( dvector& vdSize23, uivector& vuiSize23Dims ) 
                       {vdSize23 = m_vdSize23; vuiSize23Dims = m_vuiSize23Dims;}
        void setSize23( const dvector& vdSize23, const uivector& vuiSize23Dims ) 
                       {m_vdSize23 = vdSize23; m_vuiSize23Dims = vuiSize23Dims;}

        void getSize3( dvector& vdSize3, uivector& vuiSize3Dims ) 
                       {vdSize3 = m_vdSize3; vuiSize3Dims = m_vuiSize3Dims;}
        void setSize3( const dvector& vdSize3, const uivector& vuiSize3Dims ) 
                       {m_vdSize3 = vdSize3; m_vuiSize3Dims = vuiSize3Dims;}

        // Don't allow sets of entire dimension (get it and operate on it, thus no sets or consts)
        AEDBModelGridDimension& getE()  { return m_E; }
        AEDBModelGridDimension& getQ2() { return m_Q2; }
        AEDBModelGridDimension& getQ3() { return m_Q3; }

        AEDBLossCone& getLossCone() { return m_lossCone; }
        AEDBMaker& getMakerX2X3() { return m_makerX2X3; }

        // Constant strings for db model grid type strings

        static const string AEDB_DIM_E;
        static const string AEDB_DIM_Q2;
        static const string AEDB_DIM_Q3;

        static const string AEDB_Q23TYPE_CART;
        static const string AEDB_X2X3TYPE_ALPHALM;
        static const string AEDB_X2X3TYPE_KPHI;
        static const string AEDB_X2X3TYPE_KHMIN;
        static const string AEDB_X2X3TYPE_MULTIGRID;

    protected:

        // Dimensions: E, Q2, Q3
        AEDBModelGridDimension m_E;
        AEDBModelGridDimension m_Q2;
        AEDBModelGridDimension m_Q3;

        // nested type objects
        AEDBLossCone m_lossCone;

        AEDBMaker m_makerX2X3;

        // persisted fields
        dvector  m_vdN;
        uivector m_vuiNDims;

        dvector  m_vdN23;
        uivector m_vuiN23Dims;

        dvector  m_vdN23Full;
        uivector m_vuiN23FullDims;

        dvector  m_vdNActive;
        uivector m_vuiNActiveDims;

        ucvector m_vucQ23Init;
        uivector m_vuiQ23InitDims;

        string   m_strQ23Type;

        string   m_strX2X3Type;

        dvector  m_vdActiveToAll;
        uivector m_vuiActiveToAllDims;

        dvector  m_vdAllToActive;
        uivector m_vuiAllToActiveDims;

        dvector  m_vdFullToReduced;
        uivector m_vuiFullToReducedDims;

        dvector  m_vdReducedToFull;
        uivector m_vuiReducedToFullDims;

        string   m_strLossConeType;

        dvector  m_vdSize23;
        uivector m_vuiSize23Dims;

        dvector  m_vdSize3;
        uivector m_vuiSize3Dims;
    };

};

#endif
