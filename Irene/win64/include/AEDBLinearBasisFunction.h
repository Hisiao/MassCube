/******************************************************************************
$HeadURL$

File: AEDBLinearBasisFunction.h

Description: Class declaration for the AEDBLinearBasisFunction class in the database 
    layer of the Ae9Ap9Model. Clases in the database layer wrap the persistent 
    representation and provide i/o for analogous classes in the model layer. Model 
    grid data are persisted in an HDF5 format database. This software is thus dependent 
    on the HDF5 interface library.

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
  1.0          10/02/2009 
  
SVNTag: $Id$
******************************************************************************/
#ifndef AEDBLINEARBASISFUNCTION_H
#define AEDBLINEARBASISFUNCTION_H

#include <vector>
#include <string>
using namespace std;

#include "AETypes.h"

namespace ae9ap9 
{
    class AEDBLinearBasisFunction
    {
    public:
        AEDBLinearBasisFunction();
        ~AEDBLinearBasisFunction();

        int read( uint64_t iHandle, const string& strLBFName );
        /*int write( uint64_t iHandle, const string& strLBFName );*/

        // public accessors

        void getN( dvector& vdN, uivector& vuiNDims ) 
                       {vdN = m_vdN; vuiNDims = m_vuiNDims;}
        void setN( const dvector& vdN, const uivector& vuiNDims ) 
                       {m_vdN = vdN; m_vuiNDims = vuiNDims;}

        void getX( dvector& vdX, uivector& vuiXDims ) 
                       {vdX = m_vdX; vuiXDims = m_vuiXDims;}
        void setX( const dvector& vdX, const uivector& vuiXDims ) 
                       {m_vdX = vdX; m_vuiXDims = vuiXDims;}

        void getHigh( dvector& vdHigh, uivector& vuiHighDims ) 
                       {vdHigh = m_vdHigh; vuiHighDims = m_vuiHighDims;}
        void setHigh( const dvector& vdHigh, const uivector& vuiHighDims ) 
                       {m_vdHigh = vdHigh; m_vuiHighDims = vuiHighDims;}

        void getLow( dvector& vdLow, uivector& vuiLowDims ) 
                       {vdLow = m_vdLow; vuiLowDims = m_vuiLowDims;}
        void setLow( const dvector& vdLow, const uivector& vuiLowDims ) 
                       {m_vdLow = vdLow; m_vuiLowDims = vuiLowDims;}

        void getMid( dvector& vdMid, uivector& vuiMidDims ) 
                       {vdMid = m_vdMid; vuiMidDims = m_vuiMidDims;}
        void setMid( const dvector& vdMid, const uivector& vuiMidDims ) 
                       {m_vdMid = vdMid; m_vuiMidDims = vuiMidDims;}

    protected:

        // data members

        // persisted fields
        dvector  m_vdN;
        uivector m_vuiNDims;

        dvector  m_vdX;
        uivector m_vuiXDims;

        dvector  m_vdHigh;
        uivector m_vuiHighDims;

        dvector  m_vdLow;
        uivector m_vuiLowDims;

        dvector  m_vdMid;
        uivector m_vuiMidDims;

    };

};

#endif
