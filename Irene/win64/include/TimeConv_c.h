/******************************************************************************
File: TimeConv_c.h

Description: C-language Implementation of methods for the conversion of time values
   to and from Modified Julian Dates.
   (these are direct routines, rather than wrappers, of their C++ versions)

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

  Version      Date       Notes
  1.0          1/03/2019  created

******************************************************************************/

#ifndef TIMECONV_C_H
#define TIMECONV_C_H

#ifdef __cplusplus
extern "C" {
#endif

// utilities to convert *to* MJD values
double TimeConvGetGmtSeconds( const int iHours, // 0-23
                              const int iMinutes, // 0-59
                              const double dSeconds ); // 0-59.999
int TimeConvGetDayOfYear( const int iYear, // 1901-2099
                          const int iMonth, // 1-12
                          const int iDay ); // 1-28/29/30/31
double TimeConvGetModifiedJulianDate( const int iYear, // 1901-2099
                                      const int iDdd,  // 1-365/366
                                      const double dGmtsec ); // 0-86399.999
double TimeConvGetModifiedJulianDateNow( );
double TimeConvGetModifiedJulianDateUnix( const int iUnixTime ); // 0-MAXINT
//  XSD date/time string format: "<YYYY>-<MM>-<DD>T<hh>:<mm>:<ss>[.ssss][Z]"
double TimeConvGetModifiedJulianDateXsd( char *szXsdTimeConv );

  // utilities to convert *from* MJD values
bool TimeConvisLeapYear( const int iYear );
  //   -secondary versions with pointers are for ease of wrapper interfacing
int TimeConvGetDateTime( const double dModJulDate, // 15385.0 - 88068.999
                         int* piYear, // 1901-2099
                         int* piDdd, // 1-365/366
                         double* pdGmtsec ); // 0-86399.999
int TimeConvGetHoursMinSec( const double dGmtsec, // 0-86399.999
                            int* piHours, // 0-23
                            int* piMinutes, // 0-59
                            double* pdSeconds ); // 0-59.999
int TimeConvGetMonthDay( const int iYear, // 1901-2099
                         const int iDdd, // 1-365/366
                         int* piMonth, // 1-12
                         int* piDay ); // 1-28/29/30/31

// utilities to convert *to* Composite values
double TimeConvGetCompositeTime( const int iYear, // 1901-2099
                                 const int iDdd,  // 1-365/366
                                 const double dGmtsec ); // 0-86399.999
double TimeConvGetCompositeTimeFromMJD( const double dModJulDate ); // 15385.0 - 88068.999

// utilities to convert *from* Composite values
int TimeConvGetDateTimeFromComposite( const double dCompositeTime,
                                      int* piYear, // 1901-2099
                                      int* piDdd, // 1-365/366
                                       double* pdGmtsec ); // 0-86399.999
double TimeConvGetMJDFromComposite( const double dCompositeTime );


#ifdef __cplusplus
} // extern "C"
#endif

#endif  // TIMECONV_C_H
