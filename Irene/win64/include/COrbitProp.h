/***********************************************************************

Module: COrbitProp.h

Description: Class definition for COrbitProp object

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

   VERSION            DATE       NOTES
     1.0       Fri Oct 3, 2008   First release

------------------------------------------------------------------------

***********************************************************************/
#include "GenericModel.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include "CTimeValue.h"

#ifndef CORBIT_PROP_MODEL_DEFINE
#define CORBIT_PROP_MODEL_DEFINE


struct STwoLineElements {
// 1st line
   string sLine1;
   long lSatelliteNumber;
   string sClass;
   int iInternationalDesignatorYear; // 2 digit year
   int iLaunchNumber;
   string sPiece;
   int iEpochYear;
   double dEpochDayofYear;
   double dMeanMotion1stDerivative;
   double dMeanMotion2ndDerivative;
   double dBstar;
   int iEphemerisType;
   int iElementNumber;
   int iCheckSumLine1;

// 2nd line
   string sLine2;
   double dInclination;
   double dRightAscension;
   double dEccentricity;
   double dArgofPerigee;
   double dMeanAnomaly;
   double dMeanMotion;
   long lEpochRevolution;
   int iCheckSumLine2;

   STwoLineElements():sLine1(""),lSatelliteNumber(0),sClass(""),
      iInternationalDesignatorYear(0),iLaunchNumber(0),sPiece(""),
      iEpochYear(0),dEpochDayofYear(0.0),dMeanMotion1stDerivative(0.0),
      dMeanMotion2ndDerivative(0.0),dBstar(0.0),iEphemerisType(0),
      iElementNumber(0),iCheckSumLine1(0),
      sLine2(""),dInclination(0.0),dRightAscension(0.0),dEccentricity(0.),
      dArgofPerigee(0.0),dMeanAnomaly(0.0),dMeanMotion(0.0),lEpochRevolution(0),
      iCheckSumLine2(0){};

};

typedef std::vector<STwoLineElements> STwoLineElementsVec;


struct SMeanElements {
   double dInclination;
   double dRightAscension;
   double dEccentricity;
   double dArgofPerigee;
   double dMeanAnomaly;
   double dMeanMotion;
   double dMeanMotion1stDerivative;
   double dMeanMotion2ndDerivative;
   double dBstar;
// Time (YYYYDDDSSSSS.S)
   double dRefTime;

   SMeanElements():dInclination(0.0),dRightAscension(0.0),dEccentricity(0.),
      dArgofPerigee(0.0),dMeanAnomaly(0.0),dMeanMotion(0.0),
      dMeanMotion1stDerivative(0.0),dMeanMotion2ndDerivative(0.0),dBstar(0.0),
      dRefTime(0.0){};
};

typedef std::vector<SMeanElements> SMeanElementsVec;


struct SEphemeris {
// Distances in km, Velocities in km/s
   double dGeoX;
   double dGeoY;
   double dGeoZ;
   double dGeoXdot;
   double dGeoYdot;
   double dGeoZdot;
// Time (YYYYDDDSSSSS.S)
   double dTime;

   SEphemeris():dGeoX(0.0),dGeoY(0.0),dGeoZ(0.0),dGeoXdot(0.0),
      dGeoYdot(0.0),dGeoZdot(0.0),dTime(0.0){};

};

typedef std::vector<SEphemeris> SEphemerisVec;


//   ***Deprecated, but retained for backward compatibility***
struct SEphemTimeCompSpecs {
// All times formatted as YYYYDDDSSSSS.S
   double dStart;
   double dStop;
   double dStep;

   SEphemTimeCompSpecs():dStart(0.0),dStop(0.0),dStep(0.0){};
};

//
// Class: COrbitProp
//

/**
 * @brief Orbit Propagated ephemeris generation class
 *
 * The "OrbitProp" class provides methods for generating satellite
 * ephemeris information for orbits defined defined by two-line elements,
 * various sets of keplerian elements or state vectors, using either
 * the SGP4 or 'SatEph' propagator models.
 */

class COrbitProp:public GenericModel{

public:

/*------------------------------------------------------------------*/
// function COrbitProp::OrbitProp
/**
*
* Default constructor
*
*/
   COrbitProp();

/*------------------------------------------------------------------*/
// function COrbitProp::OrbitProp
/**
*
* Constructor that reads in a TLE file
*
*/
   COrbitProp(const string& sTleFilename);

/*------------------------------------------------------------------*/
// function COrbitProp::~OrbitProp
/**
*
* Destructor
*
*/
   virtual ~COrbitProp();

/*------------------------------------------------------------------*/
// function COrbitProp::readTwoLineElements
/**
*
*  Reads in two line element sets from a file
*
*  @param sFilename
*      string filename containing the TLE
*
*  @return int
*      0: success, else fail (see GenericModelEnum.h)
*/
   eGENERIC_ERROR_CODE readTwoLineElements(const string& sFilename);

/*------------------------------------------------------------------*/
// function COrbitProp::readTwoLineElements
/**
*
*  Reads in two line element sets from a file
*  ***Deprecated, but retained for backward compatibility***
*/
   eGENERIC_ERROR_CODE readTwoLineElements(const string& sFilename,
      vector<STwoLineElements>* svTLE);

//  sets and gets

/*------------------------------------------------------------------*/
// function COrbitProp::setTwoLineElements
/**
*
*  Sets the class variable cSvTwoLineElements to the value of SvTLE
*  ***Deprecated, but retained for backward compatibility***
*/
   eGENERIC_ERROR_CODE setTwoLineElements(const vector<STwoLineElements>& SvTLE);

/*------------------------------------------------------------------*/
// function COrbitProp::getTwoLineElements
/**
*
*  Returns the value of the class variable cSvTwoLineElements to the pointer psvTle
*  [Need for this method is limited to very special types of calculations]
*/
   eGENERIC_ERROR_CODE getTwoLineElements(STwoLineElementsVec* psvTLE);

   // methods for accessing TLE strings currently loaded in TLE Deck

/*------------------------------------------------------------------*/
//function COrbitProp::getTLEStrings
/**
 *
 * get the full set of defined TLE strings
 *
 *  @param[out] pvTleLines
 *      vector of pair: vector of pair<string,string> container containing
 *                       the set of defined TLE strings
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
   eGENERIC_ERROR_CODE getTLEstrings ( vector< pair<string,string> >& pvTleLines );

/*------------------------------------------------------------------*/
//function COrbitProp::getTLEStrings
/**
 *
 * get the full set of defined TLE strings
 *
 *  @param[out] strvTleLine1
 *      string vector: vector of strings containing the first line of the TLE entries
 *  @param[out] strvTleLine2
 *      string vector: vector of strings containing the second line of the TLE entries
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
   eGENERIC_ERROR_CODE getTLEstrings( svector& strvTleLine1,
                                      svector& strvTleLine2 );

/*------------------------------------------------------------------*/
//function COrbitProp::getTLEPair
/**
 *
 * get a specified TLE pair
 *
 *  @param iIndex
 *      int: index of TLE entries to retrieve
 *
 *  @param[out] TLEpair
 *      string vector: vector of strings containing the requested two lines
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
   eGENERIC_ERROR_CODE getTLEpair( int index, svector& TLEpair);

/*------------------------------------------------------------------*/
//function COrbitProp::getNumTLEs
/**
 *
 * get the number of specified TLE pairs
 *
 *  @return int
 *      number of currently specified TLE pairs in the model
 */
   int getNumTLEs () { return (int)cSvTwoLineElements.size(); }

/*------------------------------------------------------------------*/
// function COrbitProp::setMeanElements
/**
*
*  Sets the class variable cSvMeanElements to the value of SvMeanEle
*  ***Deprecated, but retained for backward compatibility***
*
*/
   eGENERIC_ERROR_CODE setMeanElements(const vector<SMeanElements>& SvMeanEle);

/*------------------------------------------------------------------*/
// function COrbitProp::getMeanElements
/**
*
*  Returns the value of the class variable cSvMeanElements to the pointer
*  psvME
*  ***Deprecated, but retained for backward compatibility***
*
*/
   eGENERIC_ERROR_CODE getMeanElements(SMeanElementsVec* psvME);


// useful methods to be used for loading TLE lines directly onto TLE deck
//
/*------------------------------------------------------------------*/
// function COrbitProp::reset
/**
 *
 * clear the loaded MJD ephemeris times and TLE deck
 *
 */
   void reset() { cvdMjdTimes.clear(); cSvTwoLineElements.clear(); }

// function clearTwoLineElements
/**
 *
 * clear the loaded TLE deck
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
   eGENERIC_ERROR_CODE clearTwoLineElements();

/*------------------------------------------------------------------*/
// function COrbitProp::loadTwoLineElements
/**
 *
 * load TLE deck with a two separate TLE strings
 *
 *  @param sTleLine1
 *      string: formatted first line of TLE information
 *  @param sTleLine2
 *      string: formatted second line of TLE information
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
   eGENERIC_ERROR_CODE loadTwoLineElements( const string& sTleLine1,
                                            const string& sTleLine2 );

/*------------------------------------------------------------------*/
// function COrbitProp::loadTwoLineElements
/**
 *
 * load TLE deck with a 'pair' container of TLE strings
 *
 *  @param pTleLines
 *      pair<string,string>: first and second lines of TLE information
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
   eGENERIC_ERROR_CODE loadTwoLineElements(const pair<string,string> pTleLines );

/*------------------------------------------------------------------*/
// function COrbitProp::checkElementSets
/**
 *
 * verify current loaded TLEs are same satellite, and in correct time order
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
   eGENERIC_ERROR_CODE checkTwoLineElements();

   // methods for loading of specific orbital element values (no structures)
   //    (these are overloaded for different time specifications)

   // methods to permit direct loading of 'mean' element values
/*------------------------------------------------------------------*/
// function COrbitProp::clearMeanElements
/**
 *
 * clear the loaded Mean Elements
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
   eGENERIC_ERROR_CODE clearMeanElements() { return ( clearTwoLineElements() );}

/*------------------------------------------------------------------*/
// function COrbitProp::loadMeanElements
/**
 *
 * load a set of mean elements
 *
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
   eGENERIC_ERROR_CODE loadMeanElements (
     const double& dRefTime,
     const double& dInclination,
     const double& dRightAscension,
     const double& dEccentricity,
     const double& dArgofPerigee,
     const double& dMeanAnomaly,
     const double& dMeanMotion,
     const double& dMeanMotion1stDerivative = 0.0,
     const double& dMeanMotion2ndDerivative = 0.0,
     const double& dBstar = 0.0 );

/*------------------------------------------------------------------*/
// function COrbitProp::loadMeanElements
/**
 *
 * load a set of mean elements, CTimeValue time reference
 *
 *  @param tRefTime
 *      CTimeValue: element reference time, using CTimeValue object
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
   eGENERIC_ERROR_CODE loadMeanElements (
     const CTimeValue& tRefTime,
     const double& dInclination,
     const double& dRightAscension,
     const double& dEccentricity,
     const double& dArgofPerigee,
     const double& dMeanAnomaly,
     const double& dMeanMotion,
     const double& dMeanMotion1stDerivative = 0.0,
     const double& dMeanMotion2ndDerivative = 0.0,
     const double& dBstar = 0.0 );

   // methods to permit direct loading of 'solar' element values
/*------------------------------------------------------------------*/
// function COrbitProp::clearSolarElements
/**
 *
 * clear the loaded Solar Elements
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
   eGENERIC_ERROR_CODE clearSolarElements() { return ( clearTwoLineElements() );}

/*------------------------------------------------------------------*/
// function COrbitProp::loadSolarElements
/**
 *
 * load a set of solar elements
 *
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
 *      double: orbit's mean anomaly value associated with reference time [degrees] [default=180]
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
   eGENERIC_ERROR_CODE loadSolarElements (
     const double& dRefTime,
     const double& dInclination,
     const double& dApogee,
     const double& dPerigee,
     const double& dLocalTimeofApogee,
     const double& dLocalTimeofMaxIncl,
     const double& dMeanAnomaly = 180.0 ); // backward compat

/*------------------------------------------------------------------*/
// function COrbitProp::loadSolarElements
/**
 *
 * load a set of solar elements, with CTimeValue reference time
 *
 *  @param tRefTime
 *      double: element reference time, using CTimeValue object
 *  @param dInclination
 *      double: orbital inclination [degrees]
 *  @param dApogee
 *      double: orbit's maximum height [km], above Earth's 6371.2km radius
 *  @param dPerigee
 *      double: orbit's minimum height [km], above Earth's 6371.2km radius
 *  @param dLocalTimeofApogee
 *      double: local time [hours.frac] of orbit's apogee, for this reference time
 *  @param dLocalTimeofMaxIncl
 *      double: local time [hours.frac] of orbit's maximum latitude point, fort this reference time
 *  @param dMeanAnomaly
 *      double: orbit's mean anomaly value associated with reference time [degrees] [default=180]
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
   eGENERIC_ERROR_CODE loadSolarElements (
     const CTimeValue& tRefTime,
     const double& dInclination,
     const double& dApogee,
     const double& dPerigee,
     const double& dLocalTimeofApogee,
     const double& dLocalTimeofMaxIncl,
     const double& dMeanAnomaly = 180.0 ); // backward compat

   // methods to permit direct loading of 'simple' element values
/*------------------------------------------------------------------*/
// function COrbitProp::clearSimpleElements
/**
 *
 * clear the loaded Simple Elements
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
   eGENERIC_ERROR_CODE clearSimpleElements() { return ( clearTwoLineElements() );}

/*------------------------------------------------------------------*/
// function COrbitProp::loadSimpleElements
/**
 *
 * load a set of simple elements
 *
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
 *      double: orbit's mean anomaly value associated with reference time [degrees] [default=180]
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
   eGENERIC_ERROR_CODE loadSimpleElements (
     const double& dRefTime,
     const double& dInclination,
     const double& dApogee,
     const double& dPerigee,
     const double& dRightAscension,
     const double& dArgofPerigee,
     const double& dMeanAnomaly = 180.0 ); // backward compat

//*------------------------------------------------------------------*/
// function COrbitProp::loadSimpleElements
/**
 *
 * load a set of simple elements, using CTimeValue time reference
 *
 *  @param tRefTime
 *      CTimeValue: element reference time, using CTimeValue object
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
 *      double: orbit's mean anomaly value associated with reference time [degrees] [default=180]
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
   eGENERIC_ERROR_CODE loadSimpleElements (
     const CTimeValue& tTime,
     const double& dInclination,
     const double& dApogee,
     const double& dPerigee,
     const double& dRightAscension,
     const double& dArgofPerigee,
     const double& dMeanAnomaly = 180.0 ); // backward compat

   // methods to permit direct loading of 'classical' element values
/*------------------------------------------------------------------*/
// function COrbitProp::clearClassicalElements
/**
 *
 * clear the loaded Classical Elements
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
   eGENERIC_ERROR_CODE clearClassicalElements() { return ( clearTwoLineElements() );}

/*------------------------------------------------------------------*/
// function COrbitProp::loadClassicalElements
/**
 *
 * load a set of classical elements
 *
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
 *      double: orbit's mean anomaly value associated with reference time [degrees] [default=180]
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
   eGENERIC_ERROR_CODE loadClassicalElements (
     const double& dRefTime,
     const double& dInclination,
     const double& dSemiMajorAxis,// units of km preferred; will autoconvert Re
     const double& dEccentricity,
     const double& dRightAscension,
     const double& dArgofPerigee,
     const double& dMeanAnomaly = 180.0 ); // backward compat

/*------------------------------------------------------------------*/
// function COrbitProp::loadClassicalElements
/**
 *
 * load a set of classical elements, using CTimeValue reference
 *
 *  @param tRefTime
 *      CTimeValue: element reference time, using CTimeValue object
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
 *      double: orbit's mean anomaly value associated with reference time [degrees] [default=180]
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
   eGENERIC_ERROR_CODE loadClassicalElements (
     const CTimeValue& tRefTime,
     const double& dInclination,
     const double& dSemiMajorAxis,
     const double& dEccentricity,
     const double& dRightAscension,
     const double& dArgofPerigee,
     const double& dMeanAnomaly = 180.0 ); // backward compat

/*------------------------------------------------------------------*/
// function COrbitProp::clearGeoElements
/**
 *
 * clear the loaded Geo Elements
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
  eGENERIC_ERROR_CODE clearGeoElements() { return ( clearTwoLineElements() );}

/*------------------------------------------------------------------*/
// function COrbitProp::loadGeoElements
/**
 *
 * load a set of Geo elements
 *
 *  @param dRefTime
 *      double: element reference time, in MJD or Composite form
 *  @param dLongitude
 *      double: orbital Geosynchronous position, in East Longitude degrees
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
eGENERIC_ERROR_CODE loadGeoElements( const double& dRefTime,
                                     const double& dLongitude );

/*------------------------------------------------------------------*/
// function COrbitProp::loadGeoElements
/**
 *
 * load a set of Geo elements, using CTimeValue reference
 *
 *  @param tRefTime
 *      CTimeValue: element reference time, using CTimeValue object
 *  @param dLongitude
 *      double: orbital Geosynchronous position, in East Longitude degrees
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
    eGENERIC_ERROR_CODE loadGeoElements( const CTimeValue& tRefTime,
                                         const double& dLongitude );

// methods implemented with COrbitPropSateph class only
  eGENERIC_ERROR_CODE clearStateVectors() { return ( clearTwoLineElements() );}
  virtual eGENERIC_ERROR_CODE loadStateVectors ( const double& dTime,
    const S3Tuple& s3Pos, const S3Tuple& s3Vel,
    const double& dPDecay=0.0 ) { return eNotImplemented; }
  virtual eGENERIC_ERROR_CODE loadStateVectors ( const CTimeValue& tTime,
    const S3Tuple& s3Pos, const S3Tuple& s3Vel,
    const double& dPDecay=0.0 ) { return eNotImplemented; }
  virtual eGENERIC_ERROR_CODE convertStateVectorsToTLEstrings(const double& dTime,
    const S3Tuple& s3Pos, const S3Tuple& s3Vel, string& sTleOne, string& sTleTwo,
    const double& dPDecay=0.0 ) { return eNotImplemented; }
  virtual eGENERIC_ERROR_CODE convertStateVectorsToTLEstrings(const CTimeValue& tTime,
    const S3Tuple& s3Pos, const S3Tuple& s3Vel, string& sTleOne, string& sTleTwo,
    const double& dPDecay=0.0 ) { return eNotImplemented; }

/*------------------------------------------------------------------*/
// function COrbitProp::setEphemTimeCompSpecs
/**
*
*  Set the class variable cSEphemTimeCompSpecs to the value of SEphemTimeCompSpecs
*  ***Deprecated, but retained for backward compatibility***
*/
   eGENERIC_ERROR_CODE setEphemTimeCompSpecs(const SEphemTimeCompSpecs& SEphemTimeCompSpecs);

/*------------------------------------------------------------------*/
// function COrbitProp::getEphemTimeCompSpecs
/**
*
*  Returns the value of the class variable cSEphemTimeCompSpecs to the pointer
*  pSEphemTimeCompSpecs
*  ***Deprecated, but retained for backward compatibility***
*/
   eGENERIC_ERROR_CODE getEphemTimeCompSpecs(SEphemTimeCompSpecs* pSEphemTimeCompSpecs);

/*------------------------------------------------------------------*/
// function COrbitProp::setTimes
/**
 *
 *  Specify a collection of time values for the ephemeris generation
 *
 *  @param vdTimes
 *      double vector: vector of time values, in MJD or Composite form
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
   eGENERIC_ERROR_CODE setTimes( const dvector& vdTimes );

/*------------------------------------------------------------------*/
// function COrbitProp::setGeoTimes
/**
 *
 *  Specify a collection of time values for the ephemeris generation
 *
 *  @param dvgeotimes
 *      double vector: vector of time values, in MJD or Composite form
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
   eGENERIC_ERROR_CODE setGeoTimes( const dvector& dvgeotimes );

/*------------------------------------------------------------------*/
//function COrbitProp::setTimes
/**
 *
 * Specify the time range (via CTimeValue) and timestep increment for the ephemeris generation
 *
 *  @param tStartTime
 *      CTimeValue: starting time for ephemeris, in CTimeValue object
 *  @param tEndTime
 *      CTimeValue: ending time for ephemeris, in CTimeValue object
 *  @param dTimeStep
 *      double: number of seconds between ephemeris time entries
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
   eGENERIC_ERROR_CODE setTimes( const CTimeValue& tStartTime,
                                 const CTimeValue& tStopTime,
                                 const double& dStepTime );

/*------------------------------------------------------------------*/
//function COrbitProp::setTimes
/**
 *
 * Specify the time range and timestep increment for the ephemeris generation
 *
 *  @param dStartTime
 *      double: starting time for ephemeris, in Composite or MJD form
 *  @param dEndTime
 *      double: ending time for ephemeris, in Composite or MJD form
 *  @param dTimeStep
 *      double: number of seconds between ephemeris time entries
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
   eGENERIC_ERROR_CODE setTimes( const double& dStartTime,
                                 const double& dStopTime,
                                 const double& dStepTime );

/*------------------------------------------------------------------*/
// function COrbitProp::setGeoTimes
/**
 *
 *  @param dStartTime
 *      double: starting time for ephemeris, in Composite or MJD form
 *  @param dEndTime
 *      double: ending time for ephemeris, in Composite or MJD form
 *  @param dTimeStep
 *      double: number of seconds between ephemeris time entries
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
   eGENERIC_ERROR_CODE setGeoTimes( const double& dStartTime,
                                    const double& dStopTime,
                                    const double& dStepTime );

/*------------------------------------------------------------------*/
// function COrbitProp::getGeoTimes
/**
*
*  Returns the vector of currently defined times
*
*  @param[out] pdvgeotimes
*      double vector pointer: vector of currently defined times, in Composite form
*
*  @return int
*      0: success, else fail (see GenericModelEnum.h)
*/
   eGENERIC_ERROR_CODE getGeoTimes( dvector* pdvgeotimes );

//  -- times are returned in MJD format

/*------------------------------------------------------------------*/
// function COrbitProp::getTimes
/**
*
*  Returns the vector of currently defined times
*
*  @param[out] pdvgeotimes
*      double vector pointer: vector of currently defined times, in MJD form
*
*  @return int
*      0: success, else fail (see GenericModelEnum.h)
*/
   eGENERIC_ERROR_CODE getTimes( dvector* pvdMjdTimes );

/*------------------------------------------------------------------*/
// function COrbitProp::getTimes
/**
*
*  Returns the vector of currently defined times
*
*  @param[out] pdvgeotimes
*      double vector: vector of currently defined times, in MJD form
*
*  @return int
*      0: success, else fail (see GenericModelEnum.h)
*/
   eGENERIC_ERROR_CODE getTimes( dvector& vdMjdTimes );

/*------------------------------------------------------------------*/
// function COrbitProp::getNumTimes
/**
*
*  Returns the number of currently defined times
*
*  @return int
*      >=0: success, number of defined times, else fail (see GenericModelEnum.h)
*/
   int getNumTimes() { return (int)cvdMjdTimes.size(); }

//-------------------------------------------------------------------
// function COrbitProp::computeEphem
/**
 *
 * Compute the ephemeris for the current elements, times and propagator
 *
 *  @param[out] vdMjdTimes
 *      double vector: vector of ephemeris times, in MJD form
 *  @param[out] vvdEciPos
 *      double 2D vector: vector of GEI XYZ coordinate positions [km]
 *  @param[out] vvdEciVel
 *      double 2D vector: vector of GEI XYZ coordinate velocities [km/sec]
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
   eGENERIC_ERROR_CODE computeEphem( dvector& vdMjdTimes,
                                     vdvector& vvdEciPos,
                                     vdvector& vvdEciVel );

//-------------------------------------------------------------------
// function COrbitProp::computeEphem
/**
 *
 * Compute the ephemeris for the current elements, times and propagator
 *
 *  @param[out] vdMjdTimes
 *      double vector: vector of ephemeris times, in MJD form
 *  @param[out] vdEciPosX
 *      double vector: vector of GEI X coordinate positions [km]
 *  @param[out] vdEciPosY
 *      double vector: vector of GEI Y coordinate positions [km]
 *  @param[out] vdEciPosZ
 *      double vector: vector of GEI Z coordinate positions [km]
 *  @param[out] vdEciVelX
 *      double vector: vector of GEI X coordinate velocities [km/sec]
 *  @param[out] vdEciVelY
 *      double vector: vector of GEI Y coordinate velocities [km/sec]
 *  @param[out] vdEciVelZ
 *      double vector: vector of GEI Z coordinate velocities [km/sec]
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
   eGENERIC_ERROR_CODE computeEphem( dvector& vdMjdTimes,
                                     dvector& vdEciPosX,
                                     dvector& vdEciPosY,
                                     dvector& vdEciPosZ,
                                     dvector& vdEciVelX,
                                     dvector& vdEciVelY,
                                     dvector& vdEciVelZ );

// overloaded computeEphem's
//   [methods with arguments of time or orbital element structure(s) are DEPRECATED]
/*------------------------------------------------------------------*/
// function COrbitProp::computeEphem
/**
*
*  Computes the ephemeris given a vector of two-line element sets, a structure of
*  type SEphemTimeCompSpecs containing the Start, Stop, and Step times specifying
*  the orbit, and returns the ephemeris via the pointer pSEphemeris
*  ***Deprecated, but retained for backward compatibility***
*/
   eGENERIC_ERROR_CODE computeEphem(const vector<STwoLineElements>& SvTLE,
                                    const SEphemTimeCompSpecs& SEphemTimeCompSpecs,
                                    SEphemerisVec* pSEphemeris);

/*------------------------------------------------------------------*/
// function COrbitProp::computeEphem
/**
*
*  Computes the ephemeris given a file containing two-line element sets, a structure of
*  type SEphemTimeCompSpecs containing the Start, Stop, and Step times specifying
*  the orbit, and returns the ephemeris via the pointer pSEphemeris
*  ***Deprecated, but retained for backward compatibility***
*/
   eGENERIC_ERROR_CODE computeEphem(const string& sTleFilename,
                                    const SEphemTimeCompSpecs& SEphemTimeCompSpecs,
                                    SEphemerisVec* pSEphemeris);


/*------------------------------------------------------------------*/
// function COrbitProp::computeEphem
/**
*
*  Computes the ephemeris given a file containing two-line element sets, the Start,
*  Stop, and Step times specifying the orbit, and returns the ephemeris via
*  the pointer pSEphemeris
* ***Deprecated, but retained for backward compatibility***
*/
   eGENERIC_ERROR_CODE computeEphem(const string& sTleFilename,
                                    const double& dStartTime,
				                            const double& dStopTime,
				                            const double& dStepTime,
                                    SEphemerisVec* pSEphemeris);

/*------------------------------------------------------------------*/
// function COrbitProp::computeEphem
/**
*
*  Computes the ephemeris for previously loaded two-line element sets, the Start,
*  Stop, and Step times specifying the orbit, and returns the ephemeris via
*  the pointer pSEphemeris
* ***Deprecated, but retained for backward compatibility***
*/
   eGENERIC_ERROR_CODE computeEphem(const double& dStartTime,
				                            const double& dStopTime,
				                            const double& dStepTime,
                                    SEphemerisVec* pSEphemeris);

/*------------------------------------------------------------------*/
// function COrbitProp::computeEphem
/**
*
*  Computes the ephemeris given a file containing two-line element sets, and array
*  of times for which the Ephemeris is to be computed, and returns the ephemeris via
*  the pointer pSEphemeris
* ***Deprecated, but retained for backward compatibility***
*/
   eGENERIC_ERROR_CODE computeEphem(const string& sTleFilename,
                                    const dvector& dvGeotimes,
                                    SEphemerisVec* pSEphemeris);

/*------------------------------------------------------------------*/
// function COrbitProp::computeEphem
/**
*
*  Computes the ephemeris and returns the it via
*  the pointer pSEphemeris.  This method assumes that the two-line elements
*  sets and the parameters specifying the orbit have already been set
* ***Deprecated, but retained for backward compatibility***
*/
   eGENERIC_ERROR_CODE computeEphem(SEphemerisVec* pSEphemeris);

/*------------------------------------------------------------------*/
// function COrbitProp::computeEphem
/**
*
*  Computes the ephemeris given a vector of Mean element structures, a structure of
*  type SEphemTimeCompSpecs containing the Start, Stop, and Step times specifying
*  the orbit, and returns the ephemeris via the pointer pSEphemeris
*  ***Deprecated, but retained for backward compatibility***
*/
   eGENERIC_ERROR_CODE computeEphem(const vector<SMeanElements>& SvME,
                                    const SEphemTimeCompSpecs& SEphemTimeCompSpecs,
                                    SEphemerisVec* pSEphemeris);

/*------------------------------------------------------------------*/
// function COrbitProp::computeEphem
/**
*
*  Computes the ephemeris given a vector containing mean element structures, the Start,
*  Stop, and Step times specifying the orbit, and returns the ephemeris via
*  the pointer pSEphemeris
*  ***Deprecated, but retained for backward compatibility***
*/
   eGENERIC_ERROR_CODE computeEphem(const vector<SMeanElements>& SvME,
                                    const double& dStartTime,
				                            const double& dStopTime,
				                            const double& dStepTime,
                                    SEphemerisVec* pSEphemeris);

/*------------------------------------------------------------------*/
// function COrbitProp::computeEphem
/**
*
*  Computes the ephemeris given a vector containing mean element structures, and array
*  of times for which the Ephemeris is to be computed, and returns the ephemeris via
*  the pointer pSEphemeris
*  ***Deprecated, but retained for backward compatibility***
*/
   eGENERIC_ERROR_CODE computeEphem(const vector<SMeanElements>& SvME,
                                    const dvector& dvGeotimes,
                                    SEphemerisVec* pSEphemeris);

// utility functions (no class variables accessed)
/*------------------------------------------------------------------*/
// function COrbitProp::getEarthRotAngle
/**
 *
 * Get the Earth's rotation angle [degrees] for the specified MJD time value
 *
 *  @param dModJulDate
 *      double: time in MJD form
 *
 *  @param[out] dEarthRotAngle
 *      double: returned Earth Rotation Angle
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
   eGENERIC_ERROR_CODE getEarthRotAngle( const double& dTime,
                                         double& dEarthRotAngle );

/*------------------------------------------------------------------*/
// function COrbitProp::getEarthRotAngle
/**
 *
 * Get the Earth's rotation angle [degrees] for the specified MJD time value
 *
 *  @param dModJulDate
 *      double: time in MJD form
 *
 *  @param[out] dEarthRotAngle
 *      double pointer: returned Earth Rotation Angle
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
   eGENERIC_ERROR_CODE getEarthRotAngle( const double& dTime,
                                         double* pdEarthRotAngle );

/*------------------------------------------------------------------*/
// function COrbitProp::getEarthRotAngle
/**
 *
 * Get the Earth's rotation angle [degrees], using a CTimeValue time reference
 *
 *  @param tTime
 *      CTimeValue: time specification using a CTimeValue object
 *
 *  @param[out] dEarthRotAngle
 *      double: returned Earth Rotation Angle
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
   eGENERIC_ERROR_CODE getEarthRotAngle (const CTimeValue& tTime,
                                         double& dEarthRotAngle);

/*------------------------------------------------------------------*/
// function COrbitProp::convertMeanToTLEstrings
/**
 *
 * Convert a set of mean elements to their TLE equivalent strings
 *
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
 *  @param[out] sTleOne
 *      string: returned first line of TLE information
 *  @param[out] sTleTwo
 *      string: returned second line of TLE information
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
   eGENERIC_ERROR_CODE convertMeanToTLEstrings(
     const double& dRefTime,
     const double& dInclination,
     const double& dRightAscension,
     const double& dEccentricity,
     const double& dArgofPerigee,
     const double& dMeanAnomaly,
     const double& dMeanMotion,
     const double& dMeanMotion1stDerivative,
     const double& dMeanMotion2ndDerivative,
     const double& dBstar,
     string& sTleOne,
     string& sTleTwo );

/*------------------------------------------------------------------*/
// function COrbitProp::convertMeanToTLEstrings
/**
 *
 * Convert a set of mean elements to their TLE equivalent strings, using a CTimeValue reference
 *
 *  @param tTime
 *      CTimeValue: element reference time, as a CTimeValue object
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
 *  @param[out] sTleOne
 *      string: returned first line of TLE information
 *  @param[out] sTleTwo
 *      string: returned second line of TLE information
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
   eGENERIC_ERROR_CODE convertMeanToTLEstrings (
     const CTimeValue& tTime,
     const double& dInclination,
     const double& dRightAscension,
     const double& dEccentricity,
     const double& dArgofPerigee,
     const double& dMeanAnomaly,
     const double& dMeanMotion,
     const double& dMeanMotion1stDerivative,
     const double& dMeanMotion2ndDerivative,
     const double& dBstar,
     string& sTleOne,
     string& sTleTwo );

   // should be 'protected', but called by unit test...
   //  --deprecated--
   eGENERIC_ERROR_CODE getEpochTime(const STwoLineElements& STle, double* epochtime);
   eGENERIC_ERROR_CODE getEpochMjdTime(const STwoLineElements& STle, double& dEpochMjdTime);

// virtual functions

/*------------------------------------------------------------------*/
// function COrbitProp::computeModelEphem()
/**
*
*  Computes the ephemeris using the methods of a subclass of COrbitProp
*/
   virtual eGENERIC_ERROR_CODE computeModelEphem()=0;

protected:

   CTimeValue ctStartTime;
   CTimeValue ctStopTime;
   double cdTimeStep;

   STwoLineElementsVec cSvTwoLineElements;

   dvector cvdMjdTimes;
   vdvector cvvdEphemPos;
   vdvector cvvdEphemVel;

   int ciMax21stCenturyYear;

   SMeanElementsVec cSvMeanElements;

// internal methods

//  Set the class variable cvdMjdTimes to values computed from start/stop/step specifications
   eGENERIC_ERROR_CODE setGeoTimes();

   //  load TLE via class-defined structure
   eGENERIC_ERROR_CODE loadTwoLineElements( STwoLineElements& sTLE );
   //  inserts new TLE into TLE stack, in proper chronological order
   eGENERIC_ERROR_CODE insertTwoLineElements( STwoLineElements& sTLE );

/*------------------------------------------------------------------*/
// function COrbitProp::convertMeanEletoTwoLineEle(const vector<SMeanElements>& SvMeanEle,
/**      vector<STwoLineElements>* pSTLE)
*
*  Converts the mean elements specfied by SMeanElements to two-line element set
* and returns the result to pSTLE
*/
//##   eGENERIC_ERROR_CODE convertMeanEletoTwoLineEle(const vector<SMeanElements>& SvMeanEle,
//##      vector<STwoLineElements>* pSTLE);
   eGENERIC_ERROR_CODE convertMeanEleToTwoLineEle(const vector<SMeanElements>& SvMeanEle,
      vector<STwoLineElements>& svTLE);
   eGENERIC_ERROR_CODE convertMeanEleToTwoLineEle(const SMeanElements& sMeanEle,
      STwoLineElements& sTLE);

   eGENERIC_ERROR_CODE convertSolarEleToMeanEle (
     const double& dTime,
     const double& dInclination,
     const double& dApogee,
     const double& dPerigee,
     const double& dLocalTimeofApogee,
     const double& dLocalTimeofMaxIncl,
     const double& dMeanAnomaly,
     SMeanElements& sMeanEle );
   eGENERIC_ERROR_CODE convertSolarEleToMeanEle (
     const CTimeValue& tTime,
     const double& dInclination,
     const double& dApogee,
     const double& dPerigee,
     const double& dLocalTimeofApogee,
     const double& dLocalTimeofMaxIncl,
     const double& dMeanAnomaly,
     SMeanElements& sMeanEle );

   // load 'simple' elements into Mean element structure
   eGENERIC_ERROR_CODE convertSimpleEleToMeanEle (
   const double& dTime,
     const double& dInclination,
     const double& dApogee,
     const double& dPerigee,
     const double& dRightAscension,
     const double& dArgofPerigee,
     const double& dMeanAnomaly,
     SMeanElements& sMeanEle );
   eGENERIC_ERROR_CODE convertSimpleEleToMeanEle (
   const CTimeValue& tTime,
     const double& dInclination,
     const double& dApogee,
     const double& dPerigee,
     const double& dRightAscension,
     const double& dArgofPerigee,
     const double& dMeanAnomaly,
     SMeanElements& sMeanEle );

   // load 'GEO' elements into Mean element structure
  eGENERIC_ERROR_CODE convertGeoToMeanEle (
    const double& dTime,
    const double& dLongitude,
    SMeanElements& sMeanEle );
  eGENERIC_ERROR_CODE convertGeoToMeanEle (
    const CTimeValue& tTime,
    const double& dLongitude,
    SMeanElements& sMeanEle );

/*------------------------------------------------------------------*/
// function COrbitProp::parseTwoLineElements(STwoLineElementsVec* psvTLE)
/*
*  Parses the members sLine1 and sLine2 of thentwo-line element structures
* contained in psvTLE and fills in the remaining elements of the structures
*
*
*/
 //###  eGENERIC_ERROR_CODE parseTwoLineElements(STwoLineElementsVec* psvTLE);
   eGENERIC_ERROR_CODE parseTwoLineElements(STwoLineElementsVec& svTLE);
   eGENERIC_ERROR_CODE parseTwoLineElements(STwoLineElements& sTLE);


/*------------------------------------------------------------------*/
// function COrbitProp::checkTimeOrder(const vector<STwoLineElements>& SvTLE)
/*
* Checks the vector of two-line elements contained in SvTLE and returns
* an error if any of the elements are out of sequence.
*
*
*/
   eGENERIC_ERROR_CODE checkTwoLineEle();
   eGENERIC_ERROR_CODE checkTwoLineEle(const vector<STwoLineElements>& svTLE);

   // function to load ephemeris structure from class storage, needed for backward compatibility
   void loadEphemStruct ( SEphemerisVec& vsEphemeris );

};


#endif
