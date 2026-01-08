/******************************************************************************

 File: CEphemModel_c.h

 Description: Declarations for methods to perform orbit propagation using
   the underlying Kepler, SGP4 or SatEph (Lokangle) propagator models.

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
  1.0          09/18/2015  Created 
  
******************************************************************************/

#ifndef CEPHEMMODEL_C_H
#define CEPHEMMODEL_C_H

#ifdef __cplusplus
#include "CEphemModel.h"

extern "C" {
#endif

typedef int HANDLE;

HANDLE EphemStartUp( );
int EphemShutDown( HANDLE zHandle );

int EphemSetModelDBDir( HANDLE zHandle,
                        char* szModelDBDir );
int EphemGetModelDBDir( HANDLE zHandle,
                        char* szModelDBDir ); // > 80 char
int EphemSetMagfieldDBFile( HANDLE zHandle,
                            char* szMagfieldDBFile );
int EphemGetMagfieldDBFile( HANDLE zHandle,
                            char* szMagfieldDBFile );
int EphemSetChunkSize( HANDLE zHandle,
                       int iChunkSize );
int EphemGetChunkSize( HANDLE zHandle );

int EphemSetStartTime( HANDLE zHandle,
                       double dStartTime );
int EphemGetStartTime( HANDLE zHandle,
                       double* pdStartTime );
int EphemSetEndTime( HANDLE zHandle,
                     double dEndTime );
int EphemGetEndTime( HANDLE zHandle,
                     double* pdEndTime );
int EphemSetTimeStep( HANDLE zHandle,
                      double dInterval );
int EphemGetTimeStep( HANDLE zHandle,
                      double* pdInterval );
int EphemSetTimes( HANDLE zHandle,
                   double dStartTime,
                   double dEndTime,
                   double dTimestepSecs );
int EphemGetTimes( HANDLE zHandle,
                   double* pdStartTime, 
                   double* pdEndTime, 
                   double* pdTimeStepSec );
int EphemSetVarTimeStep( HANDLE zHandle,
                         double dTimeMinStepSec,
                         double dTimeMaxStepSec,
                         double dTimeRoundSec );
int EphemSetVarTimes( HANDLE zHandle,
                      double dStartTime, 
                      double dEndTime, 
                      double dTimeMinStepSec,
                      double dTimeMaxStepSec,
                      double dTimeRoundSec );
int EphemGetVarTimeStep( HANDLE zHandle,
                         double* pdTimeMinStepSec,
                         double* pdTimeMaxStepSec,
                         double* pdTimeRoundSec );
int EphemGetVarTimes( HANDLE zHandle,
                      double* pdStartTime, 
                      double* pdEndTime, 
                      double* pdTimeMinStepSec,
                      double* pdTimeMaxStepSec,
                      double* pdTimeRoundSec );
int EphemSetTimesList( HANDLE zHandle,
                       double *pvdTimes, 
                       int iNumTimes );  
int EphemGetNumTimesList( HANDLE zHandle );
int EphemGetNumTimes( HANDLE zHandle )
     { return EphemGetNumTimesList( zHandle ); }
int EphemGetTimesList( HANDLE zHandle,
                       double *pvdTimes );
int EphemClearTimesList( HANDLE zHandle );

int EphemSetPropagator( HANDLE zHandle,
                        char* szPropSpec );
int EphemGetPropagator( HANDLE zHandle,
                        char* szPropSpec ); // >= 8 char
int EphemSetSGP4param( HANDLE zHandle,
                       char* szMode, char* strWGS );
int EphemGetSGP4Mode( HANDLE zHandle,
                       char* szMode );
int EphemGetSGP4WGS( HANDLE zHandle,
                       char* strWGS );
int EphemSetKeplerUseJ2( HANDLE zHandle,
                         int iUseJ2 );
int EphemGetKeplerUseJ2( HANDLE zHandle );
int EphemSetVerbose( HANDLE zHandle,
                     int iVerbose );

int EphemSetTLEFile( HANDLE zHandle,
                     char* szTLEFile );
int EphemGetTLEFile( HANDLE zHandle,
                     char* szTLEFile ); // > 80 char

int EphemResetTLE( HANDLE zHandle );

int EphemSetTLE( HANDLE zHandle,
                 char* szTLELine1,
                 char* szTLELine2 );
int EphemGetNumTLE( HANDLE zHandle );
int EphemGetTLE( HANDLE zHandle,
                 int iEntry,
                 char* szTLELine1,
                 char* szTLELine2 );

int EphemSetInclination( HANDLE zHandle,
                         double dInclination );
int EphemGetInclination( HANDLE zHandle,
                         double* pdInclination );
int EphemSetArgOfPerigee( HANDLE zHandle,
                          double dArgOfPerigee );
int EphemGetArgOfPerigee( HANDLE zHandle,
                          double* pdArgOfPerigee );
int EphemSetMeanAnomaly( HANDLE zHandle,
                         double dMeanAnomaly );
int EphemGetMeanAnomaly( HANDLE zHandle,
                         double* pdMeanAnomaly );
int EphemSetElementTime( HANDLE zHandle,
                         double dElementTime );
int EphemGetElementTime( HANDLE zHandle,
                         double* pdElementTime );
int EphemSetEccentricity( HANDLE zHandle,
                          double dEccentricity );
int EphemGetEccentricity( HANDLE zHandle,
                          double* pdEccentricity );
int EphemSetRightAscension( HANDLE zHandle,
                            double dRtAscOfAscNode );
int EphemGetRightAscension( HANDLE zHandle,
                            double* pdRtAscOfAscNode );
int EphemSetMeanMotion( HANDLE zHandle,
                        double dMeanMotion );
int EphemGetMeanMotion( HANDLE zHandle,
                        double* pdMeanMotion );
int EphemSetMeanMotion1stDeriv( HANDLE zHandle,
                                double d1stDerivMeanMotion );
int EphemGetMeanMotion1stDeriv( HANDLE zHandle,
                                double* pdMeanMotion1stDeriv );
int EphemSetMeanMotion2ndDeriv( HANDLE zHandle,
                                double d2ndDerivMeanMotion );
int EphemGetMeanMotion2ndDeriv( HANDLE zHandle,
                                double* pdMeanMotion2ndDeriv );
int EphemSetBStar( HANDLE zHandle,
                   double dBStar );
int EphemGetBStar( HANDLE zHandle,
                   double* pdBStar );
int EphemSetAltitudeOfApogee( HANDLE zHandle,
                              double dOrbAltApogee );
int EphemGetAltitudeOfApogee( HANDLE zHandle,
                              double* pdAltApogee );
int EphemSetAltitudeOfPerigee( HANDLE zHandle,
                               double dOrbAltPerigee );
int EphemGetAltitudeOfPerigee( HANDLE zHandle,
                               double* pdAltPerigee );
int EphemSetLocalTimeOfApogee( HANDLE zHandle,
                               double dOrbLocTimeApogee );
int EphemGetLocalTimeOfApogee( HANDLE zHandle,
                               double* pdLocTimeApogee );
int EphemSetLocalTimeMaxInclination( HANDLE zHandle,
                                     double dOrbLocTimeMaxIncl );
int EphemGetLocalTimeMaxInclination( HANDLE zHandle,
                                     double* pdLocTimeMaxIncl );
int EphemSetTimeOfPerigee( HANDLE zHandle,
                           double dTime );
int EphemGetTimeOfPerigee( HANDLE zHandle,
                           double* pdTimeOfPerigee );
int EphemSetSemiMajorAxis( HANDLE zHandle,
                           double dAxisInRe );
int EphemGetSemiMajorAxis( HANDLE zHandle,
                           double* pdSemiMajorAxis );
int EphemSetPositionGEI( HANDLE zHandle,
                         double dPosX,
                         double dPosY,
                         double dPosZ );
int EphemGetPositionGEI( HANDLE zHandle,
                         double* pdPosX,
                         double* pdPosY,
                         double* pdPosZ );
int EphemSetVelocityGEI( HANDLE zHandle,
                         double dVelX,
                         double dVelY,
                         double dVelZ );
int EphemGetVelocityGEI( HANDLE zHandle,
                         double* pdVelX,
                         double* pdVelY,
                         double* pdVelZ );
int EphemSetStateVectors( HANDLE zHandle,
                          double *pvdPos,
                          double *pvdVel );
int EphemGetStateVectors( HANDLE zHandle,
                          double* pvdPos,
                          double* pvdVel );
int EphemSetGeosynchLon( HANDLE zHandle,
                         double dLongitudeDeg );
int EphemGetGeosynchLon( HANDLE zHandle,
                         double* pdGeosynchLon );

int EphemResetOrbitParameters( HANDLE zHandle );

int EphemRestartEphemeris( HANDLE zHandle );

// for EphemComputeEphemeris[GEI]() methods, return >=0 is data count; <0 is error
int EphemComputeEphemerisGEI( HANDLE zHandle,
                              double* pvdTimes, // output!
                              double* pvdXGEI,
                              double* pvdYGEI,
                              double* pvdZGEI,
                              double* pvdXDotGEI,
                              double* pvdYDotGEI,
                              double* pvdZDotGEI );

int EphemComputeEphemeris( HANDLE zHandle,
                           char* szCoordSys,
                           char* szCoordUnits, // input
                           double* pvdTimes, // output!
                           double* pvdCoord1,
                           double* pvdCoord2,
                           double* pvdCoord3 );

int EphemConvertCoordinatesSingle( HANDLE zHandle,
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
int EphemConvertCoordinates( HANDLE zHandle,
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

int EphemSetMainField( HANDLE zHandle,
                       char* szMainField );

int EphemSetExternalField( HANDLE zHandle,
                           char* szExternalField );

int EphemGetMainField( HANDLE zHandle,
                       char* szMainField );

int EphemGetExternalField( HANDLE zHandle,
                           char* szExternalField );

int EphemUpdateTime( HANDLE zHandle,
                     double dTimeMjd );

int EphemSetKpValue( HANDLE zHandle,
                     double dKpVal );
int EphemSetKpValues( HANDLE zHandle,
                      double dRefTime,
                      double* pvdKpVals,
                      int iNumEntries );
int EphemGetKpValue( HANDLE zHandle,
                     double* pdKpVal );
int EphemGetKpValuesRefTime( HANDLE zHandle,
                             double* pdRefTime );
int EphemGetKpValuesEndTime( HANDLE zHandle,
                             double* pdEndTime );

int EphemComputeBfieldSingle( HANDLE zHandle,
                              char* szCoordSys,
                              char* szCoordUnits,
                              double dTime,
                              double dCoord1,
                              double dCoord2,
                              double dCoord3,
                              double* pvdBVecGeo,
                              double* pdBMag,
                              double* pdBMin,
                              double* pdLm );

int EphemComputeBfield( HANDLE zHandle,
                        char* szCoordSys,
                        char* szCoordUnits,
                        double* pvdTimes,
                        double* pvdCoord1,
                        double* pvdCoord2,
                        double* pvdCoord3,
                        int iNumTimes,
                        double* pvvBVecGeo,
                        double* pvdBMag,
                        double* pvdBMin,
                        double* pvdLm );

int EphemComputeInvariantsSingle( HANDLE zHandle,
                                  char* szCoordSys,
                                  char* szCoordUnits,
                                  double dTime,
                                  double dCoord1,
                                  double dCoord2,
                                  double dCoord3,
                                  double* pvdPitchAngles,
                                  int iNumPitchAngles,
                                  double* pdBMin,
                                  double* pvdBMinPosGeo,
                                  double* pvdBVecGeo,
                                  double* pvdLm,
                                  double* pvdI );

int EphemComputeInvariants( HANDLE zHandle,
                            char* szCoordSys,
                            char* szCoordUnits,
                            double* pvdTimes,
                            double* pvdCoord1,
                            double* pvdCoord2,
                            double* pvdCoord3,
                            int iNumTimes,
                            double* pvdPitchAngles,
                            int iNumPitchAngles,
                            double* pvdBMin,
                            double* pvvdBMinPosGeo,
                            double* pvvdBVecGeo,
                            double* pvvdLm,
                            double* pvvdI );

#ifdef __cplusplus
} // extern "C"
#endif

#endif  // CEPHEMMODEL_C_H
