/***********************************************************************
 File: COrbitPropEnums_c.h
 
 Description:

   Definitions for orbit propagator C wrapper enumerations.

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


***********************************************************************/
//-------------------------------------------------------------------
// File: COrbitPropEnums_c.h
//
// Description: 
//  Model Enum values for the COrbitProp C Interface. 
//  The eOrbitPropModel enumeration lists the available models for 
//  which there is a C++ Propagator class defined, and are used as
//  inputs to the SPropagator structure (see COrbitProp_c.h).
//
// Modification History:
// 07Jun2013  ssmith   Initial revision
//-------------------------------------------------------------------

#ifndef CORBITPROPENUMS_C_H
#define CORBITPROPENUMS_C_H

typedef enum eOrbitPropModel {
    eORBITPROP_MODEL_UNKNOWN,
    eORBITPROP_MODEL_SGP4,
    eORBITPROP_MODEL_SATEPH
} eOrbitPropModel;

#endif //CORBITPROPENUMS_C_H

