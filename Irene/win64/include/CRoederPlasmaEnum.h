/***********************************************************************
 File: CRoederPlasmaEnum.h
 
 Description:

   Definitions of plasma model enumerations.

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
#ifndef CROEDERPLASMAENUM_H
#define CROEDERPLASMAENUM_H

// CRoederPlasmaEnum.h

enum eRoederMagField {  // value and order is critical
  eRoederMagUndef = -1,
  eRoederMagIGRF = 0,
  eRoederMagIGRF_OP,
  eRoederMagT87      //###no magnetic field model available for T87
};

enum eRoederDataSelect {  // value and order is critical
  eRoederDataUndef = -1,
  eRoederDataAll = 0,
  eRoederDataExclDST = 1
};

enum eRoederPitchAngleBin {  // value and order is critical
  eRoederPitchUndef = -1,
  eRoederOmniDirect = 0,
  eRoederPitch00_10 = 1,
  eRoederPitch10_20,
  eRoederPitch20_30,
  eRoederPitch30_40,
  eRoederPitch40_50,
  eRoederPitch50_60,
  eRoederPitch60_70,
  eRoederPitch70_80,
  eRoederPitch80_90,
  eRoederPitch90_100,
  eRoederPitch100_110,
  eRoederPitch110_120,
  eRoederPitch120_130,
  eRoederPitch130_140,
  eRoederPitch140_150,
  eRoederPitch150_160,
  eRoederPitch160_170,
  eRoederPitch170_180
};

enum eRoederSpecie {  // value and order is critical
  eRoederSpecieUndef = -1,
  eRoederSpecieHPlus = 0,
  eRoederSpecieHePlus,
  eRoederSpecieHePlus2,
  eRoederSpecieOPlus,
  eRoederSpecieH,
  eRoederSpecieHe,
  eRoederSpecieO,
  eRoederSpecieIons
};

enum eERROR_CODE {
  eERROR_DBFILE_OPEN_ERROR = -9,
  eERROR_DBFILE_READ_ERROR = -8,
  eERROR_NODATABASE_INIT   = -7,
  eERROR_INVALID_SPECIE    = -6,
  eERROR_NOSPECIE  = -5,
  eERROR_NOPITCH   = -4,
  eERROR_NOSEGMENT = -3,
  eERROR_NOMAGFLD  = -2,
  eERROR_NOTIME    = -1,
  eERROR_NONE = 0,
  eERROR_INVALID_LSHELL = 1,
  eERROR_INVALID_BB0    = 2,
  eERROR_INVALID_GMLAT  = 3
};

#endif
