/******************************************************************************
$HeadURL$

File: AEErrorHandler.h

Description: AEErrorHandler is a lightweight error propagation class. It is 
intended that classes needing to support the propagation of integer error
codes and associated text error messages derive from this class. This class
can be used to return a contextual hierarchy of error messages up the call
stack, giving the client a more complete diagnostic view of how and why an 
error occurred. To propagate errors when a called method returns an error,
use a statement of the form: 

return( logError(iReturnedError,"My error message", erroredObject.getErrorText()));

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
  1.0         01/06/2010  created 
  
SVNTag: $Id$
******************************************************************************/
#ifndef AEERRORHANDLER_H
#define AEERRORHANDLER_H

#include <string>
using namespace std;

namespace ae9ap9 
{
    class AEErrorHandler
    {
    public:
        AEErrorHandler(){}
        ~AEErrorHandler(){}

        int logError(int iError, const string& strError, const string& strNestedError )
        {
            m_strError = strError + ("\n" + strNestedError);
            return (iError);
        }

        const string& getErrorText() { return m_strError; }

    protected:

        string m_strError;

    };

} /*namespace*/

#endif
