/******************************************************************************
$HeadURL$

File: AEFInterface.h

Description: This header file contains the declarations of the fortran interace 
methods to the Ae9Ap9Application class. These methods provide access from Fortran 
code to the methods of the Ae9Ap9Application class. 

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
#ifndef AE9AP9APPFINTERF_H
#define AE9AP9APPFINTERF_H

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

// for Linux usage, overloads of all functions, with a trailing underscore in 
//  their name, are also defined, so not to require specific FORTRAN compiler 
//  options upon the end-user programs.

  // 
  // Initialization of the flux environment settings:
  // Call to reset any or all environment data that requires
  // recalculation of flux weights (flux type, energies,
  // times, positions, directions). Note: the vvdEnergies
  // parameter only accepts a 2nd column of energy values
  // when passed in the context of a 2 pt differential flux request.
  //
  // Parameters:
  // pchFluxType:    type of flux to compute (1 point differential, 2 point
  //                 differential, integral) (DIFFERENTIALE1, DIFFERENTIALE2, INTEGRAL)
  // piFluxtypeLen:  pointer to number of characters in array szFluxType
  // pchCoordSys:    type of coordinate system in which position and direction values 
  //                 are passed ()
  // piCoordSysLen:  pointer to number of characters in szCoordSys
  // piNumEnergies:  pointer to number of energies for which calculations need to be carried out
  // piNumEnergyDims: pointer to number of energy dimensions (needed for DIFFERENIALE2 and INTEGRAL) 
  // pdEnergies:     2 dimensional array of doubles defining energy levels (MeV) at which
  //                 to compute flux. Note: column 2 used only for 2 pt differential flux type,
  //                 which requires computation of flux between two energy levels
  //                 storage order (dim1e1, dim1e2, dim1e3...[,dim2e1, dim2e2, dim2e3...])
  // piNumTimes:     pointer to size of the pdTimes array, i.e. the number of date/times for which calculations
  //                 have to be run
  // pdTimes:        array of date/times in modified julian date format at which to compute flux
  //                 Lengths of time and position arrays must match, with each time corresponding
  //                 to a position at that same index in the arrays
  // pdCoordsAxis1,  Coordinates along axes in 3d space in the eCoordSys coordinate system of each
  // pdCoordsAxis2,  position at which flux is to be computed. These arrays should match the pdTimes
  // pdCoordsAxis3:  array in length and correspond to those times at each position.
  // piNumDirs:      size of the directions dimension of the following 2 d direction arrays
  // pdFluxDir1,     Directions at which to compute flux at each timestep in pdTimes. To compute
  // pdFluxDir2,     omnidirectional flux, call cae9ap9app_setfluxenvironment.
  // pdFluxDir3:     Multiple directions can be computed at each timestep. Thus, these arrays
  //                 are 2 dimensional (nTimes,nDirections). Define in szCoordSys coordinates.
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
          ae9ap9app_setfluxenvironment_f( char*   pchFluxType, 
                                          int*    piFluxTypeLen,
                                          char*   pchCoordSys, 
                                          int*    piCoordSysLen,
                                          int*    piNumEnergies, 
                                          int*    piNumEnergyDims,
                                          double* pdEnergies, 
                                          int*    piNumTimes,
                                          double* pdTimes, 
                                          double* pdCoordsAxis1,
                                          double* pdCoordsAxis2, 
                                          double* pdCoordsAxis3 );

  // Set environment for flux fixed set of pitch angles

  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_setfluxenvironmentpitch_f( char*   pchFluxType, 
                                               int*    piFluxTypeLen,
                                               char*   pchCoordSys,
                                               int*    piCoordSysLen, 
                                               int*    piNumEnergies, 
                                               int*    piNumEnergyDims,
                                               double* pdEnergies,
                                               int*    piNumTimes,
                                               double* pdTimes, 
                                               double* pdCoordsAxis1,
                                               double* pdCoordsAxis2, 
                                               double* pdCoordsAxis3,
                                               int*    piNumPitch,
                                               double* pdPitchAngles );

  // Set environment for directional flux

  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_setfluxenvironmentdir_f( char*   pchFluxType, 
                                             int*    piFluxTypeLen,
                                             char*   pchCoordSys,
                                             int*    piCoordSysLen, 
                                             int*    piNumEnergies, 
                                             int*    piNumEnergyDims,
                                             double* pdEnergies,
                                             int*    piNumTimes,
                                             double* pdTimes, 
                                             double* pdCoordsAxis1,
                                             double* pdCoordsAxis2, 
                                             double* pdCoordsAxis3,
                                             int*    piNumFluxDirs,
                                             double* pdFluxDir1, 
                                             double* pdFluxDir2,
                                             double* pdFluxDir3 );

  // Set environment for flux at pitch angles for each ephemeris

  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_setfluxenvironmentpitcht_f( char*   pchFluxType, 
                                                int*    piFluxTypeLen,
                                                char*   pchCoordSys,
                                                int*    piCoordSysLen, 
                                                int*    piNumEnergies, 
                                                int*    piNumEnergyDims,
                                                double* pdEnergies,
                                                int*    piNumTimes,
                                                double* pdTimes, 
                                                double* pdCoordsAxis1,
                                                double* pdCoordsAxis2, 
                                                double* pdCoordsAxis3,
                                                int*    piNumPitch,
                                                double* pdPitchAngles );

  // toggle mode for calculation of adiabatic coordinates
  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_setadiabaticcoords_f( int* piMode );

  // access pitch angles associated with adiabatic coordinates
  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_getpitchangles_f(double* ppdPitchAngles);

  // access calculated adiabatic coordinate values
  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_getadiabaticcoords_f(double* ppdAlpha,
                                         double* ppdLm,
                                         double* ppdK, 
                                         double* ppdPhi,
                                         double* ppdHmin,
                                         double* ppdLstar, 
                                         double* pdBmin,
                                         double* pdBlocal, 
                                         double* pdMagLT );
                                                              
                                                              
  //
  // "Fly-in" routines compute flux for a flux type and sets of times, 
  // positions, energies and directions passed in the most recent call 
  // to setFluxEnvironment. Fly-in routines are defined for computing
  // mean flux, perturbed mean flux, percentile flux and monte carlo scenarios of flux.
  // Each routine returns flux as a 3 dimensional vector (time,energy,direction) of doubles.
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
  //                 perturbations dwarf observed physical effects (default NULL implies true, perturb on)
  //
  // Return:
  // pppdFluxData:    3 dimensional array (time,energy,direction) of calculated flux data (MeV)
  //                  Sufficient memory should be pre-allocated when calling these functions.
  //                  storage order as follows: (to support fortran column-major format)
  //
  //                     flux[time1 energy1 dir1]
  //                     flux[time2 energy1 dir1]
  //                     flux[time3 energy1 dir1]
  //                     flux[time1 energy2 dir1]
  //                     flux[time2 energy2 dir1]
  //                     flux[time3 energy2 dir1]
  //                     flux[time1 energy1 dir2]
  //                     flux[time2 energy1 dir2]
  //                     flux[time3 energy1 dir2]
  //                     flux[time1 energy2 dir2] ...
  // 
  // int             Error code, 0: success, else see AEErrors.h

  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_flyinmean_f( double* pppdFluxData);

  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_flyinperturbedmean_f( int* piScenario, double* pppdFluxData);

  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_flyinpercentile_f( int* piPercentile, double* pppdFluxData);

  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_flyinscenario_f( double* pdEpoch, 
                                     int*    piScenario, 
                                     double* pppdFluxData, 
                                     int*    pbPerturbFluxMap = NULL );

  //
  // Retrieve text associated with a returned error code
  //        
  // Parameters:
  //    piLength
  //           pointer to max length of message buffer
  //
  // Return:
  //    pchErrorText
  //           pointer to message return buffer
  //    piLength
  //           pointer to length of message returned
  //    int    
  //           Error code, 0: success, else see AEErrors.h

  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_geterrortext_f( char* pchErrorText, int* piLength);

  //
  // Set this to the path and file name of the HDF5 file containing the data for the selected model
  // example: app.setDataSource( "../modelData/TEM2_runtime_tables.mat" ;
  // [ Defaults to current working directory and TEM2_runtime_tables.mat for eAEModelTEM, and
  // AP9VB_runtime_tables.mat for eAEModelTPM. ]
  //
  // Parameters:
  //   pchDataSource   
  //          pointer to character string representing the path to the model data source 
  //   piLength
  //          pointer to length of the string representing the path to the model data source 
  //
  // Return:
  //    int    
  //           Error code, 0: success, else see AEErrors.h

  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_setmodeldatasource_f( char* pchDataSource, int* piLength );

  //
  // Set this to the path and file of the neural network HDF5 file for the K-Phi
  // model. 
  //
  // Parameters:
  //   pchDataSource   
  //          pointer to character string representing the path to the neural net data source 
  //   piLength
  //          pointer to length of the string representing the path to the neural netdata source 
  //
  // Return:
  //    int    
  //           Error code, 0: success, else see AEErrors.h

  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_setkphineuralnetdatasource_f( char* pchDataSource, int* piLength );

 
  //
  // Set this to the path and file of the neural network HDF5 file for the K-Hmin
  // model. 
  //
  // Parameters:
  //   pchDataSource   
  //          pointer to character string representing the path to the k phi neural net data source 
  //   piLength
  //          pointer to length of the string representing the path to the k phi neural netdata source 
  //
  // Return:
  //    int    
  //           Error code, 0: success, else see AEErrors.h

  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_setkhminneuralnetdatasource_f( char* pchDataSource, int* piLength );

  //
  // Set this to the path and file name of the HF5 file containing the magnetic field model data
  // example: app.setMagfieldModelDataSource( "../modelData/igrfDB.h5" ;
  // [ Defaults to igrfDB.h5 at the same path as DataSource ]
  //
  // Parameters:
  //   pchDataSource   
  //          pointer to character string representing the path to the magfield model data source 
  //   piLength
  //          pointer to length of the string representing the path to the magfield model data source 
  //
  // Return:
  //    int    
  //           Error code, 0: success, else see AEErrors.h

  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_setmagfieldmodeldatasource_f( char* pchDataSource, int* piLength );

  //
  // Call this to ensure that underlying model objects get
  // properly deallocated. Failure to do so can result in a
  // number of HDF5 messages to the console on exit.
  //

  AE9AP9DLL_API void AE9AP9CALL_CONV
          ae9ap9app_cleanup_f();

  //
  // Retrieve version of Ae9Ap9 model 
  //        
  // Parameters:
  //    piLength
  //           pointer to max length of message buffer
  //
  // Return:
  //    pchVersionText
  //           pointer to version text return buffer
  //    piLength
  //           pointer to length of message returned
  //    int    
  //           Error code, 0: success, else see AEErrors.h


  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_getversion_f( char* pchVersionText, int* piLength);

#ifndef _WIN32
  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_setfluxenvironment_f_( char*   pchFluxType, 
                                           int*    piFluxTypeLen,
                                           char*   pchCoordSys, 
                                           int*    piCoordSysLen,
                                           int*    piNumEnergies, 
                                           int*    piNumEnergyDims,
                                           double* pdEnergies, 
                                           int*    piNumTimes,
                                           double* pdTimes, 
                                           double* pdCoordsAxis1,
                                           double* pdCoordsAxis2, 
                                           double* pdCoordsAxis3 )
           { return ae9ap9app_setfluxenvironment_f( pchFluxType, piFluxTypeLen, 
                                  pchCoordSys, piCoordSysLen,
                                  piNumEnergies, piNumEnergyDims, pdEnergies, piNumTimes,
                                  pdTimes, pdCoordsAxis1, pdCoordsAxis2, pdCoordsAxis3 ); }
  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_setfluxenvironmentpitch_f_( char*   pchFluxType, 
                                               int*    piFluxTypeLen,
                                               char*   pchCoordSys,
                                               int*    piCoordSysLen, 
                                               int*    piNumEnergies, 
                                               int*    piNumEnergyDims,
                                               double* pdEnergies,
                                               int*    piNumTimes,
                                               double* pdTimes, 
                                               double* pdCoordsAxis1,
                                               double* pdCoordsAxis2, 
                                               double* pdCoordsAxis3,
                                               int*    piNumPitch,
                                               double* pdPitchAngles )
           { return ae9ap9app_setfluxenvironmentpitch_f( pchFluxType, piFluxTypeLen,
                                  pchCoordSys, piCoordSysLen, 
                                  piNumEnergies, piNumEnergyDims, pdEnergies, piNumTimes,
                                  pdTimes, pdCoordsAxis1, pdCoordsAxis2, pdCoordsAxis3,
                                  piNumPitch, pdPitchAngles ); }
  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_setfluxenvironmentdir_f_( char*   pchFluxType, 
                                             int*    piFluxTypeLen,
                                             char*   pchCoordSys,
                                             int*    piCoordSysLen, 
                                             int*    piNumEnergies, 
                                             int*    piNumEnergyDims,
                                             double* pdEnergies,
                                             int*    piNumTimes,
                                             double* pdTimes, 
                                             double* pdCoordsAxis1,
                                             double* pdCoordsAxis2, 
                                             double* pdCoordsAxis3,
                                             int*    piNumFluxDirs,
                                             double* pdFluxDir1, 
                                             double* pdFluxDir2,
                                             double* pdFluxDir3 )
           { return ae9ap9app_setfluxenvironmentdir_f( pchFluxType, piFluxTypeLen,
                                  pchCoordSys, piCoordSysLen, 
                                  piNumEnergies, piNumEnergyDims, pdEnergies, piNumTimes,
                                  pdTimes, pdCoordsAxis1, pdCoordsAxis2, pdCoordsAxis3,
                                  piNumFluxDirs, pdFluxDir1, pdFluxDir2, pdFluxDir3 ); }
  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_setfluxenvironmentpitcht_f_( char*   pchFluxType, 
                                                int*    piFluxTypeLen,
                                                char*   pchCoordSys,
                                                int*    piCoordSysLen, 
                                                int*    piNumEnergies, 
                                                int*    piNumEnergyDims,
                                                double* pdEnergies,
                                                int*    piNumTimes,
                                                double* pdTimes, 
                                                double* pdCoordsAxis1,
                                                double* pdCoordsAxis2, 
                                                double* pdCoordsAxis3,
                                                int*    piNumPitch,
                                                double* pdPitchAngles )
           { return ae9ap9app_setfluxenvironmentpitcht_f( pchFluxType, piFluxTypeLen,
                                  pchCoordSys, piCoordSysLen, 
                                  piNumEnergies, piNumEnergyDims, pdEnergies, piNumTimes,
                                  pdTimes, pdCoordsAxis1, pdCoordsAxis2, pdCoordsAxis3,
                                  piNumPitch, pdPitchAngles ); }
  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_setadiabaticcoords_f_( int* piMode )
           { return ae9ap9app_setadiabaticcoords_f( piMode ); }
  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_getpitchangles_f_(double* ppdPitchAngles)
           { return ae9ap9app_getpitchangles_f(ppdPitchAngles); }
  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_getadiabaticcoords_f_(double* ppdAlpha,
                                          double* ppdLm,
                                          double* ppdK, 
                                          double* ppdPhi,
                                          double* ppdHmin,
                                          double* ppdLstar, 
                                          double* pdBmin,
                                          double* pdBlocal, 
                                          double* pdMagLT )
           { return ae9ap9app_getadiabaticcoords_f( ppdAlpha, ppdLm, ppdK, ppdPhi,
                                     ppdHmin, ppdLstar, pdBmin, pdBlocal, pdMagLT ); }
  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_flyinmean_f_( double* pppdFluxData)
           { return ae9ap9app_flyinmean_f( pppdFluxData); }
  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_flyinperturbedmean_f_( int* piScenario, double* pppdFluxData)
           { return ae9ap9app_flyinperturbedmean_f( piScenario, pppdFluxData); }
  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_flyinpercentile_f_( int* piPercentile, double* pppdFluxData)
           { return ae9ap9app_flyinpercentile_f( piPercentile, pppdFluxData); }
  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_flyinscenario_f_( double* pdEpoch, 
                                      int*    piScenario, 
                                      double* pppdFluxData, 
                                      int*    pbPerturbFluxMap = NULL )
           { return ae9ap9app_flyinscenario_f( pdEpoch, piScenario, pppdFluxData, pbPerturbFluxMap ); }
  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_geterrortext_f_( char* pchErrorText, int* piLength)
           { return ae9ap9app_geterrortext_f( pchErrorText, piLength); }
  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_setmodeldatasource_f_( char* pchDataSource, int* piLength )
           { return ae9ap9app_setmodeldatasource_f( pchDataSource, piLength ); }
  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_setkphineuralnetdatasource_f_( char* pchDataSource, int* piLength )
           { return ae9ap9app_setkphineuralnetdatasource_f( pchDataSource, piLength ); }
  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_setkhminneuralnetdatasource_f_( char* pchDataSource, int* piLength )
           { return ae9ap9app_setkhminneuralnetdatasource_f( pchDataSource, piLength ); }
  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_setmagfieldmodeldatasource_f_( char* pchDataSource, int* piLength )
           { return ae9ap9app_setmagfieldmodeldatasource_f( pchDataSource, piLength ); }
  AE9AP9DLL_API void AE9AP9CALL_CONV
          ae9ap9app_cleanup_f_() { ae9ap9app_cleanup_f(); }
  AE9AP9DLL_API int  AE9AP9CALL_CONV 
          ae9ap9app_getversion_f_( char* pchVersionText, int* piLength)
           { return ae9ap9app_getversion_f( pchVersionText, piLength); }
#endif

#ifdef __cplusplus
}
#endif

#endif
