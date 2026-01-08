
# enable use of python3 print function form in python 2.x
from __future__ import print_function

# for the 'import irene_defs' to be successful, either:
#     specify the path to the Ae9Ap9 root directory here:
#sys.path.append(os.environ['HOME']+'/Ae9Ap9')
#     or in the PYTHONPATH environement variable
#
# load definitions for required IRENE environment variables
import irene_defs

from ae9ap9Model import Ae9Ap9Model
from ephemModel import EphemModel
import numpy as np
import pytest


@pytest.fixture
def ae9ap9():
    ae9ap9 = Ae9Ap9Model()
    print('ae9ap9 zHandle=', ae9ap9.zHandle)
    ae9ap9.set_modelDBFile("../../modelData/AE9V15_runtime_tables.mat")
    ae9ap9.set_kPhiDBFile("../../modelData/fastPhi_net.mat")
    ae9ap9.set_kHMinDBFile("../../modelData/fast_hmin_net.mat")
    ae9ap9.set_magfieldDBFile("../../modelData/igrfDB.h5")
    ierr = ae9ap9.load_modelDB()
    assert(ierr == 0)
    return ae9ap9


@pytest.fixture
def ephem():
    ephem = EphemModel()
    return ephem

# In general, each method is tested first with 'invalid' inputs, then valid ones.
# The associated 'get' methods are also tested, when available, in the same manner.
# Some also include tests with invalid 'handle' values for completeness.


def test_set_modelDBDir( ):

    ae9ap9 = Ae9Ap9Model()
    print('ae9ap9 zHandle=', ae9ap9.zHandle)

    # specify a valid (but incorrect) directory
    ae9ap9.set_modelDBDir("../../kernelXml/")
    print(' get_modelDBDir: => ', ae9ap9.get_modelDBDir() )
    # next, specify invalid directory names, showing setting is blanked out
    for strString in ["goober", "../modelData", "$DATADIR/"]:
        ierr = ae9ap9.set_modelDBDir(strString)
        print(' set_modelDBDir: strString=', strString, ' => ', ierr)
        assert(ierr < 0)

        strRet = ae9ap9.get_modelDBDir()
        print(' get_modelDBDir: => ', strRet)
        assert(strRet == "")

    for strString, strModel in zip(
               ["../../kernelXml/","../../modelData",".","../../modelData/"],
               ["Ae8","CRRESpro","PlasmaE","AP9"]):
        ierr = ae9ap9.set_modelDBDir(strString)
        print(' set_modelDBDir: strString=', strString, ' => ', ierr)
        assert(ierr == 0)

        strRet = ae9ap9.get_modelDBDir()
        print(' get_modelDBDir: => ', strRet)

        print(' --model DB *prior* to load_modelDB call:')
        print(' get_model: => ', ae9ap9.get_model())
        print(' get_modelDBFile: => ', ae9ap9.get_modelDBFile())
        print(' get_kPhiDBFile: => ', ae9ap9.get_kPhiDBFile())
        print(' get_kHMinDBFile: => ', ae9ap9.get_kHMinDBFile())
        print(' get_magfieldDBFile: => ', ae9ap9.get_magfieldDBFile())
        # load db files; skip load on PlasmaE, since only one db load
        if ae9ap9.get_model() != "PLASMAE":
          ierr = ae9ap9.load_modelDB()
          print(' load_modelDB: ierr=', ierr)
          assert(ierr < 0)
        ierr = ae9ap9.set_model( strModel )
        print(' set_model(',strModel,') => ', ierr)
        if strModel == "AP9" or strModel == "PlasmaE":
          assert(ierr==0)
        else:
          assert(ierr < 0)
        print(' get_model: => ', ae9ap9.get_model())
        # compute and output results
        ierr = ae9ap9.load_modelDB()
        print(' load_modelDB: ierr=', ierr)
        if strModel == "AP9" or (strModel == "PlasmaE" and strString=="../../modelData/"):
          assert(ierr==0)
        else:
          assert(ierr < 0)

    print('-- automatically defined via ModelDBDir:')
    print(' get_modelDBFile: => ', ae9ap9.get_modelDBFile())
    print(' get_kPhiDBFile: => ', ae9ap9.get_kPhiDBFile())
    print(' get_kHMinDBFile: => ', ae9ap9.get_kHMinDBFile())
    print(' get_magfieldDBFile: => ', ae9ap9.get_magfieldDBFile())

def test_set_modelDBFile():

    ae9ap9 = Ae9Ap9Model()

    ierr = ae9ap9.test_badHandle()
    print(' test_badHandle => ', ierr)
    assert(ierr < 0)

    for strFile in ['AE9V15_runtime_tables.mat', '$DATADIR/AE9V15_runtime_tables.mat']:
        ierr = ae9ap9.set_modelDBFile(strFile)
        print(' set_modelDBFile: dbFile=', strFile, ' => ', ierr)
        assert(ierr < 0)

    strFile = ae9ap9.get_modelDBFile()
    print(' get_modelDBFile: returned file=', strFile)
    assert(strFile == "UNDEF")

    for strFile in ['../../modelData/AE9V15_runtime_tables.mat']:
        ierr = ae9ap9.set_modelDBFile(strFile)
        print(' set_modelDBFile: dbFile=', strFile, ' => ', ierr)
        assert(ierr == 0)

    strFile = ae9ap9.get_modelDBFile()
    print(' get_modelDBFile: returned file=', strFile)
    assert(strFile == '../../modelData/AE9V15_runtime_tables.mat')


def test_set_kPhiDBFile():

    ae9ap9 = Ae9Ap9Model()

    for strFile in ['fastPhi_net.mat', '$DATADIR/fastPhi_net.mat']:
        ierr = ae9ap9.set_kPhiDBFile(strFile)
        print(' set_kPhiDBFile: dbFile=', strFile, ' => ', ierr)
        assert(ierr < 0)
    strFile = ae9ap9.get_kPhiDBFile()
    print(' get_kPhiDBFile: returned file=', strFile)
    assert(strFile == "UNDEF")

    for strFile in ['../../modelData/fastPhi_net.mat']:
        ierr = ae9ap9.set_kPhiDBFile(strFile)
        print(' set_kPhiDBFile: dbFile=', strFile, ' => ', ierr)
        assert(ierr == 0)

    strFile = ae9ap9.get_kPhiDBFile()
    print(' get_kPhiDBFile: returned file=', strFile)
    assert(strFile == '../../modelData/fastPhi_net.mat')


def test_set_kHMinDBFile():

    ae9ap9 = Ae9Ap9Model()

    for strFile in ['fast_hmin_net.mat', '$DATADIR/fast_hmin_net.mat']:
        ierr = ae9ap9.set_kHMinDBFile(strFile)
        print(' set_kHMinDBFile: dbFile=', strFile, ' => ', ierr)
        assert(ierr < 0)
    strFile = ae9ap9.get_kHMinDBFile()
    print(' get_kHMinDBFile: returned file=', strFile)
    assert(strFile == "UNDEF")

    for strFile in ['../../modelData/fast_hmin_net.mat']:
        ierr = ae9ap9.set_kHMinDBFile(strFile)
        print(' set_kHMinDBFile: dbFile=', strFile, ' => ', ierr)
        assert(ierr == 0)

    strFile = ae9ap9.get_kHMinDBFile()
    print(' get_kHMinDBFile: returned file=', strFile)
    assert(strFile == '../../modelData/fast_hmin_net.mat')


def test_set_magfieldDBFile():

    ae9ap9 = Ae9Ap9Model()

    for strFile in ['igrfDB.h5', '$DATADIR/igrfDB.h5']:
        ierr = ae9ap9.set_magfieldDBFile(strFile)
        print(' set_magfieldDBFile: dbFile=', strFile, ' => ', ierr)
        assert(ierr < 0)
    strFile = ae9ap9.get_magfieldDBFile()
    print(' get_magfieldDBFile: returned file=', strFile)
    assert(strFile == "UNDEF")

    for strFile in ['../../modelData/igrfDB.h5']:
        ierr = ae9ap9.set_magfieldDBFile(strFile)
        print(' set_magfieldDBFile: dbFile=', strFile, ' => ', ierr)
        assert(ierr == 0)

    strFile = ae9ap9.get_magfieldDBFile()
    print(' get_magfieldDBFile: returned file=', strFile)
    assert(strFile == '../../modelData/igrfDB.h5')


def test_load_modelDB():
    ae9ap9 = Ae9Ap9Model()
    ae9ap9.set_modelDBFile("../../modelData/AE9V15_runtime_tables.mat")
    ae9ap9.set_kPhiDBFile("../../modelData/fastPhi_net.mat")
    ae9ap9.set_kHMinDBFile("../../modelData/fast_hmin_net.mat")
    ae9ap9.set_magfieldDBFile("../../modelData/igrfDB.h5")

    strString = ae9ap9.get_modelName()
    print(' get_modelName: returned modelName=', strString)
    assert(strString == 'DB not loaded')
    strString = ae9ap9.get_modelSpecies()
    print(' get_modelSpecies: returned modelSpecies=', strString)
    assert(strString == 'DB not loaded')
    ierr = ae9ap9.load_modelDB()
    print(' load_modelDB: => ', ierr)
    assert(ierr == 0)
    strString = ae9ap9.get_modelName()
    print(' get_modelName: returned modelName=', strString)
    assert(strString == 'AE9V15')
    strString = ae9ap9.get_modelSpecies()
    print(' get_modelSpecies: returned modelSpecies=', strString)
    assert(strString == 'e-')


def test_get_defaultPitchAngles(ae9ap9):
    daDefPitch = ae9ap9.get_defaultPitchAngles()
    print(' get_defaultPitchAngles: len(daDefPitch)=', len(daDefPitch))
    assert(len(daDefPitch) > 0)


def test_set_fluxEnvironOmni_computeFlux(ae9ap9, ephem):

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

    # cannot test with invalid energies - not caught at this level

    # set environment with this ephemeris
    #  - will fail on invalid flux type
    daEnergies = np.array([0.1, 0.5, 1.0, 2.0, 3.5, 5.0, 8.0])
    iNumE = len(daEnergies)
    strFluxType = 'Unspecified'
    ierr = ae9ap9.set_fluxEnvironOmni(strFluxType,
                                      daEnergies, daEnergies, daTimes, strCoordSys, strCoordUnits,
                                      daCoord1, daCoord2, daCoord3)
    print(' set_fluxEnvironOmni: ',
          ' FluxType=', strFluxType, ' Energies(#)=', iNumE,
          ' daTimes(#)=', len(daTimes), ', daCoord1(#)=', len(daCoord1),
          ' CoordSys=', strCoordSys, strCoordUnits)
    print('  returned ierr=', ierr)
    assert(ierr < 0)

    # set environment with this ephemeris
    strFluxType = 'Integral'
    # fail with invalid coord system
    strCoordSys = 'GPX'
    ierr = ae9ap9.set_fluxEnvironOmni(strFluxType,
                                      daEnergies, daEnergies, daTimes, strCoordSys, strCoordUnits,
                                      daCoord1, daCoord2, daCoord3)
    print(' set_fluxEnvironOmni: ',
          ' FluxType=', strFluxType, ' Energies(#)=', iNumE,
          ' daTimes(#)=', len(daTimes), ', daCoord1(#)=', len(daCoord1),
          ' CoordSys=', strCoordSys, strCoordUnits)
    print('  returned ierr=', ierr)
    assert(ierr < 0)

    iPerc = 125
    ierr, da3FluxData = ae9ap9.computeFluxPercentile(iPerc)
    print(' invalid call to computeFluxMean: iPerc=', iPerc, ' ierr=', ierr,
          ' da3Flux shape=', da3FluxData.shape)
    assert(ierr < 0)

    iPerc = 75
    ierr, da3FluxData = ae9ap9.computeFluxPercentile(iPerc)
    print(' premature call to computeFluxMean: iPerc=', iPerc, ' ierr=', ierr,
          ' da3Flux shape=', da3FluxData.shape)
    assert(ierr < 0)

    # try again, with valid flux type and coord system
    strFluxType = '1PtDiff'
    strCoordSys = 'GDZ'
    ierr = ae9ap9.set_fluxEnvironOmni(strFluxType,
                                      daEnergies, daEnergies, daTimes, strCoordSys, strCoordUnits,
                                      daCoord1, daCoord2, daCoord3)
    print(' set_fluxEnvironOmni: ',
          ' FluxType=', strFluxType, ' Energies(#)=', len(daEnergies),
          ' daTimes(#)=', len(daTimes), ', daCoord1(#)=', len(daCoord1),
          ' CoordSys=', strCoordSys, strCoordUnits)
    print('  returned ierr=', ierr)
    assert(ierr == 0)

    ierr, da2PitchAngles = ae9ap9.get_pitchAngles()
    print(' invalid call to get_pitchAngles: ierr=',ierr)
    assert(ierr < 0)

    ierr, da3FluxData = ae9ap9.computeFluxMean()
    print(' computeFluxMean: ierr=', ierr,
          ' da3Flux shape=', da3FluxData.shape)
    assert(ierr == 0)
    assert(da3FluxData.shape[0] == iNum)
    assert(da3FluxData.shape[1] == iNumE)
    assert(da3FluxData.shape[2] == 1)
    print('   [   0 ]: ', daTimes[0],
          daCoord1[0], daCoord2[0], daCoord3[0])
    print('     ', da3FluxData[0][0][0], da3FluxData[0][1]
          [0], da3FluxData[0][2][0], da3FluxData[0][iNumE-1][0])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    print('     ', da3FluxData[iNum-1][0][0], da3FluxData[iNum-1][1]
          [0], da3FluxData[iNum-1][2][0], da3FluxData[iNum-1][iNumE-1][0])

    iPerc = 75
    ierr, da3FluxData = ae9ap9.computeFluxPercentile(iPerc)
    print(' computeFluxMean: iPerc=', iPerc, ' ierr=', ierr,
          ' da3Flux shape=', da3FluxData.shape)
    assert(ierr == 0)
    assert(da3FluxData.shape[0] == iNum)
    assert(da3FluxData.shape[1] == iNumE)
    assert(da3FluxData.shape[2] == 1)
    print('   [   0 ]: ', daTimes[0],
          daCoord1[0], daCoord2[0], daCoord3[0])
    print('     ', da3FluxData[0][0][0], da3FluxData[0][1]
          [0], da3FluxData[0][2][0], da3FluxData[0][iNumE-1][0])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    print('     ', da3FluxData[iNum-1][0][0], da3FluxData[iNum-1][1]
          [0], da3FluxData[iNum-1][2][0], da3FluxData[iNum-1][iNumE-1][0])

    iPerc = 125
    ierr, da3FluxData = ae9ap9.computeFluxPercentile(iPerc)
    print(' computeFluxPercentile: iPerc=', iPerc, ' ierr=', ierr,
          ' da3Flux shape=', da3FluxData.shape)
    assert(ierr < 0)

    iPerc = 0
    ierr, da3FluxData = ae9ap9.computeFluxPercentile(iPerc)
    print(' computeFluxPercentile: iPerc=', iPerc, ' ierr=', ierr,
          ' da3Flux shape=', da3FluxData.shape)
    assert(ierr < 0)

    iScen = 0
    ierr, da3FluxData = ae9ap9.computeFluxPerturbedMean(iScen)
    print(' computeFluxPerturbedMean: iScen=', iScen,
          '; ierr=', ierr, ' da3Flux shape=', da3FluxData.shape)
    assert(ierr < 0)

    iScen = 10
    ierr, da3FluxData = ae9ap9.computeFluxPerturbedMean(iScen)
    print(' computeFluxPerturbedMean: iScen=', iScen,
          '; ierr=', ierr, ' da3Flux shape=', da3FluxData.shape)
    assert(ierr == 0)
    assert(da3FluxData.shape[0] == iNum)
    assert(da3FluxData.shape[1] == iNumE)
    assert(da3FluxData.shape[2] == 1)
    print('   [   0 ]: ', daTimes[0],
          daCoord1[0], daCoord2[0], daCoord3[0])
    print('     ', da3FluxData[0][0][0], da3FluxData[0][1]
          [0], da3FluxData[0][2][0], da3FluxData[0][iNumE-1][0])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    print('     ', da3FluxData[iNum-1][0][0], da3FluxData[iNum-1][1]
          [0], da3FluxData[iNum-1][2][0], da3FluxData[iNum-1][iNumE-1][0])

    iScen = 25
    dEpochTime = daTimes[0]
    ierr, da3FluxData = ae9ap9.computeFluxScenario(dEpochTime, iScen)
    print(' computeFluxScenario: dEpochTime=', dEpochTime, ', iScen=', iScen,
          '; ierr=', ierr, ' da3Flux shape=', da3FluxData.shape)
    assert(ierr == 0)
    assert(da3FluxData.shape[0] == iNum)
    assert(da3FluxData.shape[1] == iNumE)
    assert(da3FluxData.shape[2] == 1)
    print('   [   0 ]: ', daTimes[0],
          daCoord1[0], daCoord2[0], daCoord3[0])
    print('     ', da3FluxData[0][0][0], da3FluxData[0][1]
          [0], da3FluxData[0][2][0], da3FluxData[0][iNumE-1][0])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    print('     ', da3FluxData[iNum-1][0][0], da3FluxData[iNum-1][1]
          [0], da3FluxData[iNum-1][2][0], da3FluxData[iNum-1][iNumE-1][0])

    dEpochTime = daTimes[0]
    iScen = 512
    iPert = 1
    ierr, da3FluxData = ae9ap9.computeFluxScenario(dEpochTime, iScen, iPert)
    print(' computeFluxScenario: dEpochTime=', dEpochTime, ', iScen=', iScen,
          ', iPert=1; ierr=', ierr, ' da3Flux shape=', da3FluxData.shape)
    assert(ierr == 0)
    assert(da3FluxData.shape[0] == iNum)
    assert(da3FluxData.shape[1] == iNumE)
    assert(da3FluxData.shape[2] == 1)
    print('   [   0 ]: ', daTimes[0],
          daCoord1[0], daCoord2[0], daCoord3[0])
    print('     ', da3FluxData[0][0][0], da3FluxData[0][1]
          [0], da3FluxData[0][2][0], da3FluxData[0][iNumE-1][0])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    print('     ', da3FluxData[iNum-1][0][0], da3FluxData[iNum-1][1]
          [0], da3FluxData[iNum-1][2][0], da3FluxData[iNum-1][iNumE-1][0])


def test_set_fluxEnvironFixPitch_computeFlux(ae9ap9, ephem):

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

    # cannot test with invalid energies - not caught at this level

    # set environment with this ephemeris
    #  - will fail on invalid flux type
    daEnergies = np.array([0.1, 0.5, 1.0, 2.0, 3.5, 5.0, 8.0])
    iNumE = len(daEnergies)
    daPitchAngles = np.array([75., 85., 90.])
    iNumP = len(daPitchAngles)
    strFluxType = 'Unspecified'
    ierr = ae9ap9.set_fluxEnvironFixPitch(strFluxType,
                                          daEnergies, daEnergies, daTimes, strCoordSys, strCoordUnits,
                                          daCoord1, daCoord2, daCoord3, daPitchAngles)
    print(' set_fluxEnvironFixPitch: ',
          ' FluxType=', strFluxType, ' Energies(#)=', iNumE,
          ' daTimes(#)=', len(daTimes), ', daCoord1(#)=', len(daCoord1),
          ' CoordSys=', strCoordSys, strCoordUnits,
          ' PitchAngles=', daPitchAngles)
    print('  returned ierr=', ierr)
    assert(ierr < 0)

    # set environment with this ephemeris
    strFluxType = 'Integral'
    # fail with invalid coord system
    strCoordSys = 'GPX'
    ierr = ae9ap9.set_fluxEnvironFixPitch(strFluxType,
                                          daEnergies, daEnergies, daTimes, strCoordSys, strCoordUnits,
                                          daCoord1, daCoord2, daCoord3, daPitchAngles)
    print(' set_fluxEnvironFixPitch: ',
          ' FluxType=', strFluxType, ' Energies(#)=', iNumE,
          ' daTimes(#)=', len(daTimes), ', daCoord1(#)=', len(daCoord1),
          ' CoordSys=', strCoordSys, strCoordUnits,
          ' PitchAngles=', daPitchAngles)
    print('  returned ierr=', ierr)
    assert(ierr < 0)

    # try again, with valid flux type and coord system
    strFluxType = '1PtDiff'
    strCoordSys = 'GDZ'
    ierr = ae9ap9.set_fluxEnvironFixPitch(strFluxType,
                                          daEnergies, daEnergies, daTimes, strCoordSys, strCoordUnits,
                                          daCoord1, daCoord2, daCoord3, daPitchAngles)
    print(' set_fluxEnvironFixPitch: ',
          ' FluxType=', strFluxType, ' Energies(#)=', len(daEnergies),
          ' daTimes(#)=', len(daTimes), ', daCoord1(#)=', len(daCoord1),
          ' CoordSys=', strCoordSys, strCoordUnits,
          ' PitchAngles=', daPitchAngles)
    print('  returned ierr=', ierr)
    assert(ierr == 0)

    ierr, da3FluxData = ae9ap9.computeFluxMean()
    print(' computeFluxMean: ierr=', ierr,
          ' da3Flux shape=', da3FluxData.shape)
    assert(ierr == 0)
    assert(da3FluxData.shape[0] == iNum)
    assert(da3FluxData.shape[1] == iNumE)
    assert(da3FluxData.shape[2] == iNumP)
    print('   [   0 ]: ', daTimes[0],
          daCoord1[0], daCoord2[0], daCoord3[0])
    for ii in range(iNumP):
        print(' {', daPitchAngles[ii], '}: ', da3FluxData[0][0][ii], da3FluxData[0]
              [1][ii], da3FluxData[0][2][ii], da3FluxData[0][iNumE-1][ii])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    for ii in range(iNumP):
        print(' {', daPitchAngles[ii], '}: ', da3FluxData[iNum-1][0][ii], da3FluxData[iNum-1]
              [1][ii], da3FluxData[iNum-1][2][ii], da3FluxData[iNum-1][iNumE-1][ii])

    iPerc = 75
    ierr, da3FluxData = ae9ap9.computeFluxPercentile(iPerc)
    print(' computeFluxMean: iPerc=', iPerc, ' ierr=', ierr,
          ' da3Flux shape=', da3FluxData.shape)
    assert(ierr == 0)
    assert(da3FluxData.shape[0] == iNum)
    assert(da3FluxData.shape[1] == iNumE)
    assert(da3FluxData.shape[2] == iNumP)
    print('   [   0 ]: ', daTimes[0],
          daCoord1[0], daCoord2[0], daCoord3[0])
    for ii in range(iNumP):
        print(' {', daPitchAngles[ii], '}: ', da3FluxData[0][0][ii], da3FluxData[0]
              [1][ii], da3FluxData[0][2][ii], da3FluxData[0][iNumE-1][ii])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    for ii in range(iNumP):
        print(' {', daPitchAngles[ii], '}: ', da3FluxData[iNum-1][0][ii], da3FluxData[iNum-1]
              [1][ii], da3FluxData[iNum-1][2][ii], da3FluxData[iNum-1][iNumE-1][ii])

    iPerc = 125
    ierr, da3FluxData = ae9ap9.computeFluxPercentile(iPerc)
    print(' computeFluxPercentile: iPerc=', iPerc, ' ierr=', ierr,
          ' da3Flux shape=', da3FluxData.shape)
    assert(ierr < 0)

    iPerc = 0
    ierr, da3FluxData = ae9ap9.computeFluxPercentile(iPerc)
    print(' computeFluxPercentile: iPerc=', iPerc, ' ierr=', ierr,
          ' da3Flux shape=', da3FluxData.shape)
    assert(ierr < 0)

    iScen = 0
    ierr, da3FluxData = ae9ap9.computeFluxPerturbedMean(iScen)
    print(' computeFluxPerturbedMean: iScen=', iScen,
          '; ierr=', ierr, ' da3Flux shape=', da3FluxData.shape)
    assert(ierr < 0)

    iScen = 10
    ierr, da3FluxData = ae9ap9.computeFluxPerturbedMean(iScen)
    print(' computeFluxPerturbedMean: iScen=', iScen,
          '; ierr=', ierr, ' da3Flux shape=', da3FluxData.shape)
    assert(ierr == 0)
    assert(da3FluxData.shape[0] == iNum)
    assert(da3FluxData.shape[1] == iNumE)
    assert(da3FluxData.shape[2] == iNumP)
    print('   [   0 ]: ', daTimes[0],
          daCoord1[0], daCoord2[0], daCoord3[0])
    for ii in range(iNumP):
        print(' {', daPitchAngles[ii], '}: ', da3FluxData[0][0][ii], da3FluxData[0]
              [1][ii], da3FluxData[0][2][ii], da3FluxData[0][iNumE-1][ii])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    for ii in range(iNumP):
        print(' {', daPitchAngles[ii], '}: ', da3FluxData[iNum-1][0][ii], da3FluxData[iNum-1]
              [1][ii], da3FluxData[iNum-1][2][ii], da3FluxData[iNum-1][iNumE-1][ii])

    dEpochTime = daTimes[0]
    iScen = 25
    ierr, da3FluxData = ae9ap9.computeFluxScenario(dEpochTime, iScen)
    print(' computeFluxScenario: dEpochTime=', dEpochTime, ', iScen=', iScen,
          '; ierr=', ierr, ' da3Flux shape=', da3FluxData.shape)
    assert(ierr == 0)
    assert(da3FluxData.shape[0] == iNum)
    assert(da3FluxData.shape[1] == iNumE)
    assert(da3FluxData.shape[2] == iNumP)
    print('   [   0 ]: ', daTimes[0],
          daCoord1[0], daCoord2[0], daCoord3[0])
    for ii in range(iNumP):
        print(' {', daPitchAngles[ii], '}: ', da3FluxData[0][0][ii], da3FluxData[0]
              [1][ii], da3FluxData[0][2][ii], da3FluxData[0][iNumE-1][ii])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    for ii in range(iNumP):
        print(' {', daPitchAngles[ii], '}: ', da3FluxData[iNum-1][0][ii], da3FluxData[iNum-1]
              [1][ii], da3FluxData[iNum-1][2][ii], da3FluxData[iNum-1][iNumE-1][ii])

    dEpochTime = daTimes[0]
    iScen = 512
    iPert = 1
    ierr, da3FluxData = ae9ap9.computeFluxScenario(dEpochTime, iScen, iPert)
    print(' computeFluxScenario: dEpochTime=', dEpochTime, ', iScen=', iScen,
          ', iPert=1; ierr=', ierr, ' da3Flux shape=', da3FluxData.shape)
    assert(ierr == 0)
    assert(da3FluxData.shape[0] == iNum)
    assert(da3FluxData.shape[1] == iNumE)
    assert(da3FluxData.shape[2] == iNumP)
    print('   [   0 ]: ', daTimes[0],
          daCoord1[0], daCoord2[0], daCoord3[0])
    for ii in range(iNumP):
        print(' {', daPitchAngles[ii], '}: ', da3FluxData[0][0][ii], da3FluxData[0]
              [1][ii], da3FluxData[0][2][ii], da3FluxData[0][iNumE-1][ii])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    for ii in range(iNumP):
        print(' {', daPitchAngles[ii], '}: ', da3FluxData[iNum-1][0][ii], da3FluxData[iNum-1]
              [1][ii], da3FluxData[iNum-1][2][ii], da3FluxData[iNum-1][iNumE-1][ii])


def test_set_fluxEnvironVarPitch_computeFlux(ae9ap9, ephem):

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

    # cannot test with invalid energies - not caught at this level

    # set environment with this ephemeris
    #  - will fail on invalid flux type
    daEnergies = np.array([0.1, 0.5, 1.0, 2.0, 3.5, 5.0, 8.0])
    iNumE = len(daEnergies)

    # define base set of pitch angles
    daPitchAngles = np.array([75., 85., 90.])
    iNumP = len(daPitchAngles)
    da2PitchAngles = daPitchAngles
    daT = np.zeros(iNumP)
    for jj in range(iNum-1):
        for ii in range(iNumP):
            # make a totally arbitrary variation of pitch angle values
            daT[ii] = daPitchAngles[ii]+((jj % (ii+4)-3)*(0.27*(ii+2)))
        da2PitchAngles = np.vstack((da2PitchAngles, daT))
    print(' da2PitchAngles=', da2PitchAngles.shape)
    assert (da2PitchAngles.shape[1] == iNumP)

    strFluxType = 'Unspecified'
    ierr = ae9ap9.set_fluxEnvironVarPitch(strFluxType,
                                          daEnergies, daEnergies, daTimes, strCoordSys, strCoordUnits,
                                          daCoord1, daCoord2, daCoord3, da2PitchAngles)
    print(' set_fluxEnvironVarPitch: ',
          ' FluxType=', strFluxType, ' Energies(#)=', iNumE,
          ' daTimes(#)=', len(daTimes), ', daCoord1(#)=', len(daCoord1),
          ' CoordSys=', strCoordSys, strCoordUnits,
          ' PitchAngles=', da2PitchAngles[:15][:])
    print('  returned ierr=', ierr)
    assert(ierr < 0)

    # set environment with this ephemeris
    strFluxType = 'Integral'
    # fail with invalid coord system
    strCoordSys = 'GPX'
    ierr = ae9ap9.set_fluxEnvironVarPitch(strFluxType,
                                          daEnergies, daEnergies, daTimes, strCoordSys, strCoordUnits,
                                          daCoord1, daCoord2, daCoord3, da2PitchAngles)
    print(' set_fluxEnvironVarPitch: ',
          ' FluxType=', strFluxType, ' Energies(#)=', iNumE,
          ' daTimes(#)=', len(daTimes), ', daCoord1(#)=', len(daCoord1),
          ' CoordSys=', strCoordSys, strCoordUnits,
          ' PitchAngles=', da2PitchAngles[:15][:])
    print('  returned ierr=', ierr)
    assert(ierr < 0)

    iScen = 10
    ierr, da3FluxData = ae9ap9.computeFluxPerturbedMean(iScen)
    print(' premature computeFluxPerturbedMean: iScen=', iScen,
          '; ierr=', ierr, ' da3Flux shape=', da3FluxData.shape)
    assert(ierr < 0)

    # try again, with valid flux type and coord system
    strFluxType = '1PtDiff'
    strCoordSys = 'GDZ'
    ierr = ae9ap9.set_fluxEnvironVarPitch(strFluxType,
                                          daEnergies, daEnergies, daTimes, strCoordSys, strCoordUnits,
                                          daCoord1, daCoord2, daCoord3, da2PitchAngles)
    print(' set_fluxEnvironVarPitch: ',
          ' FluxType=', strFluxType, ' Energies(#)=', len(daEnergies),
          ' daTimes(#)=', len(daTimes), ', daCoord1(#)=', len(daCoord1),
          ' CoordSys=', strCoordSys, strCoordUnits,
          ' PitchAngles=', da2PitchAngles[:15][:])
    print('  returned ierr=', ierr)
    assert(ierr == 0)

    ierr, da3FluxData = ae9ap9.computeFluxMean()
    print(' computeFluxMean: ierr=', ierr,
          ' da3Flux shape=', da3FluxData.shape)
    assert(ierr == 0)
    assert(da3FluxData.shape[0] == iNum)
    assert(da3FluxData.shape[1] == iNumE)
    assert(da3FluxData.shape[2] == iNumP)
    print('   [   0 ]: ', daTimes[0],
          daCoord1[0], daCoord2[0], daCoord3[0])
    for ii in range(iNumP):
        print(' {', da2PitchAngles[0][ii], '}: ', da3FluxData[0][0][ii],
              da3FluxData[0][1][ii], da3FluxData[0][2][ii], da3FluxData[0][iNumE-1][ii])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    for ii in range(iNumP):
        print(' {', da2PitchAngles[iNum-1][ii], '}: ', da3FluxData[iNum-1][0][ii],
              da3FluxData[iNum-1][1][ii], da3FluxData[iNum-1][2][ii], da3FluxData[iNum-1][iNumE-1][ii])

    iPerc = 75
    ierr, da3FluxData = ae9ap9.computeFluxPercentile(iPerc)
    print(' computeFluxMean: iPerc=', iPerc, ' ierr=', ierr,
          ' da3Flux shape=', da3FluxData.shape)
    assert(ierr == 0)
    assert(da3FluxData.shape[0] == iNum)
    assert(da3FluxData.shape[1] == iNumE)
    assert(da3FluxData.shape[2] == iNumP)
    print('   [   0 ]: ', daTimes[0],
          daCoord1[0], daCoord2[0], daCoord3[0])
    for ii in range(iNumP):
        print(' {', da2PitchAngles[0][ii], '}: ', da3FluxData[0][0][ii],
              da3FluxData[0][1][ii], da3FluxData[0][2][ii], da3FluxData[0][iNumE-1][ii])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    for ii in range(iNumP):
        print(' {', da2PitchAngles[iNum-1][ii], '}: ', da3FluxData[iNum-1][0][ii],
              da3FluxData[iNum-1][1][ii], da3FluxData[iNum-1][2][ii], da3FluxData[iNum-1][iNumE-1][ii])

    iPerc = 125
    ierr, da3FluxData = ae9ap9.computeFluxPercentile(iPerc)
    print(' computeFluxPercentile: iPerc=', iPerc, ' ierr=', ierr,
          ' da3Flux shape=', da3FluxData.shape)
    assert(ierr < 0)

    iPerc = 0
    ierr, da3FluxData = ae9ap9.computeFluxPercentile(iPerc)
    print(' computeFluxPercentile: iPerc=', iPerc, ' ierr=', ierr,
          ' da3Flux shape=', da3FluxData.shape)
    assert(ierr < 0)

    iScen = 0
    ierr, da3FluxData = ae9ap9.computeFluxPerturbedMean(iScen)
    print(' computeFluxPerturbedMean: iScen=', iScen,
          '; ierr=', ierr, ' da3Flux shape=', da3FluxData.shape)
    assert(ierr < 0)

    iScen = 10
    ierr, da3FluxData = ae9ap9.computeFluxPerturbedMean(iScen)
    print(' computeFluxPerturbedMean: iScen=', iScen,
          '; ierr=', ierr, ' da3Flux shape=', da3FluxData.shape)
    assert(ierr == 0)
    assert(da3FluxData.shape[0] == iNum)
    assert(da3FluxData.shape[1] == iNumE)
    assert(da3FluxData.shape[2] == iNumP)
    print('   [   0 ]: ', daTimes[0],
          daCoord1[0], daCoord2[0], daCoord3[0])
    for ii in range(iNumP):
        print(' {', da2PitchAngles[0][ii], '}: ', da3FluxData[0][0][ii],
              da3FluxData[0][1][ii], da3FluxData[0][2][ii], da3FluxData[0][iNumE-1][ii])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    for ii in range(iNumP):
        print(' {', da2PitchAngles[iNum-1][ii], '}: ', da3FluxData[iNum-1][0][ii],
              da3FluxData[iNum-1][1][ii], da3FluxData[iNum-1][2][ii], da3FluxData[iNum-1][iNumE-1][ii])

    dEpochTime = daTimes[0]
    iScen = 25
    ierr, da3FluxData = ae9ap9.computeFluxScenario(dEpochTime, iScen)
    print(' computeFluxScenario: dEpochTime=', dEpochTime, ', iScen=', iScen,
          '; ierr=', ierr, ' da3Flux shape=', da3FluxData.shape)
    assert(ierr == 0)
    assert(da3FluxData.shape[0] == iNum)
    assert(da3FluxData.shape[1] == iNumE)
    assert(da3FluxData.shape[2] == iNumP)
    print('   [   0 ]: ', daTimes[0],
          daCoord1[0], daCoord2[0], daCoord3[0])
    for ii in range(iNumP):
        print(' {', da2PitchAngles[0][ii], '}: ', da3FluxData[0][0][ii],
              da3FluxData[0][1][ii], da3FluxData[0][2][ii], da3FluxData[0][iNumE-1][ii])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    for ii in range(iNumP):
        print(' {', da2PitchAngles[iNum-1][ii], '}: ', da3FluxData[iNum-1][0][ii],
              da3FluxData[iNum-1][1][ii], da3FluxData[iNum-1][2][ii], da3FluxData[iNum-1][iNumE-1][ii])

    dEpochTime = daTimes[0]
    iScen = 512
    iPert = 1
    ierr, da3FluxData = ae9ap9.computeFluxScenario(dEpochTime, iScen, iPert)
    print(' computeFluxScenario: dEpochTime=', dEpochTime, ', iScen=', iScen,
          ', iPert=1; ierr=', ierr, ' da3Flux shape=', da3FluxData.shape)
    assert(ierr == 0)
    assert(da3FluxData.shape[0] == iNum)
    assert(da3FluxData.shape[1] == iNumE)
    assert(da3FluxData.shape[2] == iNumP)
    print('   [   0 ]: ', daTimes[0],
          daCoord1[0], daCoord2[0], daCoord3[0])
    for ii in range(iNumP):
        print(' {', da2PitchAngles[0][ii], '}: ', da3FluxData[0][0][ii],
              da3FluxData[0][1][ii], da3FluxData[0][2][ii], da3FluxData[0][iNumE-1][ii])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    for ii in range(iNumP):
        print(' {', da2PitchAngles[iNum-1][ii], '}: ', da3FluxData[iNum-1][0][ii],
              da3FluxData[iNum-1][1][ii], da3FluxData[iNum-1][2][ii], da3FluxData[iNum-1][iNumE-1][ii])


def test_set_fluxEnvironDirVec_computeFlux(ae9ap9, ephem):

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
    iNum, daTimes, daXPos, daYPos, daZPos, daXVel, daYVel, daZVel \
        = ephem.computeGei()  # , iSize=0 )
    print(' computeGei: ')
    print('  returned iNum=', iNum, '; len(daTimes)=',
          len(daTimes), ', len(daXPos)=', len(daXPos))
    assert(iNum > 0)
    if iNum > 0:
        print('   [   0 ]: ', daTimes[0], daXPos[0], daYPos[0], daZPos[0])
        print('   [', iNum-1, ']: ', daTimes[iNum-1],
              daXPos[iNum-1], daYPos[iNum-1], daZPos[iNum-1])
    assert(iNum > 0)

    # cannot test with invalid energies - not caught at this level

    # set environment with this ephemeris
    #  - will fail on invalid flux type
    daEnergies = np.array([0.1, 0.5, 1.0, 2.0, 3.5, 5.0, 8.0])
    iNumE = len(daEnergies)

    # turn array (N,) into (N,1)
    da2DirX = np.transpose(np.array([daZVel]))
    da2DirY = np.transpose(np.array([daXVel]))
    da2DirZ = np.transpose(np.array([daYVel]))

    # add another set of direction vectors  ==> (N,2)
    da2DirX = np.hstack((da2DirX, np.transpose(np.array([daZVel/3.0]))))
    da2DirY = np.hstack((da2DirY, np.transpose(np.array([-daXVel/2.0]))))
    da2DirZ = np.hstack((da2DirZ, np.transpose(np.array([daYVel*1.5]))))
    print(' da2DirX.shape = ', da2DirX.shape)

    strCoordSys = 'GEI'
    strCoordUnits = 'km'
    strFluxType = 'Unspecified'
    ierr = ae9ap9.set_fluxEnvironDirVec(strFluxType,
                                        daEnergies, daEnergies, daTimes, strCoordSys, strCoordUnits,
                                        daXPos, daYPos, daZPos, da2DirX, da2DirY, da2DirZ)
    print(' set_fluxEnvironDirVec:',
          ' FluxType=', strFluxType, ' Energies(#)=', iNumE,
          ' daTimes(#)=', len(daTimes), ', daXPos(#)=', len(daXPos),
          ' CoordSys=', strCoordSys, strCoordUnits,
          ' DirVec=', da2DirX[:15][:], da2DirY[:15][:], da2DirZ[:15][:])
    print('  returned ierr=', ierr)
    assert(ierr < 0)

    # set environment with this ephemeris
    strFluxType = 'Integral'
    # fail with invalid coord system
    strCoordSys = 'GPX'
    ierr = ae9ap9.set_fluxEnvironDirVec(strFluxType,
                                        daEnergies, daEnergies, daTimes, strCoordSys, strCoordUnits,
                                        daXPos, daYPos, daZPos, da2DirX, da2DirY, da2DirZ)
    print(' set_fluxEnvironDirVec: ',
          ' FluxType=', strFluxType, ' Energies(#)=', iNumE,
          ' daTimes(#)=', len(daTimes), ', daXPos(#)=', len(daXPos),
          ' CoordSys=', strCoordSys, strCoordUnits,
          ' DirVec=', da2DirX[:15][:], da2DirY[:15][:], da2DirZ[:15][:])
    print('  returned ierr=', ierr)
    assert(ierr < 0)

    # try again, with valid flux type and coord system
    strFluxType = '1PtDiff'
    strCoordSys = 'GEI'  # cartesian coordinate system required
    ierr = ae9ap9.set_fluxEnvironDirVec(strFluxType,
                                        daEnergies, daEnergies, daTimes, strCoordSys, strCoordUnits,
                                        daXPos, daYPos, daZPos, da2DirX, da2DirY, da2DirZ)
    print(' set_fluxEnvironDirVec: ',
          ' FluxType=', strFluxType, ' Energies(#)=', iNumE,
          ' daTimes(#)=', len(daTimes), ', daXPos(#)=', len(daXPos),
          ' CoordSys=', strCoordSys, strCoordUnits,
          ' DirVec=', da2DirX[:15][:], da2DirY[:15][:], da2DirZ[:15][:])
    print('  returned ierr=', ierr)
    assert(ierr == 0)

    ierr, da2PitchAngles = ae9ap9.get_pitchAngles()
    print(' get_pitchAngles: ')
    assert(ierr == 0)
    assert(da2PitchAngles.size > 0)
    print(' da2PitchAngles.shape=', da2PitchAngles.shape)
    print(' da2PitchAngles[:15][:] = ', da2PitchAngles[:15][:])
    iNumP = da2PitchAngles.shape[1]

    ierr, da3FluxData = ae9ap9.computeFluxMean()
    print(' computeFluxMean: ierr=', ierr,
          ' da3Flux shape=', da3FluxData.shape)
    assert(ierr == 0)
    assert(da3FluxData.shape[0] == iNum)
    assert(da3FluxData.shape[1] == iNumE)
    assert(da3FluxData.shape[2] == iNumP)
    print('   [   0 ]: ', daTimes[0], daXPos[0], daYPos[0], daZPos[0])
    for ii in range(iNumP):
        print(' {', da2PitchAngles[0][ii], '}: ', da3FluxData[0][0][ii],
              da3FluxData[0][1][ii], da3FluxData[0][2][ii], da3FluxData[0][iNumE-1][ii])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daXPos[iNum-1], daYPos[iNum-1], daZPos[iNum-1])
    for ii in range(iNumP):
        print(' {', da2PitchAngles[iNum-1][ii], '}: ', da3FluxData[iNum-1][0][ii],
              da3FluxData[iNum-1][1][ii], da3FluxData[iNum-1][2][ii], da3FluxData[iNum-1][iNumE-1][ii])

    iPerc = 75
    ierr, da3FluxData = ae9ap9.computeFluxPercentile(iPerc)
    print(' computeFluxMean: iPerc=', iPerc, ' ierr=', ierr,
          ' da3Flux shape=', da3FluxData.shape)
    assert(ierr == 0)
    assert(da3FluxData.shape[0] == iNum)
    assert(da3FluxData.shape[1] == iNumE)
    assert(da3FluxData.shape[2] == iNumP)
    print('   [   0 ]: ', daTimes[0], daXPos[0], daYPos[0], daZPos[0])
    for ii in range(iNumP):
        print(' {', da2PitchAngles[0][ii], '}: ', da3FluxData[0][0][ii],
              da3FluxData[0][1][ii], da3FluxData[0][2][ii], da3FluxData[0][iNumE-1][ii])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daXPos[iNum-1], daYPos[iNum-1], daZPos[iNum-1])
    for ii in range(iNumP):
        print(' {', da2PitchAngles[iNum-1][ii], '}: ', da3FluxData[iNum-1][0][ii],
              da3FluxData[iNum-1][1][ii], da3FluxData[iNum-1][2][ii], da3FluxData[iNum-1][iNumE-1][ii])

    iPerc = 125
    ierr, da3FluxData = ae9ap9.computeFluxPercentile(iPerc)
    print(' computeFluxPercentile: iPerc=', iPerc, ' ierr=', ierr,
          ' da3Flux shape=', da3FluxData.shape)
    assert(ierr < 0)

    iPerc = 0
    ierr, da3FluxData = ae9ap9.computeFluxPercentile(iPerc)
    print(' computeFluxPercentile: iPerc=', iPerc, ' ierr=', ierr,
          ' da3Flux shape=', da3FluxData.shape)
    assert(ierr < 0)

    iScen = 0
    ierr, da3FluxData = ae9ap9.computeFluxPerturbedMean(iScen)
    print(' computeFluxPerturbedMean: iScen=', iScen,
          '; ierr=', ierr, ' da3Flux shape=', da3FluxData.shape)
    assert(ierr < 0)

    iScen = 10
    ierr, da3FluxData = ae9ap9.computeFluxPerturbedMean(iScen)
    print(' computeFluxPerturbedMean: iScen=', iScen,
          '; ierr=', ierr, ' da3Flux shape=', da3FluxData.shape)
    assert(ierr == 0)
    assert(da3FluxData.shape[0] == iNum)
    assert(da3FluxData.shape[1] == iNumE)
    assert(da3FluxData.shape[2] == iNumP)
    print('   [   0 ]: ', daTimes[0], daXPos[0], daYPos[0], daZPos[0])
    for ii in range(iNumP):
        print(' {', da2PitchAngles[0][ii], '}: ', da3FluxData[0][0][ii],
              da3FluxData[0][1][ii], da3FluxData[0][2][ii], da3FluxData[0][iNumE-1][ii])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daXPos[iNum-1], daYPos[iNum-1], daZPos[iNum-1])
    for ii in range(iNumP):
        print(' {', da2PitchAngles[iNum-1][ii], '}: ', da3FluxData[iNum-1][0][ii],
              da3FluxData[iNum-1][1][ii], da3FluxData[iNum-1][2][ii], da3FluxData[iNum-1][iNumE-1][ii])

    dEpochTime = daTimes[0]
    iScen = 25
    ierr, da3FluxData = ae9ap9.computeFluxScenario(dEpochTime, iScen)
    print(' computeFluxScenario: dEpochTime=', dEpochTime, ', iScen=', iScen,
          '; ierr=', ierr, ' da3Flux shape=', da3FluxData.shape)
    assert(ierr == 0)
    assert(da3FluxData.shape[0] == iNum)
    assert(da3FluxData.shape[1] == iNumE)
    assert(da3FluxData.shape[2] == iNumP)
    print('   [   0 ]: ', daTimes[0], daXPos[0], daYPos[0], daZPos[0])
    for ii in range(iNumP):
        print(' {', da2PitchAngles[0][ii], '}: ', da3FluxData[0][0][ii],
              da3FluxData[0][1][ii], da3FluxData[0][2][ii], da3FluxData[0][iNumE-1][ii])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daXPos[iNum-1], daYPos[iNum-1], daZPos[iNum-1])
    for ii in range(iNumP):
        print(' {', da2PitchAngles[iNum-1][ii], '}: ', da3FluxData[iNum-1][0][ii],
              da3FluxData[iNum-1][1][ii], da3FluxData[iNum-1][2][ii], da3FluxData[iNum-1][iNumE-1][ii])

    dEpochTime = daTimes[0]
    iScen = 512
    iPert = 1
    ierr, da3FluxData = ae9ap9.computeFluxScenario(dEpochTime, iScen, iPert)
    print(' computeFluxScenario: dEpochTime=', dEpochTime, ', iScen=', iScen,
          ', iPert=1; ierr=', ierr, ' da3Flux shape=', da3FluxData.shape)
    assert(ierr == 0)
    assert(da3FluxData.shape[0] == iNum)
    assert(da3FluxData.shape[1] == iNumE)
    assert(da3FluxData.shape[2] == iNumP)
    print('   [   0 ]: ', daTimes[0], daXPos[0], daYPos[0], daZPos[0])
    for ii in range(iNumP):
        print(' {', da2PitchAngles[0][ii], '}: ', da3FluxData[0][0][ii],
              da3FluxData[0][1][ii], da3FluxData[0][2][ii], da3FluxData[0][iNumE-1][ii])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daXPos[iNum-1], daYPos[iNum-1], daZPos[iNum-1])
    for ii in range(iNumP):
        print(' {', da2PitchAngles[iNum-1][ii], '}: ', da3FluxData[iNum-1][0][ii],
              da3FluxData[iNum-1][1][ii], da3FluxData[iNum-1][2][ii], da3FluxData[iNum-1][iNumE-1][ii])


def xest_second_instance(ae9ap9):
    # start a second instance of the ae9ap9eris object, verify not the same as the first
    ae9ap9_2nd = Ae9Ap9Model()
    print('ae9ap9 zHandle=', ae9ap9_2nd.zHandle)
    assert(ae9ap9_2nd.zHandle > 0)
    assert(ae9ap9_2nd.zHandle != ae9ap9.zHandle)

    # define a random orbit
    dValue = 55.0
    ierr = ae9ap9_2nd.set_inclination(dValue)
    print(' set_inclination: zHandle=', zHandle,
          ' dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = 5400.0
    ierr = ae9ap9_2nd.set_apogeeAlt(dValue)
    print(' set_apogeeAlt: zHandle=', zHandle,
          ' dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = 400.0
    ierr = ae9ap9_2nd.set_perigeeAlt(dValue)
    print(' set_perigeeAlt: zHandle=', zHandle,
          ' dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = 125.0
    ierr = ae9ap9_2nd.set_rightAscension(dValue)
    print(' set_rightAscension: zHandle=', zHandle,
          ' dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = 105.0
    ierr = ae9ap9_2nd.set_argPerigee(dValue)
    print(' set_argPerigee: zHandle=', zHandle,
          ' dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = 235.0
    ierr = ae9ap9_2nd.set_meanAnomaly(dValue)
    print(' set_meanAnomaly: zHandle=', zHandle,
          ' dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dMjd = 58119.5
    ierr = ae9ap9_2nd.set_elementTime(dMjd)
    print(' set_elementTime: zHandle=',
          zHandle, ' dMjd=', dMjd, ' => ', ierr)
    assert(ierr == 0)

    # verify that ZERO chunk size is specified
    iSize = ae9ap9_2nd.get_chunkSize(zHandle)
    print(' get_chunkSize: zHandle=', zHandle, ' returned iSize=', iSize)
    assert(iSize == 0)

    # make call without ae9ap9eris generation limits defined -> fails
    strCoordSys = 'RLL'
    strCoordUnits = 'Re'
    iNum, daTime, daCoord1, daCoord2, daCoord3 \
        = ae9ap9_2nd.compute(strCoordSys, strCoordUnits)
    print(' compute: zHandle=', zHandle,
          ' CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTime)=',
          len(daTime), ', len(daCoord1)=', len(daCoord1))
    assert(iNum < 0)

    # define ae9ap9eris generation limits
    dStep = 60.0
    dStart = 58119.5
    dEnd = 58120.5
    ierr = ae9ap9_2nd.set_times(dStart, dEnd, dStep)
    print(' set_times: zHandle=%s start=%.1f, end=%.1f, step=%.0f => %i' %
          (zHandle, dStart, dEnd, dStep, ierr))
    assert(ierr == 0)

    # make call again, with generation limits defined
    #   -> but fails on lack of mag field db specification
    iNum, daTime, daCoord1, daCoord2, daCoord3 \
        = ae9ap9_2nd.compute(strCoordSys, strCoordUnits)
    print(' compute: zHandle=', zHandle,
          ' CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTime)=',
          len(daTime), ', len(daCoord1)=', len(daCoord1))
    assert(iNum < 0)

    # specify the mag field db file
    strFile = '../../modelData/igrfDB.h5'
    ierr = ae9ap9_2nd.set_magfieldDBFile(strFile)
    print(' set_magfieldDBFile: zHandle=', zHandle,
          ' dbFile=', strFile, ' => ', ierr)
    assert(ierr == 0)

    # make call, this time successful
    #  -> verify sizing of data returned
    iNum, daTime, daCoord1, daCoord2, daCoord3 \
        = ae9ap9_2nd.compute(strCoordSys, strCoordUnits)
    print(' compute: zHandle=', zHandle,
          ' CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTime)=',
          len(daTime), ', len(daCoord1)=', len(daCoord1))
    assert(iNum == 1441)
    assert(len(daTime) == iNum)
    assert(len(daCoord1) == iNum)

    # shut down this alternate instance
    del ae9ap9
