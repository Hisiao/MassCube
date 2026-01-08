/******************************************************************************

 File: CDoseModel_c.h

 Description: Declarations for methods for the calculation of dosage rates
   and/or accumulations, employing the underlying ShieldDose2 model.

 Classification:

   Unclassified

 Project Name:

   AE9/AP9/SPM Radiation Environment Models

   Developed under US Government contract # FA9453-12-C-0231

 Rights and Restrictions:

   Copyright 2017 Atmospheric and Environmental Research, Inc. (AER)

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
  1.0          10/24/2017  Created 
  
******************************************************************************/

#ifndef DOSEMODEL_C_H
#define DOSEMODEL_C_H

#ifdef __cplusplus
#include "CDoseModel.h"

extern "C" {
#endif

typedef int HANDLE;

HANDLE DoseStartUp( );
int DoseShutDown( HANDLE zHandle );

int DoseSetModelDBDir( HANDLE zHandle,
                       char* szModelDBDir );
int DoseGetModelDBDir( HANDLE zHandle,
                       char* szModelDBDir ); // > 80 char

int DoseSetModelDBFile( HANDLE zHandle,
                        char* szModelDBFile );
int DoseGetModelDBFile( HANDLE zHandle,
                        char* szModelDBFile );

int DoseSetSpecies( HANDLE zHandle,
                    char* szSpecies );
int DoseGetSpecies( HANDLE zHandle,
                    char* szSpecies );

int DoseSetEnergies( HANDLE zHandle,
                     double* pvdEnergies,
                     int iNumEnergies,
                     char* szEnergyUnits );
int DoseGetNumEnergies( HANDLE zHandle );

int DoseSetDepths( HANDLE zHandle,
                   double* pvdDepths,
                   int iNumDepths,
                   char* szDepthUnits );
int DoseGetNumDepths( HANDLE zHandle );

int DoseSetDetector( HANDLE zHandle,
                    char* szDetector );
int DoseGetDetector( HANDLE zHandle,
                    char* szDetector );
int DoseSetGeometry( HANDLE zHandle,
                     char* szGeometry );
int DoseGetGeometry( HANDLE zHandle,
                     char* szGeometry );
int DoseSetNuclearAttenMode( HANDLE zHandle,
                             char* szNucAttenMode );
int DoseGetNuclearAttenMode( HANDLE zHandle,
                             char* szNucAttenMode );
int DoseSetWithBrems( HANDLE zHandle,
                      int iVerdict );
int DoseGetWithBrems( HANDLE zHandle );
int DoseSetLinearInterp( HANDLE zHandle,
                         int iVerdict );
int DoseGetLinearInterp( HANDLE zHandle );

    // 3D or 2D input/output flux[time][energy][dir=1] -> doserate[time][depth][dir=1]
int DoseComputeFluxDoseRate ( HANDLE zHandle,
                              double* pvvvdFluxData,
                              int iNumTimes,
                              int iNumEnergies,
                              double* pvvvdDoseRate );
    // 3D or 2D input/output fluence[time][energy][dir=1] -> dose[time][depth][dir=1]
int DoseComputeFluenceDose ( HANDLE zHandle,
                             double* pvvvdFluenceData,
                             int iNumTimes,
                             int iNumEnergies,
                             double* pvvvdDoseVal );

    // 1d input/output flux[energy] -> doserate[depth]
int DoseComputeFluxDose( HANDLE zHandle,
                         double* pvdFluxData,
                         int iNumEnergies,
                         double* pvdDoseData );


#ifdef __cplusplus
} // extern "C"
#endif

#endif  // DOSEMODEL_C_H

