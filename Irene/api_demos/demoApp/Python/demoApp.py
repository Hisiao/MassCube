
# enable use of python3 print function form in python 2.x
from __future__ import print_function

import os
import sys
import random
import numpy as np

# for the 'import irene_defs' to be successful, either:
#     specify the path to the Irene root directory here:
#sys.path.append(os.environ['HOME']+'/Irene')
#     or in the PYTHONPATH environement variable
#
# load definitions for required IRENE environment variables
import irene_defs

# the application import operation depends on those IRENE definitions
from application import Application

def runDemoApp(execdir, dbdir, workdir, tempdir, numproc, fulldata):
    np.set_printoptions(precision=2)

##############################################################
#### Direct MPI operation has been temporarily disabled  #####
#### -an issue exists that prevents proper MPI-based
####  spawning of the various C++ task applications
####  when Python loads the C++-based mpi-enabled shared 
####  object library files
#    app = Application( numproc!=1 )
#
#### Using a workaround that performs the MPI operations
####  wholly within C++ code, and will not affect the
####  generation of results or access to by python API
    app = Application( False )
####

    ierr = app.set_execDir(execdir)
    if ierr != 0:
        sys.exit('error in set_execDir')

    # define model, associated model database, coords db and magfield db
    ierr = app.set_model("AP9")
    if ierr != 0:
        sys.exit('error in set_model')
## new way
    ierr = app.set_modelDBDir(dbdir)
    if ierr != 0:
        sys.exit('error in set_modelDBDir')
## old way
##    ierr = app.set_modelDBFile(dbdir + "/AP9V15_runtime_tables.mat")
##    if ierr != 0:
##        sys.exit('error in set_modelDBFile')
##    ierr = app.set_kPhiDBFile(dbdir + "/fastPhi_net.mat")
##    if ierr != 0:
##        sys.exit('error in set_kPhiDBFile')
##    ierr = app.set_kHMinDBFile(dbdir + "/fast_hmin_net.mat")
##    if ierr != 0:
##        sys.exit('error in set_kHMinDBFile')
##    ierr = app.set_magfieldDBFile(dbdir + "/igrfDB.h5")
##    if ierr != 0:
##        sys.exit('error in set_magfieldDBFile')

    ierr = app.set_workDir(workdir)
    if ierr != 0:
        sys.exit('error in set_workDir')

    if tempdir != "":
        ierr = app.set_binDirName(tempdir)
        if ierr != 0:
            sys.exit('error in set_binDirName')

    #  define ephemeris generation limits and time step
    #   -note that these limits are _inclusive_, so this will produce *361* entries
    app.set_times(app.get_modJulDate(2014,190,43200.0),
                  app.get_modJulDate(2014,190,64800.0),
                  60.0)

    #  define propagator to use
    app.set_prop("SatEph")

    #  define orbital characteristics
    # demonstrate full range of time conversion utility methods available
    app.set_elementTime(app.get_modJulDate(2014,
                                           app.get_dayYear(2014,7,9),
                                           app.get_gmtsec(3,25,45.25)))
    app.set_rightAscension(123.4)
    app.set_argOfPerigee(45.0)
    app.set_inclination(35.0)
    app.set_altitudeOfApogee(2800.0)
    app.set_altitudeOfPerigee(1800.0)

    # define coord system and units of ephemeris
    app.set_coordSys("GDZ","KM")

    #  define flux type and energy levels
    app.set_fluxType("1PTDIFF")
    fluxEnergies = np.array([0.1,0.4,0.8,1,2,5,10,15,20,30,50])
    iNumE = fluxEnergies.shape[0]
    ierr = app.set_fluxEnergies(fluxEnergies)
    if ierr != 0:
        sys.exit('error in set_fluxEnergies')

    #  define flux mode calculations to perform
    app.set_fluxMean()
    app.set_fluxPerturbedScenRange(1, 15)
    #  app.setFluxPercentile(50)
    app.set_fluxPercentile(75)
    #  app.setFluxPercentile(95)
    #  app.setPitchAngle(90)
    #  app.setPitchAngle(85)
    #  app.setPitchAngle(80)
    #  app.setPitchAngle(75)

    #  define calculation of fluence results and adiabatic coords
    app.set_fluence()
    app.set_accumMode("Interval")
    app.set_accumIntervalSec( 600.0 )
    app.set_adiabatic()

    #  uncomment this to use multiple pitch angles
    #daPitchAngles = np.array([5.0, 95.0, 150.0, 180.0])
    #app.set_pitchAngles(daPitchAngles)

    #  define aggregation results for perturbed mean flux scenario values
    app.set_aggregConfLevels(np.array([50, 75, 95]))
    #  double adDepths[] = { 0.1, 0.2, 0.4, 0.5 }
    #  dvector vdDepths
    #  for (int ii=0 ii<4 ++ii) vdDepths.push_back(adDepths[ii])
    #  app.setDoseDepths( vdDepths, "mm" )
    #  app.setDoseRate()

    #  define number of processors to be used (includes one for 'control' node)
    ierr = app.set_numProc(numproc)
    if ierr != 0:
        sys.exit('error in set_numProc')

    #  specify if temporary binary files are to be deleted when 'app' class
    #    object is destroyed, or 'resetmodel' is called
    app.set_delBinDir(0)

    #  ====actual execution of all processing======
    #   --this spawns off all necessary applications
    #     to perform the requested model calculations
    ierr = app.run_model()
    print(ierr)
    if ierr != 0:
        print("Error in model run - availability of results is uncertain")
        sys.exit('error in run_model')

    # === all model run calculations have been performed, and are currently
    #      stored in binary files, in the temporary directory

    print(" === first round of data access, collect multiple parameters in tandem ===")
    print(" default chunk size of 960 is used, unless otherwise specified")
    # loop on *chunks* until end is detected (size of returned data vector is zero)

    # ** each of these 'getEphem/Data/etc,flyin' calls are reading one 'chunk's worth
    #    of results multiple calls to the same routine are necessary to collect _all_
    #    the data the app.set_chunkSize(<n>) may be used to adjust the chunk size.
    # --note that there will be fewer data records overall from accumulation-derived
    #    data calculations (such as interval fluences or doses)

    #    -->> when a particular type of results being requested by these calls
    #         were not specified in the settings prior to the call to 'runModel',
    #         an error message is shown, and no results will be returned.

    bEnd = False

    strSys = app.get_coordSys()
    strUnits = app.get_coordSysUnits()

    while not bEnd:

        print(" ---- ephemeris ---- ")
        # access one chunk's worth of ephemeris
        iNum, daTimes, daCoord1, daCoord2, daCoord3 = app.get_ephemeris()
        print('get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))
        if iNum > 0:
            i = 0
            # if fulldata is specified, all records will be printed out
            # otherwise, just the first and last record
            while i < iNum:
                iYear, iDdd, dGmtsec = app.get_dateTime(daTimes[i])
                print('ephemeris ({}/{}) [{}]: {:.5f}({},{},{:.1f}) at {:.2f},{:.2f},{:.2f}'.format(
                      strSys, strUnits, i, daTimes[i], iYear, iDdd, dGmtsec, daCoord1[i], daCoord2[i], daCoord3[i]))
                i = i+iNum-1 if not fulldata else i+1 # if not fulldata, skip to last record
        elif iNum == 0:
            print("end of data reached")
            bEnd = True

        print(" ---- flux mean ---- ")
        # access one chunk's worth of 'mean' flux results
        #  -->> no results will be returned if the particular type of results
        #       were not specified prior to the call to 'runModel'
        iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3FluxData \
 = app.flyin_meanPlus()
        print(' flyin_meanPlus: => iNum=', iNum, ', len(daTimes)=', len(daTimes),
              ', len(daCoord1)=', len(daCoord1), ', da2PitchAngles.shape=', da2PitchAngles.shape,
              ', da3FluxData.shape=', da3FluxData.shape)
        if iNum > 0:
            i = 0
            # if fulldata is specified, all records will be printed out
            # otherwise, just the first and last record
            while i < iNum:
                iYear, iDdd, dGmtsec = app.get_dateTime(daTimes[i])
                print('[{}]: {:.5f}({},{},{:.1f}) at {:.2f},{:.2f},{:.2f}'.format(
                      i, daTimes[i], iYear, iDdd, dGmtsec, daCoord1[i], daCoord2[i], daCoord3[i]))
                # print values for each pitch angle
                for j in range(0, da3FluxData.shape[2]): # number of pitch angles
                    # using string.join and map functions just to make the printed line prettier
                    print(' mean fluxes [{}] ='.format(j), ', '.join(map(lambda x: "{:.3e}".format(x), da3FluxData[i,:,j])))
                i = i+iNum-1 if not fulldata else i+1 # if not fulldata, skip to last record
        elif iNum == 0:
            print("end of data reached")
            bEnd = True

        print(" ---- flux 95th percentile ---- ")
        # access one chunk's worth of 95th 'percentile' flux results
        iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3FluxData \
 = app.flyin_percentilePlus(95)
        print(' flyin_percentilePlus(95): => iNum=', iNum, ', len(daTimes)=', len(daTimes),
              ', len(daCoord1)=', len(daCoord1), ', da2PitchAngles.shape=', da2PitchAngles.shape,
              ', da3FluxData.shape=', da3FluxData.shape)
        if iNum > 0:
            i = 0
            # if fulldata is specified, all records will be printed out
            # otherwise, just the first and last record
            while i < iNum:
                iYear, iDdd, dGmtsec = app.get_dateTime(daTimes[i])
                print('[{}]: {:.5f}({},{},{:.1f}) at {:.2f},{:.2f},{:.2f}'.format(
                      i, daTimes[i], iYear, iDdd, dGmtsec, daCoord1[i], daCoord2[i], daCoord3[i]))
                # print values for each pitch angle
                for j in range(0, da3FluxData.shape[2]): # number of pitch angles
                    # using string.join and map functions just to make the printed line prettier
                    print(' 95perc [{}] ='.format(j), ', '.join(map(lambda x: "{:.3e}".format(x), da3FluxData[i,:,j])))
                i = i+iNum-1 if not fulldata else i+1 # if not fulldata, skip to last record
        elif iNum == 0:
            print("end of data reached")
            bEnd = True

        print(" ---- flux perturbed mean scenarios [Avg] ---- ")
        # Access one chunk's worth of 'perturbed mean' flux scenario results
        for scen in range(1, 20, 8):
            iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3FluxData \
 = app.flyin_perturbedMeanPlus(scen, "interval")
            print(' flyin_perturbedMeanPlus({}, Avg=1): => iNum='.format(scen), iNum, ', len(daTimes)=', len(daTimes),
                  ', len(daCoord1)=', len(daCoord1), ', da2PitchAngles.shape=', da2PitchAngles.shape,
                  ', da3FluxData.shape=', da3FluxData.shape)
            if iNum > 0:
                i = 0
                # if fulldata is specified, all records will be printed out
                # otherwise, just the first and last record
                while i < iNum:
                    iYear, iDdd, dGmtsec = app.get_dateTime(daTimes[i])
                    print('[{}]: {:.5f}({},{},{:.1f}) at {:.2f},{:.2f},{:.2f}'.format(
                          i, daTimes[i], iYear, iDdd, dGmtsec, daCoord1[i], daCoord2[i], daCoord3[i]))
                    # print values for each pitch angle
                    for j in range(0, da3FluxData.shape[2]): # number of pitch angles
                        # using string.join and map functions just to make the printed line prettier
                        print(' pert scen', scen, 'fluxes [{}] ='.format(j), ', '.join(
                              map(lambda x: "{:.3e}".format(x), da3FluxData[i,:,j])))
                    i = i+iNum-1 if not fulldata else i+1 # if not fulldata, skip to last record
            elif iNum == 0:
                print("-,-")

        print(" ---- flux perturbed mean aggregations ---- ")
        # access one chunk's worth of aggregation results of 'perturbed mean' flux scenarios
        for scen in [50, 75, 95, 99, 100]:
            iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3FluxData \
 = app.get_aggregData('flux', 'perturbed', scen)
            print(' flyin_percentilePlus(\'flux\', \'perturbed\', {}): => iNum='.format(scen), iNum,
                    ', len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1),
                    ', da2PitchAngles.shape=', da2PitchAngles.shape,
                    ', da3FluxData.shape=', da3FluxData.shape)
            if iNum > 0:
                i = 0
                # if fulldata is specified, all records will be printed out
                # otherwise, just the first and last record
                while i < iNum:
                    if iNum > 0:
                        iYear, iDdd, dGmtsec = app.get_dateTime(daTimes[i])
                        print('[{}]: {:.5f}({},{},{:.1f}) at {:.2f},{:.2f},{:.2f}'.format(
                              i, daTimes[i], iYear, iDdd, dGmtsec, daCoord1[i], daCoord2[i], daCoord3[i]))
                        # print values for each pitch angle
                        for j in range(0, da3FluxData.shape[2]): # number of pitch angles
                            # using string.join and map functions just to make the printed line prettier
                            print(' pert aggreg', scen, 'fluxes [{}] ='.format(j), ', '.join(
                                  map(lambda x: "{:.3e}".format(x), da3FluxData[i,:,j])))
                        i = i+iNum-1 if not fulldata else i+1 # if not fulldata, skip to last record
            elif iNum == 0:
                print("end of data reached")
                bEnd = True

        print(" ---- fluence perturbed mean aggregations ---- ")
        # access one chunk's worth of aggregation results of 'perturbed mean' flux scenarios
        for scen in [50, 75, 95, 99, 100]:
            iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3FluxData \
 = app.get_aggregData('fluence', 'perturbed', scen)
            print(' flyin_percentilePlus(\'fluence\', \'perturbed\', {}): => iNum='.format(scen), iNum,
                    ', len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1),
                    ', da2PitchAngles.shape=', da2PitchAngles.shape,
                    ', da3FluxData.shape=', da3FluxData.shape)
            if iNum > 0:
                i = 0
                # if fulldata is specified, all records will be printed out
                # otherwise, just the first and last record
                while i < iNum:
                    if iNum > 0:
                        iYear, iDdd, dGmtsec = app.get_dateTime(daTimes[i])
                        print('[{}]: {:.5f}({},{},{:.1f}) at {:.2f},{:.2f},{:.2f}'.format(
                              i, daTimes[i], iYear, iDdd, dGmtsec, daCoord1[i], daCoord2[i], daCoord3[i]))
                        # print values for each pitch angle
                        for j in range(0, da3FluxData.shape[2]): # number of pitch angles
                            # using string.join and map functions just to make the printed line prettier
                            print(' pert aggreg', scen, 'fluences [{}] ='.format(j), ', '.join(
                                  map(lambda x: "{:.3e}".format(x), da3FluxData[i,:,j])))
                        i = i+iNum-1 if not fulldata else i+1 # if not fulldata, skip to last record
            elif iNum == 0:
                print("end of data reached")
                bEnd = True

        print(" ---- adiabatic coords ---- ")
        # access one chunk's worth of adiabatic coordinate values
        iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, \
            da2Alpha, da2Lm, da2K, da2Phi, da2Hmin, da2Lstar, \
            daBmin, daBlocal, daMagLT \
    = app.get_adiabaticCoordsPlus()
        print(' get_adiabaticCoordsPlus: => iNum=', iNum,
              ', len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1),
              ', da2PitchAngles.shape=', da2PitchAngles.shape, ', da2Alpha.shape=', da2Alpha.shape,
              ', da2Lm.shape=', da2Lm.shape, ', da2K.shape=', da2K.shape,
              ', da2Phi.shape=', da2Phi.shape, ', da2Hmin.shape=', da2Hmin.shape,
              ', da2Lstar.shape=', da2Lstar.shape, ', daBmin.shape=', daBmin.shape,
              ', daBlocal.shape=', daBlocal.shape, ', daMagLT.shape=', daMagLT.shape)
        if iNum > 0:
            i = 0
            # if fulldata is specified, all records will be printed out
            # otherwise, just the first and last record
            while i < iNum:
                iYear, iDdd, dGmtsec = app.get_dateTime(daTimes[i])
                print('[{}]: {:.5f}({},{},{:.1f}) at {:.2f},{:.2f},{:.2f}'.format(
                    i, daTimes[i], iYear, iDdd, dGmtsec, daCoord1[i], daCoord2[i], daCoord3[i]))
                # print values for each pitch angle
                for j in range(0, da3FluxData.shape[2]): # number of pitch angles
                    print(' adiabat values [{}] = {:.2f} {:.2f} {:.2f} {:.2f} {:.2f} {:.2f} {:.2f} {:.2f} {:.2f}'.format(
                          i, da2Alpha[i][j], da2Lm[i][j], da2K[i][j], da2Phi[i][j],
                          da2Hmin[i][j], da2Lstar[i][j], daBmin[i], daBlocal[i], daMagLT[i]))
                i = i+iNum-1 if not fulldata else i+1 # if not fulldata, skip to last record
        elif iNum == 0:
            print("-,-")

    # end of while !bEnd

    # data does not have to be all accessed in tandem
    #  -- different results can be collected independently

    # change chunk size of data access
    app.set_chunkSize(65)
    # -- a chunk size change causes an implied 'reset' of the data access routines,
    #      so any subsequent 'flyin'/'get' data calls start at the beginning
    #      of the time period

    print("\n === second round of data access, collect parameters independently ===")
    print(" ** changed chunk size to 65 **")

    bEnd = False

    print(" ---- flux 75th percentile ---- ")
    while not bEnd:

        # access one chunk's worth of 75th 'percentile' flux results
        iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3FluxData \
 = app.flyin_percentilePlus(75)
        print(' flyin_percentilePlus(75): => iNum=', iNum, ', len(daTimes)=', len(daTimes),
              ', len(daCoord1)=', len(daCoord1), ', da2PitchAngles.shape=', da2PitchAngles.shape,
              ', da3FluxData.shape=', da3FluxData.shape)
        if iNum > 0:
            i = 0
            # if fulldata is specified, all records will be printed out
            # otherwise, just the first and last record
            while i < iNum:
                iYear, iDdd, dGmtsec = app.get_dateTime(daTimes[i])
                print('[{}]: {:.5f}({},{},{:.1f}) at {:.2f},{:.2f},{:.2f}'.format(
                      i, daTimes[i], iYear, iDdd, dGmtsec, daCoord1[i], daCoord2[i], daCoord3[i]))
                # print values for each pitch angle
                for j in range(0, da3FluxData.shape[2]): # number of pitch angles
                    # using string.join and map functions just to make the printed line prettier
                    print(' 75perc [{}] ='.format(j), ', '.join(map(lambda x: "{:.3e}".format(x), da3FluxData[i,:,j])))
                i = i+iNum-1 if not fulldata else i+1 # if not fulldata, skip to last record
        elif iNum == 0:
            print("end of data reached")
            bEnd = True

    bEnd = False

    # now access the *interval average* of this same data
    # - note the additional 'true' argument in app.flyinPercentile call
    print(" ---- *interval average* flux 75th percentile ---- ")
    while not bEnd:

        # access one chunk's worth of 75th 'percentile' flux results
        # -->> no results will be returned if the particular type of results
        #      were not specified prior to the call to 'runModel'
        iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3FluxData \
 = app.flyin_percentilePlus(75, "interval")
        print(' flyin_percentilePlus(75, "interval"): => iNum=', iNum, ', len(daTimes)=', len(daTimes),
              ', len(daCoord1)=', len(daCoord1), ', da2PitchAngles.shape=', da2PitchAngles.shape,
              ', da3FluxData.shape=', da3FluxData.shape)
        if iNum > 0:
            i = 0
            # if fulldata is specified, all records will be printed out
            # otherwise, just the first and last record
            while i < iNum:
                iYear, iDdd, dGmtsec = app.get_dateTime(daTimes[i])
                print('[{}]: {:.5f}({},{},{:.1f}) at {:.2f},{:.2f},{:.2f}'.format(
                      i, daTimes[i], iYear, iDdd, dGmtsec, daCoord1[i], daCoord2[i], daCoord3[i]))
                # print values for each pitch angle
                for j in range(0, da3FluxData.shape[2]): # number of pitch angles
                    # using string.join and map functions just to make the printed line prettier
                    print(' 75perc [{}] ='.format(j), ', '.join(map(lambda x: "{:.3e}".format(x), da3FluxData[i,:,j])))
                i = i+iNum-1 if not fulldata else i+1 # if not fulldata, skip to last record
        elif iNum == 0:
            print("end of data reached")
            bEnd = True

    # === all model run calculations have been performed, and are currently
    #      stored in binary files, in the temporary directory

    print("\n === third round of data access, collect repeated parameter after reset ===")
    print(" ---- Resetting model run ------ ")
    # reset data back to beginning
    app.reset_modelData()
    # -- an explicit 'reset' of the data access routines, so any subsequent
    #    'flyin'/'get' data calls start at the beginning of the time period

    print(" ---- ephemeris ---- ")

    strSys = app.get_coordSys()
    strUnits = app.get_coordSysUnits()

    bEnd = False
    
    while not bEnd:
        # access one chunk's worth of ephemeris, and show first/last record
        iNum, daTimes, daCoord1, daCoord2, daCoord3 = app.get_ephemeris()
        print('get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))
        if iNum > 0:
            i = 0
            # if fulldata is specified, all records will be printed out
            # otherwise, just the first and last record
            while i < iNum:
                iYear, iDdd, dGmtsec = app.get_dateTime(daTimes[i])
                print('ephemeris ({}/{}) [{}]: {:.5f}({},{},{:.1f}) at {:.2f},{:.2f},{:.2f}'.format(
                      strSys, strUnits, i, daTimes[i], iYear, iDdd, dGmtsec, daCoord1[i], daCoord2[i], daCoord3[i]))
                i = i+iNum-1 if not fulldata else i+1 # if not fulldata, skip to last record
        elif iNum == 0:
            print("end of data reached")
            bEnd = True

    del app

#=======================================================================================
# Commmand-line parser
#
# Parse arguments
#
#      -x <path>      path to Irene executable files (default=$IRENE_BIN)                      [optional]
#      -d <path>      path to database directory (default=$IRENE_DB)                            [optional]
#      -w <path>      path to 'working' directory for temp files (default=current dir)          [optional]
#      -b <str>       name of directory for temp files (default=generated)                      [optional]
#      -n <int>       number of processors (including master node) default=1, 3 or larger       [optional]
#      --fulldata     report all data entries, instead of just first and last of each chunk     [optional]
#      -h             help

if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(description='DemoApp - Python API')

    irene_bin = os.environ["IRENE_BIN"]
    parser.add_argument('-x', '--execdir', default=irene_bin,
                        help='path to Irene executable files (default=IRENE_BIN env var: '+irene_bin+')')

    irene_db = os.environ['IRENE_DB']
    parser.add_argument('-d', '--dbdir', default=irene_db,
                        help='path to database directory (default=IRENE_DB env var: '+irene_db+')')

    parser.add_argument('-w', '--workdir', default="",
                        help='path to \'working\' directory for temp files (default=current dir)')

    parser.add_argument('-b', '--tempdir', default="",
                        help='name of directory for temp files (default=generated)')

    parser.add_argument('-n', '--numproc', type=int, default=1,
                        help='number of processors (including master node) default=1, 3 or larger')

    parser.add_argument('--fulldata', action='store_true',
                        help='show all data records, instead of just first and last of each chunk')

    args = parser.parse_args()

    runDemoApp(args.execdir, args.dbdir, args.workdir, args.tempdir, args.numproc, args.fulldata)
