/******************************************************************************
$HeadURL$

 File: CAccumModel_c.h

 Description: Declarations for methods for the collection and management
   of flux data values to be accumulated for the calculation of fluence
   data values.

 Classification:

   Unclassified

 Project Name:

   AE9/AP9/SPM Radiation Environment Models

   Developed under US Government contract # FA9453-12-C-0231

 Rights and Restrictions:

   Copyright 2021 Atmospheric and Environmental Research, Inc. (AER)

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
  1.0          09/29/2017  Created 
  
SVNTag: $Id$
******************************************************************************/

#ifndef ACCUMMODEL_C_H
#define ACCUMMODEL_C_H

#ifdef __cplusplus
#include "CAccumModel.h"

extern "C" {
#endif

typedef int HANDLE;

HANDLE AccumStartUp( );
int AccumShutDown( HANDLE zHandle );

int AccumSetWcLogging( HANDLE zHandle,
                       int iVerdict );

int AccumSetTimeInterval( HANDLE zHandle,
                          double dTimeInterval );

int AccumSetTimeIntervalSec( HANDLE zHandle,
                             double dTimeIntervalSec );

int AccumGetTimeInterval( HANDLE zHandle,
                          double* pdTimeInterval );

int AccumSetTimeIncrement( HANDLE zHandle,
                           double dTimeIncrement );

int AccumGetTimeIncrement( HANDLE zHandle,
                           double* pdTimeIncrement );

int AccumGetBufferDim( HANDLE zHandle,
                       int* piNumTimes,
                       int* piNumValues,
                       int* piNumDir );

int AccumClearBuffer( HANDLE zHandle );

int AccumLoadBuffer( HANDLE zHandle,
                     double* pvdTimes,
                     double* pvvvdData,
                     int iNumTimes,
                     int iNumValues,
                     int iNumDir );
int AccumAddToBuffer( HANDLE zHandle,
                      double dTime,
                      double* pvvdData,
                      int iNumValues,
                      int iNumDir );

int AccumComputeFluence( HANDLE zHandle,
                         double* pvdFluenceTimes,
                         double* pvvvdFluence );
int AccumComputeIntvFluence( HANDLE zHandle,
                             double* pvdIntvFluenceTimes,
                             double* pvvvdIntvFluence,
                             int* pviIntIndices,
                             int iPartial );
int AccumComputeFullFluence( HANDLE zHandle,
                             double* pdFullFluenceTimes,
                             double* pvvvdFullFluence,
                             int iFinal );

int AccumResetFluence( HANDLE zHandle );
int AccumResetIntvFluence( HANDLE zHandle );
int AccumResetFullFluence( HANDLE zHandle );
int AccumResetBoxcarFluence( HANDLE zHandle );
int AccumResetExponentialFlux( HANDLE zHandle );

int AccumGetFluenceStartTime( HANDLE zHandle,
                              double* pdStartTime );
int AccumGetIntvFluenceStartTime( HANDLE zHandle,
                                  double* pdStartTime );
int AccumGetFullFluenceStartTime( HANDLE zHandle,
                                  double* pdStartTime );
int AccumGetBoxcarFluenceStartTime( HANDLE zHandle,
                                    double* pdStartTime );
int AccumGetLastLength( HANDLE zHandle,
                        double* pdLastLength ); // single value

int AccumAccumIntvFluence( HANDLE zHandle,
                           double* pvdIntvFluenceTimes,
                           double* pvvvdIntvFluence,
                           int iNumTimes,
                           int iNumValues,
                           int iNumDir,
                           double* pvvvdIntvFluenceAccum,
                           int iAccumReset );
int AccumComputeAverageFlux( HANDLE zHandle,
                             double* pvdFluenceTimes,
                             double* pvvvdFluence,
                             double dIntervalSec,
                             int iNumTimes,
                             int iNumValues,
                             int iNumDir,
                             double* pvvvdFluxAvg );
int AccumComputeBoxcarFluence( HANDLE zHandle,
                               double* pvdBoxcarFluenceTimes,
                               double* pvvvdBoxcarFluence,
                               int* pviIntIndices,
                               int iPartial );
int AccumComputeExponentialFlux( HANDLE zHandle,
                                 double* pvdExpFluxTimes,
                                 double* pvvvdExpFlux,
                                 int* pviIntIndices,
                                 int iFinal );
int AccumApplyWorstToDate( HANDLE zHandle,
                           double* pvvvdData,
                           int iNumTimes,
                           int iNumValues,
                           int iNumDir,
                           double* pvvdMaxData,
                           double* pvvvdDataWorst,
                           int iReset );

int AccumGetWorstCaseLog( HANDLE zHandle,
                          double* pvdTimes, 
                          int iNumTimes,
                          double* pvdEnergies,
                          int iNumEnergies,
                          double* pvdPitchAngles,
                          int iNumDir, // 0 for omni
                          double* pvvdWorstCaseLog );

int AccumGetWorstCaseLogVarPitch( HANDLE zHandle,
                                  double* pvdTimes, 
                                  int iNumTimes,
                                  double* pvdEnergies,
                                  int iNumEnergies,
                                  double* pvvdPitchAngles,
                                  int iNumDir,
                                  double* pvvdWorstCaseLog );
#ifdef __cplusplus
} // extern "C"
#endif

#endif  // CACCUMMODEL_C_H
