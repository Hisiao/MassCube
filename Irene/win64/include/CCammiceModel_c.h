/******************************************************************************

 File: CCammiceModel_c.h

 Description: Declarations for methods for the calculation of flux values
   using the underlying CAMMICE/MICS model.

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

#ifndef CAMMICEMODEL_C_H
#define CAMMICEMODEL_C_H

#ifdef __cplusplus
#include "CCammiceModel.h"
#include <cstring>

extern "C" {
#endif

typedef int HANDLE;

HANDLE CammiceStartUp( );
int CammiceShutDown( HANDLE zHandle );

int CammiceSetModelDBDir( HANDLE zHandle,
                          char* szModelDBDir );
int CammiceGetModelDBDir( HANDLE zHandle,
                          char* szModelDBDir ); // > 80 char
int CammiceSetModelDBFile( HANDLE zHandle,
                           char* szModelDBFile );
int CammiceGetModelDBFile( HANDLE zHandle,
        										 char* szModelDBFile );
int CammiceSetMagfieldDBFile( HANDLE zHandle,
                              char* szMagfieldDBFile );
int CammiceGetMagfieldDBFile( HANDLE zHandle,
                              char* szMagfieldDBFile );
int CammiceSetMagfieldModel( HANDLE zHandle,
                             char* szMFModel );
int CammiceGetMagfieldModel( HANDLE zHandle,
                             char* szMFModel );

int CammiceSetDataFilter( HANDLE zHandle,
                          char* szDataFiter );
int CammiceGetDataFilter( HANDLE zHandle,
                          char* szDataFiter );
int CammiceSetPitchAngleBin( HANDLE zHandle,
                             char* szPitchAngleBin );
int CammiceGetPitchAngleBin( HANDLE zHandle,
                             char* szPitchAngleBin );
int CammiceSetSpecies( HANDLE zHandle,
                       char* szSpecies );
int CammiceGetSpecies( HANDLE zHandle,
                       char* szSpecies );


int CammiceSetCoordSys( HANDLE zHandle,
                        char* szCoordSys,
                        char* szCoordUnits );
int CammiceGetCoordSys( HANDLE zHandle,
                        char* szCoordSys );
int CammiceGetCoordSysUnits( HANDLE zHandle,
                             char* szCoordSysUnits );

int CammiceSetEphemeris( HANDLE zHandle,
                         double* pvdTimes,
                         double* pvdCoord1,
                         double* pvdCoord2,
                         double* pvdCoord3,
                         int iNumTimes );
int CammiceGetNumEphemeris( HANDLE zHandle );

//  Cammice model always returns fluxes for *12* pre-defined energy bins
int CammiceComputeFlux( HANDLE zHandle,
                        double* pvvdFluxData );

#ifdef __cplusplus
} // extern "C"
#endif

#endif  // CAMMICEMODEL_C_H
