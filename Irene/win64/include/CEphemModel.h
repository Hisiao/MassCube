/******************************************************************************

 File: CEphemModel.h

 Description: Declarations for methods to perform orbit propagation using
   the underlying Kepler, SGP4 or Lokangle(Sateph) propagator models.

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

#ifndef CEPHEMMODEL_H
#define CEPHEMMODEL_H

#include <vector>
#include <string>

#include "VectorTypes.h"
#include "DataBaseNames.h"

#include "CTimeValue.h"
#include "CMagfield.h"
#include "COrbitPropSateph.h"
#include "COrbitPropSGP4.h"
#include "CKeplerOrbitProp.h"

typedef enum ePropagator {
  ePropKepler,
  ePropSatEph,
  ePropSGP4,
  ePropUndef
} ePropagator;

typedef enum eOrbitDef {
  eOrbitUndefined,
  eOrbitTLEfile,
  eOrbitTLEstrings,
  eOrbitGeosynch,
  eOrbitStateVector,
  eOrbitMeanElem,
  eOrbitClassicElem,
  eOrbitSolarElem,
  eOrbitSimpleElem
} eOrbitDef;

class EphemModel
{
public:
  EphemModel( );
  virtual ~EphemModel( );

  int setModelDBDir( const string& strModelDBDir );
  string getModelDBDir( ) { return m_strModelDBDir; }
  int setMagfieldDBFile( const string& strMagfieldDBFile );
  string getMagfieldDBFile( ) { return m_strMagfieldDBFile; }
  int setChunkSize( int iChunkSize );
  int getChunkSize( ) { return m_iChunkSize; }

  int setStartTime( const double& dStartTime );
  double getStartTime( ) { return m_dStartTime; }
  int setEndTime( const double& dEndTime );
  double getEndTime( ) { return m_dEndTime; }
  int setTimeStep( const double& dTimeStep );
  double getTimeStep( ) { return m_dTimeStep; }
  int setTimes( const double& dStartMJD, 
                const double& dEndMJD, 
                const double& dTimeStepSec,
                bool bCoarseEphem = false );
  void getTimes( double& dStartMJD, 
                 double& dEndMJD, 
                 double& dTimeStepSec );
  int setVarTimeStep( const double& dTimeMinStepSec,
                      const double& dTimeMaxStepSec = 3600.0,
                      const double& dTimeRoundSec = 5.0 );
  void getVarTimeStep( double& dTimeMinStepSec,
                       double& dTimeMaxStepSec,
                       double& dTimeRoundSec );
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
  int setTimes( const dvector& vdTimesMJD ) { return setTimesList( vdTimesMJD ); }
  int setTimesList( const dvector& vdTimesMJD );
  int getNumTimes( ) { return getNumTimesList(); }
  int getNumTimesList( );
  int getTimesList( dvector& vdTimes );
  void clearTimesList( ) { m_vdEphemTimes.clear(); }


  int setPropagator( string strPropSpec );
  string getPropagator ( );
  string getPropagatorAndMode();
  int setSGP4param( string strMode, string strWGS );
  string getSGP4Mode( );
  string getSGP4WGS( );
  void setKeplerUseJ2( bool bUseJ2 );
  bool getKeplerUseJ2( ) { return m_bKeplerUseJ2; }

  void setVerbose( bool bVerdict = true ) { m_bVerbose = bVerdict; }

  int setTLEFile( const string& strTLEFile );
  string getTLEFile( ) { return m_strTLEFile; }
  void resetTLE();
  int setTLE( const string& strTLELine1,
              const string& strTLELine2 );
  int setTLE( const vector<string>& vstrTLELine1s,
              const vector<string>& vstrTLELine2s );
  void getTLE( vector<string>& vstrTLELine1s,
               vector<string>& vstrTLELine2s );

  int setInclination( const double& dInclination );
  double getInclination( ) { return m_dInclination; }
  int setArgOfPerigee( const double& dArgOfPerigee );
  double getArgOfPerigee( ) { return m_dArgOfPerigee; }
  int setMeanAnomaly( const double& dMeanAnomaly );
  double getMeanAnomaly( ) { return m_dMeanAnomaly; }
  int setMeanMotion1stDeriv( const double& d1stDerivMeanMotion );
  double getMeanMotion1stDeriv( ) { return m_dMeanMotion1stDeriv; }
  int setElementTime( const double& dElementTimeMJD );
  double getElementTime( ) { return m_dElementTimeMJD; }
  int setEccentricity( const double& dEccentricity );
  double getEccentricity( ) { return m_dEccentricity; }
  int setRightAscension( const double& dRtAscOfAscNode );
  double getRightAscension( ) { return m_dRightAscension; }
  int setMeanMotion( const double& dMeanMotion );
  double getMeanMotion( ) { return m_dMeanMotion; }
  int setMeanMotion2ndDeriv( const double& d2ndDerivMeanMotion );
  double getMeanMotion2ndDeriv( ) { return m_dMeanMotion2ndDeriv; }
  int setBStar( const double& dBStar );
  double getBStar( ) { return m_dBStar; }
  int setAltitudeOfApogee( const double& dOrbAltApogee );
  double getAltitudeOfApogee( ) { return m_dOrbAltApogee; }
  int setAltitudeOfPerigee( const double& dOrbAltPerigee );
  double getAltitudeOfPerigee( ) { return m_dOrbAltPerigee; }
  int setLocalTimeOfApogee( const double& dOrbLocTimeApogee );
  double getLocalTimeOfApogee( ) { return m_dOrbLocTimeApogee; }
  int setLocalTimeMaxInclination( const double& dOrbLocTimeMaxIncl );
  double getLocalTimeMaxInclination( ) { return m_dOrbLocTimeMaxIncl; }
  int setTimeOfPerigee( const double& dTimeMJD );
  double getTimeOfPerigee( ) { return m_dTimeOfPerigeeMJD; }
  int setSemiMajorAxis( const double& dAxisInRe );
  double getSemiMajorAxis( ) { return m_dSemiMajorAxisRe; }

  int setPositionGEI( const double& dPosX, 
                      const double& dPosY, 
                      const double& dPosZ );
  int getPositionGEI( double& dPosX, 
                      double& dPosY, 
                      double& dPosZ );
  int setVelocityGEI( const double& dVelX, 
                      const double& dVelY, 
                      const double& dVelZ );
  int getVelocityGEI( double& dVelX, 
                      double& dVelY, 
                      double& dVelZ );
  int setStateVectors( const dvector& vdStatePos,
                       const dvector& vdStateVel );
  void getStateVectors( dvector& vdStatePos,
                        dvector& vdStateVel )
                      { vdStatePos = m_vdStatePos; vdStateVel = m_vdStateVel; }

  int setGeosynchLon( const double& dLongitudeDeg );
  double getGeosynchLon( ) { return m_dGeosynchLon; }

  void resetOrbitParameters( );

  void restartEphemeris( ) { m_iChunk = 0; }
  int computeEphemeris( dvector& vdTimesMJD, // output!
                        dvector& vdXGEI,
                        dvector& vdYGEI,
                        dvector& vdZGEI,
                        dvector& vdXDotGEI,
                        dvector& vdYDotGEI,
                        dvector& vdZDotGEI,
                        bool bCoarseEphem = false );

  // for computeEphemeris() methods, return >=0 is data count; <0 is error
  int computeEphemeris( emfCoordSys eCoordSys, // input
                        dvector& vdTimesMJD, // output!
                        dvector& vdCoord1,
                        dvector& vdCoord2,
                        dvector& vdCoord3 );

  int computeEphemeris( const string& strCoordSys,
                        const string& strCoordUnits, // input
                        dvector& vdTimesMJD, // output!
                        dvector& vdCoord1,
                        dvector& vdCoord2,
                        dvector& vdCoord3 );

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

  int setMainField( const string& strMainField );
  int setExternalField( const string& strExternalField );

  string getMainField( );
  string getExternalField( );

  int updateTime( const double& dTimeMjd );

  int setKpValue( const double& dKpVal );
  int setKpValues( const double& dRefTime,
                   const dvector& vdKpVals );
  double getKpValue( );
  double getKpValuesRefTime( );
  double getKpValuesEndTime( );

  int computeBfield( const string& strCoordSys,
                     const string& strCoordUnits,
                     const double& dMjdTime,
                     const double& dCoord1,
                     const double& dCoord2,
                     const double& dCoord3,
                     dvector& vdBVecGeo,
                     double& dBMag,
                     double& dBMin,
                     double& dLm );
  int computeBfield( const string& strCoordSys,
                     const string& strCoordUnits,
                     const dvector& vdMjdTimes,
                     const dvector& vdCoord1,
                     const dvector& vdCoord2,
                     const dvector& vdCoord3,
                     vdvector& vvdBVecGeo,
                     dvector& vdBMag,
                     dvector& vdBMin,
                     dvector& vdLm );

  int computeInvariants( const string& strCoordSys,
                         const string& strCoordUnits,
                         const double& dMjdTime,
                         const double& dCoord1,
                         const double& dCoord2,
                         const double& dCoord3,
                         const dvector& vdPitchAngles,
                         double& dBMin,
                         dvector& vdBMinPosGeo,
                         dvector& vdBVecGeo,
                         dvector& vdLm,
                         dvector& vdI );
  int computeInvariants( const string& strCoordSys,
                         const string& strCoordUnits,
                         const dvector& vdMjdTimes,
                         const dvector& vdCoord1,
                         const dvector& vdCoord2,
                         const dvector& vdCoord3,
                         const dvector& vdPitchAngles,
                         dvector& vdBMin,
                         vdvector& vvdBMinPosGeo,
                         vdvector& vvdBVecGeo,
                         vdvector& vvdLm,
                         vdvector& vvdI );
protected:
  ePropagator     m_ePropagator;
  eOperationModes m_eSGP4Mode;
  eWGSConstants   m_eSGP4Wgs;
  bool            m_bKeplerUseJ2;
  bool            m_bTimeSpecMod;
  bool            m_bVerbose;


  double       m_dStartTime;
  double       m_dEndTime;
  double       m_dTimeStep;
  dvector      m_vdVarStepSec;

  string  m_strTLEFile;
  vector<string> m_vstrTLELine1s;
  vector<string> m_vstrTLELine2s;

  double       m_dElementTimeMJD;

  double       m_dInclination;
  double       m_dRightAscension;
  double       m_dEccentricity;
  double       m_dArgOfPerigee;
  double       m_dMeanAnomaly;
  double       m_dMeanMotion;
  double       m_dMeanMotion1stDeriv;
  double       m_dMeanMotion2ndDeriv;
  double       m_dBStar;

  double       m_dOrbAltApogee;
  double       m_dOrbAltPerigee;
  double       m_dOrbLocTimeApogee;
  double       m_dOrbLocTimeMaxIncl;
  double       m_dTimeOfPerigeeMJD;
  double       m_dSemiMajorAxisRe;

  dvector      m_vdStatePos;
  dvector      m_vdStateVel;
  double       m_dGeosynchLon;
  
  double       m_dRadEarth;

  eOrbitDef    m_eOrbitDef;
  
  dvector      m_vdEphemTimes;
  COrbitProp*       m_pOrbitProp;
  CKeplerOrbitProp* m_pKeplerProp;

  int          m_iChunkSize; // performance tuning parameter
  int          m_iNumChunk;
  int          m_iChunk;
  string       m_strModelDBDir; // dir for automatic file naming

  string       m_strMagfieldDBFile;
  CMagfield    *m_pMagfield;

  int updateMagfieldModel( );
  int expandEnvVarPath( const string& strEnvPath,
                        string& strExpandedPath,
                        const string& strKey );
  eOrbitDef getOrbitDefinitionType( );
  int extractMeanElementsFromTLE( );
  int generateTLEfromStateVector( );
  int invokePropagator( );
  void clearPropagator( );
  int getCoordSysEnum( const string& strCoordSys, 
                       const string& strCoordUnits,
                       emfCoordSys& eCoordSys );
  // methods used in variable timestep generation
  int generateVarTimes( );
  double calcTimestepByRadius( const double &dRadius );
  int calcPolyFit( const dvector &vdXinput,
                   const dvector &vdYinput,
                   const int &iStart,  // index ref of input vector
                   const int &iNum,    //  # points to use
                   const int &iOrder,
                   dvector &vdCoef );
  double getPolyFit( const double &dXvalue,
                     const dvector &vdCoef );
  int checkFileExists( const string& strFileName );
  int checkDirExists( const string& strDirName );
  string appendDirDelim( const string strDirName );
};

#endif
