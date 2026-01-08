/******************************************************************************

 File: CRadEnvModel_c.h

 Description: Declarations for methods for the calculation of flux values
   using the underlying AE8, AP8, CRRESELE or CRRESPRO models.

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

#ifndef RADENVMODEL_C_H
#define RADENVMODEL_C_H

#ifdef __cplusplus
#include "CRadEnvModel.h"

extern "C" {
#endif

typedef int HANDLE;

HANDLE RadEnvStartUp( );
int RadEnvShutDown( HANDLE zHandle );

int RadEnvSetModel( HANDLE zHandle,
                    char *szModel );// AE8 AP9 CRRESELE CRRESPRO
int RadEnvGetModel( HANDLE zHandle,
                    char *szModel );

int RadEnvSetModelDBDir( HANDLE zHandle,
                         char* szModelDBDir );
int RadEnvGetModelDBDir( HANDLE zHandle,
                         char* szModelDBDir ); // > 80 char
int RadEnvSetModelDBFile( HANDLE zHandle,
                          char* szModelDBFile );
int RadEnvGetModelDBFile( HANDLE zHandle,
                          char* szModelDBFile );
int RadEnvSetMagfieldDBFile( HANDLE zHandle,
                             char* szMagfieldDBFile );
int RadEnvGetMagfieldDBFile( HANDLE zHandle,
                             char* szMagfieldDBFile );
int RadEnvSetShieldDose2DBFile( HANDLE zHandle,
                                char* szShieldDose2DBFile );
int RadEnvGetShieldDose2DBFile( HANDLE zHandle,
                                char* szShieldDose2DBFile );

int RadEnvSetActivityLevel( HANDLE zHandle,
                            char* szActivityLevel );
int RadEnvGetActivityLevel( HANDLE zHandle,
                            char* szActivityLevel );
int RadEnvSetActivityRange( HANDLE zHandle,
                            char* szActivityRange );
int RadEnvGetActivityRange( HANDLE zHandle,
                            char* szActivityRange );
int RadEnvSet15DayAvgAp( HANDLE zHandle,
                         double d15DayAvgAp );
int RadEnvGet15DayAvgAp( HANDLE zHandle,
                         double* pd15DayAvgAp );

int RadEnvSetFixedEpoch( HANDLE zHandle,
                         int iVerdict );
int RadEnvGetFixedEpoch( HANDLE zHandle );
int RadEnvSetShiftSAA( HANDLE zHandle,
                       int iVerdict );
int RadEnvGetShiftSAA( HANDLE zHandle);

int RadEnvSetFluxType( HANDLE zHandle,
                       char* szFluxType );
int RadEnvGetFluxType( HANDLE zHandle,
                       char* szFluxType );
int RadEnvSetEnergies( HANDLE zHandle,
                       double* pvdEnergies,
                       int iNumEnergies );
int RadEnvGetNumEnergies( HANDLE zHandle );

int RadEnvSetCoordSys( HANDLE zHandle,
                       char* szCoordSys,
                       char* szCoordUnits );
int RadEnvGetCoordSys( HANDLE zHandle,
                       char* szCoordSys );
int RadEnvGetCoordSysUnits( HANDLE zHandle,
                            char* szCoordUnits );
int RadEnvSetEphemeris( HANDLE zHandle,
                        double* pvdTimes,
                        double* pvdCoord1,
                        double* pvdCoord2,
                        double* pvdCoord3,
                        int iNumTimes );
int RadEnvGetNumEphemeris( HANDLE zHandle );

int RadEnvComputeFlux( HANDLE zHandle,
                       double* pvvdFluxData );

#ifdef __cplusplus
} // extern "C"
#endif

#endif  // RADENVMODEL_C_H
