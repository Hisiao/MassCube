
# enable use of python3 print function form in python 2.x
from __future__ import print_function

# for the 'import irene_defs' to be successful, either:
#     specify the path to the Ae9Ap9 root directory here:
#sys.path.append(os.environ['HOME']+'/Ae9Ap9')
#     or in the PYTHONPATH environement variable
#
# load definitions for required IRENE environment variables
import irene_defs

from accumModel import AccumModel
from ae9ap9Model import Ae9Ap9Model
from ephemModel import EphemModel
import numpy as np
import math
from builtins import range
import pytest


@pytest.fixture
def accum():
    accum = AccumModel()
    print('accum zHandle=', accum.zHandle)
    return accum


@pytest.fixture
def ae9ap9():
    ae9ap9 = Ae9Ap9Model()

    ae9ap9.set_modelDBFile('../../modelData/AE9V15_runtime_tables.mat')
    ae9ap9.set_kPhiDBFile('../../modelData/fastPhi_net.mat')
    ae9ap9.set_kHMinDBFile('../../modelData/fast_hmin_net.mat')
    ae9ap9.set_magfieldDBFile('../../modelData/igrfDB.h5')
    ae9ap9.load_modelDB()

    return ae9ap9


@pytest.fixture
def ephem():
    ephem = EphemModel()
    ephem.set_magfieldDBFile('../../modelData/igrfDB.h5')

    # use ephem model to generate test inputs

    dValue = 25.0
    ierr = ephem.set_inclination(dValue)
    assert(ierr == 0)
    dValue = 0.25
    ierr = ephem.set_eccentricity(dValue)
    assert(ierr == 0)
    dValue = 8.5
    ierr = ephem.set_meanMotion(dValue)
    assert(ierr == 0)
    dValue = 218.5
    ierr = ephem.set_meanAnomaly(dValue)
    assert(ierr == 0)
    dValue = 125.0
    ierr = ephem.set_rightAscension(dValue)
    assert(ierr == 0)
    dMjd = 58119.5
    ierr = ephem.set_elementTime(dMjd)
    assert(ierr == 0)
    dStep = 60.0
    dStart = 58120.0
    dEnd = 58120.25
    ierr = ephem.set_times(dStart, dEnd, dStep)
    assert(ierr == 0)

    return ephem


def test_time_intervals(accum):
    for testVal in [-10, -1.0, 0, 14, 9999.0]:  # time values in days
        ierr = accum.set_interval(testVal)
        print(' set_interval: testVal=', testVal, ' => ', ierr)
        if testVal <= 0.0 or testVal >= 9999.0:
          assert(ierr == -1)
          # set interval will remain set to last valid value specified
        else:
          assert(ierr == 0)
          assert(accum.get_interval() == math.floor(testVal*86400.0 + 0.5))

    for testVal in [-10, -1, 0, 10, 60]:  # time values in seconds
        ierr = accum.set_intervalSec(testVal)
        print(' set_interval: testVal=', testVal, ' => ', ierr)
        if testVal <= 0.0:
          assert(ierr == -1)
          # set interval will remain set to last valid value specified
        else:
          assert(ierr == 0)
          assert(accum.get_interval() == testVal)


def test_set_increment(accum):
    for testVal in [-10, -1.0, 0, 14, 9999]:  # positive values are accepted
        initialVal = accum.get_increment()  # store initial value

        ierr = accum.set_increment(testVal)
        print(' set_increment: testVal=', testVal, ' => ', ierr)

        returnVal = accum.get_increment()
        print(' get_increment: => ', returnVal)

        if testVal >= 0:
            assert(ierr == 0)
            assert(returnVal == testVal)
        else:
            assert(ierr < 0)
            assert(returnVal == initialVal)


def test_addToBuffer1(accum, ae9ap9, ephem):

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

    # verify that buffer is initially empty
    ierr = accum.clear_buffer()
    print(' clear_buffer: => ', ierr)
    assert(ierr == 0)
    piDim1, piDim2, piDim3 = accum.get_bufferDimensions()
    print(' get_bufferDimensions: piDim1:', piDim1, ', piDim2:', piDim2,
          ', piDim3:', piDim3)
    assert(piDim1 == 0)
    assert(piDim2 == 0)
    assert(piDim3 == 0)

    # ae9ap9 -> compute flux mean
    ierr, da3FluxData = ae9ap9.computeFluxMean()
    print(' computeFluxMean: ierr=', ierr,
          ' da3Flux shape=', da3FluxData.shape)
    assert(ierr == 0)
    assert(da3FluxData.shape[0] == iNum)
    assert(da3FluxData.shape[1] == iNumE)
    assert(da3FluxData.shape[2] == 1)
    print('   [   0 ]: ', daTimes[0], daCoord1[0], daCoord2[0], daCoord3[0])
    print('     ', da3FluxData[0][0][0], da3FluxData[0][1]
          [0], da3FluxData[0][2][0], da3FluxData[0][iNumE-1][0])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    print('     ', da3FluxData[iNum-1][0][0], da3FluxData[iNum-1][1]
          [0], da3FluxData[iNum-1][2][0], da3FluxData[iNum-1][iNumE-1][0])

    # load results into buffer and verify size
    ierr = accum.loadBuffer(daTimes, da3FluxData)
    print(' loadBuffer: => ', ierr)
    assert(ierr == 0)
    piDim1, piDim2, piDim3 = accum.get_bufferDimensions()
    print(' get_bufferDimensions: piDim1:', piDim1, ', piDim2:', piDim2,
          ', piDim3:', piDim3)
    assert(piDim1 == iNum)
    assert(piDim2 == iNumE)
    assert(piDim3 == 1)

    # clear buffer and verify empty
    ierr = accum.clear_buffer()
    print(' clear_buffer: => ', ierr)
    assert(ierr == 0)
    piDim1, piDim2, piDim3 = accum.get_bufferDimensions()
    print(' get_bufferDimensions: piDim1:', piDim1, ', piDim2:', piDim2,
          ', piDim3:', piDim3)
    assert(piDim1 == 0)
    assert(piDim2 == 0)
    assert(piDim3 == 0)

    # computeFluence should error on empty buffer
    iNum2, daFluenceTimes2, da3Fluence2 = accum.computeFluence()
    print(' computeFluence: => ', ierr)
    assert(iNum2 == -1)
    assert(len(daFluenceTimes2) == 1)
    assert(da3Fluence2.shape == (1, 1, 1))

    # test with values added individually
    for i in range(15):
        ierr = accum.addToBuffer(daTimes[i], da3FluxData[i])
        print(' addToBuffer: => ', ierr)
        assert(ierr == 0)

        # verify size
        piDim1, piDim2, piDim3 = accum.get_bufferDimensions()
        print(' get_bufferDimensions: piDim1:', piDim1, ', piDim2:', piDim2,
              ', piDim3:', piDim3)
        assert(piDim1 == i+1)
        assert(piDim2 == iNumE)
        assert(piDim3 == 1)

    # compute fluence and verify size
    iNum2, daFluenceTimes2, da3Fluence2 = accum.computeFluence()
    print(' computeFluence: => ', ierr)
    assert(iNum2 == 15)
    assert(len(daFluenceTimes2) == 15)
    assert(da3Fluence2.shape == (15, iNumE, 1))


def test_addToBuffer2(accum, ae9ap9, ephem):
    # get about two orbits of ephemeris
    strCoordSys = 'GDZ'
    strCoordUnits = 'km'
    iNum, daTimes, daCoord1, daCoord2, daCoord3 \
        = ephem.compute(strCoordSys, strCoordUnits)
    print(' compute: CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTimes)=',
          len(daTimes), ', len(daCoord1)=', len(daCoord1))
    assert(iNum > 0)

    daEnergies = np.array([0.1, 0.5, 1.0, 2.0, 3.5, 5.0, 8.0])
    iNumE = len(daEnergies)
    daPitchAngles = np.array([75., 85., 90.])
    iNumP = len(daPitchAngles)

    # set environment with this ephemeris
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

    # verify that buffer is initially empty
    ierr = accum.clear_buffer()
    print(' clear_buffer: => ', ierr)
    assert(ierr == 0)
    piDim1, piDim2, piDim3 = accum.get_bufferDimensions()
    print(' get_bufferDimensions: piDim1:', piDim1, ', piDim2:', piDim2,
          ', piDim3:', piDim3)
    assert(piDim1 == 0)
    assert(piDim2 == 0)
    assert(piDim3 == 0)

    # ae9ap9 -> compute flux mean
    ierr, da3FluxData = ae9ap9.computeFluxMean()
    print(' computeFluxMean: ierr=', ierr,
          ' da3Flux shape=', da3FluxData.shape)
    assert(ierr == 0)
    assert(da3FluxData.shape[0] == iNum)
    assert(da3FluxData.shape[1] == iNumE)
    assert(da3FluxData.shape[2] == iNumP)
    print('   [   0 ]: ', daTimes[0], daCoord1[0], daCoord2[0], daCoord3[0])
    for ii in range(iNumP):
        print(' {', daPitchAngles[ii], '}: ', da3FluxData[0][0][ii], da3FluxData[0]
              [1][ii], da3FluxData[0][2][ii], da3FluxData[0][iNumE-1][ii])
        print('   [', iNum-1, ']: ', daTimes[iNum-1],
              daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    for ii in range(iNumP):
        print(' {', daPitchAngles[ii], '}: ', da3FluxData[iNum-1][0][ii], da3FluxData[iNum-1]
              [1][ii], da3FluxData[iNum-1][2][ii], da3FluxData[iNum-1][iNumE-1][ii])

    # load results into buffer and verify size
    ierr = accum.loadBuffer(daTimes, da3FluxData)
    print(' loadBuffer: => ', ierr)
    assert(ierr == 0)
    piDim1, piDim2, piDim3 = accum.get_bufferDimensions()
    print(' get_bufferDimensions: piDim1:', piDim1, ', piDim2:', piDim2,
          ', piDim3:', piDim3)
    assert(piDim1 == iNum)
    assert(piDim2 == iNumE)
    assert(piDim3 == iNumP)

    # clear buffer and verify empty
    ierr = accum.clear_buffer()
    print(' clear_buffer: => ', ierr)
    assert(ierr == 0)
    piDim1, piDim2, piDim3 = accum.get_bufferDimensions()
    print(' get_bufferDimensions: piDim1:', piDim1, ', piDim2:', piDim2,
          ', piDim3:', piDim3)
    assert(piDim1 == 0)
    assert(piDim2 == 0)
    assert(piDim3 == 0)

    # computeFluence should error on empty buffer
    iNum2, daFluenceTimes2, da3Fluence2 = accum.computeFluence()
    print(' computeFluence: => ', ierr)
    assert(iNum2 == -1)
    assert(len(daFluenceTimes2) == 1)
    assert(da3Fluence2.shape == (1, 1, 1))

    # test with values added individually
    for i in range(15):
        ierr = accum.addToBuffer(daTimes[i], da3FluxData[i])
        print(' addToBuffer: => ', ierr)
        assert(ierr == 0)

        # verify size
        piDim1, piDim2, piDim3 = accum.get_bufferDimensions()
        print(' get_bufferDimensions: piDim1:', piDim1, ', piDim2:', piDim2,
              ', piDim3:', piDim3)
        assert(piDim1 == i+1)
        assert(piDim2 == iNumE)
        assert(piDim3 == iNumP)

    # compute fluence and verify size
    iNum2, daFluenceTimes2, da3Fluence2 = accum.computeFluence()
    print(' computeFluence: => ', ierr)
    assert(iNum2 == 15)
    assert(len(daFluenceTimes2) == 15)
    assert(da3Fluence2.shape == (15, iNumE, iNumP))


def test_computeFluences1(accum, ae9ap9, ephem):

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

    # verify that buffer is initially empty
    ierr = accum.clear_buffer()
    print(' clear_buffer: => ', ierr)
    assert(ierr == 0)
    piDim1, piDim2, piDim3 = accum.get_bufferDimensions()
    print(' get_bufferDimensions: piDim1:', piDim1, ', piDim2:', piDim2,
          ', piDim3:', piDim3)
    assert(piDim1 == 0)
    assert(piDim2 == 0)
    assert(piDim3 == 0)

    # ae9ap9 -> compute flux mean
    ierr, da3FluxData = ae9ap9.computeFluxMean()
    print(' computeFluxMean: ierr=', ierr,
          ' da3Flux shape=', da3FluxData.shape)
    assert(ierr == 0)
    assert(da3FluxData.shape[0] == iNum)
    assert(da3FluxData.shape[1] == iNumE)
    assert(da3FluxData.shape[2] == 1)
    print('   [   0 ]: ', daTimes[0], daCoord1[0], daCoord2[0], daCoord3[0])
    print('     ', da3FluxData[0][0][0], da3FluxData[0][1]
          [0], da3FluxData[0][2][0], da3FluxData[0][iNumE-1][0])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    print('     ', da3FluxData[iNum-1][0][0], da3FluxData[iNum-1][1]
          [0], da3FluxData[iNum-1][2][0], da3FluxData[iNum-1][iNumE-1][0])

    # load results into buffer and verify size
    ierr = accum.loadBuffer(daTimes, da3FluxData)
    print(' loadBuffer: => ', ierr)
    assert(ierr == 0)
    piDim1, piDim2, piDim3 = accum.get_bufferDimensions()
    print(' get_bufferDimensions: piDim1:', piDim1, ', piDim2:', piDim2,
          ', piDim3:', piDim3)
    assert(piDim1 == iNum)
    assert(piDim2 == iNumE)
    assert(piDim3 == 1)

    # reset all fluences
    ierr = accum.resetFluence()
    print(' resetFluence: => ', ierr)
    assert(ierr == 0)
    ierr = accum.resetIntvFluence()
    print(' resetIntvFluence: => ', ierr)
    assert(ierr == 0)
    ierr = accum.resetFullFluence()
    print(' resetFullFluence: => ', ierr)
    assert(ierr == 0)

    ierr = accum.set_intervalSec(45)  # too small for ephemeris timestep
    print(' set_intervalSec(45): => ', ierr)
    assert(ierr == 0)

    # compute fluence and verify size
    iNum1, daFluenceTimes1, da3Fluence1 = accum.computeFluence()
    print(' computeFluence: => ', ierr)
    assert(iNum1 == iNum)
    assert(len(daFluenceTimes1) == iNum)
    assert(da3Fluence1.shape == (iNum, iNumE, 1))

    # computeIntvFluence and verify size
    iNum2, daFluenceTimes2, da3Fluence2, iaIndices = accum.computeIntvFluence()
    print(' computeIntvFluence: => ', ierr)
    print(' iNum2: ', iNum2, ', len(daFluenceTimes2): ', len(daFluenceTimes2),
          ', da3Fluence2.shape: ', da3Fluence2.shape, ', len(iaIndices): ', len(iaIndices))
    assert(iNum2 == -1)

    ierr = accum.set_intervalSec(450)
    print(' set_intervalSec(450): => ', ierr)
    assert(ierr == 0)

    # computeIntvFluence and verify size
    iNum2, daFluenceTimes2, da3Fluence2, iaIndices = accum.computeIntvFluence()
    print(' computeIntvFluence: => ', ierr)
    print(' iNum2: ', iNum2, ', len(daFluenceTimes2): ', len(daFluenceTimes2),
          ', da3Fluence2.shape: ', da3Fluence2.shape, ', len(iaIndices): ', len(iaIndices))
    assert(iNum2 == 48)
    assert(len(daFluenceTimes2) == iNum2)
    assert(da3Fluence2.shape == (iNum2, iNumE, 1))
    assert(len(iaIndices) == 48)

    # get time difference in seconds
    timeDiff = abs((daFluenceTimes2[1] - daFluenceTimes2[0]) * 86400)
    print(
        ' time difference betwen daFluenceTimes2[0] and daFluenceTimes2[1]: ', timeDiff)
    # error window of 0.1 seconds
    assert(timeDiff - 450 < 0.1)

    iNum3, da3IntvFluenceAccum = accum.accumIntvFluence(
        daFluenceTimes2, da3Fluence2, 1)
    print(' accumIntvFluence: => iNum3=', iNum3,
          ', da3IntvFluenceAccum.shape=', da3IntvFluenceAccum.shape)
    assert(iNum3 == iNum2)
    assert(da3IntvFluenceAccum.shape == da3Fluence2.shape)

    # last accumulated values should match
    totalP = 0.0
    for i in range(iNumE):
        val1 = da3Fluence1[iNum1-1][i][0]
        val2 = da3IntvFluenceAccum[iNum3-1][i][0]
        percent = abs((val1 - val2) / val1) * 100.0
        #print(' val1: ', val1, ', val2: ', val2, ', percentage ', i, ': ', percent)
        # error window of 2%
        assert(percent < 2.0)
        totalP += percent

    avgP = totalP / iNumE
    print(' Average percentage difference: ', avgP)

    # all of the buffer was used so this should return 0
    iNum4, daFluenceTimes3, da3Fluence3, iaIndices2 = accum.computeIntvFluence(
        1)
    print(' computeIntvFluence(1): => ', ierr)
    print(' iNum4: ', iNum4, ', len(daFluenceTimes3): ', len(daFluenceTimes3),
          ', da3Fluence3.shape: ', da3Fluence3.shape, ', len(iaIndices2): ', len(iaIndices2))
    assert(iNum4 == 0)
    assert(len(daFluenceTimes3) == 1)
    assert(da3Fluence3.shape == (1, 1, 1))
    assert(len(iaIndices2) == 1)

    # reset
    ierr = accum.resetIntvFluence()
    print(' resetIntvFluence: => ', ierr)
    assert(ierr == 0)

    ierr = accum.set_intervalSec(250)
    print(' set_intervalSec: => ', ierr)
    assert(ierr == 0)

    iNum6, daFluenceTimes4, da3Fluence4, iaIndices3 = accum.computeIntvFluence()
    print(' computeIntvFluence: => ', iNum6)
    print(' len(daFluenceTimes4): ', len(daFluenceTimes4),
          ', da3Fluence4.shape: ', da3Fluence4.shape, ', len(iaIndices3): ', len(iaIndices3))
    assert(iNum6 == 86)
    assert(len(daFluenceTimes4) == iNum6)
    assert(da3Fluence4.shape == (iNum6, iNumE, 1))
    assert(len(iaIndices3) == 86)

    # get time difference in seconds
    timeDiff = abs((daFluenceTimes4[1] - daFluenceTimes4[0]) * 86400)
    print(
        ' time difference betwen daFluenceTimes4[0] and daFluenceTimes4[1]: ', timeDiff)
    # error window of 0.1 seconds
    assert(timeDiff - 250 < 0.1)

    # accum initial data, resetting to clear old data
    iNum8, da3IntvFluenceAccum2 = accum.accumIntvFluence(
        daFluenceTimes4, da3Fluence4, 1)
    print(' accumIntvFluence: => iNum8=', iNum8,
          ', da3IntvFluenceAccum2.shape=', da3IntvFluenceAccum2.shape)
    assert(iNum8 == iNum6)
    assert(da3IntvFluenceAccum2.shape == da3Fluence4.shape)

    # there is still data left on the buffer so this works
    iNum7, daFluenceTimes5, da3Fluence5, iaIndices4 = accum.computeIntvFluence(
        1)
    print(' computeIntvFluence(1): => ', iNum7)
    print(' len(daFluenceTimes5): ', len(daFluenceTimes5),
          ', da3Fluence5.shape: ', da3Fluence5.shape, ', len(iaIndices4): ', len(iaIndices4))
    assert(iNum7 == 1)
    assert(len(daFluenceTimes5) == 1)
    assert(da3Fluence5.shape == (1, iNumE, 1))
    assert(len(iaIndices4) == 1)

    # accum the remaining data
    iNum9, da3IntvFluenceAccum3 = accum.accumIntvFluence(
        daFluenceTimes5, da3Fluence5)
    print(' accumIntvFluence: => iNum9=', iNum9,
          ', da3IntvFluenceAccum3.shape=', da3IntvFluenceAccum3.shape)
    assert(iNum9 == iNum7)
    assert(da3IntvFluenceAccum3.shape == da3Fluence5.shape)

    # last accumulated values should match
    totalP = 0.0
    for i in range(iNumE):
        val1 = da3IntvFluenceAccum[iNum3-1][i][0]
        val2 = da3IntvFluenceAccum3[0][i][0]
        percent = abs((val1 - val2) / val1) * 100.0
        #print(' val1: ', val1, ', val2: ', val2, ', percentage ', i, ': ', percent)
        # error window of 2%
        assert(percent < 2.0)
        totalP += percent

    avgP = totalP / iNumE
    print(' Average percentage difference: ', avgP)


def test_computeFluences2(accum, ae9ap9, ephem):

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

    ierr, da3FluxData = ae9ap9.computeFluxMean()
    print(' computeFluxMean: ierr=', ierr,
          ' da3Flux shape=', da3FluxData.shape)
    assert(ierr == 0)
    assert(da3FluxData.shape[0] == iNum)
    assert(da3FluxData.shape[1] == iNumE)
    assert(da3FluxData.shape[2] == iNumP)
    print('   [   0 ]: ', daTimes[0], daCoord1[0], daCoord2[0], daCoord3[0])
    for ii in range(iNumP):
        print(' {', daPitchAngles[ii], '}: ', da3FluxData[0][0][ii], da3FluxData[0]
              [1][ii], da3FluxData[0][2][ii], da3FluxData[0][iNumE-1][ii])
        print('   [', iNum-1, ']: ', daTimes[iNum-1],
              daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    for ii in range(iNumP):
        print(' {', daPitchAngles[ii], '}: ', da3FluxData[iNum-1][0][ii], da3FluxData[iNum-1]
              [1][ii], da3FluxData[iNum-1][2][ii], da3FluxData[iNum-1][iNumE-1][ii])

    # verify that buffer is initially empty
    ierr = accum.clear_buffer()
    print(' clear_buffer: => ', ierr)
    assert(ierr == 0)
    piDim1, piDim2, piDim3 = accum.get_bufferDimensions()
    print(' get_bufferDimensions: piDim1:', piDim1, ', piDim2:', piDim2,
          ', piDim3:', piDim3)
    assert(piDim1 == 0)
    assert(piDim2 == 0)
    assert(piDim3 == 0)

    # add results to buffer and verify size
    ierr = accum.loadBuffer(daTimes, da3FluxData)
    print(' loadBuffer: => ', ierr)
    assert(ierr == 0)
    piDim1, piDim2, piDim3 = accum.get_bufferDimensions()
    print(' get_bufferDimensions: piDim1:', piDim1, ', piDim2:', piDim2,
          ', piDim3:', piDim3)
    assert(piDim1 == iNum)
    assert(piDim2 == iNumE)
    assert(piDim3 == iNumP)

    # reset all fluences
    ierr = accum.resetFluence()
    print(' resetFluence: => ', ierr)
    assert(ierr == 0)
    ierr = accum.resetIntvFluence()
    print(' resetIntvFluence: => ', ierr)
    assert(ierr == 0)
    ierr = accum.resetFullFluence()
    print(' resetFullFluence: => ', ierr)
    assert(ierr == 0)

    ierr = accum.set_intervalSec(450)
    print(' set_intervalSec: => ', ierr)
    assert(ierr == 0)

    # compute fluence and verify size
    iNum1, daFluenceTimes1, da3Fluence1 = accum.computeFluence()
    print(' computeFluence: => ', ierr)
    assert(iNum1 == iNum)
    assert(len(daFluenceTimes1) == iNum)
    assert(da3Fluence1.shape == (iNum, iNumE, iNumP))

    # computeIntvFluence and verify size
    iNum2, daFluenceTimes2, da3Fluence2, iaIndices = accum.computeIntvFluence()
    print(' computeIntvFluence: => ', ierr)
    print(' iNum2: ', iNum2, ', len(daFluenceTimes2): ', len(daFluenceTimes2),
          ', da3Fluence2.shape: ', da3Fluence2.shape, ', len(iaIndices): ', len(iaIndices))
    assert(iNum2 == 48)
    assert(len(daFluenceTimes2) == iNum2)
    assert(da3Fluence2.shape == (iNum2, iNumE, iNumP))
    assert(len(iaIndices) == 48)

    # get time difference in seconds
    timeDiff = abs((daFluenceTimes2[1] - daFluenceTimes2[0]) * 86400)
    print(
        ' time difference betwen daFluenceTimes2[0] and daFluenceTimes2[1]: ', timeDiff)
    # error window of < 0.1 sec
    assert(timeDiff - 450 < 0.1)

    iNum3, da3IntvFluenceAccum = accum.accumIntvFluence(
        daFluenceTimes2, da3Fluence2, 1)
    print(' accumIntvFluence: => iNum3=', iNum3,
          ', da3IntvFluenceAccum.shape=', da3IntvFluenceAccum.shape)
    assert(iNum3 == iNum2)
    assert(da3IntvFluenceAccum.shape == da3Fluence2.shape)

    # last accumulated values should match
    totalP = 0.0
    for i in range(iNumE):
        for j in range(iNumP):
            val1 = da3Fluence1[iNum1-1][i][j]
            val2 = da3IntvFluenceAccum[iNum3-1][i][j]
            percent = abs((val1 - val2) / val1) * 100.0
            #print(' val1: ', val1, ', val2: ', val2, ', percentage ', i, ': ', percent)
            # error window of 2%
            assert(percent < 2.0)
            totalP += percent

    avgP = totalP / (iNumE*iNumP)
    print(' Average percentage difference: ', avgP)

    # all of the buffer was used so this should return 0
    iNum4, daFluenceTimes3, da3Fluence3, iaIndices2 = accum.computeIntvFluence(
        1)
    print(' computeIntvFluence(1): => ', ierr)
    print(' iNum4: ', iNum4, ', len(daFluenceTimes3): ', len(daFluenceTimes3),
          ', da3Fluence3.shape: ', da3Fluence3.shape, ', len(iaIndices2): ', len(iaIndices2))
    assert(iNum4 == 0)
    assert(len(daFluenceTimes3) == 1)
    assert(da3Fluence3.shape == (1, 1, 1))
    assert(len(iaIndices2) == 1)

    # reset
    ierr = accum.resetIntvFluence()
    print(' resetIntvFluence: => ', ierr)
    assert(ierr == 0)

    ## -- suspending this particular test
    ## ierr = accum.set_intervalSec(45)
    ## print(' set_intervalSec: => ', ierr)
    ## assert(ierr == 0)

    # errors because interval is too small
    ## iNum5, da3IntvFluenceAccum2 = accum.accumIntvFluence(
    ##     daFluenceTimes3, da3Fluence3)
    ## print(' accumIntvFluence: => iNum5=', iNum5,
    ##       ', da3IntvFluenceAccum2.shape=', da3IntvFluenceAccum2.shape)
    ## assert(iNum5 == -1)
    ## assert(da3IntvFluenceAccum2.shape == (1, 1, 1))

    # reset
    ## ierr = accum.resetIntvFluence()
    ## print(' resetIntvFluence: => ', ierr)
    ## assert(ierr == 0)

    ierr = accum.set_intervalSec(250)
    print(' set_intervalSec: => ', ierr)
    assert(ierr == 0)

    iNum6, daFluenceTimes4, da3Fluence4, iaIndices3 = accum.computeIntvFluence()
    print(' computeIntvFluence: => ', iNum6)
    print(' len(daFluenceTimes4): ', len(daFluenceTimes4),
          ', da3Fluence4.shape: ', da3Fluence4.shape, ', len(iaIndices3): ', len(iaIndices3))
    assert(iNum6 == 86)
    assert(len(daFluenceTimes4) == iNum6)
    assert(da3Fluence4.shape == (iNum6, iNumE, iNumP))
    assert(len(iaIndices3) == 86)

    # get time difference in seconds
    timeDiff = abs((daFluenceTimes4[1] - daFluenceTimes4[0]) * 86400)
    print(
        ' time difference betwen daFluenceTimes4[0] and daFluenceTimes4[1]: ', timeDiff)
    # error window of 0.1 sec
    assert(timeDiff - 250 < 0.1)

    # accum initial data, resetting to clear old data
    iNum8, da3IntvFluenceAccum2 = accum.accumIntvFluence(
        daFluenceTimes4, da3Fluence4, 1)
    print(' accumIntvFluence: => iNum8=', iNum8,
          ', da3IntvFluenceAccum2.shape=', da3IntvFluenceAccum2.shape)
    assert(iNum8 == iNum6)
    assert(da3IntvFluenceAccum2.shape == da3Fluence4.shape)

    # there is still data left on the buffer so this works
    iNum7, daFluenceTimes5, da3Fluence5, iaIndices4 = accum.computeIntvFluence(
        1)
    print(' computeIntvFluence(1): => ', iNum7)
    print(' len(daFluenceTimes5): ', len(daFluenceTimes5),
          ', da3Fluence5.shape: ', da3Fluence5.shape, ', len(iaIndices4): ', len(iaIndices4))
    assert(iNum7 == 1)
    assert(len(daFluenceTimes5) == 1)
    assert(da3Fluence5.shape == (1, iNumE, iNumP))
    assert(len(iaIndices4) == 1)

    # accum the remaining data
    iNum9, da3IntvFluenceAccum3 = accum.accumIntvFluence(
        daFluenceTimes5, da3Fluence5)
    print(' accumIntvFluence: => iNum9=', iNum9,
          ', da3IntvFluenceAccum3.shape=', da3IntvFluenceAccum3.shape)
    assert(iNum9 == iNum7)
    assert(da3IntvFluenceAccum3.shape == da3Fluence5.shape)

    # last accumulated values should match
    totalP = 0.0
    for i in range(iNumE):
        for j in range(iNumP):
            val1 = da3IntvFluenceAccum[iNum3-1][i][j]
            val2 = da3IntvFluenceAccum3[0][i][j]
            percent = abs((val1 - val2) / val1) * 100.0
            #print(' val1: ', val1, ', val2: ', val2, ', percentage ', i, ': ', percent)
            # error window of 2%
            assert(percent < 2.0)
            totalP += percent

    avgP = totalP / iNumE
    print(' Average percentage difference: ', avgP)


def test_computeFluences3(accum, ae9ap9, ephem):

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

    ierr, da3FluxData = ae9ap9.computeFluxMean()
    print(' computeFluxMean: ierr=', ierr,
          ' da3Flux shape=', da3FluxData.shape)
    assert(ierr == 0)
    assert(da3FluxData.shape[0] == iNum)
    assert(da3FluxData.shape[1] == iNumE)
    assert(da3FluxData.shape[2] == 1)
    print('   [   0 ]: ', daTimes[0], daCoord1[0], daCoord2[0], daCoord3[0])
    print('     ', da3FluxData[0][0][0], da3FluxData[0][1]
          [0], da3FluxData[0][2][0], da3FluxData[0][iNumE-1][0])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    print('     ', da3FluxData[iNum-1][0][0], da3FluxData[iNum-1][1]
          [0], da3FluxData[iNum-1][2][0], da3FluxData[iNum-1][iNumE-1][0])

    # verify that buffer is initially empty
    ierr = accum.clear_buffer()
    print(' clear_buffer: => ', ierr)
    assert(ierr == 0)
    piDim1, piDim2, piDim3 = accum.get_bufferDimensions()
    print(' get_bufferDimensions: piDim1:', piDim1, ', piDim2:', piDim2,
          ', piDim3:', piDim3)
    assert(piDim1 == 0)
    assert(piDim2 == 0)
    assert(piDim3 == 0)

    # reset all fluences
    ierr = accum.resetFluence()
    print(' resetFluence: => ', ierr)
    assert(ierr == 0)
    ierr = accum.resetIntvFluence()
    print(' resetIntvFluence: => ', ierr)
    assert(ierr == 0)
    ierr = accum.resetFullFluence()
    print(' resetFullFluence: => ', ierr)
    assert(ierr == 0)

    interval = 440
    ierr = accum.set_intervalSec(interval)
    print(' set_intervalSec(', interval, '): => ', ierr)
    assert(ierr == 0)

    # counter to reset on 5th bucket
    j = 1
    # first time since reset
    iTime = 0.0
    # number of buckets to be fed
    iSplits = math.ceil(iNum / 60)
    # number of times in last bucket
    iNumTimes = 0
    reset = 1
    for i in range(iSplits):
        # get the next slice of 60 values
        # this will still work for the last slice
        start = 60*i
        iTimes = daTimes[start:start+60]
        iData = da3FluxData[start:start+60]
        iNumTimes = len(iTimes)
        print(' new slice ', i, ' from ', start, ' to ', start+iNumTimes-1,
              ', length: ', iNumTimes, ', iData shape: ', iData.shape)

        if j == 5:
            # reset all fluences after 5th iteration
            ierr = accum.resetFluence()
            print(' resetFluence: => ', ierr)
            assert(ierr == 0)
            ierr = accum.resetIntvFluence()
            print(' resetIntvFluence: => ', ierr)
            assert(ierr == 0)
            ierr = accum.resetFullFluence()
            print(' resetFullFluence: => ', ierr)
            assert(ierr == 0)

            iTime = iTimes[0]

        ierr = accum.loadBuffer(iTimes, iData)
        print(' loadBuffer: => ', ierr)
        assert(ierr == 0)

        piDim1, piDim2, piDim3 = accum.get_bufferDimensions()
        print(' get_bufferDimensions: piDim1:', piDim1, ', piDim2:', piDim2,
              ', piDim3:', piDim3)
        assert(piDim1 == iNumTimes)
        assert(piDim2 == iNumE)
        assert(piDim3 == 1)

        iNumC, daFluenceTimes, da3Fluence = accum.computeFluence()
        print(' computeFluence: => ', iNumC, ', daFluenceTimes shape: ', daFluenceTimes.shape,
              ', da3Fluence shape: ', da3Fluence.shape)
        assert(iNumC == piDim1)
        assert(daFluenceTimes.shape[0] == iNumC)
        assert(da3Fluence.shape == (piDim1, piDim2, piDim3))

        # should always return 0
        iNumF, daFluenceTimes, da3Fluence = accum.computeFullFluence()
        print(' computeFullFluence: => ', iNumF, ', daFluenceTimes shape: ', daFluenceTimes.shape,
              ', da3Fluence shape: ', da3Fluence.shape)
        assert(iNumF == 0)

        iNumI, daFluenceTimes, da3Fluence, iaIndices = accum.computeIntvFluence()
        print(' computeIntvFluence: => ', iNumI, ', daFluenceTimes shape: ', daFluenceTimes.shape,
              ', da3Fluence shape: ', da3Fluence.shape, ', iaIndices shape: ', iaIndices.shape)

        # Accum if > 0
        if iNumI > 0:
            iNumA, da3IntvFluenceAccum = accum.accumIntvFluence(
                daFluenceTimes, da3Fluence, reset)
            print(' accumIntvFluence: => iNumA=', iNumA,
                  ', da3IntvFluenceAccum.shape=', da3IntvFluenceAccum.shape)
            assert(iNumA == iNumI)
            assert(da3IntvFluenceAccum.shape == da3Fluence.shape)

        j += 1
        reset = 0

    # there is remaining data so this works
    iNumI, daFluenceTimes, da3Fluence, iaIndices = accum.computeIntvFluence(1)
    print(' computeIntvFluence(1): => ', iNumI)
    print(' len(daFluenceTimes): ', len(daFluenceTimes),
          ', da3Fluence.shape: ', da3Fluence.shape, ', len(iaIndices): ', len(iaIndices))
    assert(iNumI == 1)
    assert(daFluenceTimes.shape[0] == 1)
    assert(da3Fluence.shape == (1, iNumE, 1))

    # length in seconds of last interval length
    intvLength = ((daTimes[len(daTimes)-1] - iTime) * 86400) % interval

    iLength = accum.get_lastLength()
    # error of < 1 sec
    assert(iLength - intvLength < 1)

    # (iSplits-5)*60 + iNumTimes
    # signaling end of data - should return values now
    iNumF, daFluenceTimes, da3Fluence = accum.computeFullFluence(1)
    print(' computeFullFluence(1): => ', iNumF, ', daFluenceTimes shape: ', daFluenceTimes.shape,
          ', da3Fluence shape: ', da3Fluence.shape,'; iNumE=',iNumE)
    assert(iNumF == 1)
    assert(daFluenceTimes.shape[0] == 1)
    assert(da3Fluence.shape == (1, iNumE, 1))

    # time in seconds since reset
    resetTime = (daTimes[len(daTimes)-1] - iTime) * 86400

    ## - not valid for 'full' accum
    ## iLength = accum.get_lastLength()
    ## print('iLength=',iLength,', resetTime=',resetTime)
    # error of less than 1 second
    ## assert(abs(iLength - resetTime) < 1)

    # these two should have the same start time at the beginning of the data fed (after reset)
    startTime = accum.get_fluenceStartTime()
    print(' get_fluenceStartTime: => ', startTime)
    assert(startTime == iTime)

    startTime = accum.get_fullFluenceStartTime()
    print(' get_fullFluenceStartTime: => ', startTime)
    assert(startTime == iTime)

    # this corresponds to the start time of the last interval
    startTime = accum.get_IntvFluenceStartTime()
    print(' get_IntvFluenceStartTime: => ', startTime)
    assert(startTime == daTimes[len(daTimes)-1] - (intvLength/86400))


def test_computeFluences4(accum, ae9ap9, ephem):

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

    ierr, da3FluxData = ae9ap9.computeFluxMean()
    print(' computeFluxMean: ierr=', ierr,
          ' da3Flux shape=', da3FluxData.shape)
    assert(ierr == 0)
    assert(da3FluxData.shape[0] == iNum)
    assert(da3FluxData.shape[1] == iNumE)
    assert(da3FluxData.shape[2] == iNumP)
    print('   [   0 ]: ', daTimes[0], daCoord1[0], daCoord2[0], daCoord3[0])
    for ii in range(iNumP):
        print(' {', daPitchAngles[ii], '}: ', da3FluxData[0][0][ii], da3FluxData[0]
              [1][ii], da3FluxData[0][2][ii], da3FluxData[0][iNumE-1][ii])
        print('   [', iNum-1, ']: ', daTimes[iNum-1],
              daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    for ii in range(iNumP):
        print(' {', daPitchAngles[ii], '}: ', da3FluxData[iNum-1][0][ii], da3FluxData[iNum-1]
              [1][ii], da3FluxData[iNum-1][2][ii], da3FluxData[iNum-1][iNumE-1][ii])

    # verify that buffer is initially empty
    ierr = accum.clear_buffer()
    print(' clear_buffer: => ', ierr)
    assert(ierr == 0)
    piDim1, piDim2, piDim3 = accum.get_bufferDimensions()
    print(' get_bufferDimensions: piDim1:', piDim1, ', piDim2:', piDim2,
          ', piDim3:', piDim3)
    assert(piDim1 == 0)
    assert(piDim2 == 0)
    assert(piDim3 == 0)

    # reset all fluences
    ierr = accum.resetFluence()
    print(' resetFluence: => ', ierr)
    assert(ierr == 0)
    ierr = accum.resetIntvFluence()
    print(' resetIntvFluence: => ', ierr)
    assert(ierr == 0)
    ierr = accum.resetFullFluence()
    print(' resetFullFluence: => ', ierr)
    assert(ierr == 0)

    interval = 440
    ierr = accum.set_intervalSec(interval)
    print(' set_intervalSec(', interval, '): => ', ierr)
    assert(ierr == 0)

    # counter to reset on 5th bucket
    j = 1
    # first time since reset
    iTime = 0.0
    # number of buckets to be fed
    iSplits = math.ceil(iNum / 60)
    # number of times in last bucket
    iNumTimes = 0
    reset = 1
    for i in range(iSplits):
        # get the next slice of 60 values
        # this will still work for the last slice
        start = 60*i
        iTimes = daTimes[start:start+60]
        iData = da3FluxData[start:start+60]
        iNumTimes = len(iTimes)
        print(' new slice ', i, ' from ', start, ' to ', start+iNumTimes-1,
              ', length: ', iNumTimes, ', iData shape: ', iData.shape)

        if j == 5:
            # reset all fluences after 5th iteration
            ierr = accum.resetFluence()
            print(' resetFluence: => ', ierr)
            assert(ierr == 0)
            ierr = accum.resetIntvFluence()
            print(' resetIntvFluence: => ', ierr)
            assert(ierr == 0)
            ierr = accum.resetFullFluence()
            print(' resetFullFluence: => ', ierr)
            assert(ierr == 0)

            iTime = iTimes[0]

        ierr = accum.loadBuffer(iTimes, iData)
        print(' loadBuffer: => ', ierr)
        assert(ierr == 0)

        piDim1, piDim2, piDim3 = accum.get_bufferDimensions()
        print(' get_bufferDimensions: piDim1:', piDim1, ', piDim2:', piDim2,
              ', piDim3:', piDim3)
        assert(piDim1 == iNumTimes)
        assert(piDim2 == iNumE)
        assert(piDim3 == iNumP)

        iNumC, daFluenceTimes, da3Fluence = accum.computeFluence()
        print(' computeFluence: => ', iNumC, ', daFluenceTimes shape: ', daFluenceTimes.shape,
              ', da3Fluence shape: ', da3Fluence.shape)
        assert(iNumC == piDim1)
        assert(daFluenceTimes.shape[0] == iNumC)
        assert(da3Fluence.shape == (piDim1, piDim2, piDim3))

        # should always return 0
        iNumF, daFluenceTimes, da3Fluence = accum.computeFullFluence()
        print(' computeFullFluence: => ', iNumF, ', daFluenceTimes shape: ', daFluenceTimes.shape,
              ', da3Fluence shape: ', da3Fluence.shape)
        assert(iNumF == 0)

        iNumI, daFluenceTimes, da3Fluence, iaIndices = accum.computeIntvFluence()
        print(' computeIntvFluence: => ', iNumI, ', daFluenceTimes shape: ', daFluenceTimes.shape,
              ', da3Fluence shape: ', da3Fluence.shape, ', iaIndices shape: ', iaIndices.shape)

        # Accum if > 0
        if iNumI > 0:
            iNumA, da3IntvFluenceAccum = accum.accumIntvFluence(
                daFluenceTimes, da3Fluence, reset)
            print(' accumIntvFluence: => iNumA=', iNumA,
                  ', da3IntvFluenceAccum.shape=', da3IntvFluenceAccum.shape)
            assert(iNumA == iNumI)
            assert(da3IntvFluenceAccum.shape == da3Fluence.shape)

        j += 1
        reset = 0

    # there is remaining data so this works
    iNumI, daFluenceTimes, da3Fluence, iaIndices = accum.computeIntvFluence(1)
    print(' computeIntvFluence(1): => ', iNumI)
    print(' len(daFluenceTimes): ', len(daFluenceTimes),
          ', da3Fluence.shape: ', da3Fluence.shape, ', len(iaIndices): ', len(iaIndices))
    assert(iNumI == 1)
    assert(daFluenceTimes.shape[0] == 1)
    assert(da3Fluence.shape == (1, iNumE, iNumP))

    # length in seconds of last interval length
    intvLength = ((daTimes[len(daTimes)-1] - iTime) * 86400) % interval

    iLength = accum.get_lastLength()
    # error of < .1 sec
    assert(iLength - intvLength < .1)

    # (iSplits-5)*60 + iNumTimes
    # signaling end of data - should return values now
    iNumF, daFluenceTimes, da3Fluence = accum.computeFullFluence(1)
    print(' computeFullFluence(1): => ', iNumF, ', daFluenceTimes shape: ', daFluenceTimes.shape,
          ', da3Fluence shape: ', da3Fluence.shape,'; iNumE=',iNumE,', iNumP=',iNumP)
    assert(iNumF == 1)
    assert(daFluenceTimes.shape[0] == 1)
    assert(da3Fluence.shape == (1, iNumE, iNumP))

    # time in seconds since reset
    resetTime = (daTimes[len(daTimes)-1] - iTime) * 86400

    ## - not valid for 'full' accum
    ## iLength = accum.get_lastLength()
    ## print('iLength=',iLength,', resetTime=',resetTime)
    # error of < .1 sec
    ## assert(abs(iLength - resetTime) < 0.1)

    # these two should have the same start time at the beginning of the data fed (after reset)
    startTime = accum.get_fluenceStartTime()
    print(' get_fluenceStartTime: => ', startTime)
    assert(startTime == iTime)

    startTime = accum.get_fullFluenceStartTime()
    print(' get_fullFluenceStartTime: => ', startTime)
    assert(startTime == iTime)

    # this corresponds to the start time of the last interval
    startTime = accum.get_IntvFluenceStartTime()
    print(' get_IntvFluenceStartTime: => ', startTime)
    assert(startTime == daTimes[len(daTimes)-1] - (intvLength/86400))


def test_advancedFluxes1(accum, ae9ap9, ephem):

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

    ierr, da3FluxData = ae9ap9.computeFluxMean()
    print(' computeFluxMean: ierr=', ierr,
          ' da3Flux shape=', da3FluxData.shape)
    assert(ierr == 0)
    assert(da3FluxData.shape[0] == iNum)
    assert(da3FluxData.shape[1] == iNumE)
    assert(da3FluxData.shape[2] == 1)
    print('   [   0 ]: ', daTimes[0], daCoord1[0], daCoord2[0], daCoord3[0])
    print('     ', da3FluxData[0][0][0], da3FluxData[0][1]
          [0], da3FluxData[0][2][0], da3FluxData[0][iNumE-1][0])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    print('     ', da3FluxData[iNum-1][0][0], da3FluxData[iNum-1][1]
          [0], da3FluxData[iNum-1][2][0], da3FluxData[iNum-1][iNumE-1][0])

    # verify that buffer is initially empty
    ierr = accum.clear_buffer()
    print(' clear_buffer: => ', ierr)
    assert(ierr == 0)
    piDim1, piDim2, piDim3 = accum.get_bufferDimensions()
    print(' get_bufferDimensions: piDim1:', piDim1, ', piDim2:', piDim2,
          ', piDim3:', piDim3)
    assert(piDim1 == 0)
    assert(piDim2 == 0)
    assert(piDim3 == 0)

    # reset all fluences
    ierr = accum.resetFluence()
    print(' resetFluence: => ', ierr)
    assert(ierr == 0)
    ierr = accum.resetIntvFluence()
    print(' resetIntvFluence: => ', ierr)
    assert(ierr == 0)
    ierr = accum.resetFullFluence()
    print(' resetFullFluence: => ', ierr)
    assert(ierr == 0)

    # valid interval value
    interval = 300
    ierr = accum.set_intervalSec(interval)
    print(' set_intervalSec(', interval, '): => ', ierr)
    assert(ierr == 0)
    ierr = accum.get_interval()
    print(' get_interval(): => ', ierr)
    assert(ierr == interval)

    # increment at ephemeris timestep
    increment = 0
    ierr = accum.set_increment(increment)
    print(' set_increment(', increment, '): => ', ierr)
    assert(ierr == 0)
    ierr = accum.get_increment()
    print(' get_increment(): => ', ierr)
    assert(ierr == increment)

    # should throw error on interval of 0
    iNumB, daBoxcarTimes, da3BoxcarFluence, iaIndices = accum.computeBoxcarFluence()
    print(' computeBoxcarFluence(): => ', iNumB)
    print(' len(daBoxcarTimes): ', len(daBoxcarTimes),
          ', da3BoxcarFluence.shape: ', da3BoxcarFluence.shape, ', len(iaIndices): ', len(iaIndices))
    assert(iNumB < 0)
    assert(daBoxcarTimes.shape[0] == 1)
    assert(da3BoxcarFluence.shape == (1, 1, 1))
    assert(iaIndices.shape[0] == 1)

    iNumEx, daExponTimes, da3ExponFlux, iaIndices = accum.computeExponentialFlux()
    print(' computeBoxcarFluence(): => ', iNumEx)
    print(' len(daExponTimes): ', len(daExponTimes),
          ', da3ExponFlux.shape: ', da3ExponFlux.shape, ', len(iaIndices): ', len(iaIndices))
    assert(iNumEx < 0)
    assert(daExponTimes.shape[0] == 1)
    assert(da3ExponFlux.shape == (1, 1, 1))
    assert(iaIndices.shape[0] == 1)

    # valid interval value
    interval = 750
    ierr = accum.set_intervalSec(interval)
    print(' set_intervalSec(', interval, '): => ', ierr)
    assert(ierr == 0)
    ierr = accum.get_interval()
    print(' get_interval(): => ', ierr)
    assert(ierr == interval)

    # still errors for invalid increment
    iNumB, daBoxcarTimes, da3BoxcarFluence, iaIndices = accum.computeBoxcarFluence()
    print(' computeBoxcarFluence(): => ', iNumB)
    print(' len(daBoxcarTimes): ', len(daBoxcarTimes),
          ', da3BoxcarFluence.shape: ', da3BoxcarFluence.shape, ', len(iaIndices): ', len(iaIndices))
    assert(iNumB < 0)
    assert(daBoxcarTimes.shape[0] == 1)
    assert(da3BoxcarFluence.shape == (1, 1, 1))
    assert(iaIndices.shape[0] == 1)

    # valid increment
    increment = 300
    ierr = accum.set_increment(increment)
    print(' set_increment(300): => ', ierr)
    returnVal = accum.get_increment()
    print(' get_increment: => ', returnVal)
    assert(returnVal == increment)

    # error because buffer is empty
    iNumB, daBoxcarTimes, da3BoxcarFluence, iaIndices = accum.computeBoxcarFluence()
    print(' computeBoxcarFluence(): => ', iNumB)
    print(' len(daBoxcarTimes): ', len(daBoxcarTimes),
          ', da3BoxcarFluence.shape: ', da3BoxcarFluence.shape, ', len(iaIndices): ', len(iaIndices))
    assert(iNumB < 0)
    assert(daBoxcarTimes.shape[0] == 1)
    assert(da3BoxcarFluence.shape == (1, 1, 1))
    assert(iaIndices.shape[0] == 1)

    iNumEx, daExponTimes, da3ExponFlux, iaIndices = accum.computeExponentialFlux()
    print(' computeBoxcarFluence(): => ', iNumEx)
    print(' len(daExponTimes): ', len(daExponTimes),
          ', da3ExponFlux.shape: ', da3ExponFlux.shape, ', len(iaIndices): ', len(iaIndices))
    assert(iNumEx < 0)
    assert(daExponTimes.shape[0] == 1)
    assert(da3ExponFlux.shape == (1, 1, 1))
    assert(iaIndices.shape[0] == 1)

    da2max = np.zeros((iNumE, 1))

    # number of buckets to be fed
    iSplits = math.ceil(iNum / 60)
    for i in range(iSplits):
        # get the next slice of 60 values
        # this will still work for the last slice
        start = 60*i
        iTimes = daTimes[start:start+60]
        iData = da3FluxData[start:start+60]
        iNumTimes = len(iTimes)
        print(' new slice ', i, ' from ', start, ' to ', start+iNumTimes-1,
              ', length: ', iNumTimes, ', iData shape: ', iData.shape)

        ierr = accum.loadBuffer(iTimes, iData)
        print(' loadBuffer: => ', ierr)
        assert(ierr == 0)

        piDim1, piDim2, piDim3 = accum.get_bufferDimensions()
        print(' get_bufferDimensions: piDim1:', piDim1, ', piDim2:', piDim2,
              ', piDim3:', piDim3)
        assert(piDim1 == iNumTimes)
        assert(piDim2 == iNumE)
        assert(piDim3 == 1)

        iNumB, daBoxcarTimes, da3BoxcarFluence, iaIndices = accum.computeBoxcarFluence()
        print(' computeBoxcarFluence(): => ', iNumB)
        print(' len(daBoxcarTimes): ', len(daBoxcarTimes),
              ', da3BoxcarFluence.shape: ', da3BoxcarFluence.shape, ', len(iaIndices): ', len(iaIndices))
        assert(iNumB >= 0)
        if iNumB > 2:
            assert(daBoxcarTimes.shape[0] == iNumB)
            assert(da3BoxcarFluence.shape == (iNumB, iNumE, 1))
            assert(iaIndices.shape[0] == iNumB)
            timeDiff1 = (daBoxcarTimes[1] - daBoxcarTimes[0]) * 86400
            timeDiff2 = (daBoxcarTimes[2] - daBoxcarTimes[1]) * 86400
            print(
                ' time Difference daBoxcarTimes[1] - daBoxcarTimes[0]: ', timeDiff1)
            print(
                ' time Difference daBoxcarTimes[2] - daBoxcarTimes[1]: ', timeDiff2)
            # error window of < .1 sec
            assert(timeDiff1 - interval < 0.1)
            assert(timeDiff2 - interval < 0.1)
        else:
            assert(daBoxcarTimes.shape[0] == 1)
            assert(da3BoxcarFluence.shape == (1, 1, 1))
            assert(iaIndices.shape[0] == 1)

        iNumEx, daExponTimes, da3ExponFlux, iaIndices = accum.computeExponentialFlux()
        print(' computeExponentialFlux(): => ', iNumEx)
        print(' len(daExponTimes): ', len(daExponTimes),
              ', da3ExponFlux.shape: ', da3ExponFlux.shape, ', len(iaIndices): ', len(iaIndices),'; iNumE=',iNumE)
        assert(iNumEx >= 0)
        if iNumEx > 2:
            assert(daExponTimes.shape[0] == iNumEx)
            assert(da3ExponFlux.shape == (iNumEx, iNumE, 1))
            assert(iaIndices.shape[0] == iNumEx)

            timeDiff3 = (daExponTimes[1] - daExponTimes[0]) * 86400
            timeDiff4 = (daExponTimes[2] - daExponTimes[1]) * 86400
            print(
                ' time Difference daExponTimes[1] - daExponTimes[0]: ', timeDiff3)
            print(
                ' time Difference daExponTimes[2] - daExponTimes[1]: ', timeDiff4)
            # error window of < .1 sec
            assert(timeDiff3 - interval < 0.1)
            assert(timeDiff4 - interval < 0.1)

            # call applyWorstToDate
            prevVals = da2max
            ierr, da3DataWorst, da2MaxData = accum.applyWorstToDate(
                da3ExponFlux, da2max)
            print(' applyWorstToDate(): => ', ierr)
            print(' da3DataWorst.shape: ', da3DataWorst.shape,
                  ', da2MaxData.shape: ', da2MaxData.shape)
            assert(ierr > 0)
            assert(da3DataWorst.shape == da3ExponFlux.shape)
            assert(da2MaxData.shape == (iNumE, 1))

            # verify values are constant or increasing
            for ii in range(iNumEx - 1):
                for jj in range(iNumE):
                    assert(da3DataWorst[ii][jj][0] <=
                           da3DataWorst[ii+1][jj][0])

            # verify last value match max values
            assert(np.array_equal(da3DataWorst[iNumEx-1], da2MaxData))

            # i=3 is 4th iteration
            if i == 3:
                print(' --resetting worst to date--')

                # call applyWorstToDate with reset=1
                iNumW2D, da3DataWorst, da2MaxData = accum.applyWorstToDate(
                    da3ExponFlux, da2max, 1)
                print(' applyWorstToDate(1): => ', iNumW2D)
                print(' da3DataWorst.shape: ', da3DataWorst.shape,
                      ', da2MaxData.shape: ', da2MaxData.shape)
                assert(iNumW2D > 0)
                assert(da3DataWorst.shape == da3ExponFlux.shape)
                assert(da2MaxData.shape == (iNumE, 1))

                # verify values are constant or increasing
                for ii in range(iNumEx - 1):
                    for jj in range(iNumE):
                        assert(da3DataWorst[ii][jj][0] <=
                               da3DataWorst[ii+1][jj][0])

                # verify last value match max values
                assert(np.array_equal(da3DataWorst[iNumEx-1], da2MaxData))

                # verify values are less than or equal to last set of max values
                for ii in range(iNumE):
                    assert(da2MaxData[i][0] <= prevVals[i][0])

            da2max = da2MaxData
        else:
            iNe = 1
            if iNumEx == 1: iNe = iNumE
            assert(daExponTimes.shape[0] == 1)
            assert(da3ExponFlux.shape == (1, iNe, 1))
            assert(iaIndices.shape[0] == 1)

    # there is remaining data so this works
    iNumB, daBoxcarTimes, da3BoxcarFluence, iaIndices = accum.computeBoxcarFluence(
        1)
    print(' computeBoxcarFluence(1): => ', iNumB)
    print(' len(daBoxcarTimes): ', len(daBoxcarTimes),
          ', da3BoxcarFluence.shape: ', da3BoxcarFluence.shape, ', len(iaIndices): ', len(iaIndices))
    assert(iNumB == 1)
    assert(daBoxcarTimes.shape[0] == 1)
    assert(da3BoxcarFluence.shape == (1, iNumE, 1))
    assert(iaIndices.shape[0] == 1)

    # --there will be NO remaining data for ExponFlux
    iNumEx, daExponTimes, da3ExponFlux, iaIndices = accum.computeExponentialFlux(
        1)
    print(' computeExponentialFlux(1): => ', iNumEx)
    print(' len(daExponTimes): ', len(daExponTimes),
          ', da3ExponFlux.shape: ', da3ExponFlux.shape, ', len(iaIndices): ', len(iaIndices))
    assert(iNumEx == 0)
    assert(daExponTimes.shape[0] == 1)
    assert(da3ExponFlux.shape == (1, 1, 1))
    assert(iaIndices.shape[0] == 1)

##    iLength = accum.get_lastLength()
##    assert(iLength < interval)


def test_advancedFluxes2(accum, ae9ap9, ephem):

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

    ierr, da3FluxData = ae9ap9.computeFluxMean()
    print(' computeFluxMean: ierr=', ierr,
          ' da3Flux shape=', da3FluxData.shape)
    assert(ierr == 0)
    assert(da3FluxData.shape[0] == iNum)
    assert(da3FluxData.shape[1] == iNumE)
    assert(da3FluxData.shape[2] == iNumP)
    print('   [   0 ]: ', daTimes[0], daCoord1[0], daCoord2[0], daCoord3[0])
    for ii in range(iNumP):
        print(' {', daPitchAngles[ii], '}: ', da3FluxData[0][0][ii], da3FluxData[0]
              [1][ii], da3FluxData[0][2][ii], da3FluxData[0][iNumE-1][ii])
        print('   [', iNum-1, ']: ', daTimes[iNum-1],
              daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    for ii in range(iNumP):
        print(' {', daPitchAngles[ii], '}: ', da3FluxData[iNum-1][0][ii], da3FluxData[iNum-1]
              [1][ii], da3FluxData[iNum-1][2][ii], da3FluxData[iNum-1][iNumE-1][ii])

    # verify that buffer is initially empty
    ierr = accum.clear_buffer()
    print(' clear_buffer: => ', ierr)
    assert(ierr == 0)
    piDim1, piDim2, piDim3 = accum.get_bufferDimensions()
    print(' get_bufferDimensions: piDim1:', piDim1, ', piDim2:', piDim2,
          ', piDim3:', piDim3)
    assert(piDim1 == 0)
    assert(piDim2 == 0)
    assert(piDim3 == 0)

    # reset all fluences
    ierr = accum.resetFluence()
    print(' resetFluence: => ', ierr)
    assert(ierr == 0)
    ierr = accum.resetIntvFluence()
    print(' resetIntvFluence: => ', ierr)
    assert(ierr == 0)
    ierr = accum.resetFullFluence()
    print(' resetFullFluence: => ', ierr)
    assert(ierr == 0)
    ierr = accum.resetBoxcarFluence()
    print(' resetBoxcarFluence: => ', ierr)
    assert(ierr == 0)
    ierr = accum.resetExponentialFlux()
    print(' resetExponentialFlux: => ', ierr)
    assert(ierr == 0)

    # valid interval value
    interval = 300
    ierr = accum.set_intervalSec(interval)
    print(' set_intervalSec(', interval, '): => ', ierr)
    assert(ierr == 0)
    ierr = accum.get_interval()
    print(' get_interval(): => ', ierr)
    assert(ierr == interval)

    # increment at ephemeris timestep
    increment = 0
    ierr = accum.set_increment(increment)
    print(' set_increment(', increment, '): => ', ierr)
    assert(ierr == 0)
    ierr = accum.get_increment()
    print(' get_increment(): => ', ierr)
    assert(ierr == increment)

    # should throw error on interval of 0
    iNumB, daBoxcarTimes, da3BoxcarFluence, iaIndices = accum.computeBoxcarFluence()
    print(' computeBoxcarFluence(): => ', iNumB)
    print(' len(daBoxcarTimes): ', len(daBoxcarTimes),
          ', da3BoxcarFluence.shape: ', da3BoxcarFluence.shape, ', len(iaIndices): ', len(iaIndices))
    assert(iNumB < 0)
    assert(daBoxcarTimes.shape[0] == 1)
    assert(da3BoxcarFluence.shape == (1, 1, 1))
    assert(iaIndices.shape[0] == 1)

    iNumEx, daExponTimes, da3ExponFlux, iaIndices = accum.computeExponentialFlux()
    print(' computeBoxcarFluence(): => ', iNumEx)
    print(' len(daExponTimes): ', len(daExponTimes),
          ', da3ExponFlux.shape: ', da3ExponFlux.shape, ', len(iaIndices): ', len(iaIndices))
    assert(iNumEx < 0)
    assert(daExponTimes.shape[0] == 1)
    assert(da3ExponFlux.shape == (1, 1, 1))
    assert(iaIndices.shape[0] == 1)

    # valid interval value
    interval = 1000
    ierr = accum.set_intervalSec(interval)
    print(' set_intervalSec(', interval, '): => ', ierr)
    assert(ierr == 0)
    ierr = accum.get_interval()
    print(' get_interval(): => ', ierr)
    assert(ierr == interval)

    # still errors for clear buffer
    iNumB, daBoxcarTimes, da3BoxcarFluence, iaIndices = accum.computeBoxcarFluence()
    print(' computeBoxcarFluence(): => ', iNumB)
    print(' len(daBoxcarTimes): ', len(daBoxcarTimes),
          ', da3BoxcarFluence.shape: ', da3BoxcarFluence.shape, ', len(iaIndices): ', len(iaIndices))
    assert(iNumB < 0)
    assert(daBoxcarTimes.shape[0] == 1)
    assert(da3BoxcarFluence.shape == (1, 1, 1))
    assert(iaIndices.shape[0] == 1)

    # valid increment
    increment = 500
    ierr = accum.set_increment(increment)
    print(' set_increment(', increment, '): => ', ierr)
    returnVal = accum.get_increment()
    print(' get_increment: => ', returnVal)
    assert(returnVal == increment)

    # error because buffer is empty
    iNumB, daBoxcarTimes, da3BoxcarFluence, iaIndices = accum.computeBoxcarFluence()
    print(' computeBoxcarFluence(): => ', iNumB)
    print(' len(daBoxcarTimes): ', len(daBoxcarTimes),
          ', da3BoxcarFluence.shape: ', da3BoxcarFluence.shape, ', len(iaIndices): ', len(iaIndices))
    assert(iNumB < 0)
    assert(daBoxcarTimes.shape[0] == 1)
    assert(da3BoxcarFluence.shape == (1, 1, 1))
    assert(iaIndices.shape[0] == 1)

    iNumEx, daExponTimes, da3ExponFlux, iaIndices = accum.computeExponentialFlux()
    print(' computeExponentialFlux(): => ', iNumEx)
    print(' len(daExponTimes): ', len(daExponTimes),
          ', da3ExponFlux.shape: ', da3ExponFlux.shape, ', len(iaIndices): ', len(iaIndices))
    assert(iNumEx < 0)
    assert(daExponTimes.shape[0] == 1)
    assert(da3ExponFlux.shape == (1, 1, 1))
    assert(iaIndices.shape[0] == 1)

    da2max = np.zeros((iNumE, iNumP))

    # number of buckets to be fed
    iSplits = math.ceil(iNum / 60)
    for i in range(iSplits):
        # get the next slice of 60 values
        # this will still work for the last slice
        start = 60*i
        iTimes = daTimes[start:start+60]
        iData = da3FluxData[start:start+60]
        iNumTimes = len(iTimes)
        print(' new slice ', i, ' from ', start, ' to ', start+iNumTimes-1,
              ', length: ', iNumTimes, ', iData shape: ', iData.shape)

        ierr = accum.loadBuffer(iTimes, iData)
        print(' loadBuffer: => ', ierr)
        assert(ierr == 0)

        piDim1, piDim2, piDim3 = accum.get_bufferDimensions()
        print(' get_bufferDimensions: piDim1:', piDim1, ', piDim2:', piDim2,
              ', piDim3:', piDim3)
        assert(piDim1 == iNumTimes)
        assert(piDim2 == iNumE)
        assert(piDim3 == iNumP)

        iNumB, daBoxcarTimes, da3BoxcarFluence, iaIndices = accum.computeBoxcarFluence()
        print(' computeBoxcarFluence(): => ', iNumB)
        print(' len(daBoxcarTimes): ', len(daBoxcarTimes),
              ', da3BoxcarFluence.shape: ', da3BoxcarFluence.shape, ', len(iaIndices): ', len(iaIndices))
        assert(iNumB >= 0)
        if iNumB > 2:
            assert(daBoxcarTimes.shape[0] == iNumB)
            assert(da3BoxcarFluence.shape == (iNumB, iNumE, iNumP))
            assert(iaIndices.shape[0] == iNumB)
            timeDiff1 = (daBoxcarTimes[1] - daBoxcarTimes[0]) * 86400
            timeDiff2 = (daBoxcarTimes[2] - daBoxcarTimes[1]) * 86400
            print(
                ' time Difference daBoxcarTimes[1] - daBoxcarTimes[0]: ', timeDiff1)
            print(
                ' time Difference daBoxcarTimes[2] - daBoxcarTimes[1]: ', timeDiff2)
            # error window of < .1 sec
            assert(timeDiff1 - interval < 0.1)
            assert(timeDiff2 - interval < 0.1)
        else:
            assert(daBoxcarTimes.shape[0] == 1)
            assert(da3BoxcarFluence.shape == (1, 1, 1))
            assert(iaIndices.shape[0] == 1)

        iNumEx, daExponTimes, da3ExponFlux, iaIndices = accum.computeExponentialFlux()
        print(' computeExponentialFlux(): => ', iNumEx)
        print(' len(daExponTimes): ', len(daExponTimes),
              ', da3ExponFlux.shape: ', da3ExponFlux.shape, ', len(iaIndices): ', len(iaIndices))
        assert(iNumEx >= 0)
        if iNumEx > 2:
            assert(daExponTimes.shape[0] == iNumEx)
            assert(da3ExponFlux.shape == (iNumEx, iNumE, iNumP))
            assert(iaIndices.shape[0] == iNumEx)

            timeDiff3 = (daExponTimes[1] - daExponTimes[0]) * 86400
            timeDiff4 = (daExponTimes[2] - daExponTimes[1]) * 86400
            print(
                ' time Difference daExponTimes[1] - daExponTimes[0]: ', timeDiff3)
            print(
                ' time Difference daExponTimes[2] - daExponTimes[1]: ', timeDiff4)
            # error window of < .1 sec
            assert(timeDiff3 - interval < 0.1)
            assert(timeDiff4 - interval < 0.1)

            # call applyWorstToDate
            prevVals = da2max
            ierr, da3DataWorst, da2MaxData = accum.applyWorstToDate(
                da3ExponFlux, da2max)
            print(' applyWorstToDate(): => ', ierr)
            print(' da3DataWorst.shape: ', da3DataWorst.shape,
                  ', da2MaxData.shape: ', da2MaxData.shape)
            assert(ierr > 0)
            assert(da3DataWorst.shape == da3ExponFlux.shape)
            assert(da2MaxData.shape == (iNumE, iNumP))

            # verify values are constant or increasing
            for ii in range(iNumEx - 1):
                for jj in range(iNumE):
                    for kk in range(iNumP):
                        assert(da3DataWorst[ii][jj][kk] <=
                               da3DataWorst[ii+1][jj][kk])

            # verify last value match max values
            assert(np.array_equal(da3DataWorst[iNumEx-1], da2MaxData))

            # i=3 is 4th iteration
            if i == 3:
                print(' --resetting worst to date--')

                # call applyWorstToDate with reset=1
                iNumW2D, da3DataWorst, da2MaxData = accum.applyWorstToDate(
                    da3ExponFlux, da2max, 1)
                print(' applyWorstToDate(1): => ', iNumW2D)
                print(' da3DataWorst.shape: ', da3DataWorst.shape,
                      ', da2MaxData.shape: ', da2MaxData.shape)
                assert(iNumW2D > 0)
                assert(da3DataWorst.shape == da3ExponFlux.shape)
                assert(da2MaxData.shape == (iNumE, iNumP))

                # verify values are constant or increasing
                for ii in range(iNumEx - 1):
                    for jj in range(iNumE):
                        for kk in range(iNumP):
                            assert(da3DataWorst[ii][jj][kk]
                                   <= da3DataWorst[ii+1][jj][kk])

                # verify last value match max values
                assert(np.array_equal(da3DataWorst[iNumEx-1], da2MaxData))

                # verify values are less than or equal to last set of max values
                for ii in range(iNumE):
                    for jj in range(iNumP):
                        assert(da2MaxData[ii][jj] <= prevVals[ii][jj])

            da2max = da2MaxData
        else:
            iNe = 1
            iNp = 1
            if iNumEx == 1: 
              iNe = iNumE
              iNp = iNumP
            assert(daExponTimes.shape[0] == 1)
            assert(da3ExponFlux.shape == (1, iNe, iNp))
            assert(iaIndices.shape[0] == 1)

    print(' (before partial return) boxcar start time = ',
          accum.get_boxcarFluenceStartTime())
    # there is remaining data so this works
    iNumB, daBoxcarTimes, da3BoxcarFluence, iaIndices = accum.computeBoxcarFluence(
        1)
    print(' computeBoxcarFluence(1): => ', iNumB)
    print(' len(daBoxcarTimes): ', len(daBoxcarTimes),
          ', da3BoxcarFluence.shape: ', da3BoxcarFluence.shape, ', len(iaIndices): ', len(iaIndices))
    assert(iNumB == 1)
    assert(daBoxcarTimes.shape[0] == 1)
    assert(da3BoxcarFluence.shape == (1, iNumE, iNumP))
    assert(iaIndices.shape[0] == 1)
    print('(after) boxcar start time = ', accum.get_boxcarFluenceStartTime())

    # --there will be NO remaining data for ExponFlux
    iNumEx, daExponTimes, da3ExponFlux, iaIndices = accum.computeExponentialFlux(
        1)
    print(' computeExponentialFlux(1): => ', iNumEx)
    print(' len(daExponTimes): ', len(daExponTimes),
          ', da3ExponFlux.shape: ', da3ExponFlux.shape, ', len(iaIndices): ', len(iaIndices))
    assert(iNumEx == 0)
    assert(daExponTimes.shape[0] == 1)
    assert(da3ExponFlux.shape == (1, 1, 1))
    assert(iaIndices.shape[0] == 1)
##    print(' exponential flux start time = ',
##          accum.get_exponentialFluxStartTime())
##    dDelta = (daTimes[-1]-accum.get_exponentialFluxStartTime())*86400.0
##    print(' end time=', daTimes[-1], ' delta=', dDelta)

##    dLength = accum.get_lastLength()
##    assert(dLength < interval)
##    print(' last length value = ', dLength)
##    assert(abs(dLength-dDelta) < 0.1)
