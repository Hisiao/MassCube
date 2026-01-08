/***********************************************************************

 File: demoApp.cpp

 Description:

   Sample program to demonstrate usage of application-level API

 Classification :

   Unclassified

 Project Name:

   AE9/AP9/SPM Radiation Environment Models

   Developed under US Government contract # FA9453-12-C-0231

 Rights and Restrictions:

   Copyright 2017 Atmospheric and Environmental Research, Inc.(AER)

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
  
  % DemoAppC [-d path_to_"~/Irene/modelData"] [-x path_to_"~/Irene/<os>/bin"]

***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
  // only requires application-level api header file
#include "CApplication_c.h"

#ifdef _WIN32
extern int getopt(int argc, char** argv, const char* szArgFlags);
#endif

#include "AEVersion.h"

int help_args(char* pname)
{
  printf("\nUsage : %s -[options]\n\n", pname);

  printf("options:\n\n"
         "\t-x <str>\tpath to Irene executable files(default=current dir)\n"
         "\t-d <str>\tpath to database directory(default='../../modeData')\n"
         "\t-w <str>\tpath to 'working' directory for temp files(default=current dir)\n"
         "\t-b <str>\tname of directory for temp files(default=generated)\n"
         "\t-n <int>\tnumber of processors(including master node); default=1, 3 or larger\n"
         "\t-h \thelp\n"
         "\t-v \tversion\n\n");

  exit(1);

  return(0);
}

  // -- main program ---
int main(int argc, char** argv)
{
  int iInputCode;
  int iNumCpu = 1;
  char* szExecDir = (char *)"";
  char* szDataDir = (char *)"../../modelData";
  char* szWorkDir = (char *)"";
  char* szBinDir = NULL;
  extern char* optarg;

  // collect any command line arguments
  while((iInputCode = getopt(argc, argv, "x:d:w:b:n:vh")) != EOF) {

    switch(iInputCode) {
      case 'x':
        szExecDir = optarg;
        break;
      case 'd':
        szDataDir = optarg;
        break;
      case 'w':
        szWorkDir = optarg;
        break;
      case 'b':
        szBinDir = optarg;
        break;
      case 'n':
        iNumCpu = atoi(optarg);
        break;
      case 'v':
        printf("Irene Version: %s\n", getIreneVersion());
        exit(0);
        break;
      default:
        help_args(argv[0]);
    }
  }

  int ierr;
  int ii, jj, kk, nn;

  char *szGDZ = (char *)"GDZ", *szKm = (char *)"km";

  // initialize application class, define Exec(required) and Work dirs
  HANDLE zApp = AppStartUp();
  if (zApp < 0) return zApp;

  ierr = AppSetExecDir(zApp, szExecDir);
  if(ierr != 0) return ierr;
  ierr = AppSetWorkDir(zApp, szWorkDir);
  if(ierr != 0) return ierr;
  if (szBinDir != NULL) {
    ierr = AppSetBinDirName(zApp, szBinDir);
    if(ierr != 0) return ierr;
  }

  // define model, associated model database, coords db and magfield db
  ierr = AppSetModel(zApp, (char*)"AP9");  // protons-----V
  if(ierr != 0) return ierr;      //   specify proton database

// new way
  ierr = AppSetModelDBDir(zApp, szDataDir);
  if(ierr != 0) return ierr;
//
// old way
//  const int buffLen = strlen(szDataDir) + 30;
//  char *szModelDB, *szKPhiDB, *szKHMinDB, *szMagDB;
//  szModelDB = (char*)malloc(buffLen * sizeof(char));
//  strcpy(szModelDB, szDataDir);
//  strcat(szModelDB, (char*)"/AP9V15_runtime_tables.mat");
//  szKPhiDB = (char*)malloc(buffLen * sizeof(char));
//  strcpy(szKPhiDB, szDataDir);
//  strcat(szKPhiDB, (char*)"/fastPhi_net.mat");
//  szKHMinDB = (char*)malloc(buffLen * sizeof(char));
//  strcpy(szKHMinDB, szDataDir);
//  strcat(szKHMinDB, (char*)"/fast_hmin_net.mat");
//  szMagDB = (char*)malloc(buffLen * sizeof(char));
//  strcpy(szMagDB, szDataDir);
//  strcat(szMagDB, (char*)"/igrfDB.h5");
//
//  ierr = AppSetModelDBFile(zApp, szModelDB);
//  if(ierr != 0) return ierr;
//  ierr = AppSetKPhiDBFile(zApp, szKPhiDB);
//  if(ierr != 0) return ierr;
//  ierr = AppSetKHMinDBFile(zApp, szKHMinDB);
//  if(ierr != 0) return ierr;
//  ierr = AppSetMagfieldDBFile(zApp, szMagDB);
//  if(ierr != 0) return ierr;
//

  // define ephemeris generation limits and time step
  //  -note that these limits are _inclusive_, so this will produce *361* entries
  AppSetTimes(zApp, AppGetModifiedJulianDate(zApp, 2014,190,43200.0),
              AppGetModifiedJulianDate(zApp, 2014,190,64800.0), 60.0);
  // define propagator to use
  AppSetPropagator(zApp, (char*)"sateph");

  // define orbital characteristics
  // demonstrate full range of time conversion utility methods available
  AppSetElementTime(zApp, AppGetModifiedJulianDate(zApp, 2014,
                                                   AppGetDayOfYear(zApp, 2014, 7, 9),
                                                   AppGetGmtSeconds(zApp, 3, 25, 45.25)));
  AppSetRightAscension(zApp, 123.4);
  AppSetArgOfPerigee(zApp, 45.0);
  AppSetInclination(zApp, 35.0);
  AppSetAltitudeOfApogee(zApp, 2800.0);
  AppSetAltitudeOfPerigee(zApp, 1800.0);

  // define coord system and units of ephemeris
  AppSetCoordSys(zApp, szGDZ, szKm);

  // define flux type and energy levels
  AppSetFluxType(zApp, (char*)"1PtDiff");
  // define list of proton energy levels
  double adProE[] = { 0.1, 0.4, 0.8, 1, 2, 5, 10, 15, 20, 30, 50 };
  int iNumProEnergies = 11;
  AppSetFluxEnergies(zApp, adProE, iNumProEnergies);

  // define flux mode calculations to perform
  AppSetFluxMean(zApp, 1);
  AppSetFluxPerturbedScenRange(zApp, 1, 15);
  // AppSetFluxPercentile(zApp, 50);
  AppSetFluxPercentile(zApp, 75);
  // AppSetFluxPercentile(zApp, 95);
  // AppSetPitchAngle(zApp, 90);
  // AppSetPitchAngle(zApp, 85);
  // AppSetPitchAngle(zApp, 80);
  // AppSetPitchAngle(zApp, 75);

  // define calculation of fluence results and adiabatic coords
  AppSetFluence(zApp, 1);
  // AppSetAccumMode(zApp, (char*)"Cumulative");
  AppSetAccumMode(zApp, (char*)"Interval");
  AppSetAccumIntervalSec(zApp, 600.0);
  AppSetAdiabatic(zApp, 1);

  // define aggregation results for perturbed mean flux scenario values
  AppSetAggregMedian(zApp);
  // AppSetAggregConfLevel(zApp, 50);
  AppSetAggregConfLevel(zApp, 75);
  AppSetAggregConfLevel(zApp, 95);
  // double adDepths[] = { 0.1, 0.2, 0.4, 0.5 };
  // int iNumDepths = 4;
  // AppSetDoseDepths(zApp, adDepths, iNumDepths, (char*)"mm");
  // AppSetDoseRate(zApp, 1);

  // define number of processors to be used(includes one for 'control' node)
  ierr = AppSetNumProc(zApp, iNumCpu);
  if (ierr != 0) return ierr;

  // specify if temporary binary files are to be deleted when 'app' class
  //   object is destroyed, or 'resetmodel' is called
  AppSetDelBinDir(zApp, 0);

  // ====actual execution of all processing======
  //  --this spawns off all necessary applications
  //    to perform the requested model calculations
  ierr = AppRunModel(zApp);
  if(ierr != 0) {
    fprintf(stderr, " error in model run - availability of results is uncertain\n");
    return -1;
  }

  int iChunk = AppGetChunkSize(zApp);

  // 1 if omnidirectional, otherwise number of set pitch angles
  int iNumDir = AppGetNumDir(zApp);

  int iCount;
  // also get time, ephemeris and pitch angle data
  double *pdaDates = (double*)malloc(iChunk*sizeof(double));
  double *pdaCoord1 = (double*)malloc(iChunk*sizeof(double));
  double *pdaCoord2 = (double*)malloc(iChunk*sizeof(double));
  double *pdaCoord3 = (double*)malloc(iChunk*sizeof(double));
  double *pda2PitchAngles = (double*)malloc(iChunk*iNumDir*sizeof(double));
  double *pda3FluxData = (double*)malloc(iChunk*iNumProEnergies*iNumDir*sizeof(double));

  double *pda2Alpha = (double*)malloc(iChunk*iNumDir*sizeof(double));
  double *pda2Lm = (double*)malloc(iChunk*iNumDir*sizeof(double));
  double *pda2K = (double*)malloc(iChunk*iNumDir*sizeof(double));
  double *pda2Phi = (double*)malloc(iChunk*iNumDir*sizeof(double));
  double *pda2Hmin = (double*)malloc(iChunk*iNumDir*sizeof(double));
  double *pda2Lstar = (double*)malloc(iChunk*iNumDir*sizeof(double));
  double *pdaBmin = (double*)malloc(iChunk*sizeof(double));
  double *pdaBlocal = (double*)malloc(iChunk*sizeof(double));
  double *pdaMagLT = (double*)malloc(iChunk*sizeof(double));
  int iEnd = 0;

  // === all model run calculations have been performed, and are currently
  //     stored in binary files, in the temporary directory

  printf("\n === first round of data access, collect multiple parameters in tandem ===");
  printf("\n default chunk size of 960 is used, unless otherwise specified\n");
  // loop on *chunks* until end is detected(size of returned data vector is zero)

  // ** each of these 'getEphem/Data/etc,flyin' calls are reading one 'chunk's worth
  //    of results; multiple calls to the same routine are necessary to collect _all_
  //    the data; the AppSetChunkSize(<n>) may be used to adjust the chunk size.
  // --note that there will be fewer data records overall from accumulation-derived
  //    data calculations(such as interval fluences or doses)

  //   -->> when a particular type of results being requested by these calls
  //        were not specified in the settings prior to the call to 'runModel',
  //        an error message is shown, and no results will be returned.

  char *szCoordSys = (char*)malloc(16*sizeof(char));
  AppGetCoordSys(zApp, szCoordSys);
  char *szCoordUnits = (char*)malloc(16*sizeof(char));
  AppGetCoordSysUnits(zApp, szCoordUnits);

  while(!iEnd) {

    //  access one chunk's worth of ephemeris, and show first/last record
    printf(" ---- ephemeris -----\n");
    iCount = AppGetEphemeris(zApp, pdaDates, pdaCoord1, pdaCoord2, pdaCoord3);
    if(iCount < 0) {
      fprintf(stderr, " AppGetEphemeris error: ierr=%d\n", iCount);
    }
    else {
      printf("AppGetEphemeris returned iCount=%d\n", iCount);
      if(iCount > 0) {
        ii = 0;
        while(ii < iCount) {
          int iYear,iDdd;
          double dGmtsec;
          AppGetDateTime(zApp, pdaDates[ii], &iYear, &iDdd, &dGmtsec);
          printf("ephemeris (%s/%s) [%d]: %.5f(%d,%d,%.1f) at %.2f,%.2f,%.2f\n",
                 szCoordSys, szCoordUnits, ii, pdaDates[ii], iYear, iDdd, dGmtsec,
                 pdaCoord1[ii], pdaCoord2[ii], pdaCoord3[ii]);
          if(iCount==1)
            ++ii;
          else
            ii +=(iCount-1);
        }
      }
      else {
        printf("end of data reached\n");
        iEnd = 1;
      }
    }

    printf(" ---- flux mean -----\n");
    //  access one chunk's worth of 'mean' flux results, and show first/last record
    //   -->> no results will be returned if the particular type of results
    //        were not specified prior to the call to 'runModel'
    int iNum = AppFlyinMeanPlus(zApp, pdaDates, pdaCoord1, pdaCoord2, pdaCoord3,
                                pda2PitchAngles, pda3FluxData, "default", 1);
    if(iNum < 0) fprintf(stderr, " AppFlyinMeanPlus error: ierr=%d\n", iNum);
    else {
      printf("AppFlyinMeanPlus returned iNum=%d\n", iNum);
      if(iNum > 0) {
        ii=0;
        while(ii < iNum) {
          int iYear,iDdd;
          double dGmtsec;
          AppGetDateTime(zApp, pdaDates[ii], &iYear, &iDdd, &dGmtsec);
          printf("[%d]: %.5f(%d,%d,%.1f) at %.2f,%.2f,%.2f",
                 ii, pdaDates[ii], iYear, iDdd, dGmtsec,
                 pdaCoord1[ii], pdaCoord2[ii], pdaCoord3[ii]);
          if(iNumDir > 1) {
            printf("; pitch angles = ");
            for(jj=0; jj<iNumDir; ++jj)
              printf("%1f ", pda2PitchAngles[ii*iNumDir + jj]);
          }
          printf("\n");
          for(kk=0; kk<iNumDir; ++kk) {
            printf(" mean fluxes [%d] = ", kk);
            for(jj=0; jj<iNumProEnergies; ++jj)
              // since flux values are returned in a contiguous memory array,
              //   calculate index of flux for specific time entry, energy and direction
              printf("%.3e ", pda3FluxData[ii*iNumProEnergies*iNumDir + jj*iNumDir + kk]);
            printf("\n");
          }
          if(iNum == 1)
            ++ii;
          else
            ii += iNum-1;
        }
      }
      else {
        printf("end of data reached\n");
        iEnd = 1;
      }
    }

    printf(" ---- flux 95th percentile -----\n");
    //  access one chunk's worth of 95th 'percentile' flux results, and show first/last record
    iNum = AppFlyinPercentilePlus(zApp, 95, pdaDates, pdaCoord1, pdaCoord2, pdaCoord3,
                                  pda2PitchAngles, pda3FluxData, "default", 1);
    if(iNum < 0) fprintf(stderr, " AppFlyinPercentilePlus error: ierr=%d\n", iNum);
    else {
      printf("AppFlyinPercentilePlus returned iNum=%d\n", iNum);
      if(iNum > 0) {
        ii=0;
        while(ii < iNum) {
          int iYear,iDdd;
          double dGmtsec;
          AppGetDateTime(zApp, pdaDates[ii], &iYear, &iDdd, &dGmtsec);
          printf("[%d]: %.5f(%d,%d,%.1f) at %.2f,%.2f,%.2f",
                 ii, pdaDates[ii], iYear, iDdd, dGmtsec,
                 pdaCoord1[ii], pdaCoord2[ii], pdaCoord3[ii]);
          if(iNumDir > 1) {
            printf("; pitch angles = ");
            for(jj=0; jj<iNumDir; ++jj)
              printf("%1f ", pda2PitchAngles[ii*iNumDir + jj]);
          }
          printf("\n");
          for(kk=0; kk<iNumDir; ++kk) {
            printf(" 95perc fluxes [%d] = ", kk);
            for(jj=0; jj<iNumProEnergies; ++jj)
              // since flux values are returned in a contiguous memory array,
              //   calculate index of flux for specific time entry, energy and direction
              printf("%.3e ", pda3FluxData[ii*iNumProEnergies*iNumDir + jj*iNumDir + kk]);
            printf("\n");
          }
          if(iNum == 1)
            ++ii;
          else
            ii += iNum-1;
        }
      }
      else {
        printf("end of data reached\n");
        iEnd = 1;
      }
    }

    printf(" ---- flux perturbed mean scenarios [Avg] -----\n");
    //  access one chunk's worth of 'perturbed mean' flux scenario results,
    //     and show first/last record
    for(nn=1;nn<20;nn+=8){
      iNum = AppFlyinPerturbedMeanPlus(zApp, nn, pdaDates, pdaCoord1, pdaCoord2, pdaCoord3,
                                       pda2PitchAngles, pda3FluxData, "interval", 1);
      if(iNum < 0) fprintf(stderr, " AppFlyinPerturbedMeanPlus error: ierr=%d\n", iNum);
      else {
        printf("AppFlyinPerturbedMeanPlus returned iNum=%d\n", iNum);
        if(iNum > 0) {
          ii=0;
          while(ii < iNum) {
            int iYear,iDdd;
            double dGmtsec;
            AppGetDateTime(zApp, pdaDates[ii], &iYear, &iDdd, &dGmtsec);
            printf("[%d]: %.5f(%d,%d,%.1f) at %.2f,%.2f,%.2f",
                   ii, pdaDates[ii], iYear, iDdd, dGmtsec,
                   pdaCoord1[ii], pdaCoord2[ii], pdaCoord3[ii]);
            if(iNumDir > 1) {
              printf("; pitch angles = ");
              for(jj=0; jj<iNumDir; ++jj)
                printf("%1f ", pda2PitchAngles[ii*iNumDir + jj]);
            }
            printf("\n");
            for(kk=0; kk<iNumDir; ++kk) {
              printf(" pert scen %d fluxes [%d] = ", nn, kk);
              for(jj=0; jj<iNumProEnergies; ++jj)
                // since flux values are returned in a contiguous memory array,
                //   calculate index of flux for specific time entry, energy and direction
                printf("%.3e ", pda3FluxData[ii*iNumProEnergies*iNumDir + jj*iNumDir + kk]);
              printf("\n");
            }
            if(iNum == 1)
              ++ii;
            else
              ii += iNum-1;
          }
        }
        else {
          //(disabled for accum data collection)  iEnd = 1;

          //  -- since the 'flux interval average' values are requested here,
          //     fewer 'chunks' are required to collect all of the data.
          //    Therefore, collecting in tandem could cause a premature exit
          //     from this loop before all the non-accumulated data is collected.
        }
      }
    }

    printf(" ---- flux perturbed mean aggregations -----\n");
    //  access one chunk's worth of aggregation results of 'perturbed mean' flux
    //    scenarios, and show first/last record
    int mm=50;
    while(mm<100) {
      iNum = AppGetAggregData(zApp, (char*)"flux", (char*)"perturbed", mm,
                              pdaDates, pdaCoord1, pdaCoord2, pdaCoord3,
                              pda2PitchAngles, pda3FluxData, "default", 1);
      if(iNum < 0) fprintf(stderr, " AppGetAggregData error: ierr=%d\n", iNum);
      else {
        printf("AppGetAggregData returned iNum=%d\n", iNum);
        if(iNum > 0) {
          ii=0;
          while(ii < iNum) {
            int iYear,iDdd;
            double dGmtsec;
            AppGetDateTime(zApp, pdaDates[ii], &iYear, &iDdd, &dGmtsec);
            printf("[%d]: %.5f(%d,%d,%.1f) at %.2f,%.2f,%.2f",
                   ii, pdaDates[ii], iYear, iDdd, dGmtsec,
                   pdaCoord1[ii], pdaCoord2[ii], pdaCoord3[ii]);
            if(iNumDir > 1) {
              printf("; pitch angles = ");
              for(jj=0; jj<iNumDir; ++jj)
                printf("%1f ", pda2PitchAngles[ii*iNumDir + jj]);
            }
            printf("\n");
            for(kk=0; kk<iNumDir; ++kk) {
              printf(" pert aggreg %d fluxes [%d] = ", mm, kk);
              for(jj=0; jj<iNumProEnergies; ++jj)
                // since flux values are returned in a contiguous memory array,
                //   calculate index of flux for specific time entry, energy and direction
                printf("%.3e ", pda3FluxData[ii*iNumProEnergies*iNumDir + jj*iNumDir + kk]);
              printf("\n");
            }
            if(iNum == 1)
              ++ii;
            else
              ii += iNum-1;
          }
        }
        else {
          printf("end of data reached\n");
          iEnd = 1;
        }
      }
      if(mm==99) mm = 100;
      if(mm==95) mm = 99;
      if(mm==75) mm = 95;
      if(mm==50) mm = 75;
    }

    printf(" ---- fluence perturbed mean aggregations -----\n");
    //  access one chunk's worth of aggregation results of 'perturbed mean'
    //      fluence scenarios, and show first/last record
    //   --> note that this fluence data is derived from an accumulation,
    //        and therefore contain fewer entries of data to be collected
    mm=50;
    while(mm<100) {
      iNum = AppGetAggregData(zApp, (char*)"fluence", (char*)"perturbed", mm,
                              pdaDates, pdaCoord1, pdaCoord2, pdaCoord3,
                              pda2PitchAngles, pda3FluxData, "default", 1);
      if(iNum < 0) fprintf(stderr, " AppGetAggregData error: ierr=%d\n", iNum);
      else {
        printf("AppGetAggregData returned iNum=%d\n", iNum);
        if(iNum > 0) {
          ii=0;
          while(ii < iNum) {
            int iYear,iDdd;
            double dGmtsec;
            AppGetDateTime(zApp, pdaDates[ii], &iYear, &iDdd, &dGmtsec);
            printf("[%d]: %.5f(%d,%d,%.1f) at %.2f,%.2f,%.2f",
                   ii, pdaDates[ii], iYear, iDdd, dGmtsec,
                   pdaCoord1[ii], pdaCoord2[ii], pdaCoord3[ii]);
            if(iNumDir > 1) {
              printf("; pitch angles = ");
              for(jj=0; jj<iNumDir; ++jj)
                printf("%1f ", pda2PitchAngles[ii*iNumDir + jj]);
            }
            printf("\n");
            for(kk=0; kk<iNumDir; ++kk) {
              printf(" pert aggreg %d fluxes [%d] = ", mm, kk);
              for(jj=0; jj<iNumProEnergies; ++jj)
                // since flux values are returned in a contiguous memory array,
                //   calculate index of flux for specific time entry, energy and direction
                printf("%.3e ", pda3FluxData[ii*iNumProEnergies*iNumDir + jj*iNumDir + kk]);
              printf("\n");
            }
            if(iNum == 1)
              ++ii;
            else
              ii += iNum-1;
          }
        }
        else {
          //(disabled for accum data collection)  iEnd = 1;

          //  -- since the 'fluence' values are requested here,
          //     fewer 'chunks' are required to collect all of the data.
          //    Therefore, collecting in tandem could cause a premature exit
          //     from this loop before all the non-accumulated data is collected.
        }
      }
      if(mm==99) mm = 100;
      if(mm==95) mm = 99;
      if(mm==75) mm = 95;
      if(mm==50) mm = 75;
    }

    printf(" ---- adiabatic coords -----\n");
    // access one chunk's worth of adiabatic coordinate values, and show first/last record
    iNum = AppGetAdiabaticCoordsPlus(zApp, pdaDates, pdaCoord1, pdaCoord2, pdaCoord3,
                                     pda2PitchAngles, pda2Alpha, pda2Lm,
                                     pda2K, pda2Phi, pda2Hmin, pda2Lstar,
                                     pdaBmin, pdaBlocal, pdaMagLT);
    if(iNum < 0) fprintf(stderr, " AppGetAdiabaticCoordsPlus error: ierr=%d\n", iNum);
    else {
      printf("AppGetAdiabaticCoordsPlus returned iNum=%d\n", iNum);
      if(iNum > 0) {
        ii=0;
        while(ii < iNum) {
          int iYear,iDdd;
          double dGmtsec;
          AppGetDateTime(zApp, pdaDates[ii], &iYear, &iDdd, &dGmtsec);
          printf("[%d]: %.5f(%d,%d,%.1f) at %.2f,%.2f,%.2f",
                 ii, pdaDates[ii], iYear, iDdd, dGmtsec,
                 pdaCoord1[ii], pdaCoord2[ii], pdaCoord3[ii]);
          if(iNumDir > 1) {
            printf("; pitch angles = ");
            for(jj=0; jj<iNumDir; ++jj)
              printf("%1f ", pda2PitchAngles[ii*iNumDir + jj]);
          }
          printf("\n");
          for(kk=0; kk<iNumDir; ++kk) {
            printf(" adiabat values [%d] = %.2f %.3f %.5f %.5f %.1f %.3f %.3e %.3e %.2f\n",
                   kk, pda2Alpha[ii*iNumDir + kk], pda2Lm[ii*iNumDir + kk],
                   pda2K[ii*iNumDir + kk],  pda2Phi[ii*iNumDir + kk],
                   pda2Hmin[ii*iNumDir + kk], pda2Lstar[ii*iNumDir + kk],
                   pdaBmin[ii], pdaBlocal[ii], pdaMagLT[ii]);
          }
          if(iNum==1)
            ++ii;
          else
            ii += iNum-1;
        }
      }
      else {
        printf("end of data reached\n");
        iEnd = 1;
      }
    }
  } // end of while !iEnd

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
  free(szCoordSys);
  free(szCoordUnits);


  // data does not have to be all accessed in tandem
  //  -- different results can be collected independently

  // change chunk size of data access
  AppSetChunkSize(zApp, 60);
  // -- a chunk size change causes an implied 'reset' of the data access routines,
  //      so any subsequent 'flyin'/'get' data calls start at the beginning
  //      of the time period

  printf("\n === second round of data access, collect parameters independently ===\n");
  printf(" ** changed chunk size to 60 **\n");

  iEnd = 0;

  printf(" ---- flux 75th percentile -----\n");
  while(!iEnd) {
    //  access one chunk's worth of 75th 'percentile' flux results, and show first/last record
    //   -->> no results will be returned if the particular type of results
    //        were not specified prior to the call to 'runModel'
  	int iNum = AppFlyinPercentilePlus(zApp, 75, pdaDates, pdaCoord1, pdaCoord2, pdaCoord3,
                                      pda2PitchAngles, pda3FluxData, "default", 1);
    if(iNum < 0) {
      fprintf(stderr, " AppFlyinPercentilePlus error: ierr=%d\n", iNum);
      iEnd = 1;  // end when error
    }
    else {
      printf("AppFlyinPercentilePlus returned iNum=%d\n", iNum);
      if(iNum > 0) {
        ii=0;
        while(ii < iNum) {
          int iYear,iDdd;
          double dGmtsec;
          AppGetDateTime(zApp, pdaDates[ii], &iYear, &iDdd, &dGmtsec);
          printf("[%d]: %.5f(%d,%d,%.1f) at %.2f,%.2f,%.2f",
                 ii, pdaDates[ii], iYear, iDdd, dGmtsec,
                 pdaCoord1[ii], pdaCoord2[ii], pdaCoord3[ii]);
          if(iNumDir > 1) {
            printf("; pitch angles = ");
            for(jj=0; jj<iNumDir; ++jj)
              printf("%1f ", pda2PitchAngles[ii*iNumDir + jj]);
          }
          printf("\n");
          for(kk=0; kk<iNumDir; ++kk) {
            printf(" 75perc fluxes [%d] = ", kk);
            for(jj=0; jj<iNumProEnergies; ++jj)
              // since flux values are returned in a contiguous memory array,
              //   calculate index of flux for specific time entry, energy and direction
              printf("%.3e ", pda3FluxData[ii*iNumProEnergies*iNumDir + jj*iNumDir + kk]);
            printf("\n");
          }
          if(iNum == 1)
            ++ii;
          else
            ii += iNum-1;
        }
      }
      else {
        printf("end of data reached\n");
        iEnd = 1;
      }
    }
  } // end of while !iEnd

  // now access the *interval average* of this same data
  //  - note the additional 'true' argument in AppFlyinPercentile call
  iEnd = 0;

  printf(" ---- *interval average* flux 75th percentile -----\n");
  while(!iEnd) {
  //  access one chunk's worth of 75th 'percentile' flux results, and show first/last record
  //   -->> no results will be returned if the particular type of results
  //        were not specified prior to the call to 'runModel'
  	int iNum = AppFlyinPercentilePlus(zApp, 75, pdaDates, pdaCoord1, pdaCoord2, pdaCoord3,
                                      pda2PitchAngles, pda3FluxData, "interval", 1);
    if(iNum < 0) {
      fprintf(stderr, " AppFlyinPercentilePlus error: ierr=%d\n", iNum);
      iEnd = 1;  // end when error
    }
    else {
      printf("AppFlyinPercentilePlus returned iNum=%d\n", iNum);
      if(iNum > 0) {
        ii=0;
        while(ii < iNum) {
          int iYear,iDdd;
          double dGmtsec;
          AppGetDateTime(zApp, pdaDates[ii], &iYear, &iDdd, &dGmtsec);
          printf("[%d]: %.5f(%d,%d,%.1f) at %.2f,%.2f,%.2f",
                 ii, pdaDates[ii], iYear, iDdd, dGmtsec,
                 pdaCoord1[ii], pdaCoord2[ii], pdaCoord3[ii]);
          if(iNumDir > 1) {
            printf("; pitch angles = ");
            for(jj=0; jj<iNumDir; ++jj)
              printf("%1f ", pda2PitchAngles[ii*iNumDir + jj]);
          }
          printf("\n");
          for(kk=0; kk<iNumDir; ++kk) {
            printf(" 75perc fluxes [%d] = ", kk);
            for(jj=0; jj<iNumProEnergies; ++jj)
              // since flux values are returned in a contiguous memory array,
              //   calculate index of flux for specific time entry, energy and direction
              printf("%.3e ", pda3FluxData[ii*iNumProEnergies*iNumDir + jj*iNumDir + kk]);
            printf("\n");
          }
          if(iNum == 1)
            ++ii;
          else
            ii += iNum-1;
        }
      }
      else {
        printf("end of data reached\n");
        iEnd = 1;
      }
    }

  } // end of while !iEnd


  iEnd = 0;

  // === all model run calculations have been performed, and are currently
  //     stored in binary files, in the temporary directory

  printf("\n === third round of data access, collect repeated parameter after reset ===\n");
  // call to reset the data access
  AppResetModelData(zApp);
  // -- an explicit 'reset' of the data access routines, so any subsequent
  //    'flyin'/'get' data calls start at the beginning of the time period

  szCoordSys = (char*)malloc(16*sizeof(char));
  AppGetCoordSys(zApp, szCoordSys);
  szCoordUnits = (char*)malloc(16*sizeof(char));
  AppGetCoordSysUnits(zApp, szCoordUnits);

  while(!iEnd) {

    //  access one chunk's worth of ephemeris, and show first/last record
    printf(" ---- ephemeris -----\n");
    iCount = AppGetEphemeris(zApp, pdaDates, pdaCoord1, pdaCoord2, pdaCoord3);
    if(iCount < 0) {
      fprintf(stderr, " AppGetEphemeris error: ierr=%d\n", iCount);
    }
    else {
      printf("AppGetEphemeris returned iCount=%d\n", iCount);
      if(iCount > 0) {
        ii = 0;
        while(ii < iCount) {
          int iYear,iDdd;
          double dGmtsec;
          AppGetDateTime(zApp, pdaDates[ii], &iYear, &iDdd, &dGmtsec);
          printf("ephemeris (%s/%s) [%d]: %.5f(%d,%d,%.1f) at %.2f,%.2f,%.2f\n",
                 szCoordSys, szCoordUnits, ii, pdaDates[ii], iYear, iDdd, dGmtsec,
                 pdaCoord1[ii], pdaCoord2[ii], pdaCoord3[ii]);
          if(iCount==1)
            ++ii;
          else
            ii +=(iCount-1);
        }
      }
      else {
        printf("end of data reached\n");
        iEnd = 1;
      }
    }
  } // end of while !iEnd


  // free allocated memory
  free(pdaDates);
  free(pdaCoord1);
  free(pdaCoord2);
  free(pdaCoord3);
  free(pda2PitchAngles);
  free(pda3FluxData);
  free(szCoordSys);
  free(szCoordUnits);

  return 0;
}
