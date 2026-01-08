/*
 *  Module: CRadEnv_c.h
 *
 *  Description:
 *
 *      C interface methods to the CRadEnv class. Modified for pointer use to
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
 *      1.0         03/13/2014  First release
 *
 */
#ifndef CRADENV_C_H
#define CRADENV_C_H

#include "GenericModelEnum.h"
#include "CRadEnvEnums.h"
#include "CRadEnvModelBase.h"
#include "CRadEnvDB.h"

//------------------------------------------------------------------------------
// defines and typedefs
//------------------------------------------------------------------------------


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file: CRadEnv_c.h
 * These utility vector/data copy routines are duplicated amongst the model
 * classes, so to be enable each to be wholly independent & self-contained
 */

int RadEnvCopyDataToVector1D( double* pvdData,
                              int iNum,
                              dvector& vdData );
int RadEnvCopyDataToVector2D( double* pvvdData,
                              int iNum1,
                              int iNum2,
                              vdvector& vvdData );
int RadEnvCopyDataToVector3D( double* pvvvdData,
                              int iNum1,
                              int iNum2,
                              int iNum3,
                              vvdvector& vvvdData );
int RadEnvCopyVector1DToData( dvector& vdData,
                              int iNum,
                              double* pvdData );
int RadEnvCopyVector2DToData( vdvector& vvdData,
                              int iNum1,
                              int iNum2,
                              double* pvvdData );
int RadEnvCopyVector3DToData( vvdvector& vvvdData,
                              int iNum1,
                              int iNum2,
                              int iNum3,
                              double* pvvvdData );
// =======================================================

typedef int HANDLE;

HANDLE RadEnvStartUp(const char* szModel);

int RadEnvShutDown(HANDLE zHandle);

int RadEnvInitialize(HANDLE zHandle, const char* szDBFullPath);

int RadEnvAdjustEpoch(HANDLE zHandle,
                                        int iFixedEpochFlag,
                                        int* piFixedEpoch);

int RadEnvAdjustOrbitalTimes(
         HANDLE zHandle,
         int iUseFixedEpochFlag,
         int iAdjustSaaLongitudesFlag,
         int iNumCoord,
         double* pdaTimes,
         double* pdaLongOffsets);

int RadEnvSetActivityLevel(
         HANDLE zHandle,
         const char* szActivityLevel);

int RadEnvGetActivityLevel(
         HANDLE zHandle,
         char* szActivityLevel);

int RadEnvGetActivityRanges(
         HANDLE zHandle,
         int* piNumberRanges,
         char** psaActivityRange);

int RadEnvGetActivityRangeBounds(
         HANDLE zHandle,
         int* piaNumberActivityBounds,
         double* pdaActivityBounds);

int RadEnvGetDefaultParticleEnergyLevels(
         HANDLE zHandle,
         int* iNumEnergies,
         double* pdaDefaultEnergies);

int RadEnvGetNumDefaultParticleEnergyLevels(HANDLE zHandle);

int RadEnvSetParticleEnergyIDs(
         HANDLE zHandle,
         int aNumEnergyLevels,
         int* piaEnergyLevelIDs);

int RadEnvSetParticleEnergies(
         HANDLE zHandle,
         int iNumEnergies,
         double* pdaEnergyLevels);

int RadEnvGetParticleEnergies(
         HANDLE zHandle,
         int* piNumEnergies,
         double* pdaEnergyLevels);

int RadEnvResetParticleEnergies(HANDLE zHandle);

int RadEnvGetNumParticleEnergies(HANDLE zHandle);

int RadEnvSet15DayAP(
         HANDLE zHandle,
         double d15DayAP);

int RadEnvSetDoseChannel(
         HANDLE zHandle,
         const char* szChannel);

int RadEnvGetDefaultThicknesses(
         HANDLE zHandle,
         int* aNumThick,
         double* pdaThicknesses);

int RadEnvGetNumDefaultThicknesses(HANDLE zHandle);

int RadEnvSetCurrentThickness(
         HANDLE zHandle,
         double dThickness);

int RadEnvSetCurrentThicknessID(
         HANDLE zHandle,
         int iThicknessID);

int RadEnvGetCurrentThickness(
         HANDLE zHandle,
         double* pdThickness);

int RadEnvComputeDifferentialFlux(
         HANDLE zHandle,
         double dDl,
         double dBBeq,
         int* piNumEnergies,
         double* pdaDifferentialFlux);

int RadEnvComputeDifferentialFluxes(
         HANDLE zHandle,
         int iNumPoints,
         double* pdaDl,
         double* pdaBBeq,
         int* piNumEnergies,
         double* pdaDifferentialFlux);

int RadEnvComputeIntegralFlux(
         HANDLE zHandle,
         double dDl,
         double dBBeq,
         int* piNumEnergies,
         double* pdaIntegralFlux);

int RadEnvComputeIntegralFluxes(
         HANDLE zHandle,
         int iNumPoints,
         double* pdaDl,
         double* pdaBBeq,
         int* piNumEnergies,
         double* pdaIntegralFluxVector);

int RadEnvComputeDose(
         HANDLE zHandle,
         double dDl,
         double dBBeq,
         double* pdDose);

int RadEnvComputeDoses(HANDLE zHandle,
        int iNumPoints,
        double *pdaDl,
        double *pdaBBeq,
        double *pdaDose);

int RadEnvMessage(HANDLE zHandle, int code, char *cMessage);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // CRADENV_C_H
