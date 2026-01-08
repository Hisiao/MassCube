
# enable use of python3 print function form in python 2.x
from __future__ import print_function

# for the 'import irene_defs' to be successful, either:
#     specify the path to the Ae9Ap9 root directory here:
#sys.path.append(os.environ['HOME']+'/Ae9Ap9')
#     or in the PYTHONPATH environement variable
#
# load definitions for required IRENE environment variables
import irene_defs

from adiabatModel import AdiabatModel
from ephemModel import EphemModel
import numpy as np
import pytest


@pytest.fixture
def adiabat():
    adiabat = AdiabatModel()
    print('adiabat zHandle=', adiabat.zHandle)
    adiabat.set_kPhiDBFile("../../modelData/fastPhi_net.mat")
    adiabat.set_kHMinDBFile("../../modelData/fast_hmin_net.mat")
    adiabat.set_magfieldDBFile("../../modelData/igrfDB.h5")
    return adiabat


def test_set_kLimits_Uninitialized():

    adiabat = AdiabatModel()
    print('adiabat zHandle=', adiabat.zHandle)

    # tests validation of limits prior to db filenames specified
    testVals = [-1.0, 26.0, 5.0]
    for testVal in testVals:
        ierr = adiabat.set_kMin(testVal)
        print(' set_kMin: testVal=', testVal, ' => ', ierr)
        assert(ierr == 0)  # always returns 0

        ierr = adiabat.updateLimits()
        print(' updateLimits: => ', ierr)
        assert(ierr != 0)

        retVal = adiabat.get_kMin()
        print(' get_kMin: => ', retVal)

        #  add one db filename specification each loop
        if testVal < 0.0:
            strString = "../../modelData/fastPhi_net.mat"
            ierr = adiabat.set_kPhiDBFile(strString)
            print(' set_kPhiDBFile: strString=', strString, ' => ', ierr)
            assert(ierr == 0)
        if testVal > 25.0:
            strString = "../../modelData/fast_hmin_net.mat"
            ierr = adiabat.set_kHMinDBFile(strString)
            print(' set_kHMinDBFile: strString=', strString, ' => ', ierr)
            assert(ierr == 0)
        if testVal == 5.0:
            strString = "../../modelData/igrfDB.h5"
            ierr = adiabat.set_magfieldDBFile(strString)
            print(' set_magfieldDBFile: strString=', strString, ' => ', ierr)
            assert(ierr == 0)

    # now all db filenames specified...
    print(' --trying again, when all needed db filenames defined')
    ierr = adiabat.updateLimits()
    print(' updateLimits: => ', ierr)
    assert(ierr == 0)
    retVal = adiabat.get_kMin()
    print(' get_kMin: => ', retVal)

    testVal = 0.0
    ierr = adiabat.set_kMin(testVal)
    print(' set_kMin: testVal=', testVal, ' => ', ierr)
    assert(ierr == 0)
    testVal = 15.5
    ierr = adiabat.set_kMax(testVal)
    print(' set_kMax: testVal=', testVal, ' => ', ierr)
    assert(ierr == 0)

    ierr = adiabat.updateLimits()
    print(' updateLimits: => ', ierr)
    assert(ierr == 0)

    retVal = adiabat.get_kMin()
    print(' get_kMin: => ', retVal)
    assert(retVal == 0.0)
    retVal = adiabat.get_kMax()
    print(' get_kMax: => ', retVal)
    assert(retVal == testVal)


def test_set_modelDBDir(generic_ephem):

    adiabat = AdiabatModel()
    print('adiabat zHandle=', adiabat.zHandle)

    iNum, daTimes, daCoord1, daCoord2, daCoord3, strCoordSys, strCoordUnits \
        = generic_ephem

    for strString in ["goober", "../modelData", "$DATADIR/"]:
        ierr = adiabat.set_modelDBDir(strString)
        print(' set_modelDBDir: strString=', strString, ' => ', ierr)
        assert(ierr < 0)

        strRet = adiabat.get_modelDBDir()
        print(' get_modelDBDir: => ', strRet)
        assert(strRet == "")

    ierr, da2Alpha, da2Lm, da2K, da2Phi, da2Hmin, da2Lstar, daBmin, daBlocal, daMagLT, da2B, da2I \
        = adiabat.computeCoordinateSet(strCoordSys, strCoordUnits, daTimes,
                                       daCoord1, daCoord2, daCoord3, np.array([90.0]))
    print(' computeCoordinateSet without any DB defined: => ',ierr)
    assert(ierr < 0)  # passes - single pitch angle

    for strString in ["../../modelData/"]:
        ierr = adiabat.set_modelDBDir(strString)
        print(' set_modelDBDir: strString=', strString, ' => ', ierr)
        assert(ierr == 0)

        strRet = adiabat.get_modelDBDir()
        print(' get_modelDBDir: => ', strRet)
        assert(strRet == strString)

    print(' --model DB *prior* to compute call:')
    print(' get_kPhiDBFile: => ', adiabat.get_kPhiDBFile())
    print(' get_kHMinDBFile: => ', adiabat.get_kHMinDBFile())
    print(' get_magfieldDBFile: => ', adiabat.get_magfieldDBFile())
    ierr, da2Alpha, da2Lm, da2K, da2Phi, da2Hmin, da2Lstar, daBmin, daBlocal, daMagLT, da2B, da2I \
        = adiabat.computeCoordinateSet(strCoordSys, strCoordUnits, daTimes,
                                       daCoord1, daCoord2, daCoord3, np.array([90.0]))
    print(' computeCoordinateSet with ModelDBDir defined: => ', ierr)
    assert(ierr == 0)  # passes - single pitch angle
    assert(da2Alpha.shape == (iNum, 1))

    print('-- automatically defined via ModelDBDir:')
    print(' get_kPhiDBFile: => ', adiabat.get_kPhiDBFile())
    print(' get_kHMinDBFile: => ', adiabat.get_kHMinDBFile())
    print(' get_magfieldDBFile: => ', adiabat.get_magfieldDBFile())



def test_set_modelDBDir2(generic_ephem):

    adiabat = AdiabatModel()
    print('adiabat zHandle=', adiabat.zHandle)

    iNum, daTimes, daCoord1, daCoord2, daCoord3, strCoordSys, strCoordUnits \
        = generic_ephem

    for strString in ["goober", "../modelData", "$DATADIR/"]:
        ierr = adiabat.set_modelDBDir(strString)
        print(' set_modelDBDir: strString=', strString, ' => ', ierr)
        assert(ierr < 0)

        strRet = adiabat.get_modelDBDir()
        print(' get_modelDBDir: => ', strRet)
        assert(strRet == "")

    ierr, da2Alpha, da2Lm, da2K, da2Phi, da2Hmin, da2Lstar, daBmin, daBlocal, daMagLT, da2B, da2I \
        = adiabat.computeCoordinateSet(strCoordSys, strCoordUnits, daTimes,
                                       daCoord1, daCoord2, daCoord3, np.array([90.0]))
    print(' computeCoordinateSet without any DB defined: => ',ierr)
    assert(ierr < 0)  # passes - single pitch angle

    print(' defining valid directory, but incorrect')
    for strString in ["../../kernelXml/"]:
        ierr = adiabat.set_modelDBDir(strString)
        print(' set_modelDBDir: strString=', strString, ' => ', ierr)
        assert(ierr == 0)

        strRet = adiabat.get_modelDBDir()
        print(' get_modelDBDir: => ', strRet)
        assert(strRet == strString)

    ierr, da2Alpha, da2Lm, da2K, da2Phi, da2Hmin, da2Lstar, daBmin, daBlocal, daMagLT, da2B, da2I \
        = adiabat.computeCoordinateSet(strCoordSys, strCoordUnits, daTimes,
                                       daCoord1, daCoord2, daCoord3, np.array([90.0]))
    print(' computeCoordinateSet with valid but incorrect ModelDBDir defined: => ', ierr)
    assert(ierr < 0)  # passes - single pitch angle

    print('-- still not defined, due to incorrect ModelDBDir:')
    print(' get_kPhiDBFile: => ', adiabat.get_kPhiDBFile())
    print(' get_kHMinDBFile: => ', adiabat.get_kHMinDBFile())
    print(' get_magfieldDBFile: => ', adiabat.get_magfieldDBFile())

    print('  now doing explicit definition..')
    for strString in ["../../modelData/fastPhi_net.mat"]:
        ierr = adiabat.set_kPhiDBFile(strString)
        print(' set_kPhiDBFile: strString=', strString, ' => ', ierr)
        assert(ierr == 0)
    for strString in ["../../modelData/fast_hmin_net.mat"]:
        ierr = adiabat.set_kHMinDBFile(strString)
        print(' set_kHMinDBFile: strString=', strString, ' => ', ierr)
        assert(ierr == 0)
    for strString in ["../../modelData/igrfDB.h5"]:
        ierr = adiabat.set_magfieldDBFile(strString)
        print(' set_magfieldDBFile: strString=', strString, ' => ', ierr)
        assert(ierr == 0)
    print(' computeCoordinateSet with each DB defined, modelDBDir ignored: => ', ierr)
    assert(ierr == 0)  # passes - single pitch angle
    assert(da2Alpha.shape == (iNum, 1))
    print('-- manually defined, ignoring incorrect ModelDBDir:')
    print(' get_kPhiDBFile: => ', adiabat.get_kPhiDBFile())
    print(' get_kHMinDBFile: => ', adiabat.get_kHMinDBFile())
    print(' get_magfieldDBFile: => ', adiabat.get_magfieldDBFile())

def test_set_kPhiDBFile():

    adiabat = AdiabatModel()
    print('adiabat zHandle=', adiabat.zHandle)

    for strString in ["fastPhi_net.mat", "../modelData/fastPhi_net.mat", "$DATADIR/fastPhi_net.mat"]:
        ierr = adiabat.set_kPhiDBFile(strString)
        print(' set_kPhiDBFile: strString=', strString, ' => ', ierr)
        assert(ierr < 0)

        strRet = adiabat.get_kPhiDBFile()
        print(' get_kPhiDBFile: => ', strRet)
        assert(strRet == "UNDEF")

    for strString in ["../../modelData/fastPhi_net.mat"]:
        ierr = adiabat.set_kPhiDBFile(strString)
        print(' set_kPhiDBFile: strString=', strString, ' => ', ierr)
        assert(ierr == 0)

        strRet = adiabat.get_kPhiDBFile()
        print(' get_kPhiDBFile: => ', strRet)
        assert(strRet == strString)


def test_set_kHMinDBFile():

    adiabat = AdiabatModel()
    print('adiabat zHandle=', adiabat.zHandle)

    for strString in ["fast_hmin_net.mat", "../modelData/fast_hmin_net.mat", "$DATADIR/fast_hmin_net.mat"]:
        ierr = adiabat.set_kHMinDBFile(strString)
        print(' set_kHMinDBFile: strString=', strString, ' => ', ierr)
        assert(ierr < 0)

        strRet = adiabat.get_kHMinDBFile()
        print(' get_kHMinDBFile: => ', strRet)
        assert(strRet == "UNDEF")

    for strString in ["../../modelData/fast_hmin_net.mat"]:
        ierr = adiabat.set_kHMinDBFile(strString)
        print(' set_kHMinDBFile: strString=', strString, ' => ', ierr)
        assert(ierr == 0)

        strRet = adiabat.get_kHMinDBFile()
        print(' get_kHMinDBFile: => ', strRet)
        assert(strRet == strString)


def test_set_magfieldDBFile():

    adiabat = AdiabatModel()
    print('adiabat zHandle=', adiabat.zHandle)

    for strString in ["igrfDB.h5", "../modelData/igrfDB.h5", "$DATADIR/igrfDB.h5"]:
        ierr = adiabat.set_magfieldDBFile(strString)
        print(' set_magfieldDBFile: strString=', strString, ' => ', ierr)
        assert(ierr < 0)

        strRet = adiabat.get_magfieldDBFile()
        print(' get_magfieldDBFile: => ', strRet)
        assert(strRet == "UNDEF")

    for strString in ["../../modelData/igrfDB.h5"]:
        ierr = adiabat.set_magfieldDBFile(strString)
        print(' set_magfieldDBFile: strString=', strString, ' => ', ierr)
        assert(ierr == 0)

        strRet = adiabat.get_magfieldDBFile()
        print(' get_magfieldDBFile: => ', strRet)
        assert(strRet == strString)


def test_set_kMin(adiabat):
    testVals = [-1.0, 26.0, 5.0]
    for testVal in testVals:
        ierr = adiabat.set_kMin(testVal)
        print(' set_kMin: testVal=', testVal, ' => ', ierr)
        assert(ierr == 0)  # always returns 0

        ierr = adiabat.updateLimits()
        print(' updateLimits: => ', ierr)

        retVal = adiabat.get_kMin()
        print(' get_kMin: => ', retVal)

        if testVal == testVals[len(testVals) - 1]:  # only the last test passes
            assert(ierr == 0)
        else:
            assert(ierr < 0)

        # validation of limit values only occurs within updateLimits,
        #  due to database definition requirement of initialization
        #  of the underlying AdiabaticCoords class in the SpWx Library
        # [this aspect is tested in test_set_kLimits_Uninitialized]
        assert(retVal == testVal)


def test_set_kMax(adiabat):
    testVals = [50.0, 0.0, 20.0]
    for testVal in testVals:
        ierr = adiabat.set_kMax(testVal)
        print(' set_kMax: testVal=', testVal, ' => ', ierr)
        assert(ierr == 0)  # always returns 0

        ierr = adiabat.updateLimits()
        print(' updateLimits: => ', ierr)

        retVal = adiabat.get_kMax()
        print(' get_kMax: => ', retVal)

        if testVal == testVals[len(testVals) - 1]:  # only the last test passes
            assert(ierr == 0)
        else:
            assert(ierr < 0)

        # validation of limit values only occurs within updateLimits,
        #  due to database definition requirement of initialization
        #  of the underlying AdiabaticCoords class in the SpWx Library
        # [this aspect is tested in test_set_kLimits_Uninitialized]
        assert(retVal == testVal)


def test_set_hminMin(adiabat):
    testVals = [-2000.0, 50000.0, 20.0]
    for testVal in testVals:
        ierr = adiabat.set_hminMin(testVal)
        print(' set_hminMin: testVal=', testVal, ' => ', ierr)
        assert(ierr == 0)  # always returns 0

        ierr = adiabat.updateLimits()
        print(' updateLimits: => ', ierr)

        retVal = adiabat.get_hminMin()
        print(' get_hminMin: => ', retVal)

        if testVal == testVals[len(testVals) - 1]:  # only the last test passes
            assert(ierr == 0)
        else:
            assert(ierr < 0)

        # validation of limit values only occurs within updateLimits,
        #  due to database definition requirement of initialization
        #  of the underlying AdiabaticCoords class in the SpWx Library
        # [this aspect is tested in test_set_kLimits_Uninitialized]
        assert(retVal == testVal)


def test_set_hminMax(adiabat):
    testVals = [75000.0, -1000.0, 20000.0]
    for testVal in testVals:
        ierr = adiabat.set_hminMax(testVal)
        print(' set_hminMax: testVal=', testVal, ' => ', ierr)
        assert(ierr == 0)  # always returns 0

        ierr = adiabat.updateLimits()
        print(' updateLimits: => ', ierr)

        retVal = adiabat.get_hminMax()
        print(' get_hminMax: => ', retVal)

        if testVal == testVals[len(testVals) - 1]:  # only the last test passes
            assert(ierr == 0)
        else:
            assert(ierr < 0)

        # validation of limit values only occurs within updateLimits,
        #  due to database definition requirement of initialization
        #  of the underlying AdiabaticCoords class in the SpWx Library
        # [this aspect is tested in test_set_kLimits_Uninitialized]
        assert(retVal == testVal)


def test_set_phiMin(adiabat):
    testVals = [0, 2.5, 0.5]
    for testVal in testVals:
        ierr = adiabat.set_phiMin(testVal)
        print(' set_phiMin: testVal=', testVal, ' => ', ierr)
        assert(ierr == 0)  # always returns 0

        ierr = adiabat.updateLimits()
        print(' updateLimits: => ', ierr)

        retVal = adiabat.get_phiMin()
        print(' get_phiMin: => ', retVal)

        if testVal == testVals[len(testVals) - 1]:  # only the last test passes
            assert(ierr == 0)
        else:
            assert(ierr < 0)

        # validation of limit values only occurs within updateLimits,
        #  due to database definition requirement of initialization
        #  of the underlying AdiabaticCoords class in the SpWx Library
        # [this aspect is tested in test_set_kLimits_Uninitialized]
        assert(retVal == testVal)


def test_set_phiMax(adiabat):
    testVals = [3.0, 0.15, 2.25]
    for testVal in testVals:
        ierr = adiabat.set_phiMax(testVal)
        print(' set_phiMax: testVal=', testVal, ' => ', ierr)
        assert(ierr == 0)  # always returns 0

        ierr = adiabat.updateLimits()
        print(' updateLimits: => ', ierr)

        retVal = adiabat.get_phiMax()
        print(' get_phiMax: => ', retVal)

        if testVal == testVals[len(testVals) - 1]:  # only the last test passes
            assert(ierr == 0)
        else:
            assert(ierr < 0)

        # validation of limit values only occurs within updateLimits,
        #  due to database definition requirement of initialization
        #  of the underlying AdiabaticCoords class in the SpWx Library
        # [this aspect is tested in test_set_kLimits_Uninitialized]
        assert(retVal == testVal)


def test_set_magMomentTime(adiabat):
    for testVal in [33281.9, 69807.0, 190100000000.0, 205000100000.0]:
        ierr = adiabat.set_magMomentTime(testVal)
        print(' set_magMomentTime: testVal=', testVal, ' => ', ierr)
        assert(ierr < 0)

        # value is not set on fail - stays at default 0.0
        retVal = adiabat.get_magMomentTime()
        print(' get_magMomentTime: => ', retVal)
        assert(retVal == 0.0)

    for testVal in [0.0, 35000.0, 65000.0, 196000200000.0, 204936586399.99]:
        ierr = adiabat.set_magMomentTime(testVal)
        print(' set_magMomentTime: testVal=', testVal, ' => ', ierr)
        assert(ierr == 0)

        # value has been set
        retVal = adiabat.get_magMomentTime()
        print(' get_magMomentTime: => ', retVal)
        assert(retVal == testVal)


def test_computeCoordinateSet(adiabat, generic_ephem):

    iNum, daTimes, daCoord1, daCoord2, daCoord3, strCoordSys, strCoordUnits \
        = generic_ephem

    ierr, da2Alpha, da2Lm, da2K, da2Phi, da2Hmin, da2Lstar, daBmin, daBlocal, daMagLT, da2B, da2I \
        = adiabat.computeCoordinateSet("XYZ", strCoordUnits, daTimes,
                                       daCoord1, daCoord2, daCoord3, np.array([0.0]))
    assert(ierr < 0)  # bad coord system
    assert(da2Alpha.shape == (iNum, 1))

    daCoord12 = np.zeros(1)
    daCoord22 = np.zeros(2)
    ierr, da2Alpha, da2Lm, da2K, da2Phi, da2Hmin, da2Lstar, daBmin, daBlocal, daMagLT, da2B, da2I \
        = adiabat.computeCoordinateSet(strCoordSys, strCoordUnits, daTimes,
                                       daCoord12, daCoord22,
                                       daCoord3, np.array([90.0]))
    assert(ierr < 0)  # arrays of different lengths
    assert(da2Alpha.shape == (1, 1))

    ierr, da2Alpha, da2Lm, da2K, da2Phi, da2Hmin, da2Lstar, daBmin, daBlocal, daMagLT, da2B, da2I \
        = adiabat.computeCoordinateSet(strCoordSys, strCoordUnits, daTimes,
                                       daCoord1, daCoord2, daCoord3, np.array([90.0]))
    assert(ierr == 0)  # passes - single pitch angle
    assert(da2Alpha.shape == (iNum, 1))

    ierr, da2Alpha, da2Lm, da2K, da2Phi, da2Hmin, da2Lstar, daBmin, daBlocal, daMagLT, da2B, da2I\
        = adiabat.computeCoordinateSet(strCoordSys, strCoordUnits, daTimes,
                                       daCoord1, daCoord2, daCoord3, np.array([70.0, 80.0, 90.0]))
    assert(ierr == 0)  # passes - multiple pitch angles
    assert(da2Alpha.shape == (iNum, 3))

    ierr, da2Alpha, da2Lm, da2K, da2Phi, da2Hmin, da2Lstar, daBmin, daBlocal, daMagLT, da2B, da2I \
        = adiabat.computeCoordinateSet(strCoordSys, strCoordUnits, daTimes,
                                       daCoord1, daCoord2, daCoord3, np.array([70, 80, 90]))
    assert(ierr == 0)  # passes - should also work with integers
    assert(da2Alpha.shape == (iNum, 3))

def test_computeCoordinateSetVarPitch(adiabat, generic_ephem):

    iNum, daTimes, daCoord1, daCoord2, daCoord3, strCoordSys, strCoordUnits \
        = generic_ephem

    ierr, da2Alpha, da2Lm, da2K, da2Phi, da2Hmin, da2Lstar, daBmin, daBlocal, daMagLT, da2B, da2I \
       = adiabat.computeCoordinateSetVarPitch("XYZ", strCoordUnits, daTimes,
                                                       daCoord1, daCoord2, daCoord3,
                                                       np.zeros((2, 1)))
    assert(ierr < 0)  # bad coord system
    assert(da2Alpha.shape == (iNum, 1))

    daCoord12 = np.zeros(1)
    daCoord22 = np.zeros(2)
    ierr, da2Alpha, da2Lm, da2K, da2Phi, da2Hmin, da2Lstar, daBmin, daBlocal, daMagLT, da2B, da2I \
        = adiabat.computeCoordinateSetVarPitch(strCoordSys, strCoordUnits, daTimes,
                                               daCoord12, daCoord22,
                                               daCoord3, np.zeros((2, 1)))
    assert(ierr < 0)  # arrays of different lengths
    assert(da2Alpha.shape == (1, 1))

    ierr, da2Alpha, da2Lm, da2K, da2Phi, da2Hmin, da2Lstar, daBmin, daBlocal, daMagLT, da2B, da2I \
        = adiabat.computeCoordinateSetVarPitch(strCoordSys, strCoordUnits, daTimes,
                                               daCoord1, daCoord2, daCoord3, np.zeros((2, 1)))
    assert(ierr == 0)  # passes - single pitch angle
    assert(da2Alpha.shape == (iNum, 1))

    ierr, da2Alpha, da2Lm, da2K, da2Phi, da2Hmin, da2Lstar, daBmin, daBlocal, daMagLT, da2B, da2I \
        = adiabat.computeCoordinateSetVarPitch(strCoordSys, strCoordUnits, daTimes,
                                               daCoord1, daCoord2, daCoord3,
                                               np.full((iNum, 3), [70.0, 80.0, 90.0]))
    assert(ierr == 0)  # passes - multiple pitch angles
    assert(da2Alpha.shape == (iNum, 3))

    ierr, da2Alpha, da2Lm, da2K, da2Phi, da2Hmin, da2Lstar, daBmin, daBlocal, daMagLT, da2B, da2I \
        = adiabat.computeCoordinateSetVarPitch(strCoordSys, strCoordUnits, daTimes,
                                               daCoord1, daCoord2, daCoord3,
                                               np.full((iNum, 3), [70, 80, 90]))
    assert(ierr == 0)  # passes - should also work with integers
    assert(da2Alpha.shape == (iNum, 3))


def test_calcDirPitchAngles(adiabat):

    strCoordSys = 'GEI'
    strCoordUnits = 'km'
    iNum, daTimes, daCoord1, daCoord2, daCoord3 \
        = ephemeris_helper(strCoordSys, strCoordUnits)

    da2Dir1 = np.zeros((4, 1))
    da2Dir2 = np.zeros((4, 1))
    da2Dir3 = np.zeros((4, 1))
    ierr, da2PitchAngles = adiabat.calcDirPitchAngles("XYZ", strCoordUnits, daTimes,
                                                      daCoord1, daCoord2, daCoord3,
                                                      da2Dir1, da2Dir2, da2Dir3)
    assert(ierr < 0)  # bad coord system
    assert(da2PitchAngles.shape == (iNum, 1))

    daCoord12 = np.zeros(1)
    daCoord22 = np.zeros(2)
    ierr, da2PitchAngles = adiabat.calcDirPitchAngles(strCoordSys, strCoordUnits, daTimes,
                                                      daCoord12, daCoord22, daCoord3,
                                                      da2Dir1, da2Dir2, da2Dir3)
    assert(ierr < 0)  # arrays of different lengths
    assert(da2PitchAngles.shape == (1, 1))

    ierr, da2PitchAngles = adiabat.calcDirPitchAngles(strCoordSys, strCoordUnits, daTimes,
                                                      daCoord1, daCoord2, daCoord3,
                                                      da2Dir1, da2Dir2, da2Dir3)
    assert(ierr == 0)  # passes - single pitch angle
    assert(da2PitchAngles.shape == (iNum, 1))

    da2Dir1 = np.full((iNum, 4), np.array([1.0, 0.0, 0.0, -1.0]))
    da2Dir2 = np.full((iNum, 4), np.array([0.0, 1.0, 0.0, 0.0]))
    da2Dir3 = np.full((iNum, 4), np.array([0.0, 0.0, 1.0, 0.0]))
    ierr, da2PitchAngles = adiabat.calcDirPitchAngles(strCoordSys, strCoordUnits, daTimes,
                                                      daCoord1, daCoord2, daCoord3,
                                                      da2Dir1, da2Dir2, da2Dir3)
    assert(ierr == 0)  # passes - multiple pitch angles
    assert(da2PitchAngles.shape == (iNum, 4))

    da2Dir1 = np.full((iNum, 4), np.array([1, 0, 0, -1]))
    da2Dir2 = np.full((iNum, 4), np.array([0, 1, 0, 0]))
    da2Dir3 = np.full((iNum, 4), np.array([0, 0, 1, 0]))
    ierr, da2PitchAngles = adiabat.calcDirPitchAngles(strCoordSys, strCoordUnits, daTimes,
                                                      daCoord1, daCoord2, daCoord3,
                                                      da2Dir1, da2Dir2, da2Dir3)
    assert(ierr == 0)  # passes - should also work with integers
    assert(da2PitchAngles.shape == (iNum, 4))


def test_convertCoords(adiabat, ephem):

    # change chunk size, and verify
    iValue = 128
    ierr = ephem.set_chunkSize(iValue)
    print(' set_chunkSize: iValue=', iValue, ' => ', ierr)
    assert(ierr == 0)
    iSize = ephem.get_chunkSize()
    print(' get_chunkSize: returned iSize=', iSize)
    assert(iSize == iValue)

    # get first chunk of ephmeris in GEI
    iNum, daTimes, daXPos, daYPos, daZPos, daXVel, daYVel, daZVel \
        = ephem.computeGei()  # , iSize=0 )
    print(' computeGei: ')
    print('  returned iNum=', iNum, '; len(daTimes)=',
          len(daTimes), ', len(daXPos)=', len(daXPos))
    assert(iNum > 0)

    print('   [   0 ]: ', daTimes[0], daXPos[0], daYPos[0], daZPos[0])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daXPos[iNum-1], daYPos[iNum-1], daZPos[iNum-1])

    # get next chunk of ephemeris in GDZ
    strCoordSys = 'GDZ'
    strCoordUnits = 'km'
    iNum, daTimes, daCoord1, daCoord2, daCoord3 \
        = ephem.compute(strCoordSys, strCoordUnits)
    print(' compute: CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTimes)=',
          len(daTimes), ', len(daCoord1)=', len(daCoord1))
    assert(iNum > 0)

    # try coordinate conversion to imaginary coord sys
    strCoordSysIn = 'ABC'
    strCoordUnitsIn = 'in'
    ierr, daCoord1Out, daCoord2Out, daCoord3Out \
        = adiabat.convertCoords(strCoordSysIn, strCoordUnitsIn,
                                daTimes, daCoord1, daCoord2, daCoord3,
                                strCoordSys, strCoordUnits)
    print(' convertCoords: inCoords=', strCoordSysIn, strCoordUnitsIn,
          ' len(daTime)=', len(daTimes), ', len(daCoord1)=', len(daCoord1), ' outCoords=', strCoordSys, strCoordUnits)
    print('  returned ierr=', ierr, '; len(daTimes)=', len(
        daTimes), ', len(daCoord1Out)=', len(daCoord1Out))
    assert(ierr < 0)

    # now convert GEI to GDZ
    strCoordSysIn = 'GEI'
    strCoordUnitsIn = 'km'
    ierr, daCoord1Out, daCoord2Out, daCoord3Out \
        = adiabat.convertCoords(strCoordSysIn, strCoordUnitsIn,
                                daTimes, daXPos, daYPos, daZPos,
                                strCoordSys, strCoordUnits)
    print(' convertCoords: inCoords=', strCoordSysIn, strCoordUnitsIn,
          ' len(daTime)=', len(daTimes), ', len(daXPos)=', len(daXPos), ' outCoords=', strCoordSys, strCoordUnits)
    print('  returned ierr=', ierr, '; len(daTime)=', len(
        daTimes), ', len(daCoord1Out)=', len(daCoord1Out))
    assert(ierr == 0)

    iNum = len(daTimes)
    print('   [   0 ]: ', daTimes[0], daCoord1Out[0],
          daCoord2Out[0], daCoord3Out[0])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1Out[iNum-1], daCoord2Out[iNum-1], daCoord3Out[iNum-1])


def test_convertCoordsSingle(adiabat, ephem):
    # change chunk size, confirm
    iValue = 256
    ierr = ephem.set_chunkSize(iValue)
    print(' set_chunkSize: iValue=', iValue, ' => ', ierr)
    assert(ierr == 0)

    iSize = ephem.get_chunkSize()
    print(' get_chunkSize: returned iSize=', iSize)
    assert(iSize == 256)

    # get first chunk of ephemeris in GDZ
    strCoordSys = 'GDZ'
    strCoordUnits = 'km'
    iNum, daTimes, daCoord1, daCoord2, daCoord3 \
        = ephem.compute(strCoordSys, strCoordUnits)
    print(' compute: CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTime)=',
          len(daTimes), ', len(daCoord1)=', len(daCoord1))
    assert(iNum > 0)

    # restart ephemeris
    ierr = ephem.restart()
    print(' restart: => ', ierr)
    assert(ierr == 0)

    # save the time and position coordinate values for future reference
    dTime = daTimes[iNum-1]
    daPos = [daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1]]
    print(' saved values time=', dTime, ', pos=', daPos[0], daPos[1], daPos[2])

    # try coord conversion with invalid specification
    strCoordSysOut = 'Goo'
    strCoordUnitsOut = 'km'
    ierr, dCoord1, dCoord2, dCoord3  \
        = adiabat.convertCoordsSingle(strCoordSys, strCoordUnits,
                                      dTime, daPos[0], daPos[1], daPos[2], strCoordSysOut, strCoordUnitsOut)
    print('  convertCoordsSingle: inCoords=', strCoordSys, strCoordUnits)
    print('     time=', dTime, ', pos=',
          daPos[0], daPos[1], daPos[2], '; outCoords=', strCoordSysOut, strCoordUnitsOut)
    print('     returned ierr=', ierr, ', pos=', dCoord1, dCoord2, dCoord3)
    assert(ierr < 0)

    # try conversion GDZ to GDZ
    strCoordSysOut = 'GDZ'
    strCoordUnitsOut = 'km'
    ierr, dCoord1, dCoord2, dCoord3  \
        = adiabat.convertCoordsSingle(strCoordSys, strCoordUnits,
                                      dTime, daPos[0], daPos[1], daPos[2], strCoordSysOut, strCoordUnitsOut)
    print('  convertCoordsSingle: inCoords=', strCoordSys, strCoordUnits)
    print('     time=', dTime, ', pos=',
          daPos[0], daPos[1], daPos[2], '; outCoords=', strCoordSysOut, strCoordUnitsOut)
    print('     returned ierr=', ierr, ', pos=', dCoord1, dCoord2, dCoord3)
    assert(ierr < 0)

    # now finally do the single point coordinate conversion GDZ -> GEI
    strCoordSysOut = 'GEI'
    strCoordUnitsOut = 'km'
    ierr, dCoord1, dCoord2, dCoord3  \
        = adiabat.convertCoordsSingle(strCoordSys, strCoordUnits,
                                      dTime, daPos[0], daPos[1], daPos[2], strCoordSysOut, strCoordUnitsOut)
    print('  convertCoordsSingle: inCoords=', strCoordSys, strCoordUnits)
    print('     time=', dTime, ', pos=',
          daPos[0], daPos[1], daPos[2], '; outCoords=', strCoordSysOut, strCoordUnitsOut)
    print('     returned ierr=', ierr, ', pos=', dCoord1, dCoord2, dCoord3)
    assert(ierr == 0)

    # generate the first chunk of ephemeris in GEI,
    #   then confirm match to previous converted coords
    iNum, daTimes, daXPos, daYPos, daZPos, daXVel, daYVel, daZVel \
        = ephem.computeGei()  # , iSize=0 )
    print(' computeGei: ')
    print('  returned iNum=', iNum, '; len(daTimes)=',
          len(daTimes), ', len(daXPos)=', len(daXPos))
    assert(iNum > 0)

    print('   [   0 ]: ', daTimes[0], daXPos[0], daYPos[0], daZPos[0])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daXPos[iNum-1], daYPos[iNum-1], daZPos[iNum-1])
    assert (pytest.approx(daTimes[iNum-1], 0.0001) == dTime)
    assert (pytest.approx(daXPos[iNum-1], 0.0001) == dCoord1)
    assert (pytest.approx(daYPos[iNum-1], 0.0001) == dCoord2)
    assert (pytest.approx(daZPos[iNum-1], 0.0001) == dCoord3)


@pytest.fixture(scope="module")
def ephem():
    ephem = EphemModel()

    # define defaults for ephemeris
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
    print(' set_times: start=%.2f, end=%.2f, step=%.0f => %i' %
          (dStart, dEnd, dStep, ierr))
    assert(ierr == 0)

    return ephem


@pytest.fixture(scope="module")
def generic_ephem():

    strCoordSys = 'GDZ'
    strCoordUnits = 'km'

    iNum, daTimes, daCoord1, daCoord2, daCoord3 = ephemeris_helper(
        strCoordSys, strCoordUnits)

    genericEphem = dict()
    genericEphem['iNum'] = iNum
    genericEphem['daTimes'] = daTimes
    genericEphem['daCoord1'] = daCoord1
    genericEphem['daCoord2'] = daCoord2
    genericEphem['daCoord3'] = daCoord3
    genericEphem['strCoordSys'] = strCoordSys
    genericEphem['strCoordUnits'] = strCoordUnits

    return genericEphem['iNum'], genericEphem['daTimes'], \
        genericEphem['daCoord1'], genericEphem['daCoord2'], genericEphem['daCoord3'], \
        genericEphem['strCoordSys'], genericEphem['strCoordUnits']


def ephemeris_helper(strCoordSys, strCoordUnits):

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
    print(' set_times: start=%.2f, end=%.2f, step=%.0f => %i' %
          (dStart, dEnd, dStep, ierr))
    assert(ierr == 0)

    # get about two orbits of ephemeris
    iNum, daTimes, daCoord1, daCoord2, daCoord3 \
        = ephem.compute(strCoordSys, strCoordUnits)
    print(' compute: CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTimes)=',
          len(daTimes), ', len(daCoord1)=', len(daCoord1))
    assert(iNum > 0)
    del ephem

    return iNum, daTimes, daCoord1, daCoord2, daCoord3
