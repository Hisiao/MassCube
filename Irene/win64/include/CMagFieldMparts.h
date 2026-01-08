/***********************************************************************
$HeadURL$

 File: CMagFieldMparts.h
 
 Description:

   Conversion of lower-level FORTRAN routines to C++.

 Rights and Restrictions:

   Copyright 2020 Atmospheric and Environmental Research, Inc. (AER)

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
   Aug 2022 literal port from FORTRAN
   Sep 2022 refactored C++ class to remove goto's, convert method arguments
            to class variables where possible, break up large methods
   Ported FORTRAN subroutine names are retained as method names, variables
   are retained but modified according to SpWx coding conventions.

***********************************************************************/
#ifndef CMagFieldMparts_H
#define CMagFieldMparts_H

// Interface to Magnetic Field components
// BfGEOS magfield library top level components

#include <string>
#include <cmath>
#include <complex>
#include <iostream>
#include <sstream>

// include low level FORTRAN interfaces to magfield library

#include "CMagFieldLib.h"
#include "CMagFieldErrors.h"
#include "CMagFieldIGRF.h"
#include "CMagfieldEnum.h"
#include "CMagFieldOP.h"
#include "CMagFieldT89.h"
#include "CTimeValue.h"

// coordinate types
enum eCoordType {
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Label |   0     1      2      3      4      5      6      7     8 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // Coord |  NONE  GSM     SM    GEI    GEO    MLL    MAG    GSE   GSEQ
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  eCoordTypeOther = 0,
  eCoordTypeGsm,
  eCoordTypeSm,
  eCoordTypeGei,
  eCoordTypeGeo,
  eCoordTypeMll,
  eCoordTypeMag,
  eCoordTypeGse,
  eCoordTypeGseq,
};

// structure for recalc2 variables
struct sR2_t {
  double dEwobbl;      // angle offset of spin to eccliptic 23.4523D0
  double dDyinyr;      // number of days in a year 365.256D0
  double dRedist;      // Radius distance in Meters 6.37104D+06
  double dModJulDate;  // time specification
  double dDecimalYear; // year + fraction of year
  int iTiltMode;       // tilt mode always 1, time needed for geo to gsm transforms.
  // coordinate tranform coeficients output from recalc2
  double dTilt;
  double dS1, dS2, dS3, dY1, dY2, dY3, dZ1, dZ2, dZ3;
  double dYB1, dYB2, dYB3, dZB1, dZB2, dZB3;
  double dYC1, dYC2, dYC3, dZC1, dZC2, dZC3;
  double dCtseq, dStseq, dGst, dSlong, dSrasn, dSdec;
  double dSps, dCps, dCt0, dSl0;
  double dDoffst, dDelong;
  // offset dipole tranforms from runRecal2
  double dXM, dDimo, dThetaN, dPhiN, dEta, dZeta, dXi;
  int iSetRecalc2;
};

// mag field class specific parameters and configuration values
struct sMF_t {
  // configuration values
  double dEradKm;                        // Earth radius in Km
  eMainField iMainFieldModel;            // main field model
  eExternalField iExternalFieldModel;    // external field model
  eFieldLineTraceDir iFieldLineTraceDir; // field Line trace direction
  int iBFieldOutput;                     // which fields to output
  double dFootRadius;                    // radius used for footprint height (km)
  double dSphrad;                        // radius used for footprint height at earth (Re)
  double dSphion;                        // radius used for footprint height at ionosphere (Re)
  int iNumOutput;                        // number of output coordinates
  double dWdst;                          // Returned wdst Dst (nT) H-V
  double dWeqedg;                        // Returned weqedg Eq.Edge (deg MLAT) H-V
  bool bMagFieldInitialized;
  // FORTRAN unit numbers
  int iUnitLog;    // Should leave 0 unless debugging
  int iUnitBfout;  // Not used here, output passed back by default
  int iUnitHitout; // Not used here, output passed back by default
};

// variables for calculation of the various magnetic coordinates of the particles drift shell
// in invar2: the 1st and 2nd adiabatic invariants and the L parameter for a number of pitch
// angles at the specified location, the local magnetic field, the magnetic latitude, the
// minimum magnetic field on the field line, and the magnetic longitude at the field minimum
struct sInvar_t {
  double dDds, dDel, dDs, dXmdimo;
  double daBl[3], daQ[3], daQl[3], daXl[3];
  double daRsv[100];
  double daSxj[100];
  double daXsv[100][3][4];
};

// structure used for Full IGRF
struct sFullIgrf_t { 
  // full IGRF constants/coefficients
  int iNmax;        // Number of IGRF fields read (Full), = Nmax
                    // Max number of IGRF fiels to use (Fast) = nMaxn
  double daG[225];  // normalize IGRF coefs(g+h)
  double daH[225];  // h/Full
  double daXi[3];   // xi/Full
  double dDimo;     // dipole movement
  double dXM;       // dipole movement
};

// structure used for Fast IGRF
struct sFastIgrf_t {
  // fast IGRF constants/coefficients
  int iNmax;        // Number of IGRF fields read (Full), = Nmax
                    // Max number of IGRF fiels to use (Fast) = nMaxn
  double daG[121];  // normalize IGRF coefs(g+h)
  double daC[121];  // Consts/Fast
  double daFm[11];  // constants/Fast
  double daFn[11];  // constants/Fast
  double dDimo;     // dipole movement
  double dXM;       // xm, adjusted dipole movement/Fast
  double dYearLast; // yrlast, year from last call/Fast
  int iFirst;       // first call flag
};

// Magnetic field state variables
struct sMainField_t {
  int iNstep;          // nstepBfld
  double daXnst[50];   // xnst
  double daZnst[50];   // znst
  double daBnst[50];   // bnst
  double dPi;          // pi
  double dBex;         // bexbfld
  double dQ;           // qbfld
  double dCsps;        // cspsbfld
  double dSnps;        // snpsbfld
  double dQcs;         // qcsbfld
  double dQsn;         // qsnbfld
  double dCsph;        // csphbfld
  double dSnph;        // snphbfld
  double dCt;          // ctbfld
  double dSt;          // stbfld
  double daZe[20];     // zecoef
  double daZo[20];     // zocoef
  double daAx[20];     // axcoef
  double daBx[20];     // bxcoef
  double daG1[20];     // g1coef
  double daG0[20];     // g0coef
  double dAlfa;        // alfabfld
  double dCdipol;      // cdipol
  double dBimf;        // bimf
  double dCimf;        // cimf
  double dDir;         // dirbfld
}; 

#define STATIC_CONST

class CMagFieldMparts
{
  // put private first
 private:
  CMagFieldOP *cpOP;    // Olson-Pfitzer external field

  CMagFieldT89 *cpT89;  // Tsyganenko 89/C external field

  int ciLogUnit; // should leave 0 unless debugging

  // Coordinate transforms
  bool cbSetRecalc2;                  // indicator for calling geigeo
  double cdModJulDate;                // modified julian date
  double cdDegToRad;                  // degrees to radians
  double cdCtseq, cdStseq, cdGst;     // cos, sin of GSE-GSEQ rot angle, Greenwich mean sidereal time
  double cdSlong, cdSrasn, cdSdec;    // solar long. along eclip, rt ascension, declin (radians)
  double cdaGeoToMag[9];              // coefficents for transformation GEOMAG
  double cdS1, cdS2, cdS3;            // sun position in GEI coords
  double cdY1, cdY2, cdY3;            // Y = (D x S)/|D x S|, D is dipole location in GEI coords
  double cdZ1, cdZ2, cdZ3;            // Z = S x Y
  double cdYB1, cdYB2, cdYB3;         // YB = ZB x S
  double cdZB1, cdZB2, cdZB3;         // coefficients for transformation GEIGSE (uses S, YB, ZB)
  double cdYC1, cdYC2, cdYC3;         // YC = (R x S)/|R x S|, R is rot axis of sun in GEI coords
  double cdZC1, cdZC2, cdZC3;         // ZC = S x YC
  double cdSps, cdCps, cdCt0, cdSl0;  // derived from IGR coefficents

  // Magnetic field calculations
#ifdef STATIC_CONST
  const int ciTerms = 11;           // Number of terms in fast IGRF SPHRC calculation
  const double cdEarthRad = 6371.2; // Earth radius in km
  const double cdErr = 0.0005;      // Scales the error limits for the integration intgrt2()
                                    // The error in L is approximately L * cdErr
  const double cdConi = 0.95;       // 
  const double cdMiner = 1.0;       // Minimum earth radius trace threshold
  const double cdP29 = 0.29289322;  // 1 - sqrt(0.5)
  const double cdOp7 = 1.70710678;  // 1 + sqrt(0.5)
#else
  int ciTerms;        // Number of terms in fast IGRF SPHRC calculation = 11
  double cdEarthRad;  // Earth radius in km
  double cdErr;       // Scales the error limits for the integration intgrt2()
                      // The error in L is approximately L * cdErr
  double cdConi;      // 
  double cdMiner;     // Minimum earth radius trace threshold
  double cdP29;       // 1 - sqrt(0.5)
  double cdOp7;       // 1 + sqrt(0.5)
#endif

  // Spherical harmonics coefficients for fast IGRF calculation
  double cdaCona[11]; // ciTerms=11
  double cdCon, cdR, cdAr, cdXr, cdXt, cdXp;
  double cdC2, cdC3, cdC4, cdC5, cdC6, cdC7, cdC8, cdC9, cdC10, cdC11;
  double cdCp2, cdCp3, cdCp4, cdCp5, cdCp6, cdCp7, cdCp8, cdCp9, cdCp10, cdCp11;
  double cdSp2, cdSp3, cdSp4, cdSp5, cdSp6, cdSp7, cdSp8, cdSp9, cdSp10, cdSp11;
  double cdP21, cdP22;
  double cdP31, cdP32, cdP33;
  double cdP41, cdP42, cdP43, cdP44;
  double cdP51, cdP52, cdP53, cdP54, cdP55;
  double cdP61, cdP62, cdP63, cdP64, cdP65, cdP66;
  double cdP71, cdP72, cdP73, cdP74, cdP75, cdP76, cdP77;
  double cdP81, cdP82, cdP83, cdP84, cdP85, cdP86, cdP87, cdP88;
  double cdP91, cdP92, cdP93, cdP94, cdP95, cdP96, cdP97, cdP98, cdP99;
  double cdP101, cdP102, cdP103, cdP104, cdP105, cdP106, cdP107, cdP108, cdP109, cdP1010;
  double cdP111, cdP112, cdP113, cdP114, cdP115, cdP116, cdP117, cdP118, cdP119, cdP1110, cdP1111;
  double cdDp21, cdDp22;
  double cdDp31, cdDp32, cdDp33;
  double cdDp41, cdDp42, cdDp43, cdDp44;
  double cdDp51, cdDp52, cdDp53, cdDp54, cdDp55;
  double cdDp61, cdDp62, cdDp63, cdDp64, cdDp65, cdDp66;
  double cdDp71, cdDp72, cdDp73, cdDp74, cdDp75, cdDp76, cdDp77;
  double cdDp81, cdDp82, cdDp83, cdDp84, cdDp85, cdDp86, cdDp87, cdDp88;
  double cdDp91, cdDp92, cdDp93, cdDp94, cdDp95, cdDp96, cdDp97, cdDp98, cdDp99;
  double cdDp101, cdDp102, cdDp103, cdDp104, cdDp105, cdDp106, cdDp107, cdDp108, cdDp109, cdDp1010;
  double cdDp111, cdDp112, cdDp113, cdDp114, cdDp115, cdDp116, cdDp117, cdDp118, cdDp119, cdDp1110,
         cdDp1111;

  // Control flags and miscellaneous state variables
  sMF_t csMF;

  // Variables for recalc2 and other methods
  sR2_t csR2;

  // Full IGRF state variables
  sFullIgrf_t csFull;

  // Fast IGRF state variables
  sFastIgrf_t csFast;

  // Variables for iterative calculation of adiabatic invariant
  int ciBeflg, ciIntflg, ciNmang, ciNlast, ciN2last, ciNc, ciIa, ciKs;
  double cdBmax, cdDelsv, cdDsl, cdRs, cdSer, cdSf, cdXdn, cdXds, cdXxj;
  double cdaB[3];
  double cdaB2[3];
  double cdaBll[3];
  double cdaBll2[3];
  double cdaBtmp[3];
  double cdaCtmp[3];
  double cdaX[3];
  double cdaX2[3];
  double cdaDen[100];
  double cdaDen2[100];
  double cdaS[100];
  double cdaS2[100];
  double cdaRsvlast[100];
  double cdaBb[100][4];
  double cdaBb2[100][4];
  sInvar_t csInvar;

  // Kp index
  double cdKpValue;

  // backwards compatability for computeAdInvariants "old" usage
  double cdRadThresh;  // default = 0.9, change to 1.0 when compat mode
  bool cbCompat;    // default = false

  // Internal methods for magnetic field calculations (retain FORTRAN subroutine name)
  eMAGFIELD_ERROR_CODE bmap( double dX, 
                             double dY, 
                             double dZ, 
                             double dSphhit,
                             double& dXhit, 
                             double& dYhit, 
                             double& dZhit, 
                             int& iNumPts,
                             double* daBmapX, 
                             double* daBmapY, 
                             double* daBmapZ,
                             int iDir, 
                             eBfieldApp eBfApp );

  eMAGFIELD_ERROR_CODE getigrf( double& dXgsm, 
                                double& dYgsm, 
                                double& dZgsm,
                                double& dBxgsm, 
                                double& dBygsm, 
                                double& dBzgsm );

  eMAGFIELD_ERROR_CODE tiltdipg( double& dXgsm, 
                                 double& dYgsm, 
                                 double& dZgsm,
                                 double& dBxgsm, 
                                 double& dBygsm, 
                                 double& dBzgsm );

  eMAGFIELD_ERROR_CODE spigrf( int iNmaxn, 
                               double& dAor, 
                               double& dBp, 
                               double& dBr, 
                               double& dBt,
                               double& dCph, 
                               double& dCt, 
                               double& dSph, 
                               double& dSt );

  eMAGFIELD_ERROR_CODE intgrt2( double* daX,
                                double* daB,
                                double(& daBb)[100][4],
                                double* daS,
                                double* daDen,
                                double& dDs,
                                int& iN );

  eMAGFIELD_ERROR_CODE bmnext2 ( double* daXx, 
                                 double* daB, 
                                 double& dBmag );

  eMAGFIELD_ERROR_CODE check( double* daPangle, 
                              int& iNmang);

  double adens( double* daX );

  eMAGFIELD_ERROR_CODE interp( double* daBb, 
                               double* daCc, 
                               double& dD, 
                               double& dE,
                               const int iJ );

  void stepsz( double* daX, 
               double* daBvec, 
               double(& daBb)[100][4], 
               int& iN, 
               double& dDs );

  void mglong( double(& daX)[100][3], 
               double* daS, 
               double& dSf,
               double& dXmlong, 
               double* daRmin, 
               double& dRmag );

  void invr( double& dBmax, 
             double(& daBb)[100][4], 
             double* daS, 
             int& iN ); 

  eMAGFIELD_ERROR_CODE bftrac( double dXa, 
                               double dYa, 
                               double dZa,
                               double& dXn, 
                               double& dYn, 
                               double& dZn, 
                               bool& bOpen,
                               double& dDx1, 
                               double& dDy1, 
                               double& dDz1, 
                               double& dBb,
                               double& dH, 
                               double dEp, 
                               double dEr );

  eMAGFIELD_ERROR_CODE eulert( double dPhi, 
                               double dTheta, 
                               double dPsi,
                               double dXp, 
                               double dYp, 
                               double dZp,
                               double& dX, 
                               double& dY, 
                               double& dZ );

  eMAGFIELD_ERROR_CODE interpmp( double dZi, 
                                 double dXa, 
                                 double dYa, 
                                 double dZa,
                                 double dXn, 
                                 double dYn, 
                                 double dZn,
                                 double& dXi, 
                                 double& dYi );

  eMAGFIELD_ERROR_CODE fastIgrfCoefs( double dDecimalYear, 
                                      CMagFieldIGRF *psIgrf );

  eMAGFIELD_ERROR_CODE fullIgrfCoefs( double dDecimalYear, 
                                      CMagFieldIGRF *psIgrf );

  eMAGFIELD_ERROR_CODE offsetDipoleCoefs( double dDecimalYear, 
                                          CMagFieldIGRF *psIgrf,
                                          double& dThetaN, 
                                          double& dPhiN );

  eMAGFIELD_ERROR_CODE tiltedDipoleCoefs( double dDecimalYear, 
                                          CMagFieldIGRF *psIgrf,
                                          double& dThetaN, 
                                          double& dPhiN );

  // Internal methods for coordinate transformations (retain FORTRAN subroutine name)
  eMAGFIELD_ERROR_CODE geigeo( double& dXgei, 
                               double& dYgei, 
                               double& dDgei,
                               double& dXgeo, 
                               double& dYgeo, 
                               double& dZgeo,
                               bool bForward=true );

  eMAGFIELD_ERROR_CODE geigse( double& dXgei, 
                               double& dYgei, 
                               double& dZgei,
                               double& dXgse, 
                               double& dYgse, 
                               double& dZgse,
                               bool bForward=true );

  eMAGFIELD_ERROR_CODE geigseq( double& dXgei, 
                                double& dYgei, 
                                double& dZgei,
                                double& dXgseq, 
                                double& dYgseq, 
                                double& dZgseq,
                                bool bForward=true );

  eMAGFIELD_ERROR_CODE geigsm( double& dXgei, 
                               double& dYgei, 
                               double& dZgei,
                               double& dXgsm, 
                               double& dYgsm, 
                               double& dZgsm,
                               bool bForward=true );

  eMAGFIELD_ERROR_CODE geimag( double& dXgei, 
                               double& dYgei, 
                               double& dZgei,
                               double& dXmag, 
                               double& dYmag, 
                               double& dZmag,
                               bool bForward=true );

  eMAGFIELD_ERROR_CODE geimll( double& dXgei, 
                               double& dYgei, 
                               double& dZgei,
                               double& dRadius, 
                               double& dDmlat, 
                               double& dHmlt,
                               bool bForward=true );

  eMAGFIELD_ERROR_CODE geism( double& dXgei, 
                              double& dYgei, 
                              double& dZgei,
                              double& dXsm, 
                              double& dYsm, 
                              double& dZsm,
                              bool bForward=true );

  eMAGFIELD_ERROR_CODE geomag( double& dXgeo, 
                               double& dYgeo, 
                               double& dZgeo,
                               double& dXmag, 
                               double& dYmag, 
                               double& dZmag,
                               bool bForward=true );

  eMAGFIELD_ERROR_CODE geomll( double& dXgeo, 
                               double& dYgeo, 
                               double& dZgeo,
                               double& dRadius, 
                               double& dDmlat, 
                               double& dHmlt,
                               bool bForward=true );

  eMAGFIELD_ERROR_CODE gsegeo( double& dXgse, 
                               double& dYgse, 
                               double& dZgse,
                               double& dXgeo, 
                               double& dYgeo, 
                               double& dZgeo,
                               bool bForward=true );

  eMAGFIELD_ERROR_CODE gsegseq( double& dXgse, 
                                double& dYgse, 
                                double& dZgse,
                                double& dXgseq, 
                                double& dYgseq, 
                                double& dZgseq,
                                bool bForward=true );

  eMAGFIELD_ERROR_CODE gsegsm( double& dXgse, 
                               double& dYgse, 
                               double& dZgse,
                               double& dXgsm, 
                               double& dYgsm, 
                               double& dZgsm,
                               bool bForward=true );

  eMAGFIELD_ERROR_CODE gsemag( double& dXgse, 
                               double& dYgse, 
                               double& dZgse,
                               double& dXmag, 
                               double& dYmag, 
                               double& dZmag,
                               bool bForward=true );

  eMAGFIELD_ERROR_CODE gsemll( double& dXgse, 
                               double& dYgse, 
                               double& dZgse,
                               double& dRadius, 
                               double& dDmlat, 
                               double& dHmlt,
                               bool bForward=true );

  eMAGFIELD_ERROR_CODE gsesm( double& dXgse, 
                              double& dYgse, 
                              double& dZgse,
                              double& dXsm, 
                              double& dYsm, 
                              double& dZsm,
                              bool bForward=true );

  eMAGFIELD_ERROR_CODE gsmgeo( double& dXgsm, 
                               double& dYgsm, 
                               double& dZgsm,
                               double& dXgeo, 
                               double& dYgeo, 
                               double& dZgeo,
                               bool bForward=true );

  void gsmsm( double& dXgsm, 
              double& dYgsm, 
              double& dZgsm,
              double& dXsm, 
              double& dYsm, 
              double& dZsm, 
              bool bForward=true );

  eMAGFIELD_ERROR_CODE magmll( double& dXmag, 
                               double& dYmag, 
                               double& dZmag,
                               double& dRadius, 
                               double& dDmlat, 
                               double& dHmlt,
                               bool bForward=true );

  eMAGFIELD_ERROR_CODE maggsm( double& dXmag, 
                               double& dYmag, 
                               double& dZmag,
                               double& dXgsm, 
                               double& dYgsm, 
                               double& dZgsm,
                               bool bForward=true );

  eMAGFIELD_ERROR_CODE magsm( double& dXmag, 
                              double& dYmag, 
                              double& dZmag,
                              double& dXsm, 
                              double& dYsm, 
                              double& dZsm,
                              bool bForward=true );

  eMAGFIELD_ERROR_CODE mllgsm( double& dRrr, 
                               double& dHmlt, 
                               double& dDmlat,
                               double& dXgsm, 
                               double& dYgsm, 
                               double& dZgsm,
                               bool bForward=true );

  eMAGFIELD_ERROR_CODE mllsm( double& dRrr, 
                              double& dDmlat, 
                              double& dHmlt,
                              double& dXsm, 
                              double& dYsm, 
                              double& dZsm,
                              bool bForward=true );

  eMAGFIELD_ERROR_CODE smgeo( double& dXsm, 
                              double& dYsm, 
                              double& dZsm,
                              double& dXgeo, 
                              double& dYgeo, 
                              double& dZgeo,
                              bool bForward=true );

  void vxyzsp( double& dTheta, 
               double& dPhi, 
               double& dVrad, 
               double& dVtheta, 
               double& dVphi,
               double& dVx, 
               double& dVy, 
               double& dVz, 
               bool bForward=true );

  void sun2( double& dGst, 
             double& dSlong, 
             double& dSrasn, 
             double& dSdec );

  // Magnetic field helper functions (constructed from fragments of 
  // FORTRAN subroutines to eliminate goto statements etc.)
  void initbf( int iUnitLog=0 );

  void spigrfn3( double& dAor, 
                 double& dBp, 
                 double& dBr, 
                 double& dBt,
                 double& dCph, 
                 double& dCt, 
                 double& dSph, 
                 double& dSt );

  void spigrfn4( double& dAor, 
                 double& dBp, 
                 double& dBr, 
                 double& dBt,
                 double& dCph, 
                 double& dCt, 
                 double& dSph, 
                 double& dSt );

  void spigrfn5( double& dAor, 
                 double& dBp, 
                 double& dBr, 
                 double& dBt,
                 double& dCph, 
                 double& dCt, 
                 double& dSph, 
                 double& dSt );

  void spigrfn6( double& dAor, 
                 double& dBp, 
                 double& dBr, 
                 double& dBt,
                 double& dCph, 
                 double& dCt, 
                 double& dSph, 
                 double& dSt );

  void spigrfn7( double& dAor, 
                 double& dBp, 
                 double& dBr, 
                 double& dBt,
                 double& dCph, 
                 double& dCt, 
                 double& dSph, 
                 double& dSt );

  void spigrfn8( double& dAor, 
                 double& dBp, 
                 double& dBr, 
                 double& dBt,
                 double& dCph, 
                 double& dCt, 
                 double& dSph, 
                 double& dSt );

  void spigrfn9( double& dAor, 
                 double& dBp, 
                 double& dBr, 
                 double& dBt,
                 double& dCph, 
                 double& dCt, 
                 double& dSph, 
                 double& dSt );

  void spigrfn10( double& dAor, 
                  double& dBp, 
                  double& dBr, 
                  double& dBt,
                  double& dCph, 
                  double& dCt, 
                  double& dSph, 
                  double& dSt );

  void spigrfn11( double& dAor, 
                  double& dBp, 
                  double& dBr, 
                  double& dBt,
                  double& dCph, 
                  double& dCt, 
                  double& dSph, 
                  double& dSt );

  void smooth10( double& dBr, 
                 double& dBt, 
                 double& dBp, 
                 double& dCon,
                 double dSt, 
                 double dR, 
                 int iN );

  void smooth11( double& dBr, 
                 double& dBt, 
                 double& dBp, 
                 double dCon, 
                 double dSt );

  void smooth20( double& dBr, 
                 double& dBt, 
                 double& dBp, 
                 double& dCon,
                 double dSt, 
                 double dR, 
                 int iN );

  void smooth21( double& dBr, 
                 double& dBt, 
                 double& dBp, 
                 double dCon, 
                 double dSt );

  void feldcParta( int iIl, 
                   int iIh, 
                   int iI, 
                   double& dX, 
                   double& dY, 
                   double& dZ );

  void feldcPartb( int iIl, 
                   int iIh, 
                   double& dX, 
                   double& dY, 
                   double& dZ );

  void feldcPartc( int iIl, 
                   int iIh, 
                   double& dX, 
                   double& dY, 
                   double& dZ );

  int changeStepSize( int& iN, 
                      int& iNumang, 
                      double& dDs, 
                      double* daBmaxan, 
                      double* daDensty,
                      double* daXj, 
                      double* daEl );

  int secondInvariant( int& iN, 
                       int& iNumang, 
                       double& dDs, 
                       double* daBmaxan, 
                       double* daDensty,
                       double* daXj, 
                       double* daEl );

  int integrate2ndPart( int& iN, 
                        int& iNumang, 
                        double& dDs, 
                        double* daBmaxan, 
                        double* daDensty,
                        double* daXj, 
                        double* daEl ); 

  int outerLoop( int& iN, 
                 int& iNumang, 
                 double& dDs, 
                 double* daBmaxan, 
                 double* daDensty,
                 double* daXj, 
                 double* daEl );

  int innerLoop( int& iN, 
                 int& iNumang, 
                 double& dDs, 
                 double* daBmaxan, 
                 double* daDensty, 
                 double* daXj,
                 double* daEl );

  int getInvariant( int& iN );

  int getLvalues( int& iNumang, 
                  int& iNmang, 
                  double* daBmaxan, 
                  double* daDensty, 
                  double* daXj,
                  double* daEl );

  int positionOfMin( double(& daX)[100][3], 
                     double dX2, 
                     double dX3, 
                     double dDd, 
                     double dXf,
                     double* daXt, 
                     double* daRmin );

  int centeredDipole( double* daXt, 
                      double& dXp, 
                      double& dYp );

  int magneticLongitude( double dXp, 
                         double dYp, 
                         double& dXmlong );

  int bmapClosed( bool bOpen, 
                  double dXa, 
                  double dYa, 
                  double dZa, 
                  double dXn, 
                  double dYn,
                  double dZn, 
                  double& dXhit, 
                  double& dYhit, 
                  double& dZhit );

  void getBmagAndUnitVectors( double dBx, 
                              double dBy, 
                              double dBz, 
                              double& dBmag,
                              double& dBxu, 
                              double& dByu, 
                              double& dBzu );


  // Miscellaneous helper functions
  void logger( string sLogMsg );

  int a2v( int iIndex1, 
           int iIndex2, 
           int iSize );

  int getbc( double(& daBb)[100][4], 
             double* daS, 
             int& iN, 
             double* daBtmp, 
             double* daCtmp );

 public:
  // Constructor
  CMagFieldMparts();

  // Destructor
  ~CMagFieldMparts();

  // #### compatibility mode for computeAdInvariants "old" version
  void setCompatMode( bool bState=true );

  // Sets and gets (expose only as required by CMagFieldLib)
  double getEradKm() { return csMF.dEradKm; }

  void setFootRadius( double dIn ) { csMF.dFootRadius = dIn; }
  double getFootRadius() { return csMF.dFootRadius; }

  double getWdst() { return csMF.dWdst; }

  double getWeqedg() { return csMF.dWeqedg; }

  void setInitialized( bool bVal ) { csMF.bMagFieldInitialized = bVal; }
  bool getInitialized() { return csMF.bMagFieldInitialized; }

  void setMainFieldModel( int iVal )
    { csMF.iMainFieldModel = eMainField(iVal); }
  int getMainFieldModel() { return int(csMF.iMainFieldModel); }

  void setExternalFieldModel( int iVal )
    { csMF.iExternalFieldModel = eExternalField(iVal); }
  int getExternalFieldModel() { return int(csMF.iExternalFieldModel); }

  void setDBFieldLineTraceDir( int iVal )
    { csMF.iFieldLineTraceDir = eFieldLineTraceDir(iVal); }
  int getDBFieldLineTraceDir() { return int(csMF.iFieldLineTraceDir); }

  void setBFieldOutput( int iVal ) { csMF.iBFieldOutput = iVal; }
  int getBFieldOutput() { return csMF.iBFieldOutput; }

  void setUnitLog( int iVal ) { csMF.iUnitLog = iVal; }
  int getUnitLog() { return csMF.iUnitLog; }

  void setUnitBfout( int iVal ) { csMF.iUnitBfout = iVal; }
  int getUnitBfout() { return csMF.iUnitBfout; }

  void setUnitHitout( int iVal ) { csMF.iUnitHitout = iVal; }
  int getUnitHitout() { return csMF.iUnitHitout; }

  double getDyinyr() { return csR2.dDyinyr; }

  double getRedist() { return csR2.dRedist; }

  double getModJulDate() { return csR2.dModJulDate; }

  void setDecimalYear( double dIn ) { csR2.dDecimalYear = dIn; }

  void setTiltMode( int iVal ) { csR2.iTiltMode = iVal; }

  double getTilt() { return csR2.dTilt; }

  double getSR2XM() { return csR2.dXM; }

  double getSR2Dimo() { return csR2.dDimo; }

  double getThetaN() { return csR2.dThetaN; }

  double getPhiN() { return csR2.dPhiN; }

  double getSR2Eta() { return csR2.dEta; }

  double getSR2Zeta() { return csR2.dZeta; }

  double getSR2Xi() { return csR2.dXi; }

  double getSFullDimo() { return csFull.dDimo; }

  double getSFullXM() { return csFull.dXM; }

  double getSFastDimo() { return csFast.dDimo; }

  double getSFastXM() { return csFast.dXM; }

  double getReDist() { return csR2.dRedist; }

  void setXmdimo( double dDimo ) { csInvar.dXmdimo = dDimo; }

  void setKpValue( double dKpVal ) { cdKpValue = dKpVal; }
  double getKpValue() { return cdKpValue; }

  /////////////////////////////////////////////////////////////////////////////
  // Coordinate Transformation APIs (retain FORTRAN subroutine names)
  // See CMagFieldMparts.cpp for doxygen prologs

  // Convert coordinates from type 1 to type 2
  eMAGFIELD_ERROR_CODE crdtr( eCoordType iC1, 
                              eCoordType iC2,
                              double* daXi, 
                              double* daXo );

  // Calculate dipole GEO to MAG conversion matrix
  void calcGeoToMag( double& dTheta, 
                     double& dPhi );

  // Convert between GEO cartesian and MAG cartesion
  void geo2magto( double& dXgeo, 
                  double& dYgeo, 
                  double& dZgeo,
                  double& dXmag, 
                  double& dYmag, 
                  double& dZmag, 
                  bool bForward=true );

  // Convert between cartesian and spherical coordinates
  eMAGFIELD_ERROR_CODE xyzsph( double& dX, 
                               double& dY, 
                               double& dZ,
                               double& dRadius, 
                               double& dTheta, 
                               double& dPhi,
                               bool bForward=true );

  // Determine dipole tilt angle and establish coordinate transformation
  // matrix coefficients. Must be called once for each time of interest
  void recalc2( double dG10, 
                double dG11, 
                double dH11 );

  /////////////////////////////////////////////////////////////////////////////
  // Magnetic field calculation APIs (retain FORTRAN subroutine names)
  // See CMagFieldMparts.cpp for doxygen prologs

  // Magnetic field line footprint
  eMAGFIELD_ERROR_CODE bffoot( double dRadius, 
                               double dMlat, 
                               double dMlt,
                               double dThWidth, 
                               int iPts, 
                               int iPtmax,
                               double* daXfoot, 
                               double* daYfoot, 
                               double* daZfoot );

  // Magnetic field unit vector and field magnitude
  eMAGFIELD_ERROR_CODE bffunc( double& dX, 
                               double& dY, 
                               double& dZ,
                               double& dBx, 
                               double& dBy, 
                               double& dBz,
                               double& dDx, 
                               double& dDyy, 
                               double& dDz,
                               double& dB, 
                               bool& bOpen);

  // Magnetic coordinates of particle drift shell, first and second adiabatic invariants,
  // L parameter
  eMAGFIELD_ERROR_CODE invar2( double& dXlat, 
                               double& dXlong, 
                               double& dR,
                               double* daPangle, 
                               int& iNumang, 
                               double* daEl,
                               double* pdBlocal, 
                               double* pdBmin, 
                               double* pdXmlong,
                               double* pdXmlat, 
                               double* daBmaxan, 
                               double* daXj,
                               double* daDensty, 
                               double* daBvec, 
                               int& iN, 
                               double& dDs,
                               double* daRmin );

  // Calculate earth magnetic field from spherical harmonics model
  void feldc( double* daV, 
              double* daB );

  // Get SPIGRF magnetic field in GSM coordinates
  eMAGFIELD_ERROR_CODE gtspig( double& dXgsm, 
                               double& dYgsm, 
                               double& dZgsm,
                               double& dBxgsm, 
                               double& dBygsm, 
                               double& dBzgsm );

  // Calculate the L value
  void hiltel( double& dB, 
               double& dXi, 
               double& dVl );

  // Calculate field line crossing point at Earth surface
  eMAGFIELD_ERROR_CODE land( double dXa, 
                             double dYa, 
                             double dZa,
                             double dXn, 
                             double dYn, 
                             double dZn, 
                             double dSphhit,
                             double& dXrad, 
                             double& dYrad, 
                             double& dZrad );

  // Calculate GEO components of tilted dipole field
  void tiltdip( double& dXgeo, 
                double& dYgeo, 
                double& dZgeo,
                double& dBxgeo, 
                double& dBygeo, 
                double& dBzgeo );

  // Update IGRF coefficients
  eMAGFIELD_ERROR_CODE updateIGRFCoefs( double dDecimalYear, 
                                        CMagFieldIGRF *psIgrf,
                                        double* cdaG2M );

  // Update recalc2 parameters
  void updateSR2Params( double dDecimalYear, 
                        double dModJulDate, 
                        CMagFieldIGRF *psIgrf );

  // Initialize variables
  eMAGFIELD_ERROR_CODE initializeMparts();

  // Calculate coordinates of magnetic field line footprint
  eMAGFIELD_ERROR_CODE findFootPrint( double dXgsm, 
                                      double dYgsm, 
                                      double dZgsm, 
                                      int& iNumPts,
                                      double* daXhit, 
                                      double* daYhit, 
                                      double* daZhit );

  // Trace magnetic field line
  eMAGFIELD_ERROR_CODE traceFieldLine( double dXgsm, 
                                       double dYgsm, 
                                       double dZgsm, 
                                       int& iNumPts,
                                       double* daBmapX, 
                                       double* daBmapY, 
                                       double* daBmapZ );

  // Coordinate transformation, km to Earth radii
  eMAGFIELD_ERROR_CODE kmToRe( const emfCoordSys& emfCoordIn, 
                               emfCoordSys& emfCIn,
                               double* daInput );

  // Coordinate transformation, Earth radii to km
  eMAGFIELD_ERROR_CODE reToKm( emfCoordSys& emfCIn, 
                               emfCoordSys& emfCOut,
                               double* daOutput );
};
#endif  // CMagFieldMparts_H
