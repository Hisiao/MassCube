/*
 *  Module: CMagfield_c.h
 *
 *  Description:
 *
 *      C interface methods to the CMagfield class. Modified for pointer use to
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
 *      1.0         10/13/2011  First release
 *
 */
#ifndef CMAGFIELD_C_H
#define CMAGFIELD_C_H

#include "CMagfield.h"

//------------------------------------------------------------------------------
// defines and typedefs
//------------------------------------------------------------------------------

/*
 * CMagfield Options Restrictions
 *
 * These restrictions are relevant for field line computations. Other options
 * combinations may be problematic for other CMagfield functions.
 *
 * 1. Dipole fields (emfOffsetDipole & emfTiltedDipole) should not have an
 *    external field set.
 *
 * 2. IGRF fields (emfIGRF & emfFastIGRF) can have an external field, but only
 *    the Olson-Pfitzer model (eefOlsonPfitzer) has been through a thorough V&V.
 *
 * 3. The Offset Dipole model (emfOffsetDipole) cannot use McIlwain
 *    (elmMcIlwain) as the L-shell mode. For the Tilted Dipole
 *    (emfTiltedDipole), either option is allowed, but the computed L-shell is
 *    the same for either case.
 *
 * 4. Dipole fields (emfOffsetDipole & emfTiltedDipole) cannot have trace
 *    direction set to equatorward (efltGsmEquator).
 *
 * 5. Field line trace altitude should be chosen >= -100 km for IGRF fields.
 *    For all main fields, the trace altitude has no upper limit, but no field
 *    line will be generated if an input point lies below the altitude limit.
 */

/**
 * @file: CMagfield_c.h
 * CMagfield class C interface function definitions
 */

#ifdef __cplusplus
extern "C" {
#endif

// =======================================================
// These utility vector/data copy routines are duplicated amongst the model
//  classes, so to be enable each to be wholly independent & self-contained
int MagfieldCopyDataToVector1D( double* pvdData,
                                int iNum,
                                dvector& vdData );
int MagfieldCopyDataToVector2D( double* pvvdData,
                                int iNum1,
                                int iNum2,
                                vdvector& vvdData );
int MagfieldCopyDataToVector3D( double* pvvvdData,
                                int iNum1,
                                int iNum2,
                                int iNum3,
                                vvdvector& vvvdData );
int MagfieldCopyVector1DToData( dvector& vdData,
                                int iNum,
                                double* pvdData );
int MagfieldCopyVector2DToData( vdvector& vvdData,
                                int iNum1,
                                int iNum2,
                                double* pvvdData );
int MagfieldCopyVector3DToData( vvdvector& vvvdData,
                                int iNum1,
                                int iNum2,
                                int iNum3,
                                double* pvvvdData );
// =======================================================

typedef int HANDLE;

HANDLE MagfieldStartUp();

int MagfieldShutDown(HANDLE zHandle);

int MagfieldInitialize(HANDLE zHandle, const char* szDBFullPath);

int MagfieldInitializeDate(HANDLE zHandle,
        const char* szDBFullPath,
        double dDateTime);

int MagfieldSetIgrfUpdateRate(HANDLE zHandle, double dRateIn);

int MagfieldGetIgrfUpdateRate(HANDLE zHandle, double* pdRateOut);

int MagfieldSetSr2UpdateRate(HANDLE zHandle, double dRateIn);

int MagfieldGetSr2UpdateRate(HANDLE zHandle, double* pdRateOut);

int MagfieldUpdateTime(HANDLE zHandle, double dDateTimeIn);

int MagfieldForceSetTime(HANDLE zHandle, double dDateTimeIn);

int MagfieldForceUpdateSR2(HANDLE zHandle, double dDateTime);

int MagfieldGetCurIgrfTime(HANDLE zHandle, double* pdDateTimeOut);

int MagfieldGetCurSr2Time(HANDLE zHandle, double* pdDateTimeOut);

int MagfieldOverrideYear(HANDLE zHandle, int iYearIn);

int MagfieldGetOverrideYear(HANDLE zHandle, int* piYearOut);

int MagfieldSetKpValue( HANDLE zHandle,
                        double dKpVal );

int MagfieldSetKpValues( HANDLE zHandle,
                         double dTimeRef,
                         double* pdaKpVals,
                         int iNumEntries );

int MagfieldGetKpValue( HANDLE zHandle,
                        double* pdKpVal );

int MagfieldGetKpValuesRefTime( HANDLE zHandle,
                                double* pdRefTime );

int MagfieldGetKpValuesEndTime( HANDLE zHandle,
                                double* pdEndTime );

int MagfieldGetDipParams(HANDLE zHandle,
        double* pdDimo,
        double* pdTheta,
        double* pdPhi,
        double* pdEta,
        double* pdZeta,
        double* pdXi,
        double* pdGeoToMag);

int MagfieldSetMainField(HANDLE zHandle, const char* szMFIn);

int MagfieldGetMainField(HANDLE zHandle, char* szMFOut);

int MagfieldGetDBfp(HANDLE zHandle, char* szFilePathOut);

int MagfieldSetExternalField(HANDLE zHandle, const char* szEFIn);

int MagfieldGetExternalField(HANDLE zHandle, char* szEFOut);

int MagfieldGetInitialization(HANDLE zHandle, int* piStateOut);

int MagfieldComputeBfieldDate(HANDLE zHandle,
        double dDateTime,
        double dCX,
        double dCY,
        double dCZ,
        double* pdBX,
        double* pdBY,
        double* pdBZ,
        double* pdBmag);

int MagfieldComputeBfield(HANDLE zHandle,
        double dCX,
        double dCY,
        double dCZ,
        double* pdBX,
        double* pdBY,
        double* pdBZ,
        double* pdBmag);

int MagfieldComputeBfields(HANDLE zHandle,
        int iNumPoints,
        double* pdaCX,
        double* pdaCY,
        double* pdaCZ,
        double* pdaBX,
        double* pdaBY,
        double* pdaBZ,
        double* pdaBmag);

int MagfieldComputeBfieldDates(HANDLE zHandle,
        int iNumPoints,
        double* pdaDateTime,
        double* pdaCX,
        double* pdaCY,
        double* pdaCZ,
        double* pdaBX,
        double* pdaBY,
        double* pdaBZ,
        double* pdaBmag);

int MagfieldComputeLdipDate(HANDLE zHandle,
        double dDateTime,
        double dCX,
        double dCY,
        double dCZ,
        double* pdLdip);

int MagfieldComputeLmDate(HANDLE zHandle,
        double dDateTime,
        double dCX,
        double dCY,
        double dCZ,
        double* pdLm);

int MagfieldComputeLdip(HANDLE zHandle,
        double dCX,
        double dCY,
        double dCZ,
        double* pdLdip);

int MagfieldComputeLdips(HANDLE zHandle,
        int iNumPoints,
        double* pdaCX,
        double* pdaCY,
        double* pdaCZ,
        double* pdaLdip);

int MagfieldComputeLdipDates(HANDLE zHandle,
        int iNumPoints,
        double* pdaDateTime,
        double* pdaCX,
        double* pdaCY,
        double* pdaCZ,
        double* pdaLdip);

int MagfieldComputeLm(HANDLE zHandle,
        double dCX,
        double dCY,
        double dCZ,
        double* pdLm);

int MagfieldComputeLms(HANDLE zHandle,
        int iNumPoints,
        double* pdaCX,
        double* pdaCY,
        double* pdaCZ,
        double* pdaLm);

int MagfieldComputeLmDates(HANDLE zHandle,
        int iNumPoints,
        double* pdaDateTime,
        double* pdaCX,
        double* pdaCY,
        double* pdaCZ,
        double* pdaLm);

int MagfieldComputeLBBeqDate(HANDLE zHandle,
        double dDateTime,
        double dCX,
        double dCY,
        double dCZ,
        double* pdLm,
        double* pdBBeq);

int MagfieldComputeLBBeq(HANDLE zHandle,
        double dCX,
        double dCY,
        double dCZ,
        double* pdLm,
        double* pdBBeq);

int MagfieldComputeLBlocalBeqDate(HANDLE zHandle,
        double dDateTime,
        double dCX,
        double dCY,
        double dCZ,
        double* dLm,
        double* dBmag,
        double* dBeq);

int MagfieldComputeLBlocalBeq(HANDLE zHandle,
        double dCX,
        double dCY,
        double dCZ,
        double* pdLm,
        double* pdBmag,
        double* pdBeq);

int MagfieldComputeLBBeqs(HANDLE zHandle,
        int iNumPoints,
        double* pdaCX,
        double* pdaCY,
        double* pdaCZ,
        double* pdaLm,
        double* pdaBBeq);

int MagfieldComputeLBBeqDates(HANDLE zHandle,
        int iNumPoints,
        double* pdaDateTime,
        double* pdaCX,
        double* pdaCY,
        double* pdaCZ,
        double* pdaLm,
        double* padBBeq);

int MagfieldComputeAllBoutDate(HANDLE zHandle,
        double dDateTime,
        double dCX,
        double dCY,
        double dCZ,
        double* pdBX,
        double* pdBY,
        double* pdBZ,
        double* pdBmag,
        double* pdLdip,
        double* pdLm,
        double* pdBBeq);

int MagfieldComputeAllBout(HANDLE zHandle,
        double dCX,
        double dCY,
        double dCZ,
        double* pdBX,
        double* pdBY,
        double* pdBZ,
        double* pdBmag,
        double* pdLdip,
        double* pdLm,
        double* pdBBeq);

int MagfieldComputeAllBouts(HANDLE zHandle,
        int iNumPoints,
        double* pdaCX,
        double* pdaCY,
        double* pdaCZ,
        double* pdaBX,
        double* pdaBY,
        double* pdaBZ,
        double* pdaBmag,
        double* pdaLdip,
        double* pdaLm,
        double* pdaBBeq);

int MagfieldComputeAllBoutDates(HANDLE zHandle,
        int iNumPoints,
        double* pdaDateTime,
        double* pdaCX,
        double* pdaCY,
        double* pdaCZ,
        double* pdaBX,
        double* pdaBY,
        double* pdaBZ,
        double* pdaBmag,
        double* pdaLdip,
        double* pdaLm,
        double* pdaBBeq);

int MagfieldComputeFieldLineDate(HANDLE zHandle,
        int iBufferSize,
        double dDateTime,
        double dCX,
        double dCY,
        double dCZ,
        const char* szDirIn, // eFieldLineTraceDir
        const char* szModeIn, // eLshellMode
        double dTraceAlt,
        int*    piNumFLPoints,
        double* pdaFLX,
        double* pdaFLY,
        double* pdaFLZ,
        double* pdL);

int MagfieldComputeFieldLine(HANDLE zHandle,
        int iBufferSize,
        double dCX,
        double dCY,
        double dCZ,
        const char* szDirIn, // eFieldLineTraceDir
        const char* szModeIn, // eLshellMode
        double dTraceAlt,
        int*    piNumFLPoints,
        double* pdaFLX,
        double* pdaFLY,
        double* pdaFLZ,
        double* pdL);

//#ifdef GEOSPACE_RELICS
//  *** These routines may need a
//      serious re-examination if they are ever actually again

int MagfieldComputeMlatFieldLinesDate(HANDLE zHandle,
        int iMaxTotalPointsReturned,
        double dDateTime,
        double dMLT,
        double dMLAT0,
        double dMLAT1,
        int iNumFLs,
        const char* szModeIn, // eLshellMode
        double* pdFLX,
        double* pdFLY,
        double* pdFLZ,
        double* pdL,
        int* piPtsPerLine,
        int* piErrors);

int MagfieldComputeMltFieldLinesDate(HANDLE zHandle,
        int iBufferSize,
        double dDateTime,
        double dMLAT,
        double dMLT0,
        double dMLT1,
        int iNumFLs,
        const char* szModeIn, // eLshellMode
        double* pdaFLX,
        double* pdaFLY,
        double* pdaFLZ,
        double* pdaL,
        int* piaPtsPerLine,
        int* piaErrors);

int MagfieldComputeMlatFieldLines(HANDLE zHandle,
        int iBufferSize,
        double dMLT,
        double dMLAT0,
        double dMLAT1,
        int iNumFLs,
        const char* szModeIn, // eLshellMode
        double* pdaFLX,
        double* pdaFLY,
        double* pdaFLZ,
        double* pdavL,
        int* piaPtsPerLine,
        int* piaErrors);

int MagfieldComputeMltFieldLines(HANDLE zHandle,
        int iBufferSize,
        double dMLAT,
        double dMLT0,
        double dMLT1,
        int iNumFLs,
        const char* szModeIn, // eLshellMode
        double* pdaFLX,
        double* pdaFLY,
        double* pdaFLZ,
        double* pdaL,
        int* piaPtsPerLine,
        int* piaErrors);

int MagfieldComputeFluxTubeFieldLinesGeoDate(HANDLE zHandle,
        int iBufferSize,
        double dDateTime,
        double dLat,
        double dLon,
        double dAltKm,
        double dFluxTubeDiamKm,
        int iNumFLs,
        const char* szModeIn, // eLshellMode
        double* pdaFLX,
        double* pdaFLY,
        double* pdaFLZ,
        double* pdaL,
        int* piaPtsPerLine,
        int* piaErrors);
int MagfieldComputeFluxTubeFieldLinesMagDate(HANDLE zHandle,
        int iBufferSize,
        double dDateTime,
        double dMLat,
        double dMLon,
        double dRadRE,
        double dFluxTubeDiamDeg,
        int iNumFLs,
        const char* szModeIn, // eLshellMode
        double* pdaFLsX,
        double* pdaFLsY,
        double* pdaFLsZ,
        double* pdaL,
        int* piaPtsPerLine,
        int* piaErrors);

int MagfieldComputeFluxTubeFieldLinesGeo(HANDLE zHandle,
        int iBufferSize,
        double dLat,
        double dLon,
        double dAltKm,
        double dFluxTubeDiamKm,
        int iNumFLs,
        const char* szModeIn, // eLshellMode
        double* pdaFLsX,
        double* pdaFLsY,
        double* pdaFLsZ,
        double* pdaL,
        int* piaPtsPerLine,
        int* piaErrors);

int MagfieldComputeFluxTubeFieldLinesMag(HANDLE zHandle,
        int iBufferSize,
        double dMLat,
        double dMLon,
        double dRadRE,
        double dFluxTubeDiamDeg,
        int iNumFLs,
        const char* szModeIn, // eLshellMode
        double* pdaFLsX,
        double* pdaFLsY,
        double* pdaFLsZ,
        double* pdaL,
        int* piaPtsPerLine,
        int* piaErrors);
//#endif  // GEOSPACE_RELICS

int MagfieldComputeFootPrintDate(HANDLE zHandle,
        double dDateTime,
        double dCX,
        double dCY,
        double dCZ,
        double dAltSurf,
        const char* szDirIn, // eFieldLineTraceDir
        const char* szModeIn, // eLshellMode
        double* pdaFPX,
        double* pdaFPY,
        double* pdaFPZ,
        double* pdL);

int MagfieldComputeFootPrint(HANDLE zHandle,
        double dCX,
        double dCY,
        double dCZ,
        double dAltSurf,
        const char* szDirIn, // eFieldLineTraceDir
        const char* szModeIn, // eLshellMode
        double* pdaFPsX,
        double* pdaFPsY,
        double* pdaFPsZ,
        double* pdL);

int MagfieldComputeIsConjugateDate(HANDLE zHandle,
        double dDateTime,
        double dC1X,
        double dC1Y,
        double dC1Z,
        double dC2X,
        double dC2Y,
        double dC2Z,
        double dTolerance,
        const char* szModeIn, // eLshellMode
        double* pdaLs,
        int* piIsConj);

int MagfieldComputeIsConjugate(HANDLE zHandle,
        double dC1X,
        double dC1Y,
        double dC1Z,
        double dC2X,
        double dC2Y,
        double dC2Z,
        double dTolerance,
        const char* szModeIn, // eLshellMode
        double* pdaLs,
        int* piIsConj);

int MagfieldMessage1(HANDLE zHandle,
        int code,
        char* cMessage);

int MagfieldMessage2(HANDLE zHandle,
        char* cMessage);

int MagfieldComputeAdInvariants(HANDLE zHandle,
        double dTime,
        double dRadKm,
        double dLat,
        double dLon,
        double* pdPitchAngles,
        int iNumPitchAngles,
        double* pdBmin,
        double* pdaBvec,
        double* pdaLm,
        double* pdaXj);

int MagfieldConvertCoord(HANDLE zHandle,
        const char* szInCoordSys,
        const char* szInCoordSysUnits,
        const char* szOutCoordSys,
        const char* szOutCoordSysUnits,
        double dcInX,
        double dcInY,
        double dcInZ,
        double* pdcOutX,
        double* pdcOutY,
        double* pdcOutZ);

int MagfieldConvertGEOtoSPH(HANDLE zHandle,
        double dcGeoX,
        double dcGeoY,
        double dcGeoZ,
        double* pdcSphR,
        double* pdcSphLAT,
        double* pdcSphLON);

int MagfieldConvertSPHtoGEO(HANDLE zHandle,
        double dcSphR,
        double dcSphLAT,
        double dcSphLON,
        double* pdcGeoX,
        double* pdcGeoY,
        double* pdcGeoZ);

int MagfieldConvertGEOtoGSM(HANDLE zHandle,
        double dcGeoX,
        double dcGeoY,
        double dcGeoZ,
        double* pdcGsmX,
        double* pdcGsmY,
        double* pdcGsmZ);

int MagfieldConvertGSMtoGEO(HANDLE zHandle,
        double dcGsmX,
        double dcGsmY,
        double dcGsmZ,
        double* pdcGeoX,
        double* pdcGeoY,
        double* pdcGeoZ);

int MagfieldConvertGEOtoMAG(HANDLE zHandle,
        double dcGeoX,
        double dcGeoY,
        double dcGeoZ,
        double* pdcMagX,
        double* pdcMagY,
        double* pdcMagZ);

int MagfieldConvertMAGtoGEO(HANDLE zHandle,
        double dcMagX,
        double dcMagY,
        double dcMagZ,
        double* pdcGeoX,
        double* pdcGeoY,
        double* pdcGeoZ);

int MagfieldConvertGEOtoSPHs(HANDLE zHandle,
        int iNumPoints,
        double* pdacGeoX,
        double* pdacGeoY,
        double* pdacGeoZ,
        double* pdacSphR,
        double* pdacSphLAT,
        double* pdacSphLON);

int MagfieldConvertSPHtoGEOs(HANDLE zHandle,
        int iNumPoints,
        double* pdacSphR,
        double* pdacSphLAT,
        double* pdacSphLON,
        double* pdacGeoX,
        double* pdacGeoY,
        double* pdacGeoZ);

int MagfieldConvertGEOtoGSMs(HANDLE zHandle,
        int iNumPoints,
        double* pdacGeoX,
        double* pdacGeoY,
        double* pdacGeoZ,
        double* pdacGsmX,
        double* pdacGsmY,
        double* pdacGsmZ);

int MagfieldConvertGSMtoGEOs(HANDLE zHandle,
        int iNumPoints,
        double* pdacGsmX,
        double* pdacGsmY,
        double* pdacGsmZ,
        double* pdacGeoX,
        double* pdacGeoY,
        double* pdacGeoZ);

int MagfieldConvertGEOtoMAGs(HANDLE zHandle,
        int iNumPoints,
        double* pdacGeoX,
        double* pdacGeoY,
        double* pdacGeoZ,
        double* pdacMagX,
        double* pdacMagY,
        double* pdacMagZ);

int MagfieldConvertMAGtoGEOs(HANDLE zHandle,
        int iNumPoints,
        double* pdacMagX,
        double* pdacMagY,
        double* pdacMagZ,
        double* pdacGeoX,
        double* pdacGeoY,
        double* pdacGeoZ);

int MagfieldConvertGEOtoSPHDates(HANDLE zHandle,
        int iNumPoints,
        double* pdaDateTime,
        double* pdacGeoX,
        double* pdacGeoY,
        double* pdacGeoZ,
        double* pdacSphR,
        double* pdacSphLAT,
        double* pdacSphLON);

int MagfieldConvertSPHtoGEODates(HANDLE zHandle,
        int iNumPoints,
        double* pdaDTime,
        double* pdacSphR,
        double* pdacSphLAT,
        double* pdacSphLON,
        double* pdacGeoX,
        double* pdacGeoY,
        double* pdacGeoZ);
int MagfieldConvertGEOtoGSMDates(HANDLE zHandle,
        int iNumPoints,
        double* pdaDTime,
        double* pdacGeoX,
        double* pdacGeoY,
        double* pdacGeoZ,
        double* pdacGsmX,
        double* pdacGsmY,
        double* pdacGsmZ);

int MagfieldConvertGSMtoGEODates(HANDLE zHandle,
        int iNumPoints,
        double* pdaDTime,
        double* pdacGsmX,
        double* pdacGsmY,
        double* pdacGsmZ,
        double* pdacGeoX,
        double* pdacGeoY,
        double* pdacGeoZ);

int MagfieldConvertGEOtoMAGDates(HANDLE zHandle,
        int iNumPoints,
        double* pdaDTime,
        double* pdacGeoX,
        double* pdacGeoY,
        double* pdacGeoZ,
        double* pdacMagX,
        double* pdacMagY,
        double* pdacMagZ);

int MagfieldConvertMAGtoGEODates(HANDLE zHandle,
        int iNumPoints,
        double* pdaDTime,
        double* pdacMagX,
        double* pdacMagY,
        double* pdacMagZ,
        double* pdacGeoX,
        double* pdacGeoY,
        double* pdacGeoZ);

int MagfieldGetFieldLineLocation( HANDLE zHandle,
        int iNumFLPoints,
        double* pdaFLX,
        double* pdaFLY,
        double* pdaFLZ,
        double dTraceAlt,
        const char* szTraceDir, // eFieldLineTraceDir
        double* pdaFPX,
        double* pdaFPY,
        double* pdaFPZ );

#ifdef __cplusplus
} // extern "C"
#endif

#endif // CMAGFIELD_C_H
