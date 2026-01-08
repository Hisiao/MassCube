#******************************************************************************
#$HeadURL$
#
# File: dateTime.py
#
# Description: Python language routines to perform various date and time conversions.
#
# Classification:
#
#   Unclassified
#
# Project Name:
#
#   AE9/AP9/SPM Radiation Environment Models
#
#   Developed under US Government contract # FA9453-12-C-0231
#
# Rights and Restrictions:
#
#   Copyright 2018 Atmospheric and Environmental Research, Inc. (AER)
#
#   DISTRIBUTION A. Approved for public release; distribution is unlimited.
#
#   The AE9/AP9/SPM software license is contained in the 'documents/Licenses' 
#   folder of this distribution file collection.
#
# Author:
#
#   This software was developed by AER staff
#
# Contact:
#
#   Atmospheric and Environmental Research, Inc.
#   131 Hartwell Avenue
#   Lexington, MA 02421-3126 USA
#   Phone: 781.761.2288
#   email: spwx@aer.com
#
# References:
# 
#   None
#
# Revision history:
#
#  Version      Date        Notes
#  1.0          12/28/2017  Created 
#  
#SVNTag: $Id$
#******************************************************************************/

import os
import sys
import ctypes

c_double_p = ctypes.POINTER( ctypes.c_double )
c_int_p = ctypes.POINTER( ctypes.c_int )

if 'IRENE_SYS' not in os.environ:
  sys.exit('Error: undefined IRENE environment variables - calling script requires "import irene_defs.py"')
  
if os.environ['IRENE_SYS'] == 'linux':
  datetimelib = ctypes.cdll.LoadLibrary(os.environ['IRENE_LIB']+'/libdatetimeutil.so')
else:
  datetimelib = ctypes.cdll.LoadLibrary(os.environ['IRENE_BIN']+'/libdatetimeutil_c.dll')

def get_gmtsec( iHr, iMin, dSec ):
  datetimelib.DateTimeGetGmtSeconds.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_double]
  datetimelib.DateTimeGetGmtSeconds.restype = ctypes.c_double #***
  return datetimelib.DateTimeGetGmtSeconds( int(iHr), int(iMin), float(dSec) )

def get_dayYear( iYear, iMonth, iDay ):
  datetimelib.DateTimeGetDayOfYear.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_int]
  return datetimelib.DateTimeGetDayOfYear( int(iYear), int(iMonth), int(iDay) )

def get_modJulDate( iYear, iDdd, dGmtsec ):
  datetimelib.DateTimeGetModifiedJulianDate.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_double]
  datetimelib.DateTimeGetModifiedJulianDate.restype = ctypes.c_double #***
  return datetimelib.DateTimeGetModifiedJulianDate( int(iYear), int(iDdd), float(dGmtsec) )

def get_modJulDateUnix( iUnixTime ):
  datetimelib.DateTimeGetModifiedJulianDateUnix.argtypes = [ctypes.c_int]
  datetimelib.DateTimeGetModifiedJulianDateUnix.restype = ctypes.c_double #***
  return datetimelib.DateTimeGetModifiedJulianDateUnix( int(iUnixTime) )

#  XSD date/time string format: "<YYYY>-<MM>-<DD>T<hh>:<mm>:<ss>[.ssss][Z]"
def get_modJulDateXsd( strXsdDateTime ):
  datetimelib.DateTimeGetModifiedJulianDateXsd.argtypes = [ctypes.c_char_p]
  datetimelib.DateTimeGetModifiedJulianDateXsd.restype = ctypes.c_double #***
  return datetimelib.DateTimeGetModifiedJulianDateXsd( strXsdDateTime.encode() )

def get_dateTime( dMjdValue ):
  iYear = ctypes.c_int()
  iDdd = ctypes.c_int()
  dGmtsec = ctypes.c_double()
  datetimelib.DateTimeGetDateTime.argtypes = [ctypes.c_double, c_int_p, c_int_p, c_double_p]
  ierr = datetimelib.DateTimeGetDateTime( float(dMjdValue), 
            ctypes.byref(iYear), ctypes.byref(iDdd), ctypes.byref(dGmtsec) )
  return iYear.value, iDdd.value, dGmtsec.value

def get_hms( dGmtsec ):
  iHr = ctypes.c_int()
  iMin = ctypes.c_int()
  dSec = ctypes.c_double()
  datetimelib.DateTimeGetHoursMinSec.argtypes = [ctypes.c_double, c_int_p, c_int_p, c_double_p]
  ierr = datetimelib.DateTimeGetHoursMinSec( float(dGmtsec), 
            ctypes.byref(iHr), ctypes.byref(iMin), ctypes.byref(dSec) )
  return iHr.value, iMin.value, dSec.value

def get_monthDay( iYear, iDdd ):
  iMon = ctypes.c_int()
  iDay = ctypes.c_int()
  datetimelib.DateTimeGetMonthDay.argtypes = [ctypes.c_int, ctypes.c_int, c_int_p, c_int_p]
  ierr = datetimelib.DateTimeGetMonthDay( int(iYear), int(iDdd), 
            ctypes.byref(iMon), ctypes.byref(iDay) )
  return iMon.value, iDay.value


