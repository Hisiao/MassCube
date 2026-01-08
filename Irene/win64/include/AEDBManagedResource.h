/******************************************************************************
$HeadURL$

File: AEDBManagedResource.h

Description: Class declaration and implementation for the AEDBManagedResource class
    This class serves as a base class for objects that can be retrieved through the
    AEDBResourceManager and reused by multiple clients. The AEDBResourceManager will
    maintain a single instance associated with the path of the resource and manage
    the lifetime of that resource through reference counting.

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
  1.0          10/09/2009 created
  
SVNTag: $Id$
******************************************************************************/

#ifndef AEDBMANAGEDRESOURCE
#define AEDBMANAGEDRESOURCE 

#include "AEErrorHandler.h"

namespace ae9ap9 
{
    class AEDBManagedResource : public AEErrorHandler
    {
    public:
        AEDBManagedResource(): m_uiRefCount(0){}
        virtual ~AEDBManagedResource(){}

        unsigned int addRef()  { return (++m_uiRefCount); }
        unsigned int release() { return (--m_uiRefCount); }

    protected:
        unsigned int m_uiRefCount;
    };

}

#endif
