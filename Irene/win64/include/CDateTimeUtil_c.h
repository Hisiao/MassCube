/******************************************************************************
$HeadURL$

 File: CDateTimeUtil.h

 Description: Declaration for methods for the conversion of time values to and
   from Modified Julian Dates.

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

#ifndef CDATETIMEUTIL_C_H
#define CDATETIMEUTIL_C_H

#ifdef __cplusplus
#include "CDateTimeUtil.h"

extern "C" {
#endif

// utilities to convert *to* MJD values
double DateTimeGetGmtSeconds ( const int iHours, // 0-23
                               const int iMinutes, // 0-59
                               const double dSeconds ); // 0-59.999
int DateTimeGetDayOfYear ( const int iYear, // 1950-2049
                           const int iMonth, // 1-12
                           const int iDay ); // 1-28/29/30/31
double DateTimeGetModifiedJulianDate ( const int iYear, // 1950-2049
                                       const int iDdd,  // 1-365/366
                                       const double dGmtsec ); // 0-86399.999
double DateTimeGetModifiedJulianDateUnix ( const int iUnixTime ); // 0-MAXINT
//  XSD date/time string format: "<YYYY>-<MM>-<DD>T<hh>:<mm>:<ss>[.ssss][Z]"
double DateTimeGetModifiedJulianDateXsd ( char *szXsdDateTime );

  // utilities to convert *from* MJD values
int DateTimeisLeapYear( const int iYear );
  //   -secondary versions with pointers are for ease of wrapper interfacing
int DateTimeGetDateTime ( const double dModJulDate, // 33282.0 - 69806.999
                          int* piYear, // 1950-2049
                          int* piDdd, // 1-365/366
                          double* pdGmtsec ); // 0-86399.999
int DateTimeGetHoursMinSec ( const double dGmtsec, // 0-86399.999
                             int* piHours, // 0-23
                             int* piMinutes, // 0-59
                             double* pdSeconds ); // 0-59.999
int DateTimeGetMonthDay ( const int iYear, // 1950-2049
                          const int iDdd, // 1-365/366
                          int* piMonth, // 1-12
                          int* piDay ); // 1-28/29/30/31


#ifdef __cplusplus
} // extern "C"
#endif

#endif  // CDATETIMEUTIL_C_H
