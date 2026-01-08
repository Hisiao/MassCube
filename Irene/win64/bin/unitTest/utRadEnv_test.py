
# enable use of python3 print function form in python 2.x
from __future__ import print_function

# for the 'import irene_defs' to be successful, either:
#     specify the path to the Ae9Ap9 root directory here:
#sys.path.append(os.environ['HOME']+'/Ae9Ap9')
#     or in the PYTHONPATH environement variable
#
# load definitions for required IRENE environment variables
import irene_defs

from radenvModel import RadEnvModel
from ephemModel import EphemModel
import numpy as np
import pytest


@pytest.fixture
def radenv():
    radenv = RadEnvModel()
    print('radenv zHandle=', radenv.zHandle)
    return radenv


def test_set_model( radenv ):
    # failing tests
    for strString in ["AE9", "AP9", "AX8", "CRRES", ""]:
        ierr = radenv.set_model(strString)
        print(' set_model: strString=', strString, ' => ', ierr)
        assert(ierr < 0)

        model = radenv.get_model()
        print(' get_model: => ', model)
        assert(model == "none")

    # passing tests
    for strString in ["AE-8", "AE8", "AP-8", "AP8", "CRRESELE", "CRRES-ELE",
                      "CRRESPRO", "CRRES-PRO"]:
        ierr = radenv.set_model(strString)
        print(' set_model: strString=', strString, ' => ', ierr)
        assert(ierr == 0)

    model = radenv.get_model()
    print(' get_model: => ', model)
    assert(model == "CRRESPRO")


def test_set_modelDBDir( radenv ):

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
    ierr = radenv.set_ephemeris( daTime, daCoord1, daCoord2, daCoord3)
    print(' radenv.set_ephemeris: => ', ierr)
    assert(ierr == 0)  # good input of length 1

    numEphems = radenv.get_numEphemeris()
    print(' radenv.get_numEphemeris: => ', numEphems)
    assert(numEphems > 0)
    testVals = np.array([1.0, 5.0, 15.0, 25.0, 50.0, 75.0, 100.0])
    ierr = radenv.set_energies(testVals)
    print(' set_energies: testVals=', testVals, ' => ', ierr)
    assert(ierr == 0)
    numEnergies = radenv.get_numEnergies()
    print(' get_numEnergies: => ', numEnergies)
    assert(numEnergies == len(testVals))
    strString = "INTEGRAL"
    ierr = radenv.set_fluxType(strString)
    print(' set_fluxType: strString=', strString, ' => ', ierr)
    assert(ierr == 0)
    fluxType = radenv.get_fluxType()
    print(' get_fluxType: => ', fluxType)
    assert(fluxType == "Integral") # standardized

##    ierr = radenv.set_model('Ap8')
##    assert(ierr == 0)
    for strString in ["goober", "../modelData", "$DATADIR/"]:
        ierr = radenv.set_modelDBDir(strString)
        print(' set_modelDBDir: strString=', strString, ' => ', ierr)
        assert(ierr < 0)

        strRet = radenv.get_modelDBDir()
        print(' get_modelDBDir: => ', strRet)
        assert(strRet == "")

    print(' --model DB *prior* to compute call:')
    print(' get_model: => ', radenv.get_model())
    print(' get_modelDBFile: => ', radenv.get_modelDBFile())
    print(' get_magfieldDBFile: => ', radenv.get_magfieldDBFile())
    # compute and output results
    ierr, da2FluxData = radenv.computeFlux()
    print(' computeFlux: ierr=', ierr,
          ' da2Flux shape=', da2FluxData.shape)
    assert(ierr < 0)

    for strString in ["../../kernelXml/","../../modelData/"]:
        ierr = radenv.set_modelDBDir(strString)
        print(' set_modelDBDir: strString=', strString, ' => ', ierr)
        assert(ierr == 0)

        strRet = radenv.get_modelDBDir()
        print(' get_modelDBDir: => ', strRet)
        assert(strRet == strString)

        print(' --model DB *prior* to compute call:')
        print(' get_model: => ', radenv.get_model())
        print(' get_modelDBFile: => ', radenv.get_modelDBFile())
        print(' get_magfieldDBFile: => ', radenv.get_magfieldDBFile())
        # compute and output results
        ierr, da2FluxData = radenv.computeFlux()
        print(' computeFlux: ierr=', ierr,
              ' da2Flux shape=', da2FluxData.shape)
        assert(ierr < 0)
        print(' get_magfieldDBFile: => ', radenv.get_magfieldDBFile())
        ierr = radenv.set_model('Ap8')
        assert(ierr == 0)
        print(' get_model: => ', radenv.get_model())
        # compute and output results
        ierr, da2FluxData = radenv.computeFlux()
        print(' computeFlux: ierr=', ierr,
              ' da2Flux shape=', da2FluxData.shape)
        if strString == "../../modelData/":
          assert (ierr == 0)
        else:
          assert(ierr < 0)
          ierr = radenv.set_model('CRRESele')
          assert(ierr == 0)

    print('-- automatically defined via ModelDBDir:')
    print(' get_modelDBFile: => ', radenv.get_modelDBFile())
    print(' get_magfieldDBFile: => ', radenv.get_magfieldDBFile())
    print(' get_shielddose2DBFile: => ', radenv.get_shielddose2DBFile())

def test_set_modelDBFile( radenv ):
    # failing tests
    for strString in ["radiationBeltDB.h5", "../modelData/radiationBeltDB.h5", "$DATADIR/radiationBeltDB.h5"]:
        ierr = radenv.set_modelDBFile(strString)
        print(' set_modelDBFile: strString=', strString, ' => ', ierr)
        assert(ierr < 0)

        modelDB = radenv.get_modelDBFile()
        print(' get_modelDBFile: => ', modelDB)
        assert(modelDB == "UNDEF")

    # passing tests
    for strString in ["../../modelData/radiationBeltDB.h5"]:
        ierr = radenv.set_modelDBFile(strString)
        print(' set_modelDBFile: strString=', strString, ' => ', ierr)
        assert(ierr == 0)

        modelDB = radenv.get_modelDBFile()
        print(' get_modelDBFile: => ', modelDB)
        assert(modelDB == strString)


def test_set_magfieldDBFile( radenv ):
    # put in a valid file, so to fully test reset on invalid one
    ierr = radenv.set_magfieldDBFile('../../modelData/igrfDB.h5')
    assert(ierr == 0)

    # failing tests
    for strString in ["igrfDB.h5", "../modelData/igrfDB.h5", "$DATADIR/igrfDB.h5"]:
        ierr = radenv.set_magfieldDBFile(strString)
        print(' set_modelDBFile: strString=', strString, ' => ', ierr)
        assert(ierr < 0)

        magfieldDB = radenv.get_magfieldDBFile()
        print(' get_magfieldDBFile: => ', magfieldDB)
        assert(magfieldDB == "UNDEF")

    # passing tests
    for strString in ["../../modelData/igrfDB.h5"]:
        ierr = radenv.set_magfieldDBFile(strString)
        print(' set_magfieldDBFile: strString=', strString, ' => ', ierr)
        assert(ierr == 0)

        magfieldDB = radenv.get_magfieldDBFile()
        print(' get_magfieldDBFile: => ', magfieldDB)
        assert(magfieldDB == strString)


def test_set_shielddose2DBFile( radenv ):
    # failing tests
    for strString in ["sd2DB.h5", "../modelData/sd2DB.h5", "$DATADIR/sd2DB.h5"]:
        ierr = radenv.set_shielddose2DBFile(strString)
        print(' set_shielddose2DBFile: strString=', strString, ' => ', ierr)
        assert(ierr < 0)

        shielddose2DB = radenv.get_shielddose2DBFile()
        print(' get_shielddose2DBFile: => ', shielddose2DB)
        assert(shielddose2DB == "UNDEF")

    # passing tests
    for strString in ["../../modelData/sd2DB.h5"]:
        ierr = radenv.set_shielddose2DBFile(strString)
        print(' set_shielddose2DBFile: strString=', strString, ' => ', ierr)
        assert(ierr == 0)

        shielddose2DB = radenv.get_shielddose2DBFile()
        print(' get_shielddose2DBFile: => ', shielddose2DB)
        assert(shielddose2DB == strString)


def test_set_actLevel( radenv ):
    # failing tests
    for strString in ["", "MEDIUM", "NONE"]:
        ierr = radenv.set_actLevel(strString)
        print(' set_actLevel: strString=', strString, ' => ', ierr)
        assert(ierr < 0)

        actLevel = radenv.get_actLevel()
        print(' get_actLevel: => ', actLevel)
        assert(actLevel == "QUIET")

    # passing tests
    for strString in ["QUIET", "MIN", "ACTIVE", "MAX"]:
        ierr = radenv.set_actLevel(strString)
        print(' set_actLevel: strString=', strString, ' => ', ierr)
        assert(ierr == 0)

    actLevel = radenv.get_actLevel()
    print(' get_actLevel: => ', actLevel)
    assert(actLevel == "ACTIVE")


def test_set_actRange( radenv ):
    # failing tests
    for strString in ["", "min", "none", "0"]:
        ierr = radenv.set_actRange(strString)
        print(' set_actRange: strString=', strString, ' => ', ierr)
        assert(ierr < 0)

        actRange = radenv.get_actRange()
        print(' get_actRange: => ', actRange)
        assert(actRange == "5-7.5")

    # passing tests
    for strString in ["5-7.5", "7.5-10", "10-15", "15-20", "20-25", ">25",
                      "avg", "max", "all"]:
        ierr = radenv.set_actRange(strString)
        print(' set_actRange: strString=', strString, ' => ', ierr)
        assert(ierr == 0)

        actRange = radenv.get_actRange()
        print(' get_actRange: => ', actRange)
        assert(actRange == strString)


def test_set_15dayAp( radenv ):
    # failing tests
    for testVal in [-1.0, -0.1, 400.1, 9999]:
        ierr = radenv.set_15dayAp(testVal)
        print(' set_15dayAp: testVal=', testVal, ' => ', ierr)
        assert(ierr < 0)

        dayAp = radenv.get_15dayAp()
        print(' get_15dayAp: => ', dayAp)
        assert(dayAp == -1.0)

    # passing tests
    for testVal in [0.0, 5.0, 6.1, 100.0, 400]:
        ierr = radenv.set_15dayAp(testVal)
        print(' set_actRange: testVal=', testVal, ' => ', ierr)
        assert(ierr == 0)

        dayAp = radenv.get_15dayAp()
        print(' get_15dayAp: => ', dayAp)
        assert(dayAp == testVal)


def test_set_fixedEpoch( radenv ):
    # Accepts a boolean value. Any non-zero numerical input is treated as true.
    for testVal in [True, False, 0, 1, 5.1]:
        ierr = radenv.set_fixedEpoch(testVal)
        print(' set_fixedEpoch: testVal=', testVal, ' => ', ierr)
        assert(ierr == 0)

    fixedEpoch = radenv.get_fixedEpoch()
    print(' get_fixedEpoch: => ', fixedEpoch)
    assert(fixedEpoch == 1.0)


def test_set_shiftSAA( radenv ):
    # Accepts a boolean value. Any non-zero numerical input is treated as true.
    for testVal in [True, False, 0, 1, 5.1]:
        ierr = radenv.set_shiftSAA(testVal)
        print(' set_shiftSAA: testVal=', testVal, ' => ', ierr)
        assert(ierr == 0)

    shiftSAA = radenv.get_shiftSAA()
    print(' get_shiftSAA: => ', shiftSAA)
    assert(shiftSAA == 1.0)


def test_set_fluxType( radenv ):
    # failing tests
    for strString in ["NONE", "ALT", ""]:
        ierr = radenv.set_fluxType(strString)
        print(' set_fluxType: strString=', strString, ' => ', ierr)
        assert(ierr < 0)

        fluxType = radenv.get_fluxType()
        print(' get_fluxType: => ', fluxType)
        assert(fluxType == "none")

    # passing tests
    for strString in ["1PTDIFF", "DIFF", "DIFFER", "DIFFERENTIAL", "INTEGRAL", "INTEG", "INTG"]:
        ierr = radenv.set_fluxType(strString)
        print(' set_fluxType: strString=', strString, ' => ', ierr)
        assert(ierr == 0)

    fluxType = radenv.get_fluxType()
    print(' get_fluxType: => ', fluxType)
    assert(fluxType == "Integral")


def test_set_energies( radenv ):
    # failing tests
    for testVals in [np.array([0.0, 1.0, 2.0]), np.array([0.0]), np.array([])]:
        ierr = radenv.set_energies(testVals)
        print(' set_energies: testVals=', testVals, ' => ', ierr)
        assert(ierr < 0)

        numEnergies = radenv.get_numEnergies()
        print(' get_numEnergies: => ', numEnergies)
        assert(numEnergies == 0)

    # passing tests
    for testVals in [np.array([0.1, 1.0, 5.0]), np.array([1.0]),
                     np.array([1.0, 5.0, 25.0, 100.0])]:
        ierr = radenv.set_energies(testVals)
        print(' set_energies: testVals=', testVals, ' => ', ierr)
        assert(ierr == 0)

        numEnergies = radenv.get_numEnergies()
        print(' get_numEnergies: => ', numEnergies)
        assert(numEnergies == len(testVals))


def test_set_coordSys( radenv ):
    testSystems = ["GDZ", "GEO", "GSM", "GSE", "SM", "GEI", "ECI", "MAG",
                   "SPH", "RLL"]  # initial list of accepted values
    badSystems = ["", "ABC", "XYZ"]  # list of bad values to test
    testSystems += badSystems  # append bad values

    testUnits = ["", "KM", "RE"]
    badUnits = ["MI", "FT", "CM"]
    testUnits += badUnits  # same as systems

    for strSys in testSystems:
        for strUnit in testUnits:
            ierr = radenv.set_coordSys(strSys, strUnit)
            print(' set_coordSys: input={', strSys,
                  ',', strUnit, '} => ', ierr)

            retSys = radenv.get_coordSys()
            print(' get_coordSys: => ', retSys)
            retSysUnits = radenv.get_coordSysUnits()
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


def test_set_ephemeris( radenv ):
    # test 1 - assert fail
    testVals = [np.array([0.0]), np.array([0.0, 1.0, 2.0]),
                np.array([1.0]), np.array([0.0, 1.0])]
    ierr = radenv.set_ephemeris(
        testVals[0], testVals[1], testVals[2], testVals[3])
    print(' set_ephemeris: testVals=', testVals, ' => ', ierr)
    assert(ierr < 0)  # array lengths do not match

    numEphems = radenv.get_numEphemeris()
    print(' get_numEphemeris: => ', numEphems)
    assert(numEphems == 0)

    # test 2 - assert pass
    testVals = [np.array([0.0, 1.0, 2.0]), np.array([0.0, 1.0, 2.0]),
                np.array([0.0, 0.0, 0.0]), np.array([999.0, 999.0, 999.0])]
    ierr = radenv.set_ephemeris(
        testVals[0], testVals[1], testVals[2], testVals[3])
    print(' set_ephemeris: testVals=', testVals, ' => ', ierr)
    assert(ierr == 0)  # good input of length 3

    numEphems = radenv.get_numEphemeris()
    print(' get_numEphemeris: => ', numEphems)
    assert(numEphems == 3)

    # test 3 - assert pass
    testVals = [np.array([0.0]), np.array(
        [0.0]), np.array([0.0]), np.array([0.0])]
    ierr = radenv.set_ephemeris(
        testVals[0], testVals[1], testVals[2], testVals[3])
    print(' set_ephemeris: testVals=', testVals, ' => ', ierr)
    assert(ierr == 0)  # good input of length 1

    numEphems = radenv.get_numEphemeris()
    print(' get_numEphemeris: => ', numEphems)
    assert(numEphems == 1)

    # test 4 - Should this be a pass?
    testVals = [np.array([]), np.array([]), np.array([]), np.array([])]
    ierr = radenv.set_ephemeris(
        testVals[0], testVals[1], testVals[2], testVals[3])
    print(' set_ephemeris: testVals=', testVals, ' => ', ierr)
    assert(ierr == 0)  # Is this good input? Seems to be accepted by the C++ code

    numEphems = radenv.get_numEphemeris()
    print(' get_numEphemeris: => ', numEphems)
    assert(numEphems == 0)


def test_computeFlux( radenv ):

    strString = "../../modelData/igrfDB.h5"
    ierr = radenv.set_magfieldDBFile(strString)
    print(' set_magfieldDBFile: strString=', strString, ' => ', ierr)
    assert(ierr == 0)

    # use ephem model to generate test inputs
    ephem = EphemModel()

    print('ephem eHandle=', ephem.zHandle)
    strFile = '../../modelData/igrfDB.h5'
    ierr = ephem.set_magfieldDBFile(strFile)
    print(' set_magfieldDBFile: dbFile=', strFile, ' => ', ierr)
    assert(ierr == 0)

    dValue = 25.0
    ierr = ephem.set_inclination(dValue)
    print(' set_inclination: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = 0.25
    ierr = ephem.set_eccentricity(dValue)
    print(' set_eccentricity: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = 8.5
    ierr = ephem.set_meanMotion(dValue)
    print(' set_meanMotion: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = 218.5
    ierr = ephem.set_meanAnomaly(dValue)
    print(' set_meanAnomaly: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = 125.0
    ierr = ephem.set_rightAscension(dValue)
    print(' set_rightAscension: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
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
    daEnergies = np.array([0.1, 0.5, 1.0, 2.0, 3.5, 5.0, 8.0])
    iNumE = len(daEnergies)
    strFluxType = '1PtDiff'
    ierr = radenv.set_fluxType(strFluxType)
    print(' set_fluxType: ', strFluxType, ' => ', ierr)
    assert(ierr == 0)
    ierr = radenv.set_coordSys(strCoordSys, strCoordUnits)
    print(' set_coordSys: CoordSys=', strCoordSys, strCoordUnits, ' => ', ierr)
    assert(ierr == 0)

    ierr = radenv.set_energies(daEnergies)
    print(' set_energies: ', strFluxType, ' => ', ierr)
    assert(ierr == 0)

    numEnergies = radenv.get_numEnergies()
    print(' get_numEnergies: => ', numEnergies)
    assert(numEnergies == iNumE)

    ierr = radenv.set_ephemeris(daTimes, daCoord1, daCoord2, daCoord3)
    print(' set_ephemeris: => ', ierr)
    assert(ierr == 0)

    numEphems = radenv.get_numEphemeris()
    print(' get_numEphemeris: => ', numEphems)
    assert(numEphems == iNum)

    # set database and model
    strString = "../../modelData/radiationBeltDB.h5"
    ierr = radenv.set_modelDBFile(strString)
    print(' set_modelDBFile: strString=', strString, ' => ', ierr)
    assert(ierr == 0)
    radenv.set_model("AE8")

    # compute and output results
    ierr, da2FluxData = radenv.computeFlux()
    print(' computeFluxMean: ierr=', ierr,
          ' da2Flux shape=', da2FluxData.shape)
    assert(ierr == 0)
    assert(da2FluxData.shape[0] == iNum)
    assert(da2FluxData.shape[1] == iNumE)
    print('   [   0 ]: ', daTimes[0], daCoord1[0], daCoord2[0], daCoord3[0])
    print('     ', da2FluxData[0][0], da2FluxData[0]
          [1], da2FluxData[0][2], da2FluxData[0][iNumE-1])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    print('     ', da2FluxData[iNum-1][0], da2FluxData[iNum-1]
          [1], da2FluxData[iNum-1][2], da2FluxData[iNum-1][iNumE-1])

    assert(ierr == 0)
    assert(da2FluxData.shape == (iNum, iNumE))

