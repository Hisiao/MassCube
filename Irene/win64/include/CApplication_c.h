/******************************************************************************

 File: CApplication_c.h

 Description: Declarations for methods that permit the programmatic interface
   with the Ae9/Ap9/Spm/Legacy radiation belt model calculations.  The model
   run parameters are specified via a collection of 'set' method calls.  The
   actual model calculation run is performed 'behind the scenes'. The generated
   calculation results are accessible (from binary data files) via a collection 
   of 'get' and 'flyin' method calls.

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
  1.0          10/26/2017  Created 
  
******************************************************************************/

#ifndef CAPPLICATION_C_H
#define CAPPLICATION_C_H

#ifdef __cplusplus
#include "CApplication.h"

extern "C" {
#endif

typedef int HANDLE;

HANDLE AppStartUp( );
int AppShutDown( HANDLE zHandle );

  // directory in which temporary directory and files are written/read
  //  when not specified, defaults to the current working directory
int AppSetWorkDir( HANDLE zHandle,
                   char* szWorkDir );
int AppGetWorkDir( HANDLE zHandle,
                   char* szWorkDir );

  // **Required** to be able to execute CmdLine 
  //  and various Executor programs behind-the-scenes
int AppSetExecDir( HANDLE zHandle,
                   char* szExecDir );
int AppGetExecDir( HANDLE zHandle,
                   char* szExecDir );

int AppClearInEphemeris(HANDLE zHandle );
int AppSetInEphemeris( HANDLE zHandle,
                       double* pvdTimes, 
                       double* pvdCoords1,
                       double* pvdCoords2, 
                       double* pvdCoords3,
                       int iNumTimes,
                       int iAppend );
int AppGetNumInEphemeris( HANDLE zHandle ); // returns iNumTimes
int AppGetInEphemeris( HANDLE zHandle,  // returns iNumTimes
                       double* pvdTimes, 
                       double* pvdCoords1,
                       double* pvdCoords2, 
                       double* pvdCoords3 );

int AppResetModelData(HANDLE zHandle );
int AppResetModelRun( HANDLE zHandle,
                      int iDelBinDir,
                      int iResetParam );

// Windows MPI mode for 'mpiexec' launcher extra arguments:
//     Local      add '-localonly'      for use on local Windows machine with multiple processors
//     Hydra      --no extra arg--      for use on Windows cluster, relies on external 'hydra_service' 
//#depr/Intel#     SSH        add '-bootstrap ssh'  for use on Windows cluster, using ssh for MPI communication
int AppSetWindowsMpiMode( HANDLE zHandle,
                          char* szMode );
int AppGetWindowsMpiMode( HANDLE zHandle,
                          char* szMode );

int AppSetModelDBDir( HANDLE zHandle,
                      char* szModelDBDir );
int AppGetModelDBDir( HANDLE zHandle,
                      char* szModelDBDir );

int AppSetCoordSys( HANDLE zHandle,
                    char* szCoordSys,
                    char* szCoordSysUnits );
int AppSetInCoordSys( HANDLE zHandle,
                      char* szCoordSys,
                      char* szCoordSysUnits );
int AppGetCoordSys( HANDLE zHandle,
                    char* szCoordSys );
int AppGetCoordSysUnits( HANDLE zHandle,
                         char* szCoordUnits );
int AppGetInCoordSys( HANDLE zHandle,
                      char* szCoordSys );
int AppGetInCoordSysUnits( HANDLE zHandle,
                           char* szCoordUnits );
// === model invocation ===
int AppRunModel( HANDLE zHandle );
  

// data access
// analogous to computeEphemeris; accesses data in chunks  (returns iNumTimes value; max=chunk size)
int AppGetEphemeris( HANDLE zHandle,
                     double* pvdTimes,
                     double* pvdCoord1,
                     double* pvdCoord2,
                     double* pvdCoord3 );

  // replication of previous version flux calculation access methods: 
    // (all of these methods return iNumTimes value [max=chunk size, but can be zero]; 
    //   the other dimension sizes iNumEnergies and iNumDir will already be known)
    //these are currently configured to provide data in 'chunks' for each call;
    //  --> need to be able to have setting to change to reading entire file...
    // AccumAvg argument used to access the specific accumulations/averages
    //  (Intv,Full,Boxcar,Expon); use Cumul or Default for the straight flux values
    // iAccumIntvId argument used to specify the accumulation interval (1-N),
    //    where N=AppGetNumAccumIntervals(Handle); use 1 for default
int AppFlyinMean2D( HANDLE zHandle,
                    double* pvvdFluxData,  // unidirectional; will also get Legacy results
                    char* szAccumMode,// "default"="cumul"|"full"|"interval"|"boxcar"|"expon"
                    int iAccumIntvId );
int AppFlyinMean( HANDLE zHandle,
                  double* pvvvdFluxData,  // will also get Legacy results
                  char* szAccumMode,// "default"="cumul"|"full"|"interval"|"boxcar"|"expon"
                  int iAccumIntvId );
int AppFlyinMeanPlus( HANDLE zHandle,
                      double* pvdTimes,   // also get time, ephemeris and pitch angle data
                      double* pvdCoord1, 
                      double* pvdCoord2, 
                      double* pvdCoord3,
                      double* pvvdPitchAngles,
                      double* pvvvdFluxData, 
                      char* szAccumMode,// "default"="cumul"|"full"|"interval"|"boxcar"|"expon"
                      int iAccumIntvId );

int AppFlyinPercentile( HANDLE zHandle,
                        int iPercentile,
                        double* pvvvdFluxData,
                        char* szAccumMode,// "default"="cumul"|"full"|"interval"|"boxcar"|"expon"
                        int iAccumIntvId );
int AppFlyinPercentilePlus( HANDLE zHandle,
                            int iPercentile,
                            double* pvdTimes, 
                            double* pvdCoord1, 
                            double* pvdCoord2, 
                            double* pvdCoord3,
                            double* pvvdPitchAngles,
                            double* pvvvdFluxData,
                            char* szAccumMode,// "default"="cumul"|"full"|"interval"|"boxcar"|"expon"
                            int iAccumIntvId );
int AppFlyinPerturbedMean( HANDLE zHandle,
                           int iScenario,
                           double* pvvvdFluxData,
                           char* szAccumMode,// "default"="cumul"|"full"|"interval"|"boxcar"|"expon"
                           int iAccumIntvId );
int AppFlyinPerturbedMeanPlus( HANDLE zHandle,
                               int iScenario,
                               double* pvdTimes, 
                               double* pvdCoord1, 
                               double* pvdCoord2, 
                               double* pvdCoord3,
                               double* pvvdPitchAngles,
                               double* pvvvdFluxData,
                               char* szAccumMode,// "default"="cumul"|"full"|"interval"|"boxcar"|"expon"
                               int iAccumIntvId );
   // replacement for previous version 'flyinScenario' method;
   //  (additional arguments that it used are not relevant here)
int AppFlyinMonteCarlo( HANDLE zHandle,
                        int iScenario,
                        double* pvvvdFluxData,
                        char* szAccumMode,
                        int iAccumIntvId );
int AppFlyinMonteCarloPlus( HANDLE zHandle,
                            int iScenario,
                            double* pvdTimes, 
                            double* pvdCoord1, 
                            double* pvdCoord2, 
                            double* pvdCoord3,
                            double* pvvdPitchAngles,
                            double* pvvvdFluxData,
                            char* szAccumMode,// "default"="cumul"|"full"|"interval"|"boxcar"|"expon"
                            int iAccumIntvId );

int AppGetAdiabaticCoords( HANDLE zHandle,
                           double* pvvdAlpha,
                           double* pvvdLm,
                           double* pvvdK,
                           double* pvvdPhi,
                           double* pvvdHmin,
                           double* pvvdLstar,
                           double* pvdBmin, 
                           double* pvdBlocal, 
                           double* pvdMagLT ); 
int AppGetAdiabaticCoordsPlus( HANDLE zHandle,
                               double* pvdTimes, 
                               double* pvdCoord1, 
                               double* pvdCoord2, 
                               double* pvdCoord3,
                               double* pvvdPitchAngles,
                               double* pvvdAlpha,
                               double* pvvdLm,
                               double* pvvdK,
                               double* pvvdPhi,
                               double* pvvdHmin,
                               double* pvvdLstar,
                               double* pvdBmin, 
                               double* pvdBlocal, 
                               double* pvdMagLT ); 

int AppGetModelData( HANDLE zHandle,
                     char* szDataType, // "flux"|"fluence"|"doserate"|"doseaccum"
                     char* szFluxMode, // "mean"|"percent"|"perturbed"|"montecarlo"|"adiabat"(only w/ flux)
                     int iCalcVal,     //   -1  |  1-99   |  1-999    |  1-999     |  -2
                     double* pvdTimes, 
                     double* pvdCoord1, 
                     double* pvdCoord2, 
                     double* pvdCoord3,
                     double* pvvdPitchAngles, // will always be empty for dose data
                     double* pvvvdData,
                     char* szAccumMode, // "default"|"cumul"|"full"|"interval"|"boxcar"|"expon"|("undefined"<-adiabat)
                     int iAccumIntvId );

int AppGetAggregData( HANDLE zHandle,
                      char* szDataType, // "flux"|"fluence"|"doserate"|"doseaccum"
                      char* szFluxMode, // "perturbed"|"montecarlo"
                      int iPercent,     //  0-100
                      double* pvdTimes, 
                      double* pvdCoord1, 
                      double* pvdCoord2, 
                      double* pvdCoord3,
                      double* pvvdPitchAngles, // will always be empty for dose data
                      double* pvvvdData,
                      char* szAccumMode, // "default"|"cumul"|"full"|"interval"|"boxcar"|"expon"
                      int iAccumIntvId );

  // setting for optionally retaining binary directory and its contents
  // by default, the directory is cleared and removed within 'resetModel'
int AppSetDelBinDir( HANDLE zHandle,
                     int iVerdict );
int AppGetDelBinDir( HANDLE zHandle );
  
int AppSetBinDirName( HANDLE zHandle,
                      char* szBinDirName );
int AppGetBinDirName( HANDLE zHandle,
                      char* szBinDirName );

//  ==== incorporate CInputParameters c wrappers here

int AppResetParameters( HANDLE zHandle );
int AppResetOrbitParameters( HANDLE zHandle );
  
int AppValidateParameters( HANDLE zHandle );

int AppSetNumProc( HANDLE zHandle,
                   int iNumProc );
int AppGetNumProc( HANDLE zHandle );
int AppSetNumFileIo( HANDLE zHandle,
                     int iNumProc );
int AppGetNumFileIo( HANDLE zHandle );
int AppSetChunkSize( HANDLE zHandle,
                     int iChunkSize );
int AppGetChunkSize( HANDLE zHandle );
int AppSetTaskDelay( HANDLE zHandle,
                     int iTaskDelay );
int AppGetTaskDelay( HANDLE zHandle );
int AppSetModel( HANDLE zHandle,
                 char* szModel );
int AppGetModel( HANDLE zHandle,
                 char* szModel );
int AppSetModelDBFile( HANDLE zHandle,
                       char* szDataSource );
int AppGetModelDBFile( HANDLE zHandle,
                       char* szDataSource );
int AppSetKPhiDBFile( HANDLE zHandle,
                      char* szDataSource );
int AppGetKPhiDBFile( HANDLE zHandle,
                      char* szDataSource );
int AppSetKHMinDBFile( HANDLE zHandle,
                       char* szDataSource );
int AppGetKHMinDBFile( HANDLE zHandle,
                       char* szDataSource );
int AppSetMagfieldDBFile( HANDLE zHandle,
                          char* szDataSource );
int AppGetMagfieldDBFile( HANDLE zHandle,
                         char* szDataSource );
int AppSetDoseModelDBFile( HANDLE zHandle,
                           char* szDataSource );
int AppGetDoseModelDBFile( HANDLE zHandle,
                           char* szDataSource );

int AppAddExtraInfo( HANDLE zHandle,
                     char* szExtraInfo );
int AppGetNumExtraInfo( HANDLE zHandle );
int AppClearExtraInfo( HANDLE zHandle );

// ephemeris
int AppSetTimesList( HANDLE zHandle,
                     double* pvdTimes,
                     int iNumTimes );
int AppGetNumTimesList( HANDLE zHandle );  // return iNumTimes
int AppGetTimesList( HANDLE zHandle,       // return iNumTimes
                     double* pvdTimes );
int AppClearTimesList( HANDLE zHandle );
int AppSetTimes( HANDLE zHandle,
                 double dStartTime, 
                 double dEndTime, 
                 double dTimeStepSec );
int AppGetTimes( HANDLE zHandle,
                 double* pdStartTime, 
                 double* pdEndTime, 
                 double* pdTimeStepSec );
int AppSetVarTimes( HANDLE zHandle,
                    double dStartTime, 
                    double dEndTime, 
                    double dTimeMinStepSec,
                    double dTimeMaxStepSec,
                    double dTimeRoundSec );
int AppGetVarTimes( HANDLE zHandle,
                    double* pdStartTime, 
                    double* pdEndTime, 
                    double* pdTimeMinStepSec,
                    double* pdTimeMaxStepSec,
                    double* pdTimeRoundSec );
int AppSetPropagator( HANDLE zHandle,
                      char* szPropSpec );
int AppGetPropagator( HANDLE zHandle,
                      char* szPropSpec );
int AppSetSGP4Param( HANDLE zHandle,
                     char* szMode, 
                     char* szWGS );
int AppSetSGP4Mode( HANDLE zHandle,
                    char* szMode );
int AppGetSGP4Mode( HANDLE zHandle,
                    char* szMode );
int AppSetSGP4Datum( HANDLE zHandle,
                     char* szWGS );
int AppGetSGP4Datum( HANDLE zHandle,
                     char* szWGS );
int AppSetKeplerUseJ2( HANDLE zHandle,
                       int iUseJ2 );
int AppGetKeplerUseJ2( HANDLE zHandle );
int AppSetTLEFile( HANDLE zHandle,
                   char* szTLEFile );
int AppGetTLEFile( HANDLE zHandle,
                   char* szTLEFile );
int AppClearTLEFile( HANDLE zHandle );
int AppSetTimesFile( HANDLE zHandle,
                     char* szTimesFile );
int AppGetTimesFile( HANDLE zHandle,
                     char* szTimesFile );
int AppSetElementTime( HANDLE zHandle,
                       double dElementTime );
int AppGetElementTime( HANDLE zHandle,
                       double* pdElementTime );
int AppSetInclination( HANDLE zHandle,
                       double dInclination );
int AppGetInclination( HANDLE zHandle,
                       double* pdInclination );
int AppSetRightAscension( HANDLE zHandle,
                          double dRtAscOfAscNode );
int AppGetRightAscension( HANDLE zHandle,
                          double* pdRtAscOfAscNode );
int AppSetEccentricity( HANDLE zHandle,
                        double dEccentricity );
int AppGetEccentricity( HANDLE zHandle,
                        double* pdEccentricity );
int AppSetArgOfPerigee( HANDLE zHandle,
                        double dArgOfPerigee );
int AppGetArgOfPerigee( HANDLE zHandle,
                        double* pdArgOfPerigee );
int AppSetMeanAnomaly( HANDLE zHandle,
                       double dMeanAnomaly );
int AppGetMeanAnomaly( HANDLE zHandle,
                       double* pdMeanAnomaly );
int AppSetMeanMotion( HANDLE zHandle,
                      double dMeanMotion );
int AppGetMeanMotion( HANDLE zHandle,
                      double* pdMeanMotion );
int AppSetMeanMotion1stDeriv( HANDLE zHandle,
                              double dMeanMotion1stDeriv );
int AppGetMeanMotion1stDeriv( HANDLE zHandle,
                              double* pdMeanMotion1stDeriv );
int AppSetMeanMotion2ndDeriv( HANDLE zHandle,
                              double dMeanMotion2ndDeriv );
int AppGetMeanMotion2ndDeriv( HANDLE zHandle,
                              double* pdMeanMotion2ndDeriv );
int AppSetBStar( HANDLE zHandle,
                 double dBStar );
int AppGetBStar( HANDLE zHandle,
                 double* pdBStar );
int AppSetAltitudeOfApogee( HANDLE zHandle,
                            double dAltApogee );
int AppGetAltitudeOfApogee( HANDLE zHandle,
                            double* pdAltApogee );
int AppSetAltitudeOfPerigee( HANDLE zHandle,
                             double dAltPerigee );
int AppGetAltitudeOfPerigee( HANDLE zHandle,
                             double* pdAltPerigee );
int AppSetLocalTimeOfApogee( HANDLE zHandle,
                             double dLocTimeApogee );
int AppGetLocalTimeOfApogee( HANDLE zHandle,
                             double* pdLocTimeApogee );
int AppSetLocalTimeMaxInclination( HANDLE zHandle,
                                   double dLocTimeMaxIncl );
int AppGetLocalTimeMaxInclination( HANDLE zHandle,
                                   double* pdLocTimeMaxIncl );
int AppSetTimeOfPerigee( HANDLE zHandle,
                         double dTimeOfPerigee );
int AppGetTimeOfPerigee( HANDLE zHandle,
                         double* pdTimeOfPerigee );
int AppSetSemiMajorAxis( HANDLE zHandle,
                         double dSemiMajorAxis );
int AppGetSemiMajorAxis( HANDLE zHandle,
                         double* pdSemiMajorAxis );
int AppSetGeosynchLon( HANDLE zHandle,
                       double dGeosynchLon );
int AppGetGeosynchLon( HANDLE zHandle,
                       double* pdGeosynchLon );
int AppSetPositionGEI( HANDLE zHandle,
                       double dPosX,
                       double dPosY,
                       double dPosZ );
int AppSetVelocityGEI( HANDLE zHandle,
                       double dVelX,
                       double dVelY,
                       double dVelZ );
int AppGetPositionGEI( HANDLE zHandle,
                       double* pdPosX,
                       double* pdPosY,
                       double* pdPosZ );
int AppGetVelocityGEI( HANDLE zHandle,
                       double* pdVelX,
                       double* pdVelY,
                       double* pdVelZ );
int AppSetStateVectors( HANDLE zHandle,
                        double* pvdPosition,
                        double* pvdVelocity );
int AppGetStateVectors( HANDLE zHandle,  
                        double* pvdPosition,
                        double* pvdVelocity );

// model parameters
int AppSetPitchAngle( HANDLE zHandle,
                      double dPitchAngle );
int AppSetPitchAngles( HANDLE zHandle,
                       double* pvdPitchAngles,
                       int iNumPitch );
int AppGetNumPitchAngles( HANDLE zHandle );
int AppGetPitchAngles( HANDLE zHandle,
                       double* pvdPitchAngles );
int AppClearPitchAngles( HANDLE zHandle );

int AppSetAdiabatic( HANDLE zHandle,
                     int iVerdict );
int AppGetAdiabatic( HANDLE zHandle );

int AppGetNumDir( HANDLE zHandle );

int AppSetFluxType( HANDLE zHandle,
                    char* szFluxType );
int AppGetFluxType( HANDLE zHandle,
                    char* szFluxType );
int AppClearFluxEnergies( HANDLE zHandle );
int AppSetFluxEnergy( HANDLE zHandle,
                      double dEnergy );
int AppSetFluxEnergies( HANDLE zHandle,
                        double* pvdEnergies,
                        int iNumEnergies );
int AppGetNumFluxEnergies( HANDLE zHandle );
int AppGetFluxEnergies( HANDLE zHandle,
                        double* pvdEnergies );
int AppClearFluxEnergies2( HANDLE zHandle );
int AppSetFluxEnergy2( HANDLE zHandle,
                       double dEnergy );
int AppSetFluxEnergies2( HANDLE zHandle,
                         double* pvdEnergies2,
                         int iNumEnergies );
int AppGetNumFluxEnergies2( HANDLE zHandle );
int AppGetFluxEnergies2( HANDLE zHandle,
                         double* pvdEnergies2 );

// data mode
int AppSetFluxMean( HANDLE zHandle,
                    int iVerdict );
int AppGetFluxMean( HANDLE zHandle );

int AppSetFluxPercentile( HANDLE zHandle,
                          int iPercent );
int AppSetFluxPercentiles( HANDLE zHandle,
                           int* pviPercent,
                           int iNumPerc );
int AppClearFluxPercentiles( HANDLE zHandle );
int AppGetNumFluxPercentiles( HANDLE zHandle );
int AppGetFluxPercentiles( HANDLE zHandle,
                           int* pviPercent );

int AppSetFluxPerturbedScenario( HANDLE zHandle,
                                 int iScenario );
int AppSetFluxPerturbedScenarios( HANDLE zHandle,
                                  int* pviScenario,
                                  int iNumScen );
int AppSetFluxPerturbedScenRange( HANDLE zHandle,
                                  int iScenarioMin, 
                                  int iScenarioMax );
int AppClearFluxPerturbedScenarios( HANDLE zHandle );
int AppGetNumFluxPerturbedScenarios( HANDLE zHandle ); 
int AppGetFluxPerturbedScenarios( HANDLE zHandle,
                                  int* pviScenario );

int AppSetFluxMonteCarloScenario( HANDLE zHandle,
                                  int iScenario );
int AppSetFluxMonteCarloScenarios( HANDLE zHandle,
                                   int* viScenario,
                                   int iNumScen );
int AppSetFluxMonteCarloScenRange( HANDLE zHandle,
                                   int iScenarioMin, 
                                   int iScenarioMax );
int AppClearFluxMonteCarloScenarios( HANDLE zHandle );
int AppGetNumFluxMonteCarloScenarios(HANDLE zHandle);
int AppGetFluxMonteCarloScenarios( HANDLE zHandle,
                                   int* pviScenario );
int AppSetMonteCarloEpochTime( HANDLE zHandle,
                               double dMCEpochTime );
int AppGetMonteCarloEpochTime( HANDLE zHandle,
                               double* pdMCEpochTime );
int AppSetMonteCarloFluxPerturb( HANDLE zHandle,
                                 int iVerdict );
int AppGetMonteCarloFluxPerturb( HANDLE zHandle );
int AppSetMonteCarloWorstCase( HANDLE zHandle,
                               int iVerdict );
int AppGetMonteCarloWorstCase( HANDLE zHandle );
int AppSetReportTimes( HANDLE zHandle,
                       double dRefTime, // MJD
                       double dDeltaDays ); // days
int AppSetReportTimesSec( HANDLE zHandle,
                          double dRefTime, // MJD
                          double dDeltaSec ); // seconds
int AppClearReportTimes( HANDLE zHandle );
int AppGetNumReportTimes( HANDLE zHandle );
int AppGetReportTimesSec( HANDLE zHandle,
                          double* pvdReportRef,
                          double* pvdReportDelta );
int AppSetReportAtTime( HANDLE zHandle,
                        double dReportAtTime );// MJD
int AppClearReportAtTime( HANDLE zHandle );
int AppGetNumReportAtTime( HANDLE zHandle );
int AppGetReportAtTime( HANDLE zHandle,
                        double* pvdReportAtTime );

// data generation
int AppSetFluence( HANDLE zHandle,
                   int iVerdict );
int AppGetFluence( HANDLE zHandle );
int AppSetDoseRate( HANDLE zHandle,
                    int iVerdict );
int AppGetDoseRate( HANDLE zHandle );
int AppSetDoseAccum( HANDLE zHandle,
                     int iVerdict );
int AppGetDoseAccum( HANDLE zHandle );

// dose
int AppSetUseDoseKernel( HANDLE zHandle,
                         int iVerdict );
int AppGetUseDoseKernel( HANDLE zHandle );
int AppSetDoseKernelDir( HANDLE zHandle,
                         char* szDataSource );
int AppGetDoseKernelDir( HANDLE zHandle ,
                         char* szDataSource );
int AppSetDoseKernelFile( HANDLE zHandle,
                          char* szDataSource );
int AppGetDoseKernelFile( HANDLE zHandle ,
                          char* szDataSource );
int AppSetDoseDepthUnits( HANDLE zHandle,
                          char* szDepthUnits );
int AppGetDoseDepthUnits( HANDLE zHandle,
                          char* szDepthUnits ); 
int AppSetDoseDepthValues( HANDLE zHandle,
                           double* pvdDepths, 
                           int iNumDepths );
int AppGetNumDoseDepthValues( HANDLE zHandle ); 
int AppGetDoseDepthValues( HANDLE zHandle,
                           double* pvdDepths );
int AppSetDoseDepths( HANDLE zHandle,
                      double* pvdDepths, 
                      int iNumDepths,
                      char* szDepthUnits );
int AppSetDoseDetector( HANDLE zHandle,
                        char* szDetector );
int AppGetDoseDetector( HANDLE zHandle,
                        char* szDetector );
int AppSetDoseGeometry( HANDLE zHandle,
                        char* szGeometry );
int AppGetDoseGeometry( HANDLE zHandle,
                        char* szGeometry );
int AppSetDoseNuclearAttenMode( HANDLE zHandle,
                                char* szNucAttenMode );
int AppGetDoseNuclearAttenMode( HANDLE zHandle,
                                char* szNucAttenMode );
int AppSetDoseWithBrems( HANDLE zHandle,
                         int iVerdict );
int AppGetDoseWithBrems( HANDLE zHandle );

// accumulation
int AppSetAccumMode( HANDLE zHandle,
                     char* szAccumMode );
int AppClearAccumModes( HANDLE zHandle );
int AppGetNumAccumModes( HANDLE zHandle );
int AppGetAccumMode( HANDLE zHandle,  // first list entry
                     char* szAccumMode );
int AppGetAccumModeEntry( HANDLE zHandle,
                          int iIdent,  // specific list entry
                          char* szAccumMode );
int AppSetAccumInterval( HANDLE zHandle,
                         double dVal ); // units = days
int AppSetAccumIntervalSec( HANDLE zHandle,
                            double dVal ); // units = seconds
int AppClearAccumIntervals( HANDLE zHandle );
int AppGetAccumIntervalSec( HANDLE zHandle,  // first list entry
                            double* pdAccumSec );
int AppGetAccumIntervalSecEntry( HANDLE zHandle,
                                 int iIdent,  // specific list entry
                                 double* pdAccumSec );
int AppGetNumAccumIntervals( HANDLE zHandle );
int AppSetAccumIncrementSec( HANDLE zHandle,
                             double dVal ); // units = seconds
int AppGetAccumIncrementSec( HANDLE zHandle,
                             double* pdIncrementSec );
int AppSetAccumIncrementFrac( HANDLE zHandle,
                              double dVal );
int AppGetAccumIncrementFrac( HANDLE zHandle,
                              double* pdIncrementFrac );
int AppSetAggregMean( HANDLE zHandle );
int AppSetAggregMedian( HANDLE zHandle );
int AppSetAggregConfLevel( HANDLE zHandle,
                           int iPercent );
int AppSetAggregConfLevels( HANDLE zHandle,
                            int* pviPercent,
                            int iNumConf );
int AppClearAggregConfLevels( HANDLE zHandle );
int AppGetNumAggregConfLevels( HANDLE zHandle );
int AppGetAggregConfLevels( HANDLE zHandle,
                            int* pviPercent );

// legacy model-specific parameters
int AppSetLegActivityLevel( HANDLE zHandle,
                            char* szActivityLevel );
int AppGetLegActivityLevel( HANDLE zHandle,
                            char* szActivityLevel );
int AppSetLegActivityRange( HANDLE zHandle,
                            char* szActivityRange );
int AppGetLegActivityRange( HANDLE zHandle,
                            char* szActivityRange );
int AppSetLeg15DayAvgAp( HANDLE zHandle,
                         double d15DayAvgAp );
int AppGetLeg15DayAvgAp( HANDLE zHandle,
                         double* pd15DayAvgAp );
int AppSetLegFixedEpoch( HANDLE zHandle,
                         int iVerdict );
int AppGetLegFixedEpoch( HANDLE zHandle );
int AppSetLegShiftSAA( HANDLE zHandle,
                         int iVerdict );
int AppGetLegShiftSAA( HANDLE zHandle );

// cammice model-specific parameters
int AppSetCamMagfieldModel( HANDLE zHandle,
                            char* szMFModel );
int AppGetCamMagfieldModel( HANDLE zHandle,
                            char* szMFModel );
int AppSetCamDataFilter( HANDLE zHandle,
                         char* szDataFilter );
int AppGetCamDataFilter( HANDLE zHandle,
                         char* szDataFilter );
int AppSetCamPitchAngleBin( HANDLE zHandle,
                            char* szPitchAngleBin );
int AppGetCamPitchAngleBin( HANDLE zHandle,
                            char* szPitchAngleBin );
int AppSetCamSpecies( HANDLE zHandle,
                      char* szSpecies );
int AppGetCamSpecies( HANDLE zHandle,
                      char* szSpecies );


// utilities to convert *to* MJD values
double AppGetGmtSeconds( HANDLE zHandle,                
                         int iHours, // 0-23
                         int iMinutes, // 0-59
                         double dSeconds ); // 0-59.999
int AppGetDayOfYear( HANDLE zHandle,              
                     int iYear, // 1950-2049
                     int iMonth, // 1-12
                     int iDay ); // 1-28/29/30/31
double AppGetModifiedJulianDate( HANDLE zHandle,                  
                                 int iYear, // 1950-2049
                                 int iDdd,  // 1-365/366
                                 double dGmtsec ); // 0-86399.999
double AppGetModifiedJulianDateUnix( HANDLE zHandle,              
                                     int iUnixTime ); // 0-MAXINT     

// utilities to convert *from* MJD values
int AppGetDateTime( HANDLE zHandle,                            
                    double dModJulDate, // 33282.0 - 69806.999
                    int* piYear, // 1950-2049
                    int* piDdd, // 1-365/366
                    double* pdGmtsec ); // 0-86399.999
int AppGetHoursMinSec( HANDLE zHandle,                  
                       double dGmtsec, // 0-86399.999
                       int* piHours, // 0-23
                       int* piMinutes, // 0-59
                       double* pdSeconds ); // 0-59.999
int AppGetMonthDay( HANDLE zHandle,
                    int iYear, // 1950-2049
                    int iDdd, // 1-365/366
                    int* piMonth, // 1-12
                    int* piDay ); // 1-28/29/30/31

#ifdef __cplusplus
} // extern "C"
#endif

#endif  // CAPPLICATION_C_H
