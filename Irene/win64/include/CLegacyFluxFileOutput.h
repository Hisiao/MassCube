/******************************************************************************
$HeadURL$

 File: CLegacyFluxFileOutput.h

 Description: Declarations for methods for the generation of output files
   containing the flux values calculated from 'Legacy' radiation belt models
   at the user-specified ephemeris positions.

 Classification:

   Unclassified

 Project Name:

   AE9/AP9/SPM Radiation Environment Models

   Developed under US Government contract # FA9453-12-C-0231

 Rights and Restrictions:

   Copyright 2015 Atmospheric and Environmental Research, Inc. (AER)

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
  
SVNTag: $Id$
******************************************************************************/

#ifndef CLEGACYFLUXFILEOUTPUT_H
#define CLEGACYFLUXFILEOUTPUT_H

#include "CDataFileOutput.h"

#include "CRadEnvModel.h"
#include "CCammiceModel.h"
#include "VectorTypes.h"
#include "CMagfield.h"
#include "CEphemFileInput.h"
#include "CDataFileOutput.h"
#include "CDataCalcInfo.h"

// borrowed from ae9ap9 namespace:
typedef enum eLegFluxType
{
    eFTUndefined = 0,
    eFTDifferential,
    eFTIntegral,
} eLegFluxType;


class LegacyFluxFileOutput : public DataFileOutput, public DataCalcInfo
{
  public:
    LegacyFluxFileOutput ();
    virtual ~LegacyFluxFileOutput ();

    int setModel( const std::string& strModel) ;

    // these seven are required for RadEnvModel / ignored for CammiceModel
    int setFluxType( const std::string& strFluxType );
    void setEnergies( const dvector& vdEnergies ) 
      { m_vdEnergies = vdEnergies; }
    void setActivityLevel( const std::string& strActivityLevel ) 
      { m_strActivityLevel = strActivityLevel; }
    void setActivityRange( const std::string& strActivityRange ) 
      { m_strActivityRange = strActivityRange; }
    void set15DayAvgAp( double d15DayAvgAp ) 
      { m_d15DayAvgAp = d15DayAvgAp; }
    // these two default to true if not specified
    void setFixEpoch( bool bFixEpoch ) 
      { m_bFixEpoch = bFixEpoch; }
    void setShiftSAA( bool bShiftSAA ) 
      { m_bShiftSAA = bShiftSAA; }

    // these four are required for CammiceModel / ignored for RadEnvModel
    int setSpecies ( const vector<std::string>& vstrSpecies );
    int setSpecies ( const std::string& strSpecies="" );
    void setMagfieldModel( const std::string& strMagfieldModel )
      { m_strMagfieldModel = strMagfieldModel; } // 'igrf' vs 'igrfop'
    void setDataFilter( const std::string& strDataFilter )
      { m_strDataFilter = strDataFilter; }
    void setPitchAngleBin( const std::string& strPitchAngleBin )
      { m_strPitchAngleBin = strPitchAngleBin; }

    int setOrbitFile( const std::string& strOrbitFile, 
                      bool bBinary=false, 
                      int iBinId=-1 );

    void setInputFileInfo ( const std::string& strInputFile ) 
      { m_strInputFile = strInputFile; }

    void setModelDBFile( const std::string& strModelDBFile ) 
      { m_strModelDBFile = strModelDBFile; }
    void setMagfieldDBFile( const std::string& strMagfieldDBFile ) 
      { m_strMagfieldDBFile = strMagfieldDBFile; }

    void setChunkSize( const int& iChunkSize ) 
      { m_pEphemFileInput->setChunkSize( iChunkSize ); }

    int setFluxFileBase ( const std::string& strOutputFileBase, 
                          bool bBinary=false, 
                          int iBinId=-1 );
    int writeFluxFiles ( );

  private:
    std::string  m_strModel;
    std::string  m_strModelDBFile;
    std::string  m_strMagfieldDBFile;
    EphemFileInput *m_pEphemFileInput;
    CMagfield      *m_pMagfield;

    std::string  m_strInputFile;
    std::string  m_strFluxType;

    bool m_bFixEpoch;
    bool m_bShiftSAA;
    double m_d15DayAvgAp;
    std::string m_strActivityRange;
    std::string m_strActivityLevel;
    std::string m_strEnvironSetting;
    
    std::string m_strMagfieldModel;
    std::string m_strDataFilter;
    std::string m_strPitchAngleBin;
    std::string m_strFluxDir;
    vector<std::string> m_vstrSpecies;

    dvector m_vdEnergies;
    dvector m_vdEnergies2;
    dvector m_vdCamE1;
    dvector m_vdCamE2;

    std::vector<structDataCalc> m_svFluxCalc;

    double *m_pdBuffer;
   
    int setupFluxFileHeader ( const eFluxModeType& eFluxMode,
                              const std::string& strModelTag,
                              const std::string& strSpecies );
    int updateOrbitCoords ( const dvector& vdTimes, 
                            dvector& vdCoord1, 
                            dvector& vdCoord2, 
                            dvector& vdCoord3 );
};

#endif
