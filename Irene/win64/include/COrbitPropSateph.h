/***********************************************************************

Module: CSateph.h

Description: Class definition for CSateph object

Description:  Generates satellite ephemeris given a two-line element set using
the Sateph orbit propagation model.

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
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>

#include "COrbitProp.h"

#ifndef CSateph_MODEL_DEFINE
#define CSateph_MODEL_DEFINE

const double asube = 6378.137;    // equatorial radius of Earth (WGS 84)
const double fe  = 0.99664714;    // polar/equat ratio   (1-1/f) 
const double we  = 0.72921158e-4; // mean angular velocity of earth rotation
const double omu = 3.986032e+5;   // geocentric gravitational constant (G*Me)
const double xj2 = 0.0010827549;  // gravitational harmonic term for oblateness
const double xj3 = -0.253881e-05; //  """, for higher order
const double twopi = 2.0 * M_PI;  // 2*PI constant
const double rad = M_PI/180.0;    // angular degrees to radian conversion factor
const double tmu = sqrt(pow(asube,3)/omu); // "canonical time unit" (seconds)
//  "the time required by a hypothetical satellite to move one radian in a 
//    circular orbit of the earth's equatorial radius, ~13.447 minutes"

inline double sign1(const double& arg) { return (arg)<0. ? (-1.) : (1.);}
inline double sign(const double& arg1, const double& arg2) { return fabs(arg1)*sign1(arg2);}
inline double hrev(const double& arg) { return arg-360.*sign1(arg)*((int)(fabs(arg)/360.0+0.5));}

//ANGLE1 ** FUNCTION TO REDUCE AN ANGLE TO THE INTERVAL (-PI,+PI) ** 
inline double angle1(const double& arg) { return arg-twopi*sign1(arg)*((int)(fabs(arg)/twopi+0.5));}

// ANGLE2 ** FUNCTION TO REDUCE AN ANGLE TO THE INTERVAL (0,2PI) ** 
inline double angle2(const double& arg) { return fmod(arg,twopi)+M_PI-M_PI*sign1(arg);}

class CSateph:public COrbitProp{

public:
/*------------------------------------------------------------------*/
// function CSateph::CSateph()
/**
*
* Default constructor
*
*
*/
   CSateph();

/*------------------------------------------------------------------*/
// function CSateph::CSateph(const string& sTleFilename)
/**
*
* Constructor that reads in a TLE file
*
*
*/
   CSateph(const string& sTleFilename);

/*------------------------------------------------------------------*/
// function CSateph::CSateph()
/**
*
* Destructor
*
*
*/
   ~CSateph();

/*------------------------------------------------------------------*/
// function CSateph::computeModelEphem()
/**
*
*  Computes the ephemeris
*
*
*/
   eGENERIC_ERROR_CODE computeModelEphem();


   eGENERIC_ERROR_CODE convertStateVectorsToTLEstrings(const CTimeValue& tTime, 
     const S3Tuple& s3Pos, const S3Tuple& s3Vel, string& sTleOne, string& sTleTwo, 
     const double& dPDecay=0.0 );

   eGENERIC_ERROR_CODE convertStateVectorsToTLEstrings(const double& dTime, 
     const S3Tuple& s3Pos, const S3Tuple& s3Vel, string& sTleOne, string& sTleTwo, 
     const double& dPDecay=0.0 );

   eGENERIC_ERROR_CODE convertStateVectorsToTLEstrings(
     const int& iYear, const int iDdd, const double& dGmtsec, 
     const S3Tuple& s3Pos, const S3Tuple& s3Vel, string& sTleOne, string& sTleTwo, 
     const double& dPDecay=0.0 );

   eGENERIC_ERROR_CODE loadStateVectors ( const CTimeValue& tTime, 
     const S3Tuple& s3Pos, const S3Tuple& s3Vel, const double& dPDecay=0.0 );

   eGENERIC_ERROR_CODE loadStateVectors ( const double& dTime, 
     const S3Tuple& s3Pos, const S3Tuple& s3Vel, const double& dPDecay=0.0 );

   eGENERIC_ERROR_CODE loadStateVectors ( 
     const int& iYear, const int iDdd, const double& dGmtsec, 
     const S3Tuple& s3Pos, const S3Tuple& s3Vel, const double& dPDecay=0.0 );

private:

   double tm1,tm2,tm3,tm4;
   double dayprt,dayelx;
   double ftdrax,stdrax;
   double wasx,wpeix;
   double ftdapx,stdapx;
   double oinclx;
   double ftdix;
   double smx;
   double ftdsmx,stdsmx;
   double axsemi;
   double eccex;
   double ftdex,stdex;
   double erase;
   double xmeax;
   double xmx;
   double ftdmx;
   double stdmx;
   double ttdmx;
   double delday;


/*------------------------------------------------------------------*/
// function CSateph::computeElements(double* wasc,double* wperi,double* oincl,
//      double* axsmer,double* eccen, double* xmean)
/**
*
*  Computes the elements
*
*
*/
   eGENERIC_ERROR_CODE computeElements(double* wasc,double* wperi,double* oincl,
      double* axsmer,double* eccen, double* xmean);

//  routines ported from fortran code

/*------------------------------------------------------------------*/
// function CSateph::Jdate(const int& mday, const int& imon,
//      const int& imyr, double* dayjul)
/**
*
*  days elapsed from FEB. 29, 1900 TO JAN. 1, 1950 = 18204
*  WHERE -
*     MDAY = DAY OF MONTH - 2 DIGITS
*     MON = MONTH - 2 DIGITS
*     MYR = YEAR - 2 DIGITS
*
*
*/
   void Jdate(const int& mday, const int& imon,
      const int& imyr, double* dayjul);

/*------------------------------------------------------------------*/
// function CSateph::Convrt(const STwoLineElements& Stle, double* ftdmm,
//      double* stdmm, double* ftdra, double* ftdap, double* sma, 
//      double* ftdsma, double* stdsma)
/**
*
*  I don't know what this does
*
*
*/
   void Convrt(const STwoLineElements& Stle, double* ftdmm,
      double* stdmm, double* ftdra, double* ftdap, double* sma, 
      double* ftdsma, double* stdsma);

/*------------------------------------------------------------------*/
// function CSateph::Oscupv(const double& xmean,const double& wperi,const double& wasc,
//      const double& oincl,const double& eccen,const double& axsmer,
//      dvector* pv,double* trulon)
/**
*
*  ROUTINE TO CONVERT MEAN ELEMENTS TO OSCUL. POS. AND VEL. **
*     ACCEPTS AS INPUT
*             ELM - ARRAY WITH THE 6 ORBITAL ELEMENTS IN THE ORDER
*                   MEAN ANOMALY (RADIANS)
*                   ARGUMENT OF PERIGEE (RADIANS) 
*                   LONGITUDE OF THE ASCENDING NODE (RADIANS) 
*                   INCLINATION ANGLE (RADIANS) 
*                   ECCENTRICITY
*                   SEMI-MAJOR AXIS (EARTH-RADII) 
*     PRODUCES AS OUTPUT
*             PV - ARRAY WITH POSITION AND VELOCITY IN THE ORDER
*  
*
*
*/
   void Oscupv(const dvector& dvElem, dvector& dvPosVel, double& dTruLon);

/*------------------------------------------------------------------*/
// function CSateph::Mnthil(const double& xmean,const double& wperi,const double& wasc,
//      const double& oincl,const double& eccen,const double& axsmer,
//      dvector* ef,dvector* hill)
/**
*  SUBROUTINE TO CONVERT MEAN ELEMENTS TO HILL VARIABLES
*
*
*     ACCEPTS AS INPUT
*             ELM - ARRAY OF 6 ORBIT ELEMENTS 
*                   (SEE OSCUPV LISTING)
*
*     PRODUCES AS OUTPUT
*             EF - ARRAY OF 3 TRUE ANOMALY VARIABLES
*               F - TRUE ANOMALY (RADIANS)
*               ESINF - ECCEN. *  SINE OF TRUE ANOMALY
*               ECOSF - ECCEN. * COSINE OF TRUE ANOMALY 
*             HILL - ARRAY OF 6 HILL VARIABLES
*               R - RADIUS VECTOR MAGNITUDE (EARTH RADII) 
*               G - ANGULAR MOMENTUM MAGNITUDE PER UNIT MASS
*                   (EARTH RADII SQUARED PER CANONICAL UNIT 
*                    OF TIME)
*               CH - ANGULAR MOMENTUM PER UNIT MASS Z COMPONENT
*               RDOT - RADIAL VELOCITY(EARTH RADII PER CUT) 
*               U - TRUE ORBITAL LONGITUDE (RADIANS)
*               TH - LONGITUDE OF ASCENDING NODE (=ELM(3))
*/
   void Mnthil(const dvector& dvElem, dvector& dvEf, dvector& dvHill);

/*------------------------------------------------------------------*/
// function CSateph::Shp(const dvector& hill,const dvector& ef,const double& xmean,
//      const double& wperi,const double& wasc,const double& oincl,
//      const double& eccen,const double& axsmer,const bool& ieq,dvector* dhill)
/**
*  SUBROUTINE TO COMPUTE SHORT-PERIODIC CORRECTIONS TO HILL VARIABL
*     ACCEPTS AS INPUT
*             HILL - ARRAY OF 6 HILL VARIABLES (SEE MNTHIL LISTING)
*             EF - ARRAY OF 3 TRUE ANOMALY VARIABLES
*                   (SEE MNTHIL LISTING)
*             ELM - ARRAY OF 6 ORBITAL ELEMENTS 
*                   (SEE OSCUPV LISTING)
*             TJ - SECOND HARMONIC GEOPOTENTIAL COEFFICIENT 
*                    MAGNITUDE
*             TJ3 - THIRD HARMONIC GEOPOTENTIAL COEFFICIENT 
*     PRODUCES AS OUTPUT
*             DHILL - PERTURBATIONS IN 6 HILL VARIABLES 
*/
   void Shp(const dvector& dvHill, const dvector& dvEf, const dvector& dvElem, 
     dvector& dvDeltaHill, const bool& bEquat=false);

/*------------------------------------------------------------------*/
// function CSateph::Kepecn(const double& dMeanAnom, const double& dEccent,double& dEccAnom)
/**
*  KEPECN ** ROUTINE TO CONVERT MEAN ANOMALY TO ECCENTRIC ANOMALY **
*     ACCEPTS AS INPUT
*             XM - MEAN ANOMALY IN RADIANS
*             E - ECCENTRICITY
*     PRODUCES AS OUTPUT
*             ECANOM - ECCENTRIC ANOMALY IN RADIANS 
*/
   void Kepecn(const double& dMeanAnom, const double& dEccent,double& dEccAnom);

/*------------------------------------------------------------------*/
// function CSateph::Hiltpv(const dvector& hill,const bool& ieq,dvector* pv)
/**
*  HILTPV ** SUBROUTINE TO COMPUTE POS-VEL FROM HILL VARIABLES
*     ACCEPTS AS INPUT
*             HILL - ARRAY OF 6 HILL VARIABLES (SEE MNTHIL LISTING)
*     PRODUCES AS OUTPUT
*            PV - POSITION-VELOCITY ARRAY OF 6
*                  VARIABLES (SEE OSCUPV LISTING) 
*
*/
   void Hiltpv(const dvector& dvHill, dvector& dvPosVel, const bool& bEquat=false);

/*------------------------------------------------------------------*/
// function CSateph::Revol(const dvector& dv,const double& axsemi,const int& irev0,
//      const double& trulon,int* irev)
/**
*
*	* * * COMPUTE THE ANGULAR PERCENTAGE OF ORBIT FROM THE 
*	* * *	TRUE ANOMALY FOR THE CALCULATED LOCATION
*
*
*	* * * DETERMINE REVOLUTION NUMBER USING THE TRUE ORBITAL
*	* * *	LONGITUDE (U1=W+V)
*	* * * NODAL REVOLUTION (BN) AND SIGN OF THE LATITUDE (DV(3))
*
*
*/
   void Revol(const dvector& dvPosVel, const double& dAxSemiMaj, const int& iRev0,
   const double& dTruLon, int& iRev);

   // support methods for convertStateVectorsToTLEstrings()

   int Drev( const double& dPDecay, const double& dMM0, const double& dMM1,
     dvector& dvElem, const double& dTime, dvector& dvDelta, double& dCDam, 
     const bool& bInitial=false );

   int NumInt(const double& dCDam, dvector& dvElem, const double& dTime,
     dvector& dvDelta );

   int IntGnd( dvector& dvElem, const double& dEa, const double& dTime,
     dvector& dvFFt );

   double Densel(const double& dTime, const dvector& dvPosVel, const double& dDensFac );

   void Opvtmn ( const dvector& dvPosVel, dvector& dvElem );

   void Pvthil(const dvector& dvPosVel, dvector& dvHill);

   void Hiltmn(const dvector& dvHill, dvector& dvElement, const bool& bEquat=false);

   void Eccf(const double& dRadMag, const double& dAngMom,
     const double& dRadVel, double& dEccent, dvector& dvEf, double& dMeanAnom);

};

#endif
