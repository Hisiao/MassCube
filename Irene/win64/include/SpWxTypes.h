/***********************************************************************
 @File: SpWxTypes.h
 
 Description:

   Definitions for common SpWx variable types and enumerations.

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
#include <iostream>
#include <exception>
#include <vector>
#include <string>
#include <cmath>

#include <stdio.h>

#ifndef SpWx_Types_Def
#define SpWx_Types_Def

// extracted from GenericModel.h (24 Aug 2011 CJR)

typedef std::vector<int> ivector;
typedef std::vector<float> fvector;
typedef std::vector<double> dvector;
typedef std::vector<std::string> svector;
typedef std::vector<bool> bvector;
typedef std::vector<ivector> vivector;
typedef std::vector<fvector> vfvector;
typedef std::vector<dvector> vdvector;

typedef struct S3Tuple{
  double	x;
  double	y;
  double	z;

  S3Tuple():x(0.0),y(0.0),z(0.0){};
  void clear() {x=0.0;y=0.0;z=0.0;};
  double magnitude() const {return sqrt(x*x + y*y + z*z);};
} S3Tuple;

// Can't define in header - ppw12232009
//static dvector getS3Vector(const S3Tuple& s3tIn) {dvector dv(3); dv[0]=s3tIn.x; dv[1]=s3tIn.y; dv[2]=s3tIn.z; return dv;}
extern dvector getS3Vector(const S3Tuple& s3tIn);

typedef struct SGridPnt{
  double	seuProb;
  double	alt;
  S3Tuple	pos;
  
  SGridPnt() : seuProb(0.0), alt(0.0){};
  
} SGridPnt;

typedef struct STrajectory{
  double	t;	// time in seconds past some epoch in some time scale
  S3Tuple	geoPosition;
  
  STrajectory(): t(0.0){};
  
} STrajectory;

typedef struct SInterval{
  
  double entrance; // time in seconds past some epoch in some time scale
  double exit;	// time in seconds past some epoch in some time scale
  
  SInterval(): entrance(0.0), exit(0.0){};
  
} SInterval;

typedef std::vector<S3Tuple> S3TupleVec;
typedef std::vector<STrajectory> STrajectoryVec;
typedef std::vector<SInterval> SIntervalVec;
typedef std::vector<SGridPnt> SGridPntVec;

#endif  //  SpWx_Types_Def
