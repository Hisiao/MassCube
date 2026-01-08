/***********************************************************************

 File: demoModel.cpp

 Description:

   Sample program to demonstrate usage of model-level API

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
  
  % DemoModel [path_to_"~/Irene/modelData"]

***********************************************************************/

// include headers for model classes being used
#include "CDateTimeUtil.h"
#include "CEphemModel.h"
#include "Ae9Ap9Model.h"
#include "CAdiabatModel.h"
#include "CAccumModel.h"
#include "CAggregModel.h"
#include "CDoseModel.h"
#include <iomanip>

// -- main program ---
int main (int argc, char* argv[])
{

  int ierr;

//  default model database directory 
  string strDbDir = "../../modelData";

//  check if command-line argument for model database path is present
  if ( argc > 1 ) {
    // print usage info if requested
    if (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h") {
      cerr << "usage: DemoModel [modelData path]\n";
      return 0;
    }
    // copy input string to be new model database directory
    strDbDir = std::string(argv[1]);
    // (will be tested for validity further down)
  }

// instantiate object for time conversion utilities
  DateTimeUtil dateTime;

// instantiate object for ephemeris generation
  EphemModel ephem;

//  define magnetic field database file (relative to executable location)
//  (this also tests specified database path; anything not kosher is spit out)
  ierr = ephem.setMagfieldDBFile( strDbDir+"/igrfDB.h5" );
  if ( ierr != 0 ) {
    cerr << "usage: demoModel [modelData path]\n";
    return ierr;
  }
// alternatively, could just specify the database directory 
//  (but this only verifies it as a directory, not files contained)
//  ierr = ephem.setModelDBDir( strDbDir );
//  if (ierr < 0) {
//    fprintf(stderr,"usage: demoModel [modelData path]\n");
//    return ierr;
//  }

//  define time limits + step size for orbit propagation
//   -note that these limits are _inclusive_, so this will produce *361* entries
  ephem.setTimes( dateTime.getModifiedJulianDate (2014,190,43200.0),
                  dateTime.getModifiedJulianDate (2014,190,64800.0), 60.0 );

//  define propagator to use  (string is NOT case-sensitive)
  ephem.setPropagator( "SatEph" );

//  define orbital characteristics

  // set element time
  // --this demonstrates full range of time conversion utility methods available
  ephem.setElementTime( dateTime.getModifiedJulianDate (2014,
                        dateTime.getDayOfYear(2014,7,9),
                        dateTime.getGmtSeconds(3,25,45.25)) );

  // set orbit 'classical' element values
  ephem.setRightAscension( 123.4 );
  ephem.setArgOfPerigee( 45.0 );
  ephem.setInclination( 35.0 );
  ephem.setAltitudeOfApogee( 2800.0 );
  ephem.setAltitudeOfPerigee( 1800.0 );

// declare vectors to receive the ephemeris information
  dvector vdTimesMJD;
  dvector vdCoord1;
  dvector vdCoord2;
  dvector vdCoord3;
  int iYear, iDdd;
  double dGmtsec;
  int iNumTimes;

  cout << endl << " =================================== " << endl;
  cout << " ephemeris demo1: 'sateph' propagator for GDZ ephemeris" << std::endl;
//  based on all these orbit parameters defined, generate the ephemeris information,
//   also specifying the coordinate system and units of the returned positions
//   --since the 'chunk' size has NOT been specified, it defaults to zero, which means
//     that a single call will return all entries, ie *full* time range of ephemeris
  iNumTimes = ephem.computeEphemeris( "GDZ","km", vdTimesMJD,
                                      vdCoord1, vdCoord2, vdCoord3 );
  if ( iNumTimes>=0 ) {
    cout << "ephem1 sizes: vdTimesMJD="<<vdTimesMJD.size()<<"; vdCoord1="
         <<vdCoord1.size()<<endl;
    if ( iNumTimes>0 ) {
      int ii=0;
// demonstrate a sampling of results; first and last records only
      while ( ii<iNumTimes ) {
        dateTime.getDateTime( vdTimesMJD[ii], iYear, iDdd, dGmtsec );
        cout << "ephemeris (GDZ/km) ["<<ii<<"]: "
             <<fixed<<setprecision(5)<<vdTimesMJD[ii]<<"("<<iYear<<","<<iDdd
             <<","<<setprecision(1)<<dGmtsec<<") at "
             <<setprecision(3)<<vdCoord1[ii]<<","<<vdCoord2[ii]<<","<<vdCoord3[ii]<<endl;
        ii += (iNumTimes-1);
      }
    }
  }
  else {
    cerr << " ephemeris1 error: ierr="<<iNumTimes<<endl;
    return iNumTimes;
  }

//  ***********************************************************************
// demonstrate alternate ephemeris generation, returning position and velocity in
//  GEI coords, and using 'chunks' segmented processing

//  set chunk size, for segmented output of ephemeris
//    When not explicitly defined, the chunk size defaults to 0, which means all is
//    returned at once, as in first section.
//    --This limitation of the amount of data being processed is important for the
//      Ae9Ap9 model processing performance. Due to the numerous internal memory
//      allocations needed for storage of intermediary calculations for the model,
//      the total memory requirements may exceed system resources available if a
//      great number of ephemeris positions are called to be processed at once.
  ephem.setChunkSize( 960 );
//  chunk size of 960 is the recommended level for 'typical' systems;
//    otherwise, 120 for systems with limited memory;
//    systems with more can go higher, but larger than 2400 may degrade performance
  // verify chunk size via 'get'
  int iChunk = ephem.getChunkSize( );

  cout << endl << " =================================== " << endl;
  cout << " ephemeris demo2: 'sateph' propagator for GEI ephemeris, "
       << "converted to GDZ, using chunks of size "<<iChunk << std::endl;

// declare vectors to receive the ephemeris information
  dvector vdXGEI;
  dvector vdYGEI;
  dvector vdZGEI;
  dvector vdXDotGEI;
  dvector vdYDotGEI;
  dvector vdZDotGEI;
//  compute initial set of pos and vel ephemeris in GEI coords
  iNumTimes = ephem.computeEphemeris( vdTimesMJD, vdXGEI, vdYGEI, vdZGEI,
                                 vdXDotGEI, vdYDotGEI, vdZDotGEI );
//  start of ephemeris processing loop
  while ( iNumTimes > 0 ) {
    cout << "ephem2 sizes: vdTimesMJD="<<vdTimesMJD.size()<<"; vdXGEI="<<vdXGEI.size()<<endl;
//    demonstration of coordinate conversion call
    ephem.convertCoordinates( "GEI", "km", vdTimesMJD, vdXGEI, vdYGEI, vdZGEI,
                              "GDZ", "km", vdCoord1, vdCoord2, vdCoord3 );
    int ii=0;
// demonstrate a sampling of results; first and last records only, in both coordinates
    while ( ii<iNumTimes ) {
      dateTime.getDateTime( vdTimesMJD[ii], iYear, iDdd, dGmtsec );
      cout << "ephemeris (GEI/km) ["<<ii<<"]: "
           <<fixed<<setprecision(5)<<vdTimesMJD[ii]<<"("<<iYear<<","<<iDdd<<","
           <<setprecision(1)<<dGmtsec<<") at "
           <<setprecision(6)<<vdXGEI[ii]<<","<<vdYGEI[ii]<<","<<vdZGEI[ii]<<";  "
           <<setprecision(6)<<vdXDotGEI[ii]<<","<<vdYDotGEI[ii]<<","<<vdZDotGEI[ii]<<endl;
      cout << "ephemeris (GDZ/km) ["<<ii<<"]: "
           <<fixed<<setprecision(5)<<vdTimesMJD[ii]<<"("<<iYear<<","<<iDdd<<","
           <<setprecision(1)<<dGmtsec<<") at "
           <<setprecision(3)<<vdCoord1[ii]<<","<<vdCoord2[ii]<<","<<vdCoord3[ii]<<endl;
      ii += (iNumTimes-1);
    }
    // get next chunk of ephemeris information
    iNumTimes = ephem.computeEphemeris( vdTimesMJD, vdXGEI, vdYGEI, vdZGEI,
                                   vdXDotGEI, vdYDotGEI, vdZDotGEI );
  }  // end of ephemeris processing loop
  if ( iNumTimes<0 ) {
    cerr << " ephemeris2 error: ierr="<<iNumTimes<<endl;
    return iNumTimes;
  }

//  ***********************************************************************
// demonstrate basic calls to Ae9Ap9 model, in tandem with ephemeris generation in chunks.

  cout << endl << " =================================== " << endl;
  cout << " ae9ap9 demo1: AP9 omnidirectional mean differential flux " << endl;

//   initialize Ae9Ap9 model class for 'AP9'
//  --->>> note the 'ae9ap9::' namespace qualifier *required* here for the declaration
  ae9ap9::Ae9Ap9Model ap9model;

// new way
  ierr = ap9model.setModel( "AP9" );
  if (ierr < 0) return ierr;
  ierr = ap9model.setModelDBDir( strDbDir );
  if (ierr < 0) return ierr;
//
// old way
  // specify database files for AP9 model, internal coords and magnetic field
//  ap9model.setModelDBFile( strDbDir+"/AP9V15_runtime_tables.mat" );
//  ap9model.setKPhiDBFile( strDbDir+"/fastPhi_net.mat" );
//  ap9model.setKHMinDBFile( strDbDir+"/fast_hmin_net.mat" );
//  ap9model.setMagfieldDBFile( strDbDir+"/igrfDB.h5" );
//

  cout << "before model db load:" << endl;
  cout << " model name = " << ap9model.getModelName()
       <<", species = " << ap9model.getModelSpecies() << endl;

//  define list of proton energy levels
  double adProE[] = { 0.1,0.4,0.8,1,2,5,10,15,20,30,50 };
  dvector vdProEnergies( adProE, adProE + sizeof(adProE) / sizeof(adProE[0]) );

// restart the ephemeris generation using the previously-defined orbit
//  (a restart would automatically be done when any of the orbit definitions is revised,
//   the propagator or generation times changed, and/or a chunk size specified)
  ephem.restartEphemeris();

// get first chunk of ephemeris
  iNumTimes = ephem.computeEphemeris( "GDZ","km", vdTimesMJD,
                                 vdCoord1, vdCoord2, vdCoord3 );

// start of ephemeris generation and model processing loop
  while ( iNumTimes>0 ) {

//    define AP9 flux environment using ephemeris info and proton energy list
//     --for best model performance, the number of ephemeris positions input should be
//       moderated, as explained in the comments for the 'ephem.setChunkSize()' call above
    ierr = ap9model.setFluxEnvironment( "1PtDiff", vdProEnergies, vdProEnergies,
                               vdTimesMJD, "GDZ","km", vdCoord1, vdCoord2, vdCoord3 );
    if ( ierr != 0 ) {
      cerr << " ap9model setfluxEnvironment error: ierr="<<ierr<<endl;
      cerr << ap9model.getErrorText() <<endl;
      return ierr;
    }
    cout << " model name = " << ap9model.getModelName()
         <<", species = " << ap9model.getModelSpecies() << endl;

//    call to calculate AP9 "mean" flux values for the defined flux environment
    vvdvector vvvdFluxDataMean;
    ierr = ap9model.flyinMean( vvvdFluxDataMean );
    if ( ierr != 0 ) {
      cerr << " ap9model flyinMean error: ierr="<<ierr<<endl;
      return ierr;
    }
    cout << "ap9 sizes: vdTimesMJD="<<vdTimesMJD.size()<<"; vdCoord1="<<vdCoord1.size()
         <<"; flux="<<vvvdFluxDataMean.size()<<",";

    if ( int(vvvdFluxDataMean.size())>0 ) {
      cout << vvvdFluxDataMean[0].size()<<","<<vvvdFluxDataMean[0][0].size()<<endl;
      size_t ii=0;
//      demonstrate a sampling of results; first and last records only
      while ( ii<vvvdFluxDataMean.size() ) {
        dateTime.getDateTime( vdTimesMJD[ii], iYear, iDdd, dGmtsec );
//        show time and position
        cout << "time+pos ["<<ii<<"]: "
          <<fixed<<setprecision(5)<<vdTimesMJD[ii]<<"("<<iYear<<","<<iDdd<<","
          <<setprecision(1)<<dGmtsec
          <<") at "<<setprecision(3)<<vdCoord1[ii]<<","<<vdCoord2[ii]<<","<<vdCoord3[ii];
        cout << endl;
//        show associated flux values
        for (size_t kk=0; kk<vvvdFluxDataMean[ii][0].size(); ++kk) {
          cout << " ap9 mean flux ["<<ii<<"][*]["<<kk<<"] = "
               <<scientific<<setprecision(3);
          for (size_t jj=0; jj<vvvdFluxDataMean[ii].size(); ++jj)
            cout << vvvdFluxDataMean[ii][jj][kk]<<" ";
          cout << endl;
        }
        ii += (vdTimesMJD.size()-1);
      }
    }
    else {
      cerr << "-,-"<<endl<<" ap9model flux mean error: ierr="<<ierr<<endl;
      cerr << ap9model.getErrorText() <<endl;
    }
// get next chunk of ephemeris
    iNumTimes = ephem.computeEphemeris( "GDZ","km", vdTimesMJD,
                                   vdCoord1, vdCoord2, vdCoord3 );
  }  // end of ephemeris generation and model processing loop

//  ***********************************************************************
// demonstrate more advanced calls to Ae9Ap9 model, in tandem with ephemeris
//   generation in chunks.

  cout << endl << " =================================== " << endl;
  cout << " ae9ap9 demo2: AE9 unidirectional percentile integral flux " << endl;

//   initialize a *separate* Ae9Ap9 model class for 'AE9'
//    (_cannot_ load different model database into previously initialized object)
//  --->>> note the 'ae9ap9::' namespace qualifier required here for the declaration
  ae9ap9::Ae9Ap9Model ae9model;

// new way
  ierr = ae9model.setModel( "AE9" );
  if (ierr < 0) return ierr;
  ierr = ae9model.setModelDBDir( strDbDir );
  if (ierr < 0) return ierr;
//
// old way
  // specify database files for AE9 model, internal coords and magnetic field
//  ae9model.setModelDBFile( strDbDir+"/AE9V15_runtime_tables.mat" );
//  ae9model.setKPhiDBFile( strDbDir+"/fastPhi_net.mat" );
//  ae9model.setKHMinDBFile( strDbDir+"/fast_hmin_net.mat" );
//  ae9model.setMagfieldDBFile( strDbDir+"/igrfDB.h5" );
//

// demonstration showing *manual* loading of defined model database
//  --this is done automatically on first call to setFluxEnvironment
  ierr = ae9model.loadModelDB();
  if ( ierr != 0 ) {
    cerr << " ae9model loadModelDB error: ierr="<<ierr<<endl;
    cerr << ae9model.getErrorText() <<endl;
    return ierr;
  }
  cout << " model name = " << ae9model.getModelName()
       <<", species = " << ae9model.getModelSpecies() << endl;

//  define list of electron energy levels
  double adEleE[] = { 0.04,0.1,0.2,0.4,0.7,1.0,1.5,2.0,3.5,5.0 };
  dvector vdEleEnergies( adEleE, adEleE + sizeof(adEleE) / sizeof(adEleE[0]) );

//  define a list of pitch angles
  dvector vdPitchAngles;
  vdPitchAngles.push_back( 90.0 );
  vdPitchAngles.push_back( 85.0 );
  vdPitchAngles.push_back( 80.0 );
  vdPitchAngles.push_back( 75.0 );
  vdPitchAngles.push_back( 70.0 );

// reset the orbit definitions used for the ephemeris generation
//  (but this keeps the previous propagator choice and time limits)
  ephem.resetOrbitParameters();
//  -- alternatively, a new ephemeris object could have been instantiated instead

  // set element time
  // --this demonstrates full range of time conversion utility methods available
  ephem.setElementTime( dateTime.getModifiedJulianDate (2014,
                                                 dateTime.getDayOfYear(2014,7,9),
                                                 dateTime.getGmtSeconds(12,0,0.0)) );
// set orbit 'mean' element values
  ephem.setRightAscension( 65.45 );
  ephem.setArgOfPerigee( 129.4 );
  ephem.setInclination( 28.5 );
  ephem.setEccentricity( 0.255 );
  ephem.setMeanMotion( 6.53 );
  ephem.setMeanAnomaly( 305.65 );

// get first chunk of ephemeris
  iNumTimes = ephem.computeEphemeris( "GDZ","km", vdTimesMJD,
                                 vdCoord1, vdCoord2, vdCoord3 );

// start of ephemeris generation and model processing loop
  while ( iNumTimes>0 ) {

//    define AE9 flux environment using ephemeris info, electron energy list,
//      and pitch angle list
    ierr = ae9model.setFluxEnvironment( "Integral", vdEleEnergies, vdEleEnergies,
                vdTimesMJD, "GDZ","km", vdCoord1, vdCoord2, vdCoord3, vdPitchAngles );
    if ( ierr != 0 ) {
      cerr << " ae9model setfluxEnvironment error: ierr="<<ierr<<endl;
      cerr << ae9model.getErrorText() <<endl;
      return ierr;
    }

//    call to calculate three sets of AE9 "percentile" flux values,
//      for the defined flux environment
    vvdvector vvvdFluxDataPAperc50;
    vvdvector vvvdFluxDataPAperc75;
    vvdvector vvvdFluxDataPAperc95;
    ierr = ae9model.flyinPercentile( 50, vvvdFluxDataPAperc50 );
    if ( ierr != 0 ) cerr << " ae9model flux percentile 50 error: ierr="<<ierr<<endl;
    ierr = ae9model.flyinPercentile( 75, vvvdFluxDataPAperc75 );
    if ( ierr != 0 ) cerr << " ae9model flux percentile 75 error: ierr="<<ierr<<endl;
    ierr = ae9model.flyinPercentile( 95, vvvdFluxDataPAperc95 );
    if ( ierr != 0 ) cerr << " ae9model flux percentile 95 error: ierr="<<ierr<<endl;
    cout << "Ax9 sizes: vdTimesMJD="<<vdTimesMJD.size()<<"; vdCoord1="<<vdCoord1.size()
         <<"; flux="<<vvvdFluxDataPAperc95.size()<<",";

    if ( int(vvvdFluxDataPAperc95.size())>0 ) {
      cout << vvvdFluxDataPAperc95[0].size()<<","<<vvvdFluxDataPAperc95[0][0].size()<<endl;
      size_t ii=0;
//      demonstrate a sampling of 95th percentile results; first and last records only
      while ( ii<vvvdFluxDataPAperc95.size() ) {
        dateTime.getDateTime( vdTimesMJD[ii], iYear, iDdd, dGmtsec );
//        show time, position and list of pitch angles
        cout << "time+pos ["<<ii<<"]: "
          <<fixed<<setprecision(5)<<vdTimesMJD[ii]<<"("<<iYear<<","<<iDdd<<","
          <<setprecision(1)<<dGmtsec
          <<") at "<<setprecision(3)<<vdCoord1[ii]<<","<<vdCoord2[ii]<<","<<vdCoord3[ii];
        if ( int(vdPitchAngles.size())>0 ) {
          cout << "; pitch angles = "<<setprecision(1);
          for (size_t jj=0; jj<vdPitchAngles.size(); ++jj)
            cout << vdPitchAngles[jj] << " ";
        }
        cout << endl;
//        show associated flux values for each of the pitch angles
        for (size_t kk=0; kk<vvvdFluxDataPAperc95[ii][0].size(); ++kk) {
          cout << " perc95 flux ["<<ii<<"][*]["<<kk<<"] = "<<scientific<<setprecision(3);
          for (size_t jj=0; jj<vvvdFluxDataPAperc95[ii].size(); ++jj)
            cout << vvvdFluxDataPAperc95[ii][jj][kk]<<" ";
          cout << endl;
        }
        ii += (vdTimesMJD.size()-1);
      }
    }
    else {
      cerr << "-,-"<<endl<<" ae9model flux perc error: ierr="<<ierr<<endl;
      cerr << ae9model.getErrorText() <<endl;
    }
//   get next chunk of ephemeris
    iNumTimes = ephem.computeEphemeris( "GDZ","km", vdTimesMJD,
                                   vdCoord1, vdCoord2, vdCoord3 );
  } // end of ephemeris generation and model processing loop

//  ***********************************************************************
// demonstrate calls to Adiabatic model, in tandem with ephemeris generation in chunks.

  cout << endl << " =================================== " << endl;
  cout << " adiabatic demo: multiple pitch angles " << endl;

// initialize Adiabatic model class
  AdiabatModel adiabat;

// new way
  ierr = adiabat.setModelDBDir( strDbDir );
  if (ierr < 0) return ierr;
//
// old way
//   define coords and mag field database files (relative to the executable location)
//  adiabat.setKPhiDBFile( strDbDir+"/fastPhi_net.mat" );
//  adiabat.setKHMinDBFile( strDbDir+"/fast_hmin_net.mat" );
//  adiabat.setMagfieldDBFile( strDbDir+"/igrfDB.h5" );
//

// restart the ephemeris generation of the previously defined orbit at the start time
  ephem.restartEphemeris();

// get first chunk of ephemeris, but this time in a different coordinate system
  iNumTimes = ephem.computeEphemeris( "RLL","Re", vdTimesMJD,
                                 vdCoord1, vdCoord2, vdCoord3 );

// start of ephemeris generation and model processing loop
  while ( iNumTimes>0 ) {

//     call to calculate adiabatic coordinates based on input ephemeris
//       and pitch angle information
    vdvector vvdAlpha, vvdLm, vvdK, vvdPhi, vvdHmin, vvdLstar, vvdB, vvdI;
    dvector vdBmin, vdBlocal, vdMagLT;
    ierr = adiabat.computeCoordinateSet ( "RLL","Re", vdTimesMJD,
                                           vdCoord1, vdCoord2, vdCoord3,
                                           vdPitchAngles, vvdAlpha, vvdLm,
                                           vvdK, vvdPhi, vvdHmin, vvdLstar,
                                           vdBmin, vdBlocal, vdMagLT, vvdB, vvdI );
    if ( ierr != 0 ) {
      cerr << " adiabatic coordinates error: ierr="<<ierr<<endl;
      return ierr;
    }
    cout << "adiabat sizes: vdTimesMJD="<<vdTimesMJD.size()<<"; vdCoord1="
         <<vdCoord1.size()<<"; alpha="<<vvdAlpha.size()<<",";

    if ( int(vvdAlpha.size())>0 ) {
      cout << vvdAlpha[0].size()<<endl;
      size_t ii=0;
//     demonstrate a sampling of results; first and last records only
      while ( ii<vvdAlpha.size() ) {
        dateTime.getDateTime( vdTimesMJD[ii], iYear, iDdd, dGmtsec );
//        show time, position and list of pitch angles
        cout << "time+pos(RLL/Re) ["<<ii<<"]: "
            <<fixed<<setprecision(5)<<vdTimesMJD[ii]<<"("<<iYear<<","<<iDdd<<","
            <<setprecision(1)<<dGmtsec
            <<") at "<<setprecision(3)<<vdCoord1[ii]<<","<<vdCoord2[ii]<<","<<vdCoord3[ii];
        if ( int(vdPitchAngles.size())>0 ) {
          cout << "; pitch angles = "<<setprecision(1);
          for (size_t jj=0; jj<vdPitchAngles.size(); ++jj)
            cout << vdPitchAngles[jj] << " ";
        }
        cout << endl;
//        show associated adibatic coordinate values for each of the pitch angles
        for (size_t kk=0; kk<vvdAlpha[ii].size(); ++kk) {
          cout << " adiabat values ["<<kk<<"] = ";
          cout <<fixed<<setprecision(2)<< vvdAlpha[ii][kk]<<" ";
          cout <<setprecision(3)<< vvdLm[ii][kk]<<" ";
          cout <<setprecision(5)<< vvdK[ii][kk]<<" ";
          cout <<setprecision(5)<< vvdPhi[ii][kk]<<" ";
          cout <<setprecision(1)<< vvdHmin[ii][kk]<<" ";
          cout <<setprecision(3)<< vvdLstar[ii][kk]<<" ";
          cout <<scientific<<setprecision(3)<< vdBmin[ii]<<" ";
          cout <<scientific<<setprecision(3)<< vdBlocal[ii]<<" ";
          cout <<fixed<<setprecision(2)<< vdMagLT[ii]<<" ";
          cout <<scientific<<setprecision(3)<< vvdB[ii][0]<<" ";
          cout <<scientific<<setprecision(3)<< vvdB[ii][1]<<" ";
          cout <<scientific<<setprecision(3)<< vvdB[ii][2]<<" ";
          cout <<fixed<<setprecision(5)<< vvdI[ii][kk]<<" ";
          cout << endl;
        }
        ii += (vvdAlpha.size()-1);
      }
    }
    else {
      cerr << "-,-"<<endl<<" adiabatic error: ierr="<<ierr<<endl;
    }
//   get next chunk of ephemeris
    iNumTimes = ephem.computeEphemeris( "RLL","Re", vdTimesMJD,
                                   vdCoord1, vdCoord2, vdCoord3 );
  } // end of ephemeris generation and model processing loop

//  ***********************************************************************
// demonstrate additional advanced calls to Ae9Ap9 model, and aggregation of results,
//  in tandem with ephemeris generation in chunks.

  cout << endl << " =================================== " << endl;
  cout << " ae9ap9 demo3: AP9 omnidirectional aggregated perturbed mean differential flux " << endl;

//  (reuse the previously-instantiated AP9 class object)

// initialize Aggregation model class
  AggregModel aggreg;

// define list of confidence levels to be calculated from aggregation
  ivector viConfLevels;
  viConfLevels.push_back( 85 );
  viConfLevels.push_back( 95 );

// restart the ephemeris generation of the previously-defined orbit at the start time
  ephem.restartEphemeris();

// get first chunk of ephemeris
  iNumTimes = ephem.computeEphemeris( "GEO","km", vdTimesMJD,
                                 vdCoord1, vdCoord2, vdCoord3 );

// start of ephemeris generation and model processing loop
  while ( iNumTimes>0 ) {

//    define AP9 flux environment using ephemeris info and proton energy list
    ierr = ap9model.setFluxEnvironment( "1PtDiff", vdProEnergies, vdProEnergies,
                             vdTimesMJD, "GEO","km", vdCoord1, vdCoord2, vdCoord3 );
    if ( ierr != 0 ) {
      cerr << " ap9model setfluxEnvironment error: ierr="<<ierr<<endl;
      cerr << ap9model.getErrorText() <<endl;
      return ierr;
    }

    // initialize aggregation for the current time segment (chunk)
    aggreg.resetAgg();

//    call to calculate 25 scenarios of AP9 "perturbed mean" flux values
//      for the defined flux environment
    for (int jj=1; jj<=25; ++jj ) {

      vvdvector vvvdFluxData;
      ierr = ap9model.flyinPerturbedMean ( jj, vvvdFluxData );
      if ( ierr != 0 ) {
        cerr << " ae9ap9 flux perturbed mean "<<jj
             <<" error: ierr="<<ierr<<endl;
        return ierr;
      }

//      save the model results and associated time values in the aggregation object
      ierr = aggreg.addScenToAgg( vdTimesMJD, vvvdFluxData );
      if ( ierr != 0 ) {
        cerr << " ae9ap9 aggreg of perturbed mean "<<jj+1
             <<" error: ierr="<<ierr<<endl;
        return ierr;
      }

    }

//     calculate specified confidence level values of aggregation, and display
    for ( size_t icl=0; icl<viConfLevels.size(); ++icl ) {

      dvector vdTimeAgg;
      vvdvector vvvdFluxPMAgg;
      ierr = aggreg.computeConfLevel( viConfLevels[icl], vdTimeAgg, vvvdFluxPMAgg );
      if ( ierr != 0 ) {
        cerr << " ae9ap9 aggregation computeConfLevel error: ierr="<<ierr<<endl;
        return ierr;
      }
      cout << viConfLevels[icl] << "% Confidence level results from aggregation "
           << "of 25 scenarios of perturbed mean fluxes" <<endl;
      cout << "agg sizes: vdTimeAgg="<<vdTimeAgg.size()<<"; vdCoord1="<<vdCoord1.size()
           <<"; flux="<<vvvdFluxPMAgg.size()<<",";

      if ( int(vvvdFluxPMAgg.size())>0 ) {
        cout << vvvdFluxPMAgg[0].size()<<","<<vvvdFluxPMAgg[0][0].size()<<endl;
        size_t ii=0;
//       demonstrate a sampling of results; first and last records only
        while ( ii<vvvdFluxPMAgg.size() ) {
          dateTime.getDateTime( vdTimeAgg[ii], iYear, iDdd, dGmtsec );
//          show time and position
          cout << "time+pos(GEO/km) ["<<ii<<"]: "
            <<fixed<<setprecision(5)<<vdTimeAgg[ii]<<"("<<iYear<<","<<iDdd<<","
            <<setprecision(1)<<dGmtsec
            <<") at "<<setprecision(4)<<vdCoord1[ii]<<","<<vdCoord2[ii]<<","<<vdCoord3[ii];
          cout << endl;
//          show the perturbed mean flux aggregation confidence level results
//             (for single pitch angle=omnidirectional)
          for (size_t kk=0; kk<vvvdFluxPMAgg[ii][0].size(); ++kk) {
            cout << " ap9 "<< viConfLevels[icl] << "% CL PM flux ["<<ii<<"][*]["<<kk<<"] = "
                 <<scientific<<setprecision(3);
            for (size_t jj=0; jj<vvvdFluxPMAgg[ii].size(); ++jj)
              cout << vvvdFluxPMAgg[ii][jj][kk]<<" ";
            cout << endl;
          }
          ii += (vdTimeAgg.size()-1);
        }
      }
      else {
        cerr << "-,-"<<endl<<" ap9model flux perturbed mean error: ierr="<<ierr<<endl;
        cerr << ap9model.getErrorText() <<endl;
      }
    } // end of loop over confidence levels
// get next chunk of ephemeris
    iNumTimes = ephem.computeEphemeris( "GEO","km", vdTimesMJD,
                                   vdCoord1, vdCoord2, vdCoord3 );
  } // end of ephemeris generation and model processing loop

//  ***********************************************************************
//  demonstrate  calls to Ae9Ap9 model, accumulation of the results, and calculation
//   of dose results, in tandem with ephemeris generation in chunks.
  cout << endl << " =================================== " << endl;
  cout << " ae9ap9 demo4: AE9 omnidirectional mean differential flux and fluences" << endl;

//  (reuse the previously-instantiated AE9 class object)

//  initialize multiple Accumulation model class objects
//    (separate AccumModel objects are required for each data type
//     and/or accumulation interval)

  ivector viInterval;
//  set up list of accumulation interval times
//   -this is equal to 15.5 of the ephemeris timesteps
//    (chosen to be NOT an even multiple)
  viInterval.push_back( 930 );
//   -this is equal to 30 ephemeris timesteps
  viInterval.push_back( 1800 );

  vector<AccumModel> voAccum;
  for (size_t nn=0; nn<viInterval.size(); ++nn) {
    AccumModel accum;
    ierr = accum.setTimeIntervalSec( double(viInterval[nn]) );
    if ( ierr != 0 ) {
      cerr << " accum setTimeIntervalSec error: ierr="<<ierr<<endl;
      return ierr;
    }
    voAccum.push_back( accum );
  }

//  declare 3d vector to hold accumulated fluence from intervals
//   (will be used in next section for dose calculation)
  vvdvector vvvdAccumIntvFlue;

// vectors to retain most recent ephemeris
  dvector vdCoord1prev;
  dvector vdCoord2prev;
  dvector vdCoord3prev;
  int iNum;

// restart the ephemeris generation of the previously-defined orbit
  ephem.restartEphemeris();

// get first chunk of ephemeris
  iNumTimes = ephem.computeEphemeris( "GDZ","km", vdTimesMJD,
                                 vdCoord1, vdCoord2, vdCoord3 );

// start of ephemeris generation and model processing loop
  while ( iNumTimes>0 ) {

//    define AE9 flux environment using ephemeris info and electron energy list
//    --Important: Dose calculations *require* 1pt differential _omnidirectional_ flux values
    ierr = ae9model.setFluxEnvironment( "1PtDiff", vdEleEnergies, vdEleEnergies,
                               vdTimesMJD, "GDZ","km", vdCoord1, vdCoord2, vdCoord3 );
    if ( ierr != 0 ) {
      cerr << " ae9model setfluxEnvironment error: ierr="<<ierr<<endl;
      cerr << ae9model.getErrorText() <<endl;
      return ierr;
    }

//    call to calculate AE9 "mean" flux values for the defined flux environment
    vvdvector vvvdFluxDataMean;
    ierr = ae9model.flyinMean( vvvdFluxDataMean );
    if ( ierr != 0 ) {
      cerr << " ae9model flyinMean error: ierr="<<ierr<<endl;
      return ierr;
    }
    cout << "ae9 sizes: vdTimesMJD="<<vdTimesMJD.size()<<"; vdCoord1="<<vdCoord1.size()
         <<"; flux="<<vvvdFluxDataMean.size()<<",";

//    show calculated flux values
    if ( int(vvvdFluxDataMean.size())>0 ) {
      cout << vvvdFluxDataMean[0].size()<<","<<vvvdFluxDataMean[0][0].size()<<endl;
      size_t ii=0;
//      demonstrate a sampling of results; first and last records only
      while ( ii<vvvdFluxDataMean.size() ) {
        dateTime.getDateTime( vdTimesMJD[ii], iYear, iDdd, dGmtsec );
//        show time and position
        cout << "time+pos ["<<ii<<"]: "
          <<fixed<<setprecision(5)<<vdTimesMJD[ii]<<"("<<iYear<<","<<iDdd<<","
          <<setprecision(1)<<dGmtsec
          <<") at "<<setprecision(3)<<vdCoord1[ii]<<","<<vdCoord2[ii]<<","<<vdCoord3[ii];
        cout << endl;
//        show associated flux values
        for (size_t kk=0; kk<vvvdFluxDataMean[ii][0].size(); ++kk) {
          cout << " ae9 mean flux ["<<ii<<"][*]["<<kk<<"] = "<<scientific<<setprecision(3);
          for (size_t jj=0; jj<vvvdFluxDataMean[ii].size(); ++jj)
            cout << vvvdFluxDataMean[ii][jj][kk]<<" ";
          cout << endl;
        }
        ii += (vdTimesMJD.size()-1);
      }

//      loop over accumulation objects
      for ( size_t nn=0; nn<voAccum.size(); ++nn ) {

//        load this set of 'mean' flux values into accumulation object
        ierr = voAccum[nn].loadBuffer( vdTimesMJD, vvvdFluxDataMean );
        if ( ierr != 0 ) {
          cerr << " loadBuffer '"<<viInterval[nn]<<"' error: ierr="<<ierr<<endl;
          return ierr;
        }

//        calculate cumulative fluence values using contents of first accumulation buffer
//         (this would produce identical results for the second buffer)
        dvector vdFluenceTime;
        vvdvector vvvdFluence;
        iNum = voAccum[nn].computeFluence( vdFluenceTime, vvvdFluence );
        // -these are cumulative values reported at each ephemeris timestep
//        demonstrate these cumulative fluence results
        cout << " cumulative fluence ae9 sizes: vdTimesMJD="<<vdTimesMJD.size()
             <<"; vdCoord1="<<vdCoord1.size()<<"; fluence="<<vvvdFluence.size()<<",";

//        show cumulative fluence values
        if ( iNum<=0 ) {
          cout << "-,-"<<endl<<" cumulative fluence: ierr="<<iNum<<endl;
        }
        else {
          cout << vvvdFluence[0].size()<<","<<vvvdFluence[0][0].size()<<endl;
          size_t ii=0;
//          demonstrate a sampling of results; first, middle and last records only
          while ( ii<vvvdFluence.size() ) {
            dateTime.getDateTime( vdTimesMJD[ii], iYear, iDdd, dGmtsec );
//            show time and position
            cout << "time+pos ["<<ii<<"]: "
              <<fixed<<setprecision(5)<<vdTimesMJD[ii]<<"("<<iYear<<","<<iDdd<<","
              <<setprecision(1)<<dGmtsec
              <<") at "<<setprecision(3)<<vdCoord1[ii]<<","<<vdCoord2[ii]<<","<<vdCoord3[ii];
            cout << endl;
//            show associated fluence values
            for (size_t kk=0; kk<vvvdFluence[ii][0].size(); ++kk) {
              cout << " ae9 cumul fluence ["<<ii<<"][*]["<<kk<<"] = "
                <<scientific<<setprecision(3);
              for (size_t jj=0; jj<vvvdFluence[ii].size(); ++jj)
                cout << vvvdFluence[ii][jj][kk]<<" ";
              cout << endl;
            }
            ii += (vdTimesMJD.size()/2)-1;
            if ( ii<vdTimesMJD.size() && ii>vdTimesMJD.size()-5 ) ii=vdTimesMJD.size()-1;
          }
        }

//        calculate (completed) interval fluence values using contents
//          of each accumulation buffer
        dvector vdIntvFlueTime;
        vvdvector vvvdIntvFlue;
        ivector viIntvIndices;

        iNum = voAccum[nn].computeIntvFluence( vdIntvFlueTime, vvvdIntvFlue, viIntvIndices );
        // -these are interval fluence values reported at the specified interval durations
        // -note that data of _partially_ complete intervals are retained, to be included
        //  within an interval that will be completed during the next 'chunk' processing
//        demonstrate these interval fluence results
        cout << " --("<<viInterval[nn]<<"s) interval fluence ae9 sizes: vdIntvFlueTime="<<vdIntvFlueTime.size()
             <<"; vdCoord1="<<vdCoord1.size()<<"; intv fluence="<<vvvdIntvFlue.size()<<",";

//        show fluence values from each interval
       if ( iNum<=0 ) {
          cout << "-,-"<<endl<<" intv fluence: iNum="<<iNum<<endl;
        }
        else {
          cout << vvvdIntvFlue[0].size()<<","<<vvvdIntvFlue[0][0].size()<<endl;
          size_t ii=0;
//          demonstrate *all* of these results
          while ( ii<vvvdIntvFlue.size() ) {
            dateTime.getDateTime( vdIntvFlueTime[ii], iYear, iDdd, dGmtsec );
//            show time and position
            cout << "time+pos ["<<ii<<"]: "
              <<fixed<<setprecision(5)<<vdIntvFlueTime[ii]<<"("<<iYear<<","<<iDdd<<","
              <<setprecision(1)<<dGmtsec
              <<") interval end at approx "<<setprecision(3)<<vdCoord1[viIntvIndices[ii]]
              <<","<<vdCoord2[viIntvIndices[ii]]<<","<<vdCoord3[viIntvIndices[ii]]
              <<" ["<<viIntvIndices[ii]<<"]";
            cout << endl;
//            show associated fluence values
            for (size_t kk=0; kk<vvvdIntvFlue[ii][0].size(); ++kk) {
              cout << " ae9 intv fluence ["<<ii<<"][*]["<<kk<<"] = "
                   <<scientific<<setprecision(3);
              for (size_t jj=0; jj<vvvdIntvFlue[ii].size(); ++jj)
                cout << vvvdIntvFlue[ii][jj][kk]<<" ";
              cout << endl;
            }
            ii += 1;
          }

//          call to accumulate these interval-specific fluence values
          iNum = voAccum[nn].accumIntvFluence( vdIntvFlueTime, vvvdIntvFlue, vvvdAccumIntvFlue );
          if ( iNum<0 ) {
            cerr << " accumulate intv fluence calculation error: ierr="<<iNum<<endl;
          }
          else {
//          demonstrate these interval fluence results
            cout << " --("<<viInterval[nn]<<"s) accum interval fluence ae9 sizes: vdIntvFlueTime="
                 <<vdIntvFlueTime.size()<<"; vdCoord1="<<vdCoord1.size()
                 <<"; accum intv fluence="<<vvvdAccumIntvFlue.size()<<",";

//            show cumulative summation of fluence values over the intervals
            if ( iNum==0 ) {
              cout << "-,-; accumulate intv fluence '"<<viInterval[nn]<<"': iNum="<<iNum<<endl;
            }
            else {
              cout << vvvdAccumIntvFlue[0].size()<<","<<vvvdAccumIntvFlue[0][0].size()<<endl;
              size_t ii=0;
//              demonstrate *all* of these results
              while ( ii<vvvdIntvFlue.size() ) {
                dateTime.getDateTime( vdIntvFlueTime[ii], iYear, iDdd, dGmtsec );
//                show time and position
                cout << "time+pos ["<<ii<<"]: "
                  <<fixed<<setprecision(5)<<vdIntvFlueTime[ii]<<"("<<iYear<<","<<iDdd<<","
                  <<setprecision(1)<<dGmtsec
                  <<") interval end at approx "<<setprecision(3)<<vdCoord1[viIntvIndices[ii]]
                  <<","<<vdCoord2[viIntvIndices[ii]]<<","<<vdCoord3[viIntvIndices[ii]]
                  <<" ["<<viIntvIndices[ii]<<"]";
                cout << endl;
//                show associated fluence values
                for (size_t kk=0; kk<vvvdAccumIntvFlue[ii][0].size(); ++kk) {
                  cout << " ae9 accum intv fluence ["<<ii<<"][*]["<<kk<<"] = "
                       <<scientific<<setprecision(3);
                  for (size_t jj=0; jj<vvvdAccumIntvFlue[ii].size(); ++jj)
                    cout << vvvdAccumIntvFlue[ii][jj][kk]<<" ";
                  cout << endl;
                }
                ii += 1;
              } // end while loop (ii)
            } // end iNum==0 else
          } // end iNum<0 else
        }  // end iNum<=0 else
      }  // end loop over accumulation objects (nn)

    }
    else {
      cerr << "-,-"<<endl<<" ae9model flux mean error: ierr="<<ierr<<endl;
      cerr << ae9model.getErrorText() <<endl;
    }
    // retain ephemeris information for later reference
    vdCoord1prev = vdCoord1;
    vdCoord2prev = vdCoord2;
    vdCoord3prev = vdCoord3;

// get next chunk of ephemeris
    iNumTimes = ephem.computeEphemeris( "GDZ","km", vdTimesMJD,
                                   vdCoord1, vdCoord2, vdCoord3 );
    // when ephemeris generation is already complete,
    //  these returned vectors will be *empty*

  } // end of ephemeris generation and model processing loop

  // collect fluence data from last *partial* accumulation intervals, if any
  //  --note last argument added to method call, with flag to get this data
  dvector vdIntvFlueTime;
  vvdvector vvvdIntvFlue;
  ivector viIntvIndices;

//  loop over accumulation objects
  for ( size_t nn=0; nn<voAccum.size(); ++nn ) {
    iNum = voAccum[nn].computeIntvFluence( vdIntvFlueTime, vvvdIntvFlue,
                                           viIntvIndices, true );
    // -these are interval fluence values reported at the specified interval durations
    // -note that data of _partially_ complete intervals are retained, to be included
    //  within an interval that will be completed during the next 'chunk' processing
//    demonstrate these interval fluence results
    cout << " --("<<viInterval[nn]<<"s) partial interval fluence ae9 sizes: vdIntvFlueTime="<<vdIntvFlueTime.size()
         <<"; vdCoord1="<<vdCoord1prev.size()<<"; fluence="<<vvvdIntvFlue.size()<<",";
//    -returned size will either be 0 (no leftover data) or 1 (partial interval data)

//    show fluence values from last (partial) interval, if present
    if ( iNum<=0 ) {
      cerr << "-,-; iNum="<<iNum<<endl;
    }
    else {
      cout << vvvdIntvFlue[0].size()<<","<<vvvdIntvFlue[0][0].size()<<endl;
      size_t ii=0;
      // check indices value for '-1' indicating last record of input
      if ( viIntvIndices[ii]==-1 ) viIntvIndices[ii] = int(vdCoord1prev.size())-1;
//      demonstrate these last results
      dateTime.getDateTime( vdIntvFlueTime[ii], iYear, iDdd, dGmtsec );
//      show time and position
      cout << "time+pos ["<<ii<<"]: "
        <<fixed<<setprecision(5)<<vdIntvFlueTime[ii]<<"("<<iYear<<","<<iDdd<<","
        <<setprecision(1)<<dGmtsec
        <<") interval end at approx "<<setprecision(3)<<vdCoord1prev[viIntvIndices[ii]]
        <<","<<vdCoord2prev[viIntvIndices[ii]]<<","<<vdCoord3prev[viIntvIndices[ii]]<<" ["
        <<viIntvIndices[ii]<<"]";
      cout << endl;
//      show associated fluence values
      for (size_t kk=0; kk<vvvdIntvFlue[ii][0].size(); ++kk) {
        cout << " ae9 partial intv fluence ["<<ii<<"][*]["<<kk<<"] = "
             <<scientific<<setprecision(3);
        for (size_t jj=0; jj<vvvdIntvFlue[ii].size(); ++jj)
          cout << vvvdIntvFlue[ii][jj][kk]<<" ";
        cout << endl;
      }

//      call to accumulate these interval-specific fluence values
      iNum = voAccum[nn].accumIntvFluence( vdIntvFlueTime, vvvdIntvFlue, vvvdAccumIntvFlue );
      if ( iNum<0 ) {
        cerr << " accumulate intv fluence calculation error: ierr="<<iNum<<endl;
      }
      else {
//       demonstrate these interval fluence results
        cout << " --("<<viInterval[nn]<<"s) accum interval fluence ae9 sizes: vdIntvFlueTime="<<vdIntvFlueTime.size()
             <<"; vdCoord1="<<vdCoord1prev.size()
             <<"; accum intv fluence="<<vvvdAccumIntvFlue.size()<<",";

//        show final cumulative summation of fluence values over the intervals
        if ( iNum==0 ) {
          cout << "-,-; accumulate intv fluence '"<<viInterval[nn]<<"': iNum="<<iNum<<endl;
        }
        else {
          cout << vvvdAccumIntvFlue[0].size()<<","<<vvvdAccumIntvFlue[0][0].size()<<endl;
          size_t ii=0;
//          demonstrate the results
          dateTime.getDateTime( vdIntvFlueTime[ii], iYear, iDdd, dGmtsec );
//          show time and position
          cout << "time+pos ["<<ii<<"]: "
            <<fixed<<setprecision(5)<<vdIntvFlueTime[ii]<<"("<<iYear<<","<<iDdd<<","
            <<setprecision(1)<<dGmtsec
            <<") interval end at approx "<<setprecision(3)<<vdCoord1prev[viIntvIndices[ii]]
            <<","<<vdCoord2prev[viIntvIndices[ii]]<<","<<vdCoord3prev[viIntvIndices[ii]]
            <<" ["<<viIntvIndices[ii]<<"]";
          cout << endl;
//          show associated fluence values
          for (size_t kk=0; kk<vvvdAccumIntvFlue[ii][0].size(); ++kk) {
            cout << " ae9 accum intv fluence ["<<ii<<"][*]["<<kk<<"] = "
                 <<scientific<<setprecision(3);
            for (size_t jj=0; jj<vvvdAccumIntvFlue[ii].size(); ++jj)
              cout << vvvdAccumIntvFlue[ii][jj][kk]<<" ";
            cout << endl;
          }
        }  // end iNum==0 else
      } // end iNum<0 else
    } // end iNum<=0 else
  } // end loop over accumulation objects (nn)


//  ***********************************************************************
  cout << endl << " =================================== " << endl;
  cout << " dose demo: accumulated dose from AE9 omnidirectional mean differential fluences" << endl;
//  initialize Dose model class
  DoseModel dose;

// new way
  ierr = dose.setModelDBDir( strDbDir );
  if (ierr < 0) return ierr;
//
// old way
//  define shieldDose2 model database file
//  dose.setModelDBFile( strDbDir+"/sd2DB.h5" );
//

//  define flux particle species and energies
  dose.setSpecies( ae9model.getModelSpecies() );
  dose.setEnergies( vdEleEnergies );

//  define shielding depth values/units/geometry and detector type
  double adDepths[] = { 0.1, 0.2, 0.4, 0.5, 0.75, 1.0, 1.5, 2.0 };
  dvector vdDepths( adDepths, adDepths + sizeof(adDepths) / sizeof(adDepths[0]) );
  dose.setDepths( vdDepths, "mm" );
  dose.setDetector( "Si" );
  dose.setGeometry( "semiinfslab" );

// using omnidirectional electron accumulated fluence from previous section

//  IMPORTANT:  Flux and Fluence inputs to the DoseModel *must* be:
//               1pt Differential Flux type
//               Omnidirectional only
  vvdvector vvvdDoseAccum;
  ierr = dose.computeFluenceDose( vvvdAccumIntvFlue, vvvdDoseAccum );
  if ( ierr < 0 ) {
    cerr << " dose computeFluenceDose error: ierr="<<ierr<<endl;
    return ierr;
  }
  else {
//    report only the very last results
    cout << "ae9 accumulated dose over full time period at depths: ";
    cout << fixed << setprecision(2);
    for (size_t jj=0; jj<vdDepths.size(); ++jj )
      cout << vdDepths[jj] << " ";
    cout << "mm" << endl;
    size_t ii=vvvdDoseAccum.size()-1;
    cout << " ae9 accum dose ["<<ii<<"][*][0] = "
         <<scientific<<setprecision(3);
    for (size_t jj=0; jj<vvvdDoseAccum[ii].size(); ++jj)
      cout << vvvdDoseAccum[ii][jj][0] << " ";
    cout <<endl;
  }

//  ***********************************************************************

  return 0;
}
