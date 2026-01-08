/******************************************************************************

 File: CAdiabatModel.h

 Description: Declarations for methods for the calculation of adiabatic
   invariant values for the input time and spatial coordinate values.

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
  
******************************************************************************/

#ifndef CADIABATMODEL_H
#define CADIABATMODEL_H

#include <string>
#include <vector>

#include "DataBaseNames.h"

#include "CAdiabaticCoords.h"  // SpWx class

class AdiabatModel
{
  public:
    AdiabatModel();
    ~AdiabatModel();

    int setModelDBDir( const string& strModelDBDir );
    string getModelDBDir( ) { return m_strModelDBDir; }
    int setKPhiDBFile( const string& strKPhiDBFile );
    int setKHMinDBFile( const string& strKHMinDBFile );
    int setMagfieldDBFile( const string& strMagfieldDBFile );
    string getKPhiDBFile( ) { return m_strKPhiDBFile; }
    string getKHMinDBFile( ) { return m_strKHMinDBFile; }
    string getMagfieldDBFile( ) { return m_strMagfieldDBFile; }

    int initialize( const string& strModelDBDir );
    int initialize( const string& strKPhiDBFile,
                    const string& strKHMinDBFile,
                    const string& strMagfieldDBFile );
    int initialize();

    // validation of the coordinate min/max values can only be performed once the
    //  underlying AdiabaticCoords class is initialized (w/ the 3 db filenames)
    void setK_Min( const double& dK_Min=0.0 ) { m_dK_Min = dK_Min; }
    double getK_Min( ) { return m_dK_Min; }
    void setK_Max( const double& dK_Max=25.0 ) { m_dK_Max = dK_Max; }
    double getK_Max( ) { return m_dK_Max; }
    void setHminMin( const double& dHminMin=-500.0 ) { m_dHminMin = dHminMin; }
    double getHminMin( ) { return m_dHminMin; }
    void setHminMax( const double& dHminMax=50000.0 ) { m_dHminMax = dHminMax; }
    double getHminMax( ) { return m_dHminMax; }
    void setPhiMin( const double& dPhiMin=0.15849 ) { m_dPhiMin = dPhiMin; }
    double getPhiMin( ) { return m_dPhiMin; }
    void setPhiMax( const double& dPhiMax=2.0 ) { m_dPhiMax = dPhiMax; }
    double getPhiMax( ) { return m_dPhiMax; }

    // calls to validate the coordinate min/max values; but first calls to
    //   initialize the underlying AdiabaticCoords class (w/ the 3 db filenames)
    int updateLimits( );

    // date for magnetic moment value for Phi -> L* conversion;
    //  time=0 means time varying - but then no longer 'invariant'
    int setMagMomentTime( const double& dMagMomentTime = 0.0 );
    double getMagMomentTime( ) { return m_dMagMomentTime; }

    int convertCoordinates( const string& strCoordSys,
                            const string& strCoordUnits,
                            const double& dMjdTime,
                            const double& dCoord1,
                            const double& dCoord2,
                            const double& dCoord3,
                            const string& strNewCoordSys,
                            const string& strNewCoordUnits,
                            double& dNewCoord1,
                            double& dNewCoord2,
                            double& dNewCoord3 );
    int convertCoordinates( const string& strCoordSys,
                            const string& strCoordUnits,
                            const dvector& vdMjdTimes,
                            const dvector& vdCoord1,
                            const dvector& vdCoord2,
                            const dvector& vdCoord3,
                            const string& strNewCoordSys,
                            const string& strNewCoordUnits,
                            dvector& vdNewCoord1,
                            dvector& vdNewCoord2,
                            dvector& vdNewCoord3 );

    // enumerated coordsys, multiple times, with fixed set of pitch angles.
    int computeCoordinateSet( const emfCoordSys& eCoordSys,
                              const dvector& vdMjdTimes,
                              const dvector& vdCoord1,
                              const dvector& vdCoord2,
                              const dvector& vdCoord3,
                              const dvector& vdPitchAngles,
                              vdvector&      vvdAlpha,
                              vdvector&      vvdLm,
                              vdvector&      vvdK,
                              vdvector&      vvdPhi,
                              vdvector&      vvdHmin,
                              vdvector&      vvdLstar,
                              dvector&       vdBmin,
                              dvector&       vdBlocal,
                              dvector&       vdMagLT,
                              vdvector&      vvdB,
                              vdvector&      vvdI );
    // coordsys strings, multiple times, with fixed set of pitch angles.
    int computeCoordinateSet( const string& strCoordSys,
                              const string& strCoordUnits,
                              const dvector& vdMjdTimes,
                              const dvector& vdCoord1,
                              const dvector& vdCoord2,
                              const dvector& vdCoord3,
                              const dvector& vdPitchAngles,
                              vdvector&      vvdAlpha,
                              vdvector&      vvdLm,
                              vdvector&      vvdK,
                              vdvector&      vvdPhi,
                              vdvector&      vvdHmin,
                              vdvector&      vvdLstar,
                              dvector&       vdBmin,
                              dvector&       vdBlocal,
                              dvector&       vdMagLT,
                              vdvector&      vvdB,
                              vdvector&      vvdI );
    // enumerated coordsys, multiple times, with time-dependent pitch angle sets.
    int computeCoordinateSet( const emfCoordSys& eCoordSys,
                              const dvector& vdMjdTimes,
                              const dvector& vdCoord1,
                              const dvector& vdCoord2,
                              const dvector& vdCoord3,
                              const vdvector& vvdPitchAngles,
                              vdvector&      vvdAlpha,
                              vdvector&      vvdLm,
                              vdvector&      vvdK,
                              vdvector&      vvdPhi,
                              vdvector&      vvdHmin,
                              vdvector&      vvdLstar,
                              dvector&       vdBmin,
                              dvector&       vdBlocal,
                              dvector&       vdMagLT,
                              vdvector&      vvdB,
                              vdvector&      vvdI );
    // coordsys strings, multiple times, with time-dependent pitch angle sets.
    int computeCoordinateSet( const string& strCoordSys,
                              const string& strCoordUnits,
                              const dvector& vdMjdTimes,
                              const dvector& vdCoord1,
                              const dvector& vdCoord2,
                              const dvector& vdCoord3,
                              const vdvector& vvdPitchAngles,
                              vdvector&      vvdAlpha,
                              vdvector&      vvdLm,
                              vdvector&      vvdK,
                              vdvector&      vvdPhi,
                              vdvector&      vvdHmin,
                              vdvector&      vvdLstar,
                              dvector&       vdBmin,
                              dvector&       vdBlocal,
                              dvector&       vdMagLT,
                              vdvector&      vvdB,
                              vdvector&      vvdI );
    // enumerated coordsys, single time, with fixed set of pitch angles   
    int computeCoordinateSet( const emfCoordSys& eCoordSys,               
                              const double&  dMjdTime,                    
                              const double&  dCoord1,                     
                              const double&  dCoord2,                     
                              const double&  dCoord3,                     
                              const dvector& vdPitchAngles,               
                              dvector&       vdAlpha,                     
                              dvector&       vdLm,                        
                              dvector&       vdK,                         
                              dvector&       vdPhi,                       
                              dvector&       vdHmin,                      
                              dvector&       vdLstar,                     
                              double&        dBmin,                       
                              double&        dBlocal,                     
                              double&        dMagLT,                      
                              dvector&       vdB,                         
                              dvector&       vdI );                       
    // coordsys strings, single time, with fixed set of pitch angles
    int computeCoordinateSet( const string& strCoordSys,
                              const string& strCoordUnits,
                              const double&  dMjdTime,
                              const double&  dCoord1,
                              const double&  dCoord2,
                              const double&  dCoord3,
                              const dvector& vdPitchAngles,
                              dvector&       vdAlpha,
                              dvector&       vdLm,
                              dvector&       vdK,
                              dvector&       vdPhi,
                              dvector&       vdHmin,
                              dvector&       vdLstar,
                              double&        dBmin,
                              double&        dBlocal,
                              double&        dMagLT,
                              dvector&       vdB,
                              dvector&       vdI );

    // enumerated coordsys; determine pitch angles from direction vectors
    int calcDirPitchAngles( const emfCoordSys& eCoordSys,
                            const dvector& vdMjdTimes,
                            const dvector& vdCoord1,
                            const dvector& vdCoord2,
                            const dvector& vdCoord3,
                            const vdvector& vvdDirX,
                            const vdvector& vvdDirY,
                            const vdvector& vvdDirZ,
                            vdvector& vvdPitchAngles );
    // coordsys strings; determine pitch angles from direction vectors
    int calcDirPitchAngles( const string& strCoordSys,
                            const string& strCoordUnits,
                            const dvector& vdMjdTimes,
                            const dvector& vdCoord1,
                            const dvector& vdCoord2,
                            const dvector& vdCoord3,
                            const vdvector& vvdDirX,
                            const vdvector& vvdDirY,
                            const vdvector& vvdDirZ,
                            vdvector& vvdPitchAngles );

  protected:
    int expandEnvVarPath( const string& strEnvPath,
                          string& strExpandedPath,
                          const string& strKey );

    string m_strModelDBDir; // dir for automatic file naming
    string m_strMagfieldDBFile;
    string m_strKPhiDBFile;
    string m_strKHMinDBFile;

    CAdiabaticCoords *m_pAdiabatic;
    CMagfield *m_pMagfield;

    // adjustable limits on valid returned value
    double m_dK_Min;
    double m_dK_Max;
    double m_dHminMin;
    double m_dHminMax;
    double m_dPhiMin;
    double m_dPhiMax;

    double m_dMagMomentTime;

    int getCoordSysEnum( const string& strCoordSys, 
                         const string& strCoordUnits,
                         emfCoordSys& eCoordSys );
    int checkFileExists( const string& strFileName );
    int checkDirExists( const string& strDirName );
    string appendDirDelim( const string strDirName );
};

#endif
