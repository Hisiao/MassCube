/******************************************************************************
$HeadURL$

File: AEDBModelBase.h

Description: Virtual base class declaration of AEDBModel class in the database 
	layer of the Ae9Ap9Model (used to eliminate external dependencies on internal
	classes.

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
  1.0          8/29/2013 Created 
  
SVNTag: $Id$
******************************************************************************/
#ifndef AEDBMODELBASE_H
#define AEDBMODELBASE_H

namespace ae9ap9 
{
    // Stub base class to eliminate external dependencies on internal headers

    class AEDBModelBase
    {
    public:
        // construction, destruction
        AEDBModelBase(){}
        ~AEDBModelBase(){}
    };
}

#endif
