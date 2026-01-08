/******************************************************************************
$HeadURL$

File: AEDBModelGridDimension.h

Description: Class declaration for the AEDBModelGridDimension class in the database 
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
  1.0          10/02/2009 Created
  
SVNTag: $Id$
******************************************************************************/
#ifndef AEDBMODELGRIDDIMENSION_H
#define AEDBMODELGRIDDIMENSION_H

#include <string>
using namespace std;

#include "AETypes.h"
#include "AEErrorHandler.h"
#include "AEDBLinearBasisFunction.h"
#include "AEDBTransform.h"

namespace ae9ap9
{
    class AEDBModelGridDimension : public AEErrorHandler
    {
    public:
        AEDBModelGridDimension();
        ~AEDBModelGridDimension();

        int read( uint64_t iHandle, const string& strDimName );
        /*int write( uint64_t iHandle, const string& strDimName ); */

        // public accessors

        void getN1( dvector& vdN1, uivector& vuiN1Dims ) 
                 {vdN1 = m_vdN1; vuiN1Dims = m_vuiN1Dims;}
        void setN1( const dvector& vdN1, const uivector& vuiN1Dims ) 
                 {m_vdN1 = vdN1; m_vuiN1Dims = vuiN1Dims;}

        void getQ( dvector& vdQ, uivector& vuiQDims ) 
                 {vdQ = m_vdQ; vuiQDims = m_vuiQDims;}
        void setQ( const dvector& vdQ, const uivector& vuiQDims ) 
                 {m_vdQ = vdQ; m_vuiQDims = vuiQDims;}

        void getX( dvector& vdX, uivector& vuiXDims ) 
                 {vdX = m_vdX; vuiXDims = m_vuiXDims;}
        void setX( const dvector& vdX, const uivector& vuiXDims ) 
                 {m_vdX = vdX; m_vuiXDims = vuiXDims;}

        bool foundFuncParams() { return m_bFuncParamsRead; }
        void getFuncParams( dvector& vdFuncParams, uivector& vuiFuncParamsDims ) 
                       {vdFuncParams = m_vdFuncParams; vuiFuncParamsDims = m_vuiFuncParamsDims;}
        void setFuncParams( const dvector& vdFuncParams, const uivector& vuiFuncParamsDims ) 
                       {m_vdFuncParams = vdFuncParams; m_vuiFuncParamsDims = vuiFuncParamsDims;}

        void getFuncType( string& strFuncType ) { strFuncType = m_strFuncType; }
        void setFuncType( const string& strFuncType ) { m_strFuncType = strFuncType; }

        void getMax( dvector& vdMax, uivector& vuiMaxDims ) 
                 {vdMax = m_vdMax; vuiMaxDims = m_vuiMaxDims;}
        void setMax( const dvector& vdMax, const uivector& vuiMaxDims ) 
                 {m_vdMax = vdMax; m_vuiMaxDims = vuiMaxDims;}

        void getMin( dvector& vdMin, uivector& vuiMinDims ) 
                 {vdMin = m_vdMin; vuiMinDims = m_vuiMinDims;}
        void setMin( const dvector& vdMin, const uivector& vuiMinDims ) 
                 {m_vdMin = vdMin; m_vuiMinDims = vuiMinDims;}

        bool foundDelta() { return m_bDeltaRead; }
        void getDelta( dvector& vdDelta, uivector& vuiDeltaDims ) 
                 {vdDelta = m_vdDelta; vuiDeltaDims = m_vuiDeltaDims;}
        void setDelta( const dvector& vdDelta, const uivector& vuiDeltaDims ) 
                 {m_vdDelta = vdDelta; m_vuiDeltaDims = vuiDeltaDims;}

        void getName( string& strName ) { strName = m_strName; }
        void setName( const string& strName ) { m_strName = strName; }

        void getScale( string& strScale ) { strScale = m_strScale; }
        void setScale( const string& strScale ) { m_strScale = strScale; }

        void getSign( dvector& vdSign, uivector& vuiSignDims ) 
                 {vdSign = m_vdSign; vuiSignDims = m_vuiSignDims;}
        void setSign( const dvector& vdSign, const uivector& vuiSignDims ) 
                 {m_vdSign = vdSign; m_vuiSignDims = vuiSignDims;}

        void getSymbol( string& strSymbol ) { strSymbol = m_strSymbol; }
        void setSymbol( const string& strSymbol ) { m_strSymbol = strSymbol; }

        bool foundToMeV() { return m_bToMeVRead; }
        void getToMeV( dvector& vdToMeV, uivector& vuiToMeVDims ) 
                 {vdToMeV = m_vdToMeV; vuiToMeVDims = m_vuiToMeVDims;}
        void setToMeV( const dvector& vdToMeV, const uivector& vuiToMeVDims ) 
                 {m_vdToMeV = vdToMeV; m_vuiToMeVDims = vuiToMeVDims;}

        void getType( string& strType ) { strType = m_strType; }
        void setType( const string& strType ) { m_strType = strType; }

        void getUnit( string& strUnit ) { strUnit = m_strUnit; }
        void setUnit( const string& strUnit ) { m_strUnit = strUnit; }

        void getValid( dvector& vdValid, uivector& vuiValidDims ) 
                 {vdValid = m_vdValid; vuiValidDims = m_vuiValidDims;}
        void setValid( const dvector& vdValid, const uivector& vuiValidDims ) 
                 {m_vdValid = vdValid; m_vuiValidDims = vuiValidDims;}

        // Don't allow sets of entire object (get it and operate on it, thus no sets or consts)
        AEDBLinearBasisFunction& getLinearBasisFunction()  { return m_linearBasisFn; }

        AEDBTransform& getTransform() { return m_transform; }

        // Constant string values used in db model grid dimension fields

        static const string AEDB_FUNCTYPE_NONE;
        static const string AEDB_FUNCTYPE_POWER;
        static const string AEDB_FUNCTYPE_LOG10;

        static const string AEDB_SCALE_LOG;
        static const string AEDB_SCALE_LINEAR;

        static const string AEDB_DIMTYPE_LIST;
        static const string AEDB_DIMTYPE_FORMULA;

    protected:

        // data members

        // persisted fields
        dvector  m_vdN1;
        uivector m_vuiN1Dims;

        dvector  m_vdQ;
        uivector m_vuiQDims;

        dvector  m_vdX;
        uivector m_vuiXDims;

        bool     m_bFuncParamsRead;
        dvector  m_vdFuncParams;    // optional field
        uivector m_vuiFuncParamsDims;

        string   m_strFuncType;

        dvector  m_vdMax;
        uivector m_vuiMaxDims;

        dvector  m_vdMin;
        uivector m_vuiMinDims;

        bool     m_bDeltaRead;
        dvector  m_vdDelta;
        uivector m_vuiDeltaDims;

        string   m_strName;

        string   m_strScale;

        dvector  m_vdSign;
        uivector m_vuiSignDims;

        string   m_strSymbol;

        bool     m_bToMeVRead;
        dvector  m_vdToMeV;         // optional field
        uivector m_vuiToMeVDims;

        string   m_strType;

        string   m_strUnit;

        dvector  m_vdValid;
        uivector m_vuiValidDims;

        AEDBLinearBasisFunction m_linearBasisFn;

        AEDBTransform m_transform;
    };
};

#endif
