/*
 *  Module: ShieldDose2_c.h
 *
 *  Description:
 *
 *      C interface methods to the ShieldDose2 class. Modified for pointer use to
 *      allow single library instance to handle multiple fields.
 *
 *   Rights and Restrictions:

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
 *
 *      VERSION     DATE        NOTES
 *      1.0         03/18/2014  First release
 *
 */
#ifndef ShieldDose2_C_H
#define ShieldDose2_C_H

#include "GenericModelEnum.h"
#include "CShieldDose2.h"

/**
 * @file: ShieldDose2_c.h
 * ShieldDose2 class C interface function definitions
 */


#ifdef __cplusplus
extern "C" {
#endif

// =======================================================
// These utility vector/data copy routines are duplicated amongst the model
//  classes, so to be enable each to be wholly independent & self-contained
int ShieldDose2CopyDataToVector1D( double* pvdData,
                                int iNum,
                                dvector& vdData );
int ShieldDose2CopyDataToVector2D( double* pvvdData,
                                int iNum1,
                                int iNum2,
                                vdvector& vvdData );
int ShieldDose2CopyDataToVector3D( double* pvvvdData,
                                int iNum1,
                                int iNum2,
                                int iNum3,
                                vvdvector& vvvdData );
int ShieldDose2CopyVector1DToData( dvector& vdData,
                                int iNum,
                                double* pvdData );
int ShieldDose2CopyVector2DToData( vdvector& vvdData,
                                int iNum1,
                                int iNum2,
                                double* pvvdData );
int ShieldDose2CopyVector3DToData( vvdvector& vvvdData,
                                int iNum1,
                                int iNum2,
                                int iNum3,
                                double* pvvvdData );
// =======================================================

typedef int HANDLE;

HANDLE ShieldDose2StartUp();

int ShieldDose2ShutDown(HANDLE zHandle);

int ShieldDose2Initialize(HANDLE zHandle, const char* szDBFullPath);

int ShieldDose2ClearThicknesses(HANDLE zHandle);
int ShieldDose2PushThickness(
         HANDLE zHandle,
         double dThickness);
int ShieldDose2SetThicknesses(
         HANDLE zHandle,
         int iNumThick,
         double* pdaThicknessesIn);
int ShieldDose2GetThicknesses(
         HANDLE zHandle,
         int* piNumThickOut,
         double* pdaThicknessesOut);
int ShieldDose2GetNumThicknesses(HANDLE zHandle);

int ShieldDose2SetThicknessUnit(
         HANDLE zHandle,
         const char* szThicknessUnitIn);
int ShieldDose2GetThicknessUnit(
         HANDLE zHandle,
         char* szThicknessUnitOut);
int ShieldDose2SetDetectorType(
         HANDLE zHandle,
         const char* szDetectorTypeIn);
int ShieldDose2GetDetectorType(
         HANDLE zHandle,
         char* szDetectorTypeOut);
int ShieldDose2SetNuclearAttenuationMode(
         HANDLE zHandle,
         const char* szAttenuationModeIn);
int ShieldDose2GetNuclearAttenuationMode(
         HANDLE zHandle,
         char* szAttenuationModeOut);

int ShieldDose2SetIntegrationParams(
         HANDLE zHandle,
         int iNumProtonPoints,
         int iNumElectPoints,
         double dMinSolar,
         double dMaxSolar,
         double dMinTrapped,
         double dMaxTrapped,
         double dMinElect,
         double dMaxElect);

int ShieldDose2GetIntegrationParams(
         HANDLE zHandle,
         int* piNumProtonPoints,
         int* piNumElectPoints,
         double* pdMinSolar,
         double* pdMaxSolar,
         double* pdMinTrapped,
         double* pdMaxTrapped,
         double* pdMinElect,
         double* pdMaxElect);

int ShieldDose2SetUnitEnergyConversionFactor(
         HANDLE zHandle,
         double dConvFactor);

int ShieldDose2GetUnitEnergyConversionFactor(
         HANDLE zHandle,
         double* pdConvFactor);

int ShieldDose2SetTimeInterval(
         HANDLE zHandle,
         double dTimeInterval);
int ShieldDose2GetTimeInterval(
         HANDLE zHandle,
         double* pdTimeInterval);

int ShieldDose2ComputeDose(
         HANDLE zHandle,
         const char* szParticleType,
         int inArrayLength,
         double* pdEnergyArray,
         double* pdFluxArray);

int ShieldDose2ComputeAllDose(
         HANDLE zHandle,
         int iSolarProtonLength,
         double* pdSolarProtonEnergy,
         double* pdSolarProtonFlux,
         int iTrappedProtonLength,
         double* pdTrappedProtonEnergy,
         double* pdTrappedProtonFlux,
         int iElectronLength,
         double* pdElectronEnergy,
         double* pdElectronFlux);
int ShieldDose2GetDose(
         HANDLE zHandle,
         const char* szGeometry,
         int* piNumThickOut,
         double* pdTotalRate,
         double* pdTrappedProtonRate,
         double* pdElectronRate,
         double* pdBrehmRate,
         double* pdTotalElectronRate,
         double* pdSolarProtonRate);

int ShieldDose2GetAllDose(
         HANDLE zHandle,
         int* piNumThickOut,
         double* pdSemiSlabTotalRate,
         double* pdSemiSlabTrappedProtonRate,
         double* pdSemiSlabElectronRate,
         double* pdSemiSlabBrehmRate,
         double* pdSemiSlabTotalElectronRate,
         double* pdSemiSlabSolarProtonRate,
         double* pdFinSlabTotalRate,
         double* pdFinSlabTrappedProtonRate,
         double* pdFinSlabElectronRate,
         double* pdFinSlabBrehmRate,
         double* pdFinSlabTotalElectronRate,
         double* pdFinSlabSolarProtonRate,
         double* pdSphereTotalRate,
         double* pdSphereTrappedProtonRate,
         double* pdSphereElectronRate,
         double* pdSphereBrehmRate,
         double* pdSphereTotalElectronRate,
         double* pdSphereSolarProtonRate);

int ShieldDose2Message1(HANDLE zHandle,
  int code,
  char* cMessage);

int ShieldDose2Message2(HANDLE zHandle,
  char* cMessage);


#ifdef __cplusplus
} // extern "C"
#endif

#endif // ShieldDose2_C_H
