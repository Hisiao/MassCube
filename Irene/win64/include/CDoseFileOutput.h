/******************************************************************************
$HeadURL$

 File: CDoseFileOutput.h

 Description: Declarations for methods for the generation of output files
   containing the dose calculations from flux and/or fluence data values from
   existing existing data files.

 Classification:

   Unclassified

 Project Name:

   AE9/AP9/SPM Radiation Environment Models

   Developed under US Government contract # FA9453-12-C-0231

 Rights and Restrictions:

   Copyright 2018 Atmospheric and Environmental Research, Inc. (AER)

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

  Version      Date        Notes
  1.0          02/22/2018  Created 
  
SVNTag: $Id$
******************************************************************************/

#ifndef CACCUMFILEOUTPUT_H
#define CACCUMFILEOUTPUT_H

#include "CAsciiFileIO.h"
#include "CDataFileInput.h"
#include "CDataFileOutput.h"
#include "CDataCalcInfo.h"
#include "CDoseModel.h"

class DoseFileOutput : public DataFileOutput, public DataCalcInfo
{
  public:
    void setChunkSize( int iChunkSize ) { m_iChunkSize = iChunkSize; }
    DoseFileOutput();
    ~DoseFileOutput();
    int setDoseFileBase( const string& strDataFileBase,
                         bool bBinary=false,
                         int iBinId=-1 );
    int writeDoseFiles( const int& iSegment=-1 );
    int setupDoseFileHeader( );
    int setupDoseFileHeader( vector<string>& strvHeaderInput,
                             structDataCalc& sDataCalc );

    // shield dose parameters to apply to any 'doserate' or 'doseaccum' DataType calculation
 
    int setDoseGeometry( const string& strDoseGeom=string("Spherical") );
    string getDoseGeometry( ) { return m_strDoseGeom; }
    int setDoseDetector( const string& strDoseDetect=string("Silicon") );
    string getDoseDetector( ) { return m_strDoseDetect; }
    int setDoseNuclAtten( const string& strDoseAtten=string("None") );
    string getDoseNuclAtten( ) { return m_strDoseAtten; }
    void setDoseWithBrems( bool bVerdict=true )
       { m_bDoseWithBrems = bVerdict; }
    bool getDoseWithBrems( ) { return m_bDoseWithBrems; }

    int setDoseDepth( const double& dDepth = -1.0 ); //default clears list
    int setDoseDepths( const dvector& vdDepth );
    int setDoseDepthUnit( const string& strDepthUnit = string("mm") );
    
    void setDoseDepth( dvector& vdDepth ) { vdDepth = m_vdDoseDepth; }
    string getDoseDepthUnit( ) { return m_strDoseDepthUnit; }

    // no check of file validity at this point; will be done within ShieldDose initialization
    void setDoseModelDBFile( const string& strDoseModelDBFile ) 
      { m_strDoseModelDBFile = strDoseModelDBFile; }
    string getDoseModelDBFile( ) { return m_strDoseModelDBFile; }

  private:
    int m_iChunkSize;
    int m_iFluxType;
    bool m_bDoseWithBrems;

    dvector m_vdEnergies;

    // dose calculation parameters
    string m_strDoseGeom;
    string m_strDoseDetect;
    string m_strDoseAtten;
    string m_strDoseDepthUnit;
    dvector m_vdDoseDepth;
    string m_strDoseModelDBFile;

    DoseModel *m_pDoseModel;
};

#endif

