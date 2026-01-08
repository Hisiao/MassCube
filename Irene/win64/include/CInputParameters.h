/******************************************************************************

 File: CInputParameters.h

 Description: Declaration for methods for the aggregation of all input parameter 
   values that describes all aspects of a sequence of model calculations.

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
  1.0          09/18/2015  Created 
  
******************************************************************************/

#ifndef CINPUTPARAMETERS_H
#define CINPUTPARAMETERS_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

#include "VectorTypes.h"
#include "DataBaseNames.h"

// set limits to 'extra info' text lines specified by user
// -> line length 255, number of lines 16: both arbitrary, but seem reasonable
#define MAX_EXTRA_INFO_LINES 16
#define MAX_EXTRA_INFO_LENGTH 255

class InputParameters
{
public:
  InputParameters( );
  virtual ~InputParameters( );

  void resetParameters( );
  void resetOrbitParameters( );
  
  int validateParameters( );

/*------------------------------------------------------------------*/
// function InputParameters::setNumProc
/**
*
* Specifies the total number of processors to be used for the execution of the
* model calculations.  For parallelized processing, this number *includes* one 
* processor for the 'controller' node (so >= 3).  
*
*    @param iNumProc
*           int: number of processors
*    @param strKey
*           string: parameter/key identifier name for error messages
*
*
*    @return int
*           0: success, >0: number of parameter errors, <0: file open error
*/
  int setNumProc( const int& iNumProc,
                  const string& strKey="setNumProc" );
  int getNumProc( ) { return m_iNumProcInput; }
  int setNumFileIo( const int& iNumFileIo,
                    const string& strKey="setNumFileIo" );
  int getNumFileIo( ) { return m_iNumFileIo; }
  int setChunkSize( const int& iChunkSize,
                    const string& strKey="setChunkSize" );
  int getChunkSize( ) { return m_iChunkSize; }
  int setTaskDelay( const int& iTaskDelay,
                    const string& strKey="setTaskDelay" );
  int getTaskDelay( ) { return m_iTaskDelay; }
  int setModel( const string& strModel,
                const string& strKey="setModel" );
  string getModel( ) { return m_strModel; }
  bool getLegacy( ) { return m_bLegacy; }
  int setModelDBDir( const string& strModelDBDir,
                     const string& strKey="setModelDBDir" );
  string getModelDBDir( ) { return m_strModelDBDir; }
  int setModelDBFile( const string& strDataSource,
                      const string& strKey="setModelDBFile" );
  string getModelDBFile( ) { return m_strModelDBFile; }
  int setKPhiDBFile( const string& strDataSource,
                     const string& strKey="setKPhiDBFile" );
  string getKPhiDBFile( ) { return m_strKPhiDBFile; }
  int setKHMinDBFile( const string& strDataSource,
                      const string& strKey="setKHMinDBFile" );
  string getKHMinDBFile( ) { return m_strKHMinDBFile; }
  int setMagfieldDBFile( const string& strDataSource,
                         const string& strKey="setMagfieldDBFile" );
  string getMagfieldDBFile( ) { return m_strMagFieldDBFile; }
  int setDoseModelDBFile( const string& strDataSource,
                          const string& strKey="setDoseModelDBFile" );
  string getDoseModelDBFile( ) { return m_strDoseModelDBFile; }
  int setExtraInfo( const vector<string>& vstrExtraInfo,
                    const string& strKey="setExtraInfo" );
  int addExtraInfo( const string& strExtraInfo,
                    const string& strKey="addExtraInfo" );
  int getNumExtraInfo( ) { return int(m_vstrExtraInfo.size()); }
  void clearExtraInfo( ) { m_vstrExtraInfo.clear(); }
  int setTimesList( const dvector& vdTimes );
  int getNumTimesList( ) { return int(m_vdEphemTimes.size()); }
  void getTimesList( dvector& vdTimes ) { vdTimes = m_vdEphemTimes; }
  void clearTimesList( ) { m_vdEphemTimes.clear(); }
  int setTimes( const double& dStartTime, 
                const double& dEndTime, 
                const double& dTimeStepSec );
  void getTimes( double& dStartTime, 
                 double& dEndTime, 
                 double& dTimeStepSec );
  int setVarTimeStep( const ivector& viVarTimes,
                      const string& strKey );
  int setVarTimes( const double& dStartTime, 
                   const double& dEndTime, 
                   const double& dTimeMinStepSec,
                   const double& dTimeMaxStepSec = 3600.0,
                   const double& dTimeRoundSec = 5.0 );
  void getVarTimes( double& dStartTime, 
                    double& dEndTime, 
                    double& dTimeMinStepSec,
                    double& dTimeMaxStepSec,
                    double& dTimeRoundSec );
  int setPropagator( const string& strPropSpec,
                     const string& strKey="setPropagator" );
  string getPropagator( ) { return m_strOrbitProp; }
  string getPropagatorAndMode( );
  int setSGP4Param( const string& strMode, 
                    const string& strWGS );
  int setSGP4Mode( const string& strMode,
                   const string& strKey="setSGP4Mode" );
  string getSGP4Mode( ) { return m_strSGP4Mode; }
  int setSGP4Datum( const string& strWGS,
                    const string& strKey="setSGP4Datum" );
  string getSGP4Datum( ) { return m_strSGP4Datum; }
  void setKeplerUseJ2( bool bUseJ2 ) { m_bKeplerJ2 = bUseJ2; }
  bool getKeplerUseJ2( ) { return m_bKeplerJ2; }
  int setTLEFile( const string& strTLEFile,
                  const string& strKey="setTLEFile" );
  string getTLEFile( ) { return m_strTLEFile; }
  void clearTLEFile( ) { setTLEFile( "", "clearTLEFile" ); }
  int setElementTime( const double& dElementTime,
                      const string& strKey="setElementTime" );
  double getElementTime( ) { return m_dElementTime; }
  int setInclination( const double& dInclination,
                      const string& strKey="setInclination" );
  double getInclination( ) { return m_dInclination; }
  int setRightAscension( const double& dRtAscOfAscNode,
                         const string& strKey="setRightAscension" );
  double getRightAscension( ) { return m_dRtAscOfAscNode; }
  int setEccentricity( const double& dEccentricity,
                       const string& strKey="setEccentricity" );
  double getEccentricity( ) { return m_dEccentricity; }
  int setArgOfPerigee( const double& dArgOfPerigee,
                       const string& strKey="setArgOfPerigee" );
  double getArgOfPerigee( ) { return m_dArgOfPerigee; }
  int setMeanAnomaly( const double& dMeanAnomaly,
                      const string& strKey="setMeanAnomaly" );
  double getMeanAnomaly( ) { return m_dMeanAnomaly; }
  int setMeanMotion( const double& dMeanMotion,
                     const string& strKey="setMeanMotion" );
  double getMeanMotion( ) { return m_dMeanMotion; }
  int setMeanMotion1stDeriv( const double& dMeanMotion1stDeriv,
                             const string& strKey="setMeanMotion1stDeriv" );
  double getMeanMotion1stDeriv( ) { return m_dMeanMotion1stDeriv; }
  int setMeanMotion2ndDeriv( const double& dMeanMotion2ndDeriv,
                             const string& strKey="setMeanMotion2ndDeriv" );
  double getMeanMotion2ndDeriv( ) { return m_dMeanMotion2ndDeriv; }
  int setBStar( const double& dBStar,
                const string& strKey="setBStar" );
  double getBStar( ) { return m_dBStar; }
  int setAltitudeOfApogee( const double& dAltApogee,
                           const string& strKey="setAltitudeOfApogee" );
  double getAltitudeOfApogee( ) { return m_dAltApogee; }
  int setAltitudeOfPerigee( const double& dAltPerigee,
                            const string& strKey="setAltitudeOfPerigee" );
  double getAltitudeOfPerigee( ) { return m_dAltPerigee; }
  int setLocalTimeOfApogee( const double& dLocTimeApogee,
                            const string& strKey="setLocalTimeOfApogee" );
  double getLocalTimeOfApogee( ) { return m_dLocTimeApogee; }
  int setLocalTimeMaxInclination( const double& dLocTimeMaxIncl,
                                  const string& strKey="setLocalTimeMaxInclination" );
  double getLocalTimeMaxInclination( ) { return m_dLocTimeMaxIncl; }
  int setTimeOfPerigee( const double& dTimeOfPerigee,
                        const string& strKey="setTimeOfPerigee" );
  double getTimeOfPerigee( ) { return m_dTimeOfPerigee; }
  int setSemiMajorAxis( const double& dSemiMajorAxis,
                        const string& strKey="setSemiMajorAxis" );
  double getSemiMajorAxis( ) { return m_dSemiMajorAxis; }
  int setGeosynchLon( const double& dGeosynchLon,
                      const string& strKey="setGeosynchLon" );
  double getGeosynchLon( ) { return m_dGeosynchLon; }
  int setPositionGEI( const double& dX, 
                      const double& dY, 
                      const double& dZ,
                      const string& strKey="setPositionGEI" );
  int getPositionGEI( double& dX, double& dY, double& dZ );
  int setVelocityGEI( const double& dXdot, 
                      const double& dYdot, 
                      const double& dZdot,
                      const string& strKey="setVelocityGEI" );
  int getVelocityGEI( double& dXdot, double& dYdot, double& dZdot );
  int setStateVectors( const dvector& vdStatePos,
                       const dvector& vdStateVel );
  void getStateVectors( dvector& vdStatePos,
                        dvector& vdStateVel )
                      { vdStatePos = m_vdStatePos; vdStateVel = m_vdStateVel; }
    
  int setOrbitFile( const string& strOrbitFile,
                    const string& strKey="setOrbitFile" );
  string getOrbitFile( ) { return m_strOrbitFile; }
  int setTimesFile( const string& strEphemTimesFile,
                         const string& strKey="setTimesFile" );
  string getTimesFile( ) { return m_strEphemTimesFile; }
  int setPitchAngleFile( const string& strPitchAngleFile,
                         const string& strKey="setPitchAngleFile" );
  string getPitchAngleFile( ) { return m_strPitchAngleFile; }
  int setDirectionFile( const string& strDirectionFile,
                        const string& strKey="setDirectionFile" );
  string getDirectionFile( ) { return m_strDirectionFile; }

  int setPitchAngle( const double& dPitchAngle=-999.9, //default clears list
                     const string& strKey="setPitchAngle" );
  int setPitchAngles( const dvector& vdPitchAngles,
                      const string& strKey="setPitchAngles" );
  int clearPitchAngles( );
  int getNumPitchAngles( ) { return int(m_vdPitchAngles.size()); }
  void getPitchAngles( dvector& vdPitchAngles )
    { vdPitchAngles = m_vdPitchAngles; }
  int setCoordSys( const string& strCoordSys,
                   const string& strCoordSysUnits );
  int setInCoordSys( const string& strCoordSys,
                     const string& strCoordSysUnits );
  int setCoordSys( const string& strValue,
                   const string& strKey,
                   string& strCoordSys );
  int setCoordSysUnits( const string& strValue,
                        const string& strKey,
                        string& strCoordSys );

  string getCoordSys( ) { return m_strCoordSys; }
  string getCoordSysUnits( ) { return m_strCoordSysUnits; }

  string getInCoordSys( ) { return m_strInCoordSys; }
  string getInCoordSysUnits( ) { return m_strInCoordSysUnits; }

  void setAdiabatic( bool bVerdict=true ) { m_bComputeAdiabat = bVerdict; }
  bool getAdiabatic( ) { return m_bComputeAdiabat; }

  int setFluxType( const string& strFluxType,
                   const string& strKey="setFluxType" );
  string getFluxType( ) { return m_strFluxType; }
  void clearFluxEnergies( ) { m_vdEnergies.clear(); }
  int setFluxEnergy( const double& dEnergy );
  int setFluxEnergies( const dvector& vdEnergies );
  int getNumFluxEnergies( ) { return int(m_vdEnergies.size()); }
  void getFluxEnergies( dvector& vdEnergies )
    { vdEnergies = m_vdEnergies; }
  void clearFluxEnergies2( ) { m_vdEnergies2.clear(); }
  int setFluxEnergy2( const double& dEnergy );
  int setFluxEnergies2( const dvector& vdEnergies2 );
  int getNumFluxEnergies2( ) { return int(m_vdEnergies2.size()); }
  void getFluxEnergies2( dvector& vdEnergies2 )
    { vdEnergies2 = m_vdEnergies2; }

  void setFluxMean( bool bVerdict=true ) { m_bComputeMean = bVerdict; }
  bool getFluxMean( ) {  return m_bComputeMean; }

  int setFluxPercentile( const int& iPercent );
  int setFluxPercentiles( const ivector& viPercent );
  void clearFluxPercentiles( ) { setFluxPercentile( 0 ); }
  int getNumFluxPercentiles( ) { return int(m_viPercentiles.size()); }
  void getFluxPercentiles( ivector& viPercent )
    { viPercent = m_viPercentiles; }

  int setFluxPerturbedScenario( const int& iScenario );
  int setFluxPerturbedScenarios( const ivector& viScenario );
  int setFluxPerturbedScenRange( const int& iScenarioMin, 
                                 const int& iScenarioMax );
  void clearFluxPerturbedScenarios( ) { setFluxPerturbedScenario( 0 ); }
  int getNumFluxPerturbedScenarios( )
    { return int(m_viPerturbedScenarios.size()); }
  void getFluxPerturbedScenarios( ivector& viScenario )
    { viScenario = m_viPerturbedScenarios; }

  int setFluxMonteCarloScenario( const int& iScenario );
  int setFluxMonteCarloScenarios( const ivector& viScenario );
  int setFluxMonteCarloScenRange( const int& iScenarioMin, 
                                  const int& iScenarioMax );
  void clearFluxMonteCarloScenarios( ) { setFluxMonteCarloScenario( 0 ); }
  int getNumFluxMonteCarloScenarios( )
    { return int(m_viMonteCarloScenarios.size()); }
  void getFluxMonteCarloScenarios( ivector& viScenario )
    { viScenario = m_viMonteCarloScenarios; }
  int setMonteCarloEpochTime( const double &dEpochTime,
                              const string& strKey="setMonteCarloEpochTime" );
  double getMonteCarloEpochTime( ) { return m_dMCEpochTime; }
  void setMonteCarloFluxPerturb( bool bPerturbFlux )
    { m_bPerturbFlux = bPerturbFlux; }
  bool getMonteCarloFluxPerturb( ) { return m_bPerturbFlux; }
  void setMonteCarloWorstCase( bool bVerdict=true )
    { m_bMCWorstCase = bVerdict; }
  bool getMonteCarloWorstCase( ) { return m_bMCWorstCase; }
  void setCLWorstCaseLog( bool bVerdict=true ) { m_bWcLogging = bVerdict; }
  bool getCLWorstCaseLog( ) { return m_bWcLogging; }

  int setReportTimes( const double& dReportTimeRef,
                      const double& dReportTimeDelta, // units = days
                      const string& strKey="setReportTimes" );
  int setReportTimesSec( const double& dReportTimeRef,
                         const double& dReportTimeDelta, // units = seconds
                         const string& strKey="setReportTimesSec" );
  void getReportTimesSec( dvector& vdReportTimeRef,
                          dvector& vdReportTimeDelta )
    { vdReportTimeRef=m_vdReportTimeRef; vdReportTimeDelta=m_vdReportTimeDelta; }
  int getNumReportTimes( ) { return int(m_vdReportTimeRef.size()); }
  void clearReportTimes( )
    { m_vdReportTimeRef.clear(); m_vdReportTimeDelta.clear(); }
  int setReportAtTime( const double& dReportAtTime,
                       const string& strKey="setReportAtTime" );
  void getReportAtTime( dvector& vdReportAtTime )
    { vdReportAtTime=m_vdReportAtTime; }
  int getNumReportAtTime( ) { return int(m_vdReportAtTime.size()); }
  void clearReportAtTime( ) { m_vdReportAtTime.clear(); }

  void setFluence( bool bVerdict=true ) { m_bComputeFluence = bVerdict; }
  bool getFluence( ) { return m_bComputeFluence; }
  void setDoseRate( bool bVerdict=true ) { m_bComputeDoseRate = bVerdict; }
  bool getDoseRate( ) { return m_bComputeDoseRate; }
  void setDoseAccum( bool bVerdict=true ) { m_bComputeCumDose = bVerdict; }
  bool getDoseAccum( ) { return m_bComputeCumDose; }

  int setDoseDepthUnits( const string& strDepthUnits, 
                         const string& strKey="setDoseDepthUnits" );
  string getDoseDepthUnits( ) { return m_strDepthUnits; } 
  int setDoseDepthValues( const dvector& vdDepths, 
                          const string& strKey="setDoseDepthValues" );
  int getNumDoseDepthValues( ) { return int(m_vdDoseDepths.size()); }
  void getDoseDepthValues( dvector& vdDepths ) { vdDepths = m_vdDoseDepths; }
  int setDoseDepths( const dvector& vdDepths, 
                     const string& strDepthUnits );
  int setDoseDetector( const string& strDetector,
                       const string& strKey="setDoseDetector" );
  string getDoseDetector( ) { return m_strDetectorType; }
  int setDoseGeometry( const string& strGeometry,
                       const string& strKey="setDoseGeometry" );
  string getDoseGeometry( ) { return m_strDoseGeometry; }
  int setDoseNuclearAttenMode( const string& strNucAttenMode,
                               const string& strKey="setDoseNuclearAttenMode" );
  void setDoseWithBrems( bool bVerdict=true ) { m_bDoseWithBrems = bVerdict; }
  bool getDoseWithBrems( ) { return m_bDoseWithBrems; }
  string getDoseNuclearAttenMode( ) { return m_strNuclearAttenMode; }
  int setDoseKernelDir( const string& strKernelDir,
                         const string& strKey );
  string getDoseKernelDir( ) { return m_strDoseKernelDir; }
  int setDoseKernelFile( const string& strKernelFile,
                         const string& strKey );
  string getDoseKernelFile( ) { return m_strDoseKernelFile; }
  void setUseDoseKernel( bool bVerdict=true ) { m_bUseDoseKernel = bVerdict; }
  bool getUseDoseKernel( ) { return m_bUseDoseKernel; }

  int setAccumMode( const string& strAccumMode,
                    const string& strKey="setAccumMode" );
  int getNumAccumModes( ) { return int(m_vstrAccumMode.size()); }
  void getAccumModes( vector<string> &vstrAccumMode )
    { vstrAccumMode = m_vstrAccumMode; }
  string getAccumMode( const int& iIdent=1 );
  string getAccumModeEntry( const int& iIdent )
    { return getAccumMode( iIdent ); }
  void clearAccumModes( ) { m_vstrAccumMode.clear(); }
  int setAccumInterval( const double& dVal, // units = days,
                        const string& strKey="setAccumInterval" );
  int setAccumIntervalSec( const double& dVal, // units = seconds
                           const string& strKey="setAccumIntervalSec" );
  double getAccumIntervalSec( const int& iIdent=1 );
  double getAccumIntervalSecEntry( const int& iIdent )
    { return getAccumIntervalSec( iIdent ); }

  void getAccumIntervalSecs( dvector& vdIntervals )
    { vdIntervals = m_vdIntervalSec; }
  int getNumAccumIntervals( ) { return int(m_vdIntervalSec.size()); }
  void clearAccumIntervals( ) { m_vdIntervalSec.clear(); }
  int setAccumIncrementSec( const double& dVal, // units = seconds
                            const string& strKey="setAccumIncrementSec" );
  double getAccumIncrementSec( ) { return m_dIncrementSec; }
  int setAccumIncrementFrac( const double& dVal, // unitless (0,1) exclusive
                             const string& strKey="setAccumIncrementFrac" );
  double getAccumIncrementFrac( ) { return m_dIncrementFrac; }

  int setAggregMean( );
  int setAggregMedian( ) { return ( setAggregConfLevel( 50 ) ); }
  int setAggregConfLevel( const int& iPercent );
  int setAggregConfLevels( const ivector& viPercent );
  void clearAggregConfLevels( ) { m_viAggConfLevels.clear(); }
  int getNumAggregConfLevels( ) { return int(m_viAggConfLevels.size()); }
  void getAggregConfLevels( ivector &viPercent )
    { viPercent = m_viAggConfLevels; }

  int setLegActivityLevel( const string& strActivityLevel,
                           const string& strKey="setActivityLevel" );
  string getLegActivityLevel( ) { return m_strActivityLevel; }
  int setLegActivityRange( const string& strActivityRange,
                           const string& strKey="setActivityRange" );
  string getLegActivityRange( ) { return m_strActivityRange; }
  int setLeg15DayAvgAp( const double& d15DayAvgAp,
                        const string& strKey="set15DayAvgAp" );
  double getLeg15DayAvgAp( ) { return m_d15DayAvgAp; }
  void setLegFixedEpoch( bool bFixEpoch ) { m_bFixEpoch = bFixEpoch; }
  void setLegShiftSAA( bool bShiftSAA ) { m_bShiftSAA = bShiftSAA; }
  bool getLegFixedEpoch( ) { return m_bFixEpoch; }
  bool getLegShiftSAA( ) { return m_bShiftSAA; }
  int setCamMagfieldModel( const string& strMFModel,
                           const string& strKey="setMagfieldModel" );
  string getCamMagfieldModel( ) { return m_strMagfieldModel; }
  int setCamDataFilter( const string& strDataFilter,
                        const string& strKey="setDataFilter" );
  string getCamDataFilter( ) { return m_strDataFilter; }
  int setCamPitchAngleBin( const string& strPitchAngleBin,
                           const string& strKey="setPitchAngleBin" );
  string getCamPitchAngleBin( ) { return m_strPitchAngleBin; }
  int setCamSpecies( const string& strSpecies,
                     const string& strKey="setSpecies" );
  string getCamSpecies( ) {
    if ( m_vstrSpecies.empty() ) return "none";
    return m_vstrSpecies[0];
  }

  int setOutputPrefix( const string& strOutputPrefix,
                       const string& strKey="setOutputPrefix" );
  string getOutputPrefix( ) { return m_strOutputPrefix; }

  int setAsciiOut( const string& strAsciiCode,
                   const string& strKey="setAsciiOut" );
  bool isAsciiOut( const string& strAsciiCode );
  
  int setWorkDir( const string& strWorkDir,
                   const string& strKey="setWorkDir" );
  string getWorkDir( ) { return m_strWorkDir; }
  int setBinDirName ( const string& strBinDirName,
                      const string& strKey="setBinDirName" );
  string getBinDirName( ) { return m_strBinDirName; }

  void setDelBinDir( bool bVerdict ) { m_bDelBinDir = bVerdict; }
  int getDelBinDir( ) { return int(m_bDelBinDir); }

  bool isCumulMCWorstCase( );

  // option to suspend all file/dir existence tests
  void setVerifyExist( bool bVerdict=true ) { m_bVerifyExist = bVerdict; }
  bool getVerifyExist( ) { return m_bVerifyExist; }

protected:

  // internal helper methods

  int expandEnvVarPath( const string& strEnvPath,
                        string& strExpandedPath,
                        const string& strKey );
  int checkFileExists( string& strFileName,
                       const string& strKey );
  int checkDirExists( string& strDirName,
                      const string& strKey );
  int checkFileParentDirExists( string& strFileName,
                                const string& strKey );
  string getAsciiStd( const string& strAsciiCode );
  int checkCoordSys( const string& strCoordSys, 
                     const string& strCoordSysUnits, 
                     string& strCoordOrder );
  int checkDoseDepths( );
  int checkModelParameters( );
  int checkModelEnergies( );
  int checkOrbitParameters ( );
  int addToOrderedList( ivector& viOrderedList, 
                        const int& iNewValue, 
                        const int& iMinValid=0, 
                        const int& iMaxValid=0,
                        bool bIncreasing=true );
  int addToOrderedList( dvector& vdOrderedList, 
                        const double& dNewValue, 
                        const double& dMinValid=0.0, 
                        const double& dMaxValid=0.0,
                        bool bIncreasing=true );
  string appendDirDelim( const string strDirName );

  // internal data members

  string            m_strInputFile;
  string            m_strOutputPrefix;
  string            m_strOrbitFile;

  string            m_strDirectionFile;
  string            m_strPitchAngleFile;
  string            m_strEphemTimesFile;

  string            m_strModel;
  bool              m_bLegacy;
  string            m_strFluxType;
  dvector           m_vdEnergies;
  dvector           m_vdEnergies2;

  int               m_iNumProcInput;  // MPI cores to use (#nodes includes master)
  int               m_iNumFileIo;     // number of threads for 'convert' or 'concat' tasks
  int               m_iChunkSize;     // memory management tuning parameter
  int               m_iTaskDelay;     // delay time between MPI tasking (def=1sec)
  string            m_strWorkDir;     // location of temporary binary subdirectory
  string            m_strBinDirName;  // specified naame of binary subdirectory
  bool              m_bDelBinDir;     // control automatic removal of temp dir
  vector<string>    m_vstrAsciiOut;   // control binary->ascii file generation (def="all")

  bool              m_bPerturbFlux;   // diagnostic parameter for MC processing
  bool              m_bComputeAdiabat;

  bool              m_bOmnidirectional;
  bool              m_bDirectionFile;
  bool              m_bPitchAngleFile;
  bool              m_bPitchAngleList;

  dvector           m_vdPitchAngles;

  bool              m_bComputeMean;
  bool              m_bComputePercentile;
  bool              m_bComputePerturbed;
  bool              m_bComputeMonteCarlo;
  bool              m_bMCWorstCase;
  bool              m_bWcLogging;

  ivector           m_viPercentiles;
  ivector           m_viPerturbedScenarios;
  ivector           m_viMonteCarloScenarios;
  double            m_dMCEpochTime;

  dvector           m_vdReportTimeRef;
  dvector           m_vdReportTimeDelta;
  dvector           m_vdReportAtTime;

  string            m_strModelDBDir; // dir for automatic file naming

  string            m_strModelDBFile;
  string            m_strMagFieldDBFile;
  string            m_strKPhiDBFile;
  string            m_strKHMinDBFile;
  string            m_strDoseModelDBFile;

  vector<string>    m_vstrExtraInfo;

  bool              m_bComputeFlux;  // always true 
  bool              m_bComputeFluence;
  bool              m_bComputeDoseRate;
  bool              m_bComputeCumDose;

  dvector           m_vdIntervalSec;

  vector<string>    m_vstrAccumMode;
  double            m_dIncrementSec;
  double            m_dIncrementFrac;

  ivector           m_viAggConfLevels;

  string            m_strTimeSpec;
  string            m_strCoordSys;
  string            m_strCoordSysUnits;
  string            m_strCoordOrder;
  string            m_strDataDelim;


  string            m_strInTimeSpec;
  string            m_strInCoordSys;
  string            m_strInCoordSysUnits;
  string            m_strInCoordOrder;
  string            m_strInDataDelim;


  dvector           m_vdDoseDepths;
  string            m_strDepthUnits;
  string            m_strDoseGeometry;
  string            m_strDetectorType;
  string            m_strNuclearAttenMode;
  bool              m_bDoseWithBrems;

  bool              m_bUseDoseKernel;
  string            m_strDoseKernelDir;
  string            m_strDoseKernelFile;

  bool              m_bEphemGenOnly;
  string            m_strOrbitProp;

  string            m_strTLEFile;
  double            m_dEphemStartTime;
  double            m_dEphemEndTime;
  double            m_dEphemStepSec;
  dvector           m_vdEphemVarStepSec;
  dvector           m_vdEphemTimes;

  bool              m_bKeplerJ2;
  string            m_strSGP4Mode;
  string            m_strSGP4Datum;

  double            m_dElementTime;
  double            m_dInclination;
  double            m_dRtAscOfAscNode;
  double            m_dEccentricity;
  double            m_dArgOfPerigee;
  double            m_dMeanAnomaly;
  double            m_dMeanMotion;
  double            m_dMeanMotion1stDeriv;
  double            m_dMeanMotion2ndDeriv;
  double            m_dBStar;

  double            m_dAltApogee;
  double            m_dAltPerigee;
  double            m_dLocTimeApogee;
  double            m_dLocTimeMaxIncl;
  double            m_dTimeOfPerigee;
  double            m_dSemiMajorAxis;
  dvector           m_vdStatePos;
  dvector           m_vdStateVel;
  double            m_dGeosynchLon;

  string            m_strActivityLevel;
  string            m_strActivityRange;
  double            m_d15DayAvgAp;
  bool              m_bFixEpoch;
  bool              m_bShiftSAA;

  bool m_bCammice;
  string            m_strMagfieldModel;
  string            m_strDataFilter;
  vector<string>    m_vstrSpecies;
  string            m_strPitchAngleBin;

  bool              m_bAPILimit; // true for API, false for CmdLine
                                 // when true, Cammice model limited to one species
  bool              m_bVerifyExist; // true by default
                                    // set to false to suspend file/dir existence tests
};


#endif
