
# enable use of python3 print function form in python 2.x
from __future__ import print_function

# for the 'import irene_defs' to be successful, either:
#     specify the path to the Ae9Ap9 root directory here:
#sys.path.append(os.environ['HOME']+'/Ae9Ap9')
#     or in the PYTHONPATH environement variable
#
# load definitions for required IRENE environment variables
import irene_defs

from aggregModel import AggregModel
from ae9ap9Model import Ae9Ap9Model
from ephemModel import EphemModel
import numpy as np
import pytest


@pytest.fixture
def aggreg():
    aggreg = AggregModel()
    print('aggreg zHandle=', aggreg.zHandle)
    return aggreg


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

    # use ephem model to generate test inputs

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

    return ephem


def test_addScenToAgg1(aggreg, ae9ap9, ephem):

    # get about two orbits of ephemeris
    strCoordSys = 'GDZ'
    strCoordUnits = 'km'
    iNum, daTimes, daCoord1, daCoord2, daCoord3 \
        = ephem.compute(strCoordSys, strCoordUnits)
    print(' compute: CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTimes)=',
          len(daTimes), ', len(daCoord1)=', len(daCoord1))
    assert(iNum > 0)

    # set environment with this ephemeris
    daEnergies = np.array([0.1, 0.5, 1.0, 2.0, 3.5, 5.0, 8.0])
    iNumE = len(daEnergies)

    # valid flux type and coord system
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

    iNum1p, iNum2p, iNum3p = aggreg.get_aggDataDim()
    assert(iNum1p == 0)
    assert(iNum2p == 0)
    assert(iNum3p == 0)

    ierr = aggreg.get_numScenarios()
    assert(ierr == 0)
    print(' get_numScenarios: returned ierr=', ierr)

    i = 0
    while i < 10:
        ierr, da3FluxData = ae9ap9.computeFluxPerturbedMean(
            (i+1)*10)  # just to get a unique number
        assert(ierr == 0)
        assert(da3FluxData.shape[0] == iNum)
        assert(da3FluxData.shape[1] == iNumE)
        assert(da3FluxData.shape[2] == 1)

        ierr = aggreg.addScenToAgg(daTimes, da3FluxData)
        assert(ierr == 0)  # pass
        print(' addScenToAgg: returned ierr=', ierr)

        iNum1p, iNum2p, iNum3p = aggreg.get_aggDataDim()
        assert(iNum1p == da3FluxData.shape[0])
        assert(iNum2p == da3FluxData.shape[1])
        assert(iNum3p == da3FluxData.shape[2])
        print(' get_aggDataDim: iNum1p(#)=', iNum1p, ' => ', ierr)
        print(' get_aggDataDim: iNum2p(#)=', iNum2p, ' => ', ierr)
        print(' get_aggDataDim: iNum3p(#)=', iNum3p, ' => ', ierr)

        i += 1

    ierr = aggreg.addScenToAgg(
        np.zeros(iNum-2), np.zeros((iNum1p-2, iNum2p-2, iNum3p)))
    assert(ierr < 0)  # different size than existing scenarios
    print(' addScenToAgg: returned ierr=', ierr)

    ierr = aggreg.get_numScenarios()
    assert(ierr == i)
    print(' get_numScenarios: returned ierr=', ierr)

    for testVal in [-1, 105]:
        ierr, daTimes, da3clData = aggreg.computeConfLevel(testVal)
        print(' computeConfLevel: testVal=', testVal, ' => ', ierr)
        assert(ierr < 0)

    for testVal in [0, 50, 75, 90, 95]:
        ierr, daTimes, da3clData = aggreg.computeConfLevel(testVal)
        print(' computeConfLevel: testVal=', testVal, ' => ', ierr)
        assert(ierr == 0)
        assert(da3clData.shape[2] == 1)

    ierr, daTimes, da3clData = aggreg.computeMean()
    print(' computeMean: => ', ierr)
    assert(ierr == 0)
    assert(da3clData.shape[0] == iNum)
    assert(da3clData.shape[1] == iNumE)
    assert(da3clData.shape[2] == 1)

    aggreg.reset()


def test_addScenToAgg2(aggreg, ae9ap9, ephem):

    # get about two orbits of ephemeris
    strCoordSys = 'GDZ'
    strCoordUnits = 'km'
    iNum, daTimes, daCoord1, daCoord2, daCoord3 \
        = ephem.compute(strCoordSys, strCoordUnits)
    print(' compute: CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTimes)=',
          len(daTimes), ', len(daCoord1)=', len(daCoord1))
    assert(iNum > 0)

    # set environment with this ephemeris
    daEnergies = np.array([0.1, 0.5, 1.0, 2.0, 3.5, 5.0, 8.0])
    iNumE = len(daEnergies)
    daPitchAngles = np.array([75., 85., 90.])
    iNumP = len(daPitchAngles)

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

    ierr = aggreg.get_numScenarios()
    assert(ierr == 0)
    print(' get_numScenarios: returned ierr=', ierr)

    iScen = 0
    ierr, da3FluxData = ae9ap9.computeFluxPerturbedMean(iScen)
    print(' computeFluxPerturbedMean: iScen=', iScen,
          '; ierr=', ierr, ' da3Flux shape=', da3FluxData.shape)
    assert(ierr < 0)

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

    ierr = aggreg.get_numScenarios()
    assert(ierr == 0)
    print(' get_numScenarios: returned ierr=', ierr)

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

    iNum1p, iNum2p, iNum3p = aggreg.get_aggDataDim()
    assert(iNum1p == 0)
    assert(iNum2p == 0)
    assert(iNum3p == 0)

    ierr, daTimes2, da3clData2 = aggreg.computeConfLevel(50)
    print(' computeConfLevel: testVal= 50 => ', ierr)
    assert(ierr < 0)

    i = 0
    while i < 10:
        iScen = i*10 + 1  # just to get a unique number
        ierr, da3FluxData = ae9ap9.computeFluxScenario(
            dEpochTime, iScen, iPert)
        print(' computeFluxScenario: iScen=', iScen,
              ', iPert=1; ierr=', ierr, ' da3Flux shape=', da3FluxData.shape)
        assert(ierr == 0)
        assert(da3FluxData.shape[0] == iNum)
        assert(da3FluxData.shape[1] == iNumE)
        assert(da3FluxData.shape[2] == iNumP)

        ierr = aggreg.addScenToAgg(daTimes, da3FluxData)
        print(' addScenToAgg: returned ierr=', ierr)
        assert(ierr == 0)  # pass

        iNum1p, iNum2p, iNum3p = aggreg.get_aggDataDim()
        print(' get_aggDataDim: iNum1p(#)=', iNum1p, ' => ', ierr)
        print(' get_aggDataDim: iNum2p(#)=', iNum2p, ' => ', ierr)
        print(' get_aggDataDim: iNum3p(#)=', iNum3p, ' => ', ierr)
        assert(iNum1p == iNum)
        assert(iNum2p == iNumE)
        assert(iNum3p == iNumP)

        i += 1

    ierr = aggreg.addScenToAgg(
        np.zeros(iNum-2), np.zeros((iNum1p-2, iNum2p-2, iNum3p)))
    assert(ierr < 0)  # different size than existing scenarios
    print(' addScenToAgg: returned ierr=', ierr)

    ierr = aggreg.get_numScenarios()
    assert(ierr == i)
    print(' get_numScenarios: returned ierr=', ierr)

    for testVal in [-1, 105]:
        ierr, daTimes, da3clData = aggreg.computeConfLevel(testVal)
        print(' computeConfLevel: testVal=', testVal, ' => ', ierr)
        assert(ierr < 0)

    for testVal in [0, 50, 75, 90, 95]:
        ierr, daTimes, da3clData = aggreg.computeConfLevel(testVal)
        print(' computeConfLevel: testVal=', testVal, ' => ', ierr)
        assert(ierr == 0)
        assert(da3clData.shape[2] == iNumP)

    ierr, daTimes, da3clData = aggreg.computeMean()
    print(' computeMean: => ', ierr)
    assert(ierr == 0)
    assert(da3clData.shape[0] == iNum)
    assert(da3clData.shape[1] == iNumE)
    assert(da3clData.shape[2] == iNumP)

    testVal = 50
    ierr, daTimes, da3clData = aggreg.computeConfLevel(testVal)
    print(' computeConfLevel: testVal=', testVal, ' => ', ierr)
    assert(ierr == 0)
    assert(da3clData.shape[2] == iNumP)

    ierr, daTimes2, da3clData2 = aggreg.computeMedian()
    print(' computeMean: => ', ierr)
    assert(ierr == 0)
    assert(da3clData2.shape[0] == iNum)
    assert(da3clData2.shape[1] == iNumE)
    assert(da3clData2.shape[2] == iNumP)
    assert(daTimes2.shape == daTimes.shape)

    # median and 50th percentile should match exactly
    for i in range(iNum):
        for j in range(iNumE):
            for k in range(iNumP):
                assert(da3clData2[i][j][k] == da3clData[i][j][k])
