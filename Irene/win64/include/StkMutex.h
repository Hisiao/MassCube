/***********************************************************************

Module: StkMutex.h

Description: Class declaration to CStkMutex object.

   StkMutex is a generic wrapper class for platform dependent 
   implementations of mutex objects that can be used to synchronize
   access to shared data and code among threads in a single process.

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

   VERSION            DATE       NOTES
     1.0       Tue Jan 20, 2009   First release

------------------------------------------------------------------------

***********************************************************************/
#ifndef STKMUTEX_H
#define STKMUTEX_H

class CStkMutex
{
 public:
  CStkMutex();
  ~CStkMutex();

  bool lock();
  bool unlock();

 protected:
  void* cpOSMutex;  // Operating system specific mutex object
};

#endif
