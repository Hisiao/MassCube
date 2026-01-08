/******************************************************************************
$HeadURL$

File: AEDBMakerNet.h

Description: Class declaration for the AEDBMakerNet class in the database 
    layer of the Ae9Ap9Model. A number of objects within the persisted AE9AP9
    model hierarchy fit the pattern of an object with a single type identifier.
    Each is represented in memory by this class.
    Classes in the database layer wrap the persistent representation and provide 
    i/o for analogous classes in the model layer. Model grid data are persisted 
    in an HDF5 format database. This software is thus dependent on the HDF5
    interface library.

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
  1.0          10/06/2009 created
  
SVNTag: $Id$
******************************************************************************/
#ifndef AEDBMAKERNET_H
#define AEDBMAKERNET_H

#include <vector>
#include <string>
using namespace std;

#include "AETypes.h"

namespace ae9ap9
{
    class AEDBMakerNet
    {
    public:
        AEDBMakerNet();
        ~AEDBMakerNet();

        int read( uint64_t iHandle, const string& strObjName );
        /*int write( uint64_t iHandle, const string& strObjName );*/

        // public accessors

        void getNh( usvector& vusNh, uivector& vuiNhDims ) 
                       {vusNh = m_vusNh; vuiNhDims = m_vuiNhDims;}
        void setNh( const usvector& vusNh, const uivector& vuiNhDims ) 
                       {m_vusNh = vusNh; m_vuiNhDims = vuiNhDims;}

        void getNx( usvector& vusNx, uivector& vuiNxDims ) 
                       {vusNx = m_vusNx; vuiNxDims = m_vuiNxDims;}
        void setNx( const usvector& vusNx, const uivector& vuiNxDims ) 
                       {m_vusNx = vusNx; m_vuiNxDims = vuiNxDims;}

        void getNy( usvector& vusNy, uivector& vuiNyDims ) 
                       {vusNy = m_vusNy; vuiNyDims = m_vuiNyDims;}
        void setNy( const usvector& vusNy, const uivector& vuiNyDims ) 
                       {m_vusNy = vusNy; m_vuiNyDims = vuiNyDims;}

        void getSx( dvector& vdSx, uivector& vuiSxDims ) 
                       {vdSx = m_vdSx; vuiSxDims = m_vuiSxDims;}
        void setSx( const dvector& vdSx, const uivector& vuiSxDims ) 
                       {m_vdSx = vdSx; m_vuiSxDims = vuiSxDims;}

        void getSy( dvector& vdSy, uivector& vuiSyDims ) 
                       {vdSy = m_vdSy; vuiSyDims = m_vuiSyDims;}
        void setSy( const dvector& vdSy, const uivector& vuiSyDims ) 
                       {m_vdSy = vdSy; m_vuiSyDims = vuiSyDims;}

        void getTheta( dvector& vdTheta, uivector& vuiThetaDims ) 
                       {vdTheta = m_vdTheta; vuiThetaDims = m_vuiThetaDims;}
        void setTheta( const dvector& vdTheta, const uivector& vuiThetaDims ) 
                       {m_vdTheta = vdTheta; m_vuiThetaDims = vuiThetaDims;}

        void getXBar( dvector& vdXBar, uivector& vuiXBarDims ) 
                       {vdXBar = m_vdXBar; vuiXBarDims = m_vuiXBarDims;}
        void setXBar( const dvector& vdXBar, const uivector& vuiXBarDims ) 
                       {m_vdXBar = vdXBar; m_vuiXBarDims = vuiXBarDims;}

        void getYBar( dvector& vdYBar, uivector& vuiYBarDims ) 
                       {vdYBar = m_vdYBar; vuiYBarDims = m_vuiYBarDims;}
        void setYBar( const dvector& vdYBar, const uivector& vuiYBarDims ) 
                       {m_vdYBar = vdYBar; m_vuiYBarDims = vuiYBarDims;}

    protected:
        usvector m_vusNh;
        uivector m_vuiNhDims;

        usvector m_vusNx;
        uivector m_vuiNxDims;

        usvector m_vusNy;
        uivector m_vuiNyDims;

        dvector  m_vdSx;
        uivector m_vuiSxDims;

        dvector  m_vdSy;
        uivector m_vuiSyDims;

        dvector  m_vdTheta;
        uivector m_vuiThetaDims;

        dvector  m_vdXBar;
        uivector m_vuiXBarDims;

        dvector  m_vdYBar;
        uivector m_vuiYBarDims;

    };

};

#endif
