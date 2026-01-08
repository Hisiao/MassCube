/******************************************************************************
$HeadURL$

 File: CEphemFileInput.h

 Description: Declarations for methods for the reading of ephemeris
   information, and possibly direction vectors or pitch angles, from
   external files.

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

#ifndef CEPHEMFILEINPUT_H
#define CEPHEMFILEINPUT_H

#include "CDataFileInput.h"

#include "VectorTypes.h"

class EphemFileInput : public DataFileInput
  {
  public:
    EphemFileInput ();
    virtual ~EphemFileInput ();

    int setEphemFile( const std::string& strFileName, 
                      bool bBinary = false, 
                      const int& iBinId = -1 );
    int setEphemFile( ) { return setDataFile(); } 

    int readEphemChunk ( dvector& vdTimes, 
                         dvector& vdCoord1, 
                         dvector& vdCoord2, 
                         dvector& vdCoord3 );
    int readEphemChunk ( dvector& vdTimes, 
                         dvector& vdCoord1, 
                         dvector& vdCoord2, 
                         dvector& vdCoord3, 
                         vdvector& vvdDirX, 
                         vdvector& vvdDirY, 
                         vdvector& vvdDirZ );
    int readEphemChunk ( dvector& vdTimes, 
                         dvector& vdCoord1, 
                         dvector& vdCoord2, 
                         dvector& vdCoord3, 
                         vdvector& vvdPitchAngles );
    int readEphemChunk ( dvector& vdTimes, 
                         dvector& vdCoord1, 
                         dvector& vdCoord2, 
                         dvector& vdCoord3, 
                         vdvector& vvdDirX, 
                         vdvector& vvdDirY, 
                         vdvector& vvdDirZ, 
                         vdvector& vvdPitchAngles );

    // specify pitch or direction file to read (must be ascii!)
    int setPitchFile( const std::string& strPitchFile );
    int setPitchFile( ); // close/clear specification

    int setDirFile( const std::string& strDirFile );
    int setDirFile( ); // close/clear specification

    // get methods for number of directions : two sources possible
    //   -ephemeris file with integrated direction vectors and/or pitch angles
    //   -"external file" of direction vectors OR pitch angles (cannot be both)
    // these two get methods default to the 'external file' source
    int getNumPitch( bool bExtFile=true );
    int getNumDir( bool bExtFile=true );
  
    int setPitchAngles( const dvector& vdPitchAngles );
    int addPitchAngle ( const double& dPitchAngle );
    int setPitchAngle ( const double& dPitchAngle );
    int setPitchAngles ( );
    dvector getPitchAngles( ) { return m_vdPitchAngles; }

  private:

    int m_iNumPitch;   // number of directions in *external* file or static list
    char m_cPitchDelim;  //  derived direction vector or pitch angle file delimiter character
    AsciiFileIO m_ascDirFile;
    AsciiFileIO m_ascPitchFile;
    dvector m_vdPitchAngles;

    int peekDirData ();
    int readDirChunk( vdvector& vvdX, 
                      vdvector& vvdY, 
                      vdvector& vvdZ );
    int peekPitchData();
    int readPitchChunk( vdvector& vvdPitchAngles );

};

#endif
