/***********************************************************************
 File: CMagFieldErrors.h
 
 Description:

   Definitions for magnetic field model error code enumerations.

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
#ifndef CMagFieldErrors_H
#define CMagFieldErrors_H
enum eMAGFIELD_ERROR_CODE{
  emfNotImplemented=-2,
  emfUserDefined=-1,
  emfNoError = 0,
  emfInActive,
  emfExeception,
  emfError,
  emfWarning,
  emfNoDataBase,
  emfInitializationFailed,
  emfVarSizeMisMatch,
  emfInvalidNullPointer,
  emfOutOfRange,/*!< variable or index out of range*/
  // these are position specific and map to fortran return codes 7-43
  // Dean, these need to be remapped higher, above say 200
  emfEndOfGenerics=200,
  emfConvertCoordFailed,
  emfBadHVInput=206,
  emfBadInputCoordinate,
  emfBadInputRadius,
  emfBadFieldLineCoordinate, 
  emfBadNumberOfSteps,
  emfBadRangeValue,
  emfBadTraceDirection,
  emfBffootError,
  emfIGRFError,
  emfGetprError,
  emfRdgrfError,
  emfMllgsmError,
  emfBadIGRFUnit,
  emfXyzsphError,
  emfYearOutOfRange,
  emfMllsmError,
  emfSpigrfError,
  emfBadKpAeBin,
  emfBadSpecies,
  emfBadConversionDirection,
  emfRecalc2Uninitialized,
  emfPointsOutsideSphere,
  emfPointsInsideSphere,
  emfBadDirctoryPath,
  emfBadPathOption,
  emfBadBmapOutputRequest,
  emfInsufficientFieldLineStorage,
  emfInsufficientStorage,
  emfBadOption, 
  emfUnsupportedOption,
  emfUnimplementedFunction, 
  emfUnsupportedMainField, 
  emfUnsupportedFieldModel,
  emfUnsupportedBfieldOutput,
  emfHvFileError,
  // these need to be mapped from fortran due to conflicts above
  emfBadItem, 
  emfBadDecimalDay,
  emfBadMainField,
  emfBadTiltTimeMode,
  emfBadExternalFieldModel,
  emfBadStringLength,
  emfBmapBaseError,
  emfBadFieldLineTraceDirection,
  emfFieldLineInsideEarthSphere,
  emfFieldLineOutsideModelBounds,
  emfFieldLineBadTime,
  emfFieldLineTraceLimitExceeded,
  emfBadFootPrintsCadence,
  emfMagfieldErrorCodeZ
};

#endif  // CMagFieldErrors_H
