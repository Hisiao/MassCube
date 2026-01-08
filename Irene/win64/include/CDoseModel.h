/******************************************************************************

 File: CDoseModel.h

 Description: Declarations for methods for the calculation of dosage rates
   and/or accumulations, employing the underlying ShieldDose2 model.

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
  1.0          09/18/2015  Created 
  2.0          06/28/2017  removed superfluous methods, eliminated accumulation
  
******************************************************************************/

#ifndef DOSEMODEL_H
#define DOSEMODEL_H

#include <string>
#include "VectorTypes.h"
#include "DataBaseNames.h"

#include "CShieldDose2.h"

class DoseModel {
  public:
    // constructors

    DoseModel();

    // destructor

    virtual ~DoseModel();

    // Accessors for dose calculation parameters

    int setModelDBDir( const string& strModelDBDir );
    string getModelDBDir( ) { return m_strModelDBDir; }
    int setModelDBFile( const string& strModelDBFile );
    string getModelDBFile() { return m_strModelDBFile; }
    void setSpecies( ecsParticles eSpecies ) 
                   { m_eSpecies = eSpecies; m_bParamChange = true; }
    int setSpecies( const string strSpecies );
    string getSpecies();
    int setEnergies( const dvector& vdEnergies, string strEnergyUnits=string("MeV") );
    int getNumEnergies() { return int(m_vdEnergies.size()); }
    int setDepths( const dvector& vdDepths, string strDepthUnits=string("mm") );
    int getNumDepths() { return int(m_vdDepths.size()); }
    void setDetector( ecsDetectorType eDetectorType )
                   { m_eDetectorType = eDetectorType; m_bParamChange = true; }
    int setDetector( const string strDetector );
    string getDetector();
    void setGeometry( ecsDetectorGeometry eGeometry )
                   { m_eGeometry = eGeometry; m_bParamChange = true; }
    int setGeometry( const string strGeometry );
    string getGeometry();
    void setNuclearAttenMode( ecsNuclearAttenMode eNucAttenMode )
                   { m_eNucAttenMode = eNucAttenMode; m_bParamChange = true; }
    int setNuclearAttenMode( const string strNucAttenMode );
    string getNuclearAttenMode();
    void setLinearInterp( bool bVerdict=true ) 
       { m_eInterpType = bVerdict ? ecsLinear : ecsSpline; m_bParamChange = true; }
    bool getLinearInterp() { return (m_eInterpType==ecsLinear); }
    void setWithBrems( bool bVerdict=true )
       { m_bWithBrems = bVerdict; }
    bool getWithBrems( ) { return m_bWithBrems; }   

    // 3D input/output flux[time][energy][dir=1] -> doserate[time][depth][dir=1]
    int computeFluxDoseRate( const vvdvector& vvvdFluxData, 
                             vvdvector& vvvdDoseRate );
    // 3d input/output fluence[time][energy][dir=1] -> dose[time][depth][dir=1]
    int computeFluenceDose( const vvdvector& vvvdFluenceData, 
                            vvdvector& vvvdDoseVal )
        { return computeFluxDoseRate( vvvdFluenceData, vvvdDoseVal ); }

    // 2d input/output flux[energy][dir=1] -> doserate[depth][dir=1]
    //  -dose calculations only value for *omnidirectional* flux
    //  -2nd dimension retained only for compatibility with file input/output methods
    int computeFluxDose( const vdvector& vvdFluxData, 
                         vdvector& vvdDoseData );

    // 1d input/output flux[energy] -> doserate[depth]
    int computeFluxDose( const dvector& vdFluxData, 
                         dvector& vdDoseData );


  protected:

    //
    // Performs one-time initialization of embedded dose calculator
    int Initialize();

    int expandEnvVarPath( const string& strEnvPath,
                          string& strExpandedPath,
                          const string& strKey );
    int checkFileExists( const string& strFileName );
    int checkDirExists( const string& strDirName );
    string appendDirDelim( const string strDirName );

    string              m_strModelDBDir; // dir for automatic file naming
    string              m_strModelDBFile;
    // ShieldDose2 parameters
    ecsParticles        m_eSpecies;
    dvector             m_vdEnergies;
    double              m_dEScale;
    ecsShieldThickUnits m_eDepthUnits;
    dvector             m_vdDepths;
    ecsDetectorType     m_eDetectorType;
    ecsDetectorGeometry m_eGeometry;
    ecsNuclearAttenMode m_eNucAttenMode;
    ecsInterpolationType m_eInterpType;
    bool                m_bWithBrems;
    bool                m_bFullSphereGeom;  // false=2pi, true=4pi

    CShieldDose2*       m_pShieldDose2;    //###??? cast as void* to limit exposure to Ae9Ap9 clients
    bool                m_bParamChange;  // flag for (re-)loading parameters

};


#endif
