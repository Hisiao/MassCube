/***********************************************************************

Module: CShieldDoseDb.h

Description: Class definition for CShieldDoseDb object

 Encapsulates the HDF5 format database storage of
 CShieldDose2 data

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
     1.0       Tue Oct 7, 2008   First release

------------------------------------------------------------------------

***********************************************************************/
#ifndef SHIELDDOSEDB_H
#define SHIELDDOSEDB_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;


#include "Hdf5ToolKit.h"
#include "GenericModelEnum.h"

//-------------------------------------------------------------------
// Defines, typedefs and enumerations
//-------------------------------------------------------------------

typedef vector<double> dvector;         // 1D array of double
typedef vector<dvector>    vdvector;    // 2D array of double
typedef vector<vdvector>   vvdvector;   // 3D array of double
typedef vector<vvdvector>  vvvdvector;  // 4D array of double


//-------------------------------------------------------------------
// class declarations
//
// CShieldDoseDb - main db interface
//-------------------------------------------------------------------

class CShieldDoseDb
{
 public:
  CShieldDoseDb();
  ~CShieldDoseDb();

  eGENERIC_ERROR_CODE read( const string& strDBName );
  eGENERIC_ERROR_CODE read( Hdf5ToolKit* pDb );

  // public accessors

  //        proton data

  eGENERIC_ERROR_CODE getProtonSizes( int* pMMax, int* pKMax, int* pNMax,
              int* pLMax, int* pIMix );
  eGENERIC_ERROR_CODE setProtonSizes( int iMMax, int iKMax, int iNMax,
              int iLMax, int iIMix );

  dvector&   getProtonEP()    { return cdvProtonEP; }
  dvector&   getProtonRP()    { return cdvProtonRP; }
  dvector&   getProtonTEPN()  { return cdvProtonTEPN; }
  dvector&   getProtonFEPN()  { return cdvProtonFEPN; }
  dvector&   getProtonTP()    { return cdvProtonTP; }
  dvector&   getProtonZRP()   { return cdvProtonZRP; }
  vvdvector& getProtonDALP()  { return cvvdvProtonDALP; }
  vvdvector& getProtonDRATP() { return cvvdvProtonDRATP; }

  //        electron data

  eGENERIC_ERROR_CODE getElectronSizes( int* pMMax, int* pNMax, int* pLMaxS,
          int* pLMaxE, int* pLMaxT, int* pLMaxB,
          int* pIMix );
  eGENERIC_ERROR_CODE setElectronSizes( int iMMax, int iNMax, int iLMaxS,
          int iLMaxE, int iLMaxT, int iLMaxB,
          int iIMix );

  dvector&    getElectronEE()    { return cdvElectronEE; }
  dvector&    getElectronRE()    { return cdvElectronRE; }
  dvector&    getElectronYE()    { return cdvElectronYE; }
  dvector&    getElectronTE()    { return cdvElectronTE; }
  dvector&    getElectronAR()    { return cdvElectronAR; }
  dvector&    getElectronRS()    { return cdvElectronRS; }
  dvector&    getElectronBS()    { return cdvElectronBS; }
  dvector&    getElectronZRE()   { return cdvElectronZRE; }
  dvector&    getElectronZS()    { return cdvElectronZS; }
  dvector&    getElectronZB()    { return cdvElectronZB; }
  vdvector&   getElectronDALE()  { return cvdvElectronDALE; }
  vdvector&   getElectronDALB()  { return cvdvElectronDALB; }
  vvvdvector& getElectronDRATE() { return cvvvdvElectronDRATE; }
  vvvdvector& getElectronDRATB() { return cvvvdvElectronDRATB; }

 protected:

  // proton data

  //    vector sizes
  int ciProtonMMax;
  int ciProtonKMax;
  int ciProtonNMax;
  int ciProtonLMax;
  int ciProtonIMix;

  //    vectors
  dvector cdvProtonEP;        // [MMax]
  dvector cdvProtonRP;        // [MMax]
  dvector cdvProtonTEPN;      // [KMax]
  dvector cdvProtonFEPN;      // [KMax]
  dvector cdvProtonTP;        // [NMax]
  dvector cdvProtonZRP;       // [LMax]
  vvdvector cvvdvProtonDALP;  // [2][LMax][NMax]
  vvdvector cvvdvProtonDRATP; // [IMix][LMax][NMax]

  // electron data

  //    vector sizes
  int ciElectronMMax;
  int ciElectronNMax;
  int ciElectronLMaxS;
  int ciElectronLMaxE;
  int ciElectronLMaxT;
  int ciElectronLMaxB;
  int ciElectronIMix;

  //    vectors
  dvector cdvElectronEE;          // [MMax]
  dvector cdvElectronRE;          // [MMax]
  dvector cdvElectronYE;          // [MMax]
  dvector cdvElectronTE;          // [NMax]
  dvector cdvElectronAR;          // [NMax]
  dvector cdvElectronRS;          // [NMax]
  dvector cdvElectronBS;          // [LMaxS]
  dvector cdvElectronZRE;         // [LMaxE]
  dvector cdvElectronZS;          // [LMaxT]
  dvector cdvElectronZB;          // [LMaxB]
  vdvector cvdvElectronDALE;      // [LMaxS][NMax]
  vdvector cvdvElectronDALB;      // [LMaxT][NMax]
  vvvdvector cvvvdvElectronDRATE; // [IMix][2][LMaxE][NMax]
  vvvdvector cvvvdvElectronDRATB; // [IMix][2][LMaxB][NMax]

};

#endif
