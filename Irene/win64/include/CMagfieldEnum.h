/***********************************************************************
 @File CMagfieldEnum.h
 
 Description:

   Definitions for magnetic field enumerations.

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


***********************************************************************/
// CMagfield - Magfield Full Enum and Def Interface
//
// Comments or questions to Rick Quinn (rquinn@aer.com).
//

#include "GenericModelEnum.h"

#ifndef __AerSpEnv_h__
#ifndef CMAGFIELD_ENUM_DEFINE
#define CMAGFIELD_ENUM_DEFINE

// global scope enums
//! @defgroup Enumerations CMagfield enumerations
//! @{

//! @enum  eMainField
//! @brief Main field type definitions
typedef enum eMainField {
  emfIGRF, //!< Full IGRF model
  emfFastIGRF, //!< 'Fast' IGRF model
  emfOffsetDipole, //!< offset (eccentric) dipole model
  emfTiltedDipole  //!< tilted dipole model
} eMainField;

//! @enum  eExternalField
//! @brief External field type definitions
typedef enum eExternalField {
  eefNone, //!< no external field
  eefOlsonPfitzer, //!< Olson-Pfitzer Quiet (1977)
  eefTsyganenko89 //!< Tsyganenko 1989(c)
} eExternalField ;


//! @enum  eFieldLineTraceDir
//! @brief Magnetic field line trace direction definitions
typedef enum eFieldLineTraceDir {
  efltdNorth,  //!< Northward from initial point
  efltdSouth,  //!< Southward from initial point
  efltdNorthAndSouth, //!< Northward from initial point, then Southward
  efltGsmEquator      //!< toward Geomagnetic equator
} eFieldLineTraceDir;

//! @enum  eLshellMode
//! @brief Magnetic field line trace mode definitions
typedef enum eLshellMode {
  elmDipole,  //!< Dipole shell trace mode
  elmMcIlwain //!< Lm shell trace mode
} eLshellMode ;

//! @enum  eMagFieldCoordSystemType
//! @brief Coordinate System definitions
typedef enum eMagFieldCoordSystemType {
  SPHinRE, //!< Spherical:  radius [Re], theta(colat) [deg], phi [deg]
  GSMinRE, //!< Geocentric Solar Magnetospheric:  x [Re], y [Re], z [Re]
  SMinRE,  //!< Solar Magnetic Cartesian:  x [Re], y [Re], z [Re]
  GEIinRE, //!< Geocentric Equatorial Inertial:  x [Re], y [Re], z [Re]
  GEOinRE, //!< Geographic Cartesian:  x [Re], y [Re], z [Re]
  MLLinRE, //!< Magnetic Dipole:  radius, [Re], MLat [deg], MLT [hrs]
  MAGinRE, //!< Dipole Magnetic Cartesian:  x [Re], y [Re], z [Re]
  MAGSinRE,//!< Dipole Magnetic Spherical:  Radius [Re], Mlat [deg], Mlon [deg]
  GSEinRE, //!< Geocentric Solar Ecliptic:  x [Re], y [Re], z [Re]
  GSEQinRE,//!< Geocentric Solar Equatorial:  x [Re], y [Re], z [Re]
  ODXinRE, //!< Offset Dipole Cartesian: mx [Re], my [Re], mz [Re]
  ODSinRE, //!< Offset Dipole Spherical: Radius [Re], mlat [deg], mlon [deg]
  ODXinKM, //!< Offset Dipole Cartesian: mx [km], my [km], mz [km] () 
  ODSinKM, //!< Offset Dipole Spherical: Radius [km], mlat [deg], mlon [deg]
  SPHinKM, //!< Spherical:  radius [km], co-latitude [deg], phi [deg]
  GSMinKM, //!< Geocentric Solar Magnetospheric:  x [km], y [km], z [km]
  SMinKM,  //!< Solar Magnetic Cartesian:  x [km], y [km], z [km]
  GEIinKM, //!< Geocentric Equatorial Inertial:  x [km], y [km], z [km]
  GEOinKM, //!< Geographic Cartesian:  x [km], y [km], z [km]
  MLLinKM, //!< Magnetic Dipole:  radius [km], MLat [deg], MLT [hrs]
  MAGinKM, //!< Dipole Magnetic Cartesian:  x [km], y [km], z [km]
  MAGSinKM,//!< Dipole Magnetic Spherical:  radius [km], Mlat [deg], Mlon [deg]
  GSEinKM, //!< Geocentric Solar Ecliptic:  x [km], y [km], z [km]
  GSEQinKM,//!< Geocentric Solar Equatorial:  x [km], y [km], z [km]
  GEODinKM,//!< WGS84 Geodetic:  alt [km], lat [deg], lon [deg] ()
  GDZinKM, //!< WGS84 Geodetic (syn):  alt [km], lat [deg], lon [deg]
  RLLinRE, //!< RadiusLatLon:  radius [Re], lat [deg], lon [deg]
  RLLinKM  //!< RadiusLatLon:  radius [km], lat [deg], lon [deg]
} emfCoordSys;
//! @}

typedef enum eBfieldApp {
  ebfaFootprint,
  ebfaTrace
} eBfieldApp;


#endif
#endif
