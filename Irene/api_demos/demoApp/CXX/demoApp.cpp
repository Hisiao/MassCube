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

 Build Instructions:
  Linux:
    in local directory
  % cmake -DIRENE_ROOT=<path_to_"~/Irene/linux"> .
      (requires 'cmake' version 3.14 or greater)
  % make
  
  % DemoApp [-d path_to_"~/Irene/modelData"] [-x path_to_"~/Irene/<os>/bin"]

***********************************************************************/

// only requires application-level api header file
#include "CApplication.h"

#ifdef _WIN32
#include <windows.h>
extern int getopt(int argc, char** argv, const char* szArgFlags);
#else
#include <unistd.h>
#include <string.h>
#endif
#include <cstdlib>

#include "AEVersion.h"

using namespace std;

int help_args(char* pname)
{
  cerr << endl
       << "Usage : " << pname << " -[options]"
       << endl << endl;

  cerr << "options: " << endl << endl
       << "\t-x <str>\tpath to Irene executable files (default=current dir)" << endl
       << "\t-d <str>\tpath to database directory (default='../../modeData')" << endl
       << "\t-w <str>\tpath to 'working' directory for temp files (default=current dir)" << endl
       << "\t-b <str>\tname of directory for temp files (default=generated)" << endl
       << "\t-n <int>\tnumber of processors (including master node); default=1, 3 or larger" << endl
       << "\t-h \thelp" << endl
       << "\t-v \tversion" << endl << endl;

  exit(1);

  return (0);
}

// -- main program ---
int main (int argc, char** argv)
{
  int iInputCode;
  int iNumCpu = 1;
  std::string strExecDir;
  std::string strDataDir = "../../modelData";
  std::string strWorkDir;
  std::string strBinDir;
  extern char* optarg;

// collect any command line arguments
  while ((iInputCode = getopt(argc, argv, "x:d:w:b:n:vh")) != EOF) {

    switch (iInputCode) {
      case 'x':
        strExecDir = string(optarg);
        break;
      case 'd':
        strDataDir = string(optarg);
        break;
      case 'w':
        strWorkDir = string(optarg);
        break;
      case 'b':
        strBinDir = string(optarg);
        break;
      case 'n':
        iNumCpu = atoi(optarg);
        break;
      case 'v':
        cout << "Irene Version: " << getIreneVersion() << endl; exit(0);
        break;
      default:
        help_args( argv[0] );
    }
  }

  int ierr;
//  initialize application class, define Exec(required) and Work dirs
  Application app;

  ierr = app.setExecDir( strExecDir );
  if ( ierr != 0 ) return ierr;
  if ( ! strWorkDir.empty() ) {
    ierr = app.setWorkDir( strWorkDir );
    if ( ierr != 0 ) return ierr;
  }
  if ( ! strBinDir.empty() ) {
    ierr = app.setBinDirName( strBinDir );
    if ( ierr != 0 ) return ierr;
  }


//  define model, associated model database, coords db and magfield db
  ierr = app.setModel("AP9");  // protons-----V
  if ( ierr != 0 ) return ierr;       //   specify proton database

// new way
  ierr = app.setModelDBDir( strDataDir );
  if ( ierr != 0 ) return ierr;
//
// old way
//  ierr = app.setModelDBFile( strDataDir+"/AP9V15_runtime_tables.mat" );
//  if ( ierr != 0 ) return ierr;
//  ierr = app.setKPhiDBFile( strDataDir+"/fastPhi_net.mat" );
//  if ( ierr != 0 ) return ierr;
//  ierr = app.setKHMinDBFile( strDataDir+"/fast_hmin_net.mat" );
//  if ( ierr != 0 ) return ierr;
//  ierr = app.setMagfieldDBFile( strDataDir+"/igrfDB.h5" );
//  if ( ierr != 0 ) return ierr;
//

//  define ephemeris generation limits and time step
//   -note that these limits are _inclusive_, so this will produce *361* entries
  app.setTimes( app.getModifiedJulianDate (2014,190,43200.0),
                app.getModifiedJulianDate (2014,190,64800.0), 60.0 );
//  define propagator to use
  app.setPropagator( "sateph" );

//  define orbital characteristics
  // demonstrate full range of time conversion utility methods available
  app.setElementTime( app.getModifiedJulianDate (2014,
                                                 app.getDayOfYear(2014,7,9),
                                                 app.getGmtSeconds(3,25,45.25)) );
  app.setRightAscension( 123.4 );
  app.setArgOfPerigee( 45.0 );
  app.setInclination( 35.0 );
  app.setAltitudeOfApogee( 2800.0 );
  app.setAltitudeOfPerigee( 1800.0 );

// define coord system and units of ephemeris
  app.setCoordSys("GDZ","km");

//  define flux type and energy levels
  app.setFluxType( "1PtDiff" );
//  define list of proton energy levels
  double adProE[] = { 0.1,0.4,0.8,1,2,5,10,15,20,30,50 };
  dvector vdProEnergies( adProE, adProE + sizeof(adProE) / sizeof(adProE[0]) );
  app.setFluxEnergies( vdProEnergies );

//  define flux mode calculations to perform
  app.setFluxMean();
  app.setFluxPerturbedScenRange ( 1, 15);
//  app.setFluxPercentile(50);
  app.setFluxPercentile(75);
//  app.setFluxPercentile(95);
//  app.setPitchAngle(90);
//  app.setPitchAngle(85);
//  app.setPitchAngle(80);
//  app.setPitchAngle(75);

//  define calculation of fluence results and adiabatic coords
  app.setFluence();
//  app.setAccumMode("Cumulative");
  app.setAccumMode("Interval");
  app.setAccumIntervalSec( 600.0 );
  app.setAdiabatic();

//  define aggregation results for perturbed mean flux scenario values
  app.setAggregMedian();
//  app.setAggregConfLevel(50);
  app.setAggregConfLevel(75);
  app.setAggregConfLevel(95);
//  double adDepths[] = { 0.1, 0.2, 0.4, 0.5 };
//  dvector vdDepths;
//  for (int ii=0; ii<4; ++ii) vdDepths.push_back(adDepths[ii]);
//  app.setDoseDepths( vdDepths, "mm" );
//  app.setDoseRate();

//  define number of processors to be used (includes one for 'control' node)
  ierr = app.setNumProc(iNumCpu);
  if ( ierr != 0 ) return ierr;

//  specify if temporary binary files are to be deleted when 'app' class
//    object is destroyed, or 'resetmodel' is called
  app.setDelBinDir(false);

//  ====actual execution of all processing======
//   --this spawns off all necessary applications
//     to perform the requested model calculations
  ierr = app.runModel();
  if ( ierr != 0 ) {
    cerr << " error in model run - availability of results is uncertain " << endl;
    return -1;
  }

  int iCount;
  dvector vdDates;   // also get time, ephemeris and pitch angle data
  dvector vdCoord1;
  dvector vdCoord2;
  dvector vdCoord3;
  vdvector vvdPitchAngles;
  vvdvector vvvdFluxData;
  bool bEnd = false;

// === all model run calculations have been performed, and are currently
//      stored in binary files, in the temporary directory

cout << endl << " === first round of data access, collect multiple parameters in tandem ===" <<endl;
cout << " default chunk size of 960 is used, unless otherwise specified" <<endl;
 // loop on *chunks* until end is detected (size of returned data vector is zero)

  // ** each of these 'getEphem/Data/etc,flyin' calls are reading one 'chunk's worth
  //    of results; multiple calls to the same routine are necessary to collect _all_
  //    the data; the app.setChunkSize(<n>) may be used to adjust the chunk size.
  // --note that there will be fewer data records overall from accumulation-derived
  //    data calculations (such as interval fluences or doses)

//    -->> when a particular type of results being requested by these calls
//         were not specified in the settings prior to the call to 'runModel',
//         an error message is shown, and no results will be returned.

  while ( !bEnd ) {

//   access one chunk's worth of ephemeris, and show first/last record
    std::string strCoordSys = app.getCoordSys( );
    std::string strCoordUnits = app.getCoordSysUnits( );
cout << " ---- ephemeris ----- " << endl;
    iCount = app.getEphemeris( vdDates, vdCoord1, vdCoord2, vdCoord3 );
    if ( iCount < 0 ) {
      cerr << " app.getEphemeris error: ierr="<<iCount<<endl;
    }
    else {
      cout << "sizes: vdDates="<<vdDates.size()<<"; vdCoord1="<<vdCoord1.size()<<endl;
      if ( iCount>0 ) {
        int ii=0;
        while (ii<iCount ) {
          int iYear,iDdd;
          double dGmtsec;
          app.getDateTime( vdDates[ii], iYear, iDdd, dGmtsec );
          cout << "ephemeris ("<<strCoordSys<<"/"<<strCoordUnits<<") ["<<ii<<"]: "
               <<vdDates[ii]<<"("<<iYear<<","<<iDdd<<","<<dGmtsec<<") at "
               <<vdCoord1[ii]<<","<<vdCoord2[ii]<<","<<vdCoord3[ii]<<endl;
          if ( iCount==1 )
            ++ii;
          else
            ii += (iCount-1);
        }
      }
      else {
        cout << "end of data reached"<<endl;
        bEnd = true;
      }
    }

cout << " ---- flux mean ----- " << endl;
//   access one chunk's worth of 'mean' flux results, and show first/last record
//    -->> no results will be returned if the particular type of results
//         were not specified prior to the call to 'runModel'
   int iNum = app.flyinMean( vdDates, vdCoord1, vdCoord2, vdCoord3,
                             vvdPitchAngles, vvvdFluxData );
    if ( iNum >= 0 ) {
      cout << "sizes: vdDates="<<vdDates.size()<<"; vdCoord1="<<vdCoord1.size()
           <<"; flux="<<vvvdFluxData.size()<<",";
      if ( int(vvvdFluxData.size())>0 ) {
        cout << vvvdFluxData[0].size()<<","<<vvvdFluxData[0][0].size()<<endl;
        size_t ii=0;
        while (ii<vvvdFluxData.size() ) {
          int iYear,iDdd;
          double dGmtsec;
          app.getDateTime( vdDates[ii], iYear, iDdd, dGmtsec );
          cout << "["<<ii<<"]: "<<vdDates[ii]<<"("<<iYear<<","<<iDdd<<","<<dGmtsec
               <<") at "<<vdCoord1[ii]<<","<<vdCoord2[ii]<<","<<vdCoord3[ii];
          if ( int(vvdPitchAngles.size())>0 ) {
            cout << "; pitch angles = ";
            for (size_t jj=0; jj<vvdPitchAngles[ii].size(); ++jj)
              cout << vvdPitchAngles[ii][jj] << " ";
          }
          cout << endl;
          for (size_t kk=0; kk<vvvdFluxData[ii][0].size(); ++kk) {
            cout << " mean fluxes ["<<kk<<"] = ";
            for (size_t jj=0; jj<vvvdFluxData[ii].size(); ++jj)
              cout << vvvdFluxData[ii][jj][kk]<<" ";
            cout << endl;
          }
          if ( int(vdDates.size())==1 )
            ++ii;
          else
            ii += (vdDates.size()-1);
        }
      }
      else {
        cout << "-,-"<<endl;
        cout << "end of data reached"<<endl;
        bEnd = true;
      }
    }

cout << " ---- flux 95th percentile ----- " << endl;
//   access one chunk's worth of 95th 'percentile' flux results, and show first/last record
    iNum = app.flyinPercentile( 95, vdDates, vdCoord1, vdCoord2, vdCoord3,
                                vvdPitchAngles, vvvdFluxData );
    if ( iNum >= 0 ) {
      cout << "sizes: vdDates="<<vdDates.size()<<"; vdCoord1="<<vdCoord1.size()
           <<"; flux="<<vvvdFluxData.size()<<",";
      if ( int(vvvdFluxData.size())>0 ) {
        cout << vvvdFluxData[0].size()<<","<<vvvdFluxData[0][0].size()<<endl;
        size_t ii=0;
        while (ii<vvvdFluxData.size() ) {
          int iYear,iDdd;
          double dGmtsec;
          app.getDateTime( vdDates[ii], iYear, iDdd, dGmtsec );
          cout << "["<<ii<<"]: "<<vdDates[ii]<<"("<<iYear<<","<<iDdd<<","<<dGmtsec
               <<") at "<<vdCoord1[ii]<<","<<vdCoord2[ii]<<","<<vdCoord3[ii];
          if ( int(vvdPitchAngles.size())>0 ) {
            cout << "; pitch angles = ";
            for (size_t jj=0; jj<vvdPitchAngles[ii].size(); ++jj)
              cout << vvdPitchAngles[ii][jj] << " ";
          }
          cout << endl;
          for (size_t kk=0; kk<vvvdFluxData[ii][0].size(); ++kk) {
            cout << " 95perc fluxes ["<<kk<<"] = ";
            for (size_t jj=0; jj<vvvdFluxData[ii].size(); ++jj)
              cout << vvvdFluxData[ii][jj][kk]<<" ";
            cout << endl;
          }
          if ( int(vdDates.size())==1 )
            ++ii;
          else
            ii += (vdDates.size()-1);
        }
      }
      else {
        cout << "-,-"<<endl;
        cout << "end of data reached"<<endl;
        bEnd = true;
      }
    }

cout << " ---- flux perturbed mean scenarios [IntvAvg] ----- " << endl;
//   access one chunk's worth of 'perturbed mean' flux scenario results,
//      and show first/last record
  for (int nn=1;nn<20;nn+=8){
    iNum = app.flyinPerturbedMean( nn, vdDates, vdCoord1, vdCoord2, vdCoord3,
                                   vvdPitchAngles, vvvdFluxData, "interval" );
    if ( iNum >= 0 ) {
      cout << "sizes: vdDates="<<vdDates.size()<<"; vdCoord1="<<vdCoord1.size()
           <<"; flux="<<vvvdFluxData.size()<<",";
      if ( int(vvvdFluxData.size())>0 ) {
        cout << vvvdFluxData[0].size()<<","<<vvvdFluxData[0][0].size()<<endl;
        size_t ii=0;
        while (ii<vvvdFluxData.size() ) {
          int iYear,iDdd;
          double dGmtsec;
          app.getDateTime( vdDates[ii], iYear, iDdd, dGmtsec );
          cout << "["<<ii<<"]: "<<vdDates[ii]<<"("<<iYear<<","<<iDdd<<","<<dGmtsec
               <<") at "<<vdCoord1[ii]<<","<<vdCoord2[ii]<<","<<vdCoord3[ii];
          if ( int(vvdPitchAngles.size())>0 ) {
            cout << "; pitch angles = ";
            for (size_t jj=0; jj<vvdPitchAngles[ii].size(); ++jj)
              cout << vvdPitchAngles[ii][jj] << " ";
          }
          cout << endl;
          for (size_t kk=0; kk<vvvdFluxData[ii][0].size(); ++kk) {
            cout << " pert scen "<<nn<<" fluxes ["<<kk<<"] = ";
            for (size_t jj=0; jj<vvvdFluxData[ii].size(); ++jj)
              cout << vvvdFluxData[ii][jj][kk]<<" ";
            cout << endl;
          }
          if ( int(vdDates.size())==1 )
            ++ii;
          else
            ii += (vdDates.size()-1);
        }
      }
      else {
        cout << "-,-"<<endl;
        // (disabled for accum data collection)  bEnd = true;

        //  -- since the 'flux interval average' values are requested here,
        //     fewer 'chunks' are required to collect all of the data.
        //    Therefore, collecting in tandem could cause a premature exit
        //     from this loop before all the non-accumulated data is collected.
      }
    }
  }

cout << " ---- flux perturbed mean aggregations ----- " << endl;
//   access one chunk's worth of aggregation results of 'perturbed mean' flux
//     scenarios, and show first/last record
  int mm=50;
  while (mm<100) {
    iNum = app.getAggregData ( "flux", "perturbed", mm, vdDates,
                               vdCoord1, vdCoord2, vdCoord3,
                               vvdPitchAngles, vvvdFluxData );
    if ( iNum >= 0 ) {
      cout << "sizes: vdDates="<<vdDates.size()<<"; vdCoord1="<<vdCoord1.size()
           <<"; flux="<<vvvdFluxData.size()<<",";
      if ( int(vvvdFluxData.size())>0 ) {
        cout << vvvdFluxData[0].size()<<","<<vvvdFluxData[0][0].size()<<endl;
        size_t ii=0;
        while (ii<vvvdFluxData.size() ) {
          int iYear,iDdd;
          double dGmtsec;
          app.getDateTime( vdDates[ii], iYear, iDdd, dGmtsec );
          cout << "["<<ii<<"]: "<<vdDates[ii]<<"("<<iYear<<","<<iDdd<<","<<dGmtsec
               <<") at "<<vdCoord1[ii]<<","<<vdCoord2[ii]<<","<<vdCoord3[ii];
          if ( int(vvdPitchAngles.size())>0 ) {
            cout << "; pitch angles = ";
            for (size_t jj=0; jj<vvdPitchAngles[ii].size(); ++jj)
              cout << vvdPitchAngles[ii][jj] << " ";
          }
          cout << endl;
          for (size_t kk=0; kk<vvvdFluxData[ii][0].size(); ++kk) {
            cout << " pert aggreg "<<mm<<" fluxes ["<<kk<<"] = ";
            for (size_t jj=0; jj<vvvdFluxData[ii].size(); ++jj)
              cout << vvvdFluxData[ii][jj][kk]<<" ";
            cout << endl;
          }
          if ( int(vdDates.size())==1 )
            ++ii;
          else
            ii += (vdDates.size()-1);
        }
      }
      else {
        cout << "-,-"<<endl;
        cout << "end of data reached"<<endl;
        bEnd = true;
      }
    }
    if ( mm==99 ) mm = 100;
    if ( mm==95 ) mm = 99;
    if ( mm==75 ) mm = 95;
    if ( mm==50 ) mm = 75;
    }

cout << " ---- fluence perturbed mean aggregations ----- " << endl;
//   access one chunk's worth of aggregation results of 'perturbed mean'
//       fluence scenarios, and show first/last record
//    --> note that this fluence data is derived from an accumulation,
//         and therefore contain fewer entries of data to be collected
    mm=50;
    while (mm<100) {
    iNum = app.getAggregData ( "fluence", "perturbed", mm, vdDates,
                               vdCoord1, vdCoord2, vdCoord3,
                               vvdPitchAngles, vvvdFluxData );
    if ( iNum >= 0 ) {
      cout << "sizes: vdDates="<<vdDates.size()<<"; vdCoord1="<<vdCoord1.size()
           <<"; flux="<<vvvdFluxData.size()<<",";
      if ( int(vvvdFluxData.size())>0 ) {
        cout << vvvdFluxData[0].size()<<","<<vvvdFluxData[0][0].size()<<endl;
        size_t ii=0;
        while (ii<vvvdFluxData.size() ) {
          int iYear,iDdd;
          double dGmtsec;
          app.getDateTime( vdDates[ii], iYear, iDdd, dGmtsec );
          cout << "["<<ii<<"]: "<<vdDates[ii]<<"("<<iYear<<","<<iDdd<<","<<dGmtsec
               <<") at "<<vdCoord1[ii]<<","<<vdCoord2[ii]<<","<<vdCoord3[ii];
          if ( int(vvdPitchAngles.size())>0 ) {
            cout << "; pitch angles = ";
            for (size_t jj=0; jj<vvdPitchAngles[ii].size(); ++jj)
              cout << vvdPitchAngles[ii][jj] << " ";
          }
          cout << endl;
          for (size_t kk=0; kk<vvvdFluxData[ii][0].size(); ++kk) {
            cout << " pert aggreg "<<mm<<" fluences ["<<kk<<"] = ";
            for (size_t jj=0; jj<vvvdFluxData[ii].size(); ++jj)
              cout << vvvdFluxData[ii][jj][kk]<<" ";
            cout << endl;
          }
          if ( int(vdDates.size())==1 )
            ++ii;
          else
            ii += (vdDates.size()-1);
        }
      }
      else {
        cout << "-,-"<<endl;
        // (disabled for accum data collection)  bEnd = true;

        //  -- since the 'fluence' values are requested here,
        //     fewer 'chunks' are required to collect all of the data.
        //    Therefore, collecting in tandem could cause a premature exit
        //     from this loop before all the non-accumulated data is collected.
      }
    }
    if ( mm==99 ) mm = 100;
    if ( mm==95 ) mm = 99;
    if ( mm==75 ) mm = 95;
    if ( mm==50 ) mm = 75;
    }

cout << " ---- adiabatic coords ----- " << endl;
    vdvector vvdAlpha;
    vdvector vvdLm;
    vdvector vvdK;
    vdvector vvdPhi;
    vdvector vvdHmin;
    vdvector vvdLstar;
    dvector vdBmin;
    dvector vdBlocal;
    dvector vdMagLT;
//   access one chunk's worth of adiabatic coordinate values, and show first/last record
    iNum = app.getAdiabaticCoords ( vdDates, vdCoord1, vdCoord2, vdCoord3,
                                    vvdPitchAngles, vvdAlpha, vvdLm,
                                    vvdK, vvdPhi, vvdHmin, vvdLstar,
                                    vdBmin, vdBlocal, vdMagLT );
    if ( iNum >= 0 ) {
      cout << "sizes: vdDates="<<vdDates.size()<<"; vdCoord1="<<vdCoord1.size()
           <<"; alpha="<<vvdAlpha.size()<<",";
      if ( int(vvdAlpha.size())>0 ) {
        cout << vvdAlpha[0].size()<<endl;
        size_t ii=0;
        while (ii<vvdAlpha.size() ) {
          int iYear,iDdd;
          double dGmtsec;
          app.getDateTime( vdDates[ii], iYear, iDdd, dGmtsec );
          cout << "["<<ii<<"]: "<<vdDates[ii]<<"("<<iYear<<","<<iDdd<<","<<dGmtsec
               <<") at "<<vdCoord1[ii]<<","<<vdCoord2[ii]<<","<<vdCoord3[ii];
          if ( int(vvdPitchAngles.size())>0 ) {
            cout << "; pitch angles = ";
            for (size_t jj=0; jj<vvdPitchAngles[ii].size(); ++jj)
              cout << vvdPitchAngles[ii][jj] << " ";
          }
          cout << endl;
          for (size_t kk=0; kk<vvdAlpha[ii].size(); ++kk) {
            cout << " adiabat values ["<<kk<<"] = ";
            cout << vvdAlpha[ii][kk]<<" ";
            cout << vvdLm[ii][kk]<<" ";
            cout << vvdK[ii][kk]<<" ";
            cout << vvdPhi[ii][kk]<<" ";
            cout << vvdHmin[ii][kk]<<" ";
            cout << vvdLstar[ii][kk]<<" ";
            cout << vdBmin[ii]<<" ";
            cout << vdBlocal[ii]<<" ";
            cout << vdMagLT[ii]<<" ";
            cout << endl;
          }
          if ( int(vvdAlpha.size())==1 )
            ++ii;
          else
            ii += (vvdAlpha.size()-1);
        }
      }
      else {
        cout << "-,-"<<endl;
        cout << "end of data reached"<<endl;
        bEnd = true;
      }
    }

  } // end of while !bEnd

  // data does not have to be all accessed in tandem
  //  -- different results can be collected independently

  // change chunk size of data access
  ierr = app.setChunkSize( 60 );
  // -- a chunk size change causes an implied 'reset' of the data access routines,
  //      so any subsequent 'flyin'/'get' data calls start at the beginning
  //      of the time period

cout << endl << " === second round of data access, collect parameters independently ===" <<endl;
cout << " ** changed chunk size to 60 **" <<endl;

  bEnd = false;

cout << " ---- flux 75th percentile ----- " << endl;
  while ( !bEnd ) {
//   access one chunk's worth of 75th 'percentile' flux results, and show first/last record
//    -->> no results will be returned if the particular type of results
//         were not specified prior to the call to 'runModel'
  	int iNum = app.flyinPercentile( 75, vdDates, vdCoord1, vdCoord2, vdCoord3,
                                    vvdPitchAngles, vvvdFluxData );
    if ( iNum >= 0 ) {
      cout << "sizes: vdDates="<<vdDates.size()<<"; vdCoord1="<<vdCoord1.size()
           <<"; flux="<<vvvdFluxData.size()<<",";
      if ( int(vvvdFluxData.size())>0 ) {
        cout << vvvdFluxData[0].size()<<","<<vvvdFluxData[0][0].size()<<endl;
        size_t ii=0;
        while (ii<vvvdFluxData.size() ) {
          int iYear,iDdd;
          double dGmtsec;
          app.getDateTime( vdDates[ii], iYear, iDdd, dGmtsec );
          cout << "["<<ii<<"]: "<<vdDates[ii]<<"("<<iYear<<","<<iDdd<<","<<dGmtsec
               <<") at "<<vdCoord1[ii]<<","<<vdCoord2[ii]<<","<<vdCoord3[ii];
          if ( int(vvdPitchAngles.size())>0 ) {
            cout << "; pitch angles = ";
            for (size_t jj=0; jj<vvdPitchAngles[ii].size(); ++jj)
              cout << vvdPitchAngles[ii][jj] << " ";
          }
          cout << endl;
          for (size_t kk=0; kk<vvvdFluxData[ii][0].size(); ++kk) {
            cout << " 75perc fluxes ["<<kk<<"] = ";
            for (size_t jj=0; jj<vvvdFluxData[ii].size(); ++jj)
              cout << vvvdFluxData[ii][jj][kk]<<" ";
            cout << endl;
          }
          if ( int(vdDates.size())==1 )
            ++ii;
          else
            ii += (vdDates.size()-1);
        }
      }
      else {
        cout << "-,-"<<endl;
        cout << "end of data reached"<<endl;
        bEnd = true;
      }
    }
    else
      bEnd = true;  // end when error

  } // end of while !bEnd

  // now access the *interval average* of this same data
  //  - note the additional 'true' argument in app.flyinPercentile call
  bEnd = false;

cout << " ---- *interval average* flux 75th percentile ----- " << endl;
  while ( !bEnd ) {
//   access one chunk's worth of 75th 'percentile' flux results, and show first/last record
//    -->> no results will be returned if the particular type of results
//         were not specified prior to the call to 'runModel'
  	int iNum = app.flyinPercentile( 75, vdDates, vdCoord1, vdCoord2, vdCoord3,
                                    vvdPitchAngles, vvvdFluxData, "interval" );
    if ( iNum >= 0 ) {
      cout << "sizes: vdDates="<<vdDates.size()<<"; vdCoord1="<<vdCoord1.size()
           <<"; flux="<<vvvdFluxData.size()<<",";
      if ( int(vvvdFluxData.size())>0 ) {
        cout << vvvdFluxData[0].size()<<","<<vvvdFluxData[0][0].size()<<endl;
        size_t ii=0;
        while (ii<vvvdFluxData.size() ) {
          int iYear,iDdd;
          double dGmtsec;
          app.getDateTime( vdDates[ii], iYear, iDdd, dGmtsec );
          cout << "["<<ii<<"]: "<<vdDates[ii]<<"("<<iYear<<","<<iDdd<<","<<dGmtsec
               <<") at "<<vdCoord1[ii]<<","<<vdCoord2[ii]<<","<<vdCoord3[ii];
          if ( int(vvdPitchAngles.size())>0 ) {
            cout << "; pitch angles = ";
            for (size_t jj=0; jj<vvdPitchAngles[ii].size(); ++jj)
              cout << vvdPitchAngles[ii][jj] << " ";
          }
          cout << endl;
          for (size_t kk=0; kk<vvvdFluxData[ii][0].size(); ++kk) {
            cout << " 75perc fluxes ["<<kk<<"] = ";
            for (size_t jj=0; jj<vvvdFluxData[ii].size(); ++jj)
              cout << vvvdFluxData[ii][jj][kk]<<" ";
            cout << endl;
          }
          if ( int(vdDates.size())==1 )
            ++ii;
          else
            ii += (vdDates.size()-1);
        }
      }
      else {
        cout << "-,-"<<endl;
        cout << "end of data reached"<<endl;
        bEnd = true;
      }
    }
    else
      bEnd = true;  // end when error

  } // end of while !bEnd


  bEnd = false;

// === all model run calculations have been performed, and are currently
//      stored in binary files, in the temporary directory

cout << endl << " === third round of data access, collect repeated parameter after reset ===" <<endl;
  // call to reset the data access
  app.resetModelData();
  // -- an explicit 'reset' of the data access routines, so any subsequent
  //    'flyin'/'get' data calls start at the beginning of the time period

  while ( !bEnd ) {

//   access one chunk's worth of ephemeris, and show first/last record
    std::string strCoordSys = app.getCoordSys( );
    std::string strCoordUnits = app.getCoordSysUnits( );
cout << " ---- ephemeris ----- " << endl;
    iCount = app.getEphemeris ( vdDates, vdCoord1, vdCoord2, vdCoord3 );
    if ( iCount < 0 ) {
      cerr << " app.getEphemeris error: ierr="<<iCount<<endl;
    }
    else {
      cout << "sizes: vdDates="<<vdDates.size()<<"; vdCoord1="<<vdCoord1.size()<<endl;
      if ( iCount>0 ) {
        int ii=0;
        while (ii<iCount) {
          int iYear,iDdd;
          double dGmtsec;
          app.getDateTime( vdDates[ii], iYear, iDdd, dGmtsec );
          cout << "ephemeris ("<<strCoordSys<<"/"<<strCoordUnits<<") ["<<ii<<"]: "
               <<vdDates[ii]<<"("<<iYear<<","<<iDdd<<","<<dGmtsec<<") at "
               <<vdCoord1[ii]<<","<<vdCoord2[ii]<<","<<vdCoord3[ii]<<endl;
          if ( iCount==1 )
            ++ii;
          else
            ii += (iCount-1);
        }
      }
      else {
        cout << "end of data reached"<<endl;
        bEnd = true;
      }
    }
  } // end of while !bEnd

  return 0;
}
