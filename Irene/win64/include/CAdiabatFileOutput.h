/******************************************************************************

 File: CAdiabatFileOutput.h

 Description: Declarations for methods for the generation of output files
   containing the adiabatic invariant values, calculated for user-specified 
   ephemeris positions, at one or more direction vectors/pitch angles.

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

#ifndef CADIABATFILEOUTPUT_H
#define CADIABATFILEOUTPUT_H

#include "VectorTypes.h"
#include "CAdiabatModel.h"
#include "CEphemFileInput.h"
#include "CDataFileOutput.h"

class AdiabatFileOutput : public DataFileOutput, public AdiabatModel
{
  public:
    AdiabatFileOutput();
    virtual ~AdiabatFileOutput();

    void setInputFileInfo( const std::string& strInputFile ) { m_strInputFile = strInputFile; }
    int setOrbitFile( const std::string& strOrbitFile,
                      bool bBinary=false,
                      int iBinId=-1 );
    int setPitchAngles( const dvector& vdPitchAngles );
    int setDirectionFile( const std::string& strDirFile );    // always ascii
    int setPitchAngleFile( const std::string& strPitchFile ); // always ascii
    int setOmnidirectional( bool bOmni=true );

    void setChunkSize( const int& iChunkSize ) { m_pEphemFileInput->setChunkSize( iChunkSize ); }

    int setAdiabatFileBase( const std::string& strOutputFileBase,
                            bool bBinary=false,
                            int iBinId=-1 );
    int writeAdiabatFile( );

  private:
    std::string  m_strDirectionFile;
    std::string  m_strPitchAngleFile;
    bool  m_bOmnidirectional;
    bool  m_bDirectionFile;
    bool  m_bPitchAngleFile;
    bool  m_bPitchAngleList;
    bool  m_bEphemPitch;
    bool  m_bEphemDir;
    EphemFileInput *m_pEphemFileInput;

    std::string m_strInputFile;
    std::string m_strOutputFileBase;
    std::string m_strFileExt;

    int setOutputFileBase( const std::string& strFileBase );
    int setupAdiabatFileHeader( );
    int updateOrbitDirInfo( const dvector& vdTimes, 
                            dvector& vdCoord1, 
                            dvector& vdCoord2, 
                            dvector& vdCoord3, 
                            vdvector& vvdDirX, 
                            vdvector& vvdDirY, 
                            vdvector& vvdDirZ, 
                            vdvector& vvdPitchAngles );
};

#endif
