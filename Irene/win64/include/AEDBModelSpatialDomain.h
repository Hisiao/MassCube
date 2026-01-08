/******************************************************************************
$HeadURL$

File: AEDBModelSpatialDomain.h

Description: Class declaration AEDBModelSpatialDomain class in the database layer 
    of the Ae9Ap9Model. Classes in the database layer wrap the persistent representation
    and provide i/o for analogous classes in the model layer. Model spatial domain data
    are persisted in an HDF5 format database. This software is thus dependent on
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
  1.0          1/05/2011 created 
  
SVNTag: $Id$
******************************************************************************/
#ifndef AEDBMODELSPATIALDOMAIN_H
#define AEDBMODELSPATIALDOMAIN_H

#include <vector>
#include <string>
using namespace std;

#include "AETypes.h"
#include "AEErrorHandler.h"
#include "AEDBModelGrid.h"

namespace ae9ap9
{
    class AEDBModelSpatialDomain : public AEErrorHandler
    {
    public:
        AEDBModelSpatialDomain();
        ~AEDBModelSpatialDomain();

        int read( uint64_t iHandle, const string& strDomainName );
        /*int write( uint64_t iHandle, const string& strDomainName ); */

        // public accessors

        void getN( dvector& vdN, uivector& vuiNDims ) 
                 {vdN = m_vdN; vuiNDims = m_vuiNDims;}
        void setN( const dvector& vdN, const uivector& vuiNDims ) 
                 {m_vdN = vdN; m_vuiNDims = vuiNDims;}

        void getNActive( dvector& vdNActive, uivector& vuiNActiveDims ) 
                       {vdNActive = m_vdNActive; vuiNActiveDims = m_vuiNActiveDims;}
        void setNActive( const dvector& vdNActive, const uivector& vuiNActiveDims ) 
                       {m_vdNActive = vdNActive; m_vuiNActiveDims = vuiNActiveDims;}

        void getX2X3Type( string& strX2X3Type ) { strX2X3Type = m_strX2X3Type; }
        void setX2X3Type( const string& strX2X3Type ) { m_strX2X3Type = strX2X3Type; }

//        void getActiveToAll( dvector& vdActiveToAll, uivector& vuiActiveToAllDims ) 
//                       {vdActiveToAll = m_vdActiveToAll; vuiActiveToAllDims = m_vuiActiveToAllDims;}
//        void setActiveToAll( const dvector& vdActiveToAll, const uivector& vuiActiveToAllDims ) 
//                       {m_vdActiveToAll = vdActiveToAll; m_vuiActiveToAllDims = vuiActiveToAllDims;}

        void getAllToActive( dvector& vdAllToActive, uivector& vuiAllToActiveDims ) 
                       {vdAllToActive = m_vdAllToActive; vuiAllToActiveDims = m_vuiAllToActiveDims;}
        void setAllToActive( const dvector& vdAllToActive, const uivector& vuiAllToActiveDims ) 
                       {m_vdAllToActive = vdAllToActive; m_vuiAllToActiveDims = vuiAllToActiveDims;}

        short getNumGrids() { return ((short) m_vGrids.size()); }
        int getGrid( short sGrid, AEDBModelGrid** ppGrid );

        void  addGrid( AEDBModelGrid& grid );
        void  clearGrids()  { m_vGrids.clear(); }

    protected:

        // persisted fields
        dvector  m_vdN;
        uivector m_vuiNDims;

        dvector  m_vdNActive;
        uivector m_vuiNActiveDims;

        string   m_strX2X3Type;

//        dvector  m_vdActiveToAll;
//        uivector m_vuiActiveToAllDims;

        dvector  m_vdAllToActive;
        uivector m_vuiAllToActiveDims;

        vector<AEDBModelGrid> m_vGrids;
    };

};

#endif
