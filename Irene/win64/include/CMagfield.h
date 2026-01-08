/***********************************************************************
 File: CMagfield.h

 Description:

   Declarations for top-level magnetic field model library of methods.

 Rights and Restrictions:

   Copyright 2022 Atmospheric and Environmental Research, Inc. (AER)

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

   VERSION            DATE       NOTES
     1.0       Fri Aug 15, 2008   First release

*/

#include "GenericModel.h"
#include "CMagFieldErrors.h"
#include "CMagFieldLib.h"
#include "CMagfieldEnum.h"
#include "CTimeValue.h"

#ifndef CMAGFIELD_MODEL_DEFINE
#define CMAGFIELD_MODEL_DEFINE

typedef S3Tuple S3Coord;
typedef S3TupleVec S3CoordVec;
typedef eMagFieldCoordSystemType emfCoordSys;

/**
 * @brief Magfield Full Model Interface, thread-safe
 *
 * CMagfield - Magfield Full Model Interface, thread-safe
 *
 * CMagfield Options Restrictions
 *
 * These restrictions are relevant for field line computations. Other options
 * combinations may be problematic for other CMagfield functions.
 *
 * 1. Dipole fields (emfOffsetDipole & emfTiltedDipole)
 *   a. cannot have an external field set.  Ie, none (eefNone) only.
 *   b. cannot use McIlwain (elmMcIlwain) as the L-shell trace mode.
 *   c. cannot use Equatorward (efltGsmEquator) as the trace direction
 *
 * 2. IGRF fields (emfIGRF & emfFastIGRF) use an external field
 *    none (eefNone), Olson-Pfitzer (eefOlsonPfitzer) or Tsyganenko 89C (eefTsyganenko89)
 *
 * 3. Field line trace altitude should be chosen >= -100 km for IGRF fields.
 *    For all main fields, the trace altitude has no upper limit, but no field
 *    line will be generated if an input point lies below the altitude limit.
 *
 * **** General Notes about CMagField Class ****
 *
 * * unless otherwise specified, all input coordinates must be given
 *   in Geocentric-Cartesian in km.
 *
 * * unless otherwise specified, all input coordinates must be at or
 *   above the surface of the Earth.
 *   Generally, in this model, a spherically symmetric Earth of radius
 *   6371.2 km is assumed.
 *
 * * Originally, time specifications for this class were in 'GeoSpaceTime'
 *   format (a double value formed like: <year><dayofyear><gmtseconds.frac>).
 * - While this form is still accepted (for backward compatibility purposes),
 *   its use is discouraged in any future development.
 * * Any new development is encouraged to use the CTimeValue class for all
 *   time specifications.  This class accepts/returns a variety of time forms.
 *   These include 'composite' (a more generic name for 'GeoSpaceTime').
 *   Alternatively, the Modified Julian Date value (a double) may also be used
 *   for time specifications.
 *
 */

class CMagfield:public GenericModel{
public:
  CMagfield();

  ~CMagfield();

/*------------------------------------------------------------------*/
// function CMagfield::Initialize
/**
 *
 * Initializes a CMagfield Object.
 *
 *    @param sDBFullPath
 *           string: file path to IGRF database.
 *
 *    @returns
 *           eNoError or eGENERIC_ERROR_CODE enumerated error code
 */
  eGENERIC_ERROR_CODE Initialize( const string& sDBFullPath );

/*------------------------------------------------------------------*/
  // No-COM: The following preloads part of the DB into class memory
  // based on tTime or dTime; not needed for STK interface.
/*------------------------------------------------------------------*/
// function CMagfield::Initialize
/**
 *
 * Initializes a CMagfield Object. Preloads part of the DB into class
 *  memory based on tTime; not needed for STK interface.
 *
 *    @param sDBFullPath
 *           string: file path to IGRF database.
 *    @param tTime
 *           CTimeValue: Time value object.
 *
 *    @returns
 *           eMAGFIELD_ERROR_CODE: emfNoError success, else error code
 */
  eGENERIC_ERROR_CODE Initialize( const string& sDBFullPath,
                                  const CTimeValue& tTime );

/*------------------------------------------------------------------*/
// function CMagfield::Initialize
/**
 *
 * Initializes a CMagfield Object. Preloads part of the DB into class
 *  memory based on dTime; not needed for STK interface.
 *
 *    @param sDBFullPath
 *           string: file path to IGRF database.
 *    @param dTime
 *           double: Time in MJD or composite ("GeoSpace") form.
 *
 *    @returns
 *           eMAGFIELD_ERROR_CODE: emfNoError success, else error code
 */
  eGENERIC_ERROR_CODE Initialize( const string& sDBFullPath,
                                  const double& dTime );

/*------------------------------------------------------------------*/
  // No-COM: Copy Constructors however can be used as samples for the
  // COM-layer equivalent
  // No-COM: copy constructor (CMagfield* pcmf): copy 'this' state into pcmf
/*------------------------------------------------------------------*/
// function CMagfield::CMagfield
/**
*
* Copy a CMagfield Object.  All copies initialize state to current
* IGRF time; could cause slight differences in state between child and
* parent if current SR2 time is different.
*
*    @param cMF
*           CMagfield new CMagfield object to be initialized
*
*    @returns
*           eNoError or eMAGFIELD_ERROR_CODE enumerated error code
*/
  CMagfield( const CMagfield& pcmf );
/*------------------------------------------------------------------*/
  // No-COM: Assignment operator
  CMagfield& operator=(const CMagfield & other);

/*------------------------------------------------------------------*/
// function CMagfield::Cmagfield
/**
*
* Copy a CMagfield Object.  All copies initialize state to current
* IGRF time; could cause slight differences in state between child and
* parent if current SR2 time is different.
*
*    @param cMF
*           CMagfield: new CMagfield object to be initialized
*
*    @returns
*           eNoError or eMAGFIELD_ERROR_CODE enumerated error code
*/
  eGENERIC_ERROR_CODE Copy( CMagfield& cMf );

/*------------------------------------------------------------------*/
// function CMagfield::CopyFrom
/**
*
* Copy a CMagfield Object.  All copies initialize state to current
* IGRF time; could cause slight differences in state between child and
* parent if current SR2 time is different.
*
*    @param cMF
*           CMagfield: a CMagfield object to be copied from
*
*    @returns
*           eNoError or eMAGFIELD_ERROR_CODE enumerated error code
*/
  // copy from old to new new.CopyFrom(old);
  eGENERIC_ERROR_CODE CopyFrom( CMagfield& cMf );

/*------------------------------------------------------------------*/
  // ** time handling **

  // update rates are stored internally in decimal year to facilitate
  // accurate comparisons.  set and get values are translated
  // appropriately.

  // setIgrfUpdateRate(): update rate for IGRF main-field model;
  /// Update rate for IGRF main-field model
  ///
  /// @param dIn sets cdIgrfUpdateRate
  /// default is 1 day; 0 means fixed at initial date; unit is
  /// fractional days
/*------------------------------------------------------------------*/
// function CMagfield::setIgrfUpdateRate
/**
 *
 * Set IGRF update rate.
 *
 * @param dIn
 *        double: IGRF update rate [days.frac].
 *
 * @returns
 *        None
 */
  void setIgrfUpdateRate( const double& dIn ) { cdIgrfUpdateRate = dIn; };

/*------------------------------------------------------------------*/
  /// Update rate for IGRF main-field model
/*------------------------------------------------------------------*/
// function CMagfield::getIgrfUpdateRate
/**
 *
 * Get IGRF update rate.
 *
 * @returns
 *        IGRF update rate [days.frac].
 */
  const double getIgrfUpdateRate() { return cdIgrfUpdateRate; };

/*------------------------------------------------------------------*/
  // setSr2UpdateRate(): update rate in fractional hours at which to
  // update the sR2 coordinate transform coefficients (also controls
  // update rate for external field models).  Every IGRF update also
  // updates sR2, so setting this rate greater than the IgrfUpdateRate
  // will have no effect.  Default is to IgrfUpdateRate.
/*------------------------------------------------------------------*/
// function CMagfield::setSr2UpdateRate
/**
 *
 * Set sR2 update rate.
 *
 * @param dIn
 *        double: sR2 update rate [days.frac].
 *
 * @returns
 *        None
 */
  void setSr2UpdateRate( const double& dIn ) { cdSr2UpdateRate = dIn; };

/*------------------------------------------------------------------*/
// function CMagfield::getSr2UpdateRate
/**
 *
 * Get sR2 update rate.
 *
 * @returns
 *        sR2 update rate [days.frac].
 */
  const double getSr2UpdateRate() { return cdSr2UpdateRate; };

/*------------------------------------------------------------------*/
// function CMagfield::updateTime
/**
 *
 * Updates the current IGRF and SR2 internal times if tTime is
 * updateRate greater than the current times.
 *
 *    @param tTime
 *           CTimeValue: Time value object.
 *    @param bIgrfOnly
 *           bool: prevent recursive SR2 updates when true.
 *
 *    @returns
 *           eMAGFIELD_ERROR_CODE: emfNoError success, else error code
 */
  eMAGFIELD_ERROR_CODE updateTime( const CTimeValue& tTime,
                                   bool bIgrfOnly=false );

/*------------------------------------------------------------------*/
  // updateTime(): update the the current IGRF and SR2 internal times
  // if dTime is updateRate greater than the current time.
/*------------------------------------------------------------------*/
// function CMagfield::updateTime
/**
 *
 * Updates the current IGRF if tTime is updateRate greater than the
 * current times.
 *
 *    @param dTime
 *           double: Time in MJD or composite ("GeoSpace") form.
 *
 *    @returns
 *           eMAGFIELD_ERROR_CODE: emfNoError success, else error code
 */
  eMAGFIELD_ERROR_CODE updateTime( const double& dTime );

/*------------------------------------------------------------------*/
  // for internal or diagnostic use only:
  double getNewUpdateTime ( const double& dMjdTarget,
                            const double& dCadence,
                            bool bHours=false );

/*------------------------------------------------------------------*/
  // forceSetTime(): overrides all update rates and sets the time to
  // the argument (forces updates of Igrf and Sr2 coefficients)
/*------------------------------------------------------------------*/
// function CMagfield::forceSetTime
/**
 *
 * Forces updates the current IGRF and SR2 internal times (causing
 * underlying coefficients to reload.)
 *
 *    @param tTime
 *           CTimeValue: time value object
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE forceSetTime( const CTimeValue& tTime );

/*------------------------------------------------------------------*/
// function CMagfield::forceSetTime
/**
 *
 * Forces updates the current IGRF and SR2 internal times (causing
 * underlying coefficients to reload.)
 *
 *    @param dTime
 *           double: time in MJD or composite ("GeoSpace") form
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE forceSetTime( const double& dTime );

/*------------------------------------------------------------------*/
  // forceUpdateSR2(): override Sr2UpdateRate, set Sr2 current time to
  // tTime (or dTime) and update Sr2 coefficients.  In order to keep in
  // approximate sync with the IGRF model coefficients, updateTime
  // will also be called.
/*------------------------------------------------------------------*/
// function CMagfield::forceUpdateSR2
/**
 *
 * Forces update of the SR2 internal times (causing underlying
 * coefficients to reload.)  Calls updateTime() to force check on
 * whether to update IGRF.
 *
 *    @param tTime
 *           CTimeValue: time value object
 *    @param bCheckIgrf
 *           bool: keep in approximate sync with IGRF coefficients
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE forceUpdateSR2( const CTimeValue& tTime,
                                       bool bCheckIgrf=true );

/*------------------------------------------------------------------*/
// function CMagfield::forceUpdateSR2
/**
 *
 * Forces update of the SR2 internal times (causing underlying
 * coefficients to reload.)  Calls updateTime() to force check on
 * whether to update IGRF.
 *
 *    @param dTime
 *           double: time in MJD or composite ("GeoSpace") form
 *    @param bCheckIgrf
 *           bool: keep in approximate sync with IGRF coefficients
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE forceUpdateSR2( const double& dTime,
                                       bool bCheckIgrf=true );

/*------------------------------------------------------------------*/
// function CMagfield::getCurIgrfTime
/**
 *
 * Get current IGRF time associated with internal coefficients
 * (GeoSpace time format by default, MJD if specified).
 *
 *    @param bMjd
 *           bool: if false (default) return time in composite
 *            ("GeoSpace") form, else return time in MJD form
 *
 *    @returns
 *           dTime (GeoSpace time format (default), or MJD format
 *            if bMjd=true)
 */
  double getCurIgrfTime( bool bMjd=false );

/*------------------------------------------------------------------*/
// function CMagfield::getCurSr2Time
/**
 *
 * Get current IGRF time associated with internal coefficients
 * (GeoSpace time format by default, MJD if specified).
 *
 * @param bMjd
 *        bool: if false (default) return time in composite
 *         ("GeoSpace") form, else return time in MJD form
 *
 * @returns
 *        dTime (GeoSpace time format (default), or MJD format
 *         if bMjd=true))
 */
  double getCurSr2Time( bool bMjd=false );

/*------------------------------------------------------------------*/
  // overrideYear(): override the Year only on all time inputs,
  // replacing it with argument.  This remains in effect until
  // overrideYear is called again with argument 0.0.  This facilitates
  // calls by RadEnv where the user wants the database access to more
  // closely match what was used in building the Crres or Nasa
  // databases.  Argument is 4-digit integer year.
  void overrideYear( const int& iYear ) { ciOverRideYear = iYear; };

  const int getOverrideYear() { return ciOverRideYear; };

/*------------------------------------------------------------------*/
// function CMagfield::getDipParams
/**
 *
 * Get Dipole parameters.
 *
 * @param[out] pdDimo
 *        double: Returned magnetic dipole moment.
 * @param[out] pdTheta
 *        double: Returned dipole polar angle.
 * @param[out] pdPhi
 *        double: Returned dipole azimuthal angle.
 * @param[out] pdEta
 *        double: Returned geocentric equatorial coordinate.
 * @param[out] pdZeta
 *        double: Returned geocentric equatorial coordinate.
 * @param[out] pdXi
 *        double: Returned geocentric equatorial coordinate.
 * @param[out] pdvGeoToMag
 *        dvector: Returned geocentric to magnetic transformation coefficients
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE getDipParams( double* pdDimo, 
                                     double* pdTheta,
                                     double* pdPhi, 
                                     double* pdEta,
                                     double* pdZeta, 
                                     double* pdXi,
                                     dvector* pdvGeoToMag );

/*------------------------------------------------------------------*/
  // The following sets and gets must be callable through COM
  // The gets are required for the COM-layer Copy technique
  // **** Field Model Specification Methods ****
  // see eMainField enum above for choices
/*------------------------------------------------------------------*/
// function CMagfield::setMainField
/**
 *
 * Sets the main field model used by CMagfield object.
 *
 *    @param emfIn
 *           eMainField: Main field enumeration,
 *           * emfIGRF         = IGRF
 *           * emfFastIGRF     = Fast IGRF
 *           * emfOffsetDipole = Offset dipole
 *           * emfTiltedDipole = Tilted dipole
 *
 *    @returns 
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE setMainField( const eMainField& emfIn );

/*------------------------------------------------------------------*/
// function CMagfield::getMainFieldEnum
/**
 *
 * Get the main field model enumeration.
 *
 *    @returns
 *           Main field model enumeration
 */
  const eMainField getMainFieldEnum() { return ceMainField; };

/*------------------------------------------------------------------*/
// function CMagfield::setMainField
/**
 *
 * Sets the external field model used by CMagfield object.
 *
 *   @param strMainField
 *          string: string representation of main field model (case insensitive)
 *          * "IGRF"                    = IGRF
 *          * "FastIGRF"     | "Fast"   = Fast IGRF
 *          * "OffsetDipole" | "Offset" = Offset dipole
 *          * "TiltedDipole" | "Tilted" = Tilted dipole
 *
 *   @returns 
 *          emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE setMainField( const string strMainField );

/*------------------------------------------------------------------*/
// function CMagfield::getMainField
/**
 *
 * Get the string representation of the main field model
 *
 * @returns
 *        Main field string.
 */
  string getMainField();

/*------------------------------------------------------------------*/
// function CMagfield::getDBfp
/**
 *
 * Get string containing database name, set in call to initialize.
 *
 *   @returns
 *          Database name
 */
  const string getDBfp() { return csDBfp; };

/*------------------------------------------------------------------*/
// function CMagfield::setExternalField
/**
 *
 * Sets the external field model used by CMagfield object.
 *
 *   @param eefIn
 *          eExternalField: External field enumeration,
 *          * eefNone         = No external field
 *          * eefOlsonPfitzer = Olson-Pfitzer
 *          * eefTsyganenko89 = Tsyganenko 89/C
 *
 *   @returns 
 *         emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE setExternalField( const eExternalField& eefIn );

/*------------------------------------------------------------------*/
// function CMagfield::getExternalFieldEnum
/**
 *
 * Get the external field enumeration.
 *
 *   @returns
 *          External field enumeration
 */
  const eExternalField getExternalFieldEnum() { return ceExternalField; };

/*------------------------------------------------------------------*/
// function CMagfield::setExternalField
/**
 *
 * Sets the external field model used by CMagfield object.
 *
 *   @param strExternalField
 *          string: string representation of external field (case insensitive)
 *          * "None"                            = No external field
 *          * "OlsonPfitzer" | "OP"             = Olson-Pfitzer
 *          * "Tsyganenko89" | "Tsyg89" | "T89" = Tsyganenko 89/C
 *
 *   @returns 
 *          emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE setExternalField( const string strExternalField );

/*------------------------------------------------------------------*/
// function CMagfield::getExternalField
/**
 *
 * Get the string representation of the external field.
 *
 *   @returns
 *         External field string
 */
  string getExternalField();

/*------------------------------------------------------------------*/
// function CMagfield::getMagMoment
/**
 *
 * Get the earth magnetic dipole moment.
 *
 *   @returns
 *          Earth magnetic dipole moment
 */
  double getMagMoment() { return csCMagField->getDimo(); };

  // Return general state information
/*------------------------------------------------------------------*/

  // No-COM: The following two class utility routines are not required
  // across COM.
/*------------------------------------------------------------------*/
// function CMagfield::isInitialized
/**
 *
 * Get the class initialization state.
 *
 *   @returns
 *          Initialization state
 */
  const bool isInitialized() { return cbIsInitialized; }

/*------------------------------------------------------------------*/
  eMAGFIELD_ERROR_CODE getMagFieldLibPtr( CMagFieldLib *psMagfield )
    { return emfNotImplemented; };
  //                       {psMagfield=&csCMagField;return emfNoError;};


  // code clean-up: the calls with time and a single coordinate in the
  // argument list are the ones to get pulled up through COM.  In
  // general we will just supply two overloads: a vector version with
  // and without a time argument. - raq, Aug 2009
/*------------------------------------------------------------------*/
  // **** Compute Methods ****
  // ** Bfield Computations **
/*------------------------------------------------------------------*/
// function CMagfield::computeBfield
/**
 *
 * Compute B field at point.
 *
 *    @param tTime
 *           CTimeValue: Time value object.
 *    @param s3C
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] ps3B
 *           S3Tuple: Returned B nT tuple, corresponding to Cartesian coordinates.
 *    @param[out] pdBmag
 *           double: Returned magnitude of B.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeBfield( const CTimeValue& tTime,
                                      const S3Coord& s3C,
                                      S3Tuple* ps3B,
                                      double* pdBmag );

/*------------------------------------------------------------------*/
// function CMagfield::computeBfield
/**
 *
 * Compute B field at point.
 *
 *    @param dTime
 *           double: Time in MJD or composite ("GeoSpace") form.
 *    @param s3C
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] ps3B
 *           S3Tuple: Returned B nT tuple, corresponding to Cartesian coordinates.
 *    @param[out] pdBmag
 *           double: Returned magnitude of B.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeBfield( const double& dTime,
                                      const S3Coord& s3C,
                                      S3Tuple* ps3B,
                                      double* pdBmag );

/*------------------------------------------------------------------*/
  // no-com versions
/*------------------------------------------------------------------*/
// function CMagfield::computeBfield
/**
 *
 * Compute B field at point.
 *
 *    @param s3C
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] ps3B
 *           S3Tuple: Returned B nT tuple, corresponding to Cartesian coordinates.
 *    @param[out] pdBmag
 *           double: Returned magnitude of B.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeBfield( const S3Coord& s3C,
                                      S3Tuple* ps3B,
                                      double* pdBmag );

/*------------------------------------------------------------------*/
// function CMagfield::computeBfield
/**
 *
 * Compute B field at point.
 *
 *    @param s3C
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] ps3B
 *           S3Tuple: Returned B nT tuple, corresponding to Cartesian coordinates.
 *    @param[out] pdBmag
 *           double: Returned magnitude of B.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeBfield( const S3Coord& s3C,
                                      S3Tuple& s3B,
                                      double& dBmag );

/*------------------------------------------------------------------*/
// function CMagfield::computeBfield
/**
 *
 * Compute B field at given vector of points.
 *
 *    @param s3Cv
 *           S3CoordVec: Vector of position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] ps3vB
 *           S3TupleVec: Returned vector of B nT, corresponding to Cartesian coordinates.
 *    @param[out] pdvBmag
 *           dvector: Returned vector of magnitudes of B.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeBfield( const S3CoordVec& s3Cv,
                                      S3TupleVec* ps3vB,
                                      dvector* pdvBmag );

/*------------------------------------------------------------------*/
// function CMagfield::computeBfield
/**
 *
 * Compute B field at point.
 *
 *    @param tTime
 *           CTimeValue: Time value object.
 *    @param s3C
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] s3B
 *           S3Tuple: Returned B nT tuple, corresponding to Cartesian coordinates.
 *    @param[out] pdBmag
 *           double: Returned magnitude of B.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeBfield( const CTimeValue& tTime,
                                      const S3Coord& s3C,
                                      S3Tuple& s3B,
                                      double& dBmag );

/*------------------------------------------------------------------*/
// function CMagfield::computeBfield
/**
 *
 * Compute B field at point.
 *
 *    @param dTime
 *           double: Time in MJD or composite ("GeoSpace") form.
 *    @param s3C
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] ps3B
 *           S3Tuple: Returned B nT tuple, corresponding to Cartesian coordinates.
 *    @param[out] pdBmag
 *           double: Returned magnitude of B.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeBfield( const double& dTime,
                                      const S3Coord& s3C,
                                      S3Tuple& s3B,
                                      double& dBmag );

/*------------------------------------------------------------------*/
// function CMagfield::computeBfield
/**
 *
 * Compute B field at given vector of points.
 *
 *    @param vtTime:
 *           vector<CTimeValue>: Vector of time value objects.
 *    @param s3Cv
 *           S3CoordVec: Vector of position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] ps3vB
 *           S3TupleVec: Returned vector of B nT, corresponding to Cartesian coordinates.
 *    @param[out] pdvBmag
 *           dvector: Returned vector of magnitudes of B.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeBfield( const vector<CTimeValue>& vtTime,
                                      const S3CoordVec& s3Cv,
                                      S3TupleVec* ps3vB,
                                      dvector* pdvBmag );

/*------------------------------------------------------------------*/
// function CMagfield::computeBfield
/**
 *
 * Compute B field at given vector of points.
 *
 *    @param vdTime:
 *           dvector: Vector of times, in MJD or composite ("GeoSpace") form.
 *    @param s3Cv
 *           S3CoordVec: Vector of position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] ps3vB
 *           S3TupleVec: Returned vector of B nT, corresponding to Cartesian coordinates.
 *    @param[out] pdvBmag
 *           dvector: Returned vector of magnitudes of B.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeBfield( const dvector& vdTime,
                                      const S3CoordVec& s3Cv,
                                      S3TupleVec* ps3vB,
                                      dvector* pdvBmag );

/*------------------------------------------------------------------*/
  // computeLdip()/computeLm(): two computation techniques: simple
  // dipole and McIlwain (L-shell tracing.)  The input in each case
  // must be in Geocentric Cartesian km.
/*------------------------------------------------------------------*/
// function CMagfield::computeLdip
/**
 *
 * Compute L value using Dipole method.
 *
 *    @param tTime
 *           CTimeValue: Time value object.
 *    @param s3Cv
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] pdLdip
 *           double: Returned L value.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLdip( const CTimeValue& tTime,
                                    const S3Coord& s3C,
                                    double* pdLdip );

/*------------------------------------------------------------------*/
// function CMagfield::computeLdip
/**
 *
 * Compute L value using Dipole method.
 *
 *    @param dTime
 *           double: Time in MJD or composite ("GeoSpace") form.
 *    @param s3Cv
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] pdLdip
 *           double: Returned L value.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLdip( const double& dTime,
                                    const S3Coord& s3C,
                                    double* pdLdip );

/*------------------------------------------------------------------*/
// function CMagfield::computeLm
/**
 *
 * Compute L value using McIlwain method.
 *
 *    @param tTime
 *           CTimeValue: Time value object.
 *    @param s3C
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] pdLm
 *           double: Returned L value.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLm( const CTimeValue& tTime,
                                  const S3Coord& s3C,
                                  double* pdLm );

/*------------------------------------------------------------------*/
// function CMagfield::computeLm
/**
 *
 * Compute L value using McIlwain method.
 *
 *    @param dTime
 *           double: Time in MJD or composite ("GeoSpace") form.
 *    @param s3C
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] pdLm
 *           double: Returned L value.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLm( const double& dTime,
                                  const S3Coord& s3C,
                                  double* pdLm );

  // no-com versions
/*------------------------------------------------------------------*/
// function CMagfield::computeLdip
/**
 *
 * Compute L value using Dipole method.
 *
 *    @param s3Cv
 *           S3CoordVec: Vector of position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] pdLdip
 *           double: Returned L value.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLdip( const S3Coord& s3C,
                                    double* pdLdip );

/*------------------------------------------------------------------*/
// function CMagfield::computeLdip
/**
 *
 * Compute L value using Dipole method.
 *
 *    @param tTime
 *           CTimeValue: Time value object.
 *    @param s3Cv
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] dLdip
 *           double: Returned L value.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLdip( const S3Coord& s3C,
                                    double& dLdip );

/*------------------------------------------------------------------*/
// function CMagfield::computeLdip
/**
 *
 * Compute L value using Dipole method.
 *
 *    @param tTime
 *           CTimeValue: Time value object.
 *    @param s3Cv
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] dLdip
 *           double: Returned L value.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLdip( const CTimeValue& tTime,
                                    const S3Coord& s3C,
                                    double& dLdip );

/*------------------------------------------------------------------*/
// function CMagfield::computeLdip
/**
 *
 * Compute L value using Dipole method.
 *
 *    @param dTime
 *           double: Time in MJD or composite ("GeoSpace") form.
 *    @param s3Cv
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] dLdip
 *           double: Returned L value.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLdip( const double& dTime,
                                    const S3Coord& s3C,
                                    double& dLdip );

/*------------------------------------------------------------------*/
// function CMagfield::computeLdip
/**
 *
 * Compute vector of L values using Dipole method.
 *
 *    @param s3Cv
 *           S3CoordVec: Vector of position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] pdvLdip
 *           dvector: Returned vector of L values.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLdip( const S3CoordVec& s3Cv,
                                    dvector* pdvLdip );

/*------------------------------------------------------------------*/
// function CMagfield::computeLdip
/**
 *
 * Compute vector of L values using Dipole method.
 *
 *    @param vtTime
 *           vector<CTimeValue>: Vector of time value objects.
 *    @param s3Cv
 *           S3CoordVec: Vector of position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] pdvLdip
 *           dvector: Returned vector of L values.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLdip( const vector<CTimeValue>& vtTime,
                                    const S3CoordVec& s3Cv,
                                    dvector* pdvLdip );

/*------------------------------------------------------------------*/
// function CMagfield::computeLdip
/**
 *
 * Compute vector of L values using Dipole method.
 *
 *    @param vdTime
 *           dvector: Vector of times, in MJD or composite ("GeoSpace") form.
 *    @param s3Cv
 *           S3CoordVec: Vector of position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] pvdLdip
 *           dvector: Returned vector of L values.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLdip( const dvector& vdTime,
                                    const S3CoordVec& s3Cv,
                                    dvector* pdvLdip );

/*------------------------------------------------------------------*/
// function CMagfield::computeLm
/**
 *
 * Compute L value using McIlwain method.
 *
 *    @param s3C
 *           S3Coord: Position vector, geocentric Cartesian in km.
 *
 *    @param[out] pdLm
 *           double: Returned L value.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLm( const S3Coord& s3C,
                                  double* pdLm );

/*------------------------------------------------------------------*/
// function CMagfield::computeLm
/**
 *
 * Compute L value using McIlwain method.
 *
 *    @param s3C
 *           S3Coord: Position vector, geocentric Cartesian in km.
 *
 *    @param[out] pdLm
 *           double: Returned L value.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLm( const S3Coord& s3C,
                                  double& dLm );

/*------------------------------------------------------------------*/
// function CMagfield::computeLm
/**
 *
 * Compute L value using McIlwain method.
 *
 *    @param tTime
 *           CTimeValue: Time value object.
 *    @param s3C
 *           S3Coord: Position vector, geocentric Cartesian in km.
 *
 *    @param[out] pdLm
 *           double: Returned L value.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLm( CTimeValue tTime,
                                  const S3Coord& s3C,
                                  double& dLm );

/*------------------------------------------------------------------*/
// function CMagfield::computeLm
/**
 *
 * Compute L value using McIlwain method.
 *
 *    @param dTime
 *           double: Time in MJD or composite ("GeoSpace") form.
 *    @param s3C
 *           S3Coord: Position vector, geocentric Cartesian in km.
 *
 *    @param[out] pdLm
 *           double: Returned L value.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLm( double dTime,
                                  const S3Coord& s3C,
                                  double& dLm );

/*------------------------------------------------------------------*/
// function CMagfield::computeLm
/**
 *
 * Compute L value using McIlwain method.
 *
 *    @param s3Cv
 *           S3CoordVec: Vector of position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] pdvLm
 *           dvector: Returned vector of L values.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLm( const S3CoordVec& s3Cv,
                                  dvector* pdvLdip );

/*------------------------------------------------------------------*/
// function CMagfield::computeLm
/**
 *
 * Compute L value using McIlwain method.
 *
 *    @param vtTime
 *           vector<CTimeValue>: Vector of time value objects.
 *    @param s3Cv
 *           S3CoordVec: Vector of position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] pdvLm
 *           dvector: Returned vector of L values.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLm( const vector<CTimeValue>& vtTime,
                                  const S3CoordVec& s3Cv,
                                  dvector* pdvLm );

/*------------------------------------------------------------------*/
// function CMagfield::computeLm
/**
 *
 * Compute L value using McIlwain method.
 *
 *    @param vdTime
 *           dvector: Vector of times, in MJD or composite ("GeoSpace") form.
 *    @param s3Cv
 *           S3CoordVec: Vector of position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] pdvLm
 *           dvector: Returned vector of L values.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLm( const dvector& vdTime,
                                  const S3CoordVec& s3Cv,
                                  dvector* pdvLm );

/*------------------------------------------------------------------*/
// function CMagfield::computeLBBeq
/**
 *
 * Compute Lm and B magnitude / Beq
 *
 *    @param tTime
 *           CTimeValue: Time value object.
 *    @param s3C
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] pdLm
 *           double: Returned L value.
 *    @param[out] pdBBeq
 *           double: Returned BLocal/BMin
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLBBeq( const CTimeValue& tTime,
                                     const S3Coord& s3C,
                                     double* pdLm,
                                     double* pdBBeq );

/*------------------------------------------------------------------*/
// function CMagfield::computeLBBeq
/**
 *
 * Compute Lm and B magnitude / Beq
 *
 *    @param dTime
 *           double: Time in MJD or composite ("GeoSpace") form.
 *    @param s3C
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] pdLm
 *           double: Returned L value.
 *    @param[out] pdBBeq
 *           double: Returned BLocal/BMin
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLBBeq( const double& dTime,
                                     const S3Coord& s3C,
                                     double* pdLm,
                                     double* pdBBeq );

  // no com
/*------------------------------------------------------------------*/
// function CMagfield::computeLBBeq
/**
 *
 * Compute Lm and B magnitude / Beq
 *
 *    @param s3C
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] pdLm
 *           double: Returned L value.
 *    @param[out] pdBBeq
 *           double: Returned BLocal/BMin
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLBBeq( const S3Coord& s3C,
                                     double* pdLm,
                                     double* pdBBeq );

/*------------------------------------------------------------------*/
// function CMagfield::computeLBBeq
/**
 *
 * Compute Lm and B magnitude / Beq
 *
 *    @param tTime
 *           CTimeValue: Time value object.
 *    @param s3C
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] dLm
 *           double: Returned L value.
 *    @param[out] dBBeq
 *           double: Returned BLocal/BMin
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLBBeq( const S3Coord& s3C,
                                     double& dLm,
                                     double& dBBeq );

/*------------------------------------------------------------------*/
// function CMagfield::computeLBBeq
/**
 *
 * Compute Lm and B magnitude / Beq
 *
 *    @param tTime
 *           CTimeValue: Time value object.
 *    @param s3C
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] dLm
 *           double: Returned L value.
 *    @param[out] dBBeq
 *           double: Returned BLocal/BMin
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLBBeq( CTimeValue tTime,
                                     const S3Coord& s3C,
                                     double& dLm,
                                     double& dBBeq );

/*------------------------------------------------------------------*/
// function CMagfield::computeLBBeq
/**
 *
 * Compute Lm and B magnitude / Beq
 *
 *    @param dTime
 *           double: Time in MJD or composite ("GeoSpace") form.
 *    @param s3C
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] dLm
 *           double: Returned L value.
 *    @param[out] dBBeq
 *           double: Returned BLocal/BMin
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLBBeq( double dTime,
                                     const S3Coord& s3C,
                                     double& dLm,
                                     double& dBBeq );

  // new version to support alternate B/B0 computation at higher level
/*------------------------------------------------------------------*/
// function CMagfield::computeLBBeq
/**
 *
 * Compute Lm, B magnitude, and Beq
 *
 *    @param tTime
 *           CTimeValue: Time value object.
 *    @param s3C
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] pdLm
 *           double: Returned L value.
 *    @param[out] pdBmag
 *           double: Returned B magnitude.
 *    @param[out] pdBeq
 *           double: Returned B equatorial.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLBBeq( const CTimeValue& tTime,
                                     const S3Coord& s3C,
                                     double* pdLm,
                                     double* pdBmag,
                                     double* pdBeq );

/*------------------------------------------------------------------*/
// function CMagfield::computeLBBeq
/**
 *
 * Compute Lm, B magnitude, and Beq
 *
 *    @param dTime
 *           double: Time in MJD or composite ("GeoSpace") form.
 *    @param s3C
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] pdLm
 *           double: Returned L value.
 *    @param[out] pdBmag
 *           double: Returned B magnitude.
 *    @param[out] pdBeq
 *           double: Returned B equatorial.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLBBeq( const double& dTime,
                                     const S3Coord& s3C,
                                     double* pdLm,
                                     double* pdBmag,
                                     double* pdBeq );

  // no com - new version to support alt B/B0 comp at higher level
/*------------------------------------------------------------------*/
// function CMagfield::computeLBBeq
/**
 *
 * Compute Lm, B magnitude, and Beq
 *
 *    @param s3C
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] pdLm
 *           double: Returned L value.
 *    @param[out] pdBmag
 *           double: Returned B magnitude.
 *    @param[out] pdBeq
 *           double: Returned B equatorial.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLBBeq( const S3Coord& s3C,
                                     double* pdLm,
                                     double* pdBmag,
                                     double* pdBeq );

/*------------------------------------------------------------------*/
// function CMagfield::computeLBBeq
/**
 *
 * Compute Lm, B magnitude, and Beq
 *
 *    @param s3C
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] dLm
 *           double: Returned L value.
 *    @param[out] dBmag
 *           double: Returned B magnitude.
 *    @param[out] dBeq
 *           double: Returned B equatorial.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLBBeq( const S3Coord& s3C,
                                     double& dLm,
                                     double& dBmag,
                                     double& dBeq );

/*------------------------------------------------------------------*/
// function CMagfield::computeLBBeq
/**
 *
 * Compute Lm, B magnitude, and Beq
 *
 *    @param tTime
 *           CTimeValue: Time value object.
 *    @param s3C
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] dLm
 *           double: Returned L value.
 *    @param[out] dBmag
 *           double: Returned B magnitude.
 *    @param[out] dBeq
 *           double: Returned B equatorial.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLBBeq( CTimeValue tTime,
                                     const S3Coord& s3C,
                                     double& dLm,
                                     double& dBmag,
                                     double& dBeq );

/*------------------------------------------------------------------*/
// function CMagfield::computeLBBeq
/**
 *
 * Compute Lm, B magnitude, and Beq
 *
 *    @param dTime
 *           double: Time in MJD or composite ("GeoSpace") form.
 *    @param s3C
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] dLm
 *           double: Returned L value.
 *    @param[out] dBmag
 *           double: Returned B magnitude.
 *    @param[out] dBeq
 *           double: Returned B equatorial.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLBBeq( double dTime,
                                     const S3Coord& s3C,
                                     double& dLm,
                                     double& dBmag,
                                     double& dBeq );

/*------------------------------------------------------------------*/
  // fancier overloads
/*------------------------------------------------------------------*/
// function CMagfield::computeLBBeq
/**
 *
 * Compute vector of Lm and B magnitude / Beq
 *
 *    @param s3Cv
 *           S3CoordVec: Vector of position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] dvLm
 *           dvector: Returned vector of L values.
 *    @param[out] dvBBeq
 *           dvector: Returned vector of BLocal/BMin.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLBBeq( const S3CoordVec& s3Cv,
                                     dvector* pdvLm,
                                     dvector* pdvBBeq );

/*------------------------------------------------------------------*/
// function CMagfield::computeLBBeq
/**
 *
 * Compute vector of Lm and B magnitude / Beq
 *
 *    @param vtTime
 *           vector<CTimeValue>: Vector of time value objects.
 *    @param s3Cv
 *           S3CoordVec: Vector of position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] dvLm
 *           dvector: Returned vector of L values.
 *    @param[out] dvBBeq
 *           dvector: Returned vector of BLocal/BMin.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLBBeq( const vector<CTimeValue>& vtTime,
                                     const S3CoordVec& s3Cv,
                                     dvector* pdvLm,
                                     dvector* pdvBBeq );

/*------------------------------------------------------------------*/
// function CMagfield::computeLBBeq
/**
 *
 * Compute vector of Lm and B magnitude / Beq
 *
 *    @param vdTime
 *           dvector: Vector of times, in MJD or composite ("GeoSpace") form.
 *    @param s3Cv
 *           S3CoordVec: Vector of position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] dvLm
 *           dvector: Returned vector of L values.
 *    @param[out] dvBBeq
 *           dvector: Returned vector of BLocal/BMin.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeLBBeq( const dvector& vdTime,
                                     const S3CoordVec& s3Cv,
                                     dvector* pdvLm,
                                     dvector* pdvBBeq );

/*------------------------------------------------------------------*/
  // computeAllBout(): return all normally requested Bfield quant.s in
  // one call, as above.  (These are currently deprecated at COM.)
/*------------------------------------------------------------------*/
// function CMagfield::computeAllBout
/**
 *
 * Return all normally requested Bfield quantities one call.
 *
 *    @param tTime
 *           CTimeValue: Time value object.
 *    @param s3C
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] ps3B
 *           S3Tuple: Returned B field tuple.
 *    @param[out] pdBmag
 *           double: Returned B field magnitude.
 *    @param[out] pdLdip
 *           double: Returned L value.
 *    @param[out] pdBBeq
 *           double: Returned B/Beq value.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeAllBout( const CTimeValue& tTime,
                                       const S3Coord& s3C,
                                       S3Tuple* ps3B,
                                       double* pdBmag,
                                       double* pdLdip,
                                       double* pdLm,
                                       double* pdBBeq );

/*------------------------------------------------------------------*/
// function CMagfield::computeAllBout
/**
 *
 * Return all normally requested Bfield quantities one call.
 *
 *    @param dTime
 *           double: Time in MJD or composite ("GeoSpace") form.
 *    @param s3C
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] ps3B
 *           S3Tuple: Returned B field tuple.
 *    @param[out] pdBmag
 *           double: Returned B field magnitude.
 *    @param[out] pdLdip
 *           double: Returned L dipole value.
 *    @param[out] pdLm
 *           double: Returned L McIlwain value.
 *    @param[out] pdBBeq
 *           double: Returned B/Beq value.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeAllBout( const double& dTime,
                                       const S3Coord& s3C,
                                       S3Tuple* ps3B,
                                       double* pdBmag,
                                       double* pdLdip,
                                       double* pdLm,
                                       double* pdBBeq );

/*------------------------------------------------------------------*/
// function CMagfield::computeAllBout
/**
 *
 * Return all normally requested Bfield quantities one call.
 *
 *    @param s3C
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] ps3B
 *           S3Tuple: Returned B field tuple.
 *    @param[out] pdBmag
 *           double: Returned B field magnitude.
 *    @param[out] pdLdip
 *           double: Returned L dipole value.
 *    @param[out] pdLm
 *           double: Returned L McIlwain value.
 *    @param[out] pdBBeq
 *           double: Returned B/Beq value.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeAllBout( const S3Coord& s3C,
                                       S3Tuple* ps3B,
                                       double* pdBmag,
                                       double* pdLdip,
                                       double* pdLm,
                                       double* pdBBeq );

/*------------------------------------------------------------------*/
// function CMagfield::computeAllBout
/**
 *
 * Return all normally requested Bfield quantities one call.
 *
 *    @param s3C
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] ps3B
 *           S3Tuple: Returned B field tuple.
 *    @param[out] pdBmag
 *           double: Returned B field magnitude.
 *    @param[out] dLdip
 *           double: Returned L dipole value.
 *    @param[out] dLm
 *           double: Returned L McIlwain value.
 *    @param[out] dBBeq
 *           double: Returned B/Beq value.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeAllBout( const S3Coord& s3C,
                                       S3Tuple& ps3B,
                                       double& dBmag,
                                       double& dLdip,
                                       double& dLm,
                                       double& dBBeq );

/*------------------------------------------------------------------*/
// function CMagfield::computeAllBout
/**
 *
 * Return all normally requested Bfield quantities one call.
 *
 *    @param s3Cv
 *           S3CoordVec: Vector of position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] ps3vB
 *           S3TupleVec: Returned B field vectors.
 *    @param[out] pdvBmag
 *           dvector: Returned vector of B field magnitudes.
 *    @param[out] pdvLdip
 *           dvector: Returned vector of L dipole values.
 *    @param[out] plvLm
 *           dvector: Returned vector of L McIlwain values.
 *    @param[out] pdvBBeq
 *           dvector: Returned vector of B/Beq value.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeAllBout( const S3CoordVec& s3Cv,
                                       S3TupleVec* ps3vB,
                                       dvector* pdvBmag,
                                       dvector* pdvLdip,
                                       dvector* pdvLm,
                                       dvector* pdvBBeq );

/*------------------------------------------------------------------*/
// function CMagfield::computeAllBout
/**
 *
 * Return all normally requested Bfield quantities one call.
 *
 *    @param vtTime
 *           vector<CTimeValue>: Vector of time value objects.
 *    @param s3Cv
 *           S3CoordVec: Vector of position coordinates, geocentric Cartesian in km.
 *
 *    @param[out] ps3vB
 *           S3TupleVec: Returned B field vectors.
 *    @param[out] pdvBmag
 *           dvector: Returned vector of B field magnitudes.
 *    @param[out] pdvLdip
 *           dvector: Returned vector of L dipole values.
 *    @param[out] pdvLm
 *           dvector: Returned vector of L McIlwain values.
 *    @param[out] pdvBBeq
 *           dvector: Returned vector of B/Beq values.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeAllBout( const vector<CTimeValue>& vtTime,
                                       const S3CoordVec& s3Cv,
                                       S3TupleVec* ps3vB,
                                       dvector* pdvBmag,
                                       dvector* pdvLdip,
                                       dvector* pdvLm,
                                       dvector* pdvBBeq );

/*------------------------------------------------------------------*/
// function CMagfield::computeAllBout
/**
 *
 * Return all normally requested Bfield quantities one call.
 *
 *    @param vdTime
 *           dvector: Vector of times, in MJD or composite ("GeoSpace") form.
 *    @param s3Cv
 *           S3CoordVec: Vector of position coordinates.
 *
 *    @param[out] ps3vB
 *           S3TupleVec: Returned B field vectors.
 *    @param[out] pdvBmag
 *           dvector: Returned vector of B field magnitudes.
 *    @param[out] pdvLdip
 *           dvector: Returned vector of L dipole values.
 *    @param[out] pdvLm
 *           dvector: Returned vector of L McIlwain values.
 *    @param[out] pdvBBeq
 *           dvector: Returned vector of B/Beq values.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeAllBout( const dvector& vdTime,
                                       const S3CoordVec& s3Cv,
                                       S3TupleVec* ps3vB,
                                       dvector* pdvBmag,
                                       dvector* pdvLdip,
                                       dvector* pdvLm,
                                       dvector* pdvBBeq );

/*------------------------------------------------------------------*/
// function CMagfield::computeDirectionPitchAngles
/**
 *
 * Local pitch angle determination.
 *
 *    @param s3Pos
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *    @param s3vDir
 *           S3CoordVec: Vector of directions in Geocentric Cartesian, km.
 *
 *    @param[out] dvPitchAngles
 *           dvector: Returned vector of pitch angles.
 *
 *    @param bEastWest
 *           bool: Optional parameter for application of East-West effects
 *            [default=false].
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeDirectionPitchAngles( const S3Coord& s3Pos,
                                                    const S3CoordVec& s3vDir,
                                                    dvector& dvPitchAngles,
                                                    bool bEastWest=false );

/*------------------------------------------------------------------*/
// function CMagfield::computeDirectionPitchAngles
/**
 *
 * Local pitch angle determination.
 *
 *    @param tTime
 *           CTimeValue: Time value object.
 *    @param s3Pos
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *    @param s3vDir
 *           S3CoordVec: Vector of directions in geocentric Cartesian, km or unit.
 *
 *    @param[out] dvPitchAngles
 *           dvector: Returned vector of pitch angles.
 *
 *    @param bEastWest
 *           bool: Optional parameter for application of East-West effects
 *            [default=false].
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeDirectionPitchAngles( const CTimeValue& tTime,
                                                    const S3Coord& s3Pos,
                                                    const S3CoordVec& s3vDir,
                                                    dvector& dvPitchAngles,
                                                    bool bEastWest=false );

/*------------------------------------------------------------------*/
// function CMagfield::computeDirectionPitchAngles
/**
 *
 * Local pitch angle determination.
 *
 *    @param dTime
 *           double: Time in MJD or composite ("GeoSpace") form.
 *    @param s3Pos
 *           S3Coord: Position coordinates, geocentric Cartesian in km.
 *    @param s3vDir
 *           S3CoordVec: Vector of directions in geocentric Cartesian, km or unit.
 *
 *    @param[out] dvPitchAngles
 *           dvector: Returned vector of pitch angles.
 *
 *    @param bEastWest
 *           bool: Optional parameter for application of East-West effects
 *            [default=false].
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeDirectionPitchAngles( const double& dTime,
                                                    const S3Coord& s3Pos,
                                                    const S3CoordVec& s3vDir,
                                                    dvector& dvPitchAngles,
                                                    bool bEastWest=false );

/*------------------------------------------------------------------*/
  // ** Fieldline Tracing **
/*------------------------------------------------------------------*/
// function CMagfield::computeFieldLine
/**
 *
 * Trace the fieldline passing through a single point, given in
 * Geocentric Cartesian km.
 *
 * @param tTime
 *        CTimeValue: Time value object.
 *
 * @param s3C
 *        S3Coord: Position coordinates, geocentric Cartesian in km.
 * @param efltIn
 *        eFieldLineTraceDir: Trace direction enumeration,
 *        * efltdNorth         = North
 *        * efltdSouth         = South
 *        * efltdNorthAndSouth = Both North and South
 *        * efltGsmEquator     = To geomagnetic equator
 * @param elmIn
 *        eLshellMode: L shell mode enumeration,
 *        * elmDipole   = Simple dipole
 *        * elmMcIlwain = McIlwain
 * @param dTraceAlt
 *        double: FieldLines are trace to dTraceAlt.
 *
 * @param[out] ps3CvFLs
 *        S3CoordVec: Returned vector of field line coordinates, geocentric Cartesian in km.
 * @param[out] pdL
 *        double: Returned L shell value corresponding to the LshellMode used.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeFieldLine( const CTimeValue& tTime,
                                         const S3Coord& s3C,
                                         const eFieldLineTraceDir& efltIn,
                                         const eLshellMode& elmIn,
                                         const double& dTraceAlt,
                                         S3CoordVec* ps3CvFLs,
                                         double* pdL );

/*------------------------------------------------------------------*/
// function CMagfield::computeFieldLine
/**
 *
 * Trace the fieldline passing through a single point, given in
 * Geocentric Cartesian km.
 *
 * @param dTime
 *        double: Time in MJD or composite ("GeoSpace") form.
 *
 * @param s3C
 *        S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 * @param efltIn
 *        eFieldLineTraceDir: Trace direction enumeration,
 *        * efltdNorth         = North
 *        * efltdSouth         = South
 *        * efltdNorthAndSouth = Both North and South
 *        * efltGsmEquator     = To geomagnetic equator
 * @param elmIn
 *        eLshellMode: L shell mode enumeration,
 *        * elmDipole   = Simple dipole
 *        * elmMcIlwain = McIlwain
 * @param dTraceAlt
 *        double: FieldLines are trace to dTraceAlt.
 *
 * @param[out] ps3CvFLs
 *        S3CoordVec: Returned vector of field line coordinates, geocentric Cartesian in km.
 * @param[out] pdL
 *        double: Returned L shell value corresponding to the LshellMode used.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeFieldLine( const double& dTime,
                                         const S3Coord& s3C,
                                         const eFieldLineTraceDir& efltIn,
                                         const eLshellMode& elmIn,
                                         const double& dTraceAlt,
                                         S3CoordVec* ps3CvFLs,
                                         double* pdL ); 

/*------------------------------------------------------------------*/
// function CMagfield::computeFieldLine
/**
 *
 * Trace the fieldline passing through a single point, given in
 * Geocentric Cartesian km.
 *
 * @param dTime
 *        double: Time in MJD or composite ("GeoSpace") form.
 *
 * @param s3C
 *        S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 * @param strFltIn
 *        string: trace direction (case insensitive),
 *        * "North"      | "N"  = Trace North
 *        * "South"      | "S"  = Trace South
 *        * "NorthSouth" | "NS" = Trace North and South
 *        * "Equator"    | "E"  = Trace toward geomagnetic equator
 * @param strLmIn
 *        string: L shell mode (case insensitve),
 *        * "Dipole"   | "D" | "Ldip" = Dipole mode
 *        * "McIlwain" | "M" | "Lm"   = McIlwain mode
 * @param dTraceAlt
 *        double: FieldLines are trace to dTraceAlt.
 *
 * @param[out] ps3CvFLs
 *        S3CoordVec: Returned vector of field line coordinates, geocentric Cartesian in km.
 * @param[out] pdL
 *        double: Returned L shell value corresponding to the LshellMode used.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeFieldLine( const double& dTime,
                                         const S3Coord& s3C,
                                         const string& strFltIn, 
                                         const string& strLmIn,
                                         const double& dTraceAlt,
                                         S3CoordVec* ps3CvFLs,
                                         double* pdL );

/*------------------------------------------------------------------*/
// function CMagfield::computeFieldLine
/**
 *
 * Trace the fieldline passing through a single point, given in
 * Geocentric Cartesian km.
 *
 * @param s3C
 *        S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 * @param efltIn
 *        eFieldLineTraceDir: Trace direction enumeration,
 *        * efltdNorth         = North
 *        * efltdSouth         = South
 *        * efltdNorthAndSouth = Both North and South
 *        * efltGsmEquator     = To geomagnetic equator
 * @param elmIn
 *        eLshellMode: L shell mode enumeration,
 *        * elmDipole   = Simple dipole
 *        * elmMcIlwain = McIlwain
 * @param dTraceAlt
 *        double: FieldLines are trace to dTraceAlt.
 *
 * @param[out] ps3CvFLs
 *        S3CoordVec: Returned vector of field line coordinates, geocentric Cartesian in km.
 * @param[out] pdL
 *        double: Returned L shell value corresponding to the LshellMode used.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeFieldLine( const S3Coord& s3C,
                                         const eFieldLineTraceDir& efltIn,
                                         const eLshellMode& elmIn,
                                         const double& dTraceAlt,
                                         S3CoordVec* ps3CvFLs,
                                         double* pdL );

/*------------------------------------------------------------------*/
// function CMagfield::computeFieldLine
/**
 *
 * Trace the fieldline passing through a single point, given in
 * Geocentric Cartesian km.
 *
 * @param s3C
 *        S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 * @param strFltIn:
 *        string: trace direction (case insensitive),
 *        * "North"      | "N"  = Trace North
 *        * "South"      | "S"  = Trace South
 *        * "NorthSouth" | "NS" = Trace North and South
 *        * "Equator"    | "E"  = Trace toward geomagnetic equator
 * @param strLmIn
 *        string: L shell mode (case insensitve),
 *        * "Dipole"   | "D" | "Ldip" = Dipole mode
 *        * "McIlwain" | "M" | "Lm"   = McIlwain mode
 * @param dTraceAlt
 *        double: FieldLines are trace to dTraceAlt.
 *
 * @param[out] ps3CvFLs
 *        S3CoordVec: Returned vector of field line coordinates, geocentric Cartesian in km.
 * @param[out] pdL
 *        double: Returned L shell value corresponding to the LshellMode used.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeFieldLine( const S3Coord& s3C,
                                         const string& strFltIn,
                                         const string& strLmIn,
                                         const double& dTraceAlt,
                                         S3CoordVec* ps3CvFLs,
                                         double* pdL );

/*------------------------------------------------------------------*/
// function CMagfield::computeFieldLine
/**
 *
 * Trace the fieldline passing through a single point, given in
 * Geocentric Cartesian km.
 *
 * @param s3C
 *        S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 * @param efltIn
 *        eFieldLineTraceDir: Trace direction enumeration,
 *        * efltdNorth         = North
 *        * efltdSouth         = South
 *        * efltdNorthAndSouth = Both North and South
 *        * efltGsmEquator     = To geomagnetic equator
 * @param elmIn
 *        eLshellMode: L shell mode enumeration,
 *        * elmDipole   = Simple dipole
 *        * elmMcIlwain = McIlwain
 * @param dTraceAlt
 *        double: FieldLines are trace to dTraceAlt.
 *
 * @param[out] ps3CvFLs
 *        S3CoordVec: Returned vector of field line coordinates, geocentric Cartesian in km.
 * @param[out] dL
 *        double: Returned L shell value corresponding to the LshellMode used.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeFieldLine( const S3Coord& s3C,
                                         eFieldLineTraceDir efltIn,
                                         eLshellMode elmIn,
                                         double dTraceAlt,
                                         S3CoordVec& s3CvFLs,
                                         double& dL );

/*------------------------------------------------------------------*/
// function CMagfield::computeFieldLine
/**
 *
 * Trace the fieldline passing through a single point, given in
 * Geocentric Cartesian km.
 *
 * @param tTime
 *        CTimeValue: Time value object.
 *
 * @param s3C
 *        S3Coord: Position coordinates, geocentric Cartesian in km.
 * @param efltIn
 *        eFieldLineTraceDir: Trace direction enumeration,
 *        * efltdNorth         = North
 *        * efltdSouth         = South
 *        * efltdNorthAndSouth = Both North and South
 *        * efltGsmEquator     = To geomagnetic equator
 * @param elmIn
 *        eLshellMode: L shell mode enumeration,
 *        * elmDipole   = Simple dipole
 *        * elmMcIlwain = McIlwain
 * @param dTraceAlt
 *        double: FieldLines are trace to dTraceAlt.
 *
 * @param[out] ps3CvFLs
 *        S3CoordVec: Returned vector of field line coordinates, geocentric Cartesian in km.
 * @param[out] dL
 *        double: Returned L shell value corresponding to the LshellMode used.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeFieldLine( CTimeValue tTime,
                                         const S3Coord& s3C,
                                         eFieldLineTraceDir efltIn,
                                         eLshellMode elmIn,
                                         double dTraceAlt,
                                         S3CoordVec& s3CvFLs,
                                         double& dL );

/*------------------------------------------------------------------*/
// function CMagfield::computeFieldLine
/**
 *
 * Trace the fieldline passing through a single point, given in
 * Geocentric Cartesian km.
 *
 * @param dTime
 *        double: Time in MJD or composite ("GeoSpace") form.
 *
 * @param s3C
 *        S3Coord: Position coordinates, geocentric Cartesian in km.
 * @param efltIn
 *        eFieldLineTraceDir: Trace direction enumeration,
 *        * efltdNorth         = North
 *        * efltdSouth         = South
 *        * efltdNorthAndSouth = Both North and South
 *        * efltGsmEquator     = To geomagnetic equator
 * @param elmIn
 *        eLshellMode: L shell mode enumeration,
 *        * elmDipole   = Simple dipole
 *        * elmMcIlwain = McIlwain
 * @param dTraceAlt
 *        double: FieldLines are trace to dTraceAlt.
 *
 * @param[out] ps3CvFLs
 *        S3CoordVec: Returned vector of field line coordinates, geocentric Cartesian in km.
 * @param[out] dL
 *        double: Returned L shell value corresponding to the LshellMode used.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeFieldLine( double dTime,
                                         const S3Coord& s3C,
                                         eFieldLineTraceDir efltIn,
                                         eLshellMode elmIn,
                                         double dTraceAlt,
                                         S3CoordVec& s3CvFLs,
                                         double& dL );

/*------------------------------------------------------------------*/
//#ifdef GEOSPACE_RELICS
  // computeMlatFieldLines()/computeMltFieldLines(): these routines
  // compute a set of fieldlines as a function of magnetic
  // latitude(local time), at the given magnetic local time(latitude),
  // over the specified range equally divided by iNumFLs.  Errors for
  // each field line trace are returned in pivErrors.  the number of
  // field lines for each step are returned in pivPtsPerLine.
/*------------------------------------------------------------------*/
// function CMagfield::computeMlatFieldLines
/**
 *
 * Compute a set of fieldlines as a function of magnetic latitude
 * (local time), at the given magnetic local time(latitude), over
 * the specified range, equally divided.
 *
 *    @param tTime
 *           CTimeValue: Time value object.
 *    @param dMLT
 *           double: Magnetic local time.
 *    @param dMLAT0
 *           double: Magnetic latitude start value.
 *    @param dMLAT1
 *           double: Magnetic latitude stop value.
 *    @param iNumFLs
 *           int: Number of field lines.
 *    @param elmIn
 *           eLshellMode: L shell mode enumeration,
 *           * elmDipole   = Dipole mode
 *           * elmMcIlwain = McIlwain mode
 *
 *    @param[out] ps3CvFLs
 *           S3CoordVec: Returned vector of field line coordinates, geocentric Cartesian in km.
 *    @param[out] pdvL
 *           dvector: Returned vector of field line L values.
 *    @param[out] pivPtsPerLine
 *           ivector: Returned vector of number of points per field line.
 *    @param[out] pivErrors
 *           ivector: Returned vector of error codes per field line.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeMlatFieldLines( const CTimeValue& tTime,
                                              const double& dMLT,
                                              const double& dMLAT0,
                                              const double& dMLAT1,
                                              const int& iNumFLs,
                                              const eLshellMode& elmIn,
                                              S3CoordVec* ps3CvFLs,
                                              dvector* pdvL,
                                              ivector* pivPtsPerLine,
                                              ivector* pivErrors );

/*------------------------------------------------------------------*/
// function CMagfield::computeMlatFieldLines
/**
 *
 * Compute a set of fieldlines as a function of magnetic latitude
 * (local time), at the given magnetic local time(latitude), over
 * the specified range, equally divided.
 *
 *    @param dTime
 *           double: Time in MJD or composite ("GeoSpace") form.
 *    @param dMLT
 *           double: Magnetic local time.
 *    @param dMLAT0
 *           double: Magnetic latitude start value.
 *    @param dMLAT1
 *           double: Magnetic latitude stop value.
 *    @param iNumFLs
 *           int: Number of field lines.
 *    @param elmIn
 *           eLshellMode: L shell mode enumeration,
 *           * elmDipole   = Dipole mode
 *           * elmMcIlwain = McIlwain mode
 *
 *    @param[out] ps3CvFLs
 *           S3CoordVec: Returned vector of field line coordinates, geocentric Cartesian in km.
 *    @param[out] pdvL
 *           dvector: Returned vector of field line L values.
 *    @param[out] pivPtsPerLine
 *           ivector: Returned vector of number of points per field line.    
 *    @param[out] pivErrors
 *           ivector: Returned vector of error codes per field line.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeMlatFieldLines( const double& dTime,
                                              const double& dMLT,
                                              const double& dMLAT0,
                                              const double& dMLAT1,
                                              const int& iNumFLs,
                                              const eLshellMode& elmIn,
                                              S3CoordVec* ps3CvFLs,
                                              dvector* pdvL,
                                              ivector* pivPtsPerLine,
                                              ivector* pivErrors );

/*------------------------------------------------------------------*/
// function CMagfield::computeMlatFieldLines
/**
 *
 * Compute a set of fieldlines as a function of magnetic latitude
 * (local time), at the given magnetic local time(latitude), over
 * the specified range, equally divided.
 *
 *    @param dTime
 *           double: Time in MJD or composite ("GeoSpace") form.
 *    @param dMLT
 *           double: Magnetic local time.
 *    @param dMLAT0
 *           double: Magnetic latitude start value.
 *    @param dMLAT1
 *           double: Magnetic latitude stop value.
 *    @param iNumFLs
 *           int: Number of field lines.
 *    @param strLmIn
 *           string: L shell mode (case insensitve),
 *           * "Dipole"   | "D" | "Ldip" = Dipole mode
 *           * "McIlwain" | "M" | "Lm"   = McIlwain mode
 *
 *    @param[out] ps3CvFLs
 *           S3CoordVec: Returned vector of field line coordinates, geocentric Cartesian in km.
 *    @param[out] pdvL
 *           dvector: Returned vector of field line L values.
 *    @param[out] pivPtsPerLine
 *           ivector: Returned vector of number of points per field line.
 *    @param[out] pivErrors
 *           ivector: Returned vector of error codes per field line.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeMlatFieldLines( const double& dTime,
                                              const double& dMLT,
                                              const double& dMLAT0,
                                              const double& dMLAT1,
                                              const int& iNumFLs,
                                              const string& strLmIn, 
                                              S3CoordVec* ps3CvFLs,
                                              dvector* pdvL,
                                              ivector* pivPtsPerLine,
                                              ivector* pivErrors );

/*------------------------------------------------------------------*/
// function CMagfield::computeMltFieldLines
/**
 *
 * Compute a set of fieldlines as a function of magnetic local time,
 * at the given magnetic local latitude, over the given equally divided
 * range.
 *
 *    @param tTime
 *           CTimeValue: Time value object.
 *    @param dMLAT
 *           double: Magnetic latitude.
 *    @param dMLAT0
 *           double: Magnetic local time start value.
 *    @param dMLAT1
 *           double: Magnetic local time stop value.
 *    @param iNumFLs
 *           int: Number of field lines.
 *    @param elmIn
 *           eLshellMode: L shell mode enumeration,
 *           * elmDipole   = Dipole mode
 *           * elmMcIlwain = McIlwain mode
 *
 *    @param[out] ps3CvFLs
 *           S3CoordVec: Returned vector of field line coordinates, geocentric Cartesian in km.
 *    @param[out] pdvL
 *           dvector: Returned vector of field line L values.
 *    @param[out] pivPtsPerLine
 *           ivector: Returned vector of number of points per field line.
 *    @param[out] pivErrors
 *           ivector: Returned vector of error codes per field line.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeMltFieldLines( const CTimeValue& tTime,
                                             const double& dMLAT,
                                             const double& dMLT0,
                                             const double& dMLT1,
                                             const int& iNumFLs,
                                             const eLshellMode& elmIn,
                                             S3CoordVec* ps3CvFLs,
                                             dvector* pdvL,
                                             ivector* pivPtsPerLine,
                                             ivector* pivErrors );

/*------------------------------------------------------------------*/
// function CMagfield::computeMltFieldLines
/**
 *
 * Compute a set of fieldlines as a function of magnetic local time,
 * at the given magnetic local latitude, over the given equally divided
 * range.
 *
 *    @param dTime
 *           double: Time in MJD or composite ("GeoSpace") form.
 *    @param dMLAT
 *           double: Magnetic latitude.
 *    @param dMLAT0
 *           double: Magnetic local time start value.
 *    @param dMLAT1
 *           double: Magnetic local time stop value.
 *    @param iNumFLs
 *           int: Number of field lines.
 *    @param elmIn
 *           eLshellMode: L shell mode enumeration,
 *           * elmDipole   = Dipole mode
 *           * elmMcIlwain = McIlwain mode
 *
 *    @param[out] ps3CvFLs
 *           S3CoordVec: Returned vector of field line coordinates, geocentric Cartesian in km.
 *    @param[out] pdvL
 *           dvector: Returned vector of field line L values.
 *    @param[out] pivPtsPerLine
 *           ivector: Returned vector of number of points per field line.
 *    @param[out] pivErrors
 *           ivector: Returned vector of error codes per field line.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeMltFieldLines( const double& dTime,
                                             const double& dMLAT,
                                             const double& dMLT0,
                                             const double& dMLT1,
                                             const int& iNumFLs,
                                             const eLshellMode& elmIn,
                                             S3CoordVec* ps3CvFLs,
                                             dvector* pdvL,
                                             ivector* pivPtsPerLine,
                                             ivector* pivErrors );

/*------------------------------------------------------------------*/
// function CMagfield::computeMltFieldLines
/**
 *
 * Compute a set of fieldlines as a function of magnetic local time,
 * at the given magnetic local latitude, over the given equally divided
 * range.
 *
 *    @param dTime
 *           double: Time in MJD or composite ("GeoSpace") form.
 *    @param dMLAT
 *           double: Magnetic latitude.
 *    @param dMLAT0
 *           double: Magnetic local time start value.
 *    @param dMLAT1
 *           double: Magnetic local time stop value.
 *    @param iNumFLs
 *           int: Number of field lines.
 *    @param strLmIn
 *           string: L shell mode (case insensitve),
 *           * "Dipole"   | "D" | "Ldip" = Dipole mode
 *           * "McIlwain" | "M" | "Lm"   = McIlwain mode
 *
 *    @param[out] ps3CvFLs
 *           S3CoordVec: Returned vector of field line coordinates, geocentric Cartesian in km.
 *    @param[out] pdvL
 *           dvector: Returned vector of field line L values.
 *    @param[out] pivPtsPerLine
 *           ivector: Returned vector of number of points per field line.
 *    @param[out] pivErrors
 *           ivector: Returned vector of error codes per field line.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeMltFieldLines( const double& dTime,
                                             const double& dMLAT,
                                             const double& dMLT0,
                                             const double& dMLT1,
                                             const int& iNumFLs,
                                             const string& strLmIn,
                                             S3CoordVec* ps3CvFLs,
                                             dvector* pdvL,
                                             ivector* pivPtsPerLine,
                                             ivector* pivErrors );

/*------------------------------------------------------------------*/
  // no-com
/*------------------------------------------------------------------*/
// function CMagfield::computeMlatFieldLines
/**
 *
 * Compute a set of fieldlines as a function of magnetic latitude
 * (local time), at the given magnetic local time(latitude), over
 * the specified range, equally divided.
 *
 *    @param dMLT
 *           double: Magnetic local time.
 *    @param dMLAT0
 *           double: Magnetic latitude start value.
 *    @param dMLAT1
 *           double: Magnetic latitude stop value.
 *    @param iNumFLs
 *           int: Number of field lines.
 *    @param elmIn
 *           eLshellMode: L shell mode enumeration,
 *           * elmDipole   = Dipole mode
 *           * elmMcIlwain = McIlwain mode
 *
 *    @param[out] ps3CvFLs
 *           S3CoordVec: Returned vector of field line coordinates, geocentric Cartesian in km.
 *    @param[out] pdvL
 *           dvector: Returned vector of field line L values.
 *    @param[out] pivPtsPerLine
 *           ivector: Returned vector of number of points per field line.
 *    @param[out] pivErrors
 *           ivector: Returned vector of error codes per field line.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeMlatFieldLines( const double& dMLT,
                                              const double& dMLAT0,
                                              const double& dMLAT1,
                                              const int& iNumFLs,
                                              const eLshellMode& elmIn,
                                              S3CoordVec* ps3CvFLs,
                                              dvector* pdvL,
                                              ivector* pivPtsPerLine,
                                              ivector* pivErrors );

/*------------------------------------------------------------------*/
// function CMagfield::computeMlatFieldLines
/**
 *
 * Compute a set of fieldlines as a function of magnetic latitude
 * (local time), at the given magnetic local time(latitude), over
 * the specified range, equally divided.
 *
 *    @param dMLT
 *           double: Magnetic local time.
 *    @param dMLAT0
 *           double: Magnetic latitude start value.
 *    @param dMLAT1
 *           double: Magnetic latitude stop value.
 *    @param iNumFLs
 *           int: Number of field lines.
 *    @param strLmIn
 *           string: L shell mode (case insensitve),
 *           * "Dipole"   | "D" | "Ldip" = Dipole mode
 *           * "McIlwain" | "M" | "Lm"   = McIlwain mode
 *
 *    @param[out] ps3CvFLs
 *           S3CoordVec: Returned vector of field line coordinates, geocentric Cartesian in km.
 *    @param[out] pdvL
 *           dvector: Returned vector of field line L values.
 *    @param[out] pivPtsPerLine
 *           ivector: Returned vector of number of points per field line.
 *    @param[out] pivErrors
 *           ivector: Returned vector of error codes per field line.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeMlatFieldLines( const double& dMLT,
                                              const double& dMLAT0,
                                              const double& dMLAT1,
                                              const int& iNumFLs,
                                              const string& strLmIn, 
                                              S3CoordVec* ps3CvFLs,
                                              dvector* pdvL,
                                              ivector* pivPtsPerLine,
                                              ivector* pivErrors );

/*------------------------------------------------------------------*/
// function CMagfield::computeMltFieldLines
/**
 *
 * Compute a set of fieldlines as a function of magnetic local time,
 * at the given magnetic local latitude, over the given equally divided
 * range.
 *
 *    @param dMLAT
 *           double: Magnetic latitude.
 *    @param dMLAT0
 *           double: Magnetic local time start value.
 *    @param dMLAT1
 *           double: Magnetic local time stop value.
 *    @param iNumFLs
 *           int: Number of field lines.
 *    @param elmIn
 *           eLshellMode: L shell mode enumeration,
 *           * elmDipole   = Dipole mode
 *           * elmMcIlwain = McIlwain mode
 *
 *    @param[out] ps3CvFLs
 *           S3CoordVec: Returned vector of field line coordinates, geocentric Cartesian in km.
 *    @param[out] pdvL
 *           dvector: Returned vector of field line L values.
 *    @param[out] pivPtsPerLine
 *           ivector: Returned vector of number of points per field line.
 *    @param[out] pivErrors
 *           ivector: Returned vector of error codes per field line.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeMltFieldLines( const double& dMLAT,
                                             const double& dMLT0,
                                             const double& dMLT1,
                                             const int& iNumFLs,
                                             const eLshellMode& elmIn,
                                             S3CoordVec* ps3CvFLs,
                                             dvector* pdvL,
                                             ivector* pivPtsPerLine,
                                             ivector* pivErrors );

/*------------------------------------------------------------------*/
// function CMagfield::computeMltFieldLines
/**
 *
 * Compute a set of fieldlines as a function of magnetic local time,
 * at the given magnetic local latitude, over the given equally divided
 * range.
 *
 *    @param dMLAT
 *           double: Magnetic latitude.
 *    @param dMLAT0
 *           double: Magnetic local time start value.
 *    @param dMLAT1
 *           double: Magnetic local time stop value.
 *    @param iNumFLs
 *           int: Number of field lines.
 *    @param strLmIn
 *           string: L shell mode (case insensitve),
 *           * "Dipole"   | "D" | "Ldip" = Dipole mode
 *           * "McIlwain" | "M" | "Lm"   = McIlwain mode
 *
 *    @param[out] ps3CvFLs
 *           S3CoordVec: Returned vector of field line coordinates, geocentric Cartesian in km.
 *    @param[out] pdvL
 *           dvector: Returned vector of field line L values.
 *    @param[out] pivPtsPerLine
 *           ivector: Returned vector of number of points per field line.
 *    @param[out] pivErrors
 *           ivector: Returned vector of error codes per field line.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeMltFieldLines( const double& dMLAT,
                                             const double& dMLT0,
                                             const double& dMLT1,
                                             const int& iNumFLs,
                                             const string& strLmIn, 
                                             S3CoordVec* ps3CvFLs,
                                             dvector* pdvL,
                                             ivector* pivPtsPerLine,
                                             ivector* pivErrors ); 

/*------------------------------------------------------------------*/
// function CMagfield::computeFluxTubeFieldLinesGeo
/**
 *
 * Compute flux tube field lines in Geocentric Cartesian coordinates.
 *
 * @param tTime
 *        CTimeValue: Time value object.
 *
 * @param dPosGeoX
 *        double: Flux tube X location in Geocentric Cartesian coords.
 *
 * @param dPosGeoY
 *        double: Flux tube Y location in Geocentric Cartesian coords.
 *
 * @param dPosGeoZ
 *        double: Flux tube Z location in Geocentric Cartesian coords.
 *
 * @param dFluxTubeDiamKm
 *        double: Flux tube diameter in kilometers. A diameter of 0
 *         indicates a single field line trace through the given point.
 *
 * @param iNumFLs
 *        int: Number of field lines to trace. A number of 1 indicates
 *         a single field line trace through the given point.
 *
 * @param elmIn
 *        eLshellMode: L shell mode enumeration,
 *          elmDipole   = Simple dipole
 *          elmMcIlwain = McIlwain
 *
 * @param[out] ps3CvFLs
 *        S3CoordVec: Returned vector of field line coordinates, geocentric Cartesian in km.
 *
 * @param[out] pdvL
 *        dvector: Returned vector of field line L shell values.
 *
 * @param[out] pivPtsPerLine
 *        ivector: Returned vector of number of points per field line.
 *
 * @param[out] pivErrors
 *        ivector: Returned vector of error codes for the field line computations.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeFluxTubeFieldLinesGeo( const CTimeValue& tTime,
                                                     const double& dPosGeoX,
                                                     const double& dPosGeoY,
                                                     const double& dPosGeoZ,
                                                     const double& dFluxTubeDiamKm,
                                                     const int& iNumFLs,
                                                     const eLshellMode& elmIn,
                                                     S3CoordVec* ps3CvFLs,
                                                     dvector* pdvL,
                                                     ivector* pivPtsPerLine,
                                                     ivector* pivErrors );

/*------------------------------------------------------------------*/
// function CMagfield::computeFluxTubeFieldLinesGeo
/**
 *
 * Compute flux tube field lines in Geocentric Cartesian coordinates.
 *
 * @param dTime
 *        double: Time in MJD or composite ("GeoSpace") form.
 *
 * @param dPosGeoX
 *        double: Flux tube X location in Geocentric Cartesian coords.
 *
 * @param dPosGeoY
 *        double: Flux tube Y location in Geocentric Cartesian coords.
 *
 * @param dPosGeoZ
 *        double: Flux tube Z location in Geocentric Cartesian coords.
 *
 * @param dFluxTubeDiamKm
 *        double: Flux tube diameter in kilometers. A diameter of 0
 *         indicates a single field line trace through the given point.
 *
 * @param iNumFLs
 *        int: Number of field lines to trace. A number of 1 indicates
 *         a single field line trace through the given point.
 *
 * @param elmIn
 *        eLshellMode: L shell mode enumeration,
 *        * elmDipole   = Simple dipole
 *        * elmMcIlwain = McIlwain
 *
 * @param[out] ps3CvFLs
 *        S3CoordVec: Returned vector of field line coordinates, geocentric Cartesian in km.
 *
 * @param[out] pdvL
 *        dvector: Returned vector of field line L shell values.
 *
 * @param[out] pivPtsPerLine
 *        ivector: Returned vector of number of points per field line.
 *
 * @param[out] pivErrors
 *        ivector: Returned vector of error codes for the field line computations.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeFluxTubeFieldLinesGeo( const double& dTime,
                                                     const double& dPosGeoX,
                                                     const double& dPosGeoY,
                                                     const double& dPosGeoZ,
                                                     const double& dFluxTubeDiamKm,
                                                     const int& iNumFLs,
                                                     const eLshellMode& elmIn,
                                                     S3CoordVec* ps3CvFLs,
                                                     dvector* pdvL,
                                                     ivector* pivPtsPerLine,
                                                     ivector* pivErrors );

/*------------------------------------------------------------------*/
// function CMagfield::computeFluxTubeFieldLinesGeo
/**
 *
 * Compute flux tube field lines in Geocentric Cartesian coordinates.
 *
 * @param dTime
 *        double: Time in MJD or composite ("GeoSpace") form.
 * @param dPosGeoX
 *        double: Flux tube X location in Geocentric Cartesian coords.
 * @param dPosGeoY
 *        double: Flux tube Y location in Geocentric Cartesian coords.
 * @param dPosGeoZ
 *        double: Flux tube Z location in Geocentric Cartesian coords.
 * @param dFluxTubeDiamKm
 *        double: Flux tube diameter in kilometers. A diameter of 0
 *         indicates a single field line trace through the given point.
 * @param iNumFLs
 *        int: Number of field lines to trace. A number of 1 indicates
 *         a single field line trace through the given point.
 * @param strFltIn
 *        string: L shell mode (case insensitve),
 *        * "Dipole"   | "D" | "Ldip" = Dipole mode
 *        * "McIlwain" | "M" | "Lm"   = McIlwain mode
 *
 * @param[out] ps3CvFLs
 *        S3CoordVec: Returned vector of field line coordinates, geocentric Cartesian in km.
 * @param[out] pdvL
 *        dvector: Returned vector of field line L shell values.
 * @param[out] pivPtsPerLine
 *        ivector: Returned vector of number of points per field line.
 * @param[out] pivErrors
 *        ivector: Returned vector of error codes for the field line computations.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeFluxTubeFieldLinesGeo( const double& dTime,
                                                     const double& dPosGeoX,
                                                     const double& dPosGeoY,
                                                     const double& dPosGeoZ,
                                                     const double& dFluxTubeDiamKm,
                                                     const int& iNumFLs,
                                                     const string& strFltIn, 
                                                     S3CoordVec* ps3CvFLs,
                                                     dvector* pdvL,
                                                     ivector* pivPtsPerLine,
                                                     ivector* pivErrors );

/*------------------------------------------------------------------*/
// function CMagfield::computeFluxTubeFieldLinesMag
/**
 *
 * Compute flux tube field lines in magnetic coordinates.
 *
 * @param tTime
 *        CTimeValue: Time value object.
 * @param dMLat
 *        double: Magnetic Latitude.
 * @param dMLon
 *        double: Magnetic Longitude.
 * @param dRadRE
 *        double: Radial position in Earth Radii.
 * @param dFluxTubeDiamKm
 *        double: Flux tube diameter in kilometers. A diameter of 0
 *         indicates a single field line trace through the given point.
 * @param iNumFLs
 *        int: Number of field lines to trace. A number of 1 indicates
 *         a single field line trace through the given point.
 * @param elmIn
 *        eLshellMode: L shell mode enumeration,
 *        * elmDipole   = Simple dipole
 *        * elmMcIlwain = McIlwain
 *
 * @param[out] ps3CvFLs
 *        S3CoordVec: Returned vector of field line coordinates, geocentric Cartesian in km.
 * @param[out] pdvL
 *        dvector: Returned vector of field line L shell values.
 * @param[out] pivPtsPerLine
 *        ivector: Returned vector of number of points per field line.
 * @param[out] pivErrors
 *        ivector: Returned vector of error codes for the field line computations.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeFluxTubeFieldLinesMag( const CTimeValue& tTime,
                                                     const double& dMLat,
                                                     const double& dMLon,
                                                     const double& dRadRE,
                                                     const double& dFluxTubeDiamDeg,
                                                     const int& iNumFLs,
                                                     const eLshellMode& elmIn,
                                                     S3CoordVec* ps3CvFLs,
                                                     dvector* pdvL,
                                                     ivector* pivPtsPerLine,
                                                     ivector* pivErrors );

/*------------------------------------------------------------------*/
// function CMagfield::computeFluxTubeFieldLinesMag
/**
 *
 * Compute flux tube field lines in magnetic coordinates.
 *
 * @param dTime
 *        double: Time in MJD or composite ("GeoSpace") form.
 * @param dMLat
 *        double: Magnetic Latitude.
 * @param dMLon
 *        double: Magnetic Longitude.
 * @param dRadRE
 *        double: Radial position in Earth Radii.
 * @param dFluxTubeDiamKm
 *        double: Flux tube diameter in kilometers. A diameter of 0
 *         indicates a single field line trace through the given point.
 * @param iNumFLs
 *        int: Number of field lines to trace. A number of 1 indicates
 *         a single field line trace through the given point.
 * @param elmIn
 *        eLshellMode: L shell mode enumeration,
 *        * elmDipole   = Simple dipole
 *        * elmMcIlwain = McIlwain
 *
 * @param[out] ps3CvFLs
 *        S3CoordVec: Returned vector of field line coordinates, geocentric Cartesian in km.
 * @param[out] pdvL
 *        dvector: Returned vector of field line L shell values.
 * @param[out] pivPtsPerLine
 *        ivector: Returned vector of number of points per field line.
 * @param[out] pivErrors
 *        ivector: Returned vector of error codes for the field line computations.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeFluxTubeFieldLinesMag( const double& dTime,
                                                     const double& dMLat,
                                                     const double& dMLon,
                                                     const double& dRadRE,
                                                     const double& dFluxTubeDiamDeg,
                                                     const int& iNumFLs,
                                                     const eLshellMode& elmIn,
                                                     S3CoordVec* ps3CvFLs,
                                                     dvector* pdvL,
                                                     ivector* pivPtsPerLine,
                                                     ivector* pivErrors );

/*------------------------------------------------------------------*/
// function CMagfield::computeFluxTubeFieldLinesMag
/**
 *
 * Compute flux tube field lines in magnetic coordinates.
 *
 * @param dTime
 *        double: Time in MJD or composite ("GeoSpace") form.
 * @param dMLat
 *        double: Magnetic Latitude.
 * @param dMLon
 *        double: Magnetic Longitude.
 * @param dRadRE
 *        double: Radial position in Earth Radii.
 * @param dFluxTubeDiamKm
 *        double: Flux tube diameter in kilometers. A diameter of 0
 *         indicates a single field line trace through the given point.
 * @param iNumFLs
 *        int: Number of field lines to trace. A number of 1 indicates
 *         a single field line trace through the given point.
 * @param strFltIn
 *        string: L shell mode (case insensitve),
 *        * "Dipole"   | "D" | "Ldip" = Dipole mode
 *        * "McIlwain" | "M" | "Lm"   = McIlwain mode
 *
 * @param[out] ps3CvFLs
 *        S3CoordVec: Returned vector of field line coordinates, geocentric Cartesian in km.
 * @param[out] pdvL
 *        dvector: Returned vector of field line L shell values.
 * @param[out] pivPtsPerLine
 *        ivector: Returned vector of number of points per field line.
 * @param[out] pivErrors
 *        ivector: Returned vector of error codes for the field line computations.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeFluxTubeFieldLinesMag( const double& dTime,
                                                     const double& dMLat,
                                                     const double& dMLon,
                                                     const double& dRadRE,
                                                     const double& dFluxTubeDiamDeg,
                                                     const int& iNumFLs,
                                                     const string& strFltIn, 
                                                     S3CoordVec* ps3CvFLs,
                                                     dvector* pdvL,
                                                     ivector* pivPtsPerLine,
                                                     ivector* pivErrors ); 

/*------------------------------------------------------------------*/
  // no-com
/*------------------------------------------------------------------*/
// function CMagfield::computeFluxTubeFieldLinesGeo
/**
 *
 * @param dPosGeoX
 *        double: Flux tube X location in Geocentric Cartesian coords.
 * @param dPosGeoY
 *        double: Flux tube Y location in Geocentric Cartesian coords.
 * @param dPosGeoZ
 *        double: Flux tube Z location in Geocentric Cartesian coords.
 * @param dFluxTubeDiamKm
 *        double: Flux tube diameter in kilometers. A diameter of 0
 *         indicates a single field line trace through the given point.
 * @param iNumFLs
 *        int: Number of field lines to trace. A number of 1 indicates
 *         a single field line trace through the given point.
 * @param elmIn
 *        eLshellMode: L shell mode enumeration,
 *        * elmDipole   = Simple dipole
 *        * elmMcIlwain = McIlwain
 *
 * @param[out] ps3CvFLs
 *        S3CoordVec: Returned vector of field line coordinates, geocentric Cartesian in km.
 * @param[out] pdvL
 *        dvector: Returned vector of field line L shell values.
 * @param[out] pivPtsPerLine
 *        ivector: Returned vector of number of points per field line.
 * @param[out] pivErrors
 *        ivector: Returned vector of error codes for the field line computations.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeFluxTubeFieldLinesGeo( const double& dPosGeoX,
                                                     const double& dPosGeoY,
                                                     const double& dPosGeoZ,
                                                     const double& dFluxTubeDiamKm,
                                                     const int& iNumFLs,
                                                     const eLshellMode& elmIn,
                                                     S3CoordVec* ps3CvFLs,
                                                     dvector* pdvL,
                                                     ivector* pivPtsPerLine,
                                                     ivector* pivErrors );

/*------------------------------------------------------------------*/
// function CMagfield::computeFluxTubeFieldLinesGeo
/**
 *
 * Compute flux tube field lines in Geocentric Cartesian coordinates.
 *
 * @param dMLat
 *        double: Magnetic Latitude.
 * @param dMLon
 *        double: Magnetic Longitude.
 * @param dAltKm
 *        double: Altitude in km.
 * @param dFluxTubeDiamKm
 *        double: Flux tube diameter in kilometers. A diameter of 0
 *         indicates a single field line trace through the given point.
 * @param iNumFLs
 *        int: Number of field lines to trace. A number of 1 indicates
 *         a single field line trace through the given point.
 * @param strFltIn
 *        string: L shell mode (case insensitve),
 *        * "Dipole"   | "D" | "Ldip" = Dipole mode
 *        * "McIlwain" | "M" | "Lm"   = McIlwain mode
 *
 * @param[out] ps3CvFLs
 *        S3CoordVec: Returned vector of field line coordinates, geocentric Cartesian in km.
 * @param[out] pdvL
 *        dvector: Returned vector of field line L shell values.
 * @param[out] pivPtsPerLine
 *        ivector: Returned vector of number of points per field line.
 * @param[out] pivErrors
 *        ivector: Returned vector of error codes for the field line computations.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeFluxTubeFieldLinesGeo( const double& dLat,
                                                     const double& dLon,
                                                     const double& dAltKm,
                                                     const double& dFluxTubeDiamKm,
                                                     const int& iNumFLs,
                                                     const string& strFltIn, 
                                                     S3CoordVec* ps3CvFLs,
                                                     dvector* pdvL,
                                                     ivector* pivPtsPerLine,
                                                     ivector* pivErrors ); 

/*------------------------------------------------------------------*/
// function CMagfield::computeFluxTubeFieldLinesMag
/**
 *
 * Compute flux tube field lines in magnetic coordinates.
 *
 * @param dMLat
 *        double: Magnetic Latitude.
 * @param dMLon
 *        double: Magnetic Longitude.
 * @param dAltKm
 *        double: Altitude in km.
 * @param dFluxTubeDiamKm
 *        double: Flux tube diameter in kilometers. A diameter of 0
 *         indicates a single field line trace through the given point.
 * @param iNumFLs
 *        int: Number of field lines to trace. A number of 1 indicates
 *         a single field line trace through the given point.
 * @param elmIn
 *        eLshellMode: L shell mode enumeration,
 *        * elmDipole    Dipole mode
 *        * elmMcIlwain  McIlwain mode
 *
 * @param[out] ps3CvFLs
 *        S3CoordVec: Returned vector of field line coordinates, geocentric Cartesian in km.
 * @param[out] pdvL
 *        dvector: Returned vector of field line L shell values.
 * @param[out] pivPtsPerLine
 *        ivector: Returned vector of number of points per field line.
 * @param[out] pivErrors
 *        ivector: Returned vector of error codes for the field line computations.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeFluxTubeFieldLinesMag( const double& dMLat,
                                                     const double& dMLon,
                                                     const double& dRadRE,
                                                     const double& dFluxTubeDiamDeg,
                                                     const int& iNumFLs,
                                                     const eLshellMode& elmIn,
                                                     S3CoordVec* ps3CvFLs,
                                                     dvector* pdvL,
                                                     ivector* pivPtsPerLine,
                                                     ivector* pivErrors );

/*------------------------------------------------------------------*/
// function CMagfield::computeFluxTubeFieldLinesMag
/**
 *
 * Compute flux tube field lines in magnetic coordinates.
 *
 * @param dMLat
 *        double: Magnetic Latitude.
 * @param dMLon
 *        double: Magnetic Longitude.
 * @param dAltKm
 *        double: Altitude in km.
 * @param dFluxTubeDiamKm
 *        double: Flux tube diameter in kilometers. A diameter of 0
 *         indicates a single field line trace through the given point.
 * @param iNumFLs
 *        int: Number of field lines to trace. A number of 1 indicates
 *         a single field line trace through the given point.
 * @param strFltIn
 *        string: L shell mode (case insensitve),
 *        * "Dipole"   | "D" | "Ldip" = Dipole mode
 *        * "McIlwain" | "M" | "Lm"   = McIlwain mode
 *
 * @param[out] ps3CvFLs
 *        S3CoordVec: Returned vector of field line coordinates, geocentric Cartesian in km.
 * @param[out] pdvL
 *        dvector: Returned vector of field line L shell values.
 * @param[out] pivPtsPerLine
 *        ivector: Returned vector of number of points per field line.
 * @param[out] pivErrors
 *        ivector: Returned vector of error codes for the field line computations.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeFluxTubeFieldLinesMag( const double& dMLat,
                                                     const double& dMLon,
                                                     const double& dRadRE,
                                                     const double& dFluxTubeDiamDeg,
                                                     const int& iNumFLs,
                                                     const string& strLmIn,
                                                     S3CoordVec* ps3CvFLs,
                                                     dvector* pdvL,
                                                     ivector* pivPtsPerLine,
                                                     ivector* pivErrors );

/*------------------------------------------------------------------*/
//#endif // GEOSPACE_RELICS
  // ** FieldLine Footprints **
/*------------------------------------------------------------------*/
// function CMagfield::computeFootPrint
/**
 *
 * Traces fieldline to its footprint location. Footprints with
 * direction of North and South have two points returned and one
 * L shell value.
 *
 * @param tTime
 *        CTimeValue: Time value object.
 * @param s3C
 *        S3Coord: Start position in Geocentric Cartesian km coordinates.
 * @param dAltSurf
 *        double: Footprint altitude in km; an altitude of zero indicates ground footprints.
 * @param efltIn
 *        eFieldLineTraceDir: Trace direction enumeration,
 *         efltdNorth         = North
 *         efltdSouth         = South
 *         efltdNorthAndSouth = Both North and South
 * @param elmIn
 *        eLshellMode: L shell mode enumeration,
 *        * elmDipole   = Simple dipole
 *        * elmMcIlwain = McIlwain
 *        
 * @param[out] ps3cvFPs
 *        S3CoordVec: Returned vector of footprint coordinates, geocentric Cartesian in km.
 * @param[out] pdL
 *        double: Returned L shell value.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeFootPrint( const CTimeValue& tTime,
                                         const S3Coord& s3C,
                                         const double& dAltSurf,
                                         const eFieldLineTraceDir& efltIn,
                                         const eLshellMode& elmIn,
                                         S3CoordVec* ps3cvFPs,
                                         double* pdL );

/*------------------------------------------------------------------*/
// function CMagfield::computeFootPrint
/**
 *
 * Traces fieldline to its footprint location. Footprints with
 * direction of North and South have two points returned and one
 * L shell value.
 *
 * @param dTime
 *        double: Time in MJD or composite ("GeoSpace") form.
 * @param s3C
 *        S3Coord: Start position in Geocentric Cartesian km coordinates.
 * @param dAltSurf
 *        double: Footprint altitude in km; an altitude of zero indicates ground footprints.
 * @param efltIn
 *        eFieldLineTraceDir: Trace direction enumeration,
 *        * efltdNorth         = North
 *        * efltdSouth         = South
 *        * efltdNorthAndSouth = Both North and South
 * @param elmIn
 *        eLshellMode: L shell mode enumeration,
 *        * elmDipole   = Simple dipole
 *        * elmMcIlwain = McIlwain
 *        
 * @param[out] ps3cvFPs
 *        S3CoordVec: Returned vector of footprint coordinates, geocentric Cartesian in km.
 * @param[out] pdL
 *        double: Returned L shell value.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeFootPrint( const double& dTime,
                                         const S3Coord& s3C,
                                         const double& dAltSurf,
                                         const eFieldLineTraceDir& efltIn,
                                         const eLshellMode& elmIn,
                                         S3CoordVec* ps3cvFPs,
                                         double* pdL );

/*------------------------------------------------------------------*/
// function CMagfield::computeFootPrint
/**
 *
 * Traces fieldline to its footprint location. Footprints with
 * direction of North and South have two points returned and one
 * L shell value.
 *
 * @param dTime
 *        double: Time in MJD or composite ("GeoSpace") form.
 * @param s3C
 *        S3Coord: Start position in Geocentric Cartesian km coordinates.
 * @param dAltSurf
 *        double: Footprint altitude in km; an altitude of zero indicates ground footprints.
 * @param strFltIn
 *        string: trace direction (case insensitive),
 *        * "North"      | "N"  = Trace North
 *        * "South"      | "S"  = Trace South
 *        * "NorthSouth" | "NS" = Trace North and South
 * @param strLmIn
 *        string: L shell mode (case insensitve),
 *        * "Dipole"   | "D" | "Ldip" = Dipole mode
 *        * "McIlwain" | "M" | "Lm"   = McIlwain mode
 *        
 * @param[out] ps3cvFPs
 *        S3CoordVec: Returned vector of footprint coordinates, geocentric Cartesian in km.
 * @param[out] pdL
 *        double: Returned L shell value.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeFootPrint( const double& dTime,
                                         const S3Coord& s3C,
                                         const double& dAltSurf,
                                         const string& strFltIn,
                                         const string& strLmIn,
                                         S3CoordVec* ps3cvFPs,
                                         double* pdL );

/*------------------------------------------------------------------*/
  // no-com
/*------------------------------------------------------------------*/
// function CMagfield::computeFootPrint
/**
 *
 * Traces fieldline to its footprint location. Footprints with
 * direction of North and South have two points returned and one
 * L shell value.
 *
 * @param s3C
 *        S3Coord: Start position in Geocentric Cartesian km coordinates.
 * @param dAltSurf
 *        double: Footprint altitude in km; an altitude of zero indicates ground footprints.
 * @param efltIn
 *        eFieldLineTraceDir: Trace direction enumeration,
 *        * efltdNorth         = North
 *        * efltdSouth         = South
 *        * efltdNorthAndSouth = Both North and South
 * @param elmIn
 *        eLshellMode: L shell mode enumeration,
 *        * elmDipole   = Simple dipole
 *        * elmMcIlwain = McIlwain
 *        
 * @param[out] ps3cvFPs
 *        S3CoordVec: Returned vector of footprint coordinates, geocentric Cartesian in km.
 * @param[out] pdL
 *        double: Returned L shell value.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeFootPrint( const S3Coord& s3C,
                                         const double& dAltSurf,
                                         const eFieldLineTraceDir& efltIn,
                                         const eLshellMode& elmIn,
                                         S3CoordVec* ps3cvFPs,
                                         double* pdL );

/*------------------------------------------------------------------*/
// function CMagfield::computeFootPrint
/**
 *
 * Traces fieldline to its footprint location. Footprints with
 * direction of North and South have two points returned and one
 * L shell value.
 *
 * @param s3C
 *        S3Coord: Start position in Geocentric Cartesian km coordinates.
 * @param dAltSurf
 *        double: Footprint altitude in km; an altitude of zero indicates ground footprints.
 * @param strFltIn
 *        string: trace direction (case insensitive),
 *        * "North"      | "N"  = Trace North
 *        * "South"      | "S"  = Trace South
 *        * "NorthSouth" | "NS" = Trace North and South
 * @param strLmIn
 *        string: L shell mode (case insensitve),
 *        * "Dipole"   | "D" | "Ldip" = Dipole mode
 *        * "McIlwain" | "M" | "Lm"   = McIlwain mode
 *        
 * @param[out] ps3cvFPs
 *        S3CoordVec: Returned vector of footprint coordinates, geocentric Cartesian in km.
 * @param[out] pdL
 *        double: Returned L shell value.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeFootPrint( const S3Coord& s3C,
                                         const double& dAltSurf,
                                         const string& strFltIn,
                                         const string& strLmIn,
                                         S3CoordVec* ps3cvFPs,
                                         double* pdL );

/*------------------------------------------------------------------*/
// function CMagfield::computeFootPrint
/**
 *
 * Traces fieldline to its footprint location. Footprints with
 * direction of North and South have two points returned and one
 * L shell value.
 *
 * @param s3C
 *        S3Coord: Start position in Geocentric Cartesian km coordinates.
 * @param dAltSurf
 *        double: Footprint altitude in km; an altitude of zero indicates ground footprints.
 * @param efltIn
 *        eFieldLineTraceDir: Trace direction enumeration,
 *        * efltdNorth         = North
 *        * efltdSouth         = South
 *        * efltdNorthAndSouth = Both North and South
 * @param elmIn
 *        eLshellMode: L shell mode enumeration,
 *        * elmDipole   = Simple dipole
 *        * elmMcIlwain = McIlwain
 *        
 * @param[out] ps3cvFPs
 *        S3CoordVec: Returned vector of footprint coordinates, geocentric Cartesian in km.
 * @param[out] dL
 *        double: Returned L shell value.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeFootPrint( const S3Coord& s3C,
                                         const double& dAltSurf,
                                         const eFieldLineTraceDir& efltIn,
                                         const eLshellMode& elmIn,
                                         S3CoordVec& s3CvFPs,
                                         double& dL );

/*------------------------------------------------------------------*/
// function CMagfield::computeFootPrint
/**
 *
 * Traces fieldline to its footprint location. Footprints with
 * direction of North and South have two points returned and one
 * L shell value.
 *
 * @param tTime
 *        CTimeValue: Time value object.
 * @param s3C
 *        S3Coord: Start position in Geocentric Cartesian km coordinates.
 * @param dAltSurf
 *        double: Footprint altitude in km; an altitude of zero indicates ground footprints.
 * @param efltIn
 *        eFieldLineTraceDir: Trace direction enumeration,
 *        * efltdNorth         = North
 *        * efltdSouth         = South
 *        * efltdNorthAndSouth = Both North and South
 * @param elmIn
 *        eLshellMode: L shell mode enumeration,
 *        * elmDipole   = Simple dipole
 *        * elmMcIlwain = McIlwain
 *        
 * @param[out] ps3cvFPs
 *        S3CoordVec: Returned vector of footprint coordinates, geocentric Cartesian in km.
 * @param[out] dL
 *        double: Returned L shell value.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeFootPrint( const CTimeValue& tTime,
                                         const S3Coord& s3C,
                                         const double& dAltSurf,
                                         const eFieldLineTraceDir& efltIn,
                                         const eLshellMode& elmIn,
                                         S3CoordVec& s3CvFPs,
                                         double& dL );

/*------------------------------------------------------------------*/
// function CMagfield::computeFootPrint
/**
 *
 * Traces fieldline to its footprint location. Footprints with
 * direction of North and South have two points returned and one
 * L shell value.
 *
 * @param dTime
 *        double: Time in MJD or composite ("GeoSpace") form.
 * @param s3C
 *        S3Coord: Start position in Geocentric Cartesian km coordinates.
 * @param dAltSurf
 *        double: Footprint altitude in km; an altitude of zero indicates ground footprints.
 * @param efltIn
 *        eFieldLineTraceDir: Trace direction enumeration,
 *        * efltdNorth         = North
 *        * efltdSouth         = South
 *        * efltdNorthAndSouth = Both North and South
 * @param elmIn
 *        eLshellMode: L shell mode enumeration,
 *        * elmDipole   = Simple dipole
 *        * elmMcIlwain = McIlwain
 *        
 * @param[out] ps3cvFPs
 *        S3CoordVec: Returned vector of footprint coordinates, geocentric Cartesian in km.
 * @param[out] dL
 *        double: Returned L shell value.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeFootPrint( const double& dTime,
                                         const S3Coord& s3C,
                                         const double& dAltSurf,
                                         const eFieldLineTraceDir& efltIn,
                                         const eLshellMode& elmIn,
                                         S3CoordVec& s3CvFPs,
                                         double& dL );

/*------------------------------------------------------------------*/
// function CMagfield::getFieldLineLocation
/**
 * Use previously-obtained field line trace points to return position(s)
 * of field line at a specific altitude (alt < 0 means surface of oblate
 * spheroid) -- allows multiple 'footprint' locations to be determined
 * from single trace results. These field line trace results must be
 * unaltered from their original form/ordering.
 *
 *    @param s3CvFLs
 *           S3CoordVec: Vector of field line positions.
 *    @param dAlt
 *           double: Altitude.
 *      
 *    @param elmIn
 *           eLshellMode: L shell mode enumeration,
 *           * elmDipole   = Dipole mode
 *           * elmMcIlwain = McIlwain mode
 *    
 *    @param[out] ps3CvFPs
 *           S3CoordVec: Returned vector of field line coordinates at the
 *                           specified altitude, geocentric Cartesian in km.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE getFieldLineLocation ( const S3CoordVec& s3CvFLs,
                                              const double& dAlt,
                                              const eFieldLineTraceDir& efltIn,
                                              S3CoordVec* ps3CvFPs );

/*------------------------------------------------------------------*/
// function CMagfield::getFieldLineLocation
/**
 * Use previously-obtained field line trace points to return position(s)
 * of field line at a specific altitude (alt < 0 means surface of oblate
 * spheroid) -- allows multiple 'footprint' locations to be determined
 * from single trace results. These field line trace results must be
 * unaltered from their original form/ordering.
 *
 *    @param s3CvFLs
 *           S3CoordVec: Vector of field line positions.
 *    @param dAlt
 *           double: Altitude.
 *      
 *    @param strFltIn
 *           string: L shell mode (case insensitve),
 *           * "Dipole"   | "D" | "Ldip" = Dipole mode
 *           * "McIlwain" | "M" | "Lm"   = McIlwain mode
 *    
 *    @param[out] ps3CvFPs
 *           S3CoordVec: Returned vector of field line coordinates at the
 *                           specified altitude, geocentric Cartesian in km.
 *
 *    @returns
 *           emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE getFieldLineLocation ( const S3CoordVec& s3CvFLs,
                                              const double& dAlt,
                                              const string& strFltIn, 
                                              S3CoordVec* ps3CvFPs );

/*------------------------------------------------------------------*/
// function CMagfield::getOblateRadius
/**
 *
 * Get radius of oblate spheroid at given geocentric position.
 *
 * @param s3cPos
 *        S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 * @returns
 *        Radius of oblate spheroid at given position
 */
  double getOblateRadius( const S3Coord s3cPos );

/*------------------------------------------------------------------*/
  // ** FieldLine Conjugacy **

  // computeIsConjugate(): All input coords are Geocentric Cartesian
  // km.  LshellMode defines the fieldline tracing method as above.
  // dTolerance specifies a separation in L; separations less than
  // this delta-L are deemed to be "conjugate".  Two coordinates are
  // checked for conjugacy and *pbIsConj set true if they are.
/*------------------------------------------------------------------*/
// function CMagfield::computeIsConjugate
/**
 * Check given coordinate pair for conjugacy.
 *
 * @param tTime
 *        CTimeValue: Time value object.
 * @param s3C1
 *        S3Coord: First position coordinates, geocentric Cartesian in km.
 * @param s3C2
 *        S3Coord: Second position coordinates, geocentric Cartesian in km.
 * @param dTolerance
 *        double: Tolerance as a separation in L.
 * @param elmIn
 *        eLshellMode: L shell mode enumeration,
 *        * elmDipole   = Dipole mode
 *        * elmMcIlwain = McIlwain mode
 *
 * @param[out] pdvLs
 *        dvector: Returned vector of L values of the pair.
 *
 * @param pbIsConj
 *        bool: Returned conjugacy flag.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeIsConjugate( const CTimeValue& tTime,
                                           const S3Coord& s3C1,
                                           const S3Coord& s3C2,
                                           const double& dTolerance,
                                           const eLshellMode& elmIn,
                                           dvector* pdvLs,
                                           bool* pbIsConj );

/*------------------------------------------------------------------*/
// function CMagfield::computeIsConjugate
/**
 * Check given coordinate pair for conjugacy.
 *
 * @param dTime
 *        double: Time in MJD or composite ("GeoSpace") form.
 * @param s3C1
 *        S3Coord: First position coordinates, geocentric Cartesian in km.
 * @param s3C2
 *        S3Coord: Second position coordinates, geocentric Cartesian in km.
 * @param dTolerance
 *        double: Tolerance as a separation in L.
 * @param elmIn
 *        eLshellMode: L shell mode enumeration,
 *        * elmDipole   = Dipole mode
 *        * elmMcIlwain = McIlwain mode
 *
 * @param[out] pdvLs
 *        dvector: Returned vector of L values of the pair.
 *
 * @param pbIsConj
 *        bool: Returned conjugacy flag.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeIsConjugate( const double& dTime,
                                           const S3Coord& s3C1,
                                           const S3Coord& s3C2,
                                           const double& dTolerance,
                                           const eLshellMode& elmIn,
                                           dvector* pdvLs,
                                           bool* pbIsConj );

/*------------------------------------------------------------------*/
// function CMagfield::computeIsConjugate
/**
 * Check given coordinate pair for conjugacy.
 *
 * @param dTime
 *        double: Time in MJD or composite ("GeoSpace") form.
 * @param s3C1
 *        S3Coord: First position coordinates, geocentric Cartesian in km.
 * @param s3C2
 *        S3Coord: Second position coordinates, geocentric Cartesian in km.
 * @param dTolerance
 *        double: Tolerance as a separation in L.
 * @param strLmIn
 *        string: L shell mode (case insensitve),
 *        * "Dipole"   | "D" | "Ldip" = Dipole mode
 *        * "McIlwain" | "M" | "Lm"   = McIlwain mode
 *
 * @param[out] pdvLs
 *        dvector: Returned vector of L values of the pair.
 *
 * @param piIsConj
 *        int: Returned conjugacy flag.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeIsConjugate( const double& dTime,
                                           const S3Coord& s3C1,
                                           const S3Coord& s3C2,
                                           const double& dTolerance,
                                           const string& strLmIn,
                                           dvector* pdvLs,
                                           int* piIsConj ); 

/*------------------------------------------------------------------*/
  // no-com
/*------------------------------------------------------------------*/
// function CMagfield::computeIsConjugate
/**
 * Check given coordinate pair for conjugacy.
 *
 * @param s3C1
 *        S3Coord: First position coordinates, geocentric Cartesian in km.
 * @param s3C2
 *        S3Coord: Second position coordinates, geocentric Cartesian in km.
 * @param dTolerance
 *        double: Tolerance as a separation in L.
 * @param elmIn
 *        eLshellMode: L shell mode enumeration,
 *        * elmDipole   = Dipole mode
 *        * elmMcIlwain = McIlwain mode
 *
 * @param[out] pdvLs
 *        dvector: Returned vector of L values of the pair.
 *
 * @param pbIsConj
 *        bool: Returned conjugacy flag.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeIsConjugate( const S3Coord& s3C1,
                                           const S3Coord& s3C2,
                                           const double& dTolerance,
                                           const eLshellMode& elmIn,
                                           dvector* pdvLs,
                                           bool* pbIsConj );

/*------------------------------------------------------------------*/
// function CMagfield::computeIsConjugate
/**
 * Check given coordinate pair for conjugacy.
 *
 * @param s3C1
 *        S3Coord: First position coordinates, geocentric Cartesian in km.
 * @param s3C2
 *        S3Coord: Second position coordinates, geocentric Cartesian in km.
 * @param dTolerance
 *        double: Tolerance as a separation in L.
 * @param strLmIn
 *        string: L shell mode (case insensitve),
 *        * "Dipole"   | "D" | "Ldip"  Dipole mode
 *        * "McIlwain" | "M" | "Lm"    McIlwain mode
 *
 * @param[out] pdvLs
 *        dvector: Returned vector of L values of the pair.
 *
 * @param piIsConj
 *        int: Returned conjugacy flag.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeIsConjugate( const S3Coord& s3C1,
                                           const S3Coord& s3C2,
                                           const double& dTolerance,
                                           const string& strLmIn, 
                                           dvector* pdvLs,
                                           int* piIsConj ); 

/*------------------------------------------------------------------*/
// function CMagfield::computeIsConjugate
/**
 * Check given coordinate pair for conjugacy.
 *
 * @param s3C1
 *        S3Coord: First position coordinates, geocentric Cartesian in km.
 * @param s3C2
 *        S3Coord: Second position coordinates, geocentric Cartesian in km.
 * @param dTolerance
 *        double: Tolerance as a separation in L.
 * @param elmIn
 *        eLshellMode: L shell mode enumeration,
 *        * elmDipole   = Dipole mode
 *        * elmMcIlwain = McIlwain mode
 *
 * @param[out] pdvLs
 *        dvector: Returned vector of L values of the pair.
 *
 * @param pbIsConj
 *        bool: Returned conjugacy flag.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeIsConjugate( const S3Coord& s3C1,
                                           const S3Coord& s3C2,
                                           const double& dTolerance,
                                           const eLshellMode& elmIn,
                                           dvector& dvLs,
                                           bool& bIsConj );

/*------------------------------------------------------------------*/
// function CMagfield::computeIsConjugate
/**
 * Check given coordinate pair for conjugacy.
 *
 * @param tTime
 *        CTimeValue: Time value object.
 * @param s3C1
 *        S3Coord: First position coordinates, geocentric Cartesian in km.
 * @param s3C2
 *        S3Coord: Second position coordinates, geocentric Cartesian in km.
 * @param dTolerance
 *        double: Tolerance as a separation in L.
 * @param elmIn
 *        eLshellMode: L shell mode enumeration,
 *        * elmDipole   = Dipole mode
 *        * elmMcIlwain = McIlwain mode
 *
 * @param[out] dvLs
 *        dvector: Returned vector of L values of the pair.
 *
 * @param bIsConj
 *        bool: Returned conjugacy flag.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeIsConjugate( const CTimeValue& tTime,
                                           const S3Coord& s3C1,
                                           const S3Coord& s3C2,
                                           const double& dTolerance,
                                           const eLshellMode& elmIn,
                                           dvector& dvLs,
                                           bool& bIsConj );

/*------------------------------------------------------------------*/
// function CMagfield::computeIsConjugate
/**
 * Check given coordinate pair for conjugacy.
 *
 * @param dTime
 *        double: Time in MJD or composite ("GeoSpace") form.
 * @param s3C1
 *        S3Coord: First position coordinates, geocentric Cartesian in km.
 * @param s3C2
 *        S3Coord: Second position coordinates, geocentric Cartesian in km.
 * @param dTolerance
 *        double: Tolerance as a separation in L.
 * @param elmIn
 *        eLshellMode: L shell mode enumeration,
 *        * elmDipole   = Dipole mode
 *        * elmMcIlwain = McIlwain mode
 *
 * @param[out] dvLs
 *        dvector: Returned vector of L values of the pair.
 *
 * @param bIsConj
 *        bool: Returned conjugacy flag.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeIsConjugate( const double& dTime,
                                           const S3Coord& s3C1,
                                           const S3Coord& s3C2,
                                           const double& dTolerance,
                                           const eLshellMode& elmIn,
                                           dvector& dvLs,
                                           bool& bIsConj );

  // Message routines
/*------------------------------------------------------------------*/
// function CMagfield::message
  /**
  * Return message for a given error code.
  *
  * @param code
  *     eMAGFIELD_ERROR_CODE: error code enum
  *
  * @return
  *    string: associated message
  */
  const string message( const eMAGFIELD_ERROR_CODE &code );

/*------------------------------------------------------------------*/
// function CMagfield::message
  /**
  * Return error message from underlying class
  *
  * @return
  *    string: error message
  */
  const string message() { return(GenericModel::message()); }


  // *************************************************************************//
  // * No-COM: Routines below this line do NOT need to be called through COM *//
  // *************************************************************************//

  // light-weight wrappers for CMagField::runInvarmRLL routines
/*------------------------------------------------------------------*/
// function CMagfield::computeAdInvariants
/**
 *
 * Pass in RLL (radius in Km, lat in degrees, lon in degrees) and
 * call CMagField::runInvarmRLL.
 *
 * @param tTime
 *        CTimeValue: Time value object.
 * @param s3CRLLinKM
 *        S3Coord: Position coordinates, Radius/Latitude/Longitude in km.
 * @param dvPitchAngles
 *        dvector: Vector of pitch angles.
 *
 * @param[out] pdBmin
 *        double: Returned Bmin.
 * @param[out] pdvBvec
 *        dvector: Returned vector (3 components) of B vector.
 * @param[out] pdvLm
 *        dvector: Returned vector (over PA) of L values.
 * @param[out] pdvXj
 *        dvector: Returned vector (over PA) of I values.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeAdInvariants( const CTimeValue& tTime,
                                            const S3Coord& s3CRLLinKM,
                                            const dvector& dvPitchAngles,
                                            double* pdBmin,
                                            dvector* pdvBvec,
                                            dvector* pdvLm,
                                            dvector* pdvXj );

/*------------------------------------------------------------------*/
// function CMagfield::computeAdInvariants
/**
 *
 * Pass in RLL (radius in Km, lat in degrees, lon in degrees) and
 * call CMagField::runInvarmRLL.
 *
 * @param dTime
 *        double: Time in MJD or composite ("GeoSpace") form.
 * @param s3CRLLinKM
 *        S3Coord: Position coordinates, Radius/Latitude/Longitude in km.
 * @param dvPitchAngles
 *        dvector: Vector of pitch angles.
 *
 * @param[out] pdBmin
 *        double: Returned Bmin.
 * @param[out] pdvBvec
 *        dvector: Returned vector (3 components) of B vector.
 * @param[out] pdvLm
 *        dvector: Returned vector (over PA) of L values.
 * @param[out] pdvXj
 *        dvector: Returned vector (over PA) of I values.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeAdInvariants( const double& dTime,
                                            const S3Coord& s3CRLLinKM,
                                            const dvector& dvPitchAngles,
                                            double* pdBmin,
                                            dvector* pdvBvec,
                                            dvector* pdvLm,
                                            dvector* pdvXj );

/*------------------------------------------------------------------*/
// function CMagfield::computeAdInvariants
/**
 *
 * Pass in RLL (radius in Km, lat in degrees, lon in degrees) and
 * call CMagField::runInvarmRLL.
 *
 * @param tTime
 *        CTimeValue: Time value object.
 * @param s3CRLLinKM
 *        S3Coord: Position coordinates, Radius/Latitude/Longitude in km.
 * @param dvPitchAngles
 *        dvector: Vector of pitch angles.
 *
 * @param[out] pdBmin
 *        double: Returned Bmin.
 * @param[out] pdBminXyz
 *        double: Returned Bmin location.
 * @param[out] pdvB
 *        dvector: Returned vector (3 components) of B vector.
 * @param[out] pdvLm
 *        dvector: Returned vector (over PA) of L values.
 * @param[out] pdvXj
 *        dvector: Returned vector (over PA) of I values.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeAdInvariants( const CTimeValue& tTime,
                                            const S3Coord& s3CRLLinKM,
                                            const dvector& dvPitchAngles,
                                            double* pdBmin,
                                            double* pdBminXyz,
                                            dvector* pdvBvec,
                                            dvector* pdvLm,
                                            dvector* pdvXj );

/*------------------------------------------------------------------*/
// function CMagfield::computeAdInvariants
/**
 *
 * Pass in RLL (radius in Km, lat in degrees, lon in degrees) and
 * call CMagField::runInvarmRLL.
 *
 * @param dTime
 *        double: Time in MJD or composite ("GeoSpace") form.
 * @param s3CRLLinKM
 *        S3Coord: Position coordinates, Radius/Latitude/Longitude in km.
 * @param dvPitchAngles
 *        dvector: Vector of pitch angles.
 *
 * @param[out] pdBmin
 *        double: Returned Bmin.
 * @param[out] pdBminXyz
 *        double: Returned Bmin location.
 * @param[out] pdvBvec
 *        dvector: Returned vector (3 components) of B vector.
 * @param[out] pdvLm
 *        dvector: Returned vector (over PA) of L values.
 * @param[out] pdvXj
 *        dvector: Returned vector (over PA) of I values.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeAdInvariants( const double& dTime,
                                            const S3Coord& s3CRLLinKM,
                                            const dvector& dvPitchAngles,
                                            double* pdBmin,
                                            double* pdBminXyz,
                                            dvector* pdvBvec,
                                            dvector* pdvLm,
                                            dvector* pdvXj );

/*------------------------------------------------------------------*/
// function CMagfield::computeAdInvariants
/**
 *
 * Pass in RLL (radius in Km, lat in degrees, lon in degrees) and
 * call CMagField::runInvarmRLL.
 *
 * @param dTime
 *        double: Time in MJD or composite ("GeoSpace") form.
 * @param s3CRLLinKM
 *        S3Coord: Position coordinates, Radius/Latitude/Longitude in km.
 * @param dvPitchAngles
 *        dvector: Vector of pitch angles.
 * @param bUnused
 *        bool: Not used (present for backwards compatibility).
 *
 * @param[out] pdBmin
 *        double: Returned Bmin.
 * @param[out] pdvBvec
 *        dvector: Returned vector (3 components) of B vector.
 * @param[out] pdvLm
 *        dvector: Returned vector (over PA) of L values.
 * @param[out] pdvXj
 *        dvector: Returned vector (over PA) of I values.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeAdInvariants( const double& dTime,
                                            const S3Coord& s3CRLLinKM,
                                            const dvector& dvPitchAngles,
                                            const bool& bUnused,
                                            double* pdBmin,
                                            double* pdBminXyz,
                                            dvector* pdvBvec,
                                            dvector* pdvLm,
                                            dvector* pdvXj );

/*------------------------------------------------------------------*/
// function CMagfield::computeAdInvariants
/**
 *
 * Pass in RLL (radius in Km, lat in degrees, lon in degrees) and
 * call CMagField::runInvarmRLL.
 *
 * @param dTime
 *        double: Time in MJD or composite ("GeoSpace") form.
 * @param s3CRLLinKM
 *        S3Coord: Position coordinates, Radius/Latitude/Longitude in km.
 * @param dvPitchAngles
 *        dvector: Vector of pitch angles.
 * @param bUnused
 *        bool: Not used (present for backwards compatibility).
 * @param bUnused2
 *        bool: Not used (present for backwards compatibility).
 *
 * @param[out] pdBmin
 *        double: Returned Bmin.
 * @param[out] pdvBvec
 *        dvector: Returned vector (3 components) of B vector.
 * @param[out] pdvLm
 *        dvector: Returned vector (over PA) of L values.
 * @param[out] pdvXj
 *        dvector: Returned vector (over PA) of I values.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeAdInvariants( const double& dTime,
                                            const S3Coord& s3CRLLinKM,
                                            const dvector& dvPitchAngles,
                                            const bool& bUnused,
                                            const bool& bUnused2,
                                            double* pdBmin,
                                            double* pdBminXyz,
                                            dvector* pdvBvec,
                                            dvector* pdvLm,
                                            dvector* pdvXj );

/*------------------------------------------------------------------*/
// function CMagfield::setCompatMode
/**
 *
 * Adjust behavior of computeAdInvariants method results, affecting Lm trace radial threshold: <br>
 * A lower threshold enables 'computeAdInvariants' method to be able to sometimes compute valid Lm values at lower pitch angles. <br>
 * This 'original' mode is to be phased out in ~2024, for IRENE v1.60 release
 *
 * @param bState
 *        bool: compatibility mode setting. [Default=True]<br>
 *              * True  -> 1.0 Re  ("original") - used for IRENE 1.5x and prior in Ae9Ap9Model.cpp
 *              * False -> 0.9 Re ("new", default state)
 */
  void setCompatMode( bool bState=true ) { csCMagField->setCompatMode( bState ); }

/*------------------------------------------------------------------*/
// function CMagFieldLib::convertCoord
/**
 *
 * Converts a coordinate-triple in one coordinate system to another
 *  coordinate system.
 *
 * @param emfInputCoordSys
 *        emfCoordSys: Input coordinate system enumeration,
 *        * SPHinRE   = Radius (RE), Theta (colat) (deg), Phi (deg) (spherical)
 *        * GSMinRE   = x (RE), y (RE), z (RE) (geocentric solar magnetospheric)
 *        * SMinRE    = x (RE), y (RE), z (RE) (solar magnetic)
 *        * GEIinRE   = x (RE), y (RE), z (RE) (Geocentric Equatorial Inertial)
 *        * GEOinRE   = x (RE), y (RE), z (RE) (Geographic Cartesian)
 *        * MLLinRE   = Radius, (RE), MLat (deg), MLT (hr) (magnetic dipole)
 *        * MAGinRE   = x (RE), y (RE), z (RE) (dipole magnetic Cartesian)
 *        * MAGSinRE  = Radius (RE), Mlat (deg), Mlon (deg)
 *        * GSEinRE   = x (RE), y (RE), z (RE) (Geocentric Solar Ecliptic)
 *        * GSEQinRE  = x (RE), y (RE), z (RE) (geocentric solar equatorial)
 *        * ODXinRE   = mx (RE), my (RE), mz (RE) (offset-dipole Cartesian) 
 *        * ODSinRE   = Radius (RE), mlat (deg), mlon (deg) (offset-dipole Spherical)
 *        * ODXinKM   = mx (km), my (km), mz (km) (offset-dipole Cartesian) 
 *        * ODSinKM   = Radius (km), mlat (deg), mlon (deg) (offset-dipole Spherical)
 *        * SPHinKM   = Radius, (km), co-Latitude (deg), Phi (deg) (spherical)
 *        * GSMinKM   = x (km), y (km), z (km) (geocentric solar magnetospheric)
 *        * SMinKM    = x (km), y (km), z (km) (solar magnetic)
 *        * GEIinKM   = x (km), y (km), z (km) (Geocentric Equatorial Inertial)
 *        * GEOinKM   = x (km), y (km), z (km) (Geographic Cartesian)
 *        * MLLinKM   = Radius, (km), MLat (deg), MLT (hr) (magnetic dipole)
 *        * MAGinKM   = x (km), y (km), z (km) (dipole magnetic Cartesian)
 *        * MAGSinKM  = Radius (km), Mlat (deg), Mlon (deg)
 *        * GSEinKM   = x (km), y (km), z (km) (Geocentric Solar Ecliptic)
 *        * GSEQinKM  = x (km), y (km), z (km) (geocentric solar equatorial)
 *        * GEODinKM  = alt (km), lat (deg), lon (deg) (wgs84 geodetic)
 *        * GDZinKM   = alt (km), lat (deg), lon (deg) (wgs84 geodetic) (synonym for GEODinKM)
 *        * RLLinRE   = r (RE), lat (deg), lon (deg) (like SPHinRE but colat->lat)
 *        * RLLinKM   = r (km), lat (deg), lon (deg) (like SPHinKM but colat->lat)
 * @param emfOutputCoordSys
 *        emfCoordSys: Output coordinate system enumeration, see emfInputCoordSys.
 * @param s3cIn
 *        S3Coord: double vector holding input coordinate (3 elements)
 *
 * @param[out] ps3cOut
 *        S3Coord: Returned double vector holding output coordinate (3 elements)
 *
 * @returns 
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  // Generic conversion wrappers for CMagFieldLib::convertCoord()
  eMAGFIELD_ERROR_CODE convertCoord( const emfCoordSys& eInputCoordSys,
                                     const emfCoordSys& eOutputCoordSys,
                                     const S3Coord& s3cIn,
                                     S3Coord* ps3cOut );

/*------------------------------------------------------------------*/
// function CMagFieldLib::convertCoord
/**
 *
 * Converts a coordinate-triple in one coordinate system to another
 *  coordinate system.
 *
 * @param strInCoordSys
 *        string: string representation of input coordinate system (case insensitive),
 *        * "SPH"          = Spherical
 *        * "GSM"          = Geocentric Solar Magnetospheric
 *        * "SM"  | "SSM"  = Solar Magnetic
 *        * "GEI" | "ECI"  = Geocentric Equatorial Inertial
 *        * "GEO" | "ECEF" = Geographic Cartesian
 *        * "MLL"          = Magnetic Dipole
 *        * "MAG"          = Dipole Magnetic Cartesian
 *        * "MAGS"         = Dipole Magnetic Spherical
 *        * "GSE"          = Geocentric Solar Ecliptic
 *        * "GSEQ"         = Geocentric Solar Equatorial
 *        * "ODX"          = Offset Dipole Cartesian
 *        * "ODS"          = Offset Dipole Spherical
 *        * "GDZ" | "GEOD" = WGS 84 Geodetic
 *        * "RLL"          = Spherical but with colat->lat
 * @param strInCoordSysUnits
 *        string: string representation of input coordinate units (case insensitive)
 *        * "Km"       Kilometers
 *        * "Re" | ""  Earth radii
 * @param strOutCoordSys
 *        string: string representation of output coordinate system (see strInCoordSys)
 * @param strOutCoordSysUnits
 *        string: string representation of output coordinate units (see strInCoordSysUnits)
 * @param s3cIn
 *        S3Coord: double vector holding input coordinate (3 elements)
 *
 * @param[out] ps3cOut
 *        S3Coord: Returned double vector holding output coordinate (3 elements)
 *
 * @returns 
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertCoord( const string& strInCoordSys,
                                     const string& strInCoordSysUnits,
                                     const string& strOutCoordSys,
                                     const string& strOutCoordSysUnits,
                                     const S3Coord& s3cIn,
                                     S3Coord* ps3cOut ); 

/*------------------------------------------------------------------*/
// function CMagFieldLib::convertCoord
/**
 *
 * Converts a coordinate-triple in one coordinate system to another
 *  coordinate system.
 *
 * @param emfInputCoordSys
 *        emfCoordSys: Input coordinate system enumeration,
 *        * SPHinRE   = Radius (RE), Theta (colat) (deg), Phi (deg) (spherical)
 *        * GSMinRE   = x (RE), y (RE), z (RE) (geocentric solar magnetospheric)
 *        * SMinRE    = x (RE), y (RE), z (RE) (solar magnetic)
 *        * GEIinRE   = x (RE), y (RE), z (RE) (Geocentric Equatorial Inertial)
 *        * GEOinRE   = x (RE), y (RE), z (RE) (Geographic Cartesian)
 *        * MLLinRE   = Radius, (RE), MLat (deg), MLT (hr) (magnetic dipole)
 *        * MAGinRE   = x (RE), y (RE), z (RE) (dipole magnetic Cartesian)
 *        * MAGSinRE  = Radius (RE), Mlat (deg), Mlon (deg)
 *        * GSEinRE   = x (RE), y (RE), z (RE) (Geocentric Solar Ecliptic)
 *        * GSEQinRE  = x (RE), y (RE), z (RE) (geocentric solar equatorial)
 *        * ODXinRE   = mx (RE), my (RE), mz (RE) (offset-dipole Cartesian) 
 *        * ODSinRE   = Radius (RE), mlat (deg), mlon (deg) (offset-dipole Spherical)
 *        * ODXinKM   = mx (km), my (km), mz (km) (offset-dipole Cartesian) 
 *        * ODSinKM   = Radius (km), mlat (deg), mlon (deg) (offset-dipole Spherical)
 *        * SPHinKM   = Radius, (km), co-Latitude (deg), Phi (deg) (spherical)
 *        * GSMinKM   = x (km), y (km), z (km) (geocentric solar magnetospheric)
 *        * SMinKM    = x (km), y (km), z (km) (solar magnetic)
 *        * GEIinKM   = x (km), y (km), z (km) (Geocentric Equatorial Inertial)
 *        * GEOinKM   = x (km), y (km), z (km) (Geographic Cartesian)
 *        * MLLinKM   = Radius, (km), MLat (deg), MLT (hr) (magnetic dipole)
 *        * MAGinKM   = x (km), y (km), z (km) (dipole magnetic Cartesian)
 *        * MAGSinKM  = Radius (km), Mlat (deg), Mlon (deg)
 *        * GSEinKM   = x (km), y (km), z (km) (Geocentric Solar Ecliptic)
 *        * GSEQinKM  = x (km), y (km), z (km) (geocentric solar equatorial)
 *        * GEODinKM  = alt (km), lat (deg), lon (deg) (wgs84 geodetic)
 *        * GDZinKM   = alt (km), lat (deg), lon (deg) (wgs84 geodetic) (synonym for GEODinKM)
 *        * RLLinRE   = r (RE), lat (deg), lon (deg) (like SPHinRE but colat->lat)
 *        * RLLinKM   = r (km), lat (deg), lon (deg) (like SPHinKM but colat->lat)
 * @param emfOutputCoordSys
 *        emfCoordSys: Output coordinate system enumeration, see emfInputCoordSys.
 * @param s3cIn
 *        S3Coord: double vector holding input coordinate (3 elements)
 *
 * @param[out] s3cOut
 *        S3Coord: Returned double vector holding output coordinate (3 elements)
 *
 * @returns 
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertCoord( const emfCoordSys& eInputCoordSys,
                                     const emfCoordSys& eOutputCoordSys,
                                     const S3Coord& s3cIn,
                                     S3Coord& s3cOut );

/*------------------------------------------------------------------*/
// function CMagFieldLib::convertCoord
/**
 *
 * Converts a coordinate-triple in one coordinate system to another
 *  coordinate system.
 *
 * @param emfInputCoordSys
 *        emfCoordSys: Input coordinate system enumeration,
 *        * SPHinRE   = Radius (RE), Theta (colat) (deg), Phi (deg) (spherical)
 *        * GSMinRE   = x (RE), y (RE), z (RE) (geocentric solar magnetospheric)
 *        * SMinRE    = x (RE), y (RE), z (RE) (solar magnetic)
 *        * GEIinRE   = x (RE), y (RE), z (RE) (Geocentric Equatorial Inertial)
 *        * GEOinRE   = x (RE), y (RE), z (RE) (Geographic Cartesian)
 *        * MLLinRE   = Radius, (RE), MLat (deg), MLT (hr) (magnetic dipole)
 *        * MAGinRE   = x (RE), y (RE), z (RE) (dipole magnetic Cartesian)
 *        * MAGSinRE  = Radius (RE), Mlat (deg), Mlon (deg)
 *        * GSEinRE   = x (RE), y (RE), z (RE) (Geocentric Solar Ecliptic)
 *        * GSEQinRE  = x (RE), y (RE), z (RE) (geocentric solar equatorial)
 *        * ODXinRE   = mx (RE), my (RE), mz (RE) (offset-dipole Cartesian) 
 *        * ODSinRE   = Radius (RE), mlat (deg), mlon (deg) (offset-dipole Spherical)
 *        * ODXinKM   = mx (km), my (km), mz (km) (offset-dipole Cartesian) 
 *        * ODSinKM   = Radius (km), mlat (deg), mlon (deg) (offset-dipole Spherical)
 *        * SPHinKM   = Radius, (km), co-Latitude (deg), Phi (deg) (spherical)
 *        * GSMinKM   = x (km), y (km), z (km) (geocentric solar magnetospheric)
 *        * SMinKM    = x (km), y (km), z (km) (solar magnetic)
 *        * GEIinKM   = x (km), y (km), z (km) (Geocentric Equatorial Inertial)
 *        * GEOinKM   = x (km), y (km), z (km) (Geographic Cartesian)
 *        * MLLinKM   = Radius, (km), MLat (deg), MLT (hr) (magnetic dipole)
 *        * MAGinKM   = x (km), y (km), z (km) (dipole magnetic Cartesian)
 *        * MAGSinKM  = Radius (km), Mlat (deg), Mlon (deg)
 *        * GSEinKM   = x (km), y (km), z (km) (Geocentric Solar Ecliptic)
 *        * GSEQinKM  = x (km), y (km), z (km) (geocentric solar equatorial)
 *        * GEODinKM  = alt (km), lat (deg), lon (deg) (wgs84 geodetic)
 *        * GDZinKM   = alt (km), lat (deg), lon (deg) (wgs84 geodetic) (synonym for GEODinKM)
 *        * RLLinRE   = r (RE), lat (deg), lon (deg) (like SPHinRE but colat->lat)
 *        * RLLinKM   = r (km), lat (deg), lon (deg) (like SPHinKM but colat->lat)
 * @param emfOutputCoordSys
 *        emfCoordSys: Output coordinate system enumeration, see emfInputCoordSys.
 * @param dC1in
 *        double: input coordinate first element
 * @param dC2in
 *        double: input coordinate second element
 * @param dC3in
 *        double: input coordinate third element
 *
 * @param[out] pdC1out
 *        double: Returned output coordinate first element
 * @param[out] pdC2out
 *        double: Returned output coordinate second element
 * @param[out] pdC3out
 *        double: Returned output coordinate third element
 *
 * @returns 
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertCoord( const emfCoordSys& eInputCoordSys,
                                     const emfCoordSys& eOutputCoordSys,
                                     const double& dC1in,
                                     const double& dC2in,
                                     const double& dC3in,
                                     double* pdC1out,
                                     double* pdC2out,
                                     double* pdC3out );

/*------------------------------------------------------------------*/
// function CMagFieldLib::convertCoord
/**
 *
 * Converts a coordinate-triple in one coordinate system to another
 *  coordinate system.
 *
 * @param strInCoordSys
 *        string: string representation of input coordinate system (case insensitive),
 *        * "SPH"          = Spherical
 *        * "GSM"          = Geocentric Solar Magnetospheric
 *        * "SM"  | "SSM"  = Solar Magnetic
 *        * "GEI" | "ECI"  = Geocentric Equatorial Inertial
 *        * "GEO" | "ECEF" = Geographic Cartesian
 *        * "MLL"          = Magnetic Dipole
 *        * "MAG"          = Dipole Magnetic Cartesian
 *        * "MAGS"         = Dipole Magnetic Spherical
 *        * "GSE"          = Geocentric Solar Ecliptic
 *        * "GSEQ"         = Geocentric Solar Equatorial
 *        * "ODX"          = Offset Dipole Cartesian
 *        * "ODS"          = Offset Dipole Spherical
 *        * "GDZ" | "GEOD" = WGS 84 Geodetic
 *        * "RLL"          = Spherical but with colat->lat
 * @param strInCoordSysUnits
 *        string: string representation of input coordinate units (case insensitive)
 *        * "Km"       Kilometers
 *        * "Re" | ""  Earth radii
 * @param strOutCoordSys
 *        string: string representation of output coordinate system (see strInCoordSys)
 * @param strOutCoordSysUnits
 *        string: string representation of output coordinate units (see strInCoordSysUnits)
 * @param dC1in
 *        double: input coordinate first element
 * @param dC2in
 *        double: input coordinate second element
 * @param dC3in
 *        double: input coordinate third element
 *
 * @param[out] pdC1out
 *        double: Returned output coordinate first element
 * @param[out] pdC2out
 *        double: Returned output coordinate second element
 * @param[out] pdC3out
 *        double: Returned output coordinate third element
 *
 * @returns 
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertCoord( const string& strInCoordSys,
                                     const string& strInCoordSysUnits,
                                     const string& strOutCoordSys,
                                     const string& strOutCoordSysUnits,
                                     const double& dC1in,
                                     const double& dC2in,
                                     const double& dC3in,
                                     double* pdC1out,
                                     double* pdC2out,
                                     double* pdC3out ); 

/*------------------------------------------------------------------*/
// function CMagFieldLib::convertCoord
/**
 *
 * Converts a coordinate-triple in one coordinate system to another
 *  coordinate system.
 *
 * @param eInputCoordSys
 *        emfCoordSys: Input coordinate system enumeration,
 *        * SPHinRE   = Radius (RE), Theta (colat) (deg), Phi (deg) (spherical)
 *        * GSMinRE   = x (RE), y (RE), z (RE) (geocentric solar magnetospheric)
 *        * SMinRE    = x (RE), y (RE), z (RE) (solar magnetic)
 *        * GEIinRE   = x (RE), y (RE), z (RE) (Geocentric Equatorial Inertial)
 *        * GEOinRE   = x (RE), y (RE), z (RE) (Geographic Cartesian)
 *        * MLLinRE   = Radius, (RE), MLat (deg), MLT (hr) (magnetic dipole)
 *        * MAGinRE   = x (RE), y (RE), z (RE) (dipole magnetic Cartesian)
 *        * MAGSinRE  = Radius (RE), Mlat (deg), Mlon (deg)
 *        * GSEinRE   = x (RE), y (RE), z (RE) (Geocentric Solar Ecliptic)
 *        * GSEQinRE  = x (RE), y (RE), z (RE) (geocentric solar equatorial)
 *        * ODXinRE   = mx (RE), my (RE), mz (RE) (offset-dipole Cartesian) 
 *        * ODSinRE   = Radius (RE), mlat (deg), mlon (deg) (offset-dipole Spherical)
 *        * ODXinKM   = mx (km), my (km), mz (km) (offset-dipole Cartesian) 
 *        * ODSinKM   = Radius (km), mlat (deg), mlon (deg) (offset-dipole Spherical)
 *        * SPHinKM   = Radius, (km), co-Latitude (deg), Phi (deg) (spherical)
 *        * GSMinKM   = x (km), y (km), z (km) (geocentric solar magnetospheric)
 *        * SMinKM    = x (km), y (km), z (km) (solar magnetic)
 *        * GEIinKM   = x (km), y (km), z (km) (Geocentric Equatorial Inertial)
 *        * GEOinKM   = x (km), y (km), z (km) (Geographic Cartesian)
 *        * MLLinKM   = Radius, (km), MLat (deg), MLT (hr) (magnetic dipole)
 *        * MAGinKM   = x (km), y (km), z (km) (dipole magnetic Cartesian)
 *        * MAGSinKM  = Radius (km), Mlat (deg), Mlon (deg)
 *        * GSEinKM   = x (km), y (km), z (km) (Geocentric Solar Ecliptic)
 *        * GSEQinKM  = x (km), y (km), z (km) (geocentric solar equatorial)
 *        * GEODinKM  = alt (km), lat (deg), lon (deg) (wgs84 geodetic)
 *        * GDZinKM   = alt (km), lat (deg), lon (deg) (wgs84 geodetic) (synonym for GEODinKM)
 *        * RLLinRE   = r (RE), lat (deg), lon (deg) (like SPHinRE but colat->lat)
 *        * RLLinKM   = r (km), lat (deg), lon (deg) (like SPHinKM but colat->lat)
 * @param emfOutputCoordSys
 *        emfCoordSys: Output coordinate system enumeration, see emfInputCoordSys.
 * @param dC1in
 *        double: input coordinate first element
 * @param dC2in
 *        double: input coordinate second element
 * @param dC3in
 *        double: input coordinate third element
 *
 * @param[out] dC1out
 *        double: Returned output coordinate first element
 * @param[out] dC2out
 *        double: Returned output coordinate second element
 * @param[out] dC3out
 *        double: Returned output coordinate third element
 *
 * @returns 
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertCoord( const emfCoordSys& eInputCoordSys,
                                     const emfCoordSys& eOutputCoordSys,
                                     const double& dC1in,
                                     const double& dC2in,
                                     const double& dC3in,
                                     double& dC1out,
                                     double& dC2out,
                                     double& dC3out );

/*------------------------------------------------------------------*/
  // ** Coordinate Conversion Wrappers **
  // GEO = Geocentric Cartesian in x,y,z km
  // SPH = Geocentric Spherical in r,lat,lon in km and degrees (I think lat
  //  should be co-lat here - agl)
  // GSM = Geo-Solar-Magnetic in x,y,z km
  // MAG = Magnetic (tilted or offset-dipole) in x,y,z km
  // other transformations are available directly from CMagFieldLib::
  // single point transforms
  // NOTE: this first set should be hooked up; the overloads can wait. -raq
/*------------------------------------------------------------------*/
// function CMagfield::convertGEOtoSPH
/**
 *
 * Convert Geocentric Cartesian (x, y, z) to Geocentric Spherical
 * (r, lat, lon).
 *
 * @param s3cGeo
 *        S3Coord: GEO coordinates in km.
 *
 * @param[out] ps3cSph
 *        S3Coord: Returned SPH coordinates in Km and degrees.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertGEOtoSPH( const S3Coord& s3cGeo,
                                        S3Coord* ps3cSph );

/*------------------------------------------------------------------*/
// function CMagfield::convertSPHtoGEO
/**
 *
 * Convert Geocentric Spherical (r, lat, lon)i to Geocentric Cartesian
 * (x, y, z).
 *
 * @param s3cSph
 *        S3Coord: SPH coordinates in Km and degrees.
 *
 * @param[out] ps3cGeo
 *        S3Coord: Returned GEO coordinates in km.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertSPHtoGEO( const S3Coord& s3cSph,
                                        S3Coord* ps3cGeo );

/*------------------------------------------------------------------*/
// function CMagfield::convertGEOtoGSM
/**
 *
 * Convert Geocentric Cartesian (x, y, z) to Geo-Solar-Magenetic
 * (x, y, z).
 *
 * @param s3cGeo
 *        S3Coord: GEO coordinates in km.
 *
 * @param[out] ps3cGsm
 *        S3Coord: Returned GSM coordinates in km.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertGEOtoGSM( const S3Coord& s3cGeo,
                                        S3Coord* ps3cGsm );

/*------------------------------------------------------------------*/
// function CMagfield::convertGSMtoGEO
/**
 *
 * Convert Geo-Solar-Magnetic (x, y, z) to Geocentric Cartesian
 * (x, y, z).
 *
 * @param s3cGsm
 *        S3Coord: GSM coordinates in km.
 *
 * @param[out] ps3cGeo
 *        S3Coord: Returned GEO coordinates in km.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertGSMtoGEO( const S3Coord& s3cGsm,
                                        S3Coord* ps3cGeo );

/*------------------------------------------------------------------*/
// function CMagfield::convertGEOtoMAG
/**
 *
 * Convert Geocentric Cartesian (x, y, z) to Magnetic (x, y, z).
 *
 * @param s3cGeo
 *        S3Coord: GEO coordinates in km.
 *
 * @param[out] ps3cMag
 *        S3Coord: Returned MAG coordinates in km.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertGEOtoMAG( const S3Coord& s3cGeo,
                                        S3Coord* ps3cMag );

/*------------------------------------------------------------------*/
// function CMagfield::convertMAGtoGEO
/**
 *
 * Convert Magnetic (x, y, z) to Geocentric Cartesian (x, y, z).
 *
 * @param s3cMag
 *        S3Coord: MAG coordinates in km.
 *
 * @param[out] ps3cGeo
 *        S3Coord: Returned GEO coordinates in km.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertMAGtoGEO( const S3Coord& s3cMag,
                                        S3Coord* ps3cGeo );

/*------------------------------------------------------------------*/
  // vector transforms
/*------------------------------------------------------------------*/
// function CMagfield::convertGEOtoSPH
/**
 *
 * Convert Geocentric Cartesian (x, y, z) to Geocentric Spherical
 * (r, lat, lon).
 *
 * @param s3cvGeo
 *        S3CoordVec: Vector of GEO coordinates in km.
 *
 * @param[out] ps3cvSph
 *        S3CoordVec: Returned vector of SPH coordinates in Km and degrees.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertGEOtoSPH( const S3CoordVec& s3cvGeo,
                                        S3CoordVec* ps3cvSph );

/*------------------------------------------------------------------*/
// function CMagfield::convertSPHtoGEO
/**
 *
 * Convert Geocentric Spherical (r, lat, lon) to Geocentric Cartesian
 * (x, y, z).
 *
 * @param s3cvSph
 *        S3CoordVec: Vector of SPH coordinates in Km and degrees.
 *
 * @param[out] ps3cvGeo
 *        S3CoordVec: Returned vector of GEO coordinates in km.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertSPHtoGEO( const S3CoordVec& s3cvSph,
                                        S3CoordVec* ps3cvGeo );

/*------------------------------------------------------------------*/
// function CMagfield::convertGEOtoGSM
/**
 *
 * Convert Geocentric Cartesian (x, y, z) to Geo-Solar-Magnetic
 * (x, y, z).
 *
 * @param s3cvGeo
 *        S3CoordVec: Vector of GEO coordinates in km.
 *
 * @param[out] ps3cvGsm
 *        S3CoordVec: Returned vector of GSM coordinates in km.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertGEOtoGSM( const S3CoordVec& s3cvGeo,
                                        S3CoordVec* ps3cvGsm );

/*------------------------------------------------------------------*/
// function CMagfield::convertGSMtoGEO
/**
 *
 * Convert Geo-Solar-Magnetic (x, y, z) to Geocentric Cartesian
 * (x, y, x).
 *
 * @param s3cvGsm
 *        S3CoordVec: Vector of GSM coordinates in km.
 *
 * @param[out] ps3cvGeo
 *        S3CoordVec: Returned vector of GEO coordinates in Km and degrees.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertGSMtoGEO( const S3CoordVec& s3cvGsm,
                                        S3CoordVec* ps3cvGeo );

/*------------------------------------------------------------------*/
// function CMagfield::convertGEOtoMAG
/**
 *
 * Convert Geocentric Cartesian (x, y, z) to Magnetic (x, y, z).
 *
 * @param s3cvGeo
 *        S3CoordVec: Vector of GEO coordinates in km.
 *
 * @param[out] ps3cvMag
 *        S3CoordVec: Returned vector of MAG coordinates in km.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertGEOtoMAG( const S3CoordVec& s3cvGeo,
                                        S3CoordVec* ps3cvMag );

/*------------------------------------------------------------------*/
// function CMagfield::convertMAGtoGEO
/**
 *
 * Convert Magnetic (x, y, z) to Geocentric Cartesian (x, y, z).
 *
 * @param s3cvMag
 *        S3CoordVec: Vector of MAG coordinates in km.
 *
 * @param[out] ps3cvGeo
 *        S3CoordVec: Returned vector of GEO coordinates in km.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertMAGtoGEO( const S3CoordVec& s3cvMag,
                                        S3CoordVec* ps3cvGeo );

/*------------------------------------------------------------------*/
// vector transforms with date/time sets
/*------------------------------------------------------------------*/
// function CMagfield::convertGEOtoSPH
/**
 *
 * Convert Geocentric Cartesian (x, y, z) to Geocentric Spherical
 * (r, lat, lon).
 *
 * @param vtTime
 *        vector<CTimeValue>: Vector of time value objects.
 * @param s3cvGeo
 *        S3CoordVec: Vector of GEO coordinates in km.
 *
 * @param[out] ps3cvSph
 *        S3CoordVec: Returned vector of SPH coordinates in Km and degrees.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertGEOtoSPH( const vector<CTimeValue>& vtTime,
                                        const S3CoordVec& s3cvGeo,
                                        S3CoordVec* ps3cvSph );

/*------------------------------------------------------------------*/
// function CMagfield::convertGEOtoSPH
/**
 *
 * Convert Geocentric Cartesian (x, y, z) to Geocentric Spherical
 * (r, lat, lon).
 *
 * @param vdTime
 *        dvector: Vector of times, in MJD or composite ("GeoSpace") form.
 * @param s3cvGeo
 *        S3CoordVec: Vector of GEO coordinates in km.
 *
 * @param[out] ps3cvSph
 *        S3CoordVec: Returned vector of SPH coordinates in Km and degrees.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertGEOtoSPH( const dvector& vdTime,
                                        const S3CoordVec& s3cvGeo,
                                        S3CoordVec* ps3cvSph );

/*------------------------------------------------------------------*/
// function CMagfield::convertSPHtoGEO
/**
 *
 * Convert Geocentric Spherical (r, lat, lon) to Geocentric Cartesian
 * (x, y, z).
 *
 * @param vtTime
 *        vector<CTimeValue>: Vector of time value objects.
 * @param s3cvSph
 *        S3CoordVec: Vector of SPH coordinates in Km and degrees.
 *
 * @param[out] ps3cvGeo
 *        S3CoordVec: Returned vector of SPH coordinates in km.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertSPHtoGEO( const vector<CTimeValue>& vtTime,
                                        const S3CoordVec& s3cvSph,
                                        S3CoordVec* ps3cvGeo );

/*------------------------------------------------------------------*/
// function CMagfield::convertSPHtoGEO
/**
 *
 * Convert Geocentric Spherical (r, lat, lon) to Geocentric Spherical
 * (r, lat, lon).
 *
 * @param vdTime
 *        dvector: Vector of times, in MJD or composite ("GeoSpace") form.
 * @param s3cvSph
 *        S3CoordVec: Vector of SPH coordinates in Km and degrees.
 *
 * @param[out] ps3cvGeo
 *        S3CoordVec: Returned vector of SPH coordinates in km.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertSPHtoGEO( const dvector& vdTime,
                                        const S3CoordVec& s3cvSph,
                                        S3CoordVec* ps3cvGeo );

/*------------------------------------------------------------------*/
// function CMagfield::convertGEOtoGSM
/**
 *
 * Convert Geocentric Cartesian (x, y, z) to Geo-Solar-Magnetic
 * (x, y, z)
 *
 * @param vtTime
 *        vector<CTimeValue>: Vector of time value objects.
 * @param s3cvGeo
 *        S3CoordVec: Vector of GEO coordinates in km.
 *
 * @param[out] ps3cvGsm
 *        S3CoordVec: Returned vector of GSM coordinates in km.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertGEOtoGSM( const vector<CTimeValue>& vtTime,
                                        const S3CoordVec& s3cvGeo,
                                        S3CoordVec* ps3cvGsm );

/*------------------------------------------------------------------*/
// function CMagfield::convertGEOtoGSM
/**
 *
 * Convert Geocentric Cartesian (x, y, z) to Geo-Solar-Magnetic
 * (x, y, z)
 *
 * @param vdTime
 *        dvector: Vector of times, in MJD or composite ("GeoSpace") form.
 * @param s3cvGeo
 *        S3CoordVec: Vector of GEO coordinates in km.
 *
 * @param[out] ps3cvGsm
 *        S3CoordVec: Returned vector of GSM coordinates in km.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertGEOtoGSM( const dvector& vdTime,
                                        const S3CoordVec& s3cvGeo,
                                        S3CoordVec* ps3cvGsm );

/*------------------------------------------------------------------*/
// function CMagfield::convertGSMtoGEO
/**
 *
 * Convert Geo-Solar-Magnetic (x, y, z) to Geocentric Cartesian
 * (x, y, z)
 *
 * @param vtTime
 *        vector<CTimeValue>: Vector of time value objects.
 * @param s3cvGsm
 *        S3CoordVec: Vector of GSM coordinates in km.
 *
 * @param[out] ps3cvGeo
 *        S3CoordVec: Returned vector of GEO coordinates in km.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertGSMtoGEO( const vector<CTimeValue>& vtTime,
                                        const S3CoordVec& s3cvGsm,
                                        S3CoordVec* ps3cvGeo );

/*------------------------------------------------------------------*/
// function CMagfield::convertGSMtoGEO
/**
 *
 * Convert Geo-Solar-Magnetic (x, y, z) to Geocentric Cartesian
 * (x, y, z)
 *
 * @param vdTime
 *        dvector: Vector of times, in MJD or composite ("GeoSpace") form.
 * @param s3cvGsm
 *        S3CoordVec: Vector of GSM coordinates in km.
 *
 * @param[out] ps3cvGeo
 *        S3CoordVec: Returned vector of GEO coordinates in km.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertGSMtoGEO( const dvector& vdTime,
                                        const S3CoordVec& s3cvGsm,
                                        S3CoordVec* ps3cvGeo );

/*------------------------------------------------------------------*/
// function CMagfield::convertGEOtoMAG
/**
 *
 * Convert Geocentric Cartesian (x, y, z) to Magnetic (x, y, z)
 *
 * @param vtTime
 *        vector<CTimeValue>: Vector of time value objects.
 * @param s3cvGeo
 *        S3CoordVec: Vector of GEO coordinates in km.
 *
 * @param[out] ps3cvMag
 *        S3CoordVec: Returned vector of MAG coordinates in km.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertGEOtoMAG( const vector<CTimeValue>& vtTime,
                                        const S3CoordVec& s3cvGeo,
                                        S3CoordVec* ps3cvMag );

/*------------------------------------------------------------------*/
// function CMagfield::convertGEOtoMAG
/**
 *
 * Convert Geocentric Cartesian (x, y, z) to Magnetic (x, y, z)
 *
 * @param vdTime
 *        dvector: Vector of times, in MJD or composite ("GeoSpace") form.
 * @param s3cvGeo
 *        S3CoordVec: Vector of GEO coordinates in km.
 *
 * @param[out] ps3cvMag
 *        S3CoordVec: Returned vector of MAG coordinates in km.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertGEOtoMAG( const dvector& vdTime,
                                        const S3CoordVec& s3cvGeo,
                                        S3CoordVec* ps3cvMag );

/*------------------------------------------------------------------*/
// function CMagfield::convertMAGtoGEO
/**
 *
 * Convert Magnetic (x, y, z) to Geocentric Cartesian (x, y, z)
 *
 * @param vtTime
 *        vector<CTimeValue>: Vector of time value objects.
 * @param s3cvMag
 *        S3CoordVec: Vector of MAG coordinates in km.
 *
 * @param[out] ps3cvGeo
 *        S3CoordVec: Returned vector of GEO coordinates in km.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertMAGtoGEO( const vector<CTimeValue>& vtTime,
                                        const S3CoordVec& s3cvMag,
                                        S3CoordVec* ps3cvGeo );

/*------------------------------------------------------------------*/
// function CMagfield::convertMAGtoGEO
/**
 *
 * Convert Magnetic (x, y, z) to Geocentric Cartesian (x, y, z)
 *
 * @param vdTime
 *        dvector: Vector of times, in MJD or composite ("GeoSpace") form.
 * @param s3cvMag
 *        S3CoordVec: Vector of MAG coordinates in km.
 *
 * @param[out] ps3cvGeo
 *        S3CoordVec: Returned vector of GEO coordinates in km.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertMAGtoGEO( const dvector& vdTime,
                                        const S3CoordVec& s3cvMag,
                                        S3CoordVec* ps3cvGeo );

/*------------------------------------------------------------------*/
  // new ecmDipole routines (private) - Making these public for my
  // convenience in testing - raq
/*------------------------------------------------------------------*/
// function CMagfield::convertVectorSphToXyz
/**
 * 
 * Convert Spherical to Cartesian.
 *
 * @param dTheta
 *        double: dipole polar angle.
 * @param dPhi
 *        double: dipole azimuthal angle.
 * @param dVR
 *        double: radial coordinate.
 * @param dVTheta
 *        double: polar coordinate.
 * @param dVPhi
 *        double: azimuthal coordinate.
 *
 * @param[out] pdVX
 *        double: Returned x coordinate.
 * @param[out] pdVY
 *        double: Returned y coordinate.
 * @param[out] pdVZ
 *        double: Returned z coordinate.
 *
 * @returns
 *        None
 */
  void convertVectorSphToXyz( const double& dTheta,
                              const double& dPhi,
                              const double& dVR,
                              const double& dVTheta,
                              const double& dVPhi,
                              double* pdVX,
                              double* pdVY,
                              double* pdVZ );

/*------------------------------------------------------------------*/
// function CMagfield::convertVectorSphToXyz
/**
 * 
 * Convert Spherical to Cartesian.
 *
 * @param dTheta
 *        double: dipole polar angle.
 * @param dPhi
 *        double: dipole azimuthal angle.
 * @param dVR
 *        double: radial coordinate.
 * @param dVTheta
 *        double: polar coordinate.
 * @param dVPhi
 *        double: azimuthal coordinate.
 *
 * @param[out] dVX
 *        double: Returned x coordinate.
 * @param[out] dVY
 *        double: Returned y coordinate.
 * @param[out] dVZ
 *        double: Returned z coordinate.
 *
 * @returns
 *        None
 */
  void convertVectorSphToXyz( const double& dTheta,
                              const double& dPhi,
                              const double& dVR,
                              const double& dVTheta,
                              const double& dVPhi,
                              double& dVX,
                              double& dVY,
                              double& dVZ );

/*------------------------------------------------------------------*/
// function CMagfield::convertVectorXyzToSph
/**
 * 
 * Convert Cartesian to Spherical.
 *
 * @param dTheta
 *        double: dipole polar angle.
 * @param dPhi
 *        double: dipole azimuthal angle.
 * @param dVX
 *        double: x coordinate.
 * @param dVY
 *        double: y coordinate.
 * @param dVZ
 *        double: z coordinate.
 *
 * @param[out] pdVR
 *        double: Returned radial coordinate.
 * @param[out] pdVTheta
 *        double: Returned polar coordinate.
 * @param[out] pdPhi
 *        double: Returned azimuthal coordinate.
 *
 * @returns
 *        None
 */
  void convertVectorXyzToSph( const double& dTheta,
                              const double& dPhi,
                              const double& dVX,
                              const double& dVY,
                              const double& dVZ,
                              double* pdVR,
                              double* pdVTheta,
                              double* pdVPhi );

/*------------------------------------------------------------------*/
// function CMagfield::convertVectorXyzToSph
/**
 * 
 * Convert Cartesian to Spherical.
 *
 * @param dTheta
 *        double: dipole polar angle.
 * @param dPhi
 *        double: dipole azimuthal angle.
 * @param dVX
 *        double: x coordinate.
 * @param dVY
 *        double: y coordinate.
 * @param dVZ
 *        double: z coordinate.
 *
 * @param[out] dVR
 *        double: Returned radial coordinate.
 * @param[out] dVTheta
 *        double: Returned polar coordinate.
 * @param[out] dVPhi
 *        double: Returned azimuthal coordinate.
 *
 * @returns
 *        None
 */
  void convertVectorXyzToSph( const double& dTheta,
                              const double& dPhi,
                              const double& dVX,
                              const double& dVY,
                              const double& dVZ,
                              double& dVR,
                              double& dVTheta,
                              double& dVPhi );

/*------------------------------------------------------------------*/
// function CMagfield::convertDipGeoToMags
/**
 *
 * Convert Offset dipole to Magnetic Spherical coordinates
 *
 * @param s3cIn
 *        S3Coord: Offset dipole coordinates.
 *        
 * @param[out] ps3cOut
 *        S3Coord: Magnetic Spherical coordinates.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertDipGeoToMags( const S3Coord& s3cIn,
                                            S3Coord* ps3cOut );

/*------------------------------------------------------------------*/
// function CMagfield::convertDipMagsToGeo
/**
 *
 * Convert Magnetic Spherical to Offset dipole Magnetic coordinates
 *
 * @param s3cIn
 *        S3Coord: Magnetic Spherical coordinates.
 *        
 * @param[out] ps3cOut
 *        S3Coord: Offset dipole coordinates.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE convertDipMagsToGeo( const S3Coord& s3cIn,
                                            S3Coord* ps3cOut );

/*------------------------------------------------------------------*/
// function CMagfield::computeDipL
/**
 *
 * Compute L dipole value.
 *
 * @param s3cIn
 *        S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 * @param[out] pdLdip
 *        double: L value.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeDipL( const S3Coord& s3cIn,
                                    double* pdLdip );

/*------------------------------------------------------------------*/
// function CMagfield::computeDipL
/**
 *
 * Compute L dipole value.
 *
 * @param s3cIn
 *        S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 * @param[out] dLdip
 *        double: Returned L value.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeDipL( const S3Coord& s3cIn,
                                    double& dLdip );

/*------------------------------------------------------------------*/
// function CMagfield::computeDipLBBeq
/**
 *
 * Compute L, B, and Beq values.
 *
 * @param s3cIn
 *        S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 * @param[out] pdLdip
 *        double: Returned L value.
 * @param[out] pdBBeq
 *        double: Returned B/Beq value.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeDipLBBeq( const S3Coord& s3cIn,
                                        double* pdLdip,
                                        double* pdBBeq );

/*------------------------------------------------------------------*/
// function CMagfield::computeDipLBBeq
/**
 *
 * Compute L, B, and Beq values.
 *
 * @param s3cIn
 *        S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 * @param[out] dLdip
 *        double: Returned L value.
 * @param[out] dBBeq
 *        double: Returned B/Beq value.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeDipLBBeq( const S3Coord& s3cIn,
                                        double& dLdip,
                                        double& dBBeq );

/*------------------------------------------------------------------*/
// function CMagfield::computeDipLBBeq
/**
 *
 * Compute L, B, and Beq values.
 *
 * @param s3cIn
 *        S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 * @param[out] pdLdip
 *        double: Returned L value.
 * @param[out] pdBmag
 *        double: Returned Bmag value.
 * @param[out] pdBeq
 *        double: Returned Beq value.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeDipLBBeq( const S3Coord& s3cIn,
                                        double* pdLdip,
                                        double* pdBmag,
                                        double* pdBeq );

/*------------------------------------------------------------------*/
// function CMagfield::computeDipLBBeq
/**
 *
 * Compute L, B, and Beq values.
 *
 * @param s3cIn
 *        S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 * @param[out] dLdip
 *        double: Returned L value.
 * @param[out] dBmag
 *        double: Returned Bmag value.
 * @param[out] dBeq
 *        double: Returned Beq value.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeDipLBBeq( const S3Coord& s3cIn,
                                        double& dLdip,
                                        double& dBmag,
                                        double& dBeq );

/*------------------------------------------------------------------*/
// function CMagfield::computeDipBfield
/**
 *
 * Compute B field vector and magnitude.
 *
 * @param s3cIn
 *        S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 * @param[out] ps3B
 *        S3Tuple: Returned B vector.
 * @param[out] pdBmag
 *        double: Returned Bmag value.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeDipBfield( const S3Coord& s3cIn,
                                         S3Tuple* ps3B,
                                         double* pdBmag );

/*------------------------------------------------------------------*/
// function CMagfield::computeDipBfield
/**
 *
 * Compute B field vector and magnitude.
 *
 * @param s3cIn
 *        S3Coord: Position coordinates, geocentric Cartesian in km.
 *
 * @param[out] s3B
 *        S3Tuple: Returned B vector.
 * @param[out] dBmag
 *        double: Returned Bmag value.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeDipBfield( const S3Coord& s3cIn,
                                         S3Tuple& s3B,
                                         double& dBmag );

/*------------------------------------------------------------------*/
// function CMagfield::computeDipFieldLine
/**
 *
 * Compute B field line and L value.
 *
 * @param s3cIn
 *        S3Coord: Position coordinates, geocentric Cartesian in km.
 * @param efltIn
 *        eFieldLineTraceDir: Trace direction enumeration,
 *        * efltdNorth         = North
 *        * efltdSouth         = South
 *        * efltdNorthAndSouth = Both North and South
 *        * efltGsmEquator     = To geomagnetic equator
 * @param dTraceAlt
 *        double: Altitude.
 *
 * @param[out] ps3CvFLs
 *        S3CoordVec: Returned vector of field line coordinates, geocentric Cartesian in km.
 * @param[out] pdLdip
 *        double: Returned L value.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeDipFieldLine( const S3Coord& s3cIn,
                                            const eFieldLineTraceDir& efltIn,
                                            const double& dTraceAlt,
                                            S3CoordVec* ps3CvFLs,
                                            double* pdLdip );

/*------------------------------------------------------------------*/
// function CMagfield::computeDipFieldLine
/**
 *
 * Compute B field line and L value.
 *
 * @param s3cIn
 *        S3Coord: Position coordinates, geocentric Cartesian in km.
 * @param efltIn
 *        eFieldLineTraceDir: Trace direction enumeration,
 *        * efltdNorth         = North
 *        * efltdSouth         = South
 *        * efltdNorthAndSouth = Both North and South
 *        * efltGsmEquator     = To geomagnetic equator
 * @param dTraceAlt
 *        double: Altitude.
 *
 * @param[out] s3CvFLs
 *        S3CoordVec: Returned vector of field line coordinates, geocentric Cartesian in km.
 * @param[out] dLdip
 *        double: Returned L value.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeDipFieldLine( const S3Coord& s3cIn,
                                            const eFieldLineTraceDir& efltIn,
                                            const double& dTraceAlt,
                                            S3CoordVec& s3cvFl,
                                            double& dLdip );

/*------------------------------------------------------------------*/
// function CMagfield::computeDipFootPrint
/**
 *
 * Compute B field line footprint and L value.
 *
 * @param s3C
 *        S3Coord: Position coordinates, geocentric Cartesian in km.
 * @param efltIn
 *        eFieldLineTraceDir: Trace direction enumeration,
 *        * efltdNorth         = North
 *        * efltdSouth         = South
 *        * efltdNorthAndSouth = Both North and South
 * @param dTraceAlt
 *        double: Altitude.
 *
 * @param[out] ps3CvFPs
 *        S3CoordVec: Returned vector of footprint coordinates, geocentric Cartesian in km.
 * @param[out] pdLdip
 *        double: Returned L value.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeDipFootPrint( const S3Coord& s3C,
                                            const eFieldLineTraceDir& efltIn,
                                            const double& dTraceAlt,
                                            S3CoordVec* ps3cvFPs,
                                            double* pdLdip );

/*------------------------------------------------------------------*/
// function CMagfield::computeDipFootPrint
/**
 *
 * Compute B field line footprint and L value.
 *
 * @param s3C
 *        S3Coord: Position coordinates, geocentric Cartesian in km.
 * @param efltIn
 *        eFieldLineTraceDir: Trace direction enumeration,
 *        * efltdNorth         = North
 *        * efltdSouth         = South
 *        * efltdNorthAndSouth = Both North and South
 * @param dTraceAlt
 *        double: Altitude.
 *
 * @param[out] s3CvFPs
 *        S3CoordVec: Returned vector of footprint coordinates, geocentric Cartesian in km.
 * @param[out] dLdip
 *        double: Returned L value.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE computeDipFootPrint( const S3Coord& s3cIn,
                                            const eFieldLineTraceDir& efltIn,
                                            const double& dTraceAlt,
                                            S3CoordVec& s3cvFp,
                                            double& dLdip );

/*------------------------------------------------------------------*/
// function CMagfield::setKpValue
/**
 *
 * Set value of Kp index to be used in all subsequent calculations, as needed.<br>
 * Clears any previously defined list of time-referenced Kp values.
 *
 * @param dKpval
 *        double: Kp index value.  Valid range = 0.0 - 9.0.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE setKpValue( const double& dVKpal );

/*------------------------------------------------------------------*/
// function CMagfield::setKpValues
/**
 *
 * Set list values of Kp index, for every three hours from reference time. <br>
 * The "current" Kp value will be determined based on the current time.
 *
 * @param dTimeRef
 *        double: Time in MJD or composite ("GeoSpace") form.  Must be at 0000 GMT.
 * @param vdKpvals
 *        dvector: vector of Kp index values.  Valid range = 0.0 - 9.0.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE setKpValues( const double& dTimeRef,
                                    const dvector& vdVKpals );

/*------------------------------------------------------------------*/
// function CMagfield::setKpValues
/**
 *
 * Set list values of Kp index, for every three hours from reference time. <br>
 * The "current" Kp value will be determined based on the current time.
 *
 * @param tTimeRef
 *        CTimeValue: Time value object.  Must be at 0000 GMT.
 * @param vdKpvals
 *        dvector: vector of Kp index values.  Valid range = 0.0 - 9.0.
 *
 * @returns
 *        emfNoError or eMAGFIELD_ERROR_CODE enumerated error code
 */
  eMAGFIELD_ERROR_CODE setKpValues( const CTimeValue& tTimeRef,
                                    const dvector& vdVKpals );

/*------------------------------------------------------------------*/
// function CMagfield::getKpValue
/**
 *
 * Returns the current value of Kp index.  Either the defined constant value, or <br>
 *   current value from defined list for the current magfield time
 *
 * @returns
 *        double: Current value of Kp index
 */
  double getKpValue() { return csCMagField->getKpValue(); }

/*------------------------------------------------------------------*/
// function CMagfield::getKpValuesRefTime
/**
 *
 * Returns the reference time for the defined list of Kp values
 *
 * @returns
 *      double: Kp list reference time, MJD form (-1.0 if not defined)
 */
  double getKpValuesRefTime() { return cdKpMjdRef; }

/*------------------------------------------------------------------*/
// function CMagfield::getKpValuesEndTime
/**
 *
 * Returns the end time for the defined list of Kp values
 *
 * @returns
 *      double: Kp list end time, MJD form (-1.0 if not defined)
 */
  double getKpValuesEndTime();

/*------------------------------------------------------------------*/
// function CMagfield::getFltdEnum
/**
 *
 * Convert trace direction string to enumeration.
 *
 * @param strFltd
 *        string: trace direction (case insensitive),
 *        * "North"      | "N"   = Trace North
 *        * "South"      | "S"   = Trace South
 *        * "NorthSouth" | "NS"  = Trace North and South
 *        * "Equator"    | "E"   = Trace toward geomagnetic equator
 * @param[out] eFltd
 *        eFieldLineTraceDir: Trace direction enumeration,
 *        * efltdNorth          = North
 *        * efltdSouth          = South
 *        * efltdNorthAndSouth  = Both North and South
 *        * efltGsmEquator      = To geomagnetic equator
 *
 * @returns
 *        0: success, -1: unsupported
 */
  static const int getFltdEnum( const string& strFltd,
                                eFieldLineTraceDir& eFltd );

/*------------------------------------------------------------------*/
// function CMagfield::getShellModeEnum
/**
 *
 * Convert shell mode string to enumeration.
 *
 * @param strMode
 *        string: L shell mode (case insensitve),
 *        * "Dipole"   | "D" | "Ldip" =  Dipole mode
 *        * "McIlwain" | "M" | "Lm"   =  McIlwain mode
 *
 * @param[out] eMode
 *        eLshellMode: L shell mode enumeration,
 *        * elmDipole   = Dipole mode
 *        * elmMcIlwain = McIlwain mode
 *
 * @returns
 *        0: success, -1: unsupported
 */
  static const int getShellModeEnum( const string& strMode,
                                     eLshellMode& eMode );

/*------------------------------------------------------------------*/
// function CMagfield::getCoordSysEnum
/**
 *
 * Convert coordinate system and units string to enumeration.
 *
 * @param strCoordSys
 *        string: string representation of input coordinate system (case insensitive),
 *        * "SPH"           = Spherical
 *        * "GSM"           = Geocentric Solar Magnetospheric
 *        * "SM"  | "SSM"   = Solar Magnetic
 *        * "GEI" | "ECI"   = Geocentric Equatorial Inertial
 *        * "GEO" | "ECEF"  = Geographic Cartesian
 *        * "MLL"           = Magnetic Dipole
 *        * "MAG"           = Dipole Magnetic Cartesian
 *        * "MAGS"          = Dipole Magnetic Spherical
 *        * "GSE"           = Geocentric Solar Ecliptic
 *        * "GSEQ"          = Geocentric Solar Equatorial
 *        * "ODX"           = Offset Dipole Cartesian
 *        * "ODS"           = Offset Dipole Spherical
 *        * "GDZ" | "GEOD"  = WGS 84 Geodetic
 *        * "RLL"           = Spherical but with colat->lat
 * @param strCoordUnits
 *        string: string representation of input coordinate units (case insensitive)
 *        * "km"       = Kilometers
 *        * "re" | ""  = Earth radii
 * 
 * @param[out] coordSys
 *        emfCoordSys: Input coordinate system enumeration,
 *        * SPHinRE   = Radius (RE), Theta (colat) (deg), Phi (deg) (spherical)
 *        * GSMinRE   = x (RE), y (RE), z (RE) (geocentric solar magnetospheric)
 *        * SMinRE    = x (RE), y (RE), z (RE) (solar magnetic)
 *        * GEIinRE   = x (RE), y (RE), z (RE) (Geocentric Equatorial Inertial)
 *        * GEOinRE   = x (RE), y (RE), z (RE) (Geographic Cartesian)
 *        * MLLinRE   = Radius, (RE), MLat (deg), MLT (hr) (magnetic dipole)
 *        * MAGinRE   = x (RE), y (RE), z (RE) (dipole magnetic Cartesian)
 *        * MAGSinRE  = Radius (RE), Mlat (deg), Mlon (deg)
 *        * GSEinRE   = x (RE), y (RE), z (RE) (Geocentric Solar Ecliptic)
 *        * GSEQinRE  = x (RE), y (RE), z (RE) (geocentric solar equatorial)
 *        * ODXinRE   = mx (RE), my (RE), mz (RE) (offset-dipole Cartesian) 
 *        * ODSinRE   = Radius (RE), mlat (deg), mlon (deg) (offset-dipole Spherical)
 *        * ODXinKM   = mx (km), my (km), mz (km) (offset-dipole Cartesian) 
 *        * ODSinKM   = Radius (km), mlat (deg), mlon (deg) (offset-dipole Spherical)
 *        * SPHinKM   = Radius, (km), co-Latitude (deg), Phi (deg) (spherical)
 *        * GSMinKM   = x (km), y (km), z (km) (geocentric solar magnetospheric)
 *        * SMinKM    = x (km), y (km), z (km) (solar magnetic)
 *        * GEIinKM   = x (km), y (km), z (km) (Geocentric Equatorial Inertial)
 *        * GEOinKM   = x (km), y (km), z (km) (Geographic Cartesian)
 *        * MLLinKM   = Radius, (km), MLat (deg), MLT (hr) (magnetic dipole)
 *        * MAGinKM   = x (km), y (km), z (km) (dipole magnetic Cartesian)
 *        * MAGSinKM  = Radius (km), Mlat (deg), Mlon (deg)
 *        * GSEinKM   = x (km), y (km), z (km) (Geocentric Solar Ecliptic)
 *        * GSEQinKM  = x (km), y (km), z (km) (geocentric solar equatorial)
 *        * GEODinKM  = alt (km), lat (deg), lon (deg) (wgs84 geodetic)
 *        * GDZinKM   = alt (km), lat (deg), lon (deg) (wgs84 geodetic) (synonym for GEODinKM)
 *        * RLLinRE   = r (RE), lat (deg), lon (deg) (like SPHinRE but colat->lat)
 *        * RLLinKM   = r (km), lat (deg), lon (deg) (like SPHinKM but colat->lat)
 *
 * @returns
 *        0: success, -1: unsupported
 */
  static const int getCoordSysEnum( const std::string& strCoordSys,
                                    const std::string& strCoordUnits,
                                    emfCoordSys& eCoordSys );

/*------------------------------------------------------------------*/
// function CMagfield::getCoordSys
/**
 * 
 * Retrieve the string representation of the given coordinate system.
 *
 * @param coordSys
 *        emfCoordSys: Input coordinate system enumeration,
 *        * SPHinRE   = Radius (RE), Theta (colat) (deg), Phi (deg) (spherical)
 *        * GSMinRE   = x (RE), y (RE), z (RE) (geocentric solar magnetospheric)
 *        * SMinRE    = x (RE), y (RE), z (RE) (solar magnetic)
 *        * GEIinRE   = x (RE), y (RE), z (RE) (Geocentric Equatorial Inertial)
 *        * GEOinRE   = x (RE), y (RE), z (RE) (Geographic Cartesian)
 *        * MLLinRE   = Radius, (RE), MLat (deg), MLT (hr) (magnetic dipole)
 *        * MAGinRE   = x (RE), y (RE), z (RE) (dipole magnetic Cartesian)
 *        * MAGSinRE  = Radius (RE), Mlat (deg), Mlon (deg)
 *        * GSEinRE   = x (RE), y (RE), z (RE) (Geocentric Solar Ecliptic)
 *        * GSEQinRE  = x (RE), y (RE), z (RE) (geocentric solar equatorial)
 *        * ODXinRE   = mx (RE), my (RE), mz (RE) (offset-dipole Cartesian) 
 *        * ODSinRE   = Radius (RE), mlat (deg), mlon (deg) (offset-dipole Spherical)
 *        * ODXinKM   = mx (km), my (km), mz (km) (offset-dipole Cartesian) 
 *        * ODSinKM   = Radius (km), mlat (deg), mlon (deg) (offset-dipole Spherical)
 *        * SPHinKM   = Radius, (km), co-Latitude (deg), Phi (deg) (spherical)
 *        * GSMinKM   = x (km), y (km), z (km) (geocentric solar magnetospheric)
 *        * SMinKM    = x (km), y (km), z (km) (solar magnetic)
 *        * GEIinKM   = x (km), y (km), z (km) (Geocentric Equatorial Inertial)
 *        * GEOinKM   = x (km), y (km), z (km) (Geographic Cartesian)
 *        * MLLinKM   = Radius, (km), MLat (deg), MLT (hr) (magnetic dipole)
 *        * MAGinKM   = x (km), y (km), z (km) (dipole magnetic Cartesian)
 *        * MAGSinKM  = Radius (km), Mlat (deg), Mlon (deg)
 *        * GSEinKM   = x (km), y (km), z (km) (Geocentric Solar Ecliptic)
 *        * GSEQinKM  = x (km), y (km), z (km) (geocentric solar equatorial)
 *        * GEODinKM  = alt (km), lat (deg), lon (deg) (wgs84 geodetic)
 *        * GDZinKM   = alt (km), lat (deg), lon (deg) (wgs84 geodetic) (synonym for GEODinKM)
 *        * RLLinRE   = r (RE), lat (deg), lon (deg) (like SPHinRE but colat->lat)
 *        * RLLinKM   = r (km), lat (deg), lon (deg) (like SPHinKM but colat->lat)
 *
 * @returns
 *        String representation of enumerated coordinate system, e.g. "SPHinKM", 
 *          GEIinRE", etc.
 */
  static const string getCoordSys(emfCoordSys coordSys);

/*------------------------------------------------------------------*/
private:
 
  // Class (State) Variables
  bool              cbIsInitialized;
  eMainField        ceMainField;
  eExternalField    ceExternalField;
  string            csDBfp;
  // time handling variables
  double  cdIgrfUpdateRate;
  // coordinate transforms; also controls O/P and external field updates
  double  cdSr2UpdateRate;
  double  cdCurIgrfTime;
  double  cdCurSr2Time;
  double  cdMagTime;
  int     ciOverRideYear;
  // Dipole parameters
  double cdDimo, cdTheta, cdPhi, cdEta, cdZeta, cdXi;
  double cdDegToRad, cdRE;
  double cdPolarRE, cdEquatRE;
  dvector cdvGeoToMag;
  double cdKpMjdRef;
  dvector cvdKpVals;

  CMagFieldIGRF* csIGRF;     // IGRF data pointer
  CMagFieldLib* csCMagField; // magfield object

  // used to get strings from coordSys types
  static const char* cstraCoordSysNames[];
  void loadKpValue();

};

#endif
