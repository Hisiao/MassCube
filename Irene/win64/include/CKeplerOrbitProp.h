/******************************************************************************
$HeadURL$

 File: CKeplerOrbitProp.h

 Description: Declarations for methods to perform simple orbit propagation
   using Kepler's equations, with optional J2 perturbations.

 Classification:

   Unclassified

 Project Name:

   AE9/AP9/SPM Radiation Environment Models

   Developed under US Government contract # FA9453-12-C-0231

 Rights and Restrictions:

   Copyright 2015 Atmospheric and Environmental Research, Inc. (AER)

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
  
SVNTag: $Id$
******************************************************************************/

#ifndef CKEPLERORBITPROP_H
#define CKEPLERORBITPROP_H

#ifdef _WIN32
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#endif
#include <cmath>
#include <string>
#include <vector>

typedef vector<double> dvector;
typedef vector<dvector> d2dvector;
typedef vector<d2dvector> d3dvector;

const double TwoPi=2.0*M_PI;
const double dBAD_ANGLE = -9999.0;
const double dDegToRad = M_PI/180.0;

class CKeplerOrbitProp
{
  public:
    // constructors

    CKeplerOrbitProp();

    // destructor

    ~CKeplerOrbitProp();

    // Public Accessors

    void setOrbitType( char chType ); // 'c'lassical, 'm'ean, 's'olar, 'r'v, 'g'eographic
    void setOrbitalElementEpoch( double dEpochInMJD );
    void setUseJ2Perturbations( bool bUseJ2 );

    void resetElements( );
    void setInclination( double dInclinationInDeg );
    void setEccentricity( double dEccentricity );
    void setSemimajorAxis( double dSemiMajorAxisInRE );
    void setLongitudeOfAscNode( double dLongitudeInDeg );
    void setArgOfPerigee( double dArgOfPerigeeInDeg );
    void setTimeOfPerigee( double dTimeOfPerigeeInMJD );
    void setMeanAnomalyAtEpoch( double dMeanAnomalyInDeg );

    void setMeanMotion( double dMeanMotionInRevPerDay );
    void setAltitudeOfPerigee( double dAltitudeInKm );
    void setAltitudeOfApogee( double dAltitudeInKm );
    void setLocalTimeOfApogee( double dLocalTimeInHours );
    void setLocalTimeOfMaxInclination( double dLocalTimeInHours );

    void setPositionAtEpochGEI( double dX, double dY, double dZ );
    void setVelocityAtEpochGEI( double dU, double dV, double dW );

    void setGeosynchLon( double dLongitudeInDeg );

    void setOrbitTimes( double dStartMJD, double dEndMJD, double dTimestepSecs );
    void setOrbitTimes( const vector<double>& vdTimesMJD );

    void setMagfieldDB( const string& strMagfieldDB );

    // Public Compute Methods

    int computeEphemeris( dvector& vdTimesMJD,
                          dvector& vdXsGEI,
                          dvector& vdYsGEI,
                          dvector& vdZsGEI,
                          dvector& vdXDotsGEI,
                          dvector& vdYDotsGEI,
                          dvector& vdZDotsGEI );

  protected:

    // internal methods

    void setConstants( );
    int updateOrbitDefinition();
    char getOrbitType();
    void solveKepler(const dvector& vdM, 
                     const double& dEcc, 
                     dvector& vdE, 
                     double Tol=1.e-8);
    void ApplyJ2(const dvector& vdTime,
                 dvector& vdOmega,
                 dvector& vdomega,
                 dvector& vdM);
    void rv2classical();
    void solar2classical();
    void mean2classical();
    void geo2classical();
    double Magnitude(double* ptr_V);
    double* CrossProd(double* ptr_X1, double* ptr_X2);
    double DotProd(double* ptr_X1, double* ptr_X2);

    // internal data members

    bool     m_bRevised;              // flag to recalculate orbit with new/revised element values

    dvector  m_vdTimes;               // MJD times at which to compute ephemeris

    string   m_strMagfieldDB;         // magnetic field model db (for coord conversions)

  // internal data structures used by kepler propagation algorithm

  typedef struct Elements{
    double da;             // semimajor axis (Re)
    double db;             // semiminor axis (Re)
    double de;             // eccentricity
    double di;             // inclination (deg?rad?)
    double dE0;            // eccentric anomaly at epoch (deg)
    double dOmega;         // longitude of ascending node (deg)
    double domega;         // argument of perigee (deg)
    double dl0;            // true longitude at epoch (deg)
    double dr[3];          // position at epoch (GEI)
    double dv[3];          // velocity at epoch (GEI)
    double dn;             // mean motion (rev/day)
    double dQ;             // mean motion siderial day (rev/sid. day)
    double dP;             // orbit period (days)
    double dPl;            // longitude of perigee
    double dT;             // time of perigee (MJD)
    double depoch;         // orbital element time (MJD)
    double du0;            // argument of latitude at epoch (deg)
    double dnu0;           // true anomaly at epoch (deg)
    double dgeo_lon;       // geographic longitude (deg) of geosynchronous orbit
    double dM0;            // mean anomaly at epoch (deg)
    double dMe;            // Earth Mass (kg) (default 5.9737e24)
    double dRe;            // Earth radius (km) (6371.2, matlab code originally 6378.136)
    double dJ2;            // Earth ellipticity coefficient (default 1.082626e-3)
    double dG;             // Graviational constant (N m^2/kg^2) (default 6.67259e-11)
    double dA_a;           // altitude of apogee (km)
    double dA_p;           // altitude of perigee (km)
    double dH_i;           // local time of max inclination (hrs)
    double dH_a;           // local time of apogee (hrs)
    char   type;           // orbit type: 'c'lassical, 'm'ean, 's'olar, 'r'v (state vectors), 'geo'synchronous

  Elements():da(-1.0),db(-1.0),de(-1.0),di(-1.0), dE0(-1.0),dOmega(dBAD_ANGLE),domega(dBAD_ANGLE),dl0(-1.0),
             dn(-1.0),dQ(-1.0),dP(-1.0),dPl(-1.0),dT(-1.0),depoch(-1.0),du0(-1.0),dnu0(-1.0),
             dM0(dBAD_ANGLE),dMe(-1.0),dRe(-1.0),dJ2(-1.0),dG(-1.0),dA_a(-1.0),dA_p(-1.0),
             dH_i(-1.0),dH_a(-1.0),type('c')
  {dr[0] = -1.0;dr[1] = -1.0;dr[2] = -1.0;
   dv[0] = -1.0;dv[1] = -1.0;dv[2] = -1.0;
   dgeo_lon = dBAD_ANGLE;
  }
  } Elements;

  typedef struct MKSrads{
    double dRe;
    double dE;
    double dG;
    double di;
    double dn;
    double dnu;
    double dnu0;
    double dp;
    double dP;
    double dJ2;
    double dr;
    double dMe;
    double dsidereal_year;
    double dsidereal_day;
    double dOmega;
    double domega;
    double dT;
    double dcosi;
    double dsini;
    double dmu;
    double de;
    double da;
    double db;
    double dE0;
    double dM0;

  MKSrads(): dRe(-1.0),dE(-1.0),dG(-1.0),di(-1.0),dn(-1.0),dnu(-1.0),dnu0(-1.0),dp(-1.0),dP(-1.0),dJ2(-1.0),dr(-1.0),dMe(-1.0),
      dsidereal_year(-1.0),dsidereal_day(-1.0),dOmega(dBAD_ANGLE),domega(dBAD_ANGLE),dT(-1.0),dcosi(-1.0),dsini(-1.0),dmu(-1.0),
      de(-1.0),da(-1.0),db(-1.0),dE0(-1.0),dM0(dBAD_ANGLE){};
  } MKSrads;

  MKSrads mksrads;
  Elements elements;
};


#endif
