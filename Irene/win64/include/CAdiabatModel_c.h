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

   Copyright 2017 Atmospheric and Environmental Research, Inc. (AER)

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
  1.0          12/04/2017  Created 
  
******************************************************************************/

#ifndef CADIABATMODEL_C_H
#define CADIABATMODEL_C_H

#ifdef __cplusplus
#include "CAdiabatModel.h"

extern "C" {
#endif

typedef int HANDLE;

HANDLE AdiabatStartUp( );
int AdiabatShutDown( HANDLE zHandle );

int AdiabatSetModelDBDir( HANDLE zHandle,
                          char* szModelDBDir );
int AdiabatGetModelDBDir( HANDLE zHandle,
                          char* szModelDBDir ); // > 80 char
int AdiabatSetKPhiDBFile( HANDLE zHandle,
                          char* szKPhiDBFile );
int AdiabatSetKHMinDBFile( HANDLE zHandle,
                           char* szKHMinDBFile );
int AdiabatSetMagfieldDBFile( HANDLE zHandle,
                              char* szMagfieldDBFile );
int AdiabatGetKPhiDBFile( HANDLE zHandle,
                          char* szKPhiDBFile );
int AdiabatGetKHMinDBFile( HANDLE zHandle,
                           char* szKHMinDBFile );
int AdiabatGetMagfieldDBFile( HANDLE zHandle,
                              char* szMagfieldDBFile );

int AdiabatSetKMin( HANDLE zHandle,
                    double dKMin );
int AdiabatGetKMin( HANDLE zHandle,
                    double* pdKMin );
int AdiabatSetKMax( HANDLE zHandle,
                    double dKMax );
int AdiabatGetKMax( HANDLE zHandle,
                    double* pdKMax );
int AdiabatSetHMinMin( HANDLE zHandle,
                       double dHMinMin );
int AdiabatGetHMinMin( HANDLE zHandle,
                       double* pdHMinMin );
int AdiabatSetHMinMax( HANDLE zHandle,
                       double dHMinMax );
int AdiabatGetHMinMax( HANDLE zHandle,
                       double* pdHMinMax  );
int AdiabatSetPhiMin( HANDLE zHandle,
                      double dPhiMin );
int AdiabatGetPhiMin( HANDLE zHandle,
                      double* pdPhiMin );
int AdiabatSetPhiMax( HANDLE zHandle,
                      double dPhiMax );
int AdiabatGetPhiMax( HANDLE zHandle,
                      double* pdPhiMax );

int AdiabatUpdateLimits( HANDLE zHandle );

    // date for magnetic moment value for Phi -> L* conversion; 0 means time varying
int AdiabatSetMagMomentTime( HANDLE zHandle,
                             double dMagMomTime );
int AdiabatGetMagMomentTime( HANDLE zHandle,
                             double* pdMagMomTime );

    // multiple times, with fixed set of pitch angles.
int AdiabatComputeCoordinateSet( HANDLE zHandle,
                                 char* szCoordSys,
                                 char* szCoordUnits,
                                 double* pvdTimes,
                                 double* pvdCoord1,
                                 double* pvdCoord2,
                                 double* pvdCoord3,
                                 int iNumTimes,
                                 double* pvdPitchAngles,
                                 int iNumPitchAngles,
                                 double* pvvdAlpha,
                                 double* pvvdLm,
                                 double* pvvdK,
                                 double* pvvdPhi,
                                 double* pvvdHmin,
                                 double* pvvdLstar,
                                 double* pvdBmin,
                                 double* pvdBlocal,
                                 double* pvdMagLT,
                                 double* pvvdB,
                                 double* pvvdI );
                                                 
    // multiple times, with varying set of pitch angles.
int AdiabatComputeCoordinateSetVarPitch( HANDLE zHandle,
                                         char* szCoordSys,
                                         char* szCoordUnits,
                                         double* pvdTimes,
                                         double* pvdCoord1,
                                         double* pvdCoord2,
                                         double* pvdCoord3,
                                         int iNumTimes,
                                         double* pvvdPitchAngles,
                                         int iNumPitchAngles,
                                         double* pvvdAlpha,
                                         double* pvvdLm,
                                         double* pvvdK,
                                         double* pvvdPhi,
                                         double* pvvdHmin,
                                         double* pvvdLstar,
                                         double* pvdBmin,
                                         double* pvdBlocal,
                                         double* pvdMagLT,
                                         double* pvvdB,
                                         double* pvvdI );

    // determine pitch angles from direction vectors
int AdiabatCalcDirPitchAngles( HANDLE zHandle,
                               char* szCoordSys,
                               char* szCoordUnits,
                               double* pvdTimes,
                               double* pvdCoord1,
                               double* pvdCoord2,
                               double* pvdCoord3,
                               double* pvvdDir1,
                               double* pvvdDir2,
                               double* pvvdDir3,
                               int iNumTimes,
                               int iNumDirs,
                               double* pvvdPitchAngles );

int AdiabatConvertCoordinatesSingle( HANDLE zHandle,
                                     char* szCoordSys,
                                     char* szCoordUnits,
                                     double dTime,
                                     double dCoord1,
                                     double dCoord2,
                                     double dCoord3,
                                     char* szNewCoordSys,
                                     char* szNewCoordUnits,
                                     double* pdNewCoord1,
                                     double* pdNewCoord2,
                                     double* pdNewCoord3 );
int AdiabatConvertCoordinates( HANDLE zHandle,
                               char* szCoordSys,
                               char* szCoordUnits,
                               double* pvdTimes,
                               double* pvdCoord1,
                               double* pvdCoord2,
                               double* pvdCoord3,
                               int iNumTimes,
                               char* szNewCoordSys,
                               char* szNewCoordUnits,
                               double* pvdNewCoord1,
                               double* pvdNewCoord2,
                               double* pvdNewCoord3 );

#ifdef __cplusplus
} // extern "C"
#endif

#endif  // ADIABATMODEL_C_H
