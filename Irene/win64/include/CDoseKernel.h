/******************************************************************************
$HeadURL$

 File: CDoseKernel.h

 Description: Declaration for methods for the calculation of dosage rates
   and/or accumulations, employing the underlying kernel-based model.

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
  1.0          01/23/2018  Created 
  
SVNTag: $Id$
******************************************************************************/
#ifndef CDOSEKERNEL_H
#define CDOSEKERNEL_H

#include <string>
#include "VectorTypes.h"
#include "CKernelModel.h"

class DoseKernel
{
  public:
    DoseKernel();
    ~DoseKernel();

    int setKernelXmlPath( const string& strKernelXmlPath );
    string getKernelXmlPath( ) { return m_strKernelXmlPath; }
    int setKernelXmlFile( const string& strKernelXmlFile,
                          bool bExternal=true );
    string getKernelXmlFile( ) { return m_strKernelXmlFile; }

    int setSpecies( const string strSpecies );
    string getSpecies( ) { return m_strSpecies; }
    int setEnergies( const dvector& vdEnergies, 
                     string strEnergyUnits=string("MeV") );
    int getNumEnergies() { return int(m_vdEnergies.size()); }
    int setDepths( const dvector& vdDepths,
        					 string strDepthUnits=string("mm") );
    int getNumDepths() { return int(m_vdDepths.size()); }
    int setDetector( const string strDetector );
    string getDetector( ) { return m_strDetectorType; }
    int setGeometry( const string strGeometry );
    string getGeometry();
    int setNuclearAttenMode( const string strNucAttenMode );
    string getNuclearAttenMode( ) { return m_strNucAttenMode; }
    void setWithBrems( bool bVerdict=true )
       { m_bWithBrems = bVerdict; m_bParamChange = true; }
    bool getWithBrems( ) { return m_bWithBrems; }   

    int computeFluxDose( const dvector& vdFluxData, 
                         dvector& vdDoseData );
    int computeFluxDose( const vdvector& vvdFluxData, 
                         vdvector& vvdDoseData );
    int computeFluxDoseRate ( const vvdvector& vvvdFluxData, 
                              vvdvector& vvvdDoseData );
    // 3d input/output fluence[time][energy][dir=1] -> dose[time][depth][dir=1]
    int computeFluenceDose ( const vvdvector& vvvdFluenceData,
                             vvdvector& vvvdDoseVal )
        { return computeFluxDoseRate( vvvdFluenceData, vvvdDoseVal ); }

  protected:
    int setKernelXmlFile( );
    int Initialize();
    int expandEnvVarPath( const string& strEnvPath,
                          string& strExpandedPath,
                          const string& strKey );

    string m_strKernelXmlPath;
    string m_strKernelXmlFile;
    string m_strSpecies;
    string m_strDetectorType;
    string m_strGeometry;
    string m_strNucAttenMode;
    bool m_bWithBrems;

    dvector m_vdEnergies;
    dvector m_vdDepths;
    string m_strDepthUnits;
    dvector m_vdDepthsMils;

    KernelModel *m_pDoseKernel;
    bool m_bParamChange;
    bool m_bExternFileSpec;
};

#endif
