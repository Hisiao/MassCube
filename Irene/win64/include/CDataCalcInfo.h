/******************************************************************************

 File: CDataCalcInfo.h

 Description: Declarations for methods for the generation of model 'use cases'
   based on the parameter settings.

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

#ifndef DATACALCINFO_H
#define DATACALCINFO_H

#include <vector>
#include <string>

#define GEN_RawMCWC

typedef enum eFluxModeType
{
  eFluxModeUndefined = -1,
  eFluxModeAdiabat = 0,
  eFluxModeMean,
  eFluxModePercentile,
  eFluxModePerturbedMean,
  eFluxModeMonteCarlo,
  eFluxModeMonteCarloWC,  /// ### worst case variant
  eFluxModeLastEntry  // provides count of enums here
} eFluxModeType;

typedef enum eDataTypeType
{
  eDataTypeUndefined = -1,
  eDataTypeFlux = 0,
  eDataTypeFluence,
  eDataTypeDoseRate,
  eDataTypeDoseAccum,
  eDataTypeLastEntry  // provides count of enums here
} eDataTypeType;

typedef enum eAccumModeType
{
  eAccumModeUndefined = -1,
  eAccumModeFull = 0,
  eAccumModeCumul,
  eAccumModeIntv,
  eAccumModeBoxcar,
  eAccumModeExponent,
} eAccumModeType;

struct structDataCalc {
  std::string strDataType;        // flux|fluence|doserate|doseaccum
  eFluxModeType eFluxMode;        //  /  mean|percent|perturb|monteC|[adiabat]
  int iCalcVal;                   //  \  n/a | %ile  | scen# | scen# | n/a
  eAccumModeType eAccumMode;      // Full|Cumul|Intv|Boxcar|Expon
  int iInterval;                  // interval index, valid 0-8 (with Intv, Boxcar, Expon only)
  int iAggregVal;                 //  aggreg percent 0-100
  std::string strFileName;

  structDataCalc():strDataType("flux"),
                   eFluxMode(eFluxModeMean),
                   iCalcVal(-1),
                   eAccumMode(eAccumModeCumul),
                   iInterval(0),
                   iAggregVal(-1),
                   strFileName(""){};
};


class DataCalcInfo
{

  public:
    DataCalcInfo();
    virtual ~DataCalcInfo() {}

    int setDataFileBase( const std::string& strFileBase );

    int setDataType( );
    int setDataType( const std::string& strDataType );

    int setFluxMode( );
    int setFluxMode( const std::string& strFluxMode,
                     const int& iCalcVal=-1 );
    int setFluxMode( const std::string& strFluxMode,
                     const int& iCalcValMin,
                     const int& iCalcValMax,
                     const int& iCalcValIncr=1 );
    int setFluxMode( const std::string& strFluxMode,
                     const std::vector<int>& viCalcVals );
    int setFluxMode( const eFluxModeType& eFluxMode,
                     const int& iCalcVal=-1 );

    int addToOrderedList( std::vector<int>& viOrderedList,
                          const int& iNewValue,
                          const int& iMinValid=0,
                          const int& iMaxValid=0 );
    int addToOrderedList( std::vector<double>& vdOrderedList,
                          const double& dNewValue,
                          const double& dMinValid=0.0,
                          const double& dMaxValid=0.0 );
    int setAccumIntervalSec( const double& dAccumInterval );

    int getNumAccumIntervals( ) { return int( m_vdAccumInterval.size() ); }
    double getAccumIntervalSec( ) { return getAccumIntervalSecEntry( 1 ); }
    double getAccumIntervalSecEntry( const int& iIdent );

    int setAccumIncrementSec( const double& dAccumIncrement=-1.0 );
    int setAccumIncrementFrac( const double& dAccumIncrFrac=-1.0 );

    double getAccumIncrement( ) { return m_dAccumIncrement; }

    int setAccumMode( ); // clears mode settings
    int setAccumMode( const std::string& strAccumMode );
    int setAccumMode( const eAccumModeType& eAccumMode );

    int setAggregConfLevel ( ); // clears aggregation confidence level list
    int setAggregConfLevel ( const int& iPercent );
    int setAggregMedian ( ) { return setAggregConfLevel(50); }
    int setAggregMean ( ) { return -1; } // feature removed
    int setAggregConfLevels ( const std::vector<int>& viPercent );
    int getNumAggregConfLevels( ) { return int( m_viAggConfLevels.size() ); }

    bool hasAdiabat() { return m_bAdiabatCoord; }
    void canHasAdiabat( bool bVerdict ) { m_bAllowAdiabat = bVerdict; }
    void setLegacyModel( bool bVerdict ) { m_bLegacyModel = bVerdict; }

    void setMCWorstCase( bool bVerdict ) { m_bMCWorstCase = bVerdict; }
    bool getMCWorstCase() { return m_bMCWorstCase; }
    void setCLWorstCaseLog( bool bVerdict=true ) { m_bWcLogging = bVerdict; }
    bool getCLWorstCaseLog() { return m_bWcLogging; }
    // flag for extra raw flux MC WC files
    void setRawMcWc( bool bVerdict=true ) { m_bRawMcWc = bVerdict; }

    void setFirstAccum( bool bVerdict ) { m_bFirstAccum = bVerdict; }

    bool hasAccumIntv() { return m_bAccumIntv; }
    bool hasAccumBoxcar() { return m_bAccumBoxcar; }
    bool hasAccumExp() { return m_bAccumExponent; }

    int clearUseCases(); // reset all parameters to default settings
    int resetUseCases() { return clearUseCases(); }

    std::string getDataFileBase() { return m_strDataFileBase; }
    std::string getDataFileExt() { return m_strFileExt; }

    // get list of filenames of calculations matching these types/modes (""/undef=any)
    //   'iCalcVal': * individual (flux mode) percentile number (valid=1-99) 
    //               * individual (flux mode) PM/MC scenario number(valid 1-999)
    //               *  0: all PM/MC aggregation percentiles
    //               * -1: all (flux mode) percentiles and/or PM/MC scenarios
    //               * -2: all of the above
    int getFileNames( std::vector<std::string>& strvFileNames,
                      const std::string& strDataType=std::string(""),
                      const eFluxModeType& eFluxMode=eFluxModeUndefined,
                      const eAccumModeType& eAccumMode=eAccumModeUndefined,
                      const int& iInterval=-1,  // -1 for all intervals, 0-8 specific interval
                      bool bAppendToList = false );
    int getFileNames( std::vector<std::string>& strvFileNames,
                      const std::string& strDataType,
                      const eFluxModeType& eFluxMode,
                      const int& iCalcVal,
                      const eAccumModeType& eAccumMode=eAccumModeUndefined,
                      const int& iInterval=-1,  // -1 for all intervals, 0-8 specific interval
                      bool bAppendToList = false );

    // get list of use case structures matching these types/modes (""/undef=any)
    int getUseCases( std::vector<structDataCalc>& svDataCalc,
                     const std::string& strDataType=std::string(""),
                     const eFluxModeType& eFluxMode=eFluxModeUndefined,
                     const eAccumModeType& eAccumMode=eAccumModeUndefined,
                     const int& iInterval=-1,  // -1 for all intervals, 0-8 specific interval
                     bool bAppendToList = false );
    int getUseCases( std::vector<structDataCalc>& svDataCalc,
                     const std::string& strDataType,
                     const eFluxModeType& eFluxMode,
                     const int& iCalcVal,
                     const eAccumModeType& eAccumMode=eAccumModeUndefined,
                     const int& iInterval=-1,  // -1 for all intervals, 0-8 specific interval
                     bool bAppendToList = false );
    int getNumUseCases( );
    void printUseCase( const structDataCalc& svDataCalc );

    // returns the 'standardized' versions of data type and flux mode strings
    std::string getDataType( const std::string& strDataType );
    std::string getFluxMode( const std::string& strFluxMode );
    std::string getAccumMode( const std::string& strAccumMode );
    eFluxModeType getFluxModeEnum( const std::string& strFluxMode );
    eDataTypeType getDataTypeEnum( const std::string& strDataType );
    eAccumModeType getAccumModeEnum( const std::string& strAccumMode );

  protected:

    int getDataTypes ( std::vector<std::string>& vstrDataTypes );

    int generateUseCases ();
    int addUseCase( const std::string& strDataType,
                    const eFluxModeType& eFluxMode,
                    const int& iCalcVal,
                    const eAccumModeType& eAccumMode,
                    const int& iInterval=0,
                    const int& iAggregVal=0 );

    std::string m_strDataFileBase;
    std::string m_strFileExt;

    // data calculation mode settings
    bool m_bAdiabatCoord;
    bool m_bFluxMean;
    bool m_bFluxPercentile;
    bool m_bFluxPerturbed;
    bool m_bFluxMonteCarlo;
    bool m_bFluence;
    bool m_bDoseRate;
    bool m_bDoseAccum;

    // data accumulation modes
    bool m_bAccumFull;
    bool m_bAccumCumul;
    bool m_bAccumIntv;
    bool m_bAccumBoxcar;
    bool m_bAccumExponent;
    bool m_bMCWorstCase;
    bool m_bWcLogging;
    bool m_bRawMcWc;

    // flag used for controlling addition of 'implied' Cumul mode
    bool m_bFirstAccum;  // default = true

    std::vector<double>  m_vdAccumInterval; // vector, in seconds
    double  m_dAccumIncrement; // in seconds

    double  m_dAccumIncrFrac; // -1.0 when 'inactive'

    std::vector<int> m_ivFluxPercentile;
    std::vector<int> m_ivScenPerturbed;
    std::vector<int> m_ivScenMonteCarlo;

    std::vector<int> m_viAggConfLevels; // for *all* scenario aggregations
    // << add other aggregation parameter values here >>

    bool m_bParamsRevised;
    bool m_bAllowAdiabat; // set to false when adiabat not appropriate (ie aggreg)
    bool m_bLegacyModel; // set to true when using old, flat models - limits valid settings

    // full set of 'use cases', derived from settings
    std::vector<structDataCalc> m_svDataCalc;

};

#endif
