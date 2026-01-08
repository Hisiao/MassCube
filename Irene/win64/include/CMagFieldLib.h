/***********************************************************************
 File: CMagFieldLib.h
 
 Description:

   Declarations for C++ wrappers on lower-level routines.

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


***********************************************************************/
#ifndef CMagFieldLib_H
#define CMagFieldLib_H

//  Interface to Magnetic Field components
//  BfGEOS magfield library top level components
//
//

#include <string>
#include <cmath>
#include <iostream>
#include <sstream>

// include low level interfaces to magfield library

#include "CMagFieldErrors.h"
#include "CMagFieldIGRF.h"
#include "CMagfieldEnum.h"
#include "CMagFieldMparts.h"
#include "CTimeValue.h"

class CMagFieldLib
{
 // put private first
 private:

  // IGRF base object
  CMagFieldIGRF *psIgrf;

  // magnetic field calculations, was FORTRAN, now ported to C++
  CMagFieldMparts *cpMparts;

  int ciIsinitialized;   // initialization flag
  double cdDegToRad;     // degrees to radian
  double cdaG2M[9];      // dipole GEO to MAG conversion matrix
  
  // ********************************************
  //  Private Parameter Configuration (sets/gets)
  // ********************************************

  // calculate dipole field line points
  eMAGFIELD_ERROR_CODE FieldLineDipoleLand( const double &dXgeo,
                                            const double &dYgeo,
                                            const double &dZgeo,
                                            const double &dInc,
                                            const int &iDir,
                                            const bool &bReturnLast,
                                            double &dL,
                                            int &iNumPts,
                                            double* pdX,
                                            double* pdY,
                                            double* pdZ );

  // convert date to recalc2 format
  eMAGFIELD_ERROR_CODE recalc2Date( const CTimeValue& tTime,
                                    double& dDecimalYear,
                                    double& dModJulDate );

 public:
  // Constructor
  CMagFieldLib();

  // Destructor
  ~CMagFieldLib();

  eMAGFIELD_ERROR_CODE initialize( CMagFieldIGRF* psIgrfObj );

  // updateIGRF(): update the IGRF coefficients using dTime
  eMAGFIELD_ERROR_CODE updateIGRF( const double& dTime );
  eMAGFIELD_ERROR_CODE updateIGRF( const CTimeValue& tTime );

  // updateSR2(): update the SR2 (coord transform) coefficients using dTime
  eMAGFIELD_ERROR_CODE updateSR2( const double& dTime );
  eMAGFIELD_ERROR_CODE updateSR2( const CTimeValue& tTime );

  // get coeficients for coordinate transforms
  // set/get to change/query initialization paramaters
  eMAGFIELD_ERROR_CODE setMainFieldModel( const eMainField& mfmt );
  eMAGFIELD_ERROR_CODE getMainFieldModel( eMainField& mfmt )
  {
    mfmt = eMainField(cpMparts->getMainFieldModel());
    return(emfNoError);
  };

  eMAGFIELD_ERROR_CODE setExternalFieldModel( const eExternalField& mfmt );
  eMAGFIELD_ERROR_CODE getExternalFieldModel( eExternalField& mfmt )
  {
    mfmt = eExternalField(cpMparts->getExternalFieldModel());
    return(emfNoError);
  };

  //  ### compatability mode for computeAdInvariants "original" version
  //  Affects Lm trace radial threshold level: 
  //     True  => 1.0 Re  ("original") - used for IRENE 1.5x and prior in Ae9Ap9Model.cpp
  //     False => 0.9 Re  ("new", default state)
  //   -- a lower threshold enables 'computeAdInvariants' method to be able
  //       to sometimes compute valid Lm values at lower pitch angles
  //   -- this 'original' mode is to be phased out in ~2024, for IRENE v1.60 release
  void setCompatMode( bool bState=true ) { cpMparts->setCompatMode( bState ); }

  eMAGFIELD_ERROR_CODE getDipParams( double* pdDimo,
                                     double* pdTheta,
                                     double* pdPhi,
                                     double* pdEta,
                                     double* pdZeta,
                                     double* pdXi,
                                     dvector* pdvGeoToMag );

  eMAGFIELD_ERROR_CODE setDBFieldLineTraceDirType( const eFieldLineTraceDir& dbftdt );
  eMAGFIELD_ERROR_CODE getDBFieldLineTraceDirType( eFieldLineTraceDir& dbftdt )
  {
    dbftdt = eFieldLineTraceDir(cpMparts->getDBFieldLineTraceDir());
    return(emfNoError);
  };

  eMAGFIELD_ERROR_CODE setUnitLog( const int& iunit );
  eMAGFIELD_ERROR_CODE getUnitLog( int& iunit ) 
  {
    iunit = cpMparts->getUnitLog();
    return(emfNoError);
  };

  eMAGFIELD_ERROR_CODE setUnitBfout( const int& iunit );
  eMAGFIELD_ERROR_CODE getUnitBfout( int& iunit )
  {
    iunit = cpMparts->getUnitBfout();
    return(emfNoError);
  };

  eMAGFIELD_ERROR_CODE setUnitHitout( const int& iunit );
  eMAGFIELD_ERROR_CODE getUnitHitout( int& iunit )
  {
    iunit = cpMparts->getUnitHitout();
    return(emfNoError);
  };

  eMAGFIELD_ERROR_CODE getWdst( double& dwdst )
  {
    dwdst = cpMparts->getWdst();
    return(emfNoError);
  };

  eMAGFIELD_ERROR_CODE getWeqedg( double& dweqedg )
  {
    dweqedg = cpMparts->getWeqedg();
    return(emfNoError);
  };

  eMAGFIELD_ERROR_CODE getTiltAngle( double& dtilt )
  {
    dtilt = cpMparts->getTilt();
    return(emfNoError);
  };

  // getOutputSizes() computes an integer multiplier that can be used
  // to compute the size of the output array needed according to the
  // current initialization state of the model.  For instance, for
  // ComputationMode Bfield, the sizeof(daBfieldResults) =
  // getOutputSizes()*iNumGeoData.
  eMAGFIELD_ERROR_CODE getOutputSizes( int& ibfield,
                                       int& ifieldlines );

  eMAGFIELD_ERROR_CODE setFootRadius( const double& dRadiusKm );
  eMAGFIELD_ERROR_CODE getFootRadius( double& dRadiusKm )
  {
    dRadiusKm = cpMparts->getFootRadius();
    return(emfNoError);
  };

  // fill in to get offset Dipole B-field results
  eMAGFIELD_ERROR_CODE BfieldOffsetDipole( const double& dGeoDataX,  // input position
                                           const double& dGeoDataY,  // input position
                                           const double& dGeoDataZ,  // input position 
                                           double& dXgeoB,           // output Bx
                                           double& dYgeoB,           // output By
                                           double& dZgeoB,           // output Bz 
                                           int& iOpen );             // open field line

  // fill in to get L dipol L
  eMAGFIELD_ERROR_CODE BfieldGetL( const double& dGeoDataX,  // input position
                                   const double& dGeoDataY,  // input position
                                   const double& dGeoDataZ,  // input position 
                                   double& dL );             // output L

  // fill in to get L
  eMAGFIELD_ERROR_CODE BfieldGetLm( const double& dGeoDataX,  // input position
                                    const double& dGeoDataY,  // input position
                                    const double& dGeoDataZ,  // input position 
                                    double* pdLm );           // output Lm 

  // fill in to get BBeq
  eMAGFIELD_ERROR_CODE BfieldGetBBeq( const double& dGeoDataX,  // input position
                                      const double& dGeoDataY,  // input position
                                      const double& dGeoDataZ,  // input position 
                                      double* pdLm,             // output L
                                      double* pdBmag,           // output B magnitude
                                      double* pdBeq,            // output B at eq
                                      double* pdBBeq );         // output B / Beq

  // runs Field Model for single point used initial date for IGRF, FAST, Center-Dipole
  eMAGFIELD_ERROR_CODE BfieldBfunc( double& dGeoDataX,  // input position
                                    double& dGeoDataY,  // input position
                                    double& dGeoDataZ,  // input position 
                                    double& dXgeoB,     // output Bx
                                    double& dYgeoB,     // output By
                                    double& dZgeoB );   // output Bz 

  // interface single fieldline trace
  eMAGFIELD_ERROR_CODE FieldLineTrace( double& dGeoDataX0,           // input position 
                                       double& dGeoDataY0,           // input position
                                       double& dGeoDataZ0,           // input position 
                                       double* daFieldLineGeoX,      // output results arrays
                                       double* daFieldLineGeoY,      // output results arrays
                                       double* daFieldLineGeoZ,      // output results arrays
                                       int& iSizeFieldLineResutls ); // # of points in output arrays

  // Dipole Field line trace of one set of points
  eMAGFIELD_ERROR_CODE FieldLineTraceDipole( const double& dGeoDataX0,
                                             const double& dGeoDataY0,
                                             const double& dGeoDataZ0,
                                             const bool &bReturnLast,
                                             double* daFieldLineGeoX,
                                             double* daFieldLineGeoY,
                                             double* daFieldLineGeoZ,
                                             int& iSizeFieldLineResults,
                                             double &dL );

  // Foot Print trace of one set of points
  eMAGFIELD_ERROR_CODE FootPrint( double& dGeoDataX0,           // input position array
                                  double& dGeoDataY0,           // input position array
                                  double& dGeoDataZ0,           // input position array
                                  double* daFieldLineGeoX,      // output results arrays
                                  double* daFieldLineGeoY,      // output results arrays
                                  double* daFieldLineGeoZ,      // output results arrays
                                  int& iSizeFieldLineResults ); // # of points in output arrays

  // calculates position at which sphere intersects with line segment
  eMAGFIELD_ERROR_CODE FieldLineAltitudePosition( double& dXa, 
                                                  double& dYa, 
                                                  double& dZa, 
                                                  double& dXn, 
                                                  double& dYn, 
                                                  double& dZn, 
                                                  double& dSphRad,
                                                  double& dXRad, 
                                                  double& dYRad, 
                                                  double& dZRad );

  // returns full igrf b-field vector given geocords (KM): x, y, z, bx, by, bz, b
  eMAGFIELD_ERROR_CODE getFullIgrfGEO( double& dxgeo, // input position
                                       double& dygeo, // input position
                                       double& dzgeo, // input position
                                       double& dbx,   // output bx (geo)
                                       double& dby,   // output by (geo)
                                       double& dbz,   // output bz (geo)
                                       double& db );  // output b magnitude

  // returns fast igrf b-field vector given geocords (KM): x, y, z, bx, by, bz, b
  eMAGFIELD_ERROR_CODE getFastIgrfGEO( double& dxgeo, // input position
                                       double& dygeo, // input position
                                       double& dzgeo, // input position
                                       double& dbx,   // output bx (geo)
                                       double& dby,   // output by (geo)
                                       double& dbz,   // output bz (geo)
                                       double& db );  // output b magnitude

  // *****************************
  // Coordinate Conversion Methods
  // *****************************

  /*------------------------------------------------------------------*/
  // function CMagFieldLib::convertCoord

  /**
   *
   * Converts a coordinate-triple in one coordinate system to another
   * coordinate system. Input and output coordinate systems are defined
   * using the eMagFieldCoordSystemType (emfCoordSys) enum (see
   * CMagFieldLib.h). 
   *
   * @param emfCoordIn
   *    coordinate system of coordinate on input
   *
   * @param emfCoordOut
   *    coordinate system of coordinate on output
   *
   * @param dvCin
   *    double vector holding input coordinate (3 elements)
   *
   * @return pdvCout
   *   double vector holding output coordinate (3 elements)
   *
   * @return eMAGFIELD_ERROR_CODE
   *    emfNoError - success
   *   emfInitializationFailed - magfield model not initialized
   *   emfError - failed to convert coords.
   */
  eMAGFIELD_ERROR_CODE convertCoord( const emfCoordSys& emfCoordIn,
                                     const emfCoordSys& emfCoordOut,
                                     const dvector& dvCin,
                                     dvector* pdvCout );

  // wrapper for above for individual values
  eMAGFIELD_ERROR_CODE convertCoord( const emfCoordSys& emfCoordIn,
                                     const emfCoordSys& emfCoordOut,
                                     const double& dC1in,
                                     const double& dC2in,
                                     const double& dC3in,
                                     double* pdC1out,
                                     double* pdC2out,
                                     double* pdC3out );

  // convert coordinates from geo (KM) to gsm (RE)
  eMAGFIELD_ERROR_CODE convertGEOtoGSM( const double& dxgeo,
                                        const double& dygeo,
                                        const double& dzgeo,
                                        double* pdxgsm,
                                        double* pdygsm,
                                        double* pdzgsm );

  // convert coordinates from gsm (RE) to geo (RE), used for bvalues which are really nT
  eMAGFIELD_ERROR_CODE convertGSMtoGEORE( const double& dxgsm,
                                          const double& dygsm,
                                          const double& dzgsm,
                                          double* pdxgeo,
                                          double* pdygeo,
                                          double* pdzgeo );

  // convert coordinates from gsm (RE) to geo (KM)
  eMAGFIELD_ERROR_CODE convertGSMtoGEOKM( const double& dxgsm,
                                          const double& dygsm,
                                          const double& dzgsm,
                                          double* pdxgeo,
                                          double* pdygeo,
                                          double* pdzgeo );

  // convert coordinates to mll Radius (KM), MLAT(deg), MLT (hr) from geo (KM)
  eMAGFIELD_ERROR_CODE convertGEOtoMLL( const double& dXgeo,
                                        const double& dYgeo,
                                        const double& dZgeo,
                                        double* pdRadius,
                                        double* pdMlat,
                                        double* pdMlt );

  // convert coordinates from mll Radius (KM), MLAT(deg), MLT (hr) to geo (KM)
  eMAGFIELD_ERROR_CODE convertMLLtoGEO( const double& dRadius,
                                        const double& dMlat,
                                        const double& dMlt,
                                        double* pdXgeo,
                                        double* pdYgeo,
                                        double* pdZgeo );

  // Hiltel input B, xi, output L - uses dDimo from full igrf, full IGRF must be run first
  eMAGFIELD_ERROR_CODE runHiltel( double& db,
                                  double& dxi,
                                  double& dl );

  // invarm Calculate the various magnetic coordinates of the particles
  //        drift shell.  Calculate the 1st and 2nd adiabatic invariants and
  //        the l parameter for a number of pitch angles at the specified 
  //        location.  Also determine the local magnetic field, the magnetic 
  //        latitude, the minimum magnetic field on the field line and the 
  //        magnetic longitude at the field minimum. 
  //        provide consistent interface to above
  //        input coordinates are in geographic cartesian (km)
  eMAGFIELD_ERROR_CODE runInvarmGEO( const double& dxgeo, // input position
                                     const double& dygeo, // input position
                                     const double& dzgeo, // input position
                                     double* daPangle,    // A single pitch angle or an array of pitch 
                                                          // angles for the invariants will be calculated
                                     int& iNumang,        // Number of pitch angles
                                                          // outputs
                                     double* daEl,        // the l value calcluated from the invariant
                                     double* pdBlocal,    // Magnetic field (gauss)
                                     double* pdBmin,      // minimum value of b along the field line
                                     double* pdXmlong,    // magnetic longitude magnetic field minimum
                                     double* pdXmlat,     // magnetic latitude magnetic field minimum
                                     double* pdXmXyz,     // position of Bmin in geographic cartesian
                                     double* daBmaxan,    // mirror point magnetic field array
                                     double* daXj,        // second integral invariant
                                     double* daDensty,
                                     double* daBvec );    // densities

  // invarm Calculate the various magnetic coordinates of the particles
  //        drift shell.  Calculate the 1st and 2nd adiabatic invariants and
  //        the l parameter for a number of pitch angles at the specified 
  //        location.  Also determine the local magnetic field, the magnetic 
  //        latitude, the minimum magnetic field on the field line and the 
  //        magnetic longitude at the field minimum. 
  //        Provide consistent interface to above input coordinates are in
  //        lat, long, radius (duplicates original)
  eMAGFIELD_ERROR_CODE runInvarmRLL2( double& dXlat,    // input position
                                      double& dXllong,  // input position
                                      double& dRadius,  // input position
                                      double* daPangle, // A single pitch angle or an array of pitch
                                                        // angles for the invariants will be calculated
                                      int& iNumang,     // Number of pitch angles
                                                        // outputs
                                      double* daEl,     // the l value calcluated from the invariant
                                      double* pdBlocal, // Magnetic field (gauss)
                                      double* pdBmin,   // minimum value of b along the field line
                                      double* pdXmlong, // magnetic longitude magnetic field minimum
                                      double* pdXmlat,  // magnetic latitude magnetic field minimum
                                      double* pdXmXyz,  // position of Bmin in geographic cartesian
                                      double* daBmaxan, // mirror point magnetic field array
                                      double* daXj,     // second integral invariant
                                      double* daDensty,
                                      double* daBvec );


  // bffoot determines magnetic field line tracing start points along circumference
  //        of circle at a constant arc radius about a given focal point. Start points
  //        are at a constant geocentric distance. Magnetic field lines from these points
  //        can be used to visualize magnetic flux tubes. 
  eMAGFIELD_ERROR_CODE getFootPoints( const double& dXgeo,      // input geo (km)
                                      const double& dYgeo,      // input geo (km)
                                      const double& dZgeo,      // input geo (km)
                                      const double& dDiameter,  // input diameter of circle (km)
                                      int& iPts,                // input # of points on circle,
                                                                // size of [xyz]foot arrays
                                      double* daXfoot,          // output foot coord in GEO (km)
                                      double* daYfoot,          // output foot coord in GEO (km)
                                      double* daZfoot );        // output foot coord in GEO (km)

  eMAGFIELD_ERROR_CODE getFootPointsDeg( const double& dXgeo,        // input geo (km)
                                         const double& dYgeo,        // input geo (km)
                                         const double& dZgeo,        // input geo (km)
                                         const double& dDiameterDeg, // input diameter of circle (deg)
                                         int& iPts,                  // input # of points on circle,
                                                                     // size of [xyz]foot arrays
                                         double* daXfoot,            // output foot coord in GEO (km)
                                         double* daYfoot,            // output foot coord in GEO (km)
                                         double* daZfoot );          // output foot coord in GEO (km)
    
  // returns system kp value for date
  eMAGFIELD_ERROR_CODE magFieldGetSystemKp( double& dKp );

  bool isInitialized();
 
  // returns Earth Radius in Km used by cmagfield
  //double getEarthRadiusKm(){return cdEradKm;};
  double getEarthRadiusKm() { return cpMparts->getEradKm(); };

  // returns dipole movement in nT for mainfield.
  double getDimo();

  eMAGFIELD_ERROR_CODE DipoleFindLand( double dMLT,
                                       double dMlat1,
                                       double dMlat2,
                                       double dTermRadius,
                                       double dL,
                                       double dTolr,
                                       double& dDmlatL,
                                       double& dR );

  void setKpValue( const double& dKpVal ) { cpMparts->setKpValue( dKpVal ); }
  double getKpValue() { return cpMparts->getKpValue(); }

};
#endif  // CMagFieldLib_H
