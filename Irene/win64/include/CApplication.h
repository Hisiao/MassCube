/******************************************************************************

 File: CApplication.h

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

   Copyright 2016 Atmospheric and Environmental Research, Inc. (AER)

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

#ifndef CAPPLICATION_H
#define CAPPLICATION_H

#include <vector>
#include <string>
#include <map>
using namespace std;

#include "CInputParameters.h"
#include "CDateTimeUtil.h"
#include "CDataFileInput.h"
#include "CDataCalcInfo.h"

class Application : public InputParameters, public DateTimeUtil
{

public:

  Application();
  virtual ~Application();

  // directory in which temporary directory and files are written/read
  //  when not specified, defaults to the current working directory
  int setWorkDir( const string& strWorkDir );
  string getWorkDir( ) { return m_strWorkDir; }  //  ### duplicated

  // **Required** to be able to execute CmdLine 
  //  and various Executor programs behind-the-scenes
  int setExecDir( const string& strExecDir );
  string getExecDir( ) { return m_strExecDir; }

  void clearInEphemeris( );
  int setInEphemeris( const dvector& vdTimes, 
                      const dvector& vdCoords1,
                      const dvector& vdCoords2, 
                      const dvector& vdCoords3,
                      bool bAppend=false );
  int getNumInEphemeris( ) { return int(m_vdInTimes.size()); }
  void getInEphemeris( dvector& vdTimes,  
                       dvector& vdCoords1,
                       dvector& vdCoords2,
                       dvector& vdCoords3 ) { 
                           vdTimes = m_vdInTimes;
                           vdCoords1 = m_vdInCoords1;
                           vdCoords2 = m_vdInCoords2;
                           vdCoords3 = m_vdInCoords3; }

  int setChunkSize( const int& iChunkSize, //  intercept chunk size change to reset mode data information
                    const string& strKey="setChunkSize" );  // calls resetModelData, then InputParameters::setChunkSize
  void resetModelData( );
  void resetParameters( );  // includes call to InputParameters::resetParameters

  int resetModelRun( bool bDelBinDir=true,
                     bool bResetParam=false );
  int removeDirectory( const string& strDirName );

  int setWindowsMpiMode( const string& strMode );
  string getWindowsMpiMode( );

  int runModel( );

  // return value determined *during runModel* execution, querying ths number from
  //   the defined pitch angle list or pitch angle file or direction vector file
  int getNumDir( ) { return m_iNumDir; }  // returns 1 if omnidirectional

  // analogous to computeEphemeris; accesses data in chunks
  int getEphemeris( dvector& vdTimes,
                    dvector& vdCoord1,
                    dvector& vdCoord2,
                    dvector& vdCoord3 );

  // replication of previous verion flux calculation access methods:
    //these are currently configured to provide data in 'chunks' for each call;
    //  --> need to be able to have setting to change to reading entire file...
    // AccumAvg argument used to access the specific accumulations/averages
    //  (Intv,Full,Boxcar,Expon); use Cumul or Default for the straight flux values
    // iAccumIntvId argument used to specify the accumulation interval (1-N),
    //    where N=getNumAccumIntervals(); default is 1
  int flyinMean( vdvector& vvdFluxData,  // unidirectional; will also get Legacy results
                 const string& strAccumMode = "default",
                 const int& iAccumIntvId = 1 );
  int flyinMean( vvdvector& vvvdFluxData,  // will also get Legacy results
                 const string& strAccumMode = "default",
                 const int& iAccumIntvId = 1 );
  int flyinMean( dvector& vdTimes,   // also get time, ephemeris and pitch angle data
                 dvector& vdCoord1, 
                 dvector& vdCoord2, 
                 dvector& vdCoord3,
                 vdvector& vvdPitchAngles,
                 vvdvector& vvvdFluxData,  // will also get Legacy results
                 const string& strAccumMode = "default",
                 const int& iAccumIntvId = 1 );

  int flyinPercentile( const int& iPercentile,
                       vvdvector& vvvdFluxData,
                       const string& strAccumMode = "default",
                       const int& iAccumIntvId = 1 );
  int flyinPercentile( const int& iPercentile,
                       dvector& vdTimes, 
                       dvector& vdCoord1, 
                       dvector& vdCoord2, 
                       dvector& vdCoord3,
                       vdvector& vvdPitchAngles,
                       vvdvector& vvvdFluxData,
                       const string& strAccumMode = "default",
                       const int& iAccumIntvId = 1 );
  int flyinPerturbedMean( const int& iScenario,
                          vvdvector& vvvdFluxData,
                          const string& strAccumMode = "default",
                          const int& iAccumIntvId = 1 );
  int flyinPerturbedMean( const int& iScenario,
                          dvector& vdTimes, 
                          dvector& vdCoord1, 
                          dvector& vdCoord2, 
                          dvector& vdCoord3,
                          vdvector& vvdPitchAngles,
                          vvdvector& vvvdFluxData,
                          const string& strAccumMode = "default",
                          const int& iAccumIntvId = 1 );
   // replacement for previous version 'flyinScenario' method;
   //  (additional arguments that it used are not relevant here)
  int flyinMonteCarlo( const int& iScenario,
                       vvdvector& vvvdFluxData,
                       const string& strAccumMode = "default",
                       const int& iAccumIntvId = 1 );
  int flyinMonteCarlo( const int& iScenario,
                       dvector& vdTimes, 
                       dvector& vdCoord1, 
                       dvector& vdCoord2, 
                       dvector& vdCoord3,
                       vdvector& vvdPitchAngles,
                       vvdvector& vvvdFluxData,
                       const string& strAccumMode = "default",
                       const int& iAccumIntvId = 1 );

  int getAdiabaticCoords( vdvector& vvdAlpha,
                          vdvector& vvdLm,
                          vdvector& vvdK,
                          vdvector& vvdPhi,
                          vdvector& vvdHmin,
                          vdvector& vvdLstar,
                          dvector& vdBmin, 
                          dvector& vdBlocal, 
                          dvector& vdMagLT ); 

  int getAdiabaticCoords( dvector& vdTimes, 
                          dvector& vdCoord1, 
                          dvector& vdCoord2, 
                          dvector& vdCoord3,
                          vdvector& vvdPitchAngles,
                          vdvector& vvdAlpha,
                          vdvector& vvdLm,
                          vdvector& vvdK,
                          vdvector& vvdPhi,
                          vdvector& vvdHmin,
                          vdvector& vvdLstar,
                          dvector& vdBmin, 
                          dvector& vdBlocal, 
                          dvector& vdMagLT ); 

  int getModelData( const string& strDataType, // "flux"|"fluence"|"doserate"|"doseaccum"
                    const string& strFluxMode, // "mean"|"percent"|"perturbed"|"montecarlo"|"adiabat"(only w/ flux)
                    const int& iCalcVal,            //   -1  |  1-99   |  1-999    |  1-999     |  -2
                    dvector& vdTimes, 
                    dvector& vdCoord1, 
                    dvector& vdCoord2, 
                    dvector& vdCoord3,
                    vdvector& vvdPitchAngles, // will always be empty for dose data
                    vvdvector& vvvdData,
                    const string& strAccumMode = "default",
                    const int& iAccumIntvId = 1,
                    const string& strMethodName = "getModelData" );

  int getAggregData( const string& strDataType, // "flux"|"fluence"|"doserate"|"doseaccum"
                     const string& strFluxMode, // "perturbed"|"montecarlo"
                     const int& iPercent,            //  0-100
                     dvector& vdTimes, 
                     dvector& vdCoord1, 
                     dvector& vdCoord2, 
                     dvector& vdCoord3,
                     vdvector& vvdPitchAngles, // will always be empty for dose data
                     vvdvector& vvvdData,
                     const string& strAccumMode = "default",
                     const int& iAccumIntvId = 1,
                     const string& strMethodName = "getAggregData" );

  int reduceDataDimension( const vvdvector& vvvdData,
                           vdvector& vvdData );

/*
---these methods have not been developed yet-----

 
   int purgeModel();  // needed to manually call for deletion of bin file dir
   // should this also delete the generated input file too?
   //  -generated input file requires that DelBinDir set to False, 
   //   so to retain access after bootstrapped CmdLineMpi run
   //-- will need to purge on class exit/deconstructor

*/

protected:

  DataCalcInfo *m_pDataCalc;

  int writeInputFile( string& strInputFile );
  string makeValueList( const ivector& viValues,
                        const int& iWidth );
  int writeEphemFile( );
  int writeEphemTimesFile( );

  string m_strExecDir;
  string m_strBinDir;
  string m_strProcessDir;

  int m_iWinMpiMode;

  int m_iNumDir;

  string m_strPid;
  int m_iRunId;

  bool m_bEphemInput;  // flag for when time/position values explicitly defined

  dvector m_vdInTimes;
  dvector m_vdInCoords1;
  dvector m_vdInCoords2; 
  dvector m_vdInCoords3;

  bool m_bModelSuccess;

  // DataFileInput object pointers for each data type
  map<string,DataFileInput *> m_mxpDataInput;
  map<string,int> m_mxiCalcVal;  // associated percentile or scenario for DFI object file
  DataFileInput *m_pEphemInput;

};

#endif
