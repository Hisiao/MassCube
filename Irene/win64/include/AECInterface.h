/******************************************************************************
$HeadURL$

File: AECInterface.h

Description: This header file contains the declarations of the C-interace methods to the 
Ae9Ap9Application class. These methods provide access from C or Fortran code to the 
methods of the Ae9Ap9Application class. 

 Classification:

   Unclassified

 Project Name:

   AE9/AP9/SPM Radiation Environment Models

   Developed under US Government contract # FA9453-12-C-0231

 Rights and Restrictions:

   Copyright 2014 Atmospheric and Environmental Research, Inc. (AER)

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
  1.0         06/22/2011  created 
  
SVNTag: $Id$
******************************************************************************/
#ifndef AE9AP9APPCINTERF_H
#define AE9AP9APPCINTERF_H


//-------------------------------------------------------------------
// defines and typedefs
//-------------------------------------------------------------------

// Under windows, if building the dll then SPWXDLL_EXPORTS will be
// defined and the functions exported. If building a client, it 
// won't and the functions will be imported. If building other 
// platforms the SPWXDLL_API define defaults to nothing. Also, on
// windows, explicitly set calling conventions to avoid ambiguity.

#ifdef _WIN32
#ifdef AE9AP9DLL_EXPORTS
#define AE9AP9DLL_API __declspec(dllexport)
#else
#define AE9AP9DLL_API __declspec(dllimport)
#endif
#define AE9AP9CALL_CONV __cdecl
#else
#define AE9AP9DLL_API
#define AE9AP9CALL_CONV
#endif

#ifdef __cplusplus
extern "C" {
#endif

  // 
  // Initialization of the flux environment settings:
  // Call to reset any or all environment data that requires
  // recalculation of flux weights (flux type, energies,
  // times, positions, directions). Note: the vvdEnergies
  // parameter only accepts a 2nd row of energy values
  // when passed in the context of a 2 pt differential flux request.
  //
  // Parameters:
  // szFluxType:     type of flux to compute (1 point differential, 2 point
  //                 differential, integral) (1PtDiff, 2PtDiff, Integral)
  // szCoordSys:     type of coordinate system in which position and direction values 
  //                 are passed ()
  // iNumEnergyLevels: number of energies for which calculations need to be carried out
  // iNumEnergyDims: number of energy dimensions (needed for DIFFERENIALE2 and INTEGRAL) 
  // pdEnergies:     1 or 2 dimensional array of doubles defining energy levels (MeV) at which
  //                 to compute flux. Note: row 2 used only for 2 pt differential flux type,
  //                 which requires computation of flux between two energy levels
  //                 storage order (dim1e1, dim1e2, dim1e3...[,dim2e1, dim2e2, dim2e3...])
  // iNumTimes:      size of the pdTimes array, i.e. the number of date/times for which calculations
  //                 need to be run (also defines number of coords in each axis array of coords)
  // pdTimes:        array of date/times in modified julian date format at which to compute flux
  //                 Lengths of time and position arrays must match, with each time corresponding
  //                 to a position at that same index in the arrays
  // pdCoordsAxis1,  Coordinates along axes in 3d space in the eCoordSys coordinate system of each
  // pdCoordsAxis2,  position at which flux is to be computed. These arrays should match the pdTimes
  // pdCoordsAxis3:  array in length and correspond to those times at each position.
  // iNumDirs:       Size of the directions dimension in the ppdFluxDir{n} 2D arrays below
  // pdFluxDir1,     Directions at which to compute flux at each timestep in pdTimes. 
  // pdFluxDir2,     Multiple directions can be computed at each timestep. Thus, these arrays
  // pdFluxDir3:     are 2 dimensional (nTimes,nDirections). Define in szCoordSys coordinates.
  //                 storage order (time1dir1, time1dir2, time1dir3...,time2dir1, time2dir2, time2dir3...)
  // iNumPitch:      Size of the pitch angles dimension in the ppdFluxDir{n} 1D or 2D array below
  // pdPitchAngles,  Pitch angles at which to compute flux at every[1D], or each[2D] timestep in pdTimes.
  // pdFluxDir3:     Multiple pitch angles can be computed at each timestep. 
  //                 When 2 dimensional (nTimes,nPitchAngles), use storage order:
  //                  (time1PA1, time1PA2, time1PA3...,time2PA1, time2PA2, time2PA3...)
  //
  // Return:
  // int             Error code, 0: success, else see AEErrors.h


  // Set environment for omni-directional flux

  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          AE9AP9APP_setFluxEnvironment_c( char*   szFluxType, 
                                          char*   szCoordSys,
                                          int     iNumEnergyLevels, 
                                          int     iNumEnergyDims,
                                          double* pdEnergies,
                                          int     iNumTimes,
                                          double* pdTimes, 
                                          double* pdCoordsAxis1,
                                          double* pdCoordsAxis2, 
                                          double* pdCoordsAxis3);

  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          AE9AP9APP_setFluxEnvironmentPitch_c( char*   szFluxType, 
                                               char*   szCoordSys,
                                               int     iNumEnergyLevels,
                                               int     iNumEnergyDims,
                                               double* pdEnergies,
                                               int     iNumTimes,
                                               double* pdTimes, 
                                               double* pdCoordsAxis1,
                                               double* pdCoordsAxis2, 
                                               double* pdCoordsAxis3,
                                               int     iNumPitch,
                                               double* pdPitchAngles);

  // Set environment for directional flux

  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          AE9AP9APP_setFluxEnvironmentDir_c( char*   szFluxType, 
                                             char*   szCoordSys,
                                             int     iNumEnergyLevels, 
                                             int     iNumEnergyDims,
                                             double* pdEnergies,
                                             int     iNumTimes,
                                             double* pdTimes,
                                             double* pdCoordsAxis1,
                                             double* pdCoordsAxis2, 
                                             double* pdCoordsAxis3,
                                             int     iNumDirs,
                                             double* pdFluxDir1,
                                             double* pdFluxDir2,
                                             double* pdFluxDir3 );

  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          AE9AP9APP_setFluxEnvironmentPitchT_c( char*   szFluxType, 
                                                char*   szCoordSys,
                                                int     iNumEnergyLevels,
                                                int     iNumEnergyDims,
                                                double* pdEnergies,
                                                int     iNumTimes,
                                                double* pdTimes, 
                                                double* pdCoordsAxis1,
                                                double* pdCoordsAxis2, 
                                                double* pdCoordsAxis3,
                                                int     iNumPitch,
                                                double* pdPitchAngles);

  // toggle mode for calculation of adiabatic coordinates
  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          AE9AP9APP_setAdiabaticCoords_c( int iMode );

  // access pitch angles associated with adiabatic coordinates
  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          AE9AP9APP_getPitchAngles_c(double* pdPitchAngles);

  // access calculated adiabatic coordinate values
  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          AE9AP9APP_getAdiabaticCoords_c(double* pdAlpha,
                                         double* pdLm,
                                         double* pdK, 
                                         double* pdPhi,
                                         double* pdHmin,
                                         double* pdLstar, 
                                         double* pdBmin,
                                         double* pdBlocal, 
                                         double* pdMagLT );

  //
  // "Fly-in" routines compute flux for a flux type and sets of times, 
  // positions, energies and directions passed in the most recent call 
  // to setFluxEnvironment. Fly-in routines are defined for computing
  // mean flux, perturbed mean flux, percentile flux and monte carlo scenarios of flux.
  // Each routine returns flux as a 3 dimensional array (time,energy,direction) of doubles.
  // Bad values are returned as AE_NaN, defined in AEErrors.h. The function 
  // bool ae9ap9::isnan(value) should be used to test for bad values. All fly-in routines
  // return an integer error code that should be equal to AE_ERR_NOERROR (0) on success. The
  // method getErrorText() can be used to query text associated with any non-zero error codes.
  //
  // Parameters:
  // iScenario:      Perturbed mean or monte carlo scenario number (1..999) Causes flux values
  //                 to be modified and returned based on statistical distributions of flux.
  // iPercentile:    Percentile (1..99) of the flux distribution at each time and position to return
  // dEpoch:         Start date/time in modified julian date for the monte carlo scenario
  // bPerturbFluxMap: Allows disabling flux map perturbations for validation situations where these
  //                 perturbations dwarf observed physical effects (default 1 (true), perturb on; else 0)
  //
  // Return:
  // pdFluxData:      3 dimensional array (time,energy,direction) of calculated flux data (MeV)
  //                  Sufficient memory should be allocated by caller, as follows:
  //                  (# times passed to setFluxEnvironment * # Energy levels * # directions [1 for omni])
  //                  storage order: row major
  //                       t1e1d1, t1e1d2, t1e1d3, t1e2d1, t1e2d2, t1e2d3, t2e1d1...
  // int             Error code, 0: success, else see AEErrors.h

  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          AE9AP9APP_flyInMean_c( double* pdFluxData);

  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          AE9AP9APP_flyInPerturbedMean_c( int iScenario, double* pdFluxData);

  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          AE9AP9APP_flyInPercentile_c( int iPercentile, double* pdFluxData );

  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          AE9AP9APP_flyInScenario_c( double    dEpoch, 
                                     int       iScenario, 
                                     double*   pdFluxData, 
                                     int       bPerturbFluxMap = 1 );

  //
  // Retrieve text associated with a returned error code
  //        

  AE9AP9DLL_API const char*  AE9AP9CALL_CONV 
          AE9AP9APP_getErrorText_c();

  //
  // Set this to the path and file name of the HDF5 file containing the data for the selected model
  // example: app.setDataSource( "../modelData/TEM2_runtime_tables.mat" ;
  // [ Defaults to current working directory and TEM2_runtime_tables.mat for eAEModelTEM, and
  // AP9VB_runtime_tables.mat for eAEModelTPM. ]
  //
  // Return:
  // int             Error code, 0: success, else see AEErrors.h

  AE9AP9DLL_API int AE9AP9CALL_CONV 
          AE9AP9APP_setModelDataSource_c( char* szDataSource, int iLength );

  //
  // Set this to the path and file of the neural network HDF5 file for the K-Phi
  // model. 
  //
  // Return:
  // int             Error code, 0: success, else see AEErrors.h

  AE9AP9DLL_API int AE9AP9CALL_CONV 
          AE9AP9APP_setKPhiNeuralNetDataSource_c( char* szDataSource, int iLength );
 
  //
  // Set this to the path and file of the neural network HDF5 file for the K-Hmin
  // model. 
  //
  // Return:
  // int             Error code, 0: success, else see AEErrors.h

  AE9AP9DLL_API int AE9AP9CALL_CONV 
          AE9AP9APP_setKHMinNeuralNetDataSource_c( char* szDataSource, int iLength );

  //
  // Set this to the path and file name of the HF5 file containing the magnetic field model data
  // example: app.setMagfieldModelDataSource( "../../modelData/igrfDB.h5" ;
  // [ Defaults to igrfDB.h5 at the same path as DataSource ]
  //
  // Return:
  // int             Error code, 0: success, else see AEErrors.h

  AE9AP9DLL_API int AE9AP9CALL_CONV 
          AE9AP9APP_setMagfieldModelDataSource_c( char* szDataSource, int iLength );

  //
  // Call this to ensure that underlying model objects get
  // properly deallocated. Failure to do so can result in a
  // number of HDF5 messages to the console on exit.
  //

  AE9AP9DLL_API void AE9AP9CALL_CONV
          AE9AP9APP_cleanup_c();

  //
  // Call to obtain the version number of the Ae9Ap9 library in use
  //

  AE9AP9DLL_API const char* AE9AP9CALL_CONV
          AE9AP9APP_getVersion_c();

#ifdef __cplusplus
}
#endif

#endif
