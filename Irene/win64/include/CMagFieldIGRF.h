/***********************************************************************
 File: CMagFieldIGRF.h
 
 Description:

   Declarations for IGRF data table interface methods.

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
#ifndef CMAGFIELDIGRF_H
#define CMAGFIELDIGRF_H
//
// Interface to access and store IGRF data tables
//

#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>

#include "CMagFieldErrors.h"
#include "GenericModel.h"

using namespace std;

#ifdef _WIN32
#pragma warning ( disable : 4099 ) // class ref'd as struct
#endif

typedef struct sCoeffNode {
  int iYear;
  int iSize;
  int iNmax;
  dvector dvGh;
} sCoeffNode;

class CMagFieldIGRF
{
  // put private first
 private:
   int ciMinYear;
   int ciMaxYear;
   bool cbCoeffInit;
   vector<sCoeffNode> cvsCoeffNode;
   Hdf5ToolKit* cpHdfDb;
   
   // implementation of convrt fast igrf normalization routine
   eMAGFIELD_ERROR_CODE fastNormalize( double& g, int& i, int& l, 
                       const int& k, const int& nMax, double* s);
 
   eMAGFIELD_ERROR_CODE getDBint( string &sName, int& iVal );
   eMAGFIELD_ERROR_CODE getDBfloat( string &sName, fvector &fArray, int& iLen );
   eMAGFIELD_ERROR_CODE getDBdouble( string &sName, dvector &fArray, int& iLen );

 public:
  // Constructor
  CMagFieldIGRF();
  // Destructor
  ~CMagFieldIGRF();

 eMAGFIELD_ERROR_CODE initialize( Hdf5ToolKit* pHdfDb);

 int getMinYear( ) { return ciMinYear; };
 int getMaxYear( ) { return ciMaxYear; };
 
// get list of available years
 eMAGFIELD_ERROR_CODE getIGRFyears( fvector& fvYears );

 // get igrf table for a year, one g, h array
 eMAGFIELD_ERROR_CODE getIGRFyearFull( const double& dYear,
                                       dvector& daGh,
                                       int& iSize,
                                       int& iNmax );

// Fast igrf matrix is stored in a matrix
// (stored in a matrix iSize = (iMaxn+1) *(iMaxn+1) if iNmax < iMaxn, iNmax is truncated
// g(m,n) = g n, m  (n=1,iNmax)(m=1,iNmax) 
// g(n,m-1) = h n,m (n=1,iNmax)(m=1,iNmax), m>0
 eMAGFIELD_ERROR_CODE getIGRFyearFast( const double& dYear,  
                                              // year to return
                                       const int& iMaxn,  
                                              // Max degree to return 
                                              // (typically 10 for fast igrf)
                                              // should be iMatrixSize -1;
                                       const int& iMatrixSize,  
                                              // size input matrix is 
                                              // iMatrixSize * iMatrixSize
                                       dvector& daGh, //  matrix array to return 
                                              // varies in, size = iMatrixSize 
                                              // * iMatrixSize
                                       int& iNmax, // number of values to 
                                              // returned (min(iMaxn,iNmax)+1)
                                       const int& normalize //0-none, 
                                              // 1-fast igrf, 2-other?
                                        );

 // setup fast igrf g/h values
 eMAGFIELD_ERROR_CODE getIGRFCoefFast( const int& iMatrixSize, 
                                       const int& iMaxdegree, 
                                       const int& normalize, 
                                       const double& dYear, 
                                       double* daGh, 
                                       int& iNmax, 
                                       double& dimo, 
                                       double& dXm );
                                 
 // setup full igrf g/h values
 eMAGFIELD_ERROR_CODE getIGRFCoefFull( const int& normalize, 
                                       const double& dYear, 
                                       double* daGh, 
                                       int& iNmax, 
                                       double& dimo, 
                                       double& dXm );

// locate saved coefficient node for specific year
 int findCoeffNode( const int& iYear );

// get vectored data for a year
 eMAGFIELD_ERROR_CODE getIGRFCoefRawDB( const int& iYear, 
                                        dvector& dvGh, 
                                        int& iSize, 
                                        int& iNmax ); 

}; // end class


#endif  // CMAGFIELDIGRF_H
