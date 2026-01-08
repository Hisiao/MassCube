/***********************************************************************

Module:	CoordXform.h

Description: AER SpWx Coordinate Transformation Library

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
     1.0       Mon Jan 5, 2009   First release

------------------------------------------------------------------------

***********************************************************************/

#ifndef COORD_XFORM_H
#define COORD_XFORM_H 

class CoordXform
{
public:

/*------------------------------------------------------------------*/
// function CoordXform::CoordXform
/**
*
* Constructor
*/
  CoordXform();

/*------------------------------------------------------------------*/
// function CoordXform::~CoordXform
/**
*
* Destructor
*/
  ~CoordXform();

/*------------------------------------------------------------------*/
// function CoordXform::cartesianToSpherical
/**
*
* Static function to converts cartesian coordinates to spherical
*
*    @param dX
*           double: input Cartesian X coordinate [Re or km]
*    @param dY
*           double: input Cartesian Y coordinate [Re or km]
*    @param dZ
*           double: input Cartesian Z coordinate [Re or km]
*
*    @param[out] pdRad
*           double: returned Radius value [Re or km]
*    @param[out] pdLat
*           double: returned Latitude value [deg]
*    @param[out] pdLon
*           double: returned East Longitude value [deg]
*
*    @returns
*           int: 0 success, else error code
*/
  static int cartesianToSpherical( double dX,
				                           double dY,
				                           double dZ,
				                           double* pdRad,
				                           double* pdLat,
				                           double* pdLon );

/*------------------------------------------------------------------*/
// function CoordXform::sphericalToCartesian
/**
*
* Static function to convert spherical coordinates to cartesian
*
*    @param dRad
*           double: input Radius value [Re or km]
*    @param dLat
*           double: input Latitude value [deg]
*    @param dLon
*           double: input East Longitude value [deg]
*
*    @param[out] pdX
*           double: returned Cartesian X coordinate [Re or km]
*    @param[out] pdY
*           double: returned Cartesian Y coordinate [Re or km]
*    @param[out] pdZ
*           double: returned Cartesian Z coordinate [Re or km]
*
*    @returns
*           int: 0 success, else error code
*/
  static int sphericalToCartesian( double dRad,
				                           double dLat,
				                           double dLon,
				                           double* pdX,
				                           double* pdY,
				                           double* pdZ );

/*------------------------------------------------------------------*/
// function CoordXform::cartesianToGeodetic
/**
*
* Static method to convert cartesian coordinates to geodetic. 
*
* Algorithm taken from
* http://en.wikipedia.org/wiki/Geodetic_system#Conversion,
*
*    @param dX
*           double: input Cartesian X coordinate [km]
*    @param dY
*           double: input Cartesian Y coordinate [km]
*    @param dZ
*           double: input Cartesian Z coordinate [km]
*
*    @param[out] pdAlt
*           double: returned Altitude value above oblate Earth surface [km]
*    @param[out] pdLat
*           double: returned Latitude value [deg]
*    @param[out] pdLon
*           double: returned East Longitude value [deg]

*    @returns
*           int: 0 success, else error code
*/
  static int cartesianToGeodetic( double dX,
				                          double dY,
				                          double dZ,
				                          double* pdAlt,
				                          double* pdLat,
				                          double* pdLon );

/*------------------------------------------------------------------*/
// function CoordXform::geodeticToCartesian
/**
*
* Static function to convert Geodetic coordinates to cartesian <br>
* Method is taken from
* http://en.wikipedia.org/wiki/Geodetic_system#Conversion.
*
*    @param dAlt
*           double: input Altitude value [km]
*    @param dLat
*           double: input Latitude value [deg]
*    @param dLon
*           double: input East Longitude value [deg]
*
*    @param[out] pdX
*           double: returned Cartesian X coordinate [km]
*    @param[out] pdY
*           double: returned Cartesian Y coordinate [km]
*    @param[out] pdZ
*           double: returned Cartesian Z coordinate [km]
*
*    @returns
*           int: 0 success, else error code
*/
  static int geodeticToCartesian( double dAlt,
				                          double dLat,
				                          double dLon,
				                          double* pdX,
				                          double* pdY,
				                          double* pdZ );

  //protected:

};

#endif
