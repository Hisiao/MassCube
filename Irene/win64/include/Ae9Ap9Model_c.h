/******************************************************************************

File: Ae9Ap9Model_c.h

Description: Main entry point into the model layer of the Ae9Ap9 project
  The Application layer classes interact with the model layer through an
  Ae9Ap9Model object. This class retrieves its initial state from the 
  persistant data repository, then manages the data for subsequent 
  requests and calculations in-memory.

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

  Version      Date       Notes
  1.0         10/11/2017  created 
  
******************************************************************************/
#ifndef AE9AP9MODEL_C_H
#define AE9AP9MODEL_C_H

#ifdef __cplusplus
#include "Ae9Ap9Model.h"

extern "C" {
#endif

typedef int HANDLE;

HANDLE Ae9Ap9StartUp( );
int Ae9Ap9ShutDown( HANDLE zHandle );

int Ae9Ap9SetModel( HANDLE zHandle,
                    char* szModel );
int Ae9Ap9GetModel( HANDLE zHandle,
                    char* szModel ); //  16 char

int Ae9Ap9SetModelDBDir( HANDLE zHandle,
                         char* szModelDBDir );
int Ae9Ap9GetModelDBDir( HANDLE zHandle,
                         char* szModelDBDir ); // > 80 char

int Ae9Ap9SetModelDBFile( HANDLE zHandle,
                          char* szModelDBFile );
int Ae9Ap9GetModelDBFile( HANDLE zHandle,
                          char* szModelDBFile ); // > 80 char

int Ae9Ap9GetModelName( HANDLE zHandle,
                        char* szModelName ); // > 8 char
int Ae9Ap9GetModelSpecies( HANDLE zHandle,
                           char* szModelSpecies ); // > 16 char

int Ae9Ap9SetKPhiDBFile( HANDLE zHandle,
                         char* szKPhiDBFile );
int Ae9Ap9GetKPhiDBFile( HANDLE zHandle,
                         char* szKPhiDBFile ); // > 80 char

int Ae9Ap9SetKHMinDBFile( HANDLE zHandle,
                          char* szKHMinDBFile );
int Ae9Ap9GetKHMinDBFile( HANDLE zHandle,
                          char* szKHMinDBFile ); // > 80 char

int Ae9Ap9SetMagfieldDBFile( HANDLE zHandle,
                             char* szMagfieldDBFile );
int Ae9Ap9GetMagfieldDBFile( HANDLE zHandle,
                             char* szMagfieldDBFile ); // > 80 char

int Ae9Ap9LoadModelDB( HANDLE zHandle );

        // Set environment for omni-directional flux
int Ae9Ap9SetFluxEnvironmentOmni( HANDLE zHandle,
                                  char* szFluxType, 
                                  double* pvdEnergies, // 1-d energy list
                                  double* pvdEnergies2, // 1-d energy list
                                  double* pvdTimes,
                                  char* szCoordSys,
                                  char* szCoordUnits,
                                  double* pvdCoord1,
                                  double* pvdCoord2, 
                                  double* pvdCoord3, 
                                  int iNumTimes,
                                  int iNumEnergies );

        // Set environment for fixed set of pitch angles
int Ae9Ap9SetFluxEnvironmentFixPitch( HANDLE zHandle,
                                      char* szFluxType, 
                                      double* pvdEnergies, // 1-d energy list
                                      double* pvdEnergies2, // 1-d energy list
                                      double* pvdTimes, 
                                      char* szCoordSys,
                                      char* szCoordUnits,
                                      double* pvdCoord1,
                                      double* pvdCoord2, 
                                      double* pvdCoord3,
                                      double* pvdPitchAngles,
                                      int iNumTimes,
                                      int iNumEnergies,
                                      int iNumDir );

        // Set environment for directional flux
int Ae9Ap9SetFluxEnvironmentDirVec( HANDLE zHandle,
                                    char* szFluxType, 
                                    double* pvdEnergies, // 1-d energy list
                                    double* pvdEnergies2, // 1-d energy list
                                    double* pvdTimes, 
                                    char* szCoordSys,
                                    char* szCoordUnits,
                                    double* pvdCoord1,
                                    double* pvdCoord2, 
                                    double* pvdCoord3,
                                    double* pvvdFluxDir1, 
                                    double* pvvdFluxDir2,
                                    double* pvvdFluxDir3,
                                    int iNumTimes,
                                    int iNumEnergies,
                                    int iNumDir );

        // Set environment for ephemeris-associated set of pitch angles
int Ae9Ap9SetFluxEnvironmentVarPitch( HANDLE zHandle,
                                      char* szFluxType, 
                                      double* pvdEnergies, // 1-d energy list
                                      double* pvdEnergies2, // 1-d energy list
                                      double* pvdTimes, 
                                      char* szCoordSys,
                                      char* szCoordUnits,
                                      double* pvdCoord1,
                                      double* pvdCoord2, 
                                      double* pvdCoord3,
                                      double* pvvdPitchAngles,
                                      int iNumTimes,
                                      int iNumEnergies,
                                      int iNumDir );

int Ae9Ap9GetNumTimes( HANDLE zHandle );
int Ae9Ap9GetNumEnergies( HANDLE zHandle );
int Ae9Ap9GetNumDirections( HANDLE zHandle );

        // retrieve pitch angle values (particularly useful when direction vectors specified)
int Ae9Ap9GetPitchAngles( HANDLE zHandle,
                          double* pvvdPitchAngles );

        // Compute flux for a mean flux environment (date,energy,direction)
int Ae9Ap9ComputeFluxMean( HANDLE zHandle,
                           double* pvvvdFluxData );
int Ae9Ap9FlyinMean( HANDLE zHandle,
                     double* pvvvdFluxData );

        // Compute flux for a statistically perturbed, time invariant mean flux environment (date,energy,direction)
int Ae9Ap9ComputeFluxPerturbedMean( HANDLE zHandle,
                                    int iScenario,  // 1..999
                                    double* pvvvdFluxData );
int Ae9Ap9FlyinPerturbedMean( HANDLE zHandle,
                              int iScenario,  // 1..999
                              double* pvvvdFluxData );


        // Compute flux for a particular percentile flux environment (date,energy,direction)
int Ae9Ap9ComputeFluxPercentile( HANDLE zHandle,
                                 int iPercentile,  // 1..99
                                 double* pvvvdFluxData );
int Ae9Ap9FlyinPercentile( HANDLE zHandle,
                           int iPercentile,  // 1..99
                           double* pvvvdFluxData );

        // Compute flux for monte carlo (time variant) scenario flux environments (date,energy,direction)
int Ae9Ap9ComputeFluxScenario( HANDLE zHandle,
                               double dEpochTime,
                               int iScenario,  // 1..999
                               double* pvvvdFluxData,
                               int iPerturbFlux );
int Ae9Ap9FlyinScenario( HANDLE zHandle,
                         double dEpochTime,
                         int iScenario,  // 1..999
                         double* pvvvdFluxData,
                         int iPerturbFlux );

  // Retrieves a list of pitch angles used in the computation of omnidirectional flux
int Ae9Ap9GetDefaultPitchAngles( HANDLE zHandle,
                                 double* pvdPitchAngles ); // >32
        //  returns number of angles


#ifdef __cplusplus
} // extern "C"
#endif

#endif  // AE9AP9MODEL_C_H
