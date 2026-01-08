/******************************************************************************
File: CTimeValue.h

Description: Class defining static helper methods for 
converting various time values to/from many different forms/components.
Due to simplified conversion routines used for the Modified Julian Date,
these time conversions are currently limited to the 1901-2099 year range.

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
  1.0          2/26/2013 created
  
******************************************************************************/

#ifndef CTIMEVALUE_H
#define CTIMEVALUE_H

#include <string>
#include <time.h>
using namespace std;

/**
 * @brief CTimeValue: time specification and manipulation class
 *
 * The CTimeValue class may be used as a set of 'static' methods for the conversion 
 * of date and time values between different formats, or used as class object to 
 * store and manipulate the date and time, and output the value in any supported 
 * form. <br>
 * (Some classes of the SpWx Library still use 'GeoSpaceTime' objects (described elsewhere); <br>
 *  Use of that class is strongly *discouraged* for any new development.) <br>
 *
 * Time Format Definitions:
 * * Modified Julian Date = Number of days(+fraction) since 18 Nov 1858 at 0000GMT
 * * Unix Time = Number of seconds since 01 Jan 1970 0000GMT
 * * Composite Time = <YYYY><DDD><GMTseconds.frac>  (double)  [equivalent to GeoSpaceTime]
 * * XSD Format = "<YYYY>-<MM>-<DD>T<hh>:<mm>:<ss>"  (string)
 *
 * The time formats supported by the CTimeValue Class are shown below,
 * and is valid between 01 Jan 1901 and 31 Dec 2099 (except Unix time)
 * | Time Specification | Limits |
 * | :------------------------- | :--------------------- |
 * | Modified Julian Date |   MJD = 15385.0 - 88068.9999    |
 * | Year, Day of Year+Fraction | Year = 1901-2099, Day = 1-365/366(+fraction) |
 * | Year, Day of Year, GMTseconds | GMTseconds = 0.0 - 86399.999 |
 * | Year, Month, Day of Month, GMTseconds | Month = 1-12, day of month = 1-28/29/30/31 |
 * | Year, Month, Day, Hour, Minute, Seconds | Hour = 0-23, Minute = 0-59, Second = 0-59 |
 * | Composite Time (nee *GeoSpaceTime*) | 190100100000.0 - 209936586399.99 |
 * | Unix Time | 0 - 2147483647 (Jan 1970 - Jan 2038)|
 * | XSD Time String | same as Year, Month, Day |
 *
 */


class CTimeValue
{
  public:

    // methods using class state variables

/*------------------------------------------------------------------*/
// function CTimeValue::CTimeValue
/**
*
* Default Constructor
*/
    CTimeValue();

/*------------------------------------------------------------------*/
// function CTimeValue::CTimeValue
/**
*
* Constructor, setting initial value from inputs
*
*    @param iYear
*           int: year value (valid range = 1901-2099)
*    @param iMonth
*           int: month value (valid range 1-12)
*    @param iDay
*           int: day value (valid range 1-f(month))
*    @param dGmtsec
*           double: GMT seconds of day (normal range 0-86400)
*/
    CTimeValue( const int& iYear,
                const int& iMonth,
                const int& iDay,
                const double& dGmtsec );

/*------------------------------------------------------------------*/
// function CTimeValue::CTimeValue
/**
*
* Constructor, setting initial value from inputs
*
*    @param iYear
*           int: year value (valid range = 1901-2099)
*    @param iDdd
*           int: day of year value (valid range 1-365/366)
*    @param dGmtsec
*           double: GMT seconds of day (normal range 0-86400)
*/
    CTimeValue( const int& iYear,
                const int& iDdd,
                const double& dGmtsec );

/*------------------------------------------------------------------*/
// function CTimeValue::CTimeValue
/**
*
* Constructor, setting initial value from Modified Julian Date, or 'Composite' time inputs
*
*    @param dTimeValue
*           double: Modified Julian Date: days.frac from 11/17/1858 0000 GMT <br>
*            or 'composite' time value  (YYYYDDDGMTsc.fr format)
*/
    CTimeValue( const double& dTimeValue );

/*------------------------------------------------------------------*/
// function CTimeValue::CTimeValue
/**
*
* Constructor, setting initial value from Unix time inputs
*
*    @param tTimeValue
*           time_t: Unix time, number of seconds since 00:00:00 UTC, January 1, 1970
*/
    CTimeValue( const time_t& dTimeValue );

/*------------------------------------------------------------------*/
// function CTimeValue::CTimeValue
/**
*
* Constructor, setting initial value from Unix time inputs
*
*    @param iTimeValue
*           int: Unix time, number of seconds since 00:00:00 UTC, January 1, 1970
*/
    CTimeValue( const int& iTimeValue );

/*------------------------------------------------------------------*/
// function CTimeValue::CTimeValue
/**
*
* Constructor, setting initial value from XSD date/time string
*
*    @param strXsdDateTime
*           string: XSD date/time string format: "<YYYY>-<MM>-<DD>T<hh>:<mm>:<ss>[.ssss][Z]"
*/
    CTimeValue( const string& strTimeValue );

/*------------------------------------------------------------------*/
// function CTimeValue::~CTimeValue
/**
*
* Destructor
*/
    ~CTimeValue();

/*------------------------------------------------------------------*/
// function CTimeValue::setTime
/**
*
* Set state time value to current system time
*
*    @returns
*           int: 0 success, else error code 
*/
    int setTime();

/*------------------------------------------------------------------*/
// function CTimeValue::setTime
/**
*
* Set state time value from year, month, day_of_month, gmtsec values
*
*    @param iYear
*           int: year value (valid range = 1901-2099)
*    @param iMonth
*           int: month value (valid range 1-12)
*    @param iDay
*           int: day value (valid range 1-f(month))
*    @param dGmtsec
*           double: GMT seconds of day (normal range 0-86400)
*
*    @returns
*           int: 0 success, else error code 
*/
    int setTime( const int& iYear,
                 const int& iMonth,
                 const int& iDay,
                 const double& dGmtsec );

/*------------------------------------------------------------------*/
// function CTimeValue::setTime
/**
*
* Set state time value from year, day_of_year, gmtsec values
*
*    @param iYear
*           int: year value (valid range = 1901-2099)
*    @param iDdd
*           int: day of year value (valid range 1-365/366)
*    @param dGmtsec
*           double: GMT seconds of day (normal range 0-86400)
*
*    @returns
*           int: 0 success, else error code 
*/
    int setTime( const int& iYear,
                 const int& iDdd,
                 const double& dGmtsec );

/*------------------------------------------------------------------*/
// function CTimeValue::setTime
/**
*
* Set state time value from MJD or 'composite' time values
*
*    @param dTimeValue
*           double: Modified Julian Date: days.frac from 11/17/1858 0000 GMT <br>
*            or 'composite' time value  (YYYYDDDGMTsc.fr format)
*
*    @returns
*           int: 0 success, else error code 
*/
    int setTime( const double& dTimeValue ); // MJD or Composite

/*------------------------------------------------------------------*/
// function CTimeValue::setTime
/**
*
* Set state time value from Unix time value
*
*    @param tTimeValue
*           time_t: Unix time, number of seconds since 00:00:00 UTC, January 1, 1970
*
*    @returns
*           int: 0 success, else error code 
*/
    int setTime( const time_t& tTimeValue ); // Unix time (sec from 01 Jan 1970 0000UT)

/*------------------------------------------------------------------*/
// function CTimeValue::setTime
/**
*
* Set state time value from integer Unix time value
*
*    @param iTimeValue
*           int: Unix time, number of seconds since 00:00:00 UTC, January 1, 1970
*
*    @returns
*           int: 0 success, else error code 
*/
    int setTime( const int& iTimeValue ) { return setTime( (time_t)iTimeValue ); }

/*------------------------------------------------------------------*/
// function CTimeValue::setTime
/**
*
* Set state time value from XSD time string value (assumed to be GMT)
*
*    @param strXsdDateTime
*           string: XSD date/time string format: "<YYYY>-<MM>-<DD>T<hh>:<mm>:<ss>[.ssss][Z]"
*
*    @returns
*           int: 0 success, else error code 
*/
    int setTime( const string& strXsdDateTime ); // XSD date/time in string format
        //  "<YYYY>-<MM>-<DD>T<hh>:<mm>:<ss>[.ssss][Z]", always GMT
        //ie "2016-08-31T14:54:24"; the fraction of seconds, and trailing Z are optional

    //  modify current time

/*------------------------------------------------------------------*/
// function CTimeValue::adjustTime
/**
*
* Apply a time adjustment, specified in days and seconds, to state time value 
*
*    @param iDays
*           int: Number of days to adjust time value by (pos or neg)
*    @param dSeconds
*           double: Number of seconds to adjust time value by (pos or neg)
*
*    @returns
*           int: 0 success, else error code 
*/
    int adjustTime( const int& iDays,
                    const double& dSeconds );

/*------------------------------------------------------------------*/
// function CTimeValue::adjustTime
/**
*
* Apply a time adjustment, specified in seconds, to state time value 
*
*    @param dSeconds
*           double: Number of seconds to adjust time value by (pos or neg)
*
*    @returns
*           int: 0 success, else error code 
*/
    int adjustTime( const double& dSeconds );

/*------------------------------------------------------------------*/
// function CTimeValue::deltaTime
/**
*
* Determine difference, in seconds, between state time value 
*   and input time, specified as year, month, day_of_month, gmtsec values
*
*    @param iYear
*           int: year value (valid range = 1901-2099)
*    @param iMonth
*           int: month value (valid range 1-12)
*    @param iDay
*           int: day value (valid range 1-f(month))
*    @param dGmtsec
*           double: GMT seconds of day (normal range 0-86400)
*
*    @returns double
*           Number of seconds of time difference
*           (set to -999.999 if internal error occurs, for the lack of better value)
*/
    double deltaTime( const int& iYear,
                      const int& iMonth,
                      const int& iDay,
                      const double& dGmtsec );

/*------------------------------------------------------------------*/
// function CTimeValue::deltaTime
/**
*
* Determine difference, in seconds, between state time value 
*   and input time, specified as year, day_of_year, gmtsec values
*
*    @param iYear
*           int: year value (valid range = 1901-2099)
*    @param iDdd
*           int: day of year value (valid range 1-365/366)
*    @param dGmtsec
*           double: GMT seconds of day (normal range 0-86400)
*
*    @returns
*           double: Number of seconds of time difference
*           (set to -999.999 if internal error occurs, for the lack of better value)
*/
    double deltaTime( const int& iYear,
                      const int& iDdd,
                      const double& dGmtsec ) const;

/*------------------------------------------------------------------*/
// function CTimeValue::deltaTime
/**
*
* Determine difference, in seconds, between state time value 
*   and input time, specified as MJD or 'composite' time values
*
*    @param dTimeValue
*           double: Modified Julian Date: days.frac from 11/17/1858 0000 GMT
*            or 'composite' time value  (YYYYDDDGMTsc.fr format)
*
*    @returns
*           double: Number of seconds of time difference
*           (set to -999.999 if internal error occurs, for the lack of better value)
*/
    double deltaTime( const double& dTimeValue );

/*------------------------------------------------------------------*/
// function CTimeValue::operator-
/**
*
* Determine difference, in seconds, between state time value 
*   and input time, specified as another CTimeValue object
*
*    @param tTime
*           CTimeValue: time to be subtracted from current object time
*
*    @returns
*           double: Number of seconds of time difference
*           (set to -999.999 if internal error occurs, for the lack of better value)
*/
    double operator-( const CTimeValue& tTime ) const;

/*------------------------------------------------------------------*/
// function CTimeValue::undefined
/**
*
* Get verdict about state time value having been defined
*
*    @returns
*           bool: true or false
*/
    bool undefined() const { return (cdModJulDate==0.0); }

/*------------------------------------------------------------------*/
// function CTimeValue::getYear
/**
*
* Return the 4-digit 'Year' value of the current state time value
*
*    @returns
*           int: year value (1901-2099) 
*/
    int getYear() const { return ciYear; }

/*------------------------------------------------------------------*/
// function CTimeValue::getDdd
/**
*
* Return the 'Day of Year' value of the current state time value
*
*    @returns
*           int: day of year value (1-365/366) 
*/
    int getDdd() const { return ciDdd; }

/*------------------------------------------------------------------*/
// function CTimeValue::getMonth
/**
*
* Return the 'month' value of the current state time value
*
*    @returns
*           int: month value (1-12) 
*/
    int getMonth() const;

/*------------------------------------------------------------------*/
// function CTimeValue::getDayOfMonth
/**
*
* Return the 'day of month' value of the current state time value
*
*    @returns
*           int: day value (1-28,29,30 or 31, depending on month) 
*/
    int getDayOfMonth() const;
// Return the 'GMT seconds' value of the current state time value
/*           double: GMT seconds of day 
*/

/*------------------------------------------------------------------*/
// function CTimeValue::getGmtsec
/**
*
* Return the 'GMT seconds' value of the current state time value
*
*    @returns
*           double: GMT seconds of day (0-86399.99) 
*/
    double getGmtsec() const { return cdGmtsec; }

// Return the current state time value in Modified Julian Date form
/*           double: Modified Julian Date value 
*/

/*------------------------------------------------------------------*/
// function CTimeValue::getGmtsec
/**
*
* Return the 'Modified Julian Date' value of the current state time value
*
*    @returns
*           double: Modified Julian Date value
*/
    double getModJulDate() const { return cdModJulDate; }

/*------------------------------------------------------------------*/
// function CTimeValue::getCompositeTime
/**
*
* Return the current state time value in form of 'composite' time
*
*    @returns
*           double: 'composite' time value  (YYYYDDDGMTsc.fr format) 
*/
    double getCompositeTime() const;

/*------------------------------------------------------------------*/
// function CTimeValue::getUnixTime
/**
*
* Return the current state time value in form of Unix time value
*
*    @returns
*           time_t: seconds from  00:00:00 UTC, January 1, 1970
*/
    time_t getUnixTime() const;


/*------------------------------------------------------------------*/
// function CTimeValue::output
/**
*
* Returns the current state time value as string in specified format.
*
*    @param strFormat
*           string: strftime-style time format specification
*    @param iSecDec (optional, default=0)
*           int: number of decimals for fractional seconds
*
*    @returns
*           associated time string
*/
    string output( const string &strFormat,
                   const int& iSecDec=0 ) const;

/*------------------------------------------------------------------*/
// function CTimeValue::timestamp
/**
*
* Using the specified 'strftime'-style format string, returns
* the current system time value as a string.  By default, GMT-referenced
*
*    @param strFormat
*           strftime-style time format specification
*    @param bLocalRef
*           boolean for returning local-referenced time string
*           [default=false]
*
*    @returns
*           associated time string
*/
    static string timestamp( const string &strFormat,
                             const bool &bLocalRef=false );
// Return the current state time value in standard XSD-format string
/*           string: time in XSD format (whole seconds) 
*/

/*------------------------------------------------------------------*/
// function CTimeValue::getXsdDateTime
/**
*
* Returns the current state time value as string in standard XSD format. <br>
*  XSD = "<YYYY>-<MM>-<DD>T<hh>:<mm>:<ss>" (no fraction of seconds)
*
*    @returns
*           string: time in XSD format
*/
    string getXsdDateTime() const { return output( "%FT%T" ); }

/*------------------------------------------------------------------*/
// function CTimeValue::getXsdDateTimeFrac
/**
*
* Returns the current state time value as string in extended XSD format. <br>
*  XSD = "<YYYY>-<MM>-<DD>T<hh>:<mm>:<ss>[.sss]" (*with* fraction of seconds)
*
*    @param iSecDec
*           int: number of decimal places for fraction of seconds <br>
*           [default = 3]
*
*    @returns
*           string: time in XSD format (with fraction of seconds)
*/
    string getXsdDateTimeFrac ( const int& iSecDec=3 ) const { return output( "%FT%T", iSecDec ); }

//
// ====================================================================
// ======= static methods, not requiring class instantiation ==========
//
/**
 * @name 'Static' methods, not requiring class instantiation
 * @{
 */
/*------------------------------------------------------------------*/
// function CTimeValue::isLeapYear
/**
*
* Determine if specified year is a leap year
*
*    @param iYear
*           int: year value (four digits, with no range restriction)
*
*    @returns
*           bool: true/false if leap year  
*/
    static bool isLeapYear( const int& iYear );

/*------------------------------------------------------------------*/
// function CTimeValue::getDaysOfYear
/**
*
* Calculate day of year value from year, month, day value
*
*    @param iYear
*           int: year value (four digits, with no range restriction)
*    @param iMonth
*           int: month value (valid range 1-12)
*    @param iDay
*           int: day value (valid range 1-f(month))
*
*    @param[out] iDdd
*           int: corresponding day of year value
*
*    @returns
*           int: 0 success, else error code 
*/
    static int getDaysOfYear( const int& iYear,
                              const int& iMonth,
                              const int& iDay,
                              int &iDdd );

/*------------------------------------------------------------------*/
// function CTimeValue::getMonthDay
/**
* Calculate month and day values from year and day_of_year values
*
*    @param iYear
*           int: year value (no range restriction)
*    @param iDdd
*           int: day of year value (valid range = 1-365/366)
*
*    @param[out] iMonth
*           int: month value (range 1-12)
*    @param[out] iDay
*           int: day value (range 1-f(month))
*
*    @returns
*           int: 0 success, else error code 
*/
    static int getMonthDay( const int& iYear,
                            const int& iDdd,
                            int& iMonth,
                            int& iDay );

/*------------------------------------------------------------------*/
// function CTimeValue::getHrMinSecFromGmtsec
/**
*
* Calculate hour, min, sec values from GMT seconds of day value
*
*    @param dGmtsec
*           double: GMT seconds of day
*
*    @param[out] iHr
*           int: hours of day
*    @param[out] iMin
*           int: minutes of hour
*    @param[out] dSec
*           double: seconds of minute
*
*    @returns
*           int: 0 success, else error code 
*/
    static int getHrMinSecFromGmtsec( const double& dGmtsec, 
                                      int& iHr,
                                      int& iMin,
                                      double& dSec );

/*------------------------------------------------------------------*/
// function CTimeValue::getGmtsecFromHrMinSec
/**
*
* Calculate hour, min, sec values from GMT seconds of day value
*
*    @param iHr
*           int: hours of day
*    @param iMin
*           int: minutes of hour
*    @param dSec
*           double: seconds of minute
*
*    @param[out] dGmtsec
*           double: GMT seconds of day
*
*    @return int
*           0 success, else error code 
*/
    static int getGmtsecFromHrMinSec( const int& iHr,
                                      const int& iMin, 
                                      const double& dSec,
                                      double& dGmtsec );

/*------------------------------------------------------------------*/
// function CTimeValue::getDateTimeFromTimeValue
/**
*
* Convert MJD or 'composite' time value to year, day_of_year, gmtsec
*
*   Note: use of 'composite' time is discouraged
*
*    @param dTimeValue
*           double: Modified Julian Date: days.frac from 11/17/1858 0000 GMT
*            or 'composite' time value  (YYYYDDDGMTsc.fr format)
*
*    @param[out] iYear
*           int: corresponding year value
*    @param[out] iDdd
*           int: corresponding day of year value
*    @param[out] dGmtsec
*           double: corresponding GMTseconds (of day) value
*
*    @returns
*           int: 0 success, else error code 
*/
    static int getDateTimeFromTimeValue( const double& dTimeValue, 
                                         int& iYear,
                                         int& iDdd,
                                         double& dGmtsec );

/*------------------------------------------------------------------*/
// function CTimeValue::getDateTimeFromMJD
/**
*
* Convert Modified Julian Date to year, day_of_year, gmtsec, 
*  for dates within 1901-2099 year range
*
*    @param dModJulDate
*           double: Modified Julian Date: days from 11/17/1858 0000 GMT
*
*    @param[out] iYear
*           int: corresponding year value (range 1901-2099)
*    @param[out] iDdd
*           int: corresponding day of year value
*    @param[out] dGmtsec
*           double: corresponding GMTseconds (of day) value
*
*    @returns
*           int: 0 success, else error code 
*/
    static int getDateTimeFromMJD( const double& dModJulDate, 
                                   int& iYear,
                                   int& iDdd,
                                   double& dGmtsec );

/*------------------------------------------------------------------*/
// function CTimeValue::getDateTimeFromCompositeTime
/**
*
* Convert 'composite' time value to year, day_of_year, gmtsec
*
*   Note: use of 'composite' time is discouraged
*
*    @param dCompositeTime
*           double: composite time value  (YYYYDDDGMTsc.fr format)
*
*    @param[out] iYear
*           int: corresponding year value
*    @param[out] iDdd
*           int: corresponding day of year value
*    @param[out] dGmtsec
*           double: corresponding GMTseconds (of day) value
*
*    @returns
*           int: 0 success, else error code 
*/
    static int getDateTimeFromCompositeTime( const double& dCompositeTime, 
                                             int& iYear,
                                             int& iDdd,
                                             double& dGmtsec );

/*------------------------------------------------------------------*/
// function CTimeValue::getMJDFromDateTime
/**
*
* Calculate Modified Julian Date value from year, day_of_year, gmtsec values
*
*    @param iYear
*           int: year value (valid range = 1901-2099)
*    @param iDdd
*           int: day of year value (valid range 1-365/366)
*    @param dGmtsec
*           double: GMT seconds of day (normal range 0-86400)
*
*    @param[out] dModJulDate
*           double: Modified Julian Date: days from 11/17/1858 0000 GMT
*
*    @returns
*           int: 0 success, else error code 
*/
    static int getMJDFromDateTime( const int& iYear,
                                   const int& iDdd, 
                                   const double& dGmtsec,
                                   double &dModJulDate );

/*------------------------------------------------------------------*/
// function CTimeValue::getMJDFromDateTime (overload)
/**
*
* Calculate Modified Julian Date value from year, month, day, gmtsec values
*
*    @param iYear
*           int: year value (valid range = 1901-2099)
*    @param iMonth
*           int: month value (valid range 1-12)
*    @param iDay
*           int: day value (valid range 1-f(month))
*    @param dGmtsec
*           double: GMT seconds of day (normal range 0-86400)
*
*    @param[out] dModJulDate
*           double: Modified Julian Date: days from 11/17/1858 0000 GMT
*
*    @returns
*           int: 0 success, else error code 
*/
    static int getMJDFromDateTime( const int& iYear, 
                                   const int& iMonth,
                                   const int& iDay, 
                                   const double& dGmtsec,
                                   double &dModJulDate );

/*------------------------------------------------------------------*/
// function CTimeValue::getCompositeTimeFromDateTime
/**
*
* Calculate 'composite' time value from year, day_of_year, gmtsec values <br>
*   *Note: use of 'composite' time is discouraged*
*
*    @param iYear
*           int: year value 
*    @param iDdd
*           int: day of year value 
*    @param dGmtsec
*           double: GMT seconds of day
*
*    @param[out] dCompositeTime
*           double: 'composite' time value (YYYYDDDGMTsc.fr format)
*
*    @returns
*           int: 0 success, else error code 
*/
    static int getCompositeTimeFromDateTime( const int& iYear,
                                             const int& iDdd, 
                                             const double& dGmtsec,
                                             double& dCompositeTime );

/*------------------------------------------------------------------*/
// function CTimeValue::getCompositeTimeFromDateTime
/**
*
* Calculate 'composite' time value from year, month, day, gmtsec values <br>
*   *Note: use of 'composite' time is discouraged*
*
*    @param iYear
*           int: year value 
*    @param iMonth
*           int: month value (valid range 1-12)
*    @param iDay
*           int: day value (valid range 1-f(month))
*    @param dGmtsec
*           double: GMT seconds of day
*
*    @param[out] dCompositeTime
*           double: 'composite' time value (YYYYDDDGMTsc.fr format)
*
*    @returns
*           int: 0 success, else error code 
*/
    static int getCompositeTimeFromDateTime( const int& iYear,  
                                             const int& iMonth,
                                             const int& iDay,
                                             const double& dGmtsec,
                                             double& dCompositeTime );

/*------------------------------------------------------------------*/
// function CTimeValue::getTimeValueDifference
/**
*
* Get difference (in seconds) between two times (Time2 - Time1) <br>
* Due to internal use of MJD conversion routines, valid for years 1901-2099 only <br>
*   *Note: use of 'composite' time is discouraged*
*
*    @param dTimeValue1
*           double: Modified Julian Date: days.frac from 11/17/1858 0000 GMT
*            or 'composite' time value  (YYYYDDDGMTsc.fr format)
*    @param dTimeValue2
*           double: must match timeform used for dTimeValue1
*
*    @returns
*           double: Result of (Time2 - Time1) calculation, in seconds 
*/
    static double getTimeValueDifference( const double& dTimeValue1,
                                          const double& dTimeValue2 );

/*------------------------------------------------------------------*/
// function CTimeValue::getCompositeTimeDifference
/**
*
* Get difference (in seconds) between two times (Time2 - Time1) <br>
* Due to internal use of MJD conversion routines, valid for years 1901-2099 only <br>
*   *Note: use of 'composite' time is discouraged*
*
*    @param dTimeValue1
*           double: 'composite' time value  (YYYYDDDGMTsc.fr format)
*    @param dTimeValue2
*           double: must match timeform used for dTimeValue1
*
*    @returns
*           double: Result of (Time2 - Time1) calculation, in seconds 
*/
    static double getCompositeTimeDifference( const double& dTimeValue1,
                                              const double& dTimeValue2 );

/*------------------------------------------------------------------*/
// function CTimeValue::getAdjustedTimeValue
/**
*
* Adjust MJD or composite time value by specified delta <br>
*   *Note: use of 'composite' time is discouraged*
*
*    @param dTimeValue
*           double: Modified Julian Date: days.frac from 11/17/1858 0000 GMT
*            or 'composite' time value  (YYYYDDDGMTsc.fr format)
*    @param iDeltaDays
*           int: number of days to adjust time by (pos or neg)
*    @param dDeltaSeconds
*           double: number of seconds to adjust time by (pos or neg)
*
*    @returns
*           double: adjusted time value, in whichever form used for input 
*/
    static double getAdjustedTimeValue( const double& dTimeValue, 
                                        const int& iDeltaDays,
                                        const double& dDeltaSeconds );

/*------------------------------------------------------------------*/
// function CTimeValue::getAdjustedTimeValue
/**
*
* Adjust MJD or composite time value by specified delta <br>
*   *Note: use of 'composite' time is discouraged*
*
*    @param dTimeValue
*           double: Modified Julian Date: days.frac from 11/17/1858 0000 GMT
*            or 'composite' time value  (YYYYDDDGMTsc.fr format)
*    @param dDeltaSeconds
*           double: number of seconds to adjust time by (pos or neg)
*
*    @returns
*           double: adjusted time value, in whichever form used for input 
*/
    static double getAdjustedTimeValue( const double& dTimeValue,
                                        const double& dDeltaSeconds );

/*------------------------------------------------------------------*/
// function CTimeValue::getAdjustedCompositeTime
/**
*
* Adjust 'composite' time value by specified delta in seconds <br>
*   *Note: use of 'composite' time is discouraged*
*
*    @param dTimeValue
*           double: 'composite' time value  (YYYYDDDGMTsc.fr format)
*    @param dDeltaSeconds
*           double: number of seconds to adjust time by (pos or neg)
*
*    @returns
*           double: adjusted time value, in whichever form used for input 
*/
    static double getAdjustedCompositeTime( const double& dTimeValue,
                                            const double& dDeltaSeconds );

/*------------------------------------------------------------------*/
// function CTimeValue::getAdjustedCompositeTime
/**
*
* Adjust 'composite' time value by specified delta in days and seconds <br>
* Due to occassional internal use of MJD conversion routines, this method
* is valid for years 1901-2099 only. <br>
*   *Note: use of 'composite' time is discouraged*
*
*    @param dTimeValue
*           double: 'composite' time value  (YYYYDDDGMTsc.fr format)
*    @param iDeltaDays
*           int: number of days to adjust time by (pos or neg)
*    @param dDeltaSeconds
*           double: number of seconds to adjust time by (pos or neg)
*
*    @returns
*           double: adjusted time value, in 'composite' time format 
*/
    static double getAdjustedCompositeTime( const double& dTimeValue,
                                            const int& iDeltaDays,
                                            const double& dDeltaSeconds );

/*------------------------------------------------------------------*/
// function CTimeValue::getYearFracFromDateTime
/**
*
* Calculate 'year.frac' time value from year, day_of_year, gmtsec values <br>
*   *Note: use of 'year.frac' time is discouraged at high levels
*              due to roundoff errors from incrementing seconds*
*
*    @param iYear
*           int: year value 
*    @param iDdd
*           int: day of year value 
*    @param dGmtsec
*           double: GMT seconds of day
*
*    @param[out] dYearFrac
*           double: 'year.frac' time value (YYYY.fracOfYear format)
*
*    @returns
*           int: 0 success, else error code 
*/
    static int getYearFracFromDateTime( const int& iYear,
                                        const int& iDdd,
                                        const double& dGmtsec,
                                        double& dYearFrac );

/*------------------------------------------------------------------*/
// function CTimeValue::getYearFracFromTimeValue
/**
*
* Calculate 'year.frac' time value from input time value <br>
*   *Note: use of 'year.frac' time is discouraged at high levels
*              due to roundoff errors from incrementing seconds*
*
*    @param dTimeValue
*           double: Modified Julian Date: days.frac from 11/17/1858 0000 GMT
*            or 'composite' time value  (YYYYDDDGMTsc.fr format)
*
*    @param[out] dYearFrac
*           double: 'year.frac' time value (YYYY.fracOfYear format)
*
*    @return
*           int: 0 success, else error code 
*/
    static int getYearFracFromTimeValue( const double& dTimeValue,
                                         double& dYearFrac );

/*------------------------------------------------------------------*/
// function CTimeValue::getDateTimeFromYearFrac
/**
*
* Convert 'year.frac' time value to year, day_of_year, gmtsec <br>
*   *Note: use of 'year.frac' time is discouraged at high levels
*              due to roundoff errors from incrementing seconds*
*
*    @param dYearFrac
*           double: 'year.frac' time value (YYYY.fracOfYear format)
*
*    @param[out] iYear
*           int: corresponding year value
*    @param[out] iDdd
*           int: corresponding day of year value
*    @param[out] dGmtsec
*           double: corresponding GMTseconds (of day) value
*
*    @returns
*           int: 0 success, else error code 
*/
    static int getDateTimeFromYearFrac( const double& dYearFrac,
                                        int& iYear,
                                        int& iDdd,
                                        double& dGmtsec );
/**
 * end of 'static' methods
 * @}
 */

// =================================================================

  private:
    
   double cdModJulDate;
   int ciYear;
   int ciDdd;
   double cdGmtsec;
   
};

#endif
