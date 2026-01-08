/***********************************************************************
 File: CRadEnvEnums.h

 Description:

   Definitions of radiation belt enumerations.

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
// File: CRadEnvEnums.h
//
// Description:
// Enumeration values for the CRadEnv model (derived from the
// generic model error code enumerations)
//
// Modification History:
// 20080806  raq   Initial revision
//-------------------------------------------------------------------

#ifndef __AerSpEnv_h__

#ifndef CRADENVENUMS_DEFINE
#define CRADENVENUMS_DEFINE

typedef enum eRADENV_ERROR_CODE {
  ereNotImplemented=-2,
  ereUserDefined=-1,
  ereNoError = 0,
  ereInActive,
  ereException,
  ereError,
  ereWarning,
  ereNoDataBase,
  ereInitializationFailed,
  ereVarSizeMisMatch,
  ereInvalidNullPointer,
  ereOutOfRange,
  // below here are model specific error codes
  ereEndOfGenerics=200,
  ereInvalidFluxModel,
  ereRadenvError,
  ereMagfieldError,
  ereShieldDoseError,
  ereBadCompMode,
  ereInvalidZeroFluxes,
  ereNonCrresEnergy
} eRADENV_ERROR_CODE;

typedef enum ereCrresProActivity {
  ecpaQuiet=0,
  ecpaActive
} ereCrresProActivity;

typedef enum ereCrresRadActivity {
  ecraQuiet=0,
  ecraActive,
  ecraAverage
} ereCrresRadActivity;

typedef enum ereNasaActivity {
  ecnaSolarMin=0,
  ecnaSolarMax
} ereNasaActivity;

typedef enum ereDoseChannel {
  ereTotal=0,
  ereHiLet,
  ereLoLet
} ereDoseChannel;

typedef enum ereSpecies {
  efmEle=0,
  efmPro
} ereSpecies;

typedef enum ereDoseModel {
  edmApexRad=0,
  edmCrresRad
} ereDoseModel;

// model-dependent Activity Ranges
typedef enum eApexRadActivityRange {
  //  based on 15 day Avg Ap
  eara5to7_5=0,
  eara7_5to10,
  eara10to15,
  eara15to20,
  eara20to25,
  earaAll
} eApexRadActivityRange; // Chris: note change in order here!!! - Raq

typedef enum eCrresEleActivityRange {
  //  base on 15 day Avg Ap
  ecear5to7_5=0,   // order of these enum
  ecea7_5to10,    // values is crucial for
  ecea10to15,     // proper database access
  ecea15to20,
  ecea20to25,
  eceaGT25,
  eceaAvg,
  eceaMax,
  eceaAll
} eCrresEleActivityRange;

typedef enum ereCompMode {
  ecmApexRad=0,
  ecmCrresRad,
  ecmRadData,
  //  ecmCrresEle,
  //  ecmCrresPro,
  ecmCrres,
  //  ecmNasaEle,
  //  ecmNasaPro,
  ecmNasa
  //  ecmBest,
  //  ecmNasaOnly
} ereCompMode;

// looking at new enum ereModels to replace ereFluxModel,
// ereDoseModel, and ereCompMode with a single list.  ereNasaModel will
// remain to support the setNasaEnergies() method.
typedef enum ereModel {
  ermMODEL_UNKNOWN=-1,
  ermApexRad=0,
  ermCrresRad,
  ermCrresEle,
  ermCrresPro,
  ermNasaEle,
  ermNasaPro
  //  ermBest,
  //  ermNasaOnly
} ereModel;


#endif // CRADENVENUMS_DEFINE
#endif
