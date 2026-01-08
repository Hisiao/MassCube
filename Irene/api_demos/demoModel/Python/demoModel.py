
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

# the various model import operations depend on those IRENE definitions
from ae9ap9Model import Ae9Ap9Model
from ephemModel import EphemModel
from adiabatModel import AdiabatModel
from accumModel import AccumModel
from aggregModel import AggregModel
from doseModel import DoseModel
import dateTime

def runDemoModel(fulldata):
    np.set_printoptions(precision=2)

    # instantiate object for ephemeris generation
    ephem = EphemModel()

## new way
    ierr = ephem.set_modelDBDir(os.environ['IRENE_DB'])
    if ierr != 0:
        sys.exit('error in set_modelDBDir')
## old way
    #  define magnetic field database file (relative to executable location)
##    ephem.set_magfieldDBFile(os.environ['IRENE_DB']+"/igrfDB.h5")
##

    #  define time limits + step size for orbit propagation
    #   -note that these limits are _inclusive_, so this will produce *361* entries
    ephem.set_times(dateTime.get_modJulDate(2014,190,43200.0),
                    dateTime.get_modJulDate(2014,190,64800.0), 60.0)

    #  define propagator to use  (string is NOT case-sensitive)
    ephem.set_prop("SatEph")

    #  define orbital characteristics

    # set element time
    # --this demonstrates full range of time conversion utility methods available
    ephem.set_elementTime(dateTime.get_modJulDate(2014,
                              dateTime.get_dayYear(2014,7,9),
                              dateTime.get_gmtsec(3,25,45.25)))

    # set orbit 'classical' element values
    ephem.set_rightAscension(123.4)
    ephem.set_argOfPerigee(45.0)
    ephem.set_inclination(35.0)
    ephem.set_altitudeOfApogee(2800.0)
    ephem.set_altitudeOfPerigee(1800.0)

    print(" =================================== ")
    print(" ephemeris demo1: 'sateph' propagator for GDZ ephemeris")
    #  based on all these orbit parameters defined, generate the ephemeris information,
    #   also specifying the coordinate system and units of the returned positions
    #   --since the 'chunk' size has NOT been specified, it defaults to zero, which means
    #     that a single call will return all entries, ie *full* time range of ephemeris
    iNumTimes, daTimes, daCoord1, daCoord2, daCoord3 = ephem.compute("GDZ", "km")
    print("ephem1 sizes: daTimes={}, daCoord1={}".format(len(daTimes), len(daCoord1)))
    if iNumTimes > 0:
        # demonstrate a sampling of results first and last records only
        i = 0
        while i < iNumTimes:
            iYear, iDdd, dGmtsec = dateTime.get_dateTime(daTimes[i])
            print('ephemeris (GDZ/km) [{}]: {:.5f}({},{},{:.1f}) at {:.3f},{:.3f},{:.3f}'.format(
                  i ,daTimes[i], iYear, iDdd, dGmtsec, daCoord1[i], daCoord2[i], daCoord3[i]))
            i = i+iNumTimes-1 if not fulldata else i+1 # if not fulldata, skip to last record
    else:
        print(" ephemeris1 error: ierr=", iNumTimes)
        sys.exit()

    #  ***********************************************************************
    # demonstrate alternate ephemeris generation, returning position and velocity in
    #  GEI coords, and using 'chunks' segmented processing

    #  set chunk size, for segmented output of ephemeris
    #    When not explicitly defined, the chunk size defaults to 0, which means all is
    #    returned at once, as in first section.
    #    --This limitation of the amount of data being processed is important for the
    #      Ae9Ap9 model processing performance. Due to the numerous internal memory
    #      allocations needed for storage of intermediary calculations for the model,
    #      the total memory requirements may exceed system resources available if a
    #      great number of ephemeris positions are called to be processed at once.
    ephem.set_chunkSize(960)
    #  chunk size of 960 is the recommended level for 'typical' systems
    #    otherwise, 120 for systems with limited memory;
    #    systems with more can go higher, but larger than 2400 may degrade performance
    # verify chunk size via 'get'
    iChunk = ephem.get_chunkSize()

    print("\n =================================== ")
    print(" ephemeris demo2: 'sateph' propagator for GEI ephemeris,",
          "converted to GDZ, using chunks of size ")

    # compute initial set of pos and vel ephemeris in GEI coords
    iNumTimes, daTimes, daXPos, daYPos, daZPos, daXVel, daYVel, daZVel \
        = ephem.computeGei()
    if iNumTimes < 0:
        print(" ephemeris2 error: ierr=", iNumTimes)
        sys.exit()

    # start of ephemeris processing loop
    while iNumTimes > 0:
        print("ephem2 sizes: daTimes={}, daCoord1={}".format(len(daTimes), len(daXPos)))
        ierr, daXPosOut, daYPosOut, daZPosOut \
            = ephem.convertCoords("GEI", "km", daTimes, daXPos, daYPos, daZPos, "GDZ", "km")

        # demonstrate a sampling of results, in both coordinates
        #   if fulldata is specified, all records will be printed out
        #   otherwise, just the first and last record
        i = 0
        while i < iNumTimes:
            iYear, iDdd, dGmtsec = dateTime.get_dateTime(daTimes[i])
            print('ephemeris (GEI/km) [{}]: {:.5f}({},{},{:.1f}) at {:.3f},{:.3f},{:.3f}; {:.3f},{:.3f},{:.3f}'.format(
                  i, daTimes[i], iYear, iDdd, dGmtsec, daXPos[i], daYPos[i], daZPos[i],
                  daXVel[i], daYVel[i], daZVel[i]))
            print('ephemeris (GDZ/km) [{}]: {:.5f}({},{},{:.1f}) at {:.3f},{:.3f},{:.3f}'.format(
                  i, daTimes[i], iYear, iDdd, dGmtsec, daXPosOut[i], daYPosOut[i], daZPosOut[i]))
            i = i+iNumTimes-1 if not fulldata else i+1 # if not fulldata, skip to last record

        # get next chunk of ephemeris information
        iNumTimes, daTimes, daXPos, daYPos, daZPos, daXVel, daYVel, daZVel \
            = ephem.computeGei()
        if iNumTimes < 0:
            print(" ephemeris2 error: ierr=", iNumTimes)
            sys.exit()

    #  ***********************************************************************
    # demonstrate basic calls to Ae9Ap9 model, in tandem with ephemeris generation in chunks.

    print("\n =================================== ")
    print(" ae9ap9 demo1: AP9 omnidirectional mean differential flux ")

    # initialize Ae9Ap9 model class for 'AP9'
    ap9model = Ae9Ap9Model()

## new way
    ap9model.set_model("AP9")
    ierr = ap9model.set_modelDBDir(os.environ['IRENE_DB'])
    if ierr != 0:
        sys.exit('error in set_modelDBDir')
## old way
    # specify database files for AP9 model, internal coords and magnetic field
##    ap9model.set_modelDBFile(os.environ['IRENE_DB']+"/AP9V15_runtime_tables.mat")
##    ap9model.set_kPhiDBFile(os.environ['IRENE_DB']+"/fastPhi_net.mat")
##    ap9model.set_kHMinDBFile(os.environ['IRENE_DB']+"/fast_hmin_net.mat")
##    ap9model.set_magfieldDBFile(os.environ['IRENE_DB']+"/igrfDB.h5")
##

    print("before model db load:")
    print(" model name =", ap9model.get_modelName(),
          ", species =", ap9model.get_modelSpecies())

    # define list of proton energy levels
    daProEnergies = np.array([0.1, 0.4, 0.8, 1, 2, 5, 10, 15, 20, 30, 50])
    iNumE = len(daProEnergies)

    # restart the ephemeris generation using the previously-defined orbit
    #  (a restart would automatically be done when any of the orbit definitions is revised,
    #   the propagator or generation times changed, and/or a chunk size specified)
    ephem.restart()

    # get first chunk of ephemeris
    iNumTimes, daTimes, daCoord1, daCoord2, daCoord3 = ephem.compute("GDZ", "km")

    # start of ephemeris generation and model processing loop
    while iNumTimes > 0 :
        # define AP9 flux environment using ephemeris info and proton energy list
        #  --for best model performance, the number of ephemeris positions input should be
        #    moderated, as explained in the comments for the 'ephem.set_chunkSize()' call above
        ierr = ap9model.set_fluxEnvironOmni("1PtDiff", daProEnergies, daProEnergies, daTimes,
                                            "GDZ", "km", daCoord1, daCoord2, daCoord3)
        if ierr != 0:
            print(" ap9model set_fluxEnvironOmni error: ierr=", ierr)
            sys.exit()

        print(" model name = ", ap9model.get_modelName(),
              ", species =", ap9model.get_modelSpecies())

        # call to calculate AP9 "mean" flux values for the defined flux environment
        ierr, da3FluxData = ap9model.computeFlyinMean()
        print(' computeFlyinMean: => ierr=', ierr, ', da3FluxData.shape=', da3FluxData.shape)
        if ierr != 0:
            print(" ap9model flux mean error: ierr=", ierr)
            sys.exit()

        # demonstrate a sampling of results
        #   if fulldata is specified, all records will be printed out
        #   otherwise, just the first and last record
        i = 0
        while i < iNumTimes:
            iYear, iDdd, dGmtsec = dateTime.get_dateTime(daTimes[i])
            print('time+pos [{}]: {:.5f}({},{},{:.1f}) at {:.3f},{:.3f},{:.3f}'.format(
                  i, daTimes[i], iYear, iDdd, dGmtsec, daCoord1[i], daCoord2[i], daCoord3[i]))
            for j in range(0, da3FluxData.shape[2]): # number of pitch angles
                # using string.join and map functions just to make the printed line prettier
                print(' ap9 mean flux [{}][*][{}] ='.format(i, j),
                      ', '.join(map(lambda x: "{:.3e}".format(x), da3FluxData[i,:,j])))
            i = i+iNumTimes-1 if not fulldata else i+1 # if not fulldata, skip to last record

        # get next chunk of ephemeris
        iNumTimes, daTimes, daCoord1, daCoord2, daCoord3 = ephem.compute("GDZ", "km")
    # end of ephemeris generation and model processing loop

    #  ***********************************************************************
    # demonstrate more advanced calls to Ae9Ap9 model, in tandem with ephemeris
    #   generation in chunks.

    print("\n =================================== ")
    print(" ae9ap9 demo2: AE9 unidirectional percentile integral flux ")

    #   initialize a *separate* Ae9Ap9 model class for 'AE9'
    #    (_cannot_ load different model database into previously initialized object)
    ae9model = Ae9Ap9Model()

## new way
    ae9model.set_model("AE9")
    ierr = ae9model.set_modelDBDir(os.environ['IRENE_DB'])
    if ierr != 0:
        sys.exit('error in set_modelDBDir')
## old way
    # specify database files for AE9 model, internal coords and magnetic field
##    ae9model.set_modelDBFile(os.environ['IRENE_DB']+"/AE9V15_runtime_tables.mat")
##    ae9model.set_kPhiDBFile(os.environ['IRENE_DB']+"/fastPhi_net.mat")
##    ae9model.set_kHMinDBFile(os.environ['IRENE_DB']+"/fast_hmin_net.mat")
##    ae9model.set_magfieldDBFile(os.environ['IRENE_DB']+"/igrfDB.h5")
##

    # demonstration showing *manual* loading of defined model database
    #  --this is done automatically on first call to setFluxEnvironment
    ierr = ae9model.load_modelDB()
    if ierr != 0:
        print(" ae9model load_modelDB error: ierr=", ierr)

    print(" model name =", ae9model.get_modelName(),
          ", species =", ae9model.get_modelSpecies())

    # define list of electron energy levels
    daEleEnergies = np.array([0.04, 0.1, 0.2, 0.4, 0.7, 1.0, 1.5, 2.0, 3.5, 5.0])

    # define a list of pitch angles
    daPitchAngles = np.array([90.0, 85.0, 80.0, 75.0, 70.0])

    # reset the orbit definitions used for the ephemeris generation
    #  (but this keeps the previous propagator choice and time limits)
    ephem.reset_orbitParameters()
    #  -- alternatively, a new ephemeris object could have been instantiated instead

    # set element time
    # --this demonstrates full range of time conversion utility methods available
    ephem.set_elementTime(dateTime.get_modJulDate(2014,
                              dateTime.get_dayYear(2014,7,9),
                              dateTime.get_gmtsec(12,0,0.0)))
    # set orbit 'mean' element values
    ephem.set_rightAscension(65.45)
    ephem.set_argOfPerigee(129.4)
    ephem.set_inclination(28.5)
    ephem.set_eccentricity(0.255)
    ephem.set_meanMotion(6.53)
    ephem.set_meanAnomaly(305.65)

    # get first chunk of ephemeris
    iNumTimes, daTimes, daCoord1, daCoord2, daCoord3 = ephem.compute("GDZ", "km")

    # start of ephemeris generation and model processing loop
    while iNumTimes > 0 :
        # define AP9 flux environment using ephemeris info and proton energy list
        #  --for best model performance, the number of ephemeris positions input should be
        #    moderated, as explained in the comments for the 'ephem.set_chunkSize()' call above
        ierr = ae9model.set_fluxEnvironFixPitch("Integral", daEleEnergies, daEleEnergies, daTimes,
                                                "GDZ", "km", daCoord1, daCoord2, daCoord3, daPitchAngles)
        if ierr != 0:
            print(" ae9model set_fluxEnvironFixPitch error: ierr=", ierr)

        # call to calculate AP9 "mean" flux values for the defined flux environment
        ierr, da3FluxData = ae9model.computeFlyinMean()
        print(' computeFlyinMean: => ierr=', ierr, ', da3FluxData.shape=', da3FluxData.shape)
        if ierr != 0:
            print(" ae9model flux mean error: ierr=", ierr)
            sys.exit()

        # call to calculate three sets of AE9 "percentile" flux values,
        #   for the defined flux environment
        iNum, da3FluxDataPAperc50 = ae9model.computeFlyinPercentile(50)
        if iNum < 0:
            print(" ae9model flux percentile 50 error: ierr=", ierr)
            sys.exit()
        iNum, da3FluxDataPAperc75 = ae9model.computeFlyinPercentile(75)
        if iNum < 0:
            print(" ae9model flux percentile 75 error: ierr=", ierr)
            sys.exit()
        iNum, da3FluxDataPAperc95 = ae9model.computeFlyinPercentile(95)
        if iNum < 0:
            print(" ae9model flux percentile 95 error: ierr=", ierr)
            sys.exit()

        print("ae9model sizes: daTimes={}, daCoord1={}, flux={}".format(
              len(daTimes), len(daCoord1), da3FluxDataPAperc95.shape))

        # demonstrate a sampling of results
        #   if fulldata is specified, all records will be printed out
        #   otherwise, just the first and last record
        i = 0
        while i < iNumTimes:
            iYear, iDdd, dGmtsec = dateTime.get_dateTime(daTimes[i])
            print('time+pos [{}]: {:.5f}({},{},{:.1f}) at {:.3f},{:.3f},{:.3f}'.format(
                  i, daTimes[i], iYear, iDdd, dGmtsec, daCoord1[i], daCoord2[i], daCoord3[i]),
                  ", pitch angles =", ', '.join(map(lambda x: "{:.1f}".format(x), daPitchAngles)))
            for j in range(0, da3FluxDataPAperc95.shape[2]): # number of pitch angles
                # using string.join and map functions just to make the printed line prettier
                print(' perc95 flux [{}][*][{}] ='.format(i, j),
                      ', '.join(map(lambda x: "{:.3e}".format(x), da3FluxDataPAperc95[i,:,j])))
            i = i+iNumTimes-1 if not fulldata else i+1 # if not fulldata, skip to last record

        # get next chunk of ephemeris
        iNumTimes, daTimes, daCoord1, daCoord2, daCoord3 = ephem.compute("GDZ", "km")
    # end of ephemeris generation and model processing loop

    #  ***********************************************************************
    # demonstrate calls to Adiabatic model, in tandem with ephemeris generation in chunks.

    print("\n =================================== ")
    print(" adiabatic demo: multiple pitch angles")

    # initialize Adiabatic model class
    adiabat = AdiabatModel()

## new way
    ierr = adiabat.set_modelDBDir(os.environ['IRENE_DB'])
    if ierr != 0:
        sys.exit('error in set_modelDBDir')
## old way
    # define coords and mag field database files (relative to the executable location)
##    adiabat.set_kPhiDBFile(os.environ['IRENE_DB']+"/fastPhi_net.mat")
##    adiabat.set_kHMinDBFile(os.environ['IRENE_DB']+"/fast_hmin_net.mat")
##    adiabat.set_magfieldDBFile(os.environ['IRENE_DB']+"/igrfDB.h5")
##

    # restart the ephemeris generation of the previously defined orbit at the start time
    ephem.restart()

    # get first chunk of ephemeris, but this time in a different coordinate system
    iNumTimes, daTimes, daCoord1, daCoord2, daCoord3 = ephem.compute("RLL", "Re")

    # start of ephemeris generation and model processing loop
    while iNumTimes > 0 :

        # call to calculate adiabatic coordinates based on input ephemeris
        #   and pitch angle information
        ierr, da2Alpha, da2Lm, da2K, da2Phi, da2Hmin, da2Lstar, \
            daBmin, daBlocal, daMagLT, da2B, da2I = adiabat.computeCoordinateSet(
                                        "RLL", "Re", daTimes,
                                        daCoord1, daCoord2, daCoord3, daPitchAngles)
        if ierr != 0:
            print(" adiabatic coordinates error: ierr=", ierr)
            sys.exit()

        print("adiabat sizes: daTimes={}, daCoord1={}, alpha={}".format(
              len(daTimes), len(daCoord1), da2Alpha.shape))

        # demonstrate a sampling of results
        #   if fulldata is specified, all records will be printed out
        #   otherwise, just the first and last record
        i = 0
        while i < iNumTimes:
            iYear, iDdd, dGmtsec = dateTime.get_dateTime(daTimes[i])
            print('time+pos(RLL/Re) [{}]: {:.5f}({},{},{:.1f}) at {:.3f},{:.3f},{:.3f}'.format(
                  i, daTimes[i], iYear, iDdd, dGmtsec, daCoord1[i], daCoord2[i], daCoord3[i]),
                  ", pitch angles =", ', '.join(map(lambda x: "{:.1f}".format(x), daPitchAngles)))
            for j in range(0, da2Alpha.shape[1]): # number of pitch angles
                print(' adiabat values [{}][{}] = {:.2f} {:.3f} {:.5f} {:.5f} {:.1f} {:.3f} {:.3e} {:.3e} {:.2f} {:.3e} {:.3e} {:.3e} {:.5f}'.format(
                      i, j, da2Alpha[i][j], da2Lm[i][j], da2K[i][j], da2Phi[i][j],
                      da2Hmin[i][j], da2Lstar[i][j], daBmin[i], daBlocal[i], daMagLT[i],
                      da2B[i][0], da2B[i][1], da2B[i][2], da2I[i][j]))
            i = i+iNumTimes-1 if not fulldata else i+1 # if not fulldata, skip to last record

        # get next chunk of ephemeris
        iNumTimes, daTimes, daCoord1, daCoord2, daCoord3 = ephem.compute("RLL", "Re")
    # end of ephemeris generation and model processing loop

    # ***********************************************************************
    # demonstrate additional advanced calls to Ae9Ap9 model, and aggregation of results,
    #  in tandem with ephemeris generation in chunks.

    print("\n =================================== ")
    print(" ae9ap9 demo3: AP9 omnidirectional aggregated perturbed mean differential flux")

    # (reuse the previously-instantiated AP9 class object)

    # initialize Aggregation model class
    aggreg = AggregModel()

    iaConf = [85, 95]

    # restart the ephemeris generation of the previously defined orbit at the start time
    ephem.restart()

    # get first chunk of ephemeris
    iNumTimes, daTimes, daCoord1, daCoord2, daCoord3 = ephem.compute("GEO", "km")

    # start of ephemeris generation and model processing loop
    while iNumTimes > 0 :
        # define AP9 flux environment using ephemeris info and proton energy list
        #  --for best model performance, the number of ephemeris positions input should be
        #    moderated, as explained in the comments for the 'ephem.set_chunkSize()' call above
        ierr = ap9model.set_fluxEnvironOmni("1PtDiff", daProEnergies, daProEnergies, daTimes,
                                            "GEO", "km", daCoord1, daCoord2, daCoord3)
        if ierr != 0:
            print(" ap9model set_fluxEnvironOmni error: ierr=", ierr)
            sys.exit()

        # initialize aggregation for the current time segment (chunk)
        aggreg.reset()

        # call to calculate 25 scenarios of AP9 "perturbed mean" flux values
        #   for the defined flux environment
        for j in range(1, 26):
            ierr, da3FluxData = ap9model.computeFlyinPerturbedMean(j)
            if ierr != 0:
                print(" ap9model flux perturbed mean error: ierr=", ierr)
                sys.exit()

            # save the model results and associated time values in the aggregation object
            ierr = aggreg.addScenToAgg(daTimes, da3FluxData)
            if ierr != 0:
                print(" ae9ap9 aggreg of perturbed mean error: ierr=", ierr)
                sys.exit()

        # calculate specified confidence level values of aggregation, and display
        for icl in iaConf:
            ierr, daTimesAgg, da3FluxPMAgg = aggreg.computeConfLevel(icl)
            if ierr != 0:
                print(" ae9ap9 aggregation computeConfLevel error: ierr=", ierr)
                sys.exit()

            print(("{}% Confidence level results from aggregation of " +
                  "25 scenarios of perturbed mean fluxes").format(icl))
            print("agg sizes: daTimesAgg={}, daCoord1={}, flux={}".format(
                  len(daTimesAgg), len(daCoord1), da3FluxPMAgg.shape))

            # demonstrate a sampling of results
            #   if fulldata is specified, all records will be printed out
            #   otherwise, just the first and last record
            i = 0
            while i < iNumTimes:
                iYear, iDdd, dGmtsec = dateTime.get_dateTime(daTimesAgg[i])
                print('time+pos(GEO/km) [{}]: {:.5f}({},{},{:.1f}) at {:.3f},{:.3f},{:.3f}'.format(
                      i, daTimesAgg[i], iYear, iDdd, dGmtsec, daCoord1[i], daCoord2[i], daCoord3[i]))
                for j in range(0, da3FluxPMAgg.shape[2]): # number of pitch angles
                    # using string.join and map functions just to make the printed line prettier
                    print(' ap9 {}% CL PM flux [{}][*][{}] ='.format(icl, i, j),
                          ', '.join(map(lambda x: "{:.3e}".format(x), da3FluxPMAgg[i,:,j])))
                i = i+iNumTimes-1 if not fulldata else i+1 # if not fulldata, skip to last record

        # get next chunk of ephemeris
        iNumTimes, daTimes, daCoord1, daCoord2, daCoord3 = ephem.compute("GEO", "km")
    # end of ephemeris generation and model processing loop

    # ***********************************************************************
    # demonstrate  calls to Ae9Ap9 model, accumulation of the results, and calculation
    #  of dose results, in tandem with ephemeris generation in chunks.
    print("\n =================================== ")
    print(" ae9ap9 demo4: AE9 omnidirectional mean differential flux and fluences")

    # (reuse the previously-instantiated AE9 class object)

    # initialize multiple Accumulation model class objects
    #  (separate AccumModel objects are required for each data type
    #   and/or accumulation interval)

    # set up list of accumulation interval times
    #  -930 is equal to 15.5 of the ephemeris timesteps
    #   (chosen to be NOT an even multiple)
    #  -1800 is equal to 30 ephemeris timesteps
    iInterval = [930, 1800]
    iNumInt = len(iInterval)

    # list of Accum models for each interval
    accumModels = [AccumModel() for n in range(iNumInt)]

    for nn in range(iNumInt):
        ierr = accumModels[nn].set_intervalSec(iInterval[nn])
        if ierr != 0:
            print(" accum set_intervalSec error: ierr=", ierr)
            sys.exit()

    # restart the ephemeris generation of the previously-defined orbit
    ephem.restart()

    # get first chunk of ephemeris
    iNumTimes, daTimes, daCoord1, daCoord2, daCoord3 = ephem.compute("GDZ", "km")

    # initialize variables to None for use later
    daCoord1prev = daCoord2prev = daCoord3prev = None

    # start of ephemeris generation and model processing loop
    while iNumTimes > 0 :
        # define AP9 flux environment using ephemeris info and proton energy list
        #  --for best model performance, the number of ephemeris positions input should be
        #    moderated, as explained in the comments for the 'ephem.set_chunkSize()' call above
        ierr = ae9model.set_fluxEnvironOmni("1PtDiff", daEleEnergies, daEleEnergies, daTimes,
                                            "GDZ", "km", daCoord1, daCoord2, daCoord3)
        if ierr != 0:
            print(" ae9model set_fluxEnvironOmni error: ierr=", ierr)
            sys.exit()

        # call to calculate AP9 "mean" flux values for the defined flux environment
        ierr, da3FluxData = ae9model.computeFlyinMean()
        print(' computeFlyinMean: => ierr=', ierr, ', da3FluxData.shape=', da3FluxData.shape)
        if ierr != 0:
            print(" ae9model flux mean error: ierr=", ierr)
            sys.exit()

        # demonstrate a sampling of results
        #   if fulldata is specified, all records will be printed out
        #   otherwise, just the first and last record
        i = 0
        while i < iNumTimes:
            iYear, iDdd, dGmtsec = dateTime.get_dateTime(daTimes[i])
            print('time+pos [{}]: {:.5f}({},{},{:.1f}) at {:.3f},{:.3f},{:.3f}'.format(
                  i, daTimes[i], iYear, iDdd, dGmtsec, daCoord1[i], daCoord2[i], daCoord3[i]))
            for j in range(0, da3FluxData.shape[2]): # number of pitch angles
                # using string.join and map functions just to make the printed line prettier
                print(' ae9 mean flux [{}][*][{}] ='.format(i, j),
                      ', '.join(map(lambda x: "{:.3e}".format(x), da3FluxData[i,:,j])))
            i = i+iNumTimes-1 if not fulldata else i+1 # if not fulldata, skip to last record

        # loop over accumulation objects
        for nn in range(iNumInt):
            accum = accumModels[nn]
            intv = iInterval[nn]
            # load this set of 'mean' flux values into accumulation object
            accum.loadBuffer(daTimes, da3FluxData)

            # calculate cumulative fluence values using contents of first accumulation buffer
            #  (this would produce identical results for the second buffer)
            iNum, daFluenceTimes, da3FluenceData = accum.computeFluence()
            if iNum < 0:
                print(" accumModel cumulative fluence error: ierr=", iNum)
                sys.exit()

            # -these are cumulative values reported at each ephemeris timestep
            # demonstrate these cumulative fluence results
            print(" cumulative fluence ae9 sizes: daFluenceTimes={}, daCoord1={}, fluence={}".format(
                  len(daFluenceTimes), len(daCoord1), da3FluenceData.shape))

            # demonstrate a sampling of results
            #   if fulldata is specified, all records will be printed out
            #   otherwise, just the first, middle, and last record
            i = 0
            while i < iNum:
                iYear, iDdd, dGmtsec = dateTime.get_dateTime(daFluenceTimes[i])
                print('time+pos [{}]: {:.5f}({},{},{:.1f}) at {:.3f},{:.3f},{:.3f}'.format(
                      i, daFluenceTimes[i], iYear, iDdd, dGmtsec, daCoord1[i], daCoord2[i], daCoord3[i]))
                # show associated fluence values
                for j in range(0, da3FluenceData.shape[2]): # number of pitch angles
                    # using string.join and map functions just to make the printed line prettier
                    print(' ae9 cumul fluence [{}][*][{}] ='.format(i, j),
                          ', '.join(map(lambda x: "{:.3e}".format(x), da3FluenceData[i,:,j])))
                i = i+(iNum//2)-1 if not fulldata else i+1 # if not fulldata, skip to middle then last record
                if not fulldata and i == iNum-2:
                    i += 1 # fix for last value being 1 too low

            # calculate (completed) interval fluence values using contents
            #   of each accumulation buffer
            iNum, daIntvFluenceTimes, da3IntvFluence, iaIndices = accum.computeIntvFluence()
            if iNum < 0:
                print(" accumModel interval fluence error: ierr=", iNum)
                sys.exit()

            # -these are interval fluence values reported at the specified interval durations
            # -note that data of _partially_ complete intervals are retained, to be included
            #  within an interval that will be completed during the next 'chunk' processing
            #        demonstrate these interval fluence results
            print(" --({}s) interval fluence ae9 sizes: daIntvFluenceTimes={}, daCoord1={}, intv fluence={}".format(
                  intv, len(daIntvFluenceTimes), len(daCoord1), da3IntvFluence.shape))

            # show fluence values from each interval
            # demonstrate *all* of these results
            for i in range(iNum):
                iYear, iDdd, dGmtsec = dateTime.get_dateTime(daIntvFluenceTimes[i])
                index = iaIndices[i]
                print('time+pos [{}]: {:.5f}({},{},{:.1f}) interval end at approx {:.3f},{:.3f},{:.3f} [{}]'.format(
                      i, daIntvFluenceTimes[i], iYear, iDdd, dGmtsec,
                      daCoord1[index], daCoord2[index], daCoord3[index], index))
                # show associated fluence values
                print(' ae9 intv fluence [{}][*][{}] ='.format(i, j),
                      ', '.join(map(lambda x: "{:.3e}".format(x), da3IntvFluence[i,:,j])))

            # call to accumulate these interval-specific fluence values
            iNum, da3IntvFluenceAccum = accum.accumIntvFluence(
                    daIntvFluenceTimes, da3IntvFluence)
            if iNum < 0:
                print(" accumModel interval fluence calculation error: ierr=", iNum)
                sys.exit()

            # demonstrate these interval fluence results
            print(" --({}s) accum interval fluence ae9 sizes: daIntvFluenceTimes={}, daCoord1={}, accum intv fluence={}".format(
                  intv, len(daIntvFluenceTimes), len(daCoord1), da3IntvFluenceAccum.shape))

            # show fluence values from each interval
            # demonstrate *all* of these results
            for i in range(iNum):
                iYear, iDdd, dGmtsec = dateTime.get_dateTime(daIntvFluenceTimes[i])
                index = iaIndices[i]
                print('time+pos [{}]: {:.5f}({},{},{:.1f}) interval end at approx {:.3f},{:.3f},{:.3f} [{}]'.format(
                      i, daIntvFluenceTimes[i], iYear, iDdd, dGmtsec,
                      daCoord1[index], daCoord2[index], daCoord3[index], index))
                # show associated fluence values
                print(' ae9 accum intv fluence [{}][*][{}] ='.format(i, j),
                      ', '.join(map(lambda x: "{:.3e}".format(x), da3IntvFluenceAccum[i,:,j])))
            # end loop over accumulation objects (nn)

        # retain ephemeris information for later reference
        daCoord1prev = daCoord1
        daCoord2prev = daCoord2
        daCoord3prev = daCoord3

        # get next chunk of ephemeris
        iNumTimes, daTimes, daCoord1, daCoord2, daCoord3 = ephem.compute("GDZ", "km")
        # when ephemeris generation is already complete,
        #   these returned vectors will be *empty*
    # end of ephemeris generation and model processing loop

    # collect fluence data from last *partial* accumulation intervals, if any
    #  --note last argument added to method call, with flag to get this data

    # loop over accumulation objects
    for nn in range(iNumInt):
        accum = accumModels[nn]
        intv = iInterval[nn]

        # collect fluence data from last *partial* accumulation intervals, if any
        #  --note last argument added to method call, with flag to get this data
        iNum, daIntvFluenceTimes, da3IntvFluence, iaIndices = accum.computeIntvFluence(1)
        if iNum < 0:
            print(" accumModel interval fluence error: ierr=", iNum)
            sys.exit()
        # -these are interval fluence values reported at the specified interval durations
        # -note that data of _partially_ complete intervals are retained, to be included
        #  within an interval that will be completed during the next 'chunk' processing
        #        demonstrate these interval fluence results
        print(" --({}s) partial interval fluence ae9 sizes: iNum={}, daIntvFluenceTimes={}, daCoord1={}, intv fluence={}".format(
              intv, iNum, len(daIntvFluenceTimes), len(daCoord1prev), da3IntvFluence.shape))
        # -returned iNum will either be 0 (no leftover data) or 1 (partial interval data)

        # show fluence values from last (partial) interval, if present
        if iNum > 0:
            for i in range(iNum):
                iYear, iDdd, dGmtsec = dateTime.get_dateTime(daIntvFluenceTimes[i])
                index = iaIndices[i]
                # check index value for '-1' indicating last record of input
                if iaIndices[i] == -1: index = len(daCoord1prev)-1
                print('time+pos [{}]: {:.5f}({},{},{:.1f}) interval end at approx {:.3f},{:.3f},{:.3f} [{}]'.format(
                      i, daIntvFluenceTimes[i], iYear, iDdd, dGmtsec,
                      daCoord1prev[index], daCoord2prev[index], daCoord3prev[index], index))
                # show associated fluence values
                print(' ae9 partial intv fluence [{}][*][{}] ='.format(i, j), ', '.join(map(lambda x: "{:.3e}".format(x), da3IntvFluence[i,:,j])))

            # call to accumulate these interval-specific fluence values
            iNumTimes, da3IntvFluenceAccum = accum.accumIntvFluence(
                    daIntvFluenceTimes, da3IntvFluence)
            if iNum < 0:
                print(" accumModel interval fluence calculation error: ierr=", iNum)
                sys.exit()

            # demonstrate these interval fluence results
            print(" --({}s) accum interval fluence ae9 sizes: daIntvFluenceTimes={}, daCoord1={}, accum intv fluence={}".format(
                  intv, len(daIntvFluenceTimes), len(daCoord1prev), da3IntvFluenceAccum.shape))

            # show final cumulative summation of fluence values over the intervals
            if iNum > 0:
                for i in range(iNum):
                    iYear, iDdd, dGmtsec = dateTime.get_dateTime(daIntvFluenceTimes[i])
                    index = iaIndices[i]
                    # check index value for '-1' indicating last record of input
                    if iaIndices[i] == -1: index = len(daCoord1prev)-1
                    print('time+pos [{}]: {:.5f}({},{},{:.1f}) interval end at approx {:.3f},{:.3f},{:.3f} [{}]'.format(
                          i, daIntvFluenceTimes[i], iYear, iDdd, dGmtsec,
                          daCoord1prev[index], daCoord2prev[index], daCoord3prev[index], index))
                    # show associated fluence values
                    print(' ae9 accum intv fluence [{}][*][{}] ='.format(i, j),
                          ', '.join(map(lambda x: "{:.3e}".format(x), da3IntvFluenceAccum[i,:,j])))
        # end loop over accumulation objects (nn)

    # ***********************************************************************
    print("\n =================================== ")
    print(" dose demo: accumulated dose from AE9 omnidirectional mean differential fluences")

    # initialize Dose model class
    dose = DoseModel()
## new way
    ierr = dose.set_modelDBDir(os.environ['IRENE_DB'])
    if ierr != 0:
        sys.exit('error in set_modelDBDir')
## old way
    # define shieldDose2 model database file
##    dose.set_modelDBFile(os.environ['IRENE_DB']+"/sd2DB.h5")

    # define flux particle species and energies
    dose.set_species(ae9model.get_modelSpecies())
    dose.set_energies(daEleEnergies)

    # define shielding depth values/units/geometry and detector type
    daDepths = np.array([0.1, 0.2, 0.4, 0.5, 0.75, 1.0, 1.5, 2.0])
    dose.set_depths(daDepths, "mm")
    dose.set_detector("Si")
    dose.set_geometry("semiinfslab")

    # using omnidirectional electron accumulated fluence from previous section

    # IMPORTANT:  Flux and Fluence inputs to the DoseModel *must* be:
    #             1pt Differential Flux type
    #             Omnidirectional only
    ierr, da3DoseRate = dose.computeFluenceDose(da3IntvFluenceAccum)

    if ierr < 0:
        print(" dose computeFluenceDose error: ierr=", ierr)
        sys.exit()
    # report only the very last results
    else:
        i = da3DoseRate.shape[0] - 1
        print("ae9 accumulated dose over full time period at depths: ",
              ', '.join(map(lambda x: "{:.2f}".format(x), daDepths)), "mm")
        print(' ae9 accum dose [{}][*][0] ='.format(i),
              ', '.join(map(lambda x: "{:.3e}".format(x), da3DoseRate[i,:,0])))


#=======================================================================================
# Commmand-line parser
#
# Parse arguments
#
#      --fulldata   switch to turn on printing of full data                             [optional]
#                     (default is print only first and last element of every array)
#      -h           help

if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(description='DemoModel - Python API')

    parser.add_argument('--fulldata', action='store_true',
                        help='switch to turn on printing of full data')

    args = parser.parse_args()

    runDemoModel(args.fulldata)

