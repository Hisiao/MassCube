/**********************************************************************

Module: GeoSpaceTime

Description: Converts geospace time formats to y,m,d,h,m,s and back
 ### Use of this class is strongly discouraged - use CTimeValue instead ###

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

     VERSION           DATE        NOTES
       1.0      Thu Aug 25, 2008   First release

----------------------------------------------------------------------

***********************************************************************/
#ifndef GEOSPACETIME_H
#define GEOSPACETIME_H

class CGeoSpaceTime
{
 public:
  CGeoSpaceTime();
  ~CGeoSpaceTime();

  double getGeoSpaceTime(const long &lYear4,
			 const long &lDayOfYear,
			 const long &lHour,
			 const long &lMinute,
			 const long &lSeconds);

  double getGeoSpaceTime(const long &lYear4,
			 const long &lMonth,
			 const long &lMonthDay,
			 const long &lHour,
			 const long &lMinute,
			 const long &lSeconds);

  int getGeoSpaceTime(const long& lYear4, 
                      const long& lMonth,
                      const long& lMonthDay, 
		      const long& lHour, 
		      const long& lMinute, 
		      const double& dSecond,
                      double* pdTime);

  int getGeoSpaceTime(const long& lYear4, 
                      const long& lDayOfYear,
		      const double& dSecondofDay, 
		      double* pdTime);

  int setGeoSpaceTime(const double &dTime,
		      long *plYear4,
		      long *plDayOfYear,
		      long *plHour,
		      long *plMinute,
		      long *plSeconds);

  int setGeoSpaceTime(const double &dTime,
		      long *plYear4,
		      long *plMonth,
		      long *plMonthDay,
		      long *plHour,
		      long *plMinute,
		      long *pdSeconds);

   int setGeoSpaceTime(const double& dTime,
		      long *plYear4,
		      long *plMonth,
		      long *plMonthDay,
		      long *plHour,
		      long *plMinute,
		      double *pdSeconds);

   int setGeoSpaceTime(const double& dTime,
                       long* plYear4, 
		       long* plDayOfYear, 
		       double* pdSecondofDay);

   double adjustGeoSpaceTime(const double& dTime,
                             const double& dDeltaSecond);

   int convertSecondsofDaytoHMS(const double& dSecondofDay,
                                long* plHour, 
				long* plMinute, 
				double* pdSecond);

   int getJulianDate(const long& lYear4, 
                     const long& lMonth,
                     const long& lDay, 
		     const long& lHour, 
		     const long& lMinute, 
		     const double& dSecond,
                     double* pdJdate);
   
   int convertGregorianCaltoJulianDayNum(const long& lYear4, 
                                         const long& lMonth,
					 const long& lDay,
					 long* plJdnum);

   int getGregorianDate(const double& dJdate, 
                       long* plYear4,
                       long* plMonth,
		       long* plDay, 
		       long* plHour, 
		       long* plMinute, 
		       double* pdSecond);

   int convertJulianDayNumtoGregorianCal(const long& lJdnum,
                                         long* plYear4, 
					 long* plMonth, 
					 long* plDay);

   int convertGeoSpaceTimeToMJD( double dGeoDateTime, double& dMJD );

   int convertMJDToGeoSpaceTime( double dMJD, double& dGeoDateTime );

   bool isLeapYear( const long& lYear4 );

 protected:
  long ymd2doy( const long& lYear4, const long& lMonth, const long& lMonthDay );
  int doy2md( const long& lYear4, const long& lDoy, long* plMonth, long* plMonthDay );

  static int  cDaysB4[12];
  static int  cLYDaysB4[12];
};

#endif
