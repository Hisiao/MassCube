/******************************************************************************
$HeadURL$

File: AEErrors.h

Description: Integer error return values for the AE9AP9 model

 Classification:

   Unclassified

 Project Name:

   AE9/AP9/SPM Radiation Environment Models

   Developed under US Government contract # FA9453-12-C-0231

 Rights and Restrictions:

   Copyright 2014 Atmospheric and Environmental Research, Inc. (AER)

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

  Version      Date       Notes
  1.0          08/26/2009 Created
  
SVNTag: $Id$
******************************************************************************/
#ifndef AEERRORS_H
#define AEERRORS_H

#include <cmath>
#include <limits>

namespace ae9ap9
{
    // AE9AP9 Error codes

    const int AE_ERR_NOERROR = 0;
    const int AE_ERR_NOT_FOUND = -1;
    const int AE_ERR_MEMORY = -2;
    const int AE_ERR_RANGE_ERROR = -3;
    const int AE_ERR_MATH_ERROR = -4;
    const int AE_ERR_LOADFAIL = -5;
    const int AE_ERR_SAVEFAIL = -6;
    const int AE_ERR_INVALIDARG = -7;
    const int AE_ERR_NOTIMPLEMENTED = -8;
    const int AE_ERR_UNINITIALIZED = -9;
    const int AE_ERR_DUPLICATE_NAME = -10;
    const int AE_ERR_UNSUPPORTED_COMPUTATION = -11;

    const int AE_ERR_BFIELD_RANGE_ERROR = 9;
    const int AE_WARN_INCOMPLETE_DOSE_INTERVAL = 10;

    // Use a small epsilon around zero to test for zero
    const double AE_ZERO_EPSILON = 1e-9;
    inline bool isZero( const double d) { return (fabs(d) < AE_ZERO_EPSILON); }

    // Use the std NaN constant as a double bad value flag

    const double AE_NaN = std::numeric_limits<double>::quiet_NaN();
#ifndef isnan
    inline bool isnan(const double d) { return d != d; }
#endif
//#ifdef WIN32
//    const double AE_NaN = HUGE_VAL;
//#else
//    const double AE_NaN = HUGE;

} /*namespace*/

#endif
