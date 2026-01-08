/******************************************************************************

 File: CAx9FluxFileOutput.h

 Description: Declarations for methods for the generation of output files
   containing the flux values calculated from Ae9, Ap9 or SPM radiation belt 
   models at the user-specified ephemeris positions, for multiple use cases.

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
  
******************************************************************************/

#ifndef CAX9FLUXFILEOUTPUT_H
#define CAX9FLUXFILEOUTPUT_H

#include "CDataFileOutput.h"

#include "Ae9Ap9Model.h"
#include "VectorTypes.h"
#include "CMagfield.h"
#include "CEphemFileInput.h"
#include "CDataFileOutput.h"
#include "CDataCalcInfo.h"

// borrowed from ae9ap9 namespace:
typedef enum eAEEnergyFluxType
{
    eAEEFTUndefined = 0,
    eAEEFTDifferentialAtE1,
    eAEEFTDifferentialE1ToE2,
    eAEEFTIntegral,
} eAEEnergyFluxType;


class Ax9FluxFileOutput : public DataFileOutput, public DataCalcInfo
{
  public:
    Ax9FluxFileOutput ();
    virtual ~Ax9FluxFileOutput ();

    void setVerbose( bool bVerdict = true ) { m_bVerbose = bVerdict; }

    int setFluxType( eAEEnergyFluxType eFluxType );
    int setFluxType( const std::string& strFluxType );
    void setEnergies( const dvector& vdEnergies ) { m_vdEnergies = vdEnergies; }
    void setEnergies2( const dvector& vdEnergies2 ) { m_vdEnergies2 = vdEnergies2; }
    void setFluxPerturb( bool bPerturb = true ) { m_bPerturbFlux = bPerturb; }

    void setMonteCarloEpoch( const double dEpoch ) { m_dEpoch = dEpoch; }

    int setOrbitFile( const std::string& strOrbitFile, bool bBinary=false, int iBinId=-1 );
    int setPitchAngle( const double& dPitchAngle );
    int addPitchAngle( const double& dPitchAngle );
    int setPitchAngles( const dvector& vdPitchAngles );
    int setDirectionFile( const std::string& strDirFile );    // always ascii
    int setPitchAngleFile( const std::string& strPitchFile ); // always ascii
    int setOmnidirectional( bool bOmni=true );

    void setInputFileInfo( const std::string& strInputFile ) { m_strInputFile = strInputFile; }

    void setModelDBFile( const std::string& strModelDBFile ) { m_strModelDBFile = strModelDBFile; }
    void setMagfieldDBFile( const std::string& strMagfieldDBFile ) { m_strMagfieldDBFile = strMagfieldDBFile; }
    void setKPhiDBFile( const std::string& strDBFile ) { m_strKPhiDBFile = strDBFile; }
    void setKHMinDBFile( const std::string& strDBFile ) { m_strKHMinDBFile = strDBFile; }

    void setChunkSize( const int& iChunkSize ) { m_pEphemFileInput->setChunkSize( iChunkSize ); }

    int setAdiabatCoords() { return setFluxMode( eFluxModeAdiabat ); }
  
    int setFluxFileBase( const std::string& strOutputFileBase, bool bBinary=false, int iBinId=-1 );
    int writeFluxFiles( );

  private:
    std::string  m_strModelDBFile;
    std::string  m_strKPhiDBFile;
    std::string  m_strKHMinDBFile;
    std::string  m_strMagfieldDBFile;
    std::string  m_strDirectionFile;
    std::string  m_strPitchAngleFile;
    std::string  m_strDirDataSource;
    bool  m_bOmnidirectional;
    bool  m_bDirectionFile;
    bool  m_bPitchAngleFile;
    bool  m_bPitchAngleList;
    bool  m_bEphemPitch;
    bool  m_bEphemDir;
    EphemFileInput *m_pEphemFileInput;
    CMagfield      *m_pMagfield;

    std::string  m_strInputFile;
    eAEEnergyFluxType m_eFluxType;
    std::string  m_strFluxType;
    dvector m_vdEnergies;
    dvector m_vdEnergies2;
    dvector m_vdPitchAngles;
    double  m_dEpoch;
    bool    m_bPerturbFlux;
    bool    m_bVerbose;

    std::vector<structDataCalc> m_svFluxCalc;

    double *m_pdBuffer;
   
    int setupFluxFileHeader( const eFluxModeType& eFluxMode,
                             const std::string& strModelTag,
                             const std::string& strSpecies );
    int updateOrbitDirInfo( const dvector& vdDates, 
                            dvector& vdX, 
                            dvector& vdY, 
                            dvector& vdZ, 
                            vdvector& vvdDirX, 
                            vdvector& vvdDirY, 
                            vdvector& vvdDirZ, 
                            vdvector& vvdPitchAngles );
};

#endif
