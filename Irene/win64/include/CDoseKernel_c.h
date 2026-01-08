/******************************************************************************
$HeadURL$

 File: CDoseKernel_c.h

 Description: Declarations for methods for the calculation of dosage rates
   and/or accumulations, employing the underlying Dose Kernel model.

 Classification:

   Unclassified

 Project Name:

   AE9/AP9/SPM Radiation Environment Models

   Developed under US Government contract # FA9453-12-C-0231

 Rights and Restrictions:

   Copyright 2018 Atmospheric and Environmental Research, Inc. (AER)

   DISTRIBUTION A. Approved for public release; distribution is unlimited.

   The AE9/AP9/SPM software license is contained in the 'documents/Licenses' 
   folder of this distribution file collection.

 Author:

   This software was developed by AER staff

 Contact:

   Atmospheric and Environmental Research, Inc.
   131 Hartwell Avenue
   Lexington, MA 02421-3126 USA
   Phone: 781.761.2288
   email: spwx@aer.com

 References:
 
   None

 Revision history:

  Version      Date        Notes
  1.0          01/25/2018  Created 
  
SVNTag: $Id$
******************************************************************************/

#ifndef DOSEKERNEL_C_H
#define DOSEKERNEL_C_H

#ifdef __cplusplus
#include "CDoseKernel.h"

extern "C" {
#endif

typedef int HANDLE;

HANDLE DoseKStartUp( );
int DoseKShutDown( HANDLE zHandle );

int DoseKSetKernelXmlPath( HANDLE zHandle,
                           char* szXmlPath );
int DoseKGetKernelXmlPath( HANDLE zHandle,
                           char* szXmlPath );

int DoseKSetKernelXmlFile( HANDLE zHandle,
                           char* szXmlFile );
int DoseKGetKernelXmlFile( HANDLE zHandle,
                           char* szXmlFile );

int DoseKSetSpecies( HANDLE zHandle,
                     char* szSpecies );
int DoseKGetSpecies( HANDLE zHandle,
                     char* szSpecies );

int DoseKSetEnergies( HANDLE zHandle,
                      double* pvdEnergies,
                      int iNumEnergies,
                      char* szEnergyUnits );
int DoseKGetNumEnergies( HANDLE zHandle );

int DoseKSetDepths( HANDLE zHandle,
                    double* pvdDepths,
                    int iNumDepths,
                    char* szDepthUnits );
int DoseKGetNumDepths( HANDLE zHandle );

int DoseKSetDetector( HANDLE zHandle,
                      char* szDetector );
int DoseKGetDetector( HANDLE zHandle,
                      char* szDetector );
int DoseKSetGeometry( HANDLE zHandle,
                      char* szGeometry );
int DoseKGetGeometry( HANDLE zHandle,
                      char* szGeometry );
int DoseKSetNuclearAttenMode( HANDLE zHandle,
                              char* szNucAttenMode );
int DoseKGetNuclearAttenMode( HANDLE zHandle,
                              char* szNucAttenMode );
int DoseKSetWithBrems( HANDLE zHandle,
                       int iVerdict );
int DoseKGetWithBrems( HANDLE zHandle );

    // 3D or 2D input/output flux[time][energy][dir=1] -> doserate[time][depth][dir=1]
int DoseKComputeFluxDoseRate ( HANDLE zHandle,
                               double* pvvvdFluxData,
                               int iNumTimes,
                               int iNumEnergies,
                               double* pvvvdDoseRate );
    // 3D or 2D input/output fluence[time][energy][dir=1] -> dose[time][depth][dir=1]
int DoseKComputeFluenceDose ( HANDLE zHandle,
                              double* pvvvdFluenceData,
                              int iNumTimes,
                              int iNumEnergies,
                              double* pvvvdDoseVal );

    // 1d input/output flux[energy] -> doserate[depth]
int DoseKComputeFluxDose( HANDLE zHandle,
                          double* pvdFluxData,
                          int iNumEnergies,
                          double* pvdDoseData );


#ifdef __cplusplus
} // extern "C"
#endif

#endif  // DOSEKERNEL_C_H

