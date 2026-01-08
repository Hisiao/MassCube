
# enable use of python3 print function form in python 2.x
from __future__ import print_function

# for the 'import irene_defs' to be successful, either:
#     specify the path to the Ae9Ap9 root directory here:
#sys.path.append(os.environ['HOME']+'/Ae9Ap9')
#     or in the PYTHONPATH environement variable
#
# load definitions for required IRENE environment variables
import irene_defs

from cammiceModel import CammiceModel
from ephemModel import EphemModel
import numpy as np
import pytest


@pytest.fixture
def cammice():
    cammice = CammiceModel()
    print('cammice zHandle=', cammice.zHandle)
    return cammice


def test_set_modelDBFile(cammice):
    # failing tests
    for strString in ["cammiceDB.h5", "../modelData/cammiceDB.h5", "$DATADIR/cammiceDB.h5"]:
        ierr = cammice.set_modelDBFile(strString)
        print(' set_modelDBFile: strString=', strString, ' => ', ierr)
        assert(ierr < 0)

        strRet = cammice.get_modelDBFile()
        print(' get_modelDBFile: => ', strRet)
        assert(strRet == "UNDEF")

    # passing tests
    for strString in ["../../modelData/cammiceDB.h5"]:
        ierr = cammice.set_modelDBFile(strString)
        print(' set_modelDBFile: strString=', strString, ' => ', ierr)
        assert(ierr == 0)

        strRet = cammice.get_modelDBFile()
        print(' get_modelDBFile: => ', strRet)
        assert(strRet == strString)




def test_set_modelDBDir( cammice ):

    # use ephem model to generate test inputs
    ephem = EphemModel()
    dStart = 58119.5
    ierr = ephem.set_startTime(dStart)
    print(' set_startTime: start=%.1f => %i' % (dStart, ierr))
    assert(ierr == 0)
    dEnd = 58120.5
    ierr = ephem.set_endTime(dEnd)
    print(' set_endTime: end=%.1f => %i' % (dEnd, ierr))
    assert(ierr == 0)
    dStep = 300.
    ierr = ephem.set_timeStep(dStep)
    print(' set_timeStep: step=%.0f => %i' % (dStep, ierr))
    assert(ierr == 0)
    # current ephemeris start time 58119.5 is 01 Jan 2018 1200 GMT
    # This TLE epoch is 27 Jan 2005 0653 GMT; 
    strLine1 = '1 20712U 90065A   05027.28729034  .00000029  00000-0  20288-4 0   166'
    strLine2 = '2 20712  18.2943  30.1164 7147621 257.9138  20.6811  2.41951061118231'
    ierr = ephem.set_tle(strLine1, strLine2)
    print(' set_tle( <CRRES satellite> ) =>',ierr)
    ierr = ephem.set_startTime(dStart)
    print(' set_startTime: start=%.1f => %i' % (dStart, ierr))
    ierr = ephem.set_endTime(dEnd)
    print(' set_endTime: end=%.1f => %i' % (dEnd, ierr))
    ierr = ephem.set_timeStep(dStep)
    print(' set_timeStep: end=%.1f => %i' % (dStep, ierr))
    dStepSec = ephem.get_timeStep()
    assert(dStepSec==300.0)
    ierr = ephem.set_modelDBDir("../../modelData/")
    print(' ephem.set_modelDBDir: strString=', "../../modelData/", ' => ', ierr)
    assert(ierr == 0)

    strCoordSys = 'GDZ'
    strCoordUnits = 'km'
    iNum, daTime, daCoord1, daCoord2, daCoord3 \
        = ephem.compute(strCoordSys, strCoordUnits)
    print(' ephem.compute: CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTime)=',
          len(daTime), ', len(daCoord1)=', len(daCoord1))
    assert(iNum > 0)

    ierr = cammice.set_ephemeris( daTime, daCoord1, daCoord2, daCoord3)
    print(' cammice.set_ephemeris: => ', ierr)
    assert(ierr == 0)  # good input of length 1

    numEphems = cammice.get_numEphemeris()
    print(' cammice.get_numEphemeris: => ', numEphems)
    assert(numEphems > 0)

    # setting up parameters for model
    strString = "all"
    ierr = cammice.set_dataFilter(strString)
    print(' set_dataFilter: strString=', strString, ' => ', ierr)
    assert(ierr == 0)
    strString = "igrfop"
    ierr = cammice.set_magfieldModel(strString)
    print(' set_magfieldModel: strString=', strString, ' => ', ierr)
    assert(ierr == 0)

    # first run parameters - comparing different pitch angle bins
    strSpec = "H+"
    strPitch = "omni"
    ierr = cammice.set_species(strSpec)
    print(' set_species: strString=', strSpec, ' => ', ierr)
    assert(ierr == 0)
    ierr = cammice.set_pitchAngleBin(strPitch)
    print(' set_pitchAngleBin: strString=', strPitch, ' => ', ierr)
    assert(ierr == 0)

    for strString in ["goober", "../modelData", "$DATADIR/"]:
        ierr = cammice.set_modelDBDir(strString)
        print(' set_modelDBDir: strString=', strString, ' => ', ierr)
        assert(ierr < 0)

        strRet = cammice.get_modelDBDir()
        print(' get_modelDBDir: => ', strRet)
        assert(strRet == "")

    print(' --model DB *prior* to compute call:')
    print(' get_modelDBFile: => ', cammice.get_modelDBFile())
    print(' get_magfieldDBFile: => ', cammice.get_magfieldDBFile())
    # compute and output results
    ierr, da2FluxData = cammice.computeFlux()
    print(' computeFlux: ierr=', ierr,
          ' da2Flux shape=', da2FluxData.shape)
    assert(ierr < 0)

    for strString in ["../../kernelXml/","../../modelData/"]:
        ierr = cammice.set_modelDBDir(strString)
        print(' set_modelDBDir: strString=', strString, ' => ', ierr)
        assert(ierr == 0)

        strRet = cammice.get_modelDBDir()
        print(' get_modelDBDir: => ', strRet)
        assert(strRet == strString)

        print(' --model DB *prior* to compute call:')
        print(' get_modelDBFile: => ', cammice.get_modelDBFile())
        print(' get_magfieldDBFile: => ', cammice.get_magfieldDBFile())
        # compute and output results
        ierr, da2FluxData = cammice.computeFlux()
        print(' computeFlux: ierr=', ierr,
              ' da2Flux shape=', da2FluxData.shape)
        if strString == "../../modelData/":
          assert (ierr == 0)
        else:
          assert(ierr < 0)

    print('-- automatically defined via ModelDBDir:')
    print(' get_modelDBFile: => ', cammice.get_modelDBFile())
    print(' get_magfieldDBFile: => ', cammice.get_magfieldDBFile())

def test_set_magfieldDBFile(cammice):
    # put in a valid file, so to fully test reset on invalid one
    ierr = cammice.set_magfieldDBFile('../../modelData/igrfDB.h5')
    assert(ierr == 0)

    # failing tests
    for strString in ["igrfDB.h5", "../modelData/igrfDB.h5", "$DATADIR/igrfDB.h5"]:
        ierr = cammice.set_magfieldDBFile(strString)
        print(' set_modelDBFile: strString=', strString, ' => ', ierr)
        assert(ierr < 0)

        strRet = cammice.get_magfieldDBFile()
        print(' get_magfieldDBFile: => ', strRet)
        assert(strRet == "UNDEF")

    # passing tests
    for strString in ["../../modelData/igrfDB.h5"]:
        ierr = cammice.set_magfieldDBFile(strString)
        print(' set_magfieldDBFile: strString=', strString, ' => ', ierr)
        assert(ierr == 0)

        strRet = cammice.get_magfieldDBFile()
        print(' get_magfieldDBFile: => ', strRet)
        assert(strRet == strString)


def test_set_magfieldModel(cammice):
    # failing tests
    for strString in ["AE9", "AP9", "AX8", "CRRES", ""]:
        ierr = cammice.set_magfieldModel(strString)
        print(' set_magfieldModel: strString=', strString, ' => ', ierr)
        assert(ierr < 0)

        strRet = cammice.get_magfieldModel()
        print(' get_magfieldModel: => ', strRet)
        assert(strRet == "Undefined")

    # passing tests
    for strString in ["igrf", "igrfop", "igrf/op", "igrf_op"]:
        ierr = cammice.set_magfieldModel(strString)
        print(' set_magfieldModel: strString=', strString, ' => ', ierr)
        assert(ierr == 0)

        strRet = cammice.get_magfieldModel()
        print(' get_magfieldModel: => ', strRet)

        if strString == "igrf":
            assert(strRet == "IGRF")
        else:
            assert(strRet == "IGRF/OP")


def test_set_dataFilter(cammice):
    # failing tests
    for strString in ["none", ""]:
        ierr = cammice.set_dataFilter(strString)
        print(' set_dataFilter: strString=', strString, ' => ', ierr)
        assert(ierr < 0)

        strRet = cammice.get_dataFilter()
        print(' get_dataFilter: => ', strRet)
        assert(strRet == "Undefined")

    # passing tests
    for strString in ["all", "filtered"]:
        ierr = cammice.set_dataFilter(strString)
        print(' set_dataFilter: strString=', strString, ' => ', ierr)
        assert(ierr == 0)

        strRet = cammice.get_dataFilter()
        print(' get_dataFilter: => ', strRet)
        assert(strRet == strString)


def test_set_pitchAngleBin(cammice):
    # failing tests
    for strString in ["0", "180-190", "0-180", "", "all"]:
        ierr = cammice.set_pitchAngleBin(strString)
        print(' set_pitchAngleBin: strString=', strString, ' => ', ierr)
        assert(ierr < 0)

        strRet = cammice.get_pitchAngleBin()
        print(' get_pitchAngleBin: => ', strRet)
        assert(strRet == "Undefined")

    # passing tests
    for strString in ["omnidirectional", "0-10", "10-20", "20-30", "30-40", "40-50",
                      "50-60", "60-70", "70-80", "80-90", "90-100", "100-110", "110-120",
                      "120-130", "130-140", "140-150", "150-160", "160-170", "170-180"]:
        ierr = cammice.set_pitchAngleBin(strString)
        print(' set_pitchAngleBin: strString=', strString, ' => ', ierr)
        assert(ierr == 0)

        strRet = cammice.get_pitchAngleBin()
        print(' get_pitchAngleBin: => ', strRet)
        assert(strRet == strString)


def test_set_species(cammice):
    # failing tests
    for strString in ["H-", "P", "", "all"]:
        ierr = cammice.set_species(strString)
        print(' set_species: strString=', strString, ' => ', ierr)
        assert(ierr < 0)

        strRet = cammice.get_species()
        print(' get_species: => ', strRet)
        assert(strRet == "Undefined")

    # passing tests
    for strString in ["H+", "HE+", "HE+2", "O+", "H", "HE", "O", "IONS"]:
        ierr = cammice.set_species(strString)
        print(' set_species: strString=', strString, ' => ', ierr)
        assert(ierr == 0)

        strRet = cammice.get_species()
        print(' get_species: => ', strRet)
        assert(strRet == strString)


def test_set_coordSys(cammice):
    testSystems = ["GDZ", "GEO", "GSM", "GSE", "SM", "GEI", "ECI", "MAG",
                   "SPH", "RLL"]  # initial list of accepted values
    badSystems = ["", "ABC", "XYZ"]  # list of bad values to test
    testSystems += badSystems  # append bad values

    testUnits = ["", "KM", "RE"]
    badUnits = ["MI", "FT", "CM"]
    testUnits += badUnits  # same as systems

    for strSys in testSystems:
        for strUnit in testUnits:
            ierr = cammice.set_coordSys(strSys, strUnit)
            print(' set_coordSys: input={', strSys,
                  ',', strUnit, '} => ', ierr)

            retSys = cammice.get_coordSys()
            print(' get_coordSys: => ', retSys)
            retSysUnits = cammice.get_coordSysUnits()
            print(' get_coordSysUnits: => ', retSysUnits)

            if strSys in badSystems or strUnit in badUnits:
                assert(ierr < 0)

            else:
                assert(ierr == 0)

                # ECI and GEI are the same
                if strSys == "ECI":
                    assert(retSys == "GEI")
                else:
                    assert(retSys == strSys)

                # GDZ is only in KM
                if strSys == "GDZ":
                    assert(retSysUnits == "KM")
                    # unit defaults to RE if not specified
                elif strUnit == "":
                    assert(retSysUnits == 'RE')
                else:
                    assert(retSysUnits == strUnit)


def test_set_ephemeris(cammice):
    # test 1 - assert fail
    testVals = [np.array([0.0]), np.array([0.0, 1.0, 2.0]),
                np.array([1.0]), np.array([0.0, 1.0])]
    ierr = cammice.set_ephemeris(
        testVals[0], testVals[1], testVals[2], testVals[3])
    print(' set_ephemeris: testVals=', testVals, ' => ', ierr)
    assert(ierr < 0)  # array lengths do not match

    numEphems = cammice.get_numEphemeris()
    print(' get_numEphemeris: => ', numEphems)
    assert(numEphems == 0)

    # test 3 - assert pass
    testVals = [np.array([0.0, 1.0, 2.0]), np.array([0.0, 1.0, 2.0]),
                np.array([0.0, 0.0, 0.0]), np.array([999.0, 999.0, 999.0])]
    ierr = cammice.set_ephemeris(
        testVals[0], testVals[1], testVals[2], testVals[3])
    print(' set_ephemeris: testVals=', testVals, ' => ', ierr)
    assert(ierr == 0)  # good input of length 3

    numEphems = cammice.get_numEphemeris()
    print(' get_numEphemeris: => ', numEphems)
    assert(numEphems == 3)

    # test 4 - assert pass
    testVals = [np.array([0.0]), np.array(
        [0.0]), np.array([0.0]), np.array([0.0])]
    ierr = cammice.set_ephemeris(
        testVals[0], testVals[1], testVals[2], testVals[3])
    print(' set_ephemeris: testVals=', testVals, ' => ', ierr)
    assert(ierr == 0)  # good input of length 1

    numEphems = cammice.get_numEphemeris()
    print(' get_numEphemeris: => ', numEphems)
    assert(numEphems == 1)

    # test 5 - Should this be a pass?
    testVals = [np.array([]), np.array([]), np.array([]), np.array([])]
    ierr = cammice.set_ephemeris(
        testVals[0], testVals[1], testVals[2], testVals[3])
    print(' set_ephemeris: testVals=', testVals, ' => ', ierr)
    assert(ierr == 0)  # Is this good input? Seems to be accepted by the C++ code

    numEphems = cammice.get_numEphemeris()
    print(' get_numEphemeris: => ', numEphems)
    assert(numEphems == 0)


def test_computeFlux(cammice):

    # use ephem model to generate test inputs
    ephem = EphemModel()

    print('ephem eHandle=', ephem.zHandle)
    strFile = '../../modelData/igrfDB.h5'
    ierr = ephem.set_magfieldDBFile(strFile)
    print(' set_magfieldDBFile: dbFile=', strFile, ' => ', ierr)
    assert(ierr == 0)

    assert(ierr == 0)
    dValue = 210.0
    ierr = ephem.set_rightAscension(dValue)
    print(' set_rightAscension: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = 25.0
    ierr = ephem.set_argOfPerigee(dValue)
    print(' set_argOfPerigee: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = 12000.0
    ierr = ephem.set_altitudeOfApogee(dValue)
    print(' set_altitudeOfApogee: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = 8000.0
    ierr = ephem.set_altitudeOfPerigee(dValue)
    print(' set_altitudeOfPerigee: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = 45.0
    ierr = ephem.set_meanAnomaly(dValue)
    print(' set_meanAnomaly: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = 2.5
    ierr = ephem.set_meanMotion(dValue)
    print(' set_meanMotion: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = 30.0
    ierr = ephem.set_inclination(dValue)
    print(' set_inclination: dValue=', dValue, ' => ', ierr)
    dMjd = 58119.5
    ierr = ephem.set_elementTime(dMjd)
    print(' set_elementTime: dMjd=', dMjd, ' => ', ierr)
    assert(ierr == 0)
    dStep = 60.0
    dStart = 58120.0
    dEnd = 58120.25
    ierr = ephem.set_times(dStart, dEnd, dStep)
    print(' set_times: start=%.1f, end=%.1f, step=%.0f => %i' %
          (dStart, dEnd, dStep, ierr))
    assert(ierr == 0)

    # get about two orbits of ephemeris
    strCoordSys = 'GDZ'
    strCoordUnits = 'km'
    iNum, daTimes, daCoord1, daCoord2, daCoord3 \
        = ephem.compute(strCoordSys, strCoordUnits)
    print(' compute: CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTimes)=',
          len(daTimes), ', len(daCoord1)=', len(daCoord1))
    assert(iNum > 0)
    del ephem

    # set environment with this ephemeris
    ierr = cammice.set_coordSys(strCoordSys, strCoordUnits)
    print(' set_coordSys: CoordSys=', strCoordSys, strCoordUnits, ' => ', ierr)
    assert(ierr == 0)

    # setting up parameters for model
    strString = "all"
    ierr = cammice.set_dataFilter(strString)
    print(' set_dataFilter: strString=', strString, ' => ', ierr)
    assert(ierr == 0)

    ierr = cammice.set_ephemeris(daTimes, daCoord1, daCoord2, daCoord3)
    print(' set_ephemeris: => ', ierr)
    assert(ierr == 0)

    numEphems = cammice.get_numEphemeris()
    print(' get_numEphemeris: => ', numEphems)
    assert(numEphems == iNum)

    strString = "../../modelData/cammiceDB.h5"
    ierr = cammice.set_modelDBFile(strString)
    print(' set_modelDBFile: strString=', strString, ' => ', ierr)
    assert(ierr == 0)

    strString = "../../modelData/igrfDB.h5"
    ierr = cammice.set_magfieldDBFile(strString)
    print(' set_magfieldDBFile: strString=', strString, ' => ', ierr)
    assert(ierr == 0)

    strString = "igrfop"
    ierr = cammice.set_magfieldModel(strString)
    print(' set_magfieldModel: strString=', strString, ' => ', ierr)
    assert(ierr == 0)

    # first run parameters - comparing different pitch angle bins
    strSpec = "H+"
    strPitch = "omni"
    ierr = cammice.set_species(strSpec)
    print(' set_species: strString=', strSpec, ' => ', ierr)
    assert(ierr == 0)
    ierr = cammice.set_pitchAngleBin(strPitch)
    print(' set_pitchAngleBin: strString=', strPitch, ' => ', ierr)
    assert(ierr == 0)

    # compute and output results
    ierr, da2FluxData = cammice.computeFlux()
    print(' computeFluxMean: ierr=', ierr,
          ' da2Flux shape=', da2FluxData.shape)
    assert(ierr == 0)
    assert(da2FluxData.shape[0] == iNum)
    assert(da2FluxData.shape[1] == 12)
    print('   [   0 ]: ', daTimes[0], daCoord1[0], daCoord2[0], daCoord3[0])
    print('     ', da2FluxData[0][0], da2FluxData[0]
          [1], da2FluxData[0][10], da2FluxData[0][11])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    print('     ', da2FluxData[iNum-1][0], da2FluxData[iNum-1]
          [1], da2FluxData[iNum-1][10], da2FluxData[iNum-1][11])

    assert(ierr == 0)
    assert(da2FluxData.shape == (iNum, 12))

    # second run parameters
    strSpec = "H+"
    strPitch = "80-90"
    ierr = cammice.set_species(strSpec)
    print(' set_species: strString=', strSpec, ' => ', ierr)
    assert(ierr == 0)
    ierr = cammice.set_pitchAngleBin(strPitch)
    print(' set_pitchAngleBin: strString=', strPitch, ' => ', ierr)
    assert(ierr == 0)

    # compute and output results
    ierr, da2FluxData2 = cammice.computeFlux()
    print(' computeFluxMean: ierr=', ierr,
          ' da2Flux shape=', da2FluxData.shape)
    assert(ierr == 0)
    assert(da2FluxData2.shape[0] == iNum)
    assert(da2FluxData2.shape[1] == 12)
    print('   [   0 ]: ', daTimes[0], daCoord1[0], daCoord2[0], daCoord3[0])
    print('     ', da2FluxData2[0][0], da2FluxData2[0]
          [1], da2FluxData2[0][10], da2FluxData2[0][11])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    print('     ', da2FluxData2[iNum-1][0], da2FluxData2[iNum-1]
          [1], da2FluxData2[iNum-1][10], da2FluxData2[iNum-1][11])

    assert(ierr == 0)
    assert(da2FluxData2.shape == (iNum, 12))

    # does not match the fist run
    for i in range(iNum-1):
        for j in range(11):
            assert(da2FluxData2[i][j] == -1 or da2FluxData[i][j] == -1
                   or da2FluxData2[i][j] != da2FluxData[i][j])

    # third run parameters
    strSpec = "H+"
    strPitch = "90-100"
    ierr = cammice.set_species(strSpec)
    print(' set_species: strString=', strSpec, ' => ', ierr)
    assert(ierr == 0)
    ierr = cammice.set_pitchAngleBin(strPitch)
    print(' set_pitchAngleBin: strString=', strPitch, ' => ', ierr)
    assert(ierr == 0)

    # compute and output results
    ierr, da2FluxData3 = cammice.computeFlux()
    print(' computeFluxMean: ierr=', ierr,
          ' da2Flux shape=', da2FluxData.shape)
    assert(ierr == 0)
    assert(da2FluxData3.shape[0] == iNum)
    assert(da2FluxData3.shape[1] == 12)
    print('   [   0 ]: ', daTimes[0], daCoord1[0], daCoord2[0], daCoord3[0])
    print('     ', da2FluxData3[0][0], da2FluxData3[0]
          [1], da2FluxData3[0][10], da2FluxData3[0][11])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    print('     ', da2FluxData3[iNum-1][0], da2FluxData3[iNum-1]
          [1], da2FluxData3[iNum-1][10], da2FluxData3[iNum-1][11])

    assert(ierr == 0)
    assert(da2FluxData2.shape == (iNum, 12))

    # does not match the first or second runs
    for i in range(iNum-1):
        for j in range(11):
            assert(da2FluxData3[i][j] == -1 or da2FluxData2[i][j] == -1
                   or da2FluxData3[i][j] != da2FluxData2[i][j])
            assert(da2FluxData3[i][j] == -1 or da2FluxData[i][j] == -1
                   or da2FluxData3[i][j] != da2FluxData[i][j])

    # first run parameters - comparing different species
    strSpec = "H+"
    strPitch = "80-90"
    ierr = cammice.set_species(strSpec)
    print(' set_species: strString=', strSpec, ' => ', ierr)
    assert(ierr == 0)
    ierr = cammice.set_pitchAngleBin(strPitch)
    print(' set_pitchAngleBin: strString=', strPitch, ' => ', ierr)
    assert(ierr == 0)

    # compute and output results
    ierr, da2FluxData = cammice.computeFlux()
    print(' computeFluxMean: ierr=', ierr,
          ' da2Flux shape=', da2FluxData.shape)
    assert(ierr == 0)
    assert(da2FluxData.shape[0] == iNum)
    assert(da2FluxData.shape[1] == 12)
    print('   [   0 ]: ', daTimes[0], daCoord1[0], daCoord2[0], daCoord3[0])
    print('     ', da2FluxData[0][0], da2FluxData[0]
          [1], da2FluxData[0][10], da2FluxData[0][11])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    print('     ', da2FluxData[iNum-1][0], da2FluxData[iNum-1]
          [1], da2FluxData[iNum-1][10], da2FluxData[iNum-1][11])

    assert(ierr == 0)
    assert(da2FluxData.shape == (iNum, 12))

    # second run parameters
    strSpec = "He+"
    strPitch = "80-90"
    ierr = cammice.set_species(strSpec)
    print(' set_species: strString=', strSpec, ' => ', ierr)
    assert(ierr == 0)
    ierr = cammice.set_pitchAngleBin(strPitch)
    print(' set_pitchAngleBin: strString=', strPitch, ' => ', ierr)
    assert(ierr == 0)

    # compute and output results
    ierr, da2FluxData2 = cammice.computeFlux()
    print(' computeFluxMean: ierr=', ierr,
          ' da2Flux shape=', da2FluxData.shape)
    assert(ierr == 0)
    assert(da2FluxData2.shape[0] == iNum)
    assert(da2FluxData2.shape[1] == 12)
    print('   [   0 ]: ', daTimes[0], daCoord1[0], daCoord2[0], daCoord3[0])
    print('     ', da2FluxData2[0][0], da2FluxData2[0]
          [1], da2FluxData2[0][10], da2FluxData2[0][11])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    print('     ', da2FluxData2[iNum-1][0], da2FluxData2[iNum-1]
          [1], da2FluxData2[iNum-1][10], da2FluxData2[iNum-1][11])

    assert(ierr == 0)
    assert(da2FluxData2.shape == (iNum, 12))

    # does not match the fist run
    for i in range(iNum-1):
        for j in range(11):
            assert(da2FluxData2[i][j] == -1 or da2FluxData[i][j] == -1
                   or da2FluxData2[i][j] != da2FluxData[i][j])

    # third run parameters
    strSpec = "O+"
    strPitch = "80-90"
    ierr = cammice.set_species(strSpec)
    print(' set_species: strString=', strSpec, ' => ', ierr)
    assert(ierr == 0)
    ierr = cammice.set_pitchAngleBin(strPitch)
    print(' set_pitchAngleBin: strString=', strPitch, ' => ', ierr)
    assert(ierr == 0)

    # compute and output results
    ierr, da2FluxData3 = cammice.computeFlux()
    print(' computeFluxMean: ierr=', ierr,
          ' da2Flux shape=', da2FluxData.shape)
    assert(ierr == 0)
    assert(da2FluxData3.shape[0] == iNum)
    assert(da2FluxData3.shape[1] == 12)
    print('   [   0 ]: ', daTimes[0], daCoord1[0], daCoord2[0], daCoord3[0])
    print('     ', da2FluxData3[0][0], da2FluxData3[0]
          [1], da2FluxData3[0][10], da2FluxData3[0][11])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    print('     ', da2FluxData3[iNum-1][0], da2FluxData3[iNum-1]
          [1], da2FluxData3[iNum-1][10], da2FluxData3[iNum-1][11])

    assert(ierr == 0)
    assert(da2FluxData2.shape == (iNum, 12))

    # does not match the first or second runs
    for i in range(iNum-1):
        for j in range(11):
            assert(da2FluxData3[i][j] == -1 or da2FluxData2[i][j] == -1
                   or da2FluxData3[i][j] != da2FluxData2[i][j])
            assert(da2FluxData3[i][j] == -1 or da2FluxData[i][j] == -1
                   or da2FluxData3[i][j] != da2FluxData[i][j])

    # first run parameters - comparing completely different parameters
    strSpec = "O+"
    strPitch = "omni"
    ierr = cammice.set_species(strSpec)
    print(' set_species: strString=', strSpec, ' => ', ierr)
    assert(ierr == 0)
    ierr = cammice.set_pitchAngleBin(strPitch)
    print(' set_pitchAngleBin: strString=', strPitch, ' => ', ierr)
    assert(ierr == 0)

    # compute and output results
    ierr, da2FluxData = cammice.computeFlux()
    print(' computeFluxMean: ierr=', ierr,
          ' da2Flux shape=', da2FluxData.shape)
    assert(ierr == 0)
    assert(da2FluxData.shape[0] == iNum)
    assert(da2FluxData.shape[1] == 12)
    print('   [   0 ]: ', daTimes[0], daCoord1[0], daCoord2[0], daCoord3[0])
    print('     ', da2FluxData[0][0], da2FluxData[0]
          [1], da2FluxData[0][10], da2FluxData[0][11])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    print('     ', da2FluxData[iNum-1][0], da2FluxData[iNum-1]
          [1], da2FluxData[iNum-1][10], da2FluxData[iNum-1][11])

    assert(ierr == 0)
    assert(da2FluxData.shape == (iNum, 12))

    # second run parameters
    strSpec = "He+"
    strPitch = "90-100"
    ierr = cammice.set_species(strSpec)
    print(' set_species: strString=', strSpec, ' => ', ierr)
    assert(ierr == 0)
    ierr = cammice.set_pitchAngleBin(strPitch)
    print(' set_pitchAngleBin: strString=', strPitch, ' => ', ierr)
    assert(ierr == 0)

    # compute and output results
    ierr, da2FluxData2 = cammice.computeFlux()
    print(' computeFluxMean: ierr=', ierr,
          ' da2Flux shape=', da2FluxData.shape)
    assert(ierr == 0)
    assert(da2FluxData2.shape[0] == iNum)
    assert(da2FluxData2.shape[1] == 12)
    print('   [   0 ]: ', daTimes[0], daCoord1[0], daCoord2[0], daCoord3[0])
    print('     ', da2FluxData2[0][0], da2FluxData2[0]
          [1], da2FluxData2[0][10], da2FluxData2[0][11])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    print('     ', da2FluxData2[iNum-1][0], da2FluxData2[iNum-1]
          [1], da2FluxData2[iNum-1][10], da2FluxData2[iNum-1][11])

    assert(ierr == 0)
    assert(da2FluxData2.shape == (iNum, 12))

    # does not match the fist run
    for i in range(iNum-1):
        for j in range(11):
            assert(da2FluxData2[i][j] == -1 or da2FluxData[i][j] == -1
                   or da2FluxData2[i][j] <= da2FluxData[i][j])
