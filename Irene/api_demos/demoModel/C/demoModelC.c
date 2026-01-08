/***********************************************************************

 File: demoModelC.c  

 Description:

   Sample program to demonstrate usage of model-level API using C language.

 Classification :

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

 Build Instructions:
  Linux:
    in local directory
  % cmake -DIRENE_ROOT=<path_to_"~/Irene/linux"> .
      (requires 'cmake' version 3.14 or greater)
  % make
  
  % DemoModelC [path_to_"~/Irene/modelData"]

***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// include headers for model classes being used
#include "CDateTimeUtil_c.h"
#include "CEphemModel_c.h"
#include "Ae9Ap9Model_c.h"
#include "CAdiabatModel_c.h"
#include "CAccumModel_c.h"
#include "CAggregModel_c.h"
#include "CDoseModel_c.h"

// -- main program ---
int main (int argc, char* argv[])
{

  int ierr;
  int ii, jj, kk, nn;

  char *szDbDir = (char *)"../../modelData";
  char *szGDZ = (char *)"GDZ", *szKm = (char *)"km";

  //  check if command-line argument for model database path is present
  if ( argc > 1 ) {
    // print usage info if requested
    if ( strcmp(argv[1],"--help") == 0 || strcmp(argv[1],"-h")==0 ) {
      fprintf(stderr,"usage: DemoModelC [modelData path]\n");
      return 0;
    }
    // copy input string to be new model database directory
    szDbDir = argv[1];
    // (will be tested for validity further down)
  }
  int iStrLen = strlen(szDbDir);

  char *szFileName = (char *)malloc(iStrLen + 64);
  strcpy( szFileName, szDbDir );

  // initialize ephemeris model operations
  HANDLE zEphem = EphemStartUp();
  if (zEphem < 0) return zEphem;

  // define magnetic field database file (relative to executable location)
  //  (this also tests specified database path; anything not kosher is spit out)
  strcpy( szFileName+iStrLen, (char *)"/igrfDB.h5");
  ierr = EphemSetMagfieldDBFile(zEphem, szFileName);
  if (ierr < 0) {
    fprintf(stderr,"usage: demoModel [modelData path]\n");
    return ierr;
  }
// alternatively, could just specify the database directory 
//  (but this only verifies it as a directory, not files contained)
//  ierr = EphemSetModelDBDir(zEphem, szDbDir);
//  if (ierr < 0) {
//    fprintf(stderr,"usage: demoModel [modelData path]\n");
//    return ierr;
//  }

  // define time limits + step size for orbit propagation
  //   -note that these limits are _inclusive_, so this will produce *361* entries
  ierr = EphemSetTimes(zEphem, DateTimeGetModifiedJulianDate(2014,190,43200.0),
                  DateTimeGetModifiedJulianDate(2014,190,64800.0), 60.0);
  if (ierr < 0) return ierr;

  // define propagator to use  (string is NOT case-sensitive)
  ierr = EphemSetPropagator(zEphem, (char *)"SatEph");
  if (ierr < 0) return ierr;

  // define orbital characteristics

  // set element time
  // demonstrate full range of time conversion utility methods available
  ierr = EphemSetElementTime(zEphem, DateTimeGetModifiedJulianDate (2014,
                                       DateTimeGetDayOfYear(2014,7,9),
                                       DateTimeGetGmtSeconds(3,25,45.25)));
  if (ierr < 0) return ierr;

  // set orbit 'classical' element values
  ierr = EphemSetRightAscension(zEphem, 123.4);
  if (ierr < 0) return ierr;
  ierr = EphemSetArgOfPerigee(zEphem, 45.0);
  if (ierr < 0) return ierr;
  ierr = EphemSetInclination(zEphem, 35.0);
  if (ierr < 0) return ierr;
  ierr = EphemSetAltitudeOfApogee(zEphem, 2800.0);
  if (ierr < 0) return ierr;
  ierr = EphemSetAltitudeOfPerigee(zEphem, 1800.0);
  if (ierr < 0) return ierr;

  // allocate memory to receive the ephemeris information

  int iGuess = 1024; // ARBITRARY large number to hold returned data
  // -- the size needed could be determined from time duration / time step

    // when no 'chunk' size specified, need to determine number of
    // records will be generated based on time range and step size.
    // ==>if inadequate memory is allocated, program will be unstable

  double* pdaTimesMJD = (double *)malloc(iGuess*sizeof(double));
  double* pdaCoord1 = (double *)malloc(iGuess*sizeof(double));
  double* pdaCoord2 = (double *)malloc(iGuess*sizeof(double));
  double* pdaCoord3 = (double *)malloc(iGuess*sizeof(double));
  int iYear, iDdd;
  double dGmtsec;
  int iNumTimes;

  printf("\n ===================================\n");
  printf(" ephemeris demo1: 'sateph' propagator for GDZ ephemeris\n");
  //  based on all these orbit parameters defined, generate the ephemeris information,
  //   also specifying the coordinate system and units of the returned positions
  //   --since the 'chunk' size has NOT been specified, it defaults to zero, which means
  //     that a single call will return all entries, ie *full* time range of ephemeris
  iNumTimes = EphemComputeEphemeris(zEphem, szGDZ, szKm, pdaTimesMJD,
                                    pdaCoord1, pdaCoord2, pdaCoord3);



  if (iNumTimes>=0) {
    printf("sizes: pdaTimesMJD=%d\n", iNumTimes);
    if (iNumTimes>0) {
      ii=0;
      // demonstrate a sampling of results; first and last records only
      while (ii<iNumTimes) {
        DateTimeGetDateTime(pdaTimesMJD[ii], &iYear, &iDdd, &dGmtsec);
        printf("ephemeris (GDZ/km) [%d]: %.5f(%d,%d,%.1f) at %.3f,%.3f,%.3f\n",
               ii,pdaTimesMJD[ii],iYear,iDdd,dGmtsec,pdaCoord1[ii],pdaCoord2[ii],pdaCoord3[ii]);
        // skip to last entry
        ii += (iNumTimes-1);
      }
    }
  }
  else {
    fprintf(stderr, " ephemeris error: ierr=%d\n", iNumTimes);
    return iNumTimes;
  }

  // free allocated memory
  free(pdaTimesMJD);
  free(pdaCoord1);
  free(pdaCoord2);
  free(pdaCoord3);


  // ***********************************************************************
  // demonstrate alternate ephemeris generation, returning position and velocity in
  //   GEI coords, and using 'chunks' segmented processing

  //  set chunk size, for segmented output of ephemeris
  //    When not explicitly defined, the chunk size defaults to 0, which means all is
  //    returned at once, as in first section.
  //    --This limitation of the amount of data being processed is important for the
  //      Ae9Ap9 model processing performance. Due to the numerous internal memory
  //      allocations needed for storage of intermediary calculations for the model,
  //      the total memory requirements may exceed system resources available if a
  //      great number of ephemeris positions are called to be processed at once.
  EphemSetChunkSize(zEphem, 960);
  //  chunk size of 960 is the recommended level for 'typical' systems;
  //    otherwise, 120 for systems with limited memory;
  //    systems with more can go higher, but larger than 2400 may degrade performance
  // verify chunk size via 'get'
  int iChunk = EphemGetChunkSize(zEphem);

  printf("\n ===================================\n");
  printf(" ephemeris demo2: 'sateph' propagator for GEI ephemeris, ");
  printf("converted to GDZ, using chunks of size %d\n", iChunk);

  // allocate to receive the ephemeris information
  double* pdaXGEI = (double *)malloc(iChunk*sizeof(double));
  double* pdaYGEI = (double *)malloc(iChunk*sizeof(double));
  double* pdaZGEI = (double *)malloc(iChunk*sizeof(double));
  double* pdaXDotGEI = (double *)malloc(iChunk*sizeof(double));
  double* pdaYDotGEI = (double *)malloc(iChunk*sizeof(double));
  double* pdaZDotGEI = (double *)malloc(iChunk*sizeof(double));

  // reallocate memory for these using the specified chunk size
  pdaTimesMJD = (double *)malloc(iChunk*sizeof(double));
  pdaCoord1 = (double *)malloc(iChunk*sizeof(double));
  pdaCoord2 = (double *)malloc(iChunk*sizeof(double));
  pdaCoord3 = (double *)malloc(iChunk*sizeof(double));

  // compute intial set of pos and vel ephemeris in GEI coords
  iNumTimes = EphemComputeEphemerisGEI(zEphem, pdaTimesMJD,
                                       pdaXGEI, pdaYGEI, pdaZGEI,
                                       pdaXDotGEI, pdaYDotGEI, pdaZDotGEI);
  while (iNumTimes > 0) {
    printf("ephem2 sizes: pdaTimesMJD=%d\n",iNumTimes);
    //    demonstration of coordinate conversion call
    ierr = EphemConvertCoordinates(zEphem,
             (char *)"GEI", szKm, pdaTimesMJD, pdaXGEI, pdaYGEI, pdaZGEI, iNumTimes,
             szGDZ, szKm, pdaCoord1, pdaCoord2, pdaCoord3);
    if (ierr!=0) {
      fprintf(stderr, " error on coordinate conversion\n");
      return ierr;
    }
    int ii = 0;
    // demonstrate a sampling of results; first and last records only, in both coordinates
    while (ii<iNumTimes) {
      DateTimeGetDateTime(pdaTimesMJD[ii], &iYear, &iDdd, &dGmtsec);
      printf("ephemeris (GEI/km) [%d]: %.5f(%d,%d,%.1f) at %.3f,%.3f,%.3f; %.3f,%.3f,%.3f\n",
             ii,pdaTimesMJD[ii],iYear,iDdd,dGmtsec,
             pdaXGEI[ii],pdaYGEI[ii],pdaZGEI[ii],
             pdaXDotGEI[ii],pdaYDotGEI[ii],pdaZDotGEI[ii]);
      printf("ephemeris (GDZ/km) [%d]: %.5f(%d,%d,%.1f) at %.3f,%.3f,%.3f\n",
             ii,pdaTimesMJD[ii],iYear,iDdd,dGmtsec,
             pdaCoord1[ii],pdaCoord2[ii],pdaCoord3[ii]);
      // skip to last entry
      ii += (iNumTimes-1);
    }
    // get next chunk of ephemeris information
    iNumTimes = EphemComputeEphemerisGEI(zEphem, pdaTimesMJD,
                                         pdaXGEI, pdaYGEI, pdaZGEI,
                                         pdaXDotGEI, pdaYDotGEI, pdaZDotGEI);
  }  // end of ephemeris processing loop
  if (iNumTimes<0) fprintf (stderr, " ephemeris2 error: ierr=%d\n", iNumTimes);

  // free allocated memory
  free(pdaXGEI);
  free(pdaYGEI);
  free(pdaZGEI);
  free(pdaXDotGEI);
  free(pdaYDotGEI);
  free(pdaZDotGEI);


  // ***********************************************************************
  // demonstrate basic calls to Ae9Ap9 model, in tandem with ephemeris generation in chunks.

  printf("\n ===================================\n");
  printf(" ae9ap9 demo1: AP9 omnidirectional mean differential flux\n");

  // initialize Ae9Ap9 model class for 'AP9'
  HANDLE zAp9 = Ae9Ap9StartUp();

// new way
  ierr = Ae9Ap9SetModel(zAp9, (char *)"AP9");
  if (ierr < 0) return ierr;
  ierr = Ae9Ap9SetModelDBDir(zAp9, szDbDir);
  if (ierr < 0) return ierr;
//
// old way
  // specify database files for AP9 model, internal coords and magnetic field
//  strcpy( szFileName+iStrLen, (char *)"/AP9V15_runtime_tables.mat");
//  Ae9Ap9SetModelDBFile(zAp9, szFileName );
//  strcpy( szFileName+iStrLen, (char *)"/fastPhi_net.mat");
//  Ae9Ap9SetKPhiDBFile(zAp9, szFileName );
//  strcpy( szFileName+iStrLen, (char *)"/fast_hmin_net.mat");
//  Ae9Ap9SetKHMinDBFile(zAp9, szFileName );
//  strcpy( szFileName+iStrLen, (char *)"/igrfDB.h5");
//  Ae9Ap9SetMagfieldDBFile(zAp9, szFileName );
//

  printf("before model db load:\n");
  char szModel[15], szSpecies[15];
  Ae9Ap9GetModelName(zAp9, szModel);
  Ae9Ap9GetModelSpecies(zAp9, szSpecies);
  printf(" model name = %s, species = %s\n", szModel, szSpecies);

  // define list of proton energy levels
  double daProEnergies[] = { 0.1,0.4,0.8,1,2,5,10,15,20,30,50 };
  int iNumProEnergies = 11;

  int iNumDir = 1;  // omnidirectional

  double *pda3FluxDataMean;
  // allocate the necessary amount of *contiguous* memory to receive flux results
  //   based on the chunk size, number of energies and directions
  pda3FluxDataMean = (double *)malloc(iChunk*iNumProEnergies*iNumDir*sizeof(double));

  // restart the ephemeris generation using the previously-defined orbit
  //  (a restart would automatically be done when any of the orbit definitions is revised,
  //   the propagator or generation times changed, and/or a [new] chunk size specified)
  EphemRestartEphemeris(zEphem);

  // get first chunk of ephemeris
  iNumTimes = EphemComputeEphemeris(zEphem, szGDZ, szKm, pdaTimesMJD,
                                    pdaCoord1, pdaCoord2, pdaCoord3);

  // start of ephemeris generation and model processing loop
  while (iNumTimes>0) {

    // define AP9 flux environment using ephemeris info and proton energy list
    //  --for best model performance, the number of ephemeris positions input should be
    //    moderated, as explained in the comments for the 'ephem.setChunkSize()' call above
    ierr = Ae9Ap9SetFluxEnvironmentOmni(zAp9, (char *)"1PtDiff",
                                        daProEnergies, daProEnergies,
                                        pdaTimesMJD, szGDZ, szKm,
                                        pdaCoord1, pdaCoord2, pdaCoord3,
                                        iNumTimes, iNumProEnergies);
    if (ierr != 0) {
      fprintf(stderr, " ap9model setfluxEnvironment error: ierr=%d\n", ierr);
      return ierr;
    }
    Ae9Ap9GetModelName(zAp9, szModel);
    Ae9Ap9GetModelSpecies(zAp9, szSpecies);
    printf(" model name = %s, species = %s\n", szModel, szSpecies);

    // call to calculate AP9 "mean" flux values for the defined flux environment
    ierr = Ae9Ap9FlyinMean(zAp9, pda3FluxDataMean);

    if (ierr>=0) {
      ii=0;
      // demonstrate a sampling of results; first and last records only
      while (ii<iNumTimes) {  // time index
        DateTimeGetDateTime(pdaTimesMJD[ii], &iYear, &iDdd, &dGmtsec);
        // show time and position
        printf("time+pos [%d]: %.5f(%d,%d,%.1f) at %.3f,%.3f,%.3f\n",
               ii, pdaTimesMJD[ii], iYear, iDdd, dGmtsec,
               pdaCoord1[ii], pdaCoord2[ii], pdaCoord3[ii]);
        // show associated flux values
        for (kk=0; kk<iNumDir; ++kk) { // direction index
          printf(" ap9 mean flux [%d][*][%d] = ", ii, kk);
          // print fluxes for all energy levels of the current direction
          printf("%.3e", pda3FluxDataMean[ii*iNumProEnergies*iNumDir + kk]);
          for (jj=1; jj<iNumProEnergies; ++jj)  // energy index
            // since flux values are returned in a contiguous memory array,
            //   calculate index of flux for specific time entry, energy and direction
            printf(", %.3e", pda3FluxDataMean[ii*iNumProEnergies*iNumDir + jj*iNumDir + kk]);
          printf("\n");
        }
        // skip to last entry
        ii += iNumTimes-1;
      }
    }
    else {
      printf(" ap9model flyinmean error: ierr=%d\n",ierr);
    }
    // get next chunk of ephemeris
    iNumTimes = EphemComputeEphemeris(zEphem, szGDZ, szKm, pdaTimesMJD,
                                      pdaCoord1, pdaCoord2, pdaCoord3);
  }  // end of ephemeris generation and model processing loop


  // ***********************************************************************
  // demonstrate more advanced calls to Ae9Ap9 model, in tandem with ephemeris
  //   generation in chunks.

  printf("\n ===================================\n");
  printf(" ae9ap9 demo2: AE9 unidirectional percentile integral flux\n");

  // initialize a *separate* Ae9Ap9 model class for 'AE9'
  //  (_cannot_ load different model database into previously initialized object)
  HANDLE zAe9 = Ae9Ap9StartUp();

// new way
  ierr = Ae9Ap9SetModel(zAe9, (char *)"AE9");
  if (ierr < 0) return ierr;
  ierr = Ae9Ap9SetModelDBDir(zAe9, szDbDir);
  if (ierr < 0) return ierr;
//
// old way
  // specify database files for AE9 model, internal coords and magnetic field
//  strcpy( szFileName+iStrLen, (char *)"/AE9V15_runtime_tables.mat");
//  Ae9Ap9SetModelDBFile(zAe9, szFileName );
//  strcpy( szFileName+iStrLen, (char *)"/fastPhi_net.mat");
//  Ae9Ap9SetKPhiDBFile(zAe9, szFileName );
//  strcpy( szFileName+iStrLen, (char *)"/fast_hmin_net.mat");
//  Ae9Ap9SetKHMinDBFile(zAe9, szFileName );
//  strcpy( szFileName+iStrLen, (char *)"/igrfDB.h5");
//  Ae9Ap9SetMagfieldDBFile(zAe9, szFileName );
//

  // demonstration showing *manual* loading of defined model database
  //  --this is done automatically on first call to setFluxEnvironment
  ierr = Ae9Ap9LoadModelDB(zAe9);
  if (ierr != 0) {
    printf(" ae9model loadModelDB error: ierr=%i\n", ierr);
  }
  Ae9Ap9GetModelName(zAe9, szModel);
  Ae9Ap9GetModelSpecies(zAe9, szSpecies);
  printf(" model name = %s, species = %s\n", szModel, szSpecies);

  // define list of electron energy levels
  double daEleEnergies[] = { 0.04,0.1,0.2,0.4,0.7,1.0,1.5,2.0,3.5,5.0 };
  int iNumEleEnergies = 10;

  // define a list of pitch angles
  double daPitchAngles[] = { 90.0, 85.0, 80.0, 75.0, 70.0 };
  iNumDir = 5;

  // reset the orbit definitions used for the ephemeris generation
  //  (but this keeps the previous propagator choice and time limits)
  EphemResetOrbitParameters(zEphem);
  //  -- alternatively, a new ephemeris object could have been instantiated instead

  // set element time
  // --this demonstrates full range of time conversion utility methods available
  EphemSetElementTime(zEphem, DateTimeGetModifiedJulianDate(2014,
                                DateTimeGetDayOfYear(2014, 7, 9),
                                DateTimeGetGmtSeconds(12, 0, 0.0)));

  // set orbit 'mean' element values
  EphemSetRightAscension(zEphem, 65.45);
  EphemSetArgOfPerigee(zEphem, 129.4);
  EphemSetInclination(zEphem, 28.5);
  EphemSetEccentricity(zEphem, 0.255);
  EphemSetMeanMotion(zEphem, 6.53);
  EphemSetMeanAnomaly(zEphem, 305.65);

  // get first chunk of ephemeris
  iNumTimes = EphemComputeEphemeris(zEphem, szGDZ, szKm, pdaTimesMJD,
                                    pdaCoord1, pdaCoord2, pdaCoord3);

  // resize for new dimensions
  free(pda3FluxDataMean);
  pda3FluxDataMean = (double *)malloc(iChunk*iNumEleEnergies*iNumDir * sizeof(double));

  double *pda3FluxDataPAperc50 = (double *)malloc(iChunk*iNumEleEnergies*iNumDir * sizeof(double));
  double *pda3FluxDataPAperc75 = (double *)malloc(iChunk*iNumEleEnergies*iNumDir * sizeof(double));
  double *pda3FluxDataPAperc95 = (double *)malloc(iChunk*iNumEleEnergies*iNumDir * sizeof(double));

  // start of ephemeris generation and model processing loop
  while (iNumTimes>0) {

    // define AE9 flux environment using ephemeris info, electron energy list,
    //   and pitch angle list
    ierr = Ae9Ap9SetFluxEnvironmentFixPitch(zAe9, (char *)"Integral",
                                            daEleEnergies, daEleEnergies,
                                            pdaTimesMJD, szGDZ, szKm,
                                            pdaCoord1, pdaCoord2, pdaCoord3,
                                            daPitchAngles, iNumTimes,
                                            iNumEleEnergies, iNumDir);

    if (ierr != 0) {
      fprintf(stderr, " ae9model setfluxEnvironment error: ierr=%d\n",ierr);
      return ierr;
    }

    // call to calculate three sets of AE9 "percentile" flux values,
    //   for the defined flux environment
    ierr = Ae9Ap9FlyinPercentile(zAe9, 50, pda3FluxDataPAperc50);
    if (ierr != 0) fprintf(stderr, " ae9model flux percentile 50 error: ierr=%i\n", ierr);
    ierr = Ae9Ap9FlyinPercentile(zAe9, 75, pda3FluxDataPAperc75);
    if (ierr != 0) fprintf(stderr, " ae9model flux percentile 75 error: ierr=%i\n", ierr);
    ierr = Ae9Ap9FlyinPercentile(zAe9, 95, pda3FluxDataPAperc95);
    if (ierr != 0) fprintf(stderr, " ae9model flux percentile 95 error: ierr=%i\n", ierr);

    // call to calculate AE9 "mean" flux values for the defined flux environment
    ierr = Ae9Ap9FlyinMean(zAe9, pda3FluxDataMean);

    if (ierr>=0) {
      ii=0;
      // demonstrate a sampling of results; first and last records only
      while (ii<iNumTimes) {  // time index
        DateTimeGetDateTime(pdaTimesMJD[ii], &iYear, &iDdd, &dGmtsec);
        // show time and position
        printf("time+pos [%d]: %.5f(%d,%d,%.1f) at %.3f,%.3f,%.3f",
               ii, pdaTimesMJD[ii], iYear, iDdd, dGmtsec,
               pdaCoord1[ii], pdaCoord2[ii], pdaCoord3[ii]);
        // show pitch angles
        printf(", pitch angles = %.1f", daPitchAngles[0]);
        for (nn=1; nn<iNumDir; ++nn) printf(", %.1f", daPitchAngles[nn]);
        printf("\n");

        // show associated flux values
        for (kk=0; kk<iNumDir; ++kk) { // direction index
          printf(" perc95 flux [%d][*][%d] = ", ii, kk);
          // print fluxes for all energy levels of the current direction
          printf("%.3e", pda3FluxDataPAperc95[ii*iNumEleEnergies*iNumDir + kk]);
          for (jj=1; jj<iNumEleEnergies; ++jj) {  // energy index
            // since flux values are returned in a contiguous memory array,
            //   calculate index of flux for specific time entry, energy and direction
            printf(", %.3e", pda3FluxDataPAperc95[ii*iNumEleEnergies*iNumDir + jj*iNumDir + kk]);
          }
          printf("\n");
        }
        // skip to last entry
        ii += iNumTimes-1;
      }
    }
    else {
      printf(" ae9model flyinmean error: ierr=%d\n",ierr);
    }

    // get next chunk of ephemeris
    iNumTimes = EphemComputeEphemeris(zEphem, szGDZ, szKm, pdaTimesMJD,
                                      pdaCoord1, pdaCoord2, pdaCoord3);
  }  // end of ephemeris generation and model processing loop

  // free allocated memory
  free(pda3FluxDataPAperc50);
  free(pda3FluxDataPAperc75);
  free(pda3FluxDataPAperc95);


  // ***********************************************************************
  // demonstrate calls to Adiabatic model, in tandem with ephemeris generation in chunks.

  printf("\n ===================================\n");
  printf(" adiabatic demo: multiple pitch angles\n");

  // initialize Adiabatic model class
  HANDLE zAdia = AdiabatStartUp();

// new way
  ierr = AdiabatSetModelDBDir(zAdia, szDbDir);
  if (ierr < 0) return ierr;
//
// old way
  // define coords and mag field database files (relative to the executable location)
//  strcpy( szFileName+iStrLen, (char *)"/fastPhi_net.mat");
//  AdiabatSetKPhiDBFile(zAdia, szFileName );
//  strcpy( szFileName+iStrLen, (char *)"/fast_hmin_net.mat");
//  AdiabatSetKHMinDBFile(zAdia, szFileName );
//  strcpy( szFileName+iStrLen, (char *)"/igrfDB.h5");
//  AdiabatSetMagfieldDBFile(zAdia, szFileName );
//

  // restart the ephemeris generation of the previously defined orbit at the start time
  EphemRestartEphemeris(zEphem);

  // get first chunk of ephemeris
  iNumTimes = EphemComputeEphemeris(zEphem, (char *)"RLL", (char *)"Re", pdaTimesMJD,
                                    pdaCoord1, pdaCoord2, pdaCoord3);

  // initialize double arrays (chunk size x directions)
  double *pda2Alpha = (double *)malloc(iChunk*iNumDir * sizeof(double));
  double *pda2Lm = (double *)malloc(iChunk*iNumDir * sizeof(double));
  double *pda2K = (double *)malloc(iChunk*iNumDir * sizeof(double));
  double *pda2Phi = (double *)malloc(iChunk*iNumDir * sizeof(double));
  double *pda2Hmin = (double *)malloc(iChunk*iNumDir * sizeof(double));
  double *pda2Lstar = (double *)malloc(iChunk*iNumDir * sizeof(double));
  double *pda2I = (double *)malloc(iChunk*iNumDir * sizeof(double));
  double *pda2B = (double *)malloc(iChunk*3 * sizeof(double)); // three components of B vector

  // initialize single arrays (chunk size)
  double *pdaBmin = (double *)malloc(iChunk * sizeof(double));
  double *pdaBlocal = (double *)malloc(iChunk * sizeof(double));
  double *pdaMagLT = (double *)malloc(iChunk * sizeof(double));

  // start of ephemeris generation and model processing loop
  while (iNumTimes>0) {

    // call to calculate adiabatic coordinates based on input ephemeris
    //   and pitch angle information
    ierr = AdiabatComputeCoordinateSet(zAdia, (char *)"RLL", (char *)"Re",
                                       pdaTimesMJD, pdaCoord1, pdaCoord2,
                                       pdaCoord3, iNumTimes,
                                       daPitchAngles, iNumDir,
                                       pda2Alpha, pda2Lm, pda2K,
                                       pda2Phi, pda2Hmin, pda2Lstar,
                                       pdaBmin, pdaBlocal, pdaMagLT,
                                       pda2B, pda2I);
    if (ierr != 0) {
      fprintf(stderr, " adiabatic coordinates error: ierr=%d\n", ierr);
      return ierr;
    }

    if (ierr>=0) {
      ii=0;
      // demonstrate a sampling of results; first and last records only
      while (ii<iNumTimes) {  // time index
        DateTimeGetDateTime(pdaTimesMJD[ii], &iYear, &iDdd, &dGmtsec);
        // show time and position
        printf("time+pos(RLL/Re) [%d]: %.5f(%d,%d,%.1f) at %.3f,%.3f,%.3f",
               ii, pdaTimesMJD[ii], iYear, iDdd, dGmtsec,
               pdaCoord1[ii], pdaCoord2[ii], pdaCoord3[ii]);
        // show pitch angles
        printf(", pitch angles = %.1f", daPitchAngles[0]);
        for (nn=1; nn<iNumDir; ++nn) printf(", %.1f", daPitchAngles[nn]);
        printf("\n");

        // show associated adibatic coordinate values for each of the pitch angles
        for (jj=0; jj<iNumDir; ++jj) { // direction index
          printf(" adiabat values [%d] = %.2f %.3f %.5f %.5f %.1f %.3f %.3e %.3e %.2f %.3e %.3e %.3e %.5f\n",
                 jj, pda2Alpha[ii*iNumDir + jj], pda2Lm[ii*iNumDir + jj],
                 pda2K[ii*iNumDir + jj],  pda2Phi[ii*iNumDir + jj],
                 pda2Hmin[ii*iNumDir + jj], pda2Lstar[ii*iNumDir + jj],
                 pdaBmin[ii], pdaBlocal[ii], pdaMagLT[ii],
                 pda2B[ii*3], pda2B[ii*3 + 1], pda2B[ii*3 + 2], pda2I[ii*iNumDir + jj]);
        }

        // skip to last entry
        ii += iNumTimes-1;
      }
    }
    else {
      printf(" adiabatic error: ierr=%d\n",ierr);
    }
    // get next chunk of ephemeris
    iNumTimes = EphemComputeEphemeris(zEphem, (char *)"RLL", (char *)"Re", pdaTimesMJD,
                                      pdaCoord1, pdaCoord2, pdaCoord3);
  }  // end of ephemeris generation and model processing loop

  // free allocated memory
  free(pda2Alpha);
  free(pda2Lm);
  free(pda2K);
  free(pda2Phi);
  free(pda2Hmin);
  free(pda2Lstar);
  free(pdaBmin);
  free(pdaBlocal);
  free(pdaMagLT);

  ierr = AdiabatShutDown(zAdia);
  if (ierr < 0) return ierr;


  // ***********************************************************************
  // demonstrate additional advanced calls to Ae9Ap9 model, and aggregation of results,
  //   in tandem with ephemeris generation in chunks.

  printf("\n ===================================\n");
  printf(" ae9ap9 demo3: AP9 omnidirectional aggregated perturbed mean differential flux\n");

  // (reuse the previously-instantiated AP9 class object)

  iNumDir = 1;  // omnidirectional

  // initialize Aggregation model class
  HANDLE zAgg = AggregStartUp();

  // define list of confidence levels to be calculated from aggregation
  int iaConfLevels[] = { 85, 95 };
  int iNumConf = 2;

  // restart the ephemeris generation of the previously-defined orbit at the start time
  EphemRestartEphemeris(zEphem);

  // get first chunk of ephemeris
  iNumTimes = EphemComputeEphemeris(zEphem, (char *)"Geo", szKm, pdaTimesMJD,
                                    pdaCoord1, pdaCoord2, pdaCoord3);

  double *pda3FluxDataPertMean = (double *)malloc(iChunk*iNumProEnergies*iNumDir * sizeof(double));
  double *pda3FluxDataPMAgg = (double *)malloc(iChunk*iNumProEnergies*iNumDir * sizeof(double));
  double *pdaTimeAgg = (double *)malloc(iChunk * sizeof(double));

  // start of ephemeris generation and model processing loop
  while (iNumTimes>0) {

    // define AP9 flux environment using ephemeris info and proton energy list
    ierr = Ae9Ap9SetFluxEnvironmentOmni(zAp9, (char *)"1PtDiff",
                                        daProEnergies, daProEnergies,
                                        pdaTimesMJD, (char *)"Geo", szKm,
                                        pdaCoord1, pdaCoord2, pdaCoord3,
                                        iNumTimes, iNumProEnergies);
    if (ierr != 0) {
      fprintf(stderr, " ap9model setfluxEnvironment error: ierr=%d\n", ierr);
      return ierr;
    }

    // initialize aggregation for the current time segment (chunk)
    AggregResetAgg(zAgg);

    // call to calculate 25 scenarios of AP9 "perturbed mean" flux values
    //   for the defined flux environment
    for (jj=1; jj<=25; ++jj) {
      ierr = Ae9Ap9FlyinPerturbedMean(zAp9, jj, pda3FluxDataPertMean);
      if (ierr != 0) {
        fprintf(stderr, " ae9ap9 flux perturbed mean %d error: ierr=%d\n", jj, ierr);
        return ierr;
      }

      // save the model results and associated time values in the aggregation object
      ierr = AggregAddScenToAgg(zAgg, pdaTimesMJD, pda3FluxDataPertMean,
                                iNumTimes, iNumProEnergies, iNumDir);
      if (ierr != 0) {
        fprintf(stderr, " aggreg of perturbed mean %d error: ierr=%d\n", jj, ierr);
        return ierr;
      }
    }

    // calculate specified confidence level values of aggregation, and display
    for (nn=0; nn<iNumConf; ++nn) {

      ierr = AggregComputeConfLevel(zAgg, iaConfLevels[nn],
                                    pdaTimeAgg, pda3FluxDataPMAgg);
      if (ierr != 0) {
        fprintf(stderr, " aggreg computeConfLevel error: ierr=%d\n", ierr);
        return ierr;
      }
      printf("%d%% Confidence level results from aggregation of 25 scenarios "
             "of perturbed mean fluxes\n", iaConfLevels[nn]);

      ii=0;
      // demonstrate a sampling of results; first and last records only
      while (ii<iNumTimes) {  // time index
        DateTimeGetDateTime(pdaTimeAgg[ii], &iYear, &iDdd, &dGmtsec);
        // show time and position
        printf("time+pos(GEO/km) [%d]: %.5f(%d,%d,%.1f) at %.3f,%.3f,%.3f\n",
               ii, pdaTimeAgg[ii], iYear, iDdd, dGmtsec,
               pdaCoord1[ii], pdaCoord2[ii], pdaCoord3[ii]);
        // show the perturbed mean flux aggregation confidence level results
        //   (for single pitch angle=omnidirectional)
        for (kk=0; kk<iNumDir; ++kk) { // direction index
          printf(" ap9 %d%% CL PM flux [%d][*][%d] = ", iaConfLevels[nn], ii, kk);
          // print fluxes for all energy levels of the current direction
          printf("%.3e", pda3FluxDataPMAgg[ii*iNumProEnergies*iNumDir + kk]);
          for (jj=1; jj<iNumProEnergies; ++jj)  // energy index
            // since flux values are returned in a contiguous memory array,
            //   calculate index of flux for specific time entry, energy and direction
            printf(", %.3e", pda3FluxDataPMAgg[ii*iNumProEnergies*iNumDir + jj*iNumDir + kk]);
          printf("\n");
        }
        // skip to last entry
        ii += iNumTimes-1;
      }
    }
    // get next chunk of ephemeris
    iNumTimes = EphemComputeEphemeris(zEphem, (char *)"Geo", szKm, pdaTimesMJD,
                                      pdaCoord1, pdaCoord2, pdaCoord3);
  }  // end of ephemeris generation and model processing loop

  // free allocated memory
  free(pda3FluxDataPertMean);
  free(pda3FluxDataPMAgg);
  free(pdaTimeAgg);

  ierr = Ae9Ap9ShutDown(zAp9);
  if (ierr < 0) return ierr;


  // ***********************************************************************
  // demonstrate  calls to Ae9Ap9 model, accumulation of the results, and calculation
  //   of dose results, in tandem with ephemeris generation in chunks.
  printf("\n ===================================\n");
  printf(" ae9ap9 demo4: AE9 omnidirectional mean differential flux and fluences\n");

  // (reuse the previously-instantiated AE9 class object, now omnidirectional)
  iNumDir = 1;

  // initialize multiple Accumulation model class objects
  //   (separate AccumModel objects are required for each data type
  //    and/or accumulation interval)

  // set up list of accumulation interval times
  //  - 930 is equal to 15.5 of the ephemeris timesteps
  //    (chosen to be NOT an even multiple)
  //  - 1800 is equal to 30 ephemeris timesteps
  int iaInterval[] = { 930, 1800 };
  int iNumIntv = 2;

  HANDLE *zaAccum = (HANDLE *)malloc(iNumIntv * sizeof(HANDLE));
  for (nn=0; nn<iNumIntv; ++nn) {
    zaAccum[nn] = AccumStartUp();
    ierr = AccumSetTimeIntervalSec(zaAccum[nn], iaInterval[nn]);
    if (ierr != 0) {
      fprintf(stderr, " accum setTimeIntervalSec error: ierr=%d\n",ierr);
      return ierr;
    }
  }

  double *pda3Fluence = (double *)malloc(iChunk*iNumEleEnergies*iNumDir * sizeof(double));
  double *pda3IntvFluence = (double *)malloc(iChunk*iNumEleEnergies*iNumDir * sizeof(double));
  // declare 3d array to hold accumulated fluence from intervals
  //  (will be used in next section for dose calculation)
  double *pda3AccumIntvFlue = (double *)malloc(iChunk*iNumEleEnergies*iNumDir * sizeof(double));
  double *pdaFlueTime = (double *)malloc(iChunk*sizeof(double));
  double *pdaIntvFlueTime = (double *)malloc(iChunk*sizeof(double));
  int *piaIntvIndices = (int *)malloc(iChunk*sizeof(int));

  // double variables to retain the last ephemeris coordinates
  double dCoord1Prev, dCoord2Prev, dCoord3Prev;

  // restart the ephemeris generation of the previously-defined orbit
  EphemRestartEphemeris(zEphem);

  // get first chunk of ephemeris
  iNumTimes = EphemComputeEphemeris(zEphem, szGDZ, szKm, pdaTimesMJD,
                                    pdaCoord1, pdaCoord2, pdaCoord3);

  // resize for new dimensions (omnidirectional)
  free(pda3FluxDataMean);
  pda3FluxDataMean = (double *)malloc(iChunk*iNumEleEnergies*iNumDir * sizeof(double));

  // start of ephemeris generation and model processing loop
  while (iNumTimes>0) {

    // define AE9 flux environment using ephemeris info and electron energy list
    // --Important: Dose calculations *require* 1pt differential _omnidirectional_ flux values
    ierr = Ae9Ap9SetFluxEnvironmentOmni(zAe9, (char *)"1PtDiff",
                                        daEleEnergies, daEleEnergies,
                                        pdaTimesMJD, szGDZ, szKm,
                                        pdaCoord1, pdaCoord2, pdaCoord3,
                                        iNumTimes, iNumEleEnergies);

    if (ierr != 0) {
      fprintf(stderr, " ae9model setfluxEnvironment error: ierr=%d\n",ierr);
      return ierr;
    }

    // call to calculate AE9 "mean" flux values for the defined flux environment
    ierr = Ae9Ap9FlyinMean(zAe9, pda3FluxDataMean);

    if (ierr>=0) {
      ii=0;
      // demonstrate a sampling of results; first and last records only
      while (ii<iNumTimes) {  // time index
        DateTimeGetDateTime(pdaTimesMJD[ii], &iYear, &iDdd, &dGmtsec);
        // show time and position
        printf("time+pos [%d]: %.5f(%d,%d,%.1f) at %.3f,%.3f,%.3f\n",
               ii, pdaTimesMJD[ii], iYear, iDdd, dGmtsec,
               pdaCoord1[ii], pdaCoord2[ii], pdaCoord3[ii]);

        // show associated flux values
        for (kk=0; kk<iNumDir; ++kk) { // direction index
          printf(" ae9 mean flux [%d][*][%d] = ", ii, kk);
          // print fluxes for all energy levels of the current direction
          printf("%.3e", pda3FluxDataMean[ii*iNumEleEnergies*iNumDir + kk]);
          for (jj=1; jj<iNumEleEnergies; ++jj) {  // energy index
            // since flux values are returned in a contiguous memory array,
            //   calculate index of flux for specific time entry, energy and direction
            printf(", %.3e", pda3FluxDataMean[ii*iNumEleEnergies*iNumDir + jj*iNumDir + kk]);
          }
          printf("\n");
        }
        // skip to last entry
        ii += iNumTimes-1;
      }
    }
    else {
      printf(" ae9model flyinmean error: ierr=%d\n",ierr);
    }

    // loop over accumulation objects
    for (nn=0; nn<iNumIntv; ++nn) {
      HANDLE zAcc = zaAccum[nn];

      // load this set of 'mean' flux values into accumulation object
      ierr = AccumLoadBuffer(zAcc, pdaTimesMJD, pda3FluxDataMean,
                             iNumTimes, iNumEleEnergies, iNumDir);
      if (ierr != 0) {
        fprintf(stderr, " accum loadBuffer error: ierr=%d\n", ierr);
        return ierr;
      }

      // calculate cumulative fluence values using contents of first accumulation buffer
      //   (this would produce identical results for the second buffer)
      int iNum = AccumComputeFluence(zAcc, pdaFlueTime, pda3Fluence);
      // -these are cumulative values reported at each ephemeris timestep
      //  demonstrate these cumulative fluence results
      if (iNum <=0 ) fprintf(stderr, " accum computeFluence error: ierr=%d\n", ierr);
      // show cumulative fluence values
      else {
        printf(" cumulative fluence ae9 returned: iNum=%d\n", iNum);

        ii = 0;
        // demonstrate a sampling of results; first, middle and last records only
        while (ii<iNum) {  // time index
          DateTimeGetDateTime(pdaFlueTime[ii], &iYear, &iDdd, &dGmtsec);
          // show time and position
          printf("time+pos [%d]: %.5f(%d,%d,%.1f) at %.3f,%.3f,%.3f\n",
                 ii, pdaFlueTime[ii], iYear, iDdd, dGmtsec,
                 pdaCoord1[ii], pdaCoord2[ii], pdaCoord3[ii]);

          for (kk=0; kk<iNumDir; ++kk) { // direction index
            printf(" ae9 cumul fluence [%d][*][%d] = ", ii, kk);
            // show associated fluence values
            printf("%.3e", pda3Fluence[ii*iNumEleEnergies*iNumDir + kk]);
            for (jj=1; jj<iNumEleEnergies; ++jj) {  // energy index
              // since fluence values are returned in a contiguous memory array,
              //   calculate index of fluence for specific time entry, energy and direction
              printf(", %.3e", pda3Fluence[ii*iNumEleEnergies*iNumDir + jj*iNumDir + kk]);
            }
            printf("\n");
          }
          ii += (iNum/2)-1;
          if (ii<iNum && ii>iNum-5) ii=iNum-1;
        }
      }

      // calculate (completed) interval fluence values using contents
      //   of each accumulation buffer
      iNum = AccumComputeIntvFluence(zAcc, pdaIntvFlueTime, pda3IntvFluence,
                                     piaIntvIndices, 0);
      // -these are interval fluence values reported at the specified interval durations
      // -note that data of _partially_ complete intervals are retained, to be included
      //  within an interval that will be completed during the next 'chunk' processing
      // demonstrate these interval fluence results
      if (iNum <=0 ) fprintf(stderr, " accum computeIntvFluence error: ierr=%d\n", ierr);
      // show fluence values from each interval
      else {
        printf(" --(%ds) interval fluence ae9 returned: iNum=%d\n", iaInterval[nn], iNum);

        ii = 0;
        // demonstrate *all* of these results
        while (ii<iNum) {  // time index
          DateTimeGetDateTime(pdaIntvFlueTime[ii], &iYear, &iDdd, &dGmtsec);
          int index = piaIntvIndices[ii];
          // show time and position
          printf("time+pos [%d]: %.5f(%d,%d,%.1f) interval end at approx %.3f,%.3f,%.3f [%d]\n",
                 ii, pdaIntvFlueTime[ii], iYear, iDdd, dGmtsec,
                 pdaCoord1[index], pdaCoord2[index], pdaCoord3[index], index);

          for (kk=0; kk<iNumDir; ++kk) { // direction index
            printf(" ae9 intv fluence [%d][*][%d] = ", ii, kk);
            // show associated fluence values
            printf("%.3e", pda3IntvFluence[ii*iNumEleEnergies*iNumDir + kk]);
            for (jj=1; jj<iNumEleEnergies; ++jj) {  // energy index
              // since fluence values are returned in a contiguous memory array,
              //   calculate index of fluence for specific time entry, energy and direction
              printf(", %.3e", pda3IntvFluence[ii*iNumEleEnergies*iNumDir + jj*iNumDir + kk]);
            }
            printf("\n");
          }
          ii += 1;
        }
      }

      // call to accumulate these interval-specific fluence values
      iNum = AccumAccumIntvFluence(zAcc, pdaIntvFlueTime, pda3IntvFluence,
                                   iNum, iNumEleEnergies, iNumDir,
                                   pda3AccumIntvFlue, 0);
      // demonstrate these interval fluence results
      if (iNum <=0 ) {
        fprintf(stderr, " accum accumIntvFluence error: ierr=%d\n", iNum);
      }
      // show cumulative summation of fluence values over the intervals
      else {
        printf(" --(%ds) accum interval fluence ae9 returned: iNum=%d\n", iaInterval[nn], iNum);

        ii = 0;
        // demonstrate *all* of these results
        while (ii<iNum) {  // time index
          DateTimeGetDateTime(pdaIntvFlueTime[ii], &iYear, &iDdd, &dGmtsec);
          int index = piaIntvIndices[ii];
          // show time and position
          printf("time+pos [%d]: %.5f(%d,%d,%.1f) interval end at approx %.3f,%.3f,%.3f [%d]\n",
                 ii, pdaIntvFlueTime[ii], iYear, iDdd, dGmtsec,
                 pdaCoord1[index], pdaCoord2[index], pdaCoord3[index], index);

          for (kk=0; kk<iNumDir; ++kk) { // direction index
            printf(" ae9 intv fluence [%d][*][%d] = ", ii, kk);
            // show associated fluence values
            printf("%.3e", pda3AccumIntvFlue[ii*iNumEleEnergies*iNumDir + kk]);
            for (jj=1; jj<iNumEleEnergies; ++jj) {  // energy index
              // since fluence values are returned in a contiguous memory array,
              //   calculate index of fluence for specific time entry, energy and direction
              printf(", %.3e", pda3AccumIntvFlue[ii*iNumEleEnergies*iNumDir + jj*iNumDir + kk]);
            }
            printf("\n");
          }
          ii += 1;
        }
      }
    }  // end loop over accumulation objects (nn)

    // retain last ephemeris coordinates for later reference
    dCoord1Prev = pdaCoord1[iNumTimes-1];
    dCoord2Prev = pdaCoord2[iNumTimes-1];
    dCoord3Prev = pdaCoord3[iNumTimes-1];

    // get next chunk of ephemeris
    iNumTimes = EphemComputeEphemeris(zEphem, szGDZ, szKm, pdaTimesMJD,
                                      pdaCoord1, pdaCoord2, pdaCoord3);
  }  // end of ephemeris generation and model processing loop

  // collect fluence data from last *partial* accumulation intervals, if any
  //  --note last argument added to method call, with flag to get this data
  // loop over accumulation objects
  for (nn=0; nn<iNumIntv; ++nn) {
    HANDLE zAcc = zaAccum[nn];
    // calculate (completed) interval fluence values using contents
    //   of each accumulation buffer
    int iNum = AccumComputeIntvFluence(zAcc, pdaIntvFlueTime, pda3IntvFluence,
                                       piaIntvIndices, 1);
    // -these are interval fluence values reported at the specified interval durations
    // -note that data of _partially_ complete intervals are retained, to be included
    //  within an interval that will be completed during the next 'chunk' processing
    // demonstrate these interval fluence results
    if (iNum <0 ) fprintf(stderr, " accum computeIntvFluence error: ierr=%d\n", ierr);
    // -returned size will either be 0 (no leftover data) or 1 (partial interval data)

    // show fluence values from last (partial) interval, if present
    else {
      // show final cumulative summation of fluence values over the intervals
      printf(" --(%ds) partial interval fluence ae9 returned: iNum=%d\n", iaInterval[nn], iNum);

      if (iNum > 0) {
        ii = 0;
        DateTimeGetDateTime(pdaIntvFlueTime[ii], &iYear, &iDdd, &dGmtsec);
        // demonstrate these last results
        // show time and position
        printf("time+pos [%d]: %.5f(%d,%d,%.1f) interval end at approx %.3f,%.3f,%.3f\n",
               ii, pdaIntvFlueTime[ii], iYear, iDdd, dGmtsec,
               dCoord1Prev, dCoord2Prev, dCoord3Prev);

        for (kk=0; kk<iNumDir; ++kk) { // direction index
          printf(" ae9 partial intv fluence [%d][*][%d] = ", ii, kk);
          // show associated fluence values
          printf("%.3e", pda3IntvFluence[kk]);
          for (jj=1; jj<iNumEleEnergies; ++jj) {  // energy index
            // since fluence values are returned in a contiguous memory array,
            //   calculate index of fluence for specific time entry, energy and direction
            printf(", %.3e", pda3IntvFluence[jj*iNumDir + kk]);
          }
          printf("\n");
        }

        // call to accumulate these interval-specific fluence values
        iNumTimes = AccumAccumIntvFluence(zAcc, pdaIntvFlueTime, pda3IntvFluence,
                                          iNum, iNumEleEnergies, iNumDir,
                                          pda3AccumIntvFlue, 0);
        // demonstrate these interval fluence results
        if (iNum <= 0 ) {
          fprintf(stderr, " accum accumIntvFluence error: ierr=%d\n", iNum);
        }
        // show cumulative summation of fluence values over the intervals
        else {
          printf(" --(%ds) accum interval fluence ae9 returned: iNum=%d\n", iaInterval[nn], iNum);

          // show time and position
          printf("time+pos [%d]: %.5f(%d,%d,%.1f) interval end at approx %.3f,%.3f,%.3f\n",
                 ii, pdaIntvFlueTime[ii], iYear, iDdd, dGmtsec,
                 dCoord1Prev, dCoord2Prev, dCoord3Prev);

          for (kk=0; kk<iNumDir; ++kk) { // direction index
            printf(" ae9 intv fluence [%d][*][%d] = ", ii, kk);
            // show associated fluence values
            printf("%.3e", pda3AccumIntvFlue[kk]);
            for (jj=1; jj<iNumEleEnergies; ++jj) {  // energy index
              // since fluence values are returned in a contiguous memory array,
              //   calculate index of fluence for specific time entry, energy and direction
              printf(", %.3e", pda3AccumIntvFlue[jj*iNumDir + kk]);
            }
            printf("\n");
          }
        }
      }
    }
  }

  // free allocated memory
  free(pda3Fluence);
  free(pda3IntvFluence);
  free(pdaFlueTime);
  free(pdaIntvFlueTime);
  free(piaIntvIndices);

  ierr = AggregShutDown(zAgg);
  if (ierr < 0) return ierr;


  // ***********************************************************************
  printf("\n =================================== \n");
  printf(" dose demo: accumulated dose from AE9 omnidirectional mean differential fluences\n");
  // initialize Dose model class
  HANDLE zDose = DoseStartUp();

// new way
  ierr = DoseSetModelDBDir(zDose, szDbDir);
  if (ierr < 0) return ierr;
//
// old way
  // define shieldDose2 model database file
//  strcpy( szFileName+iStrLen, (char *)"/sd2DB.h5");
//  DoseSetModelDBFile(zDose, szFileName );
//

  // define flux particle species and energies
  char szEleSpecies[15];
  Ae9Ap9GetModelSpecies(zAe9, szEleSpecies);
  DoseSetSpecies(zDose, szEleSpecies);
  DoseSetEnergies(zDose, daEleEnergies, iNumEleEnergies, (char *)"MeV");

  // define shielding depth values/units/geometry and detector type
  double daDepths[] = { 0.1, 0.2, 0.4, 0.5, 0.75, 1.0, 1.5, 2.0 };
  int iNumDepths = 8;
  DoseSetDepths(zDose, daDepths, iNumDepths, (char *)"mm");
  DoseSetDetector(zDose, (char *)"Si");
  DoseSetGeometry(zDose, (char *)"semiinfslab");

  // using omnidirectional electron accumulated fluence from previous section

  //  IMPORTANT:  Flux and Fluence inputs to the DoseModel *must* be:
  //               1pt Differential Flux type
  //               Omnidirectional only
  double *pda3DoseAccum = (double *)malloc(iNumTimes*iNumEleEnergies*iNumDepths * sizeof(double));
  ierr = DoseComputeFluenceDose(zDose, pda3AccumIntvFlue, iNumTimes,
                                iNumEleEnergies, pda3DoseAccum);
  if (ierr < 0 ) {
    fprintf(stderr, " dose computeFluence error: ierr=%d\n", ierr);
    return ierr;
  }
  else {
    printf("ae9 accumulated dose over full time period at depths: %.2f", daDepths[0]);
    for (nn=1; nn<iNumDepths; ++nn) {
      printf(", %.2f", daDepths[nn]);
    }
    printf(" mm\n");

    // report only the very last results
    ii = iNumTimes-1; // time index
    kk = 0; // direction index
    printf(" ae9 accum dose [%d][*][%d] = ", ii, kk);
    // show associated fluence values
    printf("%.3e", pda3DoseAccum[kk]);
    for (jj=1; jj<iNumDepths; ++jj) {  // depth index
      printf(", %.3e", pda3DoseAccum[jj*iNumDir + kk]);
    }
    printf("\n");
  }


  // free allocated memory
  free(pdaTimesMJD);
  free(pdaCoord1);
  free(pdaCoord2);
  free(pdaCoord3);

  free(pda3FluxDataMean);
  free(pda3AccumIntvFlue);
  free(pda3DoseAccum);

  ierr = EphemShutDown(zEphem);
  if (ierr < 0) return ierr;

  ierr = Ae9Ap9ShutDown(zAe9);
  if (ierr < 0) return ierr;

  ierr = DoseShutDown(zDose);
  if (ierr < 0) return ierr;

  return 0;
}
