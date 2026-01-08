/***********************************************************************
 File: CShieldDoseEnums.h
 
 Description:

   Definitions of ShieldDose2 model enumerations.

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
// CShieldDoseEnums - enumerations related to shield dose

#ifndef __AerSpEnv_h__

#ifndef CSHIELDDOSE_ENUMS
#define CSHIELDDOSE_ENUMS

typedef enum ecsInterpolationType {
   ecsLinear=1,
   ecsSpline
} ecsInterpolationType;

typedef enum ecsShieldThickUnits {
  ecsMils=1,
  ecsGperCm2,
  ecsMilliMeters
} ecsShieldThickUnits;

typedef enum ecsDetectorType {
  ecsAluminum=1,
  ecsGraphite,
  ecsSilicon,
  ecsAir,
  ecsBone,
  ecsCalcium,
  ecsGallium,
  ecsLithium,
  ecsSiliconDioxide,
  ecsTissue,
  ecsWater
} ecsDetectorType;

typedef enum ecsDetectorGeometry {
  ecsSemiInfiniteSlab=0,
  ecsFiniteSlab,
  ecsSpherical
} ecsDetectorGeometry;

typedef enum ecsNuclearAttenMode {
  ecsNone=1,
  ecsLcsed,
  ecsLcsedPlusNeutrons
} ecsNuclearAttenMode;

typedef enum ecsParticles {
  ecsTrappedProtons=0,
  ecsElectrons,
  ecsSolarProtons
} ecsParticles;

#endif

#endif
