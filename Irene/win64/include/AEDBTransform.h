/******************************************************************************
$HeadURL$

File: AEDBTransform.h

Description: Class declaration for the AEDBTransform class in the database 
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
#ifndef AEDBTRANSFORM_H
#define AEDBTRANSFORM_H

#include <vector>
#include <string>
using namespace std;

#include "AETypes.h"

namespace ae9ap9
{
    class AEDBTransform
    {
    public:
        AEDBTransform();
        ~AEDBTransform();

        int read( uint64_t iHandle, const string& strObjName );
        /*int write( uint64_t iHandle, const string& strObjName );*/

        // public accessors

        void getType( string& strType ) { strType = m_strType; }
        void setType( const string& strType ) { m_strType = strType; }

        bool foundPower() { return m_bPowerRead; }
        void getPower( dvector& vdPower, uivector& vuiPowerDims ) 
                 {vdPower = m_vdPower; vuiPowerDims = m_vuiPowerDims;}
        void setPower( const dvector& vdPower, const uivector& vuiPowerDims ) 
                 {m_vdPower = vdPower; m_vuiPowerDims = vuiPowerDims;}

    protected:
        string   m_strType;

        bool     m_bPowerRead;    // optional field
        dvector  m_vdPower;
        uivector m_vuiPowerDims;
    };

};

#endif
