/***********************************************************************

Module: COrbitProp_c.h

Description: C interfce methods to the COrbitProp class.

The COrbitProp class serves as a base class for orbit propagation models
described by their own class structure.  The COrbitProp class provides general
methods for processing two-line element sets and class variables common to
all orbit propagation models.  Methods for computing satellite ephemeris are
provided which require a call to a pure virtual function computeModelEphem
which must be defined in a derived class for a specific orbit propagation model.

 Rights and Restrictions:

   Copyright 2014 Atmospheric and Environmental Research, Inc. (AER)

   DISTRIBUTION A. Approved for public release; distribution is unlimited.

 Author:

   This software was developed and/or modified by AER staff

 Contact:

   Atmospheric and Environmental Research, Inc.
   131 Hartwell Avenue
   Lexington, MA 02421-3126 USA
   Phone: 781.761.2288
   email: spwx@aer.com

 References:

   None

 Revision history:

   VERSION     DATE           NOTES
     1.0    08 March 2013  First release

------------------------------------------------------------------------

***********************************************************************/
#ifndef CORBITPROP_C_H
#define CORBITPROP_C_H

#include "COrbitPropEnums_c.h"
#include "COrbitProp.h"
#include "COrbitPropSGP4.h"
#include "COrbitPropSateph.h"
#include "CTimeValue.h"

//-------------------------------------------------------------------
// defines and typedefs
//-------------------------------------------------------------------

/**
 * @file: COrbitProp_c.h
 * COrbitProp class C interface function definitions
 */

#ifdef __cplusplus
extern "C" {
#endif

// =======================================================
// These utility vector/data copy routines are duplicated amongst the model
//  classes, so to be enable each to be wholly independent & self-contained
int OrbitPropCopyDataToVector1D( double* pvdData,
                                 int iNum,
                                 dvector& vdData );
int OrbitPropCopyDataToVector2D( double* pvvdData,
                                 int iNum1,
                                 int iNum2,
                                 vdvector& vvdData );
int OrbitPropCopyDataToVector3D( double* pvvvdData,
                                 int iNum1,
                                 int iNum2,
                                 int iNum3,
                                 vvdvector& vvvdData );
int OrbitPropCopyVector1DToData( dvector& vdData,
                                 int iNum,
                                 double* pvdData );
int OrbitPropCopyVector2DToData( vdvector& vvdData,
                                 int iNum1,
                                 int iNum2,
                                 double* pvvdData );
int OrbitPropCopyVector3DToData( vvdvector& vvvdData,
                                 int iNum1,
                                 int iNum2,
                                 int iNum3,
                                 double* pvvvdData );
// =======================================================

typedef int HANDLE;


//-------------------------------------------------------------------
// Basic Progression:
//
// 1. Initialize the propagator    Instantiate derived model class (SGP4/Sateph)
// 2. Load orbit elements          TLE file or string pair, Mean or Solar Elements
// 3. Set the ephemeris times      Start, Stop, Step
// 4. ComputeEphem                 Single x,y,z arrays or grouped by position, velocity, time arrays
// 5. Clear the propagator         Delete derived class and its data
//
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// COrbitProp Handling Methods to support C interface
//-------------------------------------------------------------------

// function OrbitPropStartUp
/**
 *  C interface call to the constructor of the specified propagator type (SGP4 or SatEph).
 *  Instantiates a model object and returns a HANDLE that must be passed to
 *  each of the other functions in this wrapper.
 *
 *  @param szModel
 *      type of orbit propagation model requested - either "SGP4" or "SatEph"
 *
 *  @return HANDLE
 *      handle ID to an instantiated propogator model object
 */
HANDLE OrbitPropStartUp(const char* szModel);

// function OrbitPropShutDown
/**
 *  C interface call to the model destructor.
 *  Destroys the instantiated model object, leaving the pointer null.
 *
 *  @param zHandle
 *      handle ID to an instantiated propogator model object
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
int OrbitPropShutDown(HANDLE zHandle);

// function OrbitPropGetModel
/**
 *
 * C interface support method to retrieve the current propagator type.
 *
 *  @param zHandle
 *      handle ID to an instantiated propogator model object
 *
 *  @param[out] szModel
 *      returned propogator model type
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
int OrbitPropGetModel(HANDLE zHandle, char *szModel);

//-------------------------------------------------------------------
// COrbitPropSGP4 class C interface function definitions
//    No action if incorrect model, report error
//-------------------------------------------------------------------

// function OrbitPropSetWGSChoice
/**
 *
 * C interface support method to assign the WGS Constant for an SPG4 Propagator.
 *
 *  @param zHandle
 *      handle ID to an instantiated propogator model object
 *  @param szWGSIn
 *      WGS Constant to use in the SGP4 Propagator - options are:
 *      "72Old", "72", or "84"
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
int OrbitPropSetWGSChoice(HANDLE zHandle, const char *szWGSIn);

// function OrbitPropGetWGSChoice
/**
 *
 * C interface support method to retrieve the WGS Constant for an SPG4 Propagator.
 *
 *  @param zHandle
 *      handle ID to an instantiated propogator model object
 *
 *  @param[out] szWGSOut
 *      pointer to an allocated char array to recieve the curent SGP4 WGS Constant
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
int OrbitPropGetWGSChoice(HANDLE zHandle, char *szWGSOut);

// function OrbitPropSetOperationMode
/**
 *
 * C interface support method to assign the Operation Mode for an SPG4 Propagator.
 *
 *  @param zHandle
 *      handle ID to an instantiated propogator model object
 *  @param szWGSIn
 *      Operation Mode to use in the SGP4 Propagator - options are:
 *      "AFSPC" or "Improved"
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
int OrbitPropSetOperationMode(HANDLE zHandle, const char *szOpModeIn);

// function OrbitPropGetOperationMode
/**
 *
 * C interface support method to assign the Operation Mode for an SPG4 Propagator.
 *
 *  @param zHandle
 *      handle ID to an instantiated propogator model object
 *
 *  @param[out] szWGSOut
 *      pointer to an allocated char array to recieve the curent SGP4 Operation Mode
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
int OrbitPropGetOperationMode(HANDLE zHandle, char *szOpModeOut);

//-------------------------------------------------------------------
// COrbitProp class Earth Rotation Angle C interface function definitions
//-------------------------------------------------------------------

// function OrbitPropGetEarthRotAngle
/**
 *
 * C interface call into the COrbitProp::getEarthRotAngle method (see COrbitProp.h)
 *    to get the Earth's rotation angle based on MJD
 *
 *  @param zHandle
 *      int: handle ID to an instantiated propogator model object
 *  @param dModJulDate
 *      double: modified julian date for earth rotation angle
 *
 *  @param[out] dEarthRotAngle
 *      double pointer: returned Earth Rotation Angle
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
int OrbitPropGetEarthRotAngle( HANDLE zHandle,
                               const double dModJulDate,
                               double *pdEarthRotAngle);

//-------------------------------------------------------------------
// COrbitProp class Load Elements C interface function definitions
//-------------------------------------------------------------------

// function OrbitPropReset
/**
 *
 * C interface call to clear the loaded MJD times and TLE deck
 *
 *  @param zHandle
 *      int: handle ID to an instantiated propogator model object
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
int OrbitPropReset(HANDLE zHandle);

// function OrbitPropClearTwoLineElements
/**
 *
 * C interface call to clear the loaded TLE deck
 *
 *  @param zHandle
 *      handle ID to an instantiated propogator model object
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
int OrbitPropClearTwoLineElements(HANDLE zHandle);

// function OrbitPropReadTwoLineElements
/**
 *
 * C interface call into the COrbitProp::readTwoLineElements method (see COrbitProp.h)
 *    to load TLEs from file
 *
 *  @param zHandle
 *      handle ID to an instantiated propogator model object
 *  @param szFilename
 *      string filename containing the TLE
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
int OrbitPropReadTwoLineElements( HANDLE zHandle,
                                  const char *szFilename );

// function OrbitPropLoadTwoLineElements
/**
 *
 * C interface call into the COrbitProp::loadTwoLineElements method (see COrbitProp.h)
 *    to load TLEs from a pair of strings
 *
 *  @param zHandle
 *      handle ID to an instantiated propogator model object
 *  @param szTleLine1
 *      string containing the formatted first line of TLE information
 *  @param szTleLine2
 *      string containing the formatted second line of TLE information
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
int OrbitPropLoadTwoLineElements( HANDLE zHandle,
                                  const char *szTleLine1,
                                  const char *szTleLine2 );

// function OrbitPropClearMeanElements
/**
 *
 * C interface call to clear the loaded Mean Elements
 *
 *  @param zHandle
 *      handle ID to an instantiated propogator model object
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
int OrbitPropClearMeanElements(HANDLE zHandle);

// function OrbitPropLoadMeanElements
/**
 *
 * C interface call into COrbitProp::loadMeanElements method (see COrbitProp.h)
 *    to load a set of mean elements
 *
 *  @param zHandle
 *      handle ID to an instantiated propogator model object
 *  @param dRefTime
 *      double: element reference time, in MJD or Composite form
 *  @param dInclination
 *      double: orbital inclination [degrees]
 *  @param dRightAscension
 *      double: orbit's right ascension of ascending node value [degrees]
 *  @param dEccentricity
 *      double: orbit eccentricity [unitless]
 *  @param dArgofPerigee
 *      double: orbit's argument of perigee [degrees]
 *  @param dMeanAnomaly
 *      double: orbit's mean anomaly value associated with reference time [degrees]
 *  @param dMeanMotion
 *      double: number of orbits(+fraction) during a 24 hour period
 *  @param dMeanMotion1stDerivative
 *      double: first derivative of mean motion value (ie orbital period change rate)
 *  @param dMeanMotion2ndDerivative
 *      double: second deriviative of mean motion value 
 *  @param dBstar
 *      double: drag coefficient value [unitless]
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
int OrbitPropLoadMeanElements(HANDLE zHandle,
                                const double dRefTime,
                                const double dInclination,
                                const double dRightAscension,
                                const double dEccentricity,
                                const double dArgofPerigee,
                                const double dMeanAnomaly,
                                const double dMeanMotion,
                                const double dMeanMotion1stDerivative,
                                const double dMeanMotion2ndDerivative,
                                const double dBstar);

// function OrbitPropClearClassicalElements
/**
 *
 * C interface call to clear the loaded Classical Elements
 *
 *  @param zHandle
 *      handle ID to an instantiated propogator model object
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
int OrbitPropClearClassicalElements(HANDLE zHandle);

// function OrbitPropLoadClassicalElements
/**
 *
 * C interface call into COrbitProp::loadClassicalElements method (see COrbitProp.h)
 *    to load a set of mean elements
 *
 *  @param zHandle
 *      handle ID to an instantiated propogator model object
 *  @param dRefTime
 *      double: element reference time, in MJD or Composite form
 *  @param dInclination
 *      double: orbital inclination [degrees]
 *  @param dSemiMajorAxis
 *      double: orbit semimajor axis (preferred in km, but Re unit detected)
 *  @param dEccentricity
 *      double: orbit eccentricity [unitless]
 *  @param dRightAscension
 *      double: orbit's right ascension of ascending node value [degrees]
 *  @param dArgofPerigee
 *      double: orbit's argument of perigee [degrees]
 *  @param dMeanAnomaly
 *      double: orbit's mean anomaly value associated with reference time [degrees]
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
int OrbitPropLoadClassicalElements(HANDLE zHandle,
                                 const double dRefTime,
                                 const double dInclination,
                                 const double dSemiMajorAxis,
                                 const double dEccentricity,
                                 const double dRightAscension,
                                 const double dArgOfPerigee,
                                 const double dMeanAnomaly);

// function OrbitPropClearSolarElements
/**
 *
 * C interface call to clear the loaded Solar Elements
 *
 *  @param zHandle
 *      handle ID to an instantiated propogator model object
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
int OrbitPropClearSolarElements(HANDLE zHandle);

// function OrbitPropLoadSolarElements
/**
 *
 * C interface call into COrbitProp::loadSolarElements method (see COrbitProp.h)
 *    to load a set of solar elements
 *
 *  @param zHandle
 *      handle ID to an instantiated propogator model object
 *  @param dRefTime
 *      double: element reference time, in MJD or Composite form
 *  @param dInclination
 *      double: orbital inclination [degrees]
 *  @param dApogee
 *      double: orbit's maximum height [km], above Earth's 6371.2km radius
 *  @param dPerigee
 *      double: orbit's minimum height [km], above Earth's 6371.2km radius
 *  @param dLocalTimeofApogee
 *      double: local time [hours.frac] of orbit's apogee, for this reference time
 *  @param dLocalTimeofMaxIncl
 *      double: local time [hours.frac] of orbit's maximum latitude point, for this reference time
 *  @param dMeanAnomaly
 *      double: orbit's mean anomaly value associated with reference time [degrees]
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
int OrbitPropLoadSolarElements(HANDLE zHandle,
                                 const double dRefTime,
                                 const double dInclination,
                                 const double dApogee,
                                 const double dPerigee,
                                 const double dLocalTimeofApogee,
                                 const double dLocalTimeofMaxIncl,
                                 const double dMeanAnomaly);

// function OrbitPropClearSimpleElements
/**
 *
 * C interface call to clear the loaded Simople Elements
 *
 *  @param zHandle
 *      handle ID to an instantiated propogator model object
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
int OrbitPropClearSimpleElements(HANDLE zHandle);

// function OrbitPropLoadSimpleElements
/**
 *
 * C interface call into COrbitProp::loadSimpleElements method (see COrbitProp.h)
 *    to load a set of simple elements
 *
 *  @param zHandle
 *      handle ID to an instantiated propogator model object
 *  @param dRefTime
 *      double: element reference time, in MJD or Composite form
 *  @param dInclination
 *      double: orbital inclination [degrees]
 *  @param dApogee
 *      double: orbit's maximum height [km], above Earth's 6371.2km radius
 *  @param dPerigee
 *      double: orbit's minimum height [km], above Earth's 6371.2km radius
 *  @param dRightAscension
 *      double: orbit's right ascension of ascending node value [degrees]
 *  @param dArgofPerigee
 *      double: orbit's argument of perigee [degrees]
 *  @param dMeanAnomaly
 *      double: orbit's mean anomaly value associated with reference time [degrees]
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
int OrbitPropLoadSimpleElements(HANDLE zHandle,
                                  const double dRefTime,
                                  const double dInclination,
                                  const double dApogee,
                                  const double dPerigee,
                                  const double dRightAscension,
                                  const double dArgofPerigee,
                                  const double dMeanAnomaly);

// function OrbitPropClearGeoElements
/**
 *
 * C interface call to clear the loaded Geo Elements
 *
 *  @param zHandle
 *      handle ID to an instantiated propogator model object
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
int OrbitPropClearGeoElements(HANDLE zHandle);

// function OrbitPropLoadGeoElements
/**
 *
 * C interface call into COrbitProp::loadGeoElements method (see COrbitProp.h)
 *    to load a set of geosynchronous elements
 *
 *  @param zHandle
 *      handle ID to an instantiated propogator model object
 *  @param tRefTime
 *      CTimeValue: element reference time, using CTimeValue object
 *  @param dLongitude
 *      double: orbital Geosynchronous position, in East Longitude degrees
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
int OrbitPropLoadGeoElements(HANDLE zHandle,
                               const double dRefTime,
                               const double dLongitude);

// function OrbitPropClearElementSets
/**
 *
 * C interface call into the COrbitProp::loadTwoLineElements method (see COrbitProp.h)
 *    to clear all TLE information
 *
 *  @param zHandle
 *      handle ID to an instantiated propogator model object
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
int OrbitPropClearElementSets(HANDLE zHandle);

// function OrbitPropCheckElementSets
/**
 *
 * C interface call into the COrbitProp::checkTwoLineElements method (see COrbitProp.h)
 *    to verify current TLE information
 *
 *  @param zHandle
 *      handle ID to an instantiated propogator model object
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
int OrbitPropCheckElementSets( HANDLE zHandle );

//function OrbitPropSetTimes
/**
 *
 * C interface to assign the ephemeris time range and increment and return size
 *
 *  @param zHandle
 *      handle ID to an instantiated propogator model object
 *  @param dStartTime
 *      double: starting time for ephemeris, in Composite or MJD form
 *  @param dEndTime
 *      double: ending time for ephemeris, in Composite or MJD form
 *  @param dTimeStep
 *      double: number of seconds between ephemeris time entries
 *
 *  @return int
 *      >0: number of time entries (success), else fail (see GenericModelEnum.h)
 */
int OrbitPropSetTimes( HANDLE zHandle,
                       const double dStartTime,
                       const double dEndTime,
                       const double dTimeStep );

//function OrbitPropSetTimes
/**
 *
 * C interface to assign the set of times for ephemeris generation
 *
 *  @param zHandle
 *      handle ID to an instantiated propogator model object
 *  @param iNumEphemeris
 *      number of elements in pdaTimes
 *  @param pdaTimes
 *      array of element times to be used (either MJD or Composite format)
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
int OrbitPropSetTimesArray( HANDLE zHandle,
                            int iNumTimes,
                            double* const pvdTimes );

//function OrbitPropGetTimes
/**
 *
 * C interface to assign the ephemeris time range and increment and get set size
 *
 *  @param zHandle
 *      handle ID to an instantiated propogator model object
 *
 *  @param[out] pdaTimes
 *      array of currently specified element times in model
 *  @param[out] iNumEphemeris
 *      number of elements in pdaTimes
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
int OrbitPropGetTimesArray( HANDLE zHandle,
                            double* const pvdTimes,
                            int* iNumTimes );

//function OrbitPropGetNumTimes
/**
 *
 * C interface to retrieve the ephemeris time range and increment and set size
 *
 *  @param zHandle
 *      handle ID to an instantiated propogator model object
 *
 *  @return int
 *      number of current time values specified in the model
 */
int OrbitPropGetNumTimes( HANDLE zHandle );

//function OrbitPropGetNumTLEs
/**
 *
 * C interface call to get the number of specified TLE pairs
 *
 *  @param zHandle
 *      handle ID to an instantiated propogator model object
 *
 *  @return int
 *      number of currently specified TLE pairs in the model
 */
int OrbitPropGetNumTLEs( HANDLE zHandle );

//function OrbitPropGetTLEPair
/**
 *
 * C interface call to get a specified TLE pair
 *
 *  @param zHandle
 *      handle ID to an instantiated propogator model object
 *  @param index
 *      int: index of TLE entries to retrieve
 *
 *  @param[out] szTLELine1
 *      string: first line of requested TLE entry
 *  @param[out] szTLELine2
 *      string: second line of requested TLE entry
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
int OrbitPropGetTLEpair( HANDLE zHandle,
                         const int index,
                         char* szTLELine1,
                         char* szTLELine2 );

//-------------------------------------------------------------------
// COrbitProp class Compute Ephemeris C interface function definitions
//-------------------------------------------------------------------
// function OrbitPropComputeEphemeris
/**
 *
 * C interface call into the COrbitProp::computeEphem method (see COrbitProp.h)
 *    to compute the ephemeris for the current elements, times and propagator
 *
 *  @param zHandle
 *      handle ID to an instantiated propogator model object
 *
 *  @param[out] pdTime
 *      double array of formatted time of report (MJD)
 *  @param[out] pdPositionX
 *      double array of X distance in km
 *  @param[out] pdPositionY
 *      double array of Y distance in km
 *  @param[out] pdPositionZ
 *      double array of Z distance in km
 *  @param[out] pdVelocityX
 *      double array of X velocity in km/s
 *  @param[out] pdVelocityY
 *      double array of Y velocity in km/s
 *  @param[out] pdVelocityZ
 *      double array of Z velocity in km/s
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
int OrbitPropComputeEphemeris(HANDLE zHandle,
                                double* const pdTime,
                                double* const pdPositionX,
                                double* const pdPositionY,
                                double* const pdPositionZ,
                                double* const pdVelocityX,
                                double* const pdVelocityY,
                                double* const pdVelocityZ);

// function OrbitPropComputeEphemerisArrays
/**
 *
 * C interface call into the COrbitProp::computeEphem method (see COrbitProp.h)
 *    to compute the ephemeris for the current propagator returnin grouped values
 *
 *  @param zHandle
 *      handle ID to an instantiated propogator model object
 *
 *  @param[out] pdTime
 *      double array of formatted time of report (MJD)
 *  @param[out] pdPositions
 *      double 2D array of X,Y,Z distances in km (iNumEphemeris x 3) declared as 1D block
 *      declared as 1D block, where each X is index i*3, Y is i*3+1, Z is i*3+2
 *  @param[out] pdVelocities
 *      double 2D array of X,Y,Z velocities in km/s (iNumEphemeris x 3) declared as 1D block
 *      declared as 1D block, where each X is index i*3, Y is i*3+1, Z is i*3+2
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
int OrbitPropComputeEphemerisArrays(HANDLE zHandle,
                                      double* const pdTime,
                                      double* const pdPositions,
                                      double* const pdVelocities);

// function OrbitPropConvertStateVectorToTLEs
/**
 *
 * C interface call into the COrbitProp::convertStateVectorToTLEs method (see COrbitProp.h)
 *
 *  @param zHandle
 *      handle ID to an instantiated propogator model object
 *  @param dRefTime
 *      Reference time, in MJD form
 *  @param dPositionX
 *      X coordinate of position in GEI, km
 *  @param dPositionY
 *      Y coordinate of position in GEI, km
 *  @param dPositionZ
 *      Z coordinate of position in GEI, km
 *  @param dVelocityX
 *      X coordinate of velocity in GEI, km/sec
 *  @param dVelocityY
 *      Y coordinate of velocity in GEI, km/sec
 *  @param dVelocityZ
 *      Z coordinate of velocity in GEI, km/sec
 *  @param dDecay
 *      Bstart decay factor (default to zero)
 *
 *  @param[out] szTLELine1
 *      standard format tle line one string
 *  @param[out] szTLELine2
 *      standard format tle line two string
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
int OrbitPropConvertStateVectorToTLEs(HANDLE zHandle,
                                        const double dStartTime,
                                        const double dPositionX,
                                        const double dPositionY,
                                        const double dPositionZ,
                                        const double dVelocityX,
                                        const double dVelocityY,
                                        const double dVelocityZ,
                                        const double dDecay,
                                        char* szTLELine1,
                                        char* szTLELine2);


/**
 *  C interface call into message
 *
 *  @return int
 */
int OrbitPropMessage1(HANDLE zHandle,
                        int code,
                        char* cMessage);

/**
 *  C interface call into message
 *
 *  @return int
 */
int OrbitPropMessage2(HANDLE zHandle,
                        char* cMessage);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // CORBITPROP_C_H

